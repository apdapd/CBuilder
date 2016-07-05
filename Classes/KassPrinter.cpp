//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "KassPrinter.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

CKassPrinter::CKassPrinter ( void )
   : TThread ( true )
{
   Head = new TStringList;
   Foot = new TStringList;
}

__fastcall CKassPrinter::~CKassPrinter ( void )
{
   Terminate();

//   while ( ! Suspended )
//      Sleep ( 300 );

   delete Head;
   delete Foot;
}

bool CKassPrinter::LptPrint ( TStrings *List )
{
   for ( int i = 0; i < Head->Count; i++ )
      List->Insert ( i, Head->Strings[i] );
   for ( int i = 0; i< Foot->Count; i++ )
      List->Add ( Foot->Strings[i] );
   return true;
}

void CKassPrinter::SetHead ( TStrings *List )
{
   Head->Clear ( );
   Head->AddStrings ( List );
}

void CKassPrinter::SetHead ( AnsiString Str )
{
   Head->Clear ( );
//   Head->Add ( Str );
   BreakLines ( Str, Head );
}

void CKassPrinter::SetFoot ( TStrings *List )
{
   Foot->Clear ( );
   Foot->AddStrings ( List );
}

void CKassPrinter::SetFoot ( AnsiString Str )
{
   Foot->Clear ( );
//   Foot->Add ( Str );
   BreakLines ( Str, Foot );
}

void CKassPrinter::BreakLines ( const AnsiString &str, TStrings *List )
{
   AnsiString line = "";
   for ( int i = 1; i <= str.Length(); ++i )
   {
      if ( str[i] != '\n' )
         line = line + AnsiString ( str[i] );
      else if ( str[i] != '\r' )
      {
//         if ( line.Trim().IsEmpty() )
//            line = " ";
         List->Add ( line );
         line = "";
      }
   }

   if ( ! line.IsEmpty() )
      List->Add ( line );
}

