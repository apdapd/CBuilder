//---------------------------------------------------------------------------

#ifndef SanDepH
#define SanDepH
//---------------------------------------------------------------------------

#include "ValueHistory.h"
#include <vector>

enum SANDEP_STATE
{
   DepExistWork     = 0,
   DepExistNotWork = -1,
   DepNotExist     = -2,

   DSWait    = 0,
   DSStart   = 1,
   DSPreWork  = 2,
   DSAlert   = 3,
   DSWork    = 4,
   DSStop    = 5,
   DSStopDep = 51,
   DSStopOp  = 52,
   DSPause   = 6,
   DSPausePre = 60,
   DSError   = 7,
   DSAbort   = 8,
   DSNegativDoze = 63,
   DSRepairWork = 99,
};


class SanDep
{
public:

   enum {GSM_LENGTH=20};

    SanDep ( void );


 int DepNumber;    //номер канала
 int DepError;     //Error Code
 int ValGSM;       //Current GSM's Value in mL
 int AskGSM;       //Asked GSM's Value in mL
 int PriceGSM;     //GSM's Price
 int CodeGSM;      //GSM's Code
 char NameGSM[GSM_LENGTH]; //GSM's Name
 int ImpMlValue;     // Value in mL for 1 sinchro-imp
 int ImpTDelay;      // Time to start klapan
 TTime EngActiveTimer; //
 TTime FirstImpTimer;  //
 TTime AllImpTimer;    //
 TTime LastImpTimer;   //
 TTime EngineWorkTime;//время простаивания двигателя колонки

 int GroupIndex;			//индекс группы каналов(колонка по рабоче-крестьянски)

 int VibroPeriod;    //Period of 1-Vibrator
 //int State;          //Current state of Depart
// ValueHistory<SANDEP_STATE> State;
 SANDEP_STATE State;

 Byte ButtonUse;     //Kind of button development
 Byte InvImpulse;    //
 Byte InvButton;     //
 Byte Flag;          //
 Byte ButState;      //Current state of Start/Stop button
 Byte EngState;      //Current state of Depart's Engine
 Byte DepActive;     //Current state of Depart's activity
 Byte DepCommand;    //Command from Depart window
 Byte PuAnswer;      //Answer of TRK Depart
 bool Error;         //
 int Doze;


 //для форсированной остановки
 bool ButtonWasPressed;    //кнопка была нажата
 TTime ButtonPressedTime;  //когда была нажата кнопка

 //для терминалов. Признак занятости канала
 bool bOccupancy;
 int OccupTermNum;   //номер терминала
 int PriceGSMTerminal;
 int ScPriceTerminal;  

};

class SanDepVector : public std::vector<SanDep*>
{
public:

   SanDepVector();
   ~SanDepVector();

   void FreeOccupancy ( int index );
   bool GetOccupancy ( int index, int TermNumber );

   AnsiString GetTermOccupString();

private:

   HANDLE m_hOccupancyMutex;

};

#endif
