//---------------------------------------------------------------------------

#ifndef shtrih_funcH
#define shtrih_funcH
//---------------------------------------------------------------------------

#include <vector>

namespace shtrih_func
{

template <class T>
std::vector<BYTE>::iterator
ReadValueFromAnswer ( T *Value, std::vector<BYTE>::iterator start,
  	BYTE Count );

//вставка длинного значения в вектор
template <class T, class InsertIterator>
void AddValueToVector ( InsertIterator iter, T Value, BYTE Count );

template <class InsertIterator>
void AddStringToVector ( InsertIterator iter, AnsiString str, BYTE Count );

//=============================================================================

//реализация шаблонной функции
template <class T>
std::vector<BYTE>::iterator
ReadValueFromAnswer ( T *Value, std::vector<BYTE>::iterator start,
	BYTE Count )
{
	std::vector<BYTE>::iterator iter = start;// + Count - 1;
   T pow = 1;
   *Value = 0;
   for ( BYTE i = 0; i < Count; i++ )
   {
		*Value = *Value + (*iter) * pow;
//      if ( *Value != 0 )
      	pow = pow * 0x100;
      iter++;
   }
   return iter;
}

//вставка длинного значения в вектор
template <class T, class InsertIterator>
void AddValueToVector ( InsertIterator iter, T Value, BYTE Count )
{
	T val = Value;
   BYTE buf;
	for ( BYTE i = 0; i < Count; i++ )
   {
		buf = val % 256;
      *iter = buf;
      iter++;
      val = val / 256;
   }
}

template <class InsertIterator>
void AddStringToVector ( InsertIterator iter, AnsiString str, BYTE Count )
{
	int l = min ( str.Length(), (int)Count );
   copy ( str.c_str(), str.c_str() + l, iter );
   if ( l < Count )
   {
   	for ( int i = 0; i < Count - l; i++ )
      {
//      	data.push_back ( 0 );
			*iter = 0;
         iter++;
      }
   }
}

};
#endif
