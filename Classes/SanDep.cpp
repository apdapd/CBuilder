//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SanDep.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

SanDep::SanDep()
{
   Doze = 0;
   DepNumber = 0;    //Depart Number
   DepError = 0;     //Error Code
   ValGSM = 0;       //Current GSM's Value in mL
   AskGSM = 0;       //Asked GSM's Value in mL
   PriceGSM = 0;     //GSM's Price
   CodeGSM = 0;      //GSM's Code
// char NameGSM[19]; //GSM's Name
   NameGSM[0] = '\0'; 
   ImpMlValue = 1000;     // Value in mL for 1 sinchro-imp
   ImpTDelay = 0;      // Time to start klapan
   VibroPeriod = 0;    //Period of 1-Vibrator
   State = DSWait;          //Current state of Depart
   ButState = 1;      //Current state of Start/Stop button
   EngState = 0;      //Current state of Depart's Engine
   DepActive = 0;     //Current state of Depart's activity
   DepCommand = 0;    //Command from Depart window
   PuAnswer = 0;      //Answer of TRK Depart
   Error = false;         //
   Flag = 0;

   ScPriceTerminal = 0;

   ButtonWasPressed = false;

   bOccupancy = false;
}

SanDepVector::SanDepVector()
{
   m_hOccupancyMutex = CreateMutex ( NULL, false, NULL );
}

SanDepVector::~SanDepVector()
{
   CloseHandle ( m_hOccupancyMutex );
}

void SanDepVector::FreeOccupancy ( int index )
{
   WaitForSingleObject ( m_hOccupancyMutex, INFINITE );

   int dep = (*this)[index]->GroupIndex;
   for ( int i = 0; i < size(); ++i )
   {
      if ( (*this)[i] )
      {
         if ( (*this)[i]->GroupIndex == (*this)[index]->GroupIndex )
         {
            (*this)[i]->bOccupancy = false;
            (*this)[i]->OccupTermNum = 0;
         }
      }
   }

   ReleaseMutex ( m_hOccupancyMutex );
}

bool SanDepVector::GetOccupancy ( int index, int TermNumber )
{
   if ( WaitForSingleObject ( m_hOccupancyMutex, 5000 ) == WAIT_OBJECT_0 )
   {

      bool free = true;
      int dep = (*this)[index]->GroupIndex;
      for ( int i = 0; i < size(); ++i )
      {
         if ( (*this)[i] )
         {
            if ( (*this)[i]->GroupIndex == (*this)[index]->GroupIndex )
            {
               if ( (*this)[i]->bOccupancy )
                  free = false;
            }
         }
      }

      if ( free )
      {
         for ( int i = 0; i < size(); ++i )
         {
            if ( (*this)[i] )
            {
               if ( (*this)[i]->GroupIndex == (*this)[index]->GroupIndex )
               {
                  (*this)[i]->bOccupancy = true;
                  (*this)[i]->OccupTermNum = TermNumber;
               }
            }
         }
      }

      ReleaseMutex ( m_hOccupancyMutex );
      return free;
   }
   else
      return false;
}

AnsiString SanDepVector::GetTermOccupString()
{
   AnsiString res = "";
   for ( int i = 0; i < size(); ++i )
   {
      if ( (*this)[i] )
      {
         if ( (*this)[i]->bOccupancy )
         {
            res = res + "ch " + IntToStr(i) + ":" + (*this)[i]->OccupTermNum + "; ";
         }
      }
   }
   return res;
}

