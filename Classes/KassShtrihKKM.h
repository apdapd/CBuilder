//---------------------------------------------------------------------------
//					����� ��� ������ � �����-�� � �����
//					���� ��������:             21.12.2004
//					���� ��������������:			22.12.2004
//					�����:						
//---------------------------------------------------------------------------

#ifndef KassShtrihKKMH
#define KassShtrihKKMH
//---------------------------------------------------------------------------

#include "KassKKM.h"
#include "ShtrihFR.h"

class CKassShtrihKKM : public CKassKKM
{
public:

	CKassShtrihKKM ( TComponent *Owner, AnsiString CommName, DWORD BaudRate )
      throw ( ECommError );
   virtual __fastcall ~CKassShtrihKKM ( void );

   void OpenCommPort();

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

   //������� ������������ ������ ������
   void SetTextWidth ( BYTE TextWidth );

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

	CShtrihFR *m_pKKM;

   //����������� ���������� ����� ������ ��� ������ �� ���������
   enum {STRING_MAX_LEN = 50};

   //����������� ���������� ����� ������ ��� ������
   BYTE m_StringMaxLen;

   //�������� ��������� ������ �� ��������
   bool WaitEndOfPrint ( CShtrihFR::SUBREGIM_FR &SubRegim );

   //����������� ������ �����
   void AlignText ( TStrings *Text );

   void Show ( TStrings *Text );

   /*�������������� ������ ������*/
   BYTE ConvertDepartNumber ( BYTE DepNumber );
};

#endif
