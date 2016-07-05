//---------------------------------------------------------------------------
//		Класс для работы со ШТРИХ-ФР-К. Работает с версией протокола 1.5
//		Дата создания: 			20.12.2004
//		Дата редактирования: 	22.12.2004
//		Автор: 						
//---------------------------------------------------------------------------

#ifndef ShtrihFRH
#define ShtrihFRH

#include "ShtrihProtocol.h"
#include "StdCommPort.h"
#include <vector>
//---------------------------------------------------------------------------

class CShtrihFR
{
public:

	/*Флаги ККМ*/
   struct FLAGS
   {
   	bool OperationMagazine;		//рулон операционного журнала
      bool CheakTape;				//рулон чековой ленты
      bool HightSensorUpDoc;		//верхний датчик подкладного документа
      bool LowSensorUpDoc;			//нижний датчик подкладного документа
      bool FloatingPoint;			//положение дес.точки(0 - 0 знаков, 1 - 2 знака)
      bool EKLZ;						//наличие эклз
      bool OpticalSensorOperationMagazine;	//опт. датчик опер. журнала
      bool OpticalSensorCheakTape;	//опт. датчик чековой ленты
      bool LeverTermogolovkiControlTape;		//Рычаг термоголовки
		bool LeverTermogolovkiCheakTape;
      bool Cover;						//крышка корпуса
      bool MoneyBox;					//денежный ящик
      bool ErrorRightSensorPrinter;	//отказ правого датчика принтера
      bool ErrorLeftSensorPrinter;	//отказ левого датчика принтера
      bool EKLZIfFull;				//эклз почти заполнена
      bool HightPrecission;		//увеличенная точность
   };

   /*типы режимов*/
   enum REGIM {
      regPrinterIsWork = 0,		//принтер в рабочем состоянии
      regDataOut = 1,				//выдача данных
      regShiftIsOpen = 2,			//открытая смена, 24 часа не кончились
      regShiftIsOpenTimeOver = 3,//открытая смена, 24 часа кончились
      regShiftIsClose = 4,			//закрытая смена
      regBlockWithBadNalogPassword = 5,//блокировка по неправильному паролю нал. инспектора
      regWaitCommitSetDate = 6,	//ожидание подтверждения ввода даты
      regChangeFloatingPoint = 7,//разрешение изменения положения дес. точки
      regOpenDoc = 8,				//открытый документ
      regRegimMayTechnicalZero = 9,//режим разрешения технологического обнуления
		regTest = 10,					//тестовый прогон
      regPrintFullFiskalReport = 11,//печать полного финансового отчёта
      regPrintReportEKLZ = 12,	//печать отчёта ЭКЛЗ
      regWorkWithUnderDoc = 13,	//работа с фискальным подкладным документом
      regPrintUnderDoc = 14		//печать подкладного документа
   	 };

   /*Режим ФР*/
   struct REGIM_FR
   {
   	REGIM Regim;				//режим
      BYTE RegimState;			//статус режима
   };

   /*Под режим ФР*/
   enum SUBREGIM_FR
   {
		sregWait = 0,				//ФР в состоянии ожидания
      sregWaitPaperOut = 1,   //ФР в состоянии ожидания, отсутствует бумага
      sregPrintPaperOut = 2,	//ФР в состоянии печати, но кончилась бумага
      sregWaitPrintContinue = 3,//ФР после окончания бумаги, ждёт команду продолжения печати
      sregPrintFullFiskalReport = 4, //ФР печатает полные фискальные отчёты
      sregPrint = 5				//ФР в состоянии печати
   };

	/*структура для состояния ККМ*/
   struct MAIN_STATE
   {
   	WORD 				Flags;				//Флаги ФР
      REGIM_FR 		Regim;   			//Режим ФР
      SUBREGIM_FR		SubRegim;			//Под режим ФР
      WORD 				CountOper;			//количество операций в чеке
      BYTE				UBattary;			//Напряжение резервной баттарейки
      											//Значение получается делением на 9
      BYTE				UPower;				//Напряжение истояника питания
      											//Значение получается делением на 9
   	BYTE				ErrorCodeFP;		//Код ошибки ФП
      BYTE				ErrorCodeEKLZ;		//Код ошибки ЭКЛЗ
      
   };

	/*Конструктор*/
	CShtrihFR ( AnsiString CommName, DWORD BaudRate );

   /*Деструктор*/
	~CShtrihFR ( void );

	/*поиск устройства*/
   bool FindDevice ( CShtrihProtocol::DEVICE_TYPE *DevType, DWORD *BaudRate );

   /*Получение ком порта*/
   CStdCommPort* GetCommPort ( void )
   {
   	return m_pComm;
   };

	enum ERROR_TYPE
   {
		ERR_SHORTANSWER = 0x100			//в ответе не обнаружены необходимые данные
   };

	/*команда суточный отчёт без гашения
   out: порядковый номер оператора*/
   bool XReport ( BYTE *OperatorNumber = NULL );

	/*команда суточный отчёт c гашенbtv
   out: порядковый номер оператора*/
   bool ZReport ( BYTE *OperatorNumber = NULL );

 	/*команда суточный отчёт c гашенbtv
   out: порядковый номер оператора*/
   bool ReportBySection ( BYTE *OperatorNumber = NULL );

   /*Отрезка чека
   in: Full - полная или не полная отрезка чека*/
   bool CutCheak ( bool Full, BYTE *OperatorNumber = NULL );

   /*Краткий запрос состояния ФР
   out: State - краткое состояние ККМ*/
   bool GetCutState ( MAIN_STATE *State, BYTE *OperatorNumber = NULL );

   /*Печать строки
   bCheakType - печать на чековой ленте
   bCtrlType - печать на контрольной ленте
   String - строка для печати*/
   bool PrintString ( bool bCheakTape, bool bCtrlTape, AnsiString String,
   	BYTE *OperatorNumber = NULL );

   /*Протяжка документа
   bCheakType - печать на чековой ленте
   bCtrlType - печать на контрольной ленте
   Count - количество строк для протяжки*/
	bool ListenTape ( bool bCheakTape, bool bCtrlTape, BYTE Count,
   	BYTE *OperatorNumber = NULL );

   /*Выплата
   in: Summ - сумма выплаты
   out: CheakNumber - сквозной номер документа*/
   bool DoPayment ( __int64 Summ, WORD *CheakNumber, BYTE *OperatorNumber = NULL );


   /*Продажа*/
   bool Sale ( __int64 Amount, __int64 Price, BYTE DepartNumber,
   	AnsiString Text, BYTE *OperatorNumber = NULL );

   /*Возврат продажи*/
	bool ReturnSale ( __int64 Amount, __int64 Price, BYTE DepartNumber,
   	AnsiString Text, BYTE *OperatorNumber = NULL );

   /*Сторно*/
   bool Storno ( __int64 Amount, __int64 Price, BYTE DepartNumber,
   	AnsiString Text, BYTE *OperatorNumber = NULL );

   /*Закрытие чека*/
   bool CloseTicket ( __int64 Summ, AnsiString Text, __int64 *Change,
   	BYTE *OperatorNumber = NULL );

   /*Закрытие чека*/
   bool CloseTicket ( __int64 Summ1, __int64 Summ2, __int64 Summ3, __int64 Summ4,
   	AnsiString Text, __int64 *Change, BYTE *OperatorNumber = NULL );

   /*аннулирование чека*/
   bool CancelTicket ( BYTE *OperatorNumber = NULL );

   enum TICKET_TYPE { ticSale = 0, ticBay = 1, ticReturnSale = 2, ticReturnBay = 3};

   /*открыть чек*/
   bool OpenTicket ( TICKET_TYPE type, BYTE *OperatorNumber = NULL );

   /*команда продолжения печати*/
   bool ContinuePrint ( BYTE *OperatorNumber = NULL );

	/*Повтор документа*/
   bool CopyDoc ( BYTE *OperatorNumber = NULL );

   bool GetDeviceType ( CShtrihProtocol::DEVICE_TYPE *dev );

   /*получения номера документа*/
   bool GetCheackNumber ( WORD *number );

   int GetError ( void )
   {
   	return m_ErrorCode;
   };

   AnsiString GetErrorStr ( void );

   bool IsWorking ( void )
   {
   	return m_bWorking;
   };

protected:

	/*выполнение операции
   in: CommandNumber - номер команды
   	datain - входящие параметры команды
   out: answer - параметры ответа
   Возвращается true если всё успешно*/
   bool ExecuteCommand ( BYTE CommandNumber, std::vector<BYTE> datain,
   	std::vector<BYTE> &answer );

   /*Выполняет команду и читает ответ состоящий только из номера оператора*/
   bool ExecuteSimpleCommand ( BYTE CommandNumber, std::vector<BYTE> datain,
   	std::vector<BYTE> &answer, BYTE *OperatorNumber = NULL );

private:

	CStdCommPort *m_pComm;				//ком порт
   CShtrihProtocol *m_pProt;			//протокол обмена
   BYTE m_Password[4];

   int m_ErrorCode;
   bool m_bWorking;
};

#endif
