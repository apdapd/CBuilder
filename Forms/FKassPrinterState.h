//---------------------------------------------------------------------------

#ifndef FKassPrinterStateH
#define FKassPrinterStateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

enum PRINTER_FORM_STATE { pfsOk, pfsError, pfsEndOfPaper };

class TFormKassPrinterState : public TForm
{
__published:	// IDE-managed Components
   TPanel *Panel1;
   TLabel *Label1;
   TLabel *Label2;
   void __fastcall FormCreate(TObject *Sender);
private:	// User declarations

public:		// User declarations
   __fastcall TFormKassPrinterState(TComponent* Owner);

   void SetPrinterState ( PRINTER_FORM_STATE ps );
   void SetWorking ( bool bWorking );

   __property bool Working = {write = SetWorking};
};
//---------------------------------------------------------------------------
extern PACKAGE TFormKassPrinterState *FormKassPrinterState;
//---------------------------------------------------------------------------
#endif
