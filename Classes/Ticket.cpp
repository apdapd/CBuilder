//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Ticket.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CTicket::CTicket ( void )
{
   List = new TStringList;
}

CTicket::CTicket ( TStringList *Shape )
{
   List = new TStringList;
   SetTicketShape ( Shape );
}

CTicket::~CTicket ( void )
{
   delete List;
}

void CTicket::FormTicket ( TStringList *Ticket )
{
   Ticket->Text = List->Text;
}

void CTicket::SetTicketShape ( TStringList *Shape )
{
   List->Text = Shape->Text;
}
