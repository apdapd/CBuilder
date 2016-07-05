//---------------------------------------------------------------------------

#ifndef ObjectManagerH
#define ObjectManagerH
//---------------------------------------------------------------------------

#include "ComPort.h"
#include "KassPrinter.h"
#include "KassKKM.h"
#include "DepType.h"
#include "FCardRead.h"
#include "KassTRK.h"
#include "Conveer.h"
#include "GoldCrown.h"
#include "SimpleCommPort.h"
#include "MifareCard.h"
#include "SimpleLevelGage.h"
#include "SecureKeyAZSFactory.h"
#include "TerminalServer.h"
#include "KassSMSSender.h"

class CObjectManager
{
public:
   CObjectManager ( OBJECT_OPTIONS &opt );
   ~CObjectManager ( void );

   virtual void PrepareClose ( void );

   __property CKassKKM *KKM = {read = FKKM};
   __property CKassPrinter *Printer = {read = FPrinter};
//   __property TComPort *ComPort = {read = FComPort};

   __property TFormCardRead *CardReader = {read = FCardReader};
   __property TFormCardRead *MagRead = {read = FMagRead};
   __property CKassTRK *TRK = {read = FTRK};
   __property CGoldCrown *GoldCrown = {read = FGoldCrown};
//   __property CSmartDebit *Smart = {read = FSmart};

   bool IsWorking ( void );

   bool MayPrintKKM ( int CountWorking );
   bool MayPrintPrinter ( int CountWorking );

   //получение карт Mifare
   CMifareCard* GetMifareCard ( void )
   {
      return m_MifareCard;
   };

   CSimpleLevelGage *m_pStruna;
   boost::shared_ptr<TerminalServer> m_TerminalServer;
   boost::shared_ptr<KassSMSSender> m_SMSSender;

protected:
private:

   enum {MODULE_NUMBER = 250,};

   CKassKKM *FKKM;
   CKassPrinter *FPrinter;
   CGoldCrown *FGoldCrown;

   TComPort *FComPort,//принтера
            *FScannerPort,
            *FPultPort,
            *FPrinterPort;
   CSimpleCommPort *FKKMPort,
                   *FPrinterPortS,
                   *FPultPortS,
                   *FScannerPortS;

   TFormCardRead *FCardReader;
   TFormCardRead *FMagRead;
   CKassTRK *FTRK;
//   CSmartDebit *FSmart;

   //мифаре карты
   CMifareCard *m_MifareCard;

   CRITICAL_SECTION cs;
   HANDLE hMutex;

   void DeleteAll ( void );

   void AddScannerCardRead ( OBJECT_OPTIONS &opt );
   void AddSimpleCardRead ( TComponent *Owner, const AnsiString &commName );

   void WriteErr ( const AnsiString &err );
};

extern CObjectManager *ObjectManager;

#endif
