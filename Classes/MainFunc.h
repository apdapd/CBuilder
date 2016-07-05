//---------------------------------------------------------------------------
#ifndef MainFuncH
#define MainFuncH
#include <DBTables.hpp>
#include "Sango.hpp"
/*
#include "StSBR.h"
#include "KKM_Prn.h"
#include "IB_Components.hpp"
*/

extern DWORD iIdMainThread;

//---------------------------------------------------------------------------
 AnsiString __fastcall Regread(AnsiString folder,AnsiString Key,int mode);
 void WriteProt(int Num, int ErrCode, AnsiString SendStr);
 void WriteProt(int Num, int ErrCode, AnsiString SendStr, int Kind);
 void WriteProt(TErrInfo ErrInfo);
/*
 bool PayCashAccept(CashGoods * ThisGoods, TErrInfo *pErrInfo);
 bool PayTechAccept(StGoods *ThisGoods, TErrInfo *pErrInfo, TStringList *pTicket);
 bool PayTermAccept(CashGoods * ThisGoods, TErrInfo *pErrInfo);
 bool PayBonusAccept(StGoods * ThisGoods, TErrInfo *pErrInfo, TStringList *pTicket);
 bool PayMifareAccept(StGoods * ThisGoods, TErrInfo *pErrInfo, TStringList *pTicket);
 bool PayMifareCapacity(StGoods * ThisGoods, TErrInfo *pErrInfo);
 bool GetTechTicket(StGoods *ThisGoods, TErrInfo *pErrInfo, TStringList *pTicket);
 bool GetCashTicket(CashGoods * pMyGoods, TErrInfo *pErrInfo);
 bool DepStart(int Dep, int Vol);
 bool DepAbort(int Dep);
 bool DepStop(int Dep);
 bool DepPause(int Dep);
 bool DepContinue(int Dep);
 bool DepCancel(int Dep);
 bool KEndDay(bool BaseCopy, int * NumEn, TErrInfo *pErrInfo);
 bool KSmenaPrint(int NumEn, TErrInfo *pErrInfo);
 void SetNumEnd(int NumE);
 void SetNumOper(int NumO);
 bool CheckPrint();
 void SetWaitMess(AnsiString St);
 void EndWaitMess();
 void SayBeep();
 AnsiString ErrorText(int ErrNo);
 void FasWork();
 void TicketUnpay();
 void TicketShow(TStringList * ThisTicket);
 void Configurate();
 bool RecAppend(TDataSet * T_From, TDataSet * T_To);
 bool RecAppend(TIB_Query * T_From, TIB_Query * T_To);
 bool LoadTechTable(TTable *TeTable, AnsiString *pL_Field, AnsiString *pK_Field);
 bool GetTechNum(int *pTeInN, TErrInfo *pErrInfo);
 bool GetTechList(TechInfo *TeInArr, TErrInfo *pErrInfo);
 void TicketAskPrint();
 bool TicketsGet(int kind,TStringList * ThisTicket, int nBeg, int nEnd,
      int * pnAll, TErrInfo *pErrInfo);
 bool GetCashTextTicket(CashGoods * pThisCashGoods, TErrInfo *pErrInfo,
       TStringList * ThisTicket);
 bool GetTermTextTicket(CashGoods *pThisCashGoods, TErrInfo *pErrInfo,
	 TStringList * ThisTicket);
 bool GetCashPrice(int CodeGSM, int **CaInfoArray, int *NumCash,
              TErrInfo *pErrInfo);
 bool GetBonusCurrTicket(StGoods * pMyGoods, TStringList * ThisTicket,
                    TErrInfo *pErrInfo);
 void CalcBonus(StGoods *pMyGoods);
 int CalcVolBonus(StGoods *pMyGoods);

 void SetTicketShow(bool SetT);
 bool GetMess(StGoods * pMyGoods, TStringList * TicketMess, TErrInfo *pErrInfo);
 int MakePay(double Vol, int Price);
 int GetFas(TCashList **ArrFas, TErrInfo *pErrInfo);
*/
 /*запись адреса потока в файл*/
// void WriteThreadAddress ( HANDLE hThread, DWORD ThreadId, AnsiString str );

//---------------------------------------------------------------------------
extern int NDeparts;         // Number of departs

#endif
