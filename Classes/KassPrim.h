//---------------------------------------------------------------------------
//					����� ��� ������ � �� ����08�� � �����
//					���� ��������������:			30.11.2005
//					�����:						
//---------------------------------------------------------------------------

#ifndef KassPrimH
#define KassPrimH
//---------------------------------------------------------------------------

#include "KassKKM.h"
#include "Prim08TK.h"

class CKassPrim  : public CKassKKM
{
public:
   CKassPrim (  TComponent *Owner, AnsiString CommName, DWORD BaudRate )
      throw ( ECommError );
   virtual __fastcall ~CKassPrim ( void );

   /*������ ����*/
   virtual void KKMPrint( CashGoods * ThisGoods );

   /*������������ ������*/
   virtual bool LptPrint ( TStrings *List );

   virtual void CopyTicket ( void );
   
   virtual bool IsPrintWorking ( void );

   virtual bool SearchKKM ( void );
   virtual bool SearchVersionKKM ( DWORD BaudRate );
   virtual bool TestKKM ( void );

   /*����������� ���������� � ������ �� � ���������� ������ � �����������*/
   virtual AnsiString GetVersionStr ( void );

protected:

   /*������ ������*/
   virtual void InitWork ( void );

   /*���������� z ������*/
   virtual void ZReportWork ( void );

   /*���������� x ������*/
   virtual void XReportWork ( void );

   /*��������� ���������� �� ������*/
   void FormErrInfo ( TErrInfo *pErrInfo );

private:
   CPrim08TK *m_pPrim;

   void SetState ( void );
};

#endif
