//---------------------------------------------------------------------------
//    Класс com порта, работающий со структурами данных из STL
//    дата последнего редактирования 15.10.04
//---------------------------------------------------------------------------

#ifndef StdCommPortH
#define StdCommPortH
//---------------------------------------------------------------------------

#include <algorithm>
#include <vector>
#include "comm.h"


class CStdCommPort : public TCommPort
{

public:

   /*Посылает данные из вектора
   */
   void SendData ( std::vector<BYTE> &in ) throw(ECommError);

   /*читает данные из com порта и записывает в вектор
   возвращается количество прочитанных байт*/
   int GetData ( std::vector<BYTE> &out ) throw(ECommError);

   /*читает данные из com порта не более len штук и записывает в вектор
   возвращается количество прочитанных байт*/
   int GetDataLen ( std::vector<BYTE> &out, int len ) throw(ECommError);

   /*читает данные из com порта не более Count байт и записывает в вектор
   возвращается количество прочитанных байт*/
   int GetDataCount ( std::vector<BYTE> &out, int Count ) throw(ECommError);

   /*читает данные из com порта в течении Time милисекнд байт и записывает в вектор
   чтение хоть раз, но всегда произойдёт.
   возвращается количество прочитанных байт*/
   int GetDataTime ( std::vector<BYTE> &out, int Time ) throw(ECommError);

   /*читает данные из com порта в течении Time1 милисекнд байт
   и записывает в вектор. Интервал между байтами допускается в
   Time2 милисекунд
   чтение хоть раз, но всегда произойдёт.
   возвращается количество прочитанных байт*/
   int GetDataTime ( std::vector<BYTE> &out,
      int Time1, int Time2 ) throw(ECommError);

   /*читает данные из com порта в течении Time1 милисекнд байт
   и записывает в вектор. Интервал между байтами допускается в
   Time2 милисекунд
   чтение хоть раз, но всегда произойдёт.
   возвращается количество прочитанных байт
   После каждого успешного чтения вызывается функция проверки на окончание
   функция возвращает true при необходимости продолжать чтение*/
   int GetDataTimeFunc ( std::vector<BYTE> &out,
      int Time1, int Time2, bool func(std::vector<BYTE> &) ) throw(ECommError);

   /*читает данные из com порта в течении Time1 милисекнд байт
   и записывает в вектор. Интервал между байтами допускается в
   Time2 милисекунд
   чтение хоть раз, но всегда произойдёт.
   возвращается количество прочитанных байт. Len - максимальная длина*/
   int GetDataTimeLen ( std::vector<BYTE> &out,
      int Time1, int Time2, int Len ) throw(ECommError);

   //чтение одного байта за время
   bool ReadByteTime ( BYTE *buf, int Time ) throw (ECommError);
};

#endif
 