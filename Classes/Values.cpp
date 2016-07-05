//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Values.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CValues::CValues ( void )
{
   Names = new TStringList;
   Values = new TStringList;
}

CValues::~CValues ( void )
{
   Names->Free ( );
   Values->Free ( );
}

int CValues::Find ( AnsiString Name )
{
   for ( int i = 0; i < Names->Count; i++ )
      if ( Names->Strings[i] == Name )
         return i;
   return -1;
}

void CValues::SetValueInt ( int Index, AnsiString Value )
{
   if ( Names->Count > Index )
      Values->Strings[Index] = Value;
   else
   {
      throw Exception ( "Нет поля с таким индексом" );
   }
}

AnsiString CValues::GetValueInt ( int Index )
{
   if ( Names->Count > Index )
      return Values->Strings[Index];
   else
   {
      throw Exception ( "Нет поля с таким индексом" );
   }
}

void CValues::SetValue ( AnsiString Index, AnsiString Value )
{
   int Idx = Find ( Index );
   if ( Idx == -1 )
   {
      Names->Add ( Index );
      Values->Add ( Value );
   }
   else
      Values->Strings[Idx] = Value;
}

AnsiString CValues::GetValue ( AnsiString Index )
{
   int Idx = Find ( Index );
   if ( Idx != -1 )
   {
      return Values->Strings[Idx];
   }
   else
      throw Exception ( "Нет поля " + Index );
}

void CValues::Clear ( void )
{
   Names->Clear ( );
   Values->Clear ( );
}

void CValues::DeleteValue ( AnsiString Name )
{
   int Idx = Find ( Name );
   if ( Idx != -1 )
   {
      Names->Delete ( Idx );
      Values->Delete ( Idx );
   }
}

AnsiString CValues::GetNameValue ( int Index )
{
   if ( Index < Names->Count )
      return Names->Strings[Index];
   else
      throw Exception ( "Некорректный номер" );
}

int CValues::GetCountValues ( void )
{
   return Names->Count;
}

bool CValues::IsExistValue ( AnsiString Name )
{
   if ( Find ( Name ) != -1 )
      return true;
   else
      return false;
}

bool CValues::IsExistValue ( int Number )
{
   if ( Number < Names->Count )
      return true;
   else
      return false;
}

