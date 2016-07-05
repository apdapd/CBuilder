//---------------------------------------------------------------------------
//					Класс для работы с ФР Прим08ТК в кассе
//					Дата редактирования:			30.11.2005
//					Автор:						
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

   /*печать чека*/
   virtual void KKMPrint( CashGoods * ThisGoods );

   /*произвольная печать*/
   virtual bool LptPrint ( TStrings *List );

   virtual void CopyTicket ( void );
   
   virtual bool IsPrintWorking ( void );

   virtual bool SearchKKM ( void );
   virtual bool SearchVersionKKM ( DWORD BaudRate );
   virtual bool TestKKM ( void );

   /*запрашивает информацию о версии ПО и возвращает строку с информацией*/
   virtual AnsiString GetVersionStr ( void );

protected:

   /*начало работы*/
   virtual void InitWork ( void );

   /*выполнение z отчёта*/
   virtual void ZReportWork ( void );

   /*выполнение x отчёта*/
   virtual void XReportWork ( void );

   /*формирует информацию об ошибке*/
   void FormErrInfo ( TErrInfo *pErrInfo );

private:
   CPrim08TK *m_pPrim;

   void SetState ( void );
};

#endif
