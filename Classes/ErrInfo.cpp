//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ErrInfo.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

AnsiString TErrInfo::WinErrorText(int ErrNo)
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

   AnsiString Rt = AnsiString ( (char *) lpMsgBuf );
   LocalFree( lpMsgBuf );
   return Rt;
}

