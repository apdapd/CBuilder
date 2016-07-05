//---------------------------------------------------------------------------
#include <vcl.h>
#include <Registry.hpp>
#pragma hdrstop

#include "MainFunc.h"
/*
#include "MainProt.h"
#include "MainData.h"
#include "DStand.h"
#include "MenuKass.h"
#include "MainConf.h"
#include "MyMess.h"
#include "MainTRK.h"
#include "MyFas.h"
#include "MyUnpay.h"
#include "DCash.h"
#include "MyTShow.h"
#include "DTech.h"
#include "DLitr.h"
#include "DTerm.h"
#include "DMifare.h"
//#include "DatNCT.h"
//#include "DatDKBar.h"
#include "TickPri.h"
#include "K_Mess.h"
#include "Sango.hpp"
#include "Version.h"

#include "ObjectManager.h"
#include "ThWriteProt.h"
*/
#define CashTicket   0
#define CardStTicket 1
#define CardLiTicket 2
#define TechTicket   3
#define MifareTicket 4

#pragma package(smart_init)
extern bool CashSystem,CardStSys,CardLiSys,TechSystem,CardNCTSys;
extern int OurComp;
bool CanTicketShow;

DWORD iIdMainThread = 0;

//---------------------------------------------------------------------------
AnsiString ErrorText(int ErrNo)
{
 LPVOID lpMsgBuf;

 FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    ErrNo,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPTSTR) &lpMsgBuf,
    0,
    NULL
    );

 AnsiString Rt = (char *) lpMsgBuf;
// Free the buffer.
 LocalFree( lpMsgBuf );
 return Rt;
}
//---------------------------------------------------------------------------
AnsiString __fastcall Regread(AnsiString folder,AnsiString Key,int mode)
{
 AnsiString S="";
 int p,p1;
 TRegistry *Registry = new TRegistry;
 try
 {
  Registry->RootKey = HKEY_LOCAL_MACHINE;
  if(Registry->OpenKeyReadOnly(folder))
  {
   if(mode==0)
    S = Registry->ReadString(Key);
   if(mode==1)
    Registry->WriteString("ServerN",Key);
   if(mode==2)
	Registry->WriteString("DB",Key);

  }
  Registry->CloseKey();
 }
 __finally
 {
  delete Registry;
 }
 return S;
}
//---------------------------------------------------------------------------
void WriteProt(int Num, int ErrCode, AnsiString SendStr)
{
// MyProt->WriteProt(Num, ErrCode, SendStr, 1, Now());
}
//---------------------------------------------------------------------------
void WriteProt(int Num, int ErrCode, AnsiString SendStr, int Kind)
{
// MyProt->WriteProt(Num, ErrCode, SendStr, Kind, Now());
}
//---------------------------------------------------------------------------
void WriteProt(TErrInfo ErrInfo)
{
//   DWORD id = GetCurrentThreadId();
//   if ( ThWriteProt && id != iIdMainThread )
  /////////    ThWriteProt->Add ( ErrInfo );
//   else
//      MyProt->WriteProt(ErrInfo.UUnum, ErrInfo.ErrCode, ErrInfo.ErrText,
//         ErrInfo.Kind);
}

