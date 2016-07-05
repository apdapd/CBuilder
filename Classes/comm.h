///// comm.h
/////     purpose : prototypes for for TCommPort, serial communictaions API encapsulation
/////    copyright: Harold Howe, bcbdev.com 1996-1999.
/////    notice   : This file provides an object that encapsulates the win32 serial port routines.
/////               This file may be distributed and used freely for program development,
/////               but it may not be resold as part of a software development library.
/////               In other words, don't take the source and attempt to sell it to other developers.
#ifndef COMM_H
#define COMM_H

#include <windows.h>
//#include <string>
#include <vcl.h>
#include <exception>

// When the comm port class encounters an error, it throws an ECommError exception.
// The Error member of the exception object describes what went wrong. Some of the
// items should never happen. Others are fairly common place. You should pay special
// attention to the OPEN_ERROR type. This is the error that occurs when opening the port
// fails because another app already has the port open.
class ECommError : std::exception
{
public:
    enum ErrorType
    {
        BAD_SERIAL_PORT    ,
        BAD_BAUD_RATE      ,     //ошибка установки бит рэйта
        BAD_PORT_NUMBER    ,
        BAD_STOP_BITS      ,     //ошибка установки стоп битов
        BAD_PARITY         ,     //ошибка установки чётности
        BAD_BYTESIZE       ,     //ошибка установки размера бита
        PORT_ALREADY_OPEN  ,     //ошибка повторного открытия ком порта
        PORT_NOT_OPEN      ,     //ошибка доступа к не открытому ком порту
        OPEN_ERROR         ,     //ошибка открытия ком порта
        WRITE_ERROR        ,     //ошибка записи в ком порт
        READ_ERROR         ,     //ошибка чтения из ком порта
        CLOSE_ERROR        ,     //ошибка закрытия ком порта
        PURGECOMM          ,     //ошибка очистки ком порта
        FLUSHFILEBUFFERS   ,     //ошибка сброса буферов ком порта
        GETCOMMSTATE       ,     //ошибка получения статуса ком порта
        SETCOMMSTATE       ,     //ошибка установки статуса ком порта
        SETUPCOMM          ,     //ошибка функции SetupComm
        SETCOMMTIMEOUTS    ,     //ошибка учтановки таймаутов
        CLEARCOMMERROR           //ошибка очистки ошибки ком порта
    };

    ECommError( ErrorType error);
    ECommError(void);

    ErrorType Error;
    DWORD     Errno;   // Errno == return value from GetLastError. Can be used with FormatMessage

   virtual const char *what( ) const
   {
      return m_ErrorStr.c_str();
   };

   private:
      AnsiString m_ErrorStr;
};

class TCommPort
{
  public:
    TCommPort();
    ~TCommPort();
    void OpenCommPort(void);
    void CloseCommPort(void);

    void SetCommPort(const AnsiString port);
    AnsiString GetCommPort(void);
    void SetBaudRate(unsigned int newBaud);
    unsigned int GetBaudRate(void);
    void SetParity(BYTE newParity); // see source for possible values
    BYTE GetParity(void);
    void SetByteSize(BYTE newByteSize);
    BYTE GetByteSize(void);
    void SetStopBits(BYTE newStopBits);
    BYTE GetStopBits(void);

    void SetCommDCBProperties(DCB &properties);  // avoid using DCB interface
    void GetCommDCBProperties(DCB &properties);  // Use SetBaudRate et al instead

    void GetCommProperties(COMMPROP &properties);

    void SetCommTimeouts ( COMMTIMEOUTS  nm_TimeOuts );

    void WriteString(const char *outString);
    void WriteBuffer(BYTE  *buffer, unsigned int ByteCount);
    void WriteBufferSlowly(BYTE *buffer, unsigned int ByteCount);
    int ReadString(char *string, unsigned int MaxBytes);
    int ReadBytes(BYTE *bytes, unsigned int byteCount);
    void DiscardBytes(unsigned int MaxBytes);
    void PurgeCommPort(void);
    void FlushCommPort(void);

    void  PutByte(BYTE value);
    BYTE  GetByte();
    unsigned int BytesAvailable(void);

    bool GetConnected()
    {
        return m_CommOpen;
    }

    HANDLE GetHandle() // allow access to the handle in case the user needs to
    {                  // do something hardcore. Avoid this if possible
        return m_hCom;
    }

  private:
    // Note: the destructor of the commport class automatically closes the
    //       port. This makes copy construction and assignment impossible.
    //       That is why I privatize them, and don't define them. In order
    //       to make copy construction and assignment feasible, we would need
    //       to employ a reference counting scheme.
    TCommPort(const TCommPort &);            // privatize copy construction
    TCommPort & operator=(const TCommPort&);  // and assignment.

    void VerifyOpen()
    {
        if(!m_CommOpen)
            throw ECommError(ECommError::PORT_NOT_OPEN) ;
    }
    void VerifyClosed()
    {
        if(m_CommOpen)
            throw ECommError(ECommError::PORT_ALREADY_OPEN) ;
    }

  // this stuff is private because we want to hide these details from clients
    bool          m_CommOpen;
    COMMTIMEOUTS  m_TimeOuts;
    AnsiString   m_CommPort;
    DCB           m_dcb;        // a DCB is a windows structure used for configuring the port
    HANDLE        m_hCom;       // handle to the comm port.
};

#endif
