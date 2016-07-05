//---------------------------------------------------------------------------
//					Класс для работы с Штрих-ФР в кассе
//					Дата создания:             21.12.2004
//					Дата редактирования:			22.12.2004
//					Автор:						
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

   //задание максимальное ширины строки
   void SetTextWidth ( BYTE TextWidth );

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

	CShtrihFR *m_pKKM;

   //максимально допустимая длина строки для печати по умолчания
   enum {STRING_MAX_LEN = 50};

   //максимально допустимая длина строки для печати
   BYTE m_StringMaxLen;

   //ожидания окончания печати на принтере
   bool WaitEndOfPrint ( CShtrihFR::SUBREGIM_FR &SubRegim );

   //выравнивает список строк
   void AlignText ( TStrings *Text );

   void Show ( TStrings *Text );

   /*преобразование номера отдела*/
   BYTE ConvertDepartNumber ( BYTE DepNumber );
};

#endif
