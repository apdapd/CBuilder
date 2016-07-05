//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TextAlign.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//подгонеям строки для печати под максимально допустимую длину
//SourceText - строки, которые необходимо выровнять
//AlignText - результирующий текст
//SourceText и AlignText - два разных объекта.
void CTextAlign::StringListAlign ( TStrings *SourceText, TStrings *AlignText, int Width )
{
	for ( int line = 0; line < SourceText->Count; ++line )
   {
   	StringAlign ( SourceText->Strings[line], AlignText, Width );
   }
}

//разбивает строку на несколько с учётом максимально допустимой ширины
//str - исходная строка
//AlignText - результирующий текст
//Width - максимальная ширина
void CTextAlign::StringAlign ( const AnsiString &str, TStrings *AlignText, int Width )
{
	if ( Width <= 0 )
   {
   	AlignText->Add ( str );
      return;
   }

	if ( str.Length() <= Width )
   {
   	AlignText->Add ( str );
      return;
   }

   AnsiString FirstPart = str.SubString ( 1, Width );
   AnsiString LastPart = str.SubString ( Width + 1, str.Length() - Width );

   int pos = FirstPart.LastDelimiter(" ");
   if ( pos == 0 )
   {
	   //если нет пробелов в строке
      AlignText->Add ( FirstPart );
      StringAlign ( LastPart, AlignText, Width );
   }else
   {
   	AlignText->Add ( FirstPart.SubString ( 1, pos ) );
      LastPart = FirstPart.SubString ( pos + 1, FirstPart.Length() - pos ) + LastPart;
		StringAlign ( LastPart, AlignText, Width );
   }

}
