//---------------------------------------------------------------------------

#ifndef KassKKMH
#define KassKKMH
//---------------------------------------------------------------------------

#include "KassPrinter.h"
#include "FKassPrinterState.h"
#include "Sango.hpp"
#include "Command.h"
#include "FProt.h"
//---------------------------------------------------------------------------

enum KKM_OPERATION { kkmZReport, kkmXReport, kkmInit };

class CKassKKM : public CKassPrinter
{
public:
   CKassKKM ( TComponent *Owner );
   virtual __fastcall ~CKassKKM ( void );

   virtual void PrepareClose ( void );

   virtual void KKMPrint ( CashGoods * ThisGoods ) = 0;
   virtual void KKMInit ( int CashType, int uModel, int uDefBank);
   virtual void WinKKM(int Mt,int Mb,int Ml,int Mr);
   virtual int Do_ZReport();
   virtual int Do_XReport();

   virtual void CopyTicket ( void );

//   virtual void SetState ( CAnswer *Answer ) = 0;
   virtual bool SearchKKM ( );
   virtual bool SearchVersionKKM ( DWORD BaudRate );
   virtual bool TestKKM ( void );

   virtual void __fastcall Execute ( void );

   /*запрашивает информацию о версии ПО и возвращает строку с информацией*/
   virtual AnsiString GetVersionStr ( void ) = 0;

protected:

   TFormKassPrinterState *Form;
   TFormProt *FormProt;

   void SetPrinterState ( PRINTER_FORM_STATE st );
   void PrepareProtWindow ( );
   void AddStr ( AnsiString Str );
   void EndReport ( void );

   virtual void ZReportWork ( void ) = 0;
   virtual void XReportWork ( void ) = 0;
   virtual void InitWork ( void ) = 0;

   /*тестирует ККМ на разных скоростях
   in: Speeds - массив бит рэйтов
      Count - количество элементов в массиве*/
   bool EnumerateKKMSpeed ( DWORD Speeds[], int Count );

private:

   TTimer *Timer;

   void __fastcall OnTimer ( TObject *Sender );

   //текущая операция
   KKM_OPERATION CurrentOperation;

   //синхронизированные функции
   PRINTER_FORM_STATE PrinterFormState;
   void __fastcall SetPrinterStateSync ( void );
   void __fastcall PrepareProtWindowSync ( void );
   AnsiString StringAddProt;
   void __fastcall AddStrSync ( void );
   void __fastcall EndReportSync ( void );
};
#endif
