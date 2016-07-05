//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "KassShtrihKKM.h"
#include "Sango.hpp"
#include "MainFunc.h"
#include "TextAlign.h"
#include <algorithm>

//---------------------------------------------------------------------------
#define MODULE_NUMBER 815

#pragma package(smart_init)

CKassShtrihKKM::CKassShtrihKKM ( TComponent *Owner, AnsiString CommName, DWORD BaudRate )
   	throw ( ECommError )
	: CKassKKM ( Owner )
{
	m_StringMaxLen = STRING_MAX_LEN;

	m_pKKM = new CShtrihFR ( CommName, BaudRate );
}

void CKassShtrihKKM::OpenCommPort()
{
   m_pKKM->GetCommPort()->OpenCommPort();
}

__fastcall CKassShtrihKKM::~CKassShtrihKKM ( void )
{
	delete m_pKKM;
}

/*������ ����*/
void CKassShtrihKKM::KKMPrint( CashGoods * ThisGoods )
{
	CShtrihFR::TICKET_TYPE type;
   if ( ThisGoods->Volume > 0 )
   	type = CShtrihFR::ticSale;
   else
   	type = CShtrihFR::ticReturnSale;

   int Vol = ThisGoods->Volume * 1000 / ThisGoods->Divide;

   bool bok;
   bok = m_pKKM->OpenTicket ( type );
   CShtrihFR::SUBREGIM_FR subregim;
   if ( bok )
		bok = WaitEndOfPrint ( subregim );

   if ( bok )
   {
   	if ( subregim == CShtrihFR::sregWait )
      {
      	if ( type == CShtrihFR::ticSale )
         	bok = m_pKKM->Sale ( Vol, ThisGoods->PriceGSM,
            	ConvertDepartNumber(ThisGoods->Depart), ThisGoods->NameGSM );
         else
         	bok = m_pKKM->ReturnSale ( -Vol, ThisGoods->PriceGSM,
            	ConvertDepartNumber(ThisGoods->Depart), ThisGoods->NameGSM );
      }
   }

   if ( bok )
		bok = WaitEndOfPrint ( subregim );

   if ( bok )
   {
   	if ( subregim == CShtrihFR::sregWait )
      {
      	__int64 Change;
      	if ( type == CShtrihFR::ticSale )
         {
		      if ( ThisGoods->Account == 0 )
		      	bok = m_pKKM->CloseTicket ( ThisGoods->MoneyCount, "", &Change );
   		   else
		      	bok = m_pKKM->CloseTicket ( 0, abs(ThisGoods->Pay), 0, 0, "", &Change );
         }
         else
		      if ( ThisGoods->Account == 0 )
		      	bok = m_pKKM->CloseTicket ( abs(ThisGoods->Pay), "", &Change );
   		   else
		      	bok = m_pKKM->CloseTicket ( 0, abs(ThisGoods->Pay), 0, 0, "", &Change );
      }
   }

   if ( bok )
		bok = WaitEndOfPrint ( subregim );

   if ( ! bok )
   {
   	TErrInfo err;
      FormErrInfo ( &err );
   	CShtrihFR::MAIN_STATE state;
   	if ( m_pKKM->GetCutState ( &state ) )
      {
      	if ( state.Regim.Regim == CShtrihFR::regOpenDoc )
         	if ( ! m_pKKM->CancelTicket() )
            {
            	err.ErrText = err.ErrText + "; ������ ������������� ���� " +
               	IntToStr ( m_pKKM->GetError() );
            }
      }
      throw err;
   }else
   {
   	m_pKKM->CutCheak ( false );
   }

   WORD CheakNumber = 0;
   bok = m_pKKM->GetCheackNumber ( &CheakNumber );
   if ( bok )
   {
      ThisGoods->uTicket = CheakNumber;
   }
}

/*������������ ������*/
bool CKassShtrihKKM::LptPrint ( TStrings *List )
{
   TStringList *NewList = new TStringList;
   NewList->AddStrings ( List );
   CKassPrinter::LptPrint ( NewList );

   AlignText ( NewList );

   bool bContinue = true;
	bool bok = true;
   int i = 0;

   //��� �� �������� ��������� ������ � ����-08��
   while ( bContinue && bok )
   {
   	if ( i < NewList->Count )
      {
	      bok = m_pKKM->PrintString ( true, false, NewList->Strings[i++] );
         if ( bok )
         {
         	CShtrihFR::SUBREGIM_FR SubRegim;
				bok = WaitEndOfPrint ( SubRegim );
            bContinue = ( SubRegim == CShtrihFR::sregWait );
         }
      }
      else
      	bContinue = false;
   }

   delete NewList;
   if ( ! bok )
   {
   	TErrInfo ErrInfo;
      FormErrInfo ( &ErrInfo );
      throw ErrInfo;
   }else
	{
   	m_pKKM->ListenTape ( true, false, 5 );
   	m_pKKM->CutCheak ( false );
   	return true;
   }
}

void CKassShtrihKKM::CopyTicket ( void )
{
	bool bok = m_pKKM->CopyDoc();
   if ( bok )
   {
	  	CShtrihFR::SUBREGIM_FR SubRegim;
		bok = WaitEndOfPrint ( SubRegim );
   }

   if ( ! bok )
   {
   	TErrInfo err;
      FormErrInfo ( &err );
      WriteProt ( err );
   }else
   	m_pKKM->CutCheak ( false );
}

bool CKassShtrihKKM::IsPrintWorking ( void )
{
	return m_pKKM->IsWorking();
}

bool CKassShtrihKKM::SearchKKM ( void )
{
//   DWORD br[] = {2400, 4800, 9600, 19200, 38400, 57600, 115200};
//   return EnumerateKKMSpeed ( br, 7 );
	CShtrihProtocol::DEVICE_TYPE DevType;
   DWORD BaudRate;
	return m_pKKM->FindDevice ( &DevType, &BaudRate ); 
}

bool CKassShtrihKKM::SearchVersionKKM ( DWORD BaudRate )
{
   m_pKKM->GetCommPort()->CloseCommPort();
   try
   {
   	m_pKKM->GetCommPort()->SetBaudRate ( BaudRate );
      m_pKKM->GetCommPort()->OpenCommPort ();
      return TestKKM();
   }
   catch (...)
   {
      return true;
   }
}

bool CKassShtrihKKM::TestKKM ( void )
{
	CShtrihFR::MAIN_STATE state;
   m_pKKM->GetCutState ( &state );
   if ( m_pKKM->GetError() < 0 )
      return false;
   else
      return true;
}

/*����������� ���������� � ������ �� � ���������� ������ � �����������*/
AnsiString CKassShtrihKKM::GetVersionStr ( void )
{
	CShtrihProtocol::DEVICE_TYPE dev;
   if ( m_pKKM->GetDeviceType ( &dev ) )
   {
   	AnsiString str;
      str.printf ( "%s. ������ %d.%d, ������ %d", dev.Name,
      	dev.Version, dev.DevSubType, dev.Model );
      return str;
   }else
		return "�����-��: " + m_pKKM->GetErrorStr();
}

/*������ ������*/
void CKassShtrihKKM::InitWork ( void )
{
//   m_pKKM->GetCutState ( &state );
	CShtrihFR::SUBREGIM_FR SubRegim;
	WaitEndOfPrint ( SubRegim );
   m_pKKM->ContinuePrint();
}

/*���������� z ������*/
void CKassShtrihKKM::ZReportWork ( void )
{
	bool bok = m_pKKM->ZReport();
//	bool bok = m_pKKM->ReportBySection();
	CShtrihFR::SUBREGIM_FR SubRegim;
   if ( bok )
   {
		bok = WaitEndOfPrint ( SubRegim );
      if ( bok )
      {
      	if ( SubRegim == CShtrihFR::sregWait )
		      AddStr ( "���������." );
         else
         	AddStr ( "������ ������" );
      }
   }

   if ( ! bok )
   {
   	TErrInfo ErrInfo;
      FormErrInfo ( &ErrInfo );
      AddStr ( ErrInfo.ErrText );
      ErrInfo.ErrText = "Z �����: " + ErrInfo.ErrText;
      WriteProt ( ErrInfo );
   }else
   {
   	m_pKKM->CutCheak ( false );
   }
}

/*���������� x ������*/
void CKassShtrihKKM::XReportWork ( void )
{
//   bool bok = m_pKKM->ReportBySection();
	bool bok = m_pKKM->XReport();
	CShtrihFR::SUBREGIM_FR SubRegim;
   if ( bok )
   {
		bok = WaitEndOfPrint ( SubRegim );
      if ( bok )
      {
      	if ( SubRegim == CShtrihFR::sregWait )
		      AddStr ( "���������." );
         else
         	AddStr ( "������ ������" );
      }
   }

   if ( ! bok )
   {
   	TErrInfo ErrInfo;
      FormErrInfo ( &ErrInfo );
      AddStr ( ErrInfo.ErrText );
      ErrInfo.ErrText = "Z �����: " + ErrInfo.ErrText;
      WriteProt ( ErrInfo );
   }else
   {
   	m_pKKM->CutCheak ( false );
   }
}

/*��������� ���������� �� ������*/
void CKassShtrihKKM::FormErrInfo ( TErrInfo *pErrInfo )
{
   pErrInfo->UUnum = MODULE_NUMBER;
   pErrInfo->Kind = HardError;
   pErrInfo->ErrCode = m_pKKM->GetError();
   pErrInfo->ErrText.sprintf (
      "������ ��� ������ � �����-�� � %d; ��������: %s",
      pErrInfo->ErrCode,
      m_pKKM->GetErrorStr() );
}

//�������� ��������� ������ �� ��������
bool CKassShtrihKKM::WaitEndOfPrint ( CShtrihFR::SUBREGIM_FR &SubRegim )
{
	do
   {
		CShtrihFR::MAIN_STATE state;
		if ( m_pKKM->GetCutState ( &state ) )
	   {
   		SubRegim = state.SubRegim;

		   if ( SubRegim == CShtrihFR::sregWaitPaperOut ||
   			SubRegim == CShtrihFR::sregPrintPaperOut )
		   {
   			SetPrinterState ( pfsEndOfPaper );
		   }else
   			SetPrinterState ( pfsOk );

         if ( SubRegim == CShtrihFR::sregWaitPrintContinue )
         {
         	m_pKKM->ContinuePrint();
         }

	   }else
      {
      	if ( m_pKKM->GetError() < 0 )
         {
		      SetPrinterState ( pfsError );
         }
      	return false;
      }
   }while ( SubRegim != CShtrihFR::sregWait && SubRegim != CShtrihFR::sregWaitPaperOut );
//   ( SubRegim == CShtrihFR::sregPrintFullFiskalReport
//   	|| SubRegim == CShtrihFR::sregPrint );

   return true;
}

//����������� ������ �����
void CKassShtrihKKM::AlignText ( TStrings *Text )
{
	TStringList *BufferText = new TStringList;
   BufferText->AddStrings ( Text );
   Text->Clear();

   CTextAlign::StringListAlign ( BufferText, Text, m_StringMaxLen );

   delete BufferText;
}

//������� ������������ ������ ������
void CKassShtrihKKM::SetTextWidth ( BYTE TextWidth )
{
	m_StringMaxLen = TextWidth;
}

/*�������������� ������ ������*/
BYTE CKassShtrihKKM::ConvertDepartNumber ( BYTE DepNumber )
{
	if ( DepNumber == 99 )
   	return 16;
   else
   	return ((DepNumber - 1) % 15) + 1;
}

