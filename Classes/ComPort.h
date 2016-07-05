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

    //����
	TCommPort ComPort;
//	ComName FComName;
    //��� �����
    AnsiString FComName;

    //�������� ����������
    bool __fastcall GetConnected ( void );

    //���������/��������� ����� �����
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

    //��������� ����� ����������� ����
    WORD __fastcall GetCountByteInBuffer ( void );

    TTerminated FTerminated;
    TRead FRead,
    	FNoRead;

    COMERROR ce;

public:
    //��������� ����������� �����
    HANDLE GetHandle ( void );

protected:
public:
	__fastcall TComPort();
    __fastcall ~TComPort ( void );

    //�������/�������
    virtual void Open ( void );
    virtual void Close ( void );

    //�������� ������
    void WriteData ( void *Data, int Size );
    //��������� ������
    int ReadData ( void *Data, int Size );
    void DiscardBytes(unsigned int MaxBytes);

    //�������� ����
    void WriteByte ( BYTE Data );
    //��������� ����
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
    //��������� ��� �����
    __property AnsiString ComNameStr={read=GetComName, write=SetComName};
    __property AnsiString ComName={read=GetComName, write=SetComName};
    __property BYTE *LastRead={read=FLastRead};
    //����� ��� ������
    __property BYTE *Buffer={read=FBuffer, write=FBuffer};
    //���� ������
    __property bool IsStartRead={read=bContinue};
    //����� ����������� ����
    __property DWORD CountByteInBuffer={read=GlobPos};
    //���������� ������
    __property HANDLE ThreadHandle={read=hThread};

    //������ ������
    virtual void StartRead ( void );
    //��������� ������
    virtual COMERROR RoutineProc ( void );
    //��������� ���������� ������
    virtual void EndRead ( void );

    COMERROR GetError ( void );

//__published:
	//����
//	__property ComName Port={read=FComName, write=FComName};
    //����������
    __property bool Connected={read=GetConnected};
    //������ ������
    __property DWORD BufferSize={read=FBufferSize, write=FBufferSize};
    //���� �� ���� ���������
    __property DWORD ReadBufferSize={read=FReadSize, write=FReadSize};
    //����� �������� ��� ��������� ������
    __property WORD WaitTime={read=FWaitTime, write=FWaitTime};
    //������������ ���������� ��������� ������
    __property WORD BreakCount={read=FBreakCount, write=FBreakCount};
    __property WORD StartBreakCount = {read=FStartBreakCount, write = FStartBreakCount};

    //����� ������
    __property TTerminated OnTerminated={read=FTerminated, write=FTerminated};
    //������� ������
    __property TRead OnRead={read=FRead, write=FRead};
    //��������� ������
    __property TRead OnNoRead={read=FNoRead, write=FNoRead};
};

LONG ReadProc ( TComPort *Port );

//---------------------------------------------------------------------------
#endif
