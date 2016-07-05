//---------------------------------------------------------------------------

#ifndef ErrInfoH
#define ErrInfoH
//---------------------------------------------------------------------------

struct TErrInfo
{
    int UUnum;          // Module number
    int ErrCode;        // Errr number
    int Kind;           // Error kind
    AnsiString ErrText; // Error text

   //получение описания ошибки, по коду возвращённому функцией GetLastError
   static AnsiString WinErrorText(int ErrNo);
};

#endif
 