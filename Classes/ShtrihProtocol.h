//---------------------------------------------------------------------------
//	 	Класс протокола для работы с Миникомпьютером управления ТРК "Штрих-АЗС"
//			Протокол версии 1.1
//		Дата создания:				15.10.2004
//		Дата редактирования:		20.12.2004
//		Автор:
//---------------------------------------------------------------------------

#ifndef ShtrihProtocolH
#define ShtrihProtocolH
//---------------------------------------------------------------------------

#include "StdCommPort.h"
#include <vector>

class CShtrihProtocol
{
public:

	enum ERROR_TYPE {
   	errOk,				//нет ошибок
   	errLink,				//нет связи
      errTimeOut,			//вышло время ожидания
      errLength,			//неверная длина
      errCheckSumm,		//неверная контрольная сумма
      errCommPort		//ошибка ком порта
//      errBadSimbol		//неверный символ
   };

	/*конструктор*/
	CShtrihProtocol ( CStdCommPort *pPort );

   /*настройка ком-порта*/
   static void ConfigurePort ( CStdCommPort *pPort );

   /*читает ответное сообщение
   out: Возвращае true если не было ошибок связи
   	ErrorCode - ошибка в ответном сообщении
      answer - параметры ответа
   При ниличие ошибки связи тип ошибки можно узнать методам GetError*/
   bool ReadAnswer (
//   	BYTE *ErrorCode,
      std::vector<BYTE> &answer );

   /*ожидает и читает ответное сообщение
   out: Возвращае true если не было ошибок связи
   	ErrorCode - ошибка в ответном сообщении
      answer - параметры ответа
   При ниличие ошибки связи тип ошибки можно узнать методам GetError*/
   bool WaitAnswer ( std::vector<BYTE> &answer );

   /*посылает команду*/
   bool SendMessage ( BYTE CommandNumber, std::vector<BYTE> data );

   struct DEVICE_TYPE
   {
   	BYTE DevType;
      BYTE DevSubType;
      BYTE Version;
      BYTE SubVersion;
      BYTE Model;
      BYTE Language;
      AnsiString Name;
   };

   /*получить тип устройства
   Возвращает true - если всё успешно.
   Возвращает false - если произошла ошибка.
   	ErrorNumber = 0, если это ошибка связи и ошибку можно получить используя
      	GetError.
      Если ErrorNumber != 0 - то это возвращённая ошибка*/
   bool GetDeviceType ( int *ErrorNumber, CShtrihProtocol::DEVICE_TYPE *DevType );

	/*Производит поиск штрих-устройства
   возвращает true если устройство найдено и в переменные
   	ErrorType записывается ошибка
      DevType записывается информация об устройстве
      BaudRate записывается скорость устройства*/
   bool FindDeviceType ( int *ErrorNumber, CShtrihProtocol::DEVICE_TYPE *DevType,
   	DWORD *BaudRate );

	/*возвращает последнюю ошибку*/
   ERROR_TYPE GetError ( void );

   AnsiString GetErrorStr ( ERROR_TYPE err );
   AnsiString GetErrorStr ( void );

protected:

   /*вычисляет контрольную сумму*/
   BYTE GetCheckSumm ( std::vector<BYTE>::iterator start,
   	std::vector<BYTE>::iterator end );

	/*засылает ENQ и ждёт ответ
   возвращает true - если ответ корректный(NAK или ENQ)
   Simbol - значение полученного символа*/
   bool SendENQ ( BYTE *Simbol );

   /*засылает подтверждение(ACK)
   true - если всё успешно*/
   bool SendACK ( void );

   /*засылает сообщение об ошибке(NAK)*/
   bool SendNAK ( void );

   /*ожидает подтверждения*/
   bool WaitACK ( void );

private:

	CStdCommPort *m_pPort;

   ERROR_TYPE m_Error;
};

#endif
