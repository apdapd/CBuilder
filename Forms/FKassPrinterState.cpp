//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FKassPrinterState.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKassPrinterState *FormKassPrinterState;
//---------------------------------------------------------------------------
__fastcall TFormKassPrinterState::TFormKassPrinterState(TComponent* Owner)
   : TForm(Owner)
{
   Width = 0;
   Height = 0;
}
//---------------------------------------------------------------------------

void TFormKassPrinterState::SetPrinterState ( PRINTER_FORM_STATE ps )
{
   switch ( ps )
   {
      case pfsOk:
         Label1->Caption = "Рабочий";
         break;
      case pfsError:
         Label1->Caption = "Сбой";
         break;
      case pfsEndOfPaper:
         Label1->Caption = "Нет бумаги";
         break;
   }
}

void TFormKassPrinterState::SetWorking ( bool bWorking )
{
   Label2->Caption = ( bWorking ? "Занят" : "Свободен" );
}

void __fastcall TFormKassPrinterState::FormCreate(TObject *Sender)
{
   Label1->Caption = "Не определено";
   Label2->Caption = "Свободен";
}
//---------------------------------------------------------------------------




