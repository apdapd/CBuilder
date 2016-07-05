//---------------------------------------------------------------------------

#ifndef ShapeTicketH
#define ShapeTicketH

#include "Ticket.h"
#include "Values.h"
//---------------------------------------------------------------------------

class CShapeTicket : public CTicket
{
public:
   CShapeTicket ( void );
   CShapeTicket ( TStringList *List );

   void FormTicket ( TStrings *Ticket, CValues *Values );
protected:
   virtual bool TranslateStr (
      AnsiString String,
      CValues *Values,
      AnsiString &ResultStr );
private:
};

#endif
 