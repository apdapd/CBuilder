//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitKKM.h"
#include "KassShtrihKKM.h"
#include "MainFunc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKKM *FormKKM;
//---------------------------------------------------------------------------
__fastcall TFormKKM::TFormKKM(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormKKM::ButtonStartClick(TObject *Sender)
{
 try
  {
   CKassShtrihKKM *ShtrishKKM;
   ShtrishKKM = new CKassShtrihKKM (this, EdtComm->Text, EdtBaudRate->Text.ToInt());
   // opt.Owner, opt.KKMComName, opt.PrintBaudRate );
   if ( FKKM != NULL )
	  delete FKKM;
   FKKM = ShtrishKKM;
   ShtrishKKM->SetTextWidth ( 28 );
   ShtrishKKM->OpenCommPort();
  }
 catch ( ECommError &E )
  {
				//DeleteAll();
			//throw Exception( "Не удалось открыть порт ККМ Штрих: " +
			//   AnsiString ( E.what() ) );
			WriteErr ( "Не удалось открыть порт ККМ Штрих: " + AnsiString ( E.what() ) );
  }
}
//---------------------------------------------------------------------------
void TFormKKM::WriteErr ( const AnsiString &err )
{
   TErrInfo errinfo;
   errinfo.UUnum = 111;  //MODULE_NUMBER;
   errinfo.ErrCode = -1;
   errinfo.Kind = HardError;
   errinfo.ErrText = err;
   WriteProt ( errinfo );
}
void __fastcall TFormKKM::ButtonZReportClick(TObject *Sender)
{
  FKKM->Do_ZReport();
}
//---------------------------------------------------------------------------
void __fastcall TFormKKM::ButtonXReportClick(TObject *Sender)
{
  FKKM->Do_XReport();
}
//---------------------------------------------------------------------------
