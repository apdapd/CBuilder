//---------------------------------------------------------------------------

#ifndef ComPortCommandH
#define ComPortCommandH

#include "Command.h"
#include "ComPort.h"

const int WRITE_ERROR = -1;
const int NOT_DATA = -2;
const int CANNOT_READ = -3;
//const int NOT_DATA = -10;
const int NOT_START = -11;
const int NOT_END = -12;
const int BAD_CHECK_SUM = -13;

//---------------------------------------------------------------------------

class CComPortAnswer : public CAnswer
{
public:
   virtual AnsiString ErrorStr ( void );
};

class CComPortCommand : public CCommand
{
public:
   CComPortCommand ( TComPort *nPort );
   virtual ~CComPortCommand ( void );

   void SetCmd ( char *nCmd, DWORD Size );

   virtual bool Exec ( void );

   static AnsiString GetErrorStr ( int Index );
protected:
   TComPort *Port;

   virtual void OnEndRead ( COMERROR error, char *answer, DWORD Count );
   virtual void OnRead ( int ReadBytes );

   bool StartRead ( DWORD MaxSize );

   __property char *Command = {read = Cmd};
   __property DWORD SizeCommand = {read = SizeCmd};

private:
   char *Cmd, *AnswerBuffer;
   DWORD SizeCmd, SizeAnswerBuffer;

   void __fastcall FTerminated ( TObject *Sender, COMERROR error );
   void __fastcall FRead ( TObject *Sender, int ReadBytes );
};

#endif
