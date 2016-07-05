//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FProt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormProt *FormProt;
//---------------------------------------------------------------------------
__fastcall TFormProt::TFormProt(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormProt::MemoKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if ( bEnableClose )
      if ( Key == VK_RETURN )
         ModalResult = mrOk;
}
//---------------------------------------------------------------------------


void TFormProt::SetEnableClose ( bool nEnableClose )
{
   bEnableClose = nEnableClose;
   BtnClose->Enabled = bEnableClose;
}

