
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "KassPrim.h"
#include "MainFunc.h"
#include "math.h"
#include <vector>
#include "GlobalSettings.h"
//---------------------------------------------------------------------------

#define MODULE_NUMBER 810

#pragma package(smart_init)

CKassPrim::CKassPrim ( TComponent *Owner, AnsiString CommName, DWORD BaudRate )
   throw ( ECommError )
   : CKassKKM ( Owner )
{
   m_pPrim = new CPrim08TK ( CommName, BaudRate );
   if ( GlobalSettings::Inst()->PrimProtocol )
      m_pPrim->LoggingEnable ( GlobalSettings::Inst()->PrimProtocolFileName );
}

__fastcall CKassPrim::~CKassPrim ( void )
{
   delete m_pPrim;
}

/*печать чека*/
void CKassPrim::KKMPrint( CashGoods * ThisGoods )
{
   WORD CheakNumber;
   int Summ, DocSumm;
   int ExtraPay, Change;

//   m_pPrim->ZReport();
//   m_pPrim->SetPaymentInfo(0, "НАЛИЧНЫЕ:", false, true, 0, 7, 100);
//   m_pPrim->PaymentInfo(0);

   CPrim08TK::DOCUMENT_TYPE DocType;
   if ( ThisGoods->Volume > 0 )
      DocType = CPrim08TK::docSale;
   else
   {
      if ( ThisGoods->Account == 0 )
         DocType = CPrim08TK::docReturn;
      else
         DocType = CPrim08TK::docStornoSale;
   }

   bool bContinue = true;
   bool bok;

   //код на проверку открытого сеанса с прим-08тк
   while ( bContinue )
   {
	   bok = m_pPrim->StartDocument (
   	   DocType,
      	"Оператор",
	      0, false,
   	   AnsiString ( ThisGoods->Depart ),
      	"",
	      1,
   	   1,
      	&CheakNumber );

      if ( bok )
      	bContinue = false;
      else
      {
      	if ( m_pPrim->GetErrorCode() == 7 ) //Необходима команда начало сеанса
         {
         	bok = m_pPrim->StartShow();
            if ( ! bok )
            	bContinue = false;
         }else
         	bContinue = false;
      }
   }

//   bool OpenCheak;
   AnsiString Dimention;
   if ( ThisGoods->Depart == 99 )
      Dimention = "шт.";
   else
      Dimention = "л.";
   if ( bok )
   {
      ThisGoods->uTicket = CheakNumber;
      bok = m_pPrim->Sale (
         ThisGoods->NameGSM,
         AnsiString(ThisGoods->CodeGSM),
         ThisGoods->PriceGSM,
         (fabs((double) ThisGoods->Volume))/ThisGoods->Divide,
         Dimention,
         1,
         "",
         "",
         &Summ,
         &DocSumm );
   }

   if ( bok )
   {
      bok = m_pPrim->Result ( "", &DocSumm );
   }

   if ( bok )
   {
      int mc;
      CPrim08TK::PAY_TYPE PayType;
      if ( DocType == CPrim08TK::docSale )
         mc = ThisGoods->MoneyCount;
      else
         mc = abs(ThisGoods->Pay);

      if ( ThisGoods->Account == 0 )
         PayType = CPrim08TK::payCash;
      else
      {
         PayType = CPrim08TK::payCredit;
         mc = abs(ThisGoods->Pay);
      }
      bok = m_pPrim->Calculate (
         PayType,
         mc,
         "",
         "",
         &ExtraPay,
         &Change );
   }

   if ( bok )
   {
      bok = m_pPrim->CloseDocument ( "" );
   }

   if ( ! bok )
   {

      TErrInfo Err;
      FormErrInfo ( &Err );
      AnsiString errtxt = Err.ErrText;

      /*Попытка при не получении ответа запросить состояние,
      что бы предотвратить попытку аннулирования чека
      30.11.2005           Ногин*/
      if ( m_pPrim->GetErrorCode() == CPrimProtocol::errTimeOut )
      {
         m_pPrim->StartShow();
      }

      if ( m_pPrim->m_CurrentKKMState.DocState != CPrim08TK::docClose
         && m_pPrim->m_CurrentKKMState.DocState != CPrim08TK::docAny )
      {
         if ( ! m_pPrim->CancelDocument() )
         {
            FormErrInfo ( &Err );
            Err.ErrText = "Ошибка печати чека: " + errtxt + "; Ошибка анулирования чека: " + Err.ErrText;
         }else
         {
            Err.ErrText = "Чек аннулирован; ошибка пробития чека: " + Err.ErrText;
         }
      }
      SetState();
      throw Err;
   }
   SetState();
}

/*произвольная печать*/
bool CKassPrim::LptPrint ( TStrings *List )
{
   TStringList *NewList = new TStringList;
   NewList->AddStrings ( List );
   CKassPrinter::LptPrint ( NewList );

   bool bContinue = true;
	bool bok;

   //код на проверку открытого сеанса с прим-08тк
   while ( bContinue )
   {
      bok = m_pPrim->Print ( NewList );

      if ( bok )
      	bContinue = false;
      else
      {
      	if ( m_pPrim->GetErrorCode() == 7 ) //Необходима команда начало сеанса
         {
         	bok = m_pPrim->StartShow();
            if ( ! bok )
            	bContinue = false;
         }else
         	bContinue = false;
      }
   }

   delete NewList;
   if ( ! bok )
   {
   	TErrInfo ErrInfo;
      FormErrInfo ( &ErrInfo );
      SetState();
      throw ErrInfo;
   }else
	{
      SetState();
   	return true;
   }
}

void CKassPrim::CopyTicket ( void )
{
	int Num, Cheak, reserv;
	if ( m_pPrim->GetDocNumbers ( &Num, &Cheak, &reserv ) )
   {
   	if ( m_pPrim->PrintDocumentKPK ( Num ) )
      {
	      SetState();
   		return;
      }
   }

  	TErrInfo ErrInfo;
   FormErrInfo ( &ErrInfo );
   SetState();
   throw ErrInfo;
}

bool CKassPrim::IsPrintWorking ( void )
{
   return m_pPrim->IsWorking();
}

void CKassPrim::InitWork ( void )
{
   TErrInfo ErrInfo;
   if ( ! m_pPrim->StartShow() )
   {
      FormErrInfo ( &ErrInfo );
      ErrInfo.ErrText = "Начало сеанса: " + ErrInfo.ErrText;
      WriteProt ( ErrInfo );
   }
   SetState();

   if ( ! m_pPrim->OpenShift ( "" ) )
   {
      if ( m_pPrim->GetErrorCode() != 41 )
      {
         FormErrInfo ( &ErrInfo );
         ErrInfo.ErrText = "Открытие смены: " + ErrInfo.ErrText;
         WriteProt ( ErrInfo );
      }
   }
   SetState();
}

/*выполнение z отчёта*/
void CKassPrim::ZReportWork ( void )
{
   bool bContinue = true;
   bool bok;

   //код на проверку открытого сеанса с прим-08тк
   while ( bContinue )
   {
	   bok = m_pPrim->ZReport();

      if ( bok )
      	bContinue = false;
      else
      {
      	if ( m_pPrim->GetErrorCode() == 7 ) //Необходима команда начало сеанса
         {
         	bok = m_pPrim->StartShow();
            if ( ! bok )
            	bContinue = false;
         }else
         	bContinue = false;
      }
   }

   TErrInfo ErrInfo;
   if ( ! bok )
   {
      SetState();
      FormErrInfo ( &ErrInfo );
      AddStr ( ErrInfo.ErrText );
      ErrInfo.ErrText = "Z отчёт: " + ErrInfo.ErrText;
      WriteProt ( ErrInfo );
   }else
      AddStr ( "Выполнено." );

   SetState();
   AddStr ( "Открытие смены..." );
   AnsiString Str = "Включение " + DateTimeToStr ( Now() );
   if ( ! m_pPrim->OpenShift ( Str ) )
   {
      FormErrInfo ( &ErrInfo );
      AddStr ( ErrInfo.ErrText );
      ErrInfo.ErrText = "Z отчёт: " + ErrInfo.ErrText;
      WriteProt ( ErrInfo );
   }
   SetState();

	std::vector<AnsiString> params;
   if ( ! m_pPrim->GetReport ( params ) )
   {
      FormErrInfo ( &ErrInfo );
      AddStr ( ErrInfo.ErrText );
      ErrInfo.ErrText = "Z отчёт: " + ErrInfo.ErrText;
      WriteProt ( ErrInfo );
   }

   if ( ! params.empty() )
   {
	   AnsiString str = params[params.size() - 1];
      int summ;
      try
      {
      	summ = (int)(StrToFloat ( str ) * 100 + 0.5);
      }
      catch ( EConvertError &E )
      {
      	summ = 0;
      }
   	int After, Before;
      if ( summ > 0 )
      {
		   if ( ! m_pPrim->IncassSumm ( summ, &After, &Before ) )
   		{
      		FormErrInfo ( &ErrInfo );
	      	AddStr ( ErrInfo.ErrText );
	   	   ErrInfo.ErrText = "Z отчёт: " + ErrInfo.ErrText;
   	   	WriteProt ( ErrInfo );
	   	}
      }
   }

   AddStr ( "Выполнено" );
}

/*выполнение x отчёта*/
void CKassPrim::XReportWork ( void )
{
   bool bContinue = true;
   bool bok;

   //код на проверку открытого сеанса с прим-08тк
   while ( bContinue )
   {
	   bok = m_pPrim->XReport();

      if ( bok )
      	bContinue = false;
      else
      {
      	if ( m_pPrim->GetErrorCode() == 7 ) //Необходима команда начало сеанса
         {
         	bok = m_pPrim->StartShow();
            if ( ! bok )
            	bContinue = false;
         }else
         	bContinue = false;
      }
   }
   TErrInfo ErrInfo;
   if ( ! bok )
   {
      FormErrInfo ( &ErrInfo );
      AddStr ( ErrInfo.ErrText );
      ErrInfo.ErrText = "X отчёт: " + ErrInfo.ErrText;
      WriteProt ( ErrInfo );
   }else
      AddStr ( "Выполнено." );
   SetState();
}

/*формирует информацию об ошибке*/
void CKassPrim::FormErrInfo ( TErrInfo *pErrInfo )
{
   pErrInfo->UUnum = MODULE_NUMBER;
   pErrInfo->Kind = HardError;
   pErrInfo->ErrCode = m_pPrim->GetErrorCode();
   pErrInfo->ErrText.sprintf (
      "Ошибка при работе с ФР Прим-08ТК № %d; описание: %s",
      pErrInfo->ErrCode,
      m_pPrim->GetErrorStr() );
}

void CKassPrim::SetState ( void )
{
   if ( m_pPrim->GetErrorCode() < 0 )
      SetPrinterState ( pfsError );
   else if ( m_pPrim->GetErrorCode() == 25 )
      SetPrinterState ( pfsEndOfPaper );
   else
      SetPrinterState ( pfsOk );
}

bool CKassPrim::SearchKKM ( void )
{
   DWORD br[] = {4800, 9600, 19200};
/*   for ( int i = 0; i < 3; i++ )
      if ( SearchVersionKKM ( br[i] ) )
         return true;
   return false;*/
   return EnumerateKKMSpeed ( br, 3 );
}

bool CKassPrim::SearchVersionKKM ( DWORD BaudRate )
{
//   KKM->CommPort->CloseCommPort();
//   KKM->CommPort->SetBaudRate ( BaudRate );
   m_pPrim->CloseCommPort();
   try
   {
//      KKM->CommPort->OpenCommPort();
      m_pPrim->OpenCommPort ( BaudRate );
      return TestKKM();
   }
   catch (...)
   {
      return true;
   }
}

bool CKassPrim::TestKKM ( void )
{
/*   MStarTicketMaker->LoadConstv3();
   KKM->CurrentVersion = 3;
   if ( KKM->ExecGetVersion() )
   {
      KKM->CurrentVersion = (int)(KKM->GetResultValueAsDouble ( "Версия ПО" ));
      if ( KKM->CurrentVersion == 4 )
         MStarTicketMaker->LoadConstv4();
      return true;
   }
   return false;*/
   m_pPrim->StartShow();
   if ( m_pPrim->GetErrorCode() < 0 )
      return false;
   else
      return true;
}

/*запрашивает информацию о версии ПО и возвращает строку с информацией*/
AnsiString CKassPrim::GetVersionStr ( void )
{
	CPrim08TK::KKM_INFO inf;
   AnsiString res;
	if ( m_pPrim->GetKKMInfo ( inf ) )
   {
   	res.printf ( "Тип: %s; Версия: %s; Номер %s", inf.KKMType, inf.POVersion,
      	inf.SerialNumber );
   }else
   {
   	res = "Аппарат не фискализирован или ошибка в настройках";
   }
   return res;
}

