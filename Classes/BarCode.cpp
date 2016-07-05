//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BarCode.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

BarCode::BarCode ( void )
{
/*   for(int i=0; i < 7; i++)
   {
      kod_city[i] = 0;
      klient[i]   = 0;
   }
   kod_city[7] = 0;*/
   SetToNull();
}

bool BarCode::operator== ( const BarCode &code )
{
   int equal = memcmp ( kod_city, code.kod_city, KOD_CITY_SIZE );
   //если коды городов одинаковы
   if ( equal == 0 )
   {
      equal = memcmp ( klient, code.klient, KLIENT_SIZE );
   }

   if ( equal == 0 )
      return true;
   else
      return false;
}

//установка значения в 0
void BarCode::SetToNull ( void )
{
   memset ( kod_city, 0, KOD_CITY_SIZE );
   memset ( klient, 0, KLIENT_SIZE );
}

AnsiString BarCode::ToString() const
{
   return AnsiString(kod_city,KOD_CITY_SIZE - 1) + AnsiString(klient, KLIENT_SIZE - 1);
}

