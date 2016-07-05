//---------------------------------------------------------------------------

#ifndef TicketH
#define TicketH
//---------------------------------------------------------------------------

class CTicket
{
public:
   CTicket ( void );
   CTicket ( TStringList *Shape );
   virtual ~CTicket ( void );

   void FormTicket ( TStringList *Ticket );
   void SetTicketShape ( TStringList *Shape );
protected:
   TStringList *List;
private:
};

#endif
 