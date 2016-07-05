//---------------------------------------------------------------------------

#include <vcl.h>
#include <iterator>
#pragma hdrstop

#include "ShtrihFR.h"
#include "shtrih_func.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

using namespace std;
using namespace shtrih_func;

/*Конструктор*/
CShtrihFR::CShtrihFR ( AnsiString CommName, DWORD BaudRate )
{
	m_bWorking = false;

	m_pComm = new CStdCommPort;
   m_pComm->SetCommPort ( CommName );
   CShtrihProtocol::ConfigurePort ( m_pComm );
   m_pComm->SetBaudRate ( BaudRate );

   m_Password[0] = 0x1E;
   m_Password[1] = 0;
   m_Password[2] = 0;
   m_Password[3] = 0;

   m_pProt = new CShtrihProtocol ( m_pComm );
}

/*Деструктор*/
CShtrihFR::~CShtrihFR ( void )
{
	delete m_pProt;

   delete m_pComm;
}

/*поиск устройства*/
bool CShtrihFR::FindDevice ( CShtrihProtocol::DEVICE_TYPE *DevType, DWORD *BaudRate )
{
	int err;
   if ( m_pProt->FindDeviceType ( &err, DevType, BaudRate ) )
   {
   	return true;
   }else
   {
   	if ( err == 0 )
		   m_ErrorCode = -1 * (int)m_pProt->GetError();
      else
      	m_ErrorCode = err;
      return false;
   }
}

/*команда суточный отчёт без гашения
out: порядковый номер оператора*/
bool CShtrihFR::XReport ( BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   return ExecuteSimpleCommand ( 0x40, data, answer, OperatorNumber );
}

/*команда суточный отчёт c гашенbtv
out: порядковый номер оператора*/
bool CShtrihFR::ZReport ( BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   return ExecuteSimpleCommand ( 0x41, data, answer, OperatorNumber );
}

/*команда суточный отчёт c гашенbtv
out: порядковый номер оператора*/
bool CShtrihFR::ReportBySection ( BYTE *OperatorNumber/* = NULL*/ )
{
	vector<BYTE> data, answer;
   return ExecuteSimpleCommand ( 0x42, data, answer, OperatorNumber );
}

/*Отрезка чека
in: Full - полная или не полная отрезка чека*/
bool CShtrihFR::CutCheak ( bool Full, BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   if ( Full )
	   data.push_back ( 0 );
   else
   	data.push_back ( 1 );

   return ExecuteSimpleCommand ( 0x25, data, answer, OperatorNumber );
}

/*Краткий запрос состояния ФР
out: State - краткое состояние ККМ*/
bool CShtrihFR::GetCutState ( MAIN_STATE *State, BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   if ( ExecuteCommand ( 0x10, data, answer ) )
   {
   	if ( answer.size() < 14 )
      {
      	m_ErrorCode = ERR_SHORTANSWER;
         return false;
      }

      vector<BYTE>::iterator iter = answer.begin();
      if ( OperatorNumber )
      	*OperatorNumber = *iter;

      iter++;
		iter = ReadValueFromAnswer ( &(State->Flags), iter, 2 );
      BYTE val;
      iter = ReadValueFromAnswer ( &val, iter, 1 );
      State->Regim.Regim = (REGIM)(val & 0xF);
      State->Regim.RegimState = ( val >> 4 );


      iter = ReadValueFromAnswer ( &val, iter, 1 );
      State->SubRegim = (SUBREGIM_FR)val;
      iter = ReadValueFromAnswer ( &(State->CountOper), iter, 1 );
      iter = ReadValueFromAnswer ( &(State->UBattary), iter, 1 );
      iter = ReadValueFromAnswer ( &(State->UPower), iter, 1 );
      iter = ReadValueFromAnswer ( &(State->ErrorCodeFP), iter, 1 );
      iter = ReadValueFromAnswer ( &(State->ErrorCodeEKLZ), iter, 1 );
      ReadValueFromAnswer ( &val, iter, 1 );
      State->CountOper += 256 * val;
      return true;
   };

   return false;
}

/*Печать строки
bCheakType - печать на чековой ленте
bCtrlType - печать на контрольной ленте
String - строка для печати*/
bool CShtrihFR::PrintString ( bool bCheakTape, bool bCtrlTape, AnsiString String,
  	BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   BYTE Flags;
   if ( bCheakTape )
   	Flags = Flags | 2;
   if ( bCtrlTape )
   	Flags = Flags | 1;
   data.push_back ( Flags );
/*	int l = min ( String.Length(), 40 );
   copy ( String.c_str(), String.c_str() + l, back_inserter ( data ) );
   if ( l < 40 )
   {
   	for ( int i = 0; i < 40 - l; i++ )
      	data.push_back ( 0 );
   }*/
   AddStringToVector ( back_inserter ( data ), String, 40 );

   return ExecuteSimpleCommand ( 0x17, data, answer, OperatorNumber );
}

/*Протяжка документа
bCheakType - печать на чековой ленте
bCtrlType - печать на контрольной ленте
Count - количество строк для протяжки*/
bool CShtrihFR::ListenTape ( bool bCheakTape, bool bCtrlTape, BYTE Count,
  	BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   BYTE Flags;
   if ( bCheakTape )
   	Flags = Flags | 2;
   if ( bCtrlTape )
   	Flags = Flags | 1;
   data.push_back ( Flags );
   data.push_back ( Count );
   return ExecuteSimpleCommand ( 0x29, data, answer, OperatorNumber );
}

/*Выплата
in: Summ - сумма выплаты
out: CheakNumber - сквозной номер документа*/
bool CShtrihFR::DoPayment ( __int64 Summ, WORD *CheakNumber,
	BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   AddValueToVector ( back_inserter ( data ), Summ, 5 );
   if ( ExecuteCommand ( 0x51, data, answer ) )
   {
   	if ( answer.size() < 3 )
      {
      	m_ErrorCode = ERR_SHORTANSWER;
         return false;
      }

      vector<BYTE>::iterator iter = answer.begin();
      if ( OperatorNumber )
      	*OperatorNumber = *iter;
      iter++;

      ReadValueFromAnswer ( CheakNumber, iter, 2 );
      return true;
   }

   return false;
}

/*Продажа*/
bool CShtrihFR::Sale ( __int64 Amount, __int64 Price, BYTE DepartNumber,
  	AnsiString Text, BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   AddValueToVector ( back_inserter(data), Amount, 5 );
   AddValueToVector ( back_inserter(data), Price, 5 );
   data.push_back ( DepartNumber );
   data.push_back ( 0 );
   data.push_back ( 0 );
   data.push_back ( 0 );
   data.push_back ( 0 );
   AddStringToVector ( back_inserter ( data ), Text, 40 );
   return ExecuteSimpleCommand ( 0x80, data, answer, OperatorNumber );
}

bool CShtrihFR::ReturnSale ( __int64 Amount, __int64 Price, BYTE DepartNumber,
  	AnsiString Text, BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   AddValueToVector ( back_inserter(data), Amount, 5 );
   AddValueToVector ( back_inserter(data), Price, 5 );
   data.push_back ( DepartNumber );
   data.push_back ( 0 );
   data.push_back ( 0 );
   data.push_back ( 0 );
   data.push_back ( 0 );
   AddStringToVector ( back_inserter ( data ), Text, 40 );
   return ExecuteSimpleCommand ( 0x82, data, answer, OperatorNumber );
}

/*Сторно*/
bool CShtrihFR::Storno ( __int64 Amount, __int64 Price, BYTE DepartNumber,
  	AnsiString Text, BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   AddValueToVector ( back_inserter(data), Amount, 5 );
   AddValueToVector ( back_inserter(data), Price, 5 );
   data.push_back ( DepartNumber );
   data.push_back ( 0 );
   data.push_back ( 0 );
   data.push_back ( 0 );
   data.push_back ( 0 );
   AddStringToVector ( back_inserter ( data ), Text, 40 );
   return ExecuteSimpleCommand ( 0x84, data, answer, OperatorNumber );
}

/*Закрытие чека*/
bool CShtrihFR::CloseTicket ( __int64 Summ, AnsiString Text, __int64 *Change,
  	BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   AddValueToVector ( back_inserter(data), Summ, 5 );
   AddValueToVector ( back_inserter(data), 0, 5 );
   AddValueToVector ( back_inserter(data), 0, 5 );
   AddValueToVector ( back_inserter(data), 0, 5 );
   AddValueToVector ( back_inserter(data), 0, 2 );
   data.push_back ( 0 );
   data.push_back ( 0 );
   data.push_back ( 0 );
   data.push_back ( 0 );
   AddStringToVector ( back_inserter ( data ), Text, 40 );
   if ( ExecuteCommand ( 0x85, data, answer ) )
   {
   	if ( answer.size() < 6 )
      {
      	m_ErrorCode = ERR_SHORTANSWER;
         return false;
      }

      vector<BYTE>::iterator iter = answer.begin();
      if ( OperatorNumber )
      	*OperatorNumber = *iter;
      iter++;

      ReadValueFromAnswer ( Change, iter, 5 );
      return true;
   }
   return false;
}

/*Закрытие чека*/
bool CShtrihFR::CloseTicket ( __int64 Summ1, __int64 Summ2, __int64 Summ3, __int64 Summ4,
  	AnsiString Text, __int64 *Change, BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   AddValueToVector ( back_inserter(data), Summ1, 5 );
   AddValueToVector ( back_inserter(data), Summ2, 5 );
   AddValueToVector ( back_inserter(data), Summ3, 5 );
   AddValueToVector ( back_inserter(data), Summ4, 5 );
   AddValueToVector ( back_inserter(data), 0, 2 );
   data.push_back ( 0 );
   data.push_back ( 0 );
   data.push_back ( 0 );
   data.push_back ( 0 );
   AddStringToVector ( back_inserter ( data ), Text, 40 );
   if ( ExecuteCommand ( 0x85, data, answer ) )
   {
   	if ( answer.size() < 6 )
      {
      	m_ErrorCode = ERR_SHORTANSWER;
         return false;
      }

      vector<BYTE>::iterator iter = answer.begin();
      if ( OperatorNumber )
      	*OperatorNumber = *iter;
      iter++;

      ReadValueFromAnswer ( Change, iter, 5 );
      return true;
   }
   return false;
}

/*аннулирование чека*/
bool CShtrihFR::CancelTicket ( BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   return ExecuteSimpleCommand ( 0x88, data, answer, OperatorNumber );
}

/*открыть чек*/
bool CShtrihFR::OpenTicket ( TICKET_TYPE type, BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   data.push_back ( (BYTE)type );
   return ExecuteSimpleCommand ( 0x8D, data, answer, OperatorNumber );
}

/*команда продолжения печати*/
bool CShtrihFR::ContinuePrint ( BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   return ExecuteSimpleCommand ( 0xB0, data, answer, OperatorNumber );
}

/*Повтор документа*/
bool CShtrihFR::CopyDoc ( BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   return ExecuteSimpleCommand ( 0x8C, data, answer, OperatorNumber );
}

bool CShtrihFR::GetDeviceType ( CShtrihProtocol::DEVICE_TYPE *dev )
{
	int err;
   bool bok = m_pProt->GetDeviceType ( &err, dev );
   if ( bok )
   	if ( err == 0 )
      	return true;
      else
      	m_ErrorCode = err;
   return false;
}

/*получения номера документа*/
bool CShtrihFR::GetCheackNumber ( WORD *number )
{
	vector<BYTE> data, answer;
   if ( ExecuteCommand ( 0x11, data, answer ) )
   {
   	if ( answer.size() < 14 )
      {
      	m_ErrorCode = ERR_SHORTANSWER;
         return false;
      }

      vector<BYTE>::iterator iter = answer.begin();
      for ( int i = 0; i < 9; ++i )
         iter++;

      ReadValueFromAnswer ( number, iter, 2 );
      return true;
   }

   return false;
}

/*выполнение операции
in: CommandNumber - номер команды
  	datain - входящие параметры команды
out: answer - параметры ответа
Возвращается true если всё успешно*/
bool CShtrihFR::ExecuteCommand ( BYTE CommandNumber,
	std::vector<BYTE> datain, std::vector<BYTE> &answer )
{
	m_bWorking = true;
	vector<BYTE> send, readdata;
   copy ( m_Password, m_Password + 4, back_inserter ( send ) );
   copy ( datain.begin(), datain.end(), back_inserter ( send ) );
   if ( m_pProt->SendMessage ( CommandNumber, send ) )
   {
   	if ( m_pProt->WaitAnswer ( readdata ) )
      {
      	if ( readdata.empty() )
         {
         	m_ErrorCode = ERR_SHORTANSWER;
				m_bWorking = false;
            return false;
         }

         m_ErrorCode = readdata[0];

         copy ( readdata.begin() + 1, readdata.end(), back_inserter ( answer ) );
			m_bWorking = false;
         return (m_ErrorCode == 0);
      }
   }

   m_ErrorCode = -1 * (int)m_pProt->GetError();
	m_bWorking = false;
   return false;
}

/*Выполняет команду и читает ответ состоящий только из номера оператора*/
bool CShtrihFR::ExecuteSimpleCommand ( BYTE CommandNumber, std::vector<BYTE> datain,
  	std::vector<BYTE> &answer, BYTE *OperatorNumber )
{
   if ( ExecuteCommand ( CommandNumber, datain, answer ) )
   {
   	if ( answer.empty() )
      {
      	m_ErrorCode = ERR_SHORTANSWER;
         return false;
      }

      if ( OperatorNumber )
	      *OperatorNumber = answer[0];

      return true;
   }

   return false;
}

AnsiString CShtrihFR::GetErrorStr ( void )
{
	if ( m_ErrorCode < 0 )
   {
   	return m_pProt->GetErrorStr();
   }

	switch ( m_ErrorCode )
   {
   case 0: return "ФП:  Ошибок нет";
   case 1: return "ФП:  Неисправен накопитель ФП 1, ФП 2 или часы";
   case 2: return "ФП:  Отсутствует ФП";
   case 3: return "ФП:  Отсутствует ФП 2";
   case 4: return "ФП:  Некорректные параметры в команде обращения к ФП";
   case 5: return "ФП:  Нет запрошенных данных";
   case 6: return "ФП:  ФП в режиме вывода данных";
   case 7: return "ФП:  Некорректные параметры в команде для данной реализации ФП";
   case 8: return "ФП:  Команда не поддерживается в данной реализации ФП";
   case 9: return "ФП:  Некорректная длина команды";
   case 10: return "ФП:  Формат данных не BCD";
   case 11: return "ФП:  Неисправна ячейка памяти ФП при записи итога";
   case 17: return "ФП:  Не введена лицензия";
   case 18: return "ФП:  Заводской номер уже введен";
   case 19: return "ФП:  Текущая дата меньше даты последней записи в ФП";
   case 20: return "ФП:  Область сменных итогов ФП переполнена";
   case 21: return "ФП:  Смена уже открыта";
   case 22: return "ФП:  Смена не открыта";
   case 23: return "ФП:  Номер первой смены больше номера последней смены";
   case 24: return "ФП:  Дата первой смены больше даты последней смены";
   case 25: return "ФП:  Нет данных в ФП";
   case 26: return "ФП:  Область перерегистраций в ФП переполнена";
   case 27: return "ФП:  Заводской номер не введен";
   case 28: return "ФП:  В заданном диапазоне есть поврежденная запись";
   case 29: return "ФП:  Повреждена последняя запись сменных итогов";
   case 30: return "ФП:  Область перерегистраций ФП переполнена";
   case 31: return "ФП:  Отсутствует память регистров";
   case 32: return "ФП:  Переполнение денежного регистра при добавлении";
   case 33: return "ФП:  Вычитаемая сумма больше содержимого денежного регистра";
   case 34: return "ФП:  Неверная дата";
   case 35: return "ФП:  Нет записи активизации";
   case 36: return "ФП:  Область активизаций переполнена";
   case 37: return "ФП:  Нет активизации с запрашиваемым номером";
   case 38: return "ФР:  Вносимая клиентом сумма меньше суммы чека";
   case 43: return "ФР:  Невозможно отменить предыдущую команду";
   case 44: return "ФР:  Обнулённая касса (повторное гашение невозможно)";
   case 45: return "ФР:  Сумма чека по секции меньше суммы сторно";
   case 46: return "ФР:  В ФР нет денег для выплаты";
   case 48: return "ФР:  ФР заблокирован, ждет ввода пароля налогового инспектора";
   case 50: return "ФР:  Требуется выполнение общего гашения";
   case 51: return "ФР:  Некорректные параметры в команде";
   case 52: return "ФР:  Нет данных";
   case 53: return "ФР:  Некорректный параметр при данных настройках";
   case 54: return "ФР:  Некорректные параметры в команде для данной реализации ФР";
   case 55: return "ФР:  Команда не поддерживается в данной реализации ФР";
   case 56: return "ФР:  Ошибка в ПЗУ";
   case 57: return "ФР:  Внутренняя ошибка ПО ФР";
   case 58: return "ФР:  Переполнение накопления по надбавкам в смене";
   case 59: return "ФР:  Переполнение накопления в смене";
   case 60: return "ФР:  ЭКЛЗ: неверный регистрационный номер";
   case 61: return "ФР:  Смена не открыта . операция невозможна";
   case 62: return "ФР:  Переполнение накопления по секциям в смене";
   case 63: return "ФР:  Переполнение накопления по скидкам в смене";
   case 64: return "ФР:  Переполнение диапазона скидок";
   case 65: return "ФР:  Переполнение диапазона оплаты наличными";
   case 66: return "ФР:  Переполнение диапазона оплаты типом 2";
   case 67: return "ФР:  Переполнение диапазона оплаты типом 3";
   case 68: return "ФР:  Переполнение диапазона оплаты типом 4";
   case 69: return "ФР:  Cумма всех типов оплаты меньше итога чека";
   case 70: return "ФР:  Не хватает наличности в кассе";
   case 71: return "ФР:  Переполнение накопления по налогам в смене";
   case 72: return "ФР:  Переполнение итога чека";
   case 73: return "ФР:  Операция невозможна в открытом чеке данного типа";
   case 74: return "ФР:  Открыт чек . операция невозможна";
   case 75: return "ФР:  Буфер чека переполнен";
   case 76: return "ФР:  Переполнение накопления по обороту налогов в смене";
   case 77: return "ФР:  Вносимая безналичной оплатой сумма больше суммы чека";
   case 78: return "ФР:  Смена превысила 24 часа";
   case 79: return "ФР:  Неверный пароль";
   case 80: return "ФР:  Идет печать предыдущей команды";
   case 81: return "ФР:  Переполнение накоплений наличными в смене";
   case 82: return "ФР:  Переполнение накоплений по типу оплаты 2 в смене";
   case 83: return "ФР:  Переполнение накоплений по типу оплаты 3 в смене";
   case 84: return "ФР:  Переполнение накоплений по типу оплаты 4 в смене";
   case 85: return "ФР:  Чек закрыт . операция невозможна";
   case 86: return "ФР:  Нет документа для повтора";
   case 87: return "ФР:  ЭКЛЗ: количество закрытых смен не совпадает с ФП";
   case 88: return "ФР:  Ожидание команды продолжения печати";
   case 89: return "ФР:  Документ открыт другим оператором";
   case 90: return "ФР:  Скидка превышает накопления в чеке";
   case 91: return "ФР:  Переполнение диапазона надбавок";
   case 92: return "ФР:  Понижено напряжение 24В";
   case 93: return "ФР:  Таблица не определена";
   case 94: return "ФР:  Некорректная операция";
   case 95: return "ФР:  Отрицательный итог чека";
   case 96: return "ФР:  Переполнение при умножении";
   case 97: return "ФР:  Переполнение диапазона цены";
   case 98: return "ФР:  Переполнение диапазона количества";
   case 99: return "ФР:  Переполнение диапазона отдела";
   case 100: return "ФР:  ФП отсутствует";
   case 101: return "ФР:  Не хватает денег в секции";
   case 102: return "ФР:  Переполнение денег в секции";
   case 103: return "ФР:  Ошибка связи с ФП";
   case 104: return "ФР:  Не хватает денег по обороту налогов";
   case 105: return "ФР:  Переполнение денег по обороту налогов";
   case 106: return "ФР:  Ошибка питания в момент ответа по I2C";
   case 107: return "ФР:  Нет чековой ленты";
   case 108: return "ФР:  Нет контрольной ленты";
   case 109: return "ФР:  Не хватает денег по налогу";
   case 110: return "ФР:  Переполнение денег по налогу";
   case 111: return "ФР:  Переполнение по выплате в смене";
   case 112: return "ФР:  Переполнение ФП";
   case 113: return "ФР:  Ошибка отрезчика";
   case 114: return "ФР:  Команда не поддерживается в данном подрежиме";
   case 115: return "ФР:  Команда не поддерживается в данном режиме";
   case 116: return "ФР:  Ошибка ОЗУ";
   case 117: return "ФР:  Ошибка питания";
   case 118: return "ФР:  Ошибка принтера: нет импульсов с тахогенератора";
   case 119: return "ФР:  Ошибка принтера: нет сигнала с датчиков";
   case 120: return "ФР:  Замена ПО";
   case 121: return "ФР:  Замена ФП";
   case 122: return "ФР:  Поле не редактируется";
   case 123: return "ФР:  Ошибка оборудования";
   case 124: return "ФР:  Не совпадает дата";
   case 125: return "ФР:  Неверный формат даты";
   case 126: return "ФР:  Неверное значение в поле длины";
   case 127: return "ФР:  Переполнение диапазона итога чека";
   case 128: return "ФР:  Ошибка связи с ФП";
   case 129: return "ФР:  Ошибка связи с ФП";
   case 130: return "ФР:  Ошибка связи с ФП";
   case 131: return "ФР:  Ошибка связи с ФП";
   case 132: return "ФР:  Переполнение наличности";
   case 133: return "ФР:  Переполнение по продажам в смене";
   case 134: return "ФР:  Переполнение по покупкам в смене";
   case 135: return "ФР:  Переполнение по возвратам продаж в смене";
   case 136: return "ФР:  Переполнение по возвратам покупок в смене";
   case 137: return "ФР:  Переполнение по внесению в смене";
   case 138: return "ФР:  Переполнение по надбавкам в чеке";
   case 139: return "ФР:  Переполнение по скидкам в чеке";
   case 140: return "ФР:  Отрицательный итог надбавки в чеке";
   case 141: return "ФР:  Отрицательный итог скидки в чеке";
   case 142: return "ФР:  Нулевой итог чека";
   case 143: return "ФР:  Касса не фискализирована";
   case 144: return "ФР:  Поле превышает размер, установленный в настройках";
   case 145: return "ФР:  Выход за границу поля печати при данных настройках шрифта";
   case 146: return "ФР:  Наложение полей";
   case 147: return "ФР:  Восстановление ОЗУ прошло успешно";
   case 148: return "ФР:  Исчерпан лимит операций в чеке";
   case 160: return "ФР:  Ошибка связи с ЭКЛЗ";
   case 161: return "ФР:  ЭКЛЗ отсутствует";
   case 162: return "ЭКЛЗ:  ЭКЛЗ: Некорректный формат или параметр команды";
   case 163: return "ЭКЛЗ:  Некорректное состояние ЭКЛЗ";
   case 164: return "ЭКЛЗ:  Авария ЭКЛЗ";
   case 165: return "ЭКЛЗ:  Авария КС в составе ЭКЛЗ";
   case 166: return "ЭКЛЗ:  Исчерпан временной ресурс ЭКЛЗ";
   case 167: return "ЭКЛЗ:  ЭКЛЗ переполнена";
   case 168: return "ЭКЛЗ:  ЭКЛЗ: Неверные дата и время";
   case 169: return "ЭКЛЗ:  ЭКЛЗ: Нет запрошенных данных";
   case 170: return "ЭКЛЗ:  Переполнение ЭКЛЗ (отрицательный итог документа)";
   case 176: return "ФР:  ЭКЛЗ: Переполнение в параметре количество";
   case 177: return "ФР:  ЭКЛЗ: Переполнение в параметре сумма";
   case 178: return "ФР:  ЭКЛЗ: Уже активизирована";
   case 192: return "ФР:  Контроль даты и времени (подтвердите дату и время)";
   case 193: return "ФР:  ЭКЛЗ: суточный отчёт с гашением прервать нельзя";
   case 194: return "ФР:  Превышение напряжения в блоке питания";
   case 195: return "ФР:  Несовпадение итогов чека и ЭКЛЗ";
   case 196: return "ФР:  Несовпадение номеров смен";
   case 197: return "ФР:  Буфер подкладного документа пуст";
   case 198: return "ФР:  Подкладной документ отсутствует";
   case 199: return "ФР:  Поле не редактируется в данном режиме";
   default: return "";
   }
}

