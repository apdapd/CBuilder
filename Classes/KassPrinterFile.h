//---------------------------------------------------------------------------

#ifndef KassPrinterFileH
#define KassPrinterFileH
//---------------------------------------------------------------------------

#include "KassPrinter.h"

class CKassPrinterFile : public CKassPrinter
{
public:
   CKassPrinterFile ( AnsiString Name );
   __fastcall ~CKassPrinterFile ( void );

   virtual bool LptPrint ( TStrings *List );
   virtual bool IsPrintWorking ( void );

   virtual void __fastcall Execute ( void );
protected:
private:
   HANDLE File;
   static int ModuleNumber;
   int	m_SleepTime;				//����� ��������
   bool m_bOverlapped;		//���������, ������������ �� ��������������� ���� �����
};

#endif
 