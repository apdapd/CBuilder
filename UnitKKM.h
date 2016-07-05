//---------------------------------------------------------------------------

#ifndef UnitKKMH
#define UnitKKMH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "KassKKM.h"
//---------------------------------------------------------------------------
class TFormKKM : public TForm
{
__published:	// IDE-managed Components
	TEdit *EdtComm;
	TButton *ButtonStart;
	TEdit *EdtBaudRate;
	TButton *ButtonZReport;
	TButton *ButtonXReport;
	void __fastcall ButtonStartClick(TObject *Sender);
	void __fastcall ButtonZReportClick(TObject *Sender);
	void __fastcall ButtonXReportClick(TObject *Sender);
private:	// User declarations
   CKassKKM *FKKM;
   void WriteErr ( const AnsiString &err );

public:		// User declarations
	__fastcall TFormKKM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormKKM *FormKKM;
//---------------------------------------------------------------------------
#endif
