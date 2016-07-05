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
         Label1->Caption = "�������";
         break;
      case pfsError:
         Label1->Caption = "����";
         break;
      case pfsEndOfPaper:
         Label1->Caption = "��� ������";
         break;
   }
}

void TFormKassPrinterState::SetWorking ( bool bWorking )
{
   Label2->Caption = ( bWorking ? "�����" : "��������" );
}

void __fastcall TFormKassPrinterState::FormCreate(TObject *Sender)
{
   Label1->Caption = "�� ����������";
   Label2->Caption = "��������";
}
//---------------------------------------------------------------------------




