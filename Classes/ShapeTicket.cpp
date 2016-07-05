//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ShapeTicket.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CShapeTicket::CShapeTicket ( void )
{
}

CShapeTicket::CShapeTicket ( TStringList *List )
   : CTicket ( List )
{
}

void CShapeTicket::FormTicket ( TStrings *Ticket, CValues *Values )
{
try
{
   Ticket->Clear();
   AnsiString Str;
   for ( int i = 0; i < List->Count; i++ )
   {
      if ( ! TranslateStr ( List->Strings[i], Values, Str ) )
         throw Exception ( "Неверные данные в строке " + IntToStr ( i ) );
      Ticket->Add ( Str );
   }
}
catch ( Exception &E )
{
   throw Exception ( E.Message + "FM" );
}
}

bool CShapeTicket::TranslateStr (
   AnsiString String,
   CValues *Values,
   AnsiString &ResultStr )
{
   ResultStr = "";
   int i = 1;
   bool Perem = false;
   AnsiString PeremName;
   try
   {
      while ( i <= String.Length ( ) )
      {
         switch ( String[i] )
         {
            case '%':
               if ( ! Perem )
                  PeremName = "";
               else
                  ResultStr += Values->Value[PeremName];
               Perem = ! Perem;
               i++;
               break;
            default:
               if ( Perem )
                  PeremName += String[i++];
               else
                  ResultStr += String[i++];
               break;
         }
      }
   }
   catch ( Exception &E )
   {
      return false;
   }
   return true;
}
