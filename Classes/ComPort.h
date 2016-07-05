//---------------------------------------------------------------------------

#ifndef ComPortH
#define ComPortH
//---------------------------------------------------------------------------
//#include <SysUtils.hpp>
//#include <Controls.hpp>
#include <Classes.hpp>
//#include <Forms.hpp>

#include "comm.h"
//---------------------------------------------------------------------------
enum ComName {COM1, COM2, COM3, COM4,
	COM5, COM6, COM7, COM8, LPT1, LPT2, LPT3, LPT4};

enum COMERROR { ceOk, ceTimeUp, ceTerminated, ceOutOfData };

typedef void __fastcall (__closure *TTerminated)(System::TObject *Sender, COMERROR error);
typedef void __fastcall (__closure *TRead)(System::TObject *Sender, int ReadBytes);

class PACKAGE TComPort : public TObject// : public TComponent
{
private:

    //порт
	TCommPort ComPort;
//	ComName FComName;
    //имя порта
    AnsiString FComName;

    //проверка соединения
    bool __fastcall GetConnected ( void );

    //получение/установка имени порта
    AnsiString __fastcall GetComName ( void );
    void __fastcall SetComName ( AnsiString Name );

    BYTE *FBuffer;
    BYTE *FLastRead;
    DWORD FBufferSize;
    DWORD FReadSize;
    DWORD GlobPos;

    WORD FWaitTime,
    	FBreakCount,
      FStartBreakCount,
        CurBreakCount;

    bool bContinue;

    HANDLE hThread;
    int ThreadId;

    //получение числа прочитанных байт
    WORD __fastcall GetCountByteInBuffer ( void );

    TTerminated FTerminated;
    TRead FRead,
    	FNoRead;

    COMERROR ce;

public:
    //получение дескриптора порта
    HANDLE GetHandle ( void );

protected:
public:
	__fastcall TComPort();
    __fastcall ~TComPort ( void );

    //открыть/закрыть
    virtual void Open ( void );
    virtual void Close ( void );

    //записать данные
    void WriteData ( void *Data, int Size );
    //прочитать данные
    int ReadData ( void *Data, int Size );
    void DiscardBytes(unsigned int MaxBytes);

    //записать байт
    void WriteByte ( BYTE Data );
    //прочитать байт
    BYTE ReadByte ( void );

    void SetCommDCBProperties(DCB &properties);
    void GetCommDCBProperties(DCB &properties);
    void SetCommTimeounts ( COMMTIMEOUTS  nm_TimeOuts );

    void __fastcall SetBaudRate(unsigned int newBaud);
    unsigned int __fastcall  GetBaudRate(void);
    void __fastcall  SetParity(BYTE newParity);
    BYTE __fastcall  GetParity(void);
    void __fastcall  SetByteSize(BYTE newByteSize);
    BYTE __fastcall  GetByteSize(void);
    void __fastcall  SetStopBits(BYTE newStopBits);
    BYTE __fastcall  GetStopBits(void);

    __property HANDLE Handle={read=GetHandle};
    //текстовое имя порта
    __property AnsiString ComNameStr={read=GetComName, write=SetComName};
    __property AnsiString ComName={read=GetComName, write=SetComName};
    __property BYTE *LastRead={read=FLastRead};
    //буфер для чтения
    __property BYTE *Buffer={read=FBuffer, write=FBuffer};
    //если читает
    __property bool IsStartRead={read=bContinue};
    //число прочитанных байт
    __property DWORD CountByteInBuffer={read=GlobPos};
    //дескриптор потока
    __property HANDLE ThreadHandle={read=hThread};

    //начало чтения
    virtual void StartRead ( void );
    //процедура чтения
    virtual COMERROR RoutineProc ( void );
    //насильное завершения чтения
    virtual void EndRead ( void );

    COMERROR GetError ( void );

//__published:
	//порт
//	__property ComName Port={read=FComName, write=FComName};
    //активность
    __property bool Connected={read=GetConnected};
    //размер буфера
    __property DWORD BufferSize={read=FBufferSize, write=FBufferSize};
    //байт за одно прочтение
    __property DWORD ReadBufferSize={read=FReadSize, write=FReadSize};
    //время ожидания при неудачним чтении
    __property WORD WaitTime={read=FWaitTime, write=FWaitTime};
    //максимальное количество неудачных чтений
    __property WORD BreakCount={read=FBreakCount, write=FBreakCount};
    __property WORD StartBreakCount = {read=FStartBreakCount, write = FStartBreakCount};

    //Конец чтения
    __property TTerminated OnTerminated={read=FTerminated, write=FTerminated};
    //удачное чтение
    __property TRead OnRead={read=FRead, write=FRead};
    //неудачное чтение
    __property TRead OnNoRead={read=FNoRead, write=FNoRead};
};

LONG ReadProc ( TComPort *Port );

//---------------------------------------------------------------------------
#endif
