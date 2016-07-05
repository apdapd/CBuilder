//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Command.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CAnswer::CAnswer ( void )
{
   Operands = 0;
   Answer = 0;
   Ok = false;
}

CAnswer::~CAnswer ( void )
{
   if ( Operands )
      delete []Operands;
   if ( Answer )
      delete []Answer;
}

void CAnswer::SetOperands ( char *source, int Count )
{
   if ( Operands )
      delete []Operands;
   Operands = new char[Count];
   memcpy ( Operands, source, Count );
   SizeOperands = Count;
}

void CAnswer::SetAnswer ( char *source, int Count )
{
   if ( Answer )
      delete []Answer;
   Answer = new char[Count];
   memcpy ( Answer, source, Count );
   SizeAnswer = Count;
}

AnsiString CAnswer::ErrorStr ( void )
{
   return CCommand::GetErrorStr ( Error );
}

AnsiString CAnswer::GetErrorStrDefault ( void )
{
   return "Ошибка (" + (Error > 0 ? IntToHex ( Error, 2 ) : IntToStr ( Error ) )
       + ") " + ErrorStr () +
      " при работе с " + DevName; 
}

//================================================================

CCommand::CCommand ( void )
{
   FEndAnswer = 0;
   Sender = 0;
   LastAnswer = 0;
}

CCommand::~CCommand ( void )
{
   FEndAnswer = 0;
   if ( LastAnswer )
      delete LastAnswer;
}

void CCommand::CommitAnswer ( CAnswer *Answer, const bool NextCmd )
{
   if ( Answer )
   {
      Answer->Sender = Sender;
      Answer->Last = NextCmd;
   }
   if ( EndAnswer )
      EndAnswer ( Answer, NextCmd );
   else
      LastAnswer = Answer;
}

AnsiString CCommand::GetErrorStr ( int Index )
{
   return "Неизвестная ошибка";
}

CAnswer *CCommand::DropAnswer ( void )
{
   CAnswer *buf = LastAnswer;
   LastAnswer = 0;
   return buf;
}

