//---------------------------------------------------------------------------

#ifndef KassPrinterH
#define KassPrinterH

#include "Values.h"
#include "ShapeTicket.h"

class CKassPrinter : public TThread
{
public:
   CKassPrinter ( void );
   virtual __fastcall ~CKassPrinter ( void );

   void SetHead ( TStrings *List );
   void SetHead ( AnsiString Str );
   void SetFoot ( TStrings *List );
   void SetFoot ( AnsiString Str );

   virtual bool LptPrint ( TStrings *List );
   virtual bool IsPrintWorking ( void ) = 0;
protected:
   TStringList *Head, *Foot;
private:
//   bool bContinue;

   void BreakLines ( const AnsiString &str, TStrings *List );

};

#endif
