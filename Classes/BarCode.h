//---------------------------------------------------------------------------

#ifndef BarCodeH
#define BarCodeH
//---------------------------------------------------------------------------

class BarCode
{
public:

   enum {
      KOD_CITY_SIZE=8,     //размер кода города
      KLIENT_SIZE=7,       //размер кода клиента
      };

   char kod_city[KOD_CITY_SIZE];
   char klient[KLIENT_SIZE];

   /*конструктор*/
   BarCode ( void );

   bool operator== ( const BarCode &code );

   //установка значения в 0
   void SetToNull ( void );

   AnsiString ToString() const;
};

#endif
