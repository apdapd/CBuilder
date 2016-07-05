//---------------------------------------------------------------------------

#ifndef FProtH
#define FProtH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormProt : public TForm
{
__published:	// IDE-managed Components
   TMemo *Memo;
   TButton *BtnClose;
   void __fastcall MemoKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
   bool bEnableClose;

   void SetEnableClose ( bool nEnableClose );
public:		// User declarations
   __fastcall TFormProt(TComponent* Owner);

   __property bool EnableClose = {read = bEnableClose, write = SetEnableClose};
};
//---------------------------------------------------------------------------
extern PACKAGE TFormProt *FormProt;
//---------------------------------------------------------------------------
#endif
