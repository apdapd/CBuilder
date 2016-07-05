//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "KassKKM.h"
#include "MainFunc.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

const int ModuleNumber = 803;

CKassKKM::CKassKKM ( TComponent *Owner )
{
   Form = new TFormKassPrinterState ( Owner );
   Form->Show ( );

   Timer = new TTimer ( 0 );
   Timer->OnTimer = OnTimer;
   Timer->Interval = 1000;
   Timer->Enabled = true;

   FormProt = new TFormProt ( Owner );
}

__fastcall CKassKKM::~CKassKKM ( void )
{
   Timer->Free ( );
   Form->Free ( );
   FormProt->Free ( );
}

void CKassKKM::PrepareClose ( void )
{
   Timer->Enabled = false;
}

void CKassKKM::SetPrinterState ( PRINTER_FORM_STATE st )
{
   PrinterFormState = st;
   Synchronize ( &SetPrinterStateSync );
}

void __fastcall CKassKKM::OnTimer ( TObject *Sender )
{
   Form->Working = IsPrintWorking ( );
}

void CKassKKM::WinKKM(int Mt,int Mb,int Ml,int Mr)
{
   MoveWindow ( Form->Handle, Ml, Mt, Mr - Ml, Mb - Mt, true );
}

int CKassKKM::Do_ZReport()
{
   if ( Suspended )
   {
      PrepareProtWindow ( );
      FormProt->Memo->Lines->Add ( "Инициализация Z отчёта..." );

      CurrentOperation = kkmZReport;
      Resume();

      FormProt->ShowModal ( );
   }else
   {
      TErrInfo ErrInfo;
      ErrInfo.UUnum = ModuleNumber;
      ErrInfo.ErrCode = -1;
      ErrInfo.Kind = OpError;
      ErrInfo.ErrText = "Запуск Z отчёта невозможен в связи с занятостью ККМ";
      WriteProt ( ErrInfo );
   }
   return 0;
}

int CKassKKM::Do_XReport()
{
   if ( Suspended )
   {
      PrepareProtWindow ( );
      FormProt->Memo->Lines->Add ( "Инициализация X отчёта..." );

      CurrentOperation = kkmXReport;
      Resume();

      FormProt->ShowModal ( );
   }else
   {
      TErrInfo ErrInfo;
      ErrInfo.UUnum = ModuleNumber;
      ErrInfo.ErrCode = -1;
      ErrInfo.Kind = OpError;
      ErrInfo.ErrText = "Запуск X отчёта невозможен в связи с занятостью ККМ";
      WriteProt ( ErrInfo );
   }
   return 0;
}

void CKassKKM::CopyTicket ( void )
{
}

void CKassKKM::KKMInit ( int CashType, int uModel, int uDefBank)
{
   if ( Suspended )
   {
      CurrentOperation = kkmInit;
      Resume();
   }else
   {
      TErrInfo ErrInfo;
      ErrInfo.UUnum = ModuleNumber;
      ErrInfo.ErrCode = -1;
      ErrInfo.Kind = OpError;
      ErrInfo.ErrText = "Открытие сессии невозможно в связи с занятостью ККМ";
      WriteProt ( ErrInfo );
   }
}

void CKassKKM::PrepareProtWindow ( )
{
//   Synchronize ( PrepareProtWindowSync );
   FormProt->Memo->Lines->Clear ( );
   FormProt->EnableClose = false;
}

void CKassKKM::AddStr ( AnsiString Str )
{
   StringAddProt = Str;
   Synchronize ( AddStrSync );
}

void CKassKKM::EndReport ( void )
{
   Synchronize ( EndReportSync );
}

bool CKassKKM::SearchKKM ( )
{
   return TestKKM();
}

bool CKassKKM::SearchVersionKKM ( DWORD BaudRate )
{
   return TestKKM();
}

bool CKassKKM::TestKKM ( void )
{
   return true;
}

void __fastcall CKassKKM::Execute ( void )
{
   while ( true )
   {
      switch ( CurrentOperation )
      {
         case kkmZReport:
            ZReportWork();
            EndReport();
            break;
         case kkmXReport:
            XReportWork();
            EndReport();
            break;
         case kkmInit:
            InitWork();
            break;
      }
      Suspend();
   }
}

/*тестирует ККМ на разных скоростях
in: Speeds - массив бит рэйтов
   Count - количество элементов в массиве*/
bool CKassKKM::EnumerateKKMSpeed ( DWORD Speeds[], int Count )
{
   for ( int i = 0; i < Count; i++ )
      if ( SearchVersionKKM ( Speeds[i] ) )
         return true;
   return false;
}

//==============  синхронизированные функции ======================

void __fastcall CKassKKM::SetPrinterStateSync ( void )
{
   Form->SetPrinterState ( PrinterFormState );
}

void __fastcall CKassKKM::PrepareProtWindowSync ( void )
{
}

void __fastcall CKassKKM::AddStrSync ( void )
{
   FormProt->Memo->Lines->Add ( StringAddProt );
}

void __fastcall CKassKKM::EndReportSync ( void )
{
   FormProt->EnableClose = true;
   FormProt->Memo->Lines->Add ( "Нажмите Enter" );
}


