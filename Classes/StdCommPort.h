//---------------------------------------------------------------------------
//    ����� com �����, ���������� �� ����������� ������ �� STL
//    ���� ���������� �������������� 15.10.04
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

   /*�������� ������ �� �������
   */
   void SendData ( std::vector<BYTE> &in ) throw(ECommError);

   /*������ ������ �� com ����� � ���������� � ������
   ������������ ���������� ����������� ����*/
   int GetData ( std::vector<BYTE> &out ) throw(ECommError);

   /*������ ������ �� com ����� �� ����� len ���� � ���������� � ������
   ������������ ���������� ����������� ����*/
   int GetDataLen ( std::vector<BYTE> &out, int len ) throw(ECommError);

   /*������ ������ �� com ����� �� ����� Count ���� � ���������� � ������
   ������������ ���������� ����������� ����*/
   int GetDataCount ( std::vector<BYTE> &out, int Count ) throw(ECommError);

   /*������ ������ �� com ����� � ������� Time ��������� ���� � ���������� � ������
   ������ ���� ���, �� ������ ���������.
   ������������ ���������� ����������� ����*/
   int GetDataTime ( std::vector<BYTE> &out, int Time ) throw(ECommError);

   /*������ ������ �� com ����� � ������� Time1 ��������� ����
   � ���������� � ������. �������� ����� ������� ����������� �
   Time2 ����������
   ������ ���� ���, �� ������ ���������.
   ������������ ���������� ����������� ����*/
   int GetDataTime ( std::vector<BYTE> &out,
      int Time1, int Time2 ) throw(ECommError);

   /*������ ������ �� com ����� � ������� Time1 ��������� ����
   � ���������� � ������. �������� ����� ������� ����������� �
   Time2 ����������
   ������ ���� ���, �� ������ ���������.
   ������������ ���������� ����������� ����
   ����� ������� ��������� ������ ���������� ������� �������� �� ���������
   ������� ���������� true ��� ������������� ���������� ������*/
   int GetDataTimeFunc ( std::vector<BYTE> &out,
      int Time1, int Time2, bool func(std::vector<BYTE> &) ) throw(ECommError);

   /*������ ������ �� com ����� � ������� Time1 ��������� ����
   � ���������� � ������. �������� ����� ������� ����������� �
   Time2 ����������
   ������ ���� ���, �� ������ ���������.
   ������������ ���������� ����������� ����. Len - ������������ �����*/
   int GetDataTimeLen ( std::vector<BYTE> &out,
      int Time1, int Time2, int Len ) throw(ECommError);

   //������ ������ ����� �� �����
   bool ReadByteTime ( BYTE *buf, int Time ) throw (ECommError);
};

#endif
 