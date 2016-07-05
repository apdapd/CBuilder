//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ComPort.h"

#include "comm.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TComPort::TComPort()
{
	FRead = 0;
    FNoRead = 0;
    FTerminated = 0;
	FBuffer = 0;
    FLastRead = 0;
    bContinue = false;
    hThread = 0;
    FBufferSize = 100;
    FReadSize = 10;
    GlobPos = 0;
    FWaitTime = 100;
    FBreakCount = 0;
    FStartBreakCount = 0;
    ce = ceOk;
    FComName = "COM1";

}
//---------------------------------------------------------------------------
bool __fastcall TComPort::GetConnected ( void )
{
	return ComPort.GetConnected ( );
}

void TComPort::Open ( void )
{
	if ( ! GetConnected ( ) )
    {
		ComPort.SetCommPort ( GetComName ( ) );
	    ComPort.OpenCommPort ( );
    }
    else
        throw Exception ( "Уже открыт" );
}

void TComPort::Close ( void )
{
	if ( GetConnected ( ) )
    {
    	if ( IsStartRead )
        	EndRead ( );
    	ComPort.CloseCommPort ( );
    }
}

void TComPort::WriteData ( void *Data, int Size )
{
	if ( GetConnected ( ) )
    {
    	DWORD written;
        if ( ! WriteFile ( ComPort.GetHandle ( ), Data, Size, &written, 0 ) )
			throw ECommError ( ECommError::WRITE_ERROR );
    }
}

int TComPort::ReadData ( void *Data, int Size )
{
   	DWORD readen;
	if ( GetConnected ( ) )
    {
        if ( ! ReadFile ( ComPort.GetHandle ( ), Data, Size, &readen, 0 ) )
			throw ECommError ( ECommError::READ_ERROR );
    }
    return readen;
}

void TComPort::WriteByte ( BYTE Data )
{
	ComPort.PutByte ( Data );
}

BYTE TComPort::ReadByte ( void )
{
	return ComPort.GetByte ( );
}

void TComPort::DiscardBytes(unsigned int MaxBytes)
{
   ComPort.DiscardBytes ( MaxBytes );
}

AnsiString __fastcall TComPort::GetComName ( void )
{
    return FComName;
}

void __fastcall TComPort::SetComName ( AnsiString Name )
{
    FComName = Name;
}

void TComPort::StartRead ( void )
{
	if ( IsStartRead || ( ! GetConnected ( ) ) )
    	return;
	bContinue = true;
   if ( FReadSize > FBufferSize )
      FReadSize = FBufferSize;
    if ( FBufferSize == 0 ||
    	 FBuffer == 0)
    	bContinue = false;

   if ( hThread )
      CloseHandle ( hThread );

       hThread = CreateThread (
    				0,
    				0,
                    (LPTHREAD_START_ROUTINE)ReadProc,
                    (TComPort*)this,
                    0,//CREATE_SUSPENDED,
                    (LPDWORD)&ThreadId );
}

void TComPort::EndRead ( void )
{
	if ( IsStartRead )
    {
     	ce = ceTerminated;
	   bContinue = false;
		TerminateThread ( hThread, 0 );
		delete []FLastRead;
	    CloseHandle ( hThread );
      hThread = 0;
		if ( FTerminated != 0 )
	    {
		    FTerminated ( this, ce );
	    }
    }
}

LONG ReadProc ( TComPort *Port )
{
	Port->RoutineProc ( );
	return 0;
}

COMERROR TComPort::RoutineProc ( void )
{
	FLastRead = new BYTE[FReadSize + 1];
   GlobPos = 0;
   CurBreakCount = 0;
   ce = ceOk;

   memset ( FBuffer, 0, FBufferSize );
	while ( bContinue )
   {
      DWORD read,
         readsize = FReadSize;
		if ( readsize > FBufferSize - GlobPos )
         readsize = FBufferSize - GlobPos;

      TDateTime time = Now();
      read = ReadData ( FLastRead, readsize );
      TDateTime cur = Now();
      TDateTime interval = cur - time;

      if ( read != 0  )
         CurBreakCount = 0;
      for ( DWORD i = 0; i < read; i++, GlobPos++ )
         FBuffer[GlobPos] = FLastRead[i];
      if ( GlobPos >= FBufferSize )
         bContinue = false;
      if ( read == 0 )
      {
         if ( ( FBreakCount != 0 && GlobPos != 0 ) || ( GlobPos == 0 && FStartBreakCount != 0 ) )
         {
            CurBreakCount++;
            if ( GlobPos != 0   )
            {
               if ( CurBreakCount == FBreakCount )
    	    	   {
                  ce = ceTimeUp;
    	        	   bContinue = false;
               }
            }
            else
            {
               if ( CurBreakCount == FStartBreakCount )
               {
                  ce = ceOutOfData;
                  bContinue = false;
               }
            }
         }
         if ( FNoRead != 0 )
            FNoRead ( this, read );

         if ( bContinue )
         {
            unsigned short hour, min, sec, msec;
            DecodeTime ( interval, hour, min, sec, msec );
            int LastTime = msec + sec * 1000 + min * 1000 * 60 + hour * 1000 * 60 * 60;
            if ( FWaitTime > LastTime )
              	Sleep ( FWaitTime - LastTime );//, true );
         }

      }
      else
      {
         if ( FRead != 0 )
	        	FRead ( this, read );
      }
   }

	delete []FLastRead;
	if ( FTerminated != 0 )
    	FTerminated ( this, ce );
   return ce;
}

WORD __fastcall TComPort::GetCountByteInBuffer ( void )
{
	return GlobPos + 1;
}

__fastcall TComPort::~TComPort ( void )
{
   FTerminated = 0;
   FRead = 0;
   FNoRead = 0;
	Close ( );
}

COMERROR TComPort::GetError ( void )
{
	return ce;
}

void __fastcall  TComPort::SetBaudRate(unsigned int newBaud)
{
	ComPort.SetBaudRate ( newBaud );
}

unsigned int __fastcall  TComPort::GetBaudRate(void)
{
	return ComPort.GetBaudRate ( );
}

void __fastcall  TComPort::SetParity(BYTE newParity)
{
	ComPort.SetParity ( newParity );
}

BYTE __fastcall  TComPort::GetParity(void)
{
	return ComPort.GetParity ( );
}

void __fastcall  TComPort::SetByteSize(BYTE newByteSize)
{
	ComPort.SetByteSize ( newByteSize );
}

BYTE __fastcall  TComPort::GetByteSize(void)
{
	return ComPort.GetByteSize ( );
}

void __fastcall  TComPort::SetStopBits(BYTE newStopBits)
{
	ComPort.SetStopBits ( newStopBits );
}

BYTE __fastcall  TComPort::GetStopBits(void)
{
	return ComPort.GetStopBits ( );
}

void TComPort::SetCommDCBProperties(DCB &properties)
{
	ComPort.SetCommDCBProperties ( properties );
}

void TComPort::GetCommDCBProperties(DCB &properties)
{
	ComPort.GetCommDCBProperties ( properties );
}

HANDLE TComPort::GetHandle ( void )
{
	return ComPort.GetHandle ( );
}

void TComPort::SetCommTimeounts ( COMMTIMEOUTS  nm_TimeOuts )
{
   ComPort.SetCommTimeouts ( nm_TimeOuts );
}

