//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ObjectManager.h"
#include "PultScannerCommand.h"
#include "KassMStar.h"
#include "KassStrokeKKM.h"
#include "KassPrim.h"
#include "KassShtrihKKM.h"

#include "MainData.h"

//#include "MStarDeviceErrFixed.h"
#include "ScannerCardRead.h"
#include "KassPult.h"
#include "KassPult_v1.h"
#include "KassPult_v2.h"
#include "KassNoPult.h"
#include "KassTRKAZT.h"
#include "KassPrinterFile.h"
#include "SimpleCardRead.h"
#include "ListenStruna_v1.h"
#include "ListenStruna_v2.h"
#include "KassStrokeAZS.h"
#include "KassVesnaTRK.h"
#include "CombineCardRead.h"
#include "KassTRKDresserWayne.h"
#include "LevelGageIgla.h"
#include "LevelGageInet.h"

#include "MainFunc.h"
#include "Sango.hpp"
//---------------------------------------------------------------------------

#pragma package(smart_init)

CObjectManager *ObjectManager;

CObjectManager::CObjectManager ( OBJECT_OPTIONS &opt )
{

   FKKM = 0;
   FComPort = 0;//принтера
   FScannerPort = 0;
   FPultPort = 0;
   FPrinterPort = 0;

   FKKMPort = 0;
   FPrinterPortS = 0;
   FPultPortS = 0;
   FScannerPortS = 0;

   FCardReader = 0;
   FTRK = 0;
   FGoldCrown = 0;
   FMagRead = 0;
   m_MifareCard = 0;

   m_pStruna = NULL;

   TErrInfo ErrInfo;
   ErrInfo.UUnum = 850;
   ErrInfo.Kind = HardError;
   ErrInfo.ErrCode = -1;

/*   FComPort = new TComPort;
   FComPort->ComName = opt.PrintComName;
   FScannerPort = new TComPort;
   FScannerPort->ComName = opt.CardReadComName;*/

   InitializeCriticalSection ( &cs );

/*   opt.KKMType = PRINTER_MSTAR;
   opt.KKMComName = "COM2";
   opt.PrintBaudRate = 0;//115200;*/

//   opt.KKMType = NOT_INSTALLED;
   switch ( opt.KKMType )
   {
      CKassMStar *MStar;
      CKassShtrihKKM *ShtrishKKM;

      case PRINTER_MSTAR:
         FKKMPort = new CSimpleCommPort;

         FKKMPort->SetCommPort ( opt.KKMComName );
         MStar = new CKassMStar ( opt.Owner, FKKMPort, opt.PrintBaudRate );
         FKKM = MStar;
         break;

      case KKM_STROKE:
			try
         {
         	ShtrishKKM = new CKassShtrihKKM ( opt.Owner, opt.KKMComName, opt.PrintBaudRate );
            FKKM = ShtrishKKM;
            ShtrishKKM->SetTextWidth ( 28 );
            ShtrishKKM->OpenCommPort();
         }catch ( ECommError &E )
         {
				//DeleteAll();
            //throw Exception( "Не удалось открыть порт ККМ Штрих: " +
            //   AnsiString ( E.what() ) );
            WriteErr ( "Не удалось открыть порт ККМ Штрих: " + AnsiString ( E.what() ) );
         }
         break;

      case KKM_PRIM:
         try
         {
            FKKM = new CKassPrim (opt.Owner, opt.KKMComName, opt.PrintBaudRate );
         }catch ( ECommError E )
         {
				//DeleteAll();
            //throw Exception( "Не удалось открыть порт ККМ Прим: " +
            //   AnsiString ( E.what() ) );
            WriteErr ( "Не удалось открыть порт ККМ Прим: " + AnsiString (E.what()) );
         }
         break;

      case NOT_INSTALLED:
         break;
      default:
         ErrInfo.ErrText = "Не известная ККМ";
         throw ErrInfo;
   }

   if ( ( KKM && opt.PrintBaudRate == 0 ) || ( KKM && opt.PrintBaudRate != 0 && ! KKM->TestKKM() ) )
      if ( ! KKM->SearchKKM() )
      {
         //ошибка поиска ккм
      }

   switch ( opt.KKMType )
   {
      TStringList *FormBuy, *FormReturnBuy;
      CKassMStar *MStar;

      case PRINTER_MSTAR:

         MStar = (CKassMStar*)FKKM;

         FormBuy = new TStringList;
         FormReturnBuy = new TStringList;
         if ( Data->GetPatternMStar (
            FormBuy,
            FormReturnBuy,
            &ErrInfo ) )
         {
            if(!FormBuy->Text.Trim().IsEmpty() )
               MStar->SetFormBuy ( FormBuy );
            else
                MStar->SetDefaultFormBuy();
            if(!FormReturnBuy->Text.Trim().IsEmpty() )
               MStar->SetFormReturnBuy ( FormReturnBuy );
            else
                MStar->SetDefaultReturnBuy();
         }
         else
         {
            MStar->SetDefaultFormBuy();
            MStar->SetDefaultReturnBuy();
//            WriteProt ( ErrInfo );
         }
         delete FormReturnBuy;
         delete FormBuy;
         break;
   }

   switch ( opt.PrinterType )
   {
      CKassShtrihKKM *ShtrishKKM;

      case PRINTER_MSTAR:
         if ( opt.KKMType == PRINTER_MSTAR )
            FPrinter = FKKM;
         else
         {
            FPrinterPortS = new CSimpleCommPort;
            FPrinterPortS->SetCommPort ( opt.PrintComName );
            FPrinter = new CKassMStar ( opt.Owner, FPrinterPortS, opt.PrintBaudRate );
            if ( opt.PrintBaudRate == 0 )
               ((CKassKKM*)FPrinter)->SearchKKM();
         }
         break;
      case PRINTER_FILE:
         FPrinter = new CKassPrinterFile ( opt.PrintComName );
         break;
      case NOT_INSTALLED:
         break;
      case KKM_STROKE:
         if ( opt.KKMType == KKM_STROKE )
            FPrinter = FKKM;
         else
         {
            ShtrishKKM = new CKassShtrihKKM ( opt.Owner, opt.PrintComName, opt.PrintBaudRate );
//            ShtrishKKM->SetTextWidth ( 28 );
            ShtrishKKM->OpenCommPort();
            FPrinter = ShtrishKKM;
         }
         break;

      case KKM_PRIM:
         if ( opt.KKMType == KKM_PRIM )
            FPrinter = FKKM;
         else
            try
            {
               FPrinter = new CKassPrim ( opt.Owner, opt.PrintComName, opt.PrintBaudRate );
            } catch ( ECommError &E )
            {
					//DeleteAll();
               //throw Exception( "Не удалось открыть порт ККМ Прим: " +
               //   AnsiString ( E.what() ) );
               WriteErr ( "Не удалось открыть порт ККМ Прим: " + AnsiString (E.what()) );
            }
         break;

      default:
			DeleteAll();
         ErrInfo.ErrText = "Не известный принтер";
         throw ErrInfo;
   }

   AnsiString Foot, Head;
   if ( Data->GetTicketAttr ( &Head, &Foot, &ErrInfo ) )
   {
      FPrinter->SetHead ( Head );
      FPrinter->SetFoot ( Foot );
   }
//   else
//      WriteProt ( ErrInfo );

   if ( opt.ExistPult )
   {
      switch ( opt.PultType )
      {
         CKassPult_v2 *pv2;
         CKassTRKAZT *azt;
         case NOT_INSTALLED:
            FTRK = new CKassNoPult ( opt.Owner );
            break;

         case PULT_AMUR:
            FPultPortS = new CSimpleCommPort;
            CPultScannerCommand::SetDCBProperty ( FPultPortS );
            FPultPortS->SetCommPort ( opt.PultComName );
            FTRK = new CKassPult_v1 ( opt.Owner, FPultPortS, &cs );
            break;

         case HEAD_VESNA:
            try
            {
               FTRK = new CKassVesnaTRK ( opt.Owner, opt.PultComName, 9600 );
            }catch ( ECommError &E )
            {
               DeleteAll();
               throw Exception( "Не удалось открыть порт Весны: " +
                  AnsiString ( E.what() ) );

            }
            break;

         case PULT_AMUR_v2:
            FPultPortS = new CSimpleCommPort;
            CPultScannerCommand::SetDCBProperty ( FPultPortS );
            FPultPortS->SetCommPort ( opt.PultComName );
            pv2 = new CKassPult_v2 ( opt.Owner, FPultPortS, &cs, 2 );
            FTRK = pv2;
            pv2->AddColon ( 0, 0x10 );
            pv2->AddColon ( 1, 0x11 );
            break;

         case HEAD_AZT:
         	try
            {
				   FTRK = new CKassTRKAZT ( opt.Owner, opt.PultComName, opt.HeadBaudRate );
            }catch ( ECommError &E )
            {
		      	DeleteAll();
               throw Exception( "Не удалось открыть порт ТРК АЗТ: " +
                  AnsiString ( E.what() ) );
            }
         	break;

         case HEAD_STROKE_MINI_COMP:
         case HEAD_STROKE_TRK:
            try
            {
            	FTRK = new CKassStrokeAZS ( opt.Owner, opt.PultComName, opt.HeadBaudRate/*38400/*9600*/,
               	(opt.PultType == HEAD_STROKE_MINI_COMP) );
            }catch ( ECommError &E )
            {
					DeleteAll();
               throw Exception( "Не удалось открыть порт Штрих АЗС: " +
                  AnsiString ( E.what() ) );
            }
            break;

         case HEAD_DRESSER_WAYNE:
            try
            {
               FTRK = new KassTRKDresserWayne ( opt.Owner, opt.PultComName, opt.HeadBaudRate );
            }catch ( ECommError &E )
            {
					DeleteAll();
               throw Exception( "Не удалось открыть порт TRK Dresser Wayne: " +
                  AnsiString ( E.what() ) );
            }
            break;

         default:
            ErrInfo.ErrText = "Неизвестный пульт";
		   	DeleteAll();
            throw ErrInfo;
      }
   }else
   {
      FTRK = new CKassNoPult ( opt.Owner );
   }

   switch ( opt.CardReadType )
   {
      case CARD_READ_SCANNER:
/*         if ( opt.UnionScannerAndPult )
         {
            if (( opt.PultType != PULT_AMUR && opt.PultType != PULT_AMUR_v2 )
                 || !opt.ExistPult)
            {
               FScannerPortS = new CSimpleCommPort;
               CPultScannerCommand::SetDCBProperty ( FScannerPortS );
               FScannerPortS->SetCommPort ( opt.CardReadComName );
               scannerCardRead = new CScannerCardRead ( FScannerPortS, &cs );
            }else
               scannerCardRead = new CScannerCardRead ( FPultPortS, &cs );
         }
         else
         {
            FScannerPortS = new CSimpleCommPort;
            CPultScannerCommand::SetDCBProperty ( FScannerPortS );
            FScannerPortS->SetCommPort ( opt.CardReadComName );
            scannerCardRead = new CScannerCardRead ( FScannerPortS, 0 );
         }
         FCardReader = new TFormCardRead ( opt.Owner );
         FCardReader->PushScanner ( scannerCardRead) ; */
         AddScannerCardRead(opt);
         break;
      case NOT_INSTALLED:
         break;
      case CARD_READ_SIMPLE:
         /*simpleCardRead = new CSimpleCardRead ( opt.CardReadComName, 9600, 1, ONESTOPBIT, 8 );
         FCardReader = new TFormCardRead ( opt.Owner );
         FCardReader->PushScanner ( simpleCardRead) ;*/
         AddSimpleCardRead ( opt.Owner, opt.CardReadComName );
         break;

/*      case CARD_READ_COMBINE:
         if ( opt.UnionScannerAndPult )
         {
            if (( opt.PultType != PULT_AMUR && opt.PultType != PULT_AMUR_v2 )
                 || !opt.ExistPult)
            {
               FScannerPortS = new CSimpleCommPort;
               CPultScannerCommand::SetDCBProperty ( FScannerPortS );
               FScannerPortS->SetCommPort ( opt.CardReadComName );
               FCardReader = new CCombineCardRead ( opt.Owner, FScannerPortS, &cs, opt.CardReadComName, 9600, 1, ONESTOPBIT, 8 );
            }else
               FCardReader = new CCombineCardRead ( opt.Owner, FPultPortS, &cs, opt.CardReadComName, 9600, 1, ONESTOPBIT, 8 );
         }
         else
         {
            FScannerPortS = new CSimpleCommPort;
            CPultScannerCommand::SetDCBProperty ( FScannerPortS );
            FScannerPortS->SetCommPort ( opt.CardReadComName );
            FCardReader = new CCombineCardRead ( opt.Owner, FScannerPortS, 0, opt.CardReadComName, 9600, 1, ONESTOPBIT, 8 );
         }
         break; */
		case CARD_READ_COMBINE:
         AddScannerCardRead(opt);
         AddSimpleCardRead (opt.Owner, opt.CardReadComName );
      	break;

      default:
         //ErrInfo.ErrText = "Не известный сканер";
         //throw ErrInfo;
         WriteErr ( "Не известный сканер" );
   }

   if ( opt.bUseSecondScanner )
   {
      AddSimpleCardRead (opt.Owner, opt.SecondScannerCommName );
   }

   if ( opt.UseGoldCrown )
   {
      FGoldCrown = new CGoldCrown ( opt.GoldCrownComName );
   }


   if ( opt.MagReader )
   {
      CSimpleCardRead *simpleCardRead = new CSimpleCardRead ( opt.MagComName );
      FMagRead = new TFormCardRead (opt.Owner );
      FMagRead->PushScanner ( simpleCardRead );
   }


   if ( opt.UseMifareCard )
   {
      try
      {
         m_MifareCard = new CMifareCard ( "001DC001", opt.MifarePortName );
      }
      catch ( const TErrInfo &err )
      {
         opt.UseMifareCard = false;
         WriteProt ( err );
      }
   }

   switch ( opt.m_siu )
   {

   case siuStruna:
   	m_pStruna = new CListenStruna_v1 ( opt.StrunaCommName );
      if ( ! m_pStruna->StartListen() )
         WriteErr ( "Не удалось соединится со струной" );
      break;

   case siuIgla:
      m_pStruna = new LevelGageIgla ( opt.StrunaCommName );
      if ( ! m_pStruna->StartListen() )
         WriteErr ( "Не удалось соединится с Иглой" );
      break;

   case siuService:
      m_pStruna = new  LevelGageInet(opt.m_SIUIPAddress, opt.m_SIUPort);
      if ( ! m_pStruna->StartListen() )
         WriteErr ( "Не удалось соединится с Сервисом СИУ" );
      break;

   default:
      m_pStruna = NULL;
      break;
   }

   if ( opt.TerminalRegim )
   {
      m_TerminalServer = boost::shared_ptr<TerminalServer> ( new TerminalServer() );
   }

   if ( opt.SendSMSMode )
   {
      m_SMSSender = boost::shared_ptr<KassSMSSender> (
         new KassSMSSender(opt.PhoneCommPortName, opt.PhoneCommPortBaudRate ) );
   }

   int OpenPortNumber = 0;
   try
   {
      if ( FComPort )
         FComPort->Open ( );
      OpenPortNumber = 1;
      if ( FScannerPort )
         FScannerPort->Open ( );
      OpenPortNumber = 2;
      if ( FPrinterPort )
         FPrinterPort->Open ( );
      OpenPortNumber = 3;
      if ( FPultPort )
         FPultPort->Open ( );
      OpenPortNumber = 4;
      if ( FPrinterPortS )
         FPrinterPortS->OpenCommPort();
      OpenPortNumber = 5;
      if ( FKKMPort && FKKMPort->GetConnected() )
         FKKMPort->OpenCommPort();
      OpenPortNumber = 6;
      if ( FPultPortS )
         FPultPortS->OpenCommPort();
      OpenPortNumber = 7;
      if ( FScannerPortS )
         FScannerPortS->OpenCommPort();
   }
   catch ( ECommError E )
   {
      /*Информативное оповещение об ошибках ком порта
      30.11.2005        Ногин В.Г.*/
      AnsiString errinfo;
      switch ( OpenPortNumber )
      {
      case 0:
         errinfo = "Не удалось открыть ком порт, который кстати давно не используется, чёрт знает зачем мы его открываем: ";
   		DeleteAll();
         throw Exception( errinfo + AnsiString(E.what()) );
      case 1:
         errinfo = "Не удалось открыть ком порт, который кстати давно не используется, но был для сканера, чёрт знает зачем мы его открываем: ";
   		DeleteAll();
         throw Exception( errinfo + AnsiString(E.what()) );
      case 2:
         errinfo = "Не удалось открыть ком порт, который кстати давно не используется, но был судя по названию для принтера, чёрт знает зачем мы его открываем: ";
   		DeleteAll();
         throw Exception( errinfo + AnsiString(E.what()) );
      case 3:
         errinfo = "Не удалось открыть ком порт, который кстати давно не используется, но был для пульта, чёрт знает зачем мы его открываем: ";
   		DeleteAll();
         throw Exception( errinfo + AnsiString(E.what()) );
      case 4:
         errinfo = "Не удалось открыть ком порт принтера (M-Start ФР): ";
         WriteErr ( errinfo + AnsiString(E.what()) );
         break;
      case 5:
         errinfo = "Не удалось открыть ком порт ККМ (M-Start ФР): ";
         WriteErr ( errinfo + AnsiString(E.what()) );
         break;
      case 6:
         errinfo = "Не удалось открыть порт пульта: ";
   		DeleteAll();
         throw Exception( errinfo + AnsiString(E.what()) );
      case 7:
         errinfo = "Не уадлось открыть порт штрих кодового сканера: ";
         WriteErr ( errinfo + AnsiString(E.what()) );
         break;
      default:
   		DeleteAll();
         throw Exception( AnsiString(E.what()) );
      }
   }
}

void CObjectManager::DeleteAll ( void )
{
   if ( m_TerminalServer )
      m_TerminalServer->WaitDisposing();

   if ( FTRK )
      FTRK->bExit = true;
   if ( FTRK )
      delete FTRK;
   if ( FPultPort )
      delete FPultPort;

   if ( FKKM )
   {
      if ( FKKM != FPrinter )
      {
         if ( FPrinter )
            delete FPrinter;
         FPrinter = 0;
      }else
         FPrinter = 0;
      delete FKKM;
   }
   if ( FPrinter )
      delete FPrinter;
   if ( FCardReader )
      delete FCardReader;
//   Sleep ( 300 );

   if ( FComPort )
      delete FComPort;
   if ( FPrinterPort )
      delete FPrinterPort;
   if ( FScannerPort )
      delete FScannerPort;


   if ( FPultPortS )
      delete FPultPortS;
   if ( FScannerPortS )
      delete FScannerPortS;


   DeleteCriticalSection ( &cs );

   if ( FGoldCrown )
      delete FGoldCrown;
   if ( FMagRead )
      delete FMagRead;

/*   if ( FSmart )
   {
      FSmart->Close();
      delete FSmart;
   }*/
   if ( m_MifareCard )
      delete m_MifareCard;

   if ( FPrinterPortS )
      delete FPrinterPortS;
   if ( FKKMPort )
      delete FKKMPort;

	if ( m_pStruna )
   {
      m_pStruna->Dispose();
   	delete m_pStruna;
      m_pStruna = NULL;
   }
}

CObjectManager::~CObjectManager ( void )
{
//   FComPort->EndRead ( );
//   FScannerPort->EndRead ( );
//   Sleep ( 10000 );
	DeleteAll();
}

bool CObjectManager::IsWorking ( void )
{
   return ( KKM && KKM->IsPrintWorking () ) || ( FPrinter && FPrinter->IsPrintWorking ( ) );
}

bool CObjectManager::MayPrintKKM ( int CountWorking )
{
   switch ( CountWorking )
   {
      case 0: return ( ! FKKM->IsPrintWorking ( ) );
      case 1: return ( FPrinter->IsPrintWorking ( ) && ! FKKM->IsPrintWorking ( ) );
      default: return false;
   }
}

bool CObjectManager::MayPrintPrinter ( int CountWorking )
{
   switch ( CountWorking )
   {
      case 0: return ( ! FPrinter->IsPrintWorking ( ) );
      case 1: return ( KKM->IsPrintWorking ( ) && ! FPrinter->IsPrintWorking ( ) );
      default: return false;
   }
}

void CObjectManager::PrepareClose ( void )
{
   if ( FTRK )
      FTRK->PrepareClose();
   if ( FKKM )
      FKKM->PrepareClose();

   if( m_pStruna )
      m_pStruna->DisposeBegin();

   if ( m_TerminalServer )
      m_TerminalServer->Dispose();
}

void CObjectManager::AddScannerCardRead ( OBJECT_OPTIONS &opt )
{
   CScannerCardRead *scannerCardRead;
	if ( opt.UnionScannerAndPult )
   {
   	if (( opt.PultType != PULT_AMUR && opt.PultType != PULT_AMUR_v2 )
      	|| !opt.ExistPult )
      {
      	FScannerPortS = new CSimpleCommPort;
         CPultScannerCommand::SetDCBProperty ( FScannerPortS );
         FScannerPortS->SetCommPort ( opt.CardReadComName );
         scannerCardRead = new CScannerCardRead ( FScannerPortS, &cs );
      }else
      	scannerCardRead = new CScannerCardRead ( FPultPortS, &cs );
   }
   else
   {
   	FScannerPortS = new CSimpleCommPort;
      CPultScannerCommand::SetDCBProperty ( FScannerPortS );
      FScannerPortS->SetCommPort ( opt.CardReadComName );
      scannerCardRead = new CScannerCardRead ( FScannerPortS, 0 );
   }
   if ( FCardReader == 0 )
	   FCardReader = new TFormCardRead ( opt.Owner );
   FCardReader->PushScanner ( scannerCardRead) ;
}

void CObjectManager::AddSimpleCardRead ( TComponent *Owner, const AnsiString &commName )
{
   CSimpleCardRead *simpleCardRead = 0;
   try
   {
      simpleCardRead = new CSimpleCardRead ( commName, CBR_9600, NOPARITY, ONESTOPBIT, 8 );
   }
   catch ( Exception &ex )
   {
      WriteErr ( ex.Message );
   }
   if (FCardReader == 0)
      FCardReader = new TFormCardRead ( Owner );
   if ( simpleCardRead )
      FCardReader->PushScanner ( simpleCardRead) ;
}

void CObjectManager::WriteErr ( const AnsiString &err )
{
   TErrInfo errinfo;
   errinfo.UUnum = MODULE_NUMBER;
   errinfo.ErrCode = -1;
   errinfo.Kind = HardError;
   errinfo.ErrText = err;
   WriteProt ( errinfo );
}

