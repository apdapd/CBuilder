//---------------------------------------------------------------------------

#ifndef ValueHistoryH
#define ValueHistoryH
//---------------------------------------------------------------------------

#include <list>
#include <boost/shared_ptr.hpp>

#include <fstream>

//переменная с историей значений
//MaxList - максимальная длина списка истории
template <class T, int MaxList = 100>
class ValueHistory
{
public:

   //конструктор по умолчанию
   ValueHistory();

   //конструктор со значением по умолчанию
   ValueHistory(T val);

   void Assign ( T val );

   T operator= ( T val );

   T FirstValue() const;
   T LastValue() const;

   //удаляет самое раннее значение
   //возвращает true если есть ещё элементы
   bool PopFirstValue();

   operator T () const;

   bool IsExist(T val) const;

private:

   T m_LastValue;    //последнее значение
   boost::shared_ptr<std::list<T> > m_History;    //история значений
   int m_SizeHistory;      //размер истории значений

};

//конструктор по умолчанию
template <class T, int MaxList = 100>
ValueHistory<T, MaxList>::ValueHistory()
{
   m_SizeHistory = 0;
   m_History = boost::shared_ptr<std::list<T> >(new std::list<T>());
};

   //конструктор со значением по умолчанию
template <class T, int MaxList = 100>
ValueHistory<T, MaxList>::ValueHistory(T val)
   :m_LastValue(val)
{
   m_SizeHistory = 0;
   m_History = boost::shared_ptr<std::list<T> >(new std::list<T>());
};

//сохраняет прошлое значение в истории
template <class T, int MaxList = 100>
T ValueHistory<T, MaxList>::operator= ( T val )
{
   if ( val != m_LastValue )
   {
      m_History->push_back(m_LastValue);
      ++m_SizeHistory;
      if ( m_SizeHistory > MaxList && MaxList != 0 )
      {
         m_History->pop_front();
         --m_SizeHistory;
      }
      m_LastValue = val;
   }

   return val;
};

template <class T, int MaxList = 100>
bool ValueHistory<T, MaxList>::IsExist(T val) const
{
   std::list<T>::iterator iter;
   for ( iter = m_History->begin(); iter != m_History->end(); ++iter )
   {
      if ( *iter == val )
         return true;
   }
   return val == m_LastValue;
}

template <class T, int MaxList = 100>
void ValueHistory<T, MaxList>::Assign ( T val )
{
   m_SizeHistory = 0;
   m_History->clear();

   m_LastValue = val;
}

//самое ранне значение
template <class T, int MaxList = 100>
T ValueHistory<T, MaxList>::FirstValue() const
{
   if ( m_History->empty() )
      return m_LastValue;
   else
      return m_History->front();
};

//самое позднее значение
template <class T, int MaxList = 100>
T ValueHistory<T, MaxList>::LastValue() const
{
   return m_LastValue;
};

   //удаляет самое раннее значение
   //возвращает true если есть ещё элементы
template <class T, int MaxList = 100>
bool ValueHistory<T, MaxList>::PopFirstValue()
{
   if ( m_History->empty() )
      return false;
   else
   {
      m_History->pop_front();
      --m_SizeHistory;
      return true;
   }
};

template <class T, int MaxList = 100>
ValueHistory<T, MaxList>::operator T () const
{
   return FirstValue();//*/m_LastValue;
};

#endif
