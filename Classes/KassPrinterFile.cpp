//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdlib.h>
#include <algorithm>
#pragma hdrstop

#include "KassPrinterFile.h"
#include "Sango.hpp"

using namespace std;
//---------------------------------------------------------------------------

#pragma package(smart_init)
int CKassPrinterFile::ModuleNumber = 802;

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

CKassPrinterFile::CKassPrinterFile ( AnsiString Name )
{
   m_bOverlapped = false;
	DWORD Param = FILE_ATTRIBUTE_NORMAL;
	if ( Name.SubString ( 1, 2 ) == "LL" )
   {
		m_SleepTime = 0;
      m_bOverlapped = false;
   }
   else
   {
   	m_bOverlapped = true;
		m_SleepTime = 2;
		Param = Param | FILE_FLAG_OVERLAPPED;
   }

   File = CreateFile (
      (LPCTSTR)Name.c_str ( ),
      GENERIC_WRITE,
      FILE_SHARE_WRITE,
      0,
      OPEN_ALWAYS,
      Param,
      0 );
    COMMTIMEOUTS m_TimeOuts;
    m_TimeOuts.ReadIntervalTimeout         = 15;
    m_TimeOuts.ReadTotalTimeoutMultiplier  = 1;
    m_TimeOuts.ReadTotalTimeoutConstant    = 250;
    m_TimeOuts.WriteTotalTimeoutMultiplier = 2;
    m_TimeOuts.WriteTotalTimeoutConstant   = 50;
    if ( ! m_bOverlapped )
	    if(!SetCommTimeouts(File, &m_TimeOuts))
   	 {
        // something is hay wire, close the port and return
//        CloseHandle(m_hCom);
//        throw ECommError(ECommError::SETCOMMTIMEOUTS);
	    }
   if ( File == INVALID_HANDLE_VALUE )
   {
      TErrInfo ErrInfo;
      ErrInfo.UUnum = ModuleNumber;
      ErrInfo.ErrCode = -1;
      ErrInfo.Kind = HardError;
      ErrInfo.ErrText = AnsiString ( "Не открылся " ) + Name;
      throw ErrInfo;
   };
   if ( ! m_bOverlapped )
	   SetFilePointer ( File, 0, 0, FILE_END );
}

__fastcall CKassPrinterFile::~CKassPrinterFile ( void )
{
   CloseHandle ( File );
}

bool CKassPrinterFile::LptPrint ( TStrings *List )
{
   for ( int i = 0; i< Foot->Count; i++ )
      List->Add ( Foot->Strings[i] );
   for ( int i = 0; i < Head->Count; i++ )
      List->Add ( Head->Strings[i] );

   TStringList *NewList = new TStringList;
   NewList->AddStrings ( List );
//   CKassPrinter::LptPrint ( NewList );

   AnsiString Temp, buf;
//   for ( int i = 0; i < List->Count; i++ )
//   {
   Temp.SetLength ( NewList->Text.Length ( ) );
   CharToOem ( NewList->Text.c_str ( ), Temp.c_str ( ) );
   char cc = 27;
   Temp += cc; Temp += "i";
//   }
   DWORD written = 0;
   bool bok = false;
   DWORD pos;

   OVERLAPPED overlapped;
   overlapped.Internal = 0;
   overlapped.InternalHigh = 0;
   overlapped.Offset = 0;
   overlapped.OffsetHigh = 0;
   HANDLE hEvent = CreateEvent ( NULL, true, false, NULL );
   overlapped.hEvent = hEvent;

   BOOL res;
   if ( m_bOverlapped )
   	res = WriteFile ( File, Temp.c_str(), 1, &written, &overlapped );
   else
		res = WriteFile ( File, Temp.c_str(), 1, &written, NULL ) && written != 0;


   if (  res != 0 || m_bOverlapped )
   {

   	if ( !m_bOverlapped || WaitForSingleObject ( hEvent, 0xFF ) == WAIT_OBJECT_0 )
      {

	   	pos = 1;
  		   bok = true;
   		while ( pos < (DWORD)Temp.Length() && bok )
	      {
			   overlapped.Internal = 0;
			   overlapped.InternalHigh = 0;
			   overlapped.Offset = 0;
			   overlapped.OffsetHigh = 0;

			   if ( m_bOverlapped )
			   	res = WriteFile ( File, Temp.c_str ( ) + pos,
	      	   	min(Temp.Length ( ) - pos, (unsigned long)1024), &written, &overlapped );
			   else
					res = WriteFile ( File, Temp.c_str ( ) + pos,
	      	   	min(Temp.Length ( ) - pos, (unsigned long)1024), &written, NULL ) && written != 0;

			   if ( ! res )
		   	{
            	if ( GetLastError() != ERROR_IO_PENDING )
	     	   		bok = false;
				}//if ( WriteFile

            if ( m_bOverlapped && bok )
            {
	            if ( WaitForSingleObject ( hEvent, 20 * 1024 * m_SleepTime ) != WAIT_OBJECT_0 )
   	         	bok = false;
               else
               {
               	DWORD Send;
               	GetOverlappedResult ( File, &overlapped, &Send, FALSE );
                  pos += Send;
               }
            }

            if ( ! m_bOverlapped )
	   	      if ( written == 0 )
   	   	   {
      	   		bok = false;
	      	   }

   	      pos += written;

            if ( ! m_bOverlapped )
	      	   if ( m_SleepTime != 0 )
		      	   Sleep ( 15 * written * m_SleepTime );
	      }

      }
   }//if ( WriteFile

//   CancelIo ( File );
//   CloseHandle ( hEvent );

   if ( ! bok )
   {
       TErrInfo ErrInfo;
       ErrInfo.UUnum = ModuleNumber;
       ErrInfo.ErrCode = ( GetLastError() > 0 ? GetLastError() : -1 );
       ErrInfo.Kind = HardError;
       ErrInfo.ErrText = "Ошибка работы печатающего устройства ";
       if ( ErrInfo.ErrCode == ERROR_IO_PENDING )
			ErrInfo.ErrText += "Печатающее устройства скорее всего не работает";
       else if ( ErrInfo.ErrCode > 0 )
          ErrInfo.ErrText += ErrorText ( ErrInfo.ErrCode );
       delete NewList; NewList = NULL;

       throw ErrInfo;
   }
   if(NewList)
     delete NewList;
   return true;
}

bool CKassPrinterFile::IsPrintWorking ( void )
{
   return false;
}

void __fastcall CKassPrinterFile::Execute ( void )
{
}

