//---------------------------------------------------------------------------

#ifndef BarCodeH
#define BarCodeH
//---------------------------------------------------------------------------

class BarCode
{
public:

   enum {
      KOD_CITY_SIZE=8,     //������ ���� ������
      KLIENT_SIZE=7,       //������ ���� �������
      };

   char kod_city[KOD_CITY_SIZE];
   char klient[KLIENT_SIZE];

   /*�����������*/
   BarCode ( void );

   bool operator== ( const BarCode &code );

   //��������� �������� � 0
   void SetToNull ( void );

   AnsiString ToString() const;
};

#endif
