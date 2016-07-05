//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "StdCommPort.h"

//---------------------------------------------------------------------------

using namespace std;

#pragma package(smart_init)


int GetMSec ( TDateTime Time )
{
   unsigned short hour, min, sec, msec;
   Time.DecodeTime ( &hour, &min, &sec, &msec );
   return msec + sec * 1000 + min * 1000 * 60 + hour * 1000 * 60 * 60;
};


/*�������� ������ �� �������
*/
void CStdCommPort::SendData ( vector<BYTE> &in )  throw(ECommError)
{
   WriteBuffer ( &(in.front()), in.size() );
}

/*������ ������ �� com ����� � ���������� � ������
������������ ���������� ����������� ����*/
int CStdCommPort::GetData ( vector<BYTE> &out ) throw(ECommError)
{
   BYTE val;
   int Count = 0;

   while ( ReadBytes(&val, 1) != 0 )
   {
      ++Count;
      out.push_back ( val );
   }

   return Count;
}

/*������ ������ �� com ����� �� ����� len ���� � ���������� � ������
������������ ���������� ����������� ����*/
int CStdCommPort::GetDataLen ( std::vector<BYTE> &out, int len ) throw(ECommError)
{
   BYTE val;
   int Count = 0;

   while ( ReadBytes(&val, 1) != 0 )
   {
      ++Count;
      out.push_back ( val );
      if ( Count == len )
      	return Count;
   }

   return Count;
}

/*������ ������ �� com ����� �� ����� Count ���� � ���������� � ������
������������ ���������� ����������� ����*/
int CStdCommPort::GetDataCount ( std::vector<BYTE> &out, int Count ) throw(ECommError)
{
   BYTE val;
   int c = 0;

   while ( ReadBytes(&val, 1) != 0 && c < Count )
   {
      ++c;
      out.push_back ( val );
   }

   return c;
}

/*������ ������ �� com ����� � ������� Time ��������� ���� � ���������� � ������
������ ���� ���, �� ������ ���������.
������������ ���������� ����������� ����*/
int CStdCommPort::GetDataTime ( std::vector<BYTE> &out, int Time ) throw(ECommError)
{
   TDateTime StartTime = Now();
   int Count = 0;
   int val;// = 0;

   do
   {
      val = GetData(out);
      Count += val;
   } while ( val != 0 || GetMSec(Now() - StartTime) < Time );

   return Count;
}

/*������ ������ �� com ����� � ������� Time1 ��������� ����
� ���������� � ������. �������� ����� ������� ����������� �
Time2 ����������
������ ���� ���, �� ������ ���������.
������������ ���������� ����������� ����*/
int CStdCommPort::GetDataTime ( std::vector<BYTE> &out,
   int Time1, int Time2 ) throw(ECommError)
{
   TDateTime StartTime = Now(), ReadTime = Now();
   int Count = 0;
   int val;// = 0;

   bool bContinue;

   do
   {
      val = GetData(out);
      if ( val != 0 )
         ReadTime = Now();
      Count += val;
//   } while ( val != 0 || GetMSec(Now() - StartTime) < Time );

      if ( Count == 0 )
      {
         bContinue = ( GetMSec(Now() - StartTime) < Time1 );
      }else
      {
         if ( val != 0 )
            bContinue = true;
         else
            bContinue = ( GetMSec(Now() - ReadTime) < Time2 );
      }

   } while ( bContinue
      /*(Count == 0 && GetMSec(Now() - StartTime) < Time1) ||
      (Count != 0 && val != 0 && GetMSec(Now() - ReadTime) < Time2)*/ );

   return Count;
}

/*������ ������ �� com ����� � ������� Time1 ��������� ����
� ���������� � ������. �������� ����� ������� ����������� �
Time2 ����������
������ ���� ���, �� ������ ���������.
������������ ���������� ����������� ����
����� ������� ��������� ������ ���������� ������� �������� �� ���������
������� ���������� true ��� ������������� ���������� ������*/
int CStdCommPort::GetDataTimeFunc ( std::vector<BYTE> &out,
  int Time1, int Time2, bool func(std::vector<BYTE> &) ) throw(ECommError)
{
   TDateTime StartTime = Now(), ReadTime = Now();
   int Count = 0;
   int val;// = 0;

   bool bContinue;

   do
   {
      val = GetData(out);
      if ( val != 0 )
         ReadTime = Now();
      Count += val;

      if ( Count == 0 )
      {
         bContinue = ( GetMSec(Now() - StartTime) < Time1 );
      }else
      {
         if ( val != 0 )
         {
            bContinue = func ( out );
         }
         else
            bContinue = ( GetMSec(Now() - ReadTime) < Time2 );
      }

   } while ( bContinue );

   return Count;
}

/*������ ������ �� com ����� � ������� Time1 ��������� ����
� ���������� � ������. �������� ����� ������� ����������� �
Time2 ����������
������ ���� ���, �� ������ ���������.
������������ ���������� ����������� ����. Len - ������������ �����*/
int CStdCommPort::GetDataTimeLen ( std::vector<BYTE> &out,
   int Time1, int Time2, int Len ) throw(ECommError)
{
   if ( Len == 0 )
      return GetDataTime ( out, Time1, Time2 );

   TDateTime StartTime = Now(), ReadTime = Now();
   int Count = 0;
   int val;// = 0;

   bool bContinue;

   do
   {
      val = GetDataLen(out, Len - Count);
      if ( val != 0 )
         ReadTime = Now();
      Count += val;

      if ( Count == 0 )
      {
         bContinue = ( GetMSec(Now() - StartTime) < Time1 );
      }else
      {
         if ( Count >= Len )
            bContinue = false;
         else if ( val != 0 )
            bContinue = true;
         else
            bContinue = ( GetMSec(Now() - ReadTime) < Time2 );
      }

   } while ( bContinue );

   return Count;
}

//������ ������ ����� �� �����
bool CStdCommPort::ReadByteTime ( BYTE *buf, int Time ) throw (ECommError)
{
   int StartTime = GetTickCount();
   bool bContinue;

   while ( true )
   {
      if ( ReadBytes(buf, 1) != 0 )
         return true;

      if ( (GetTickCount() - StartTime) > (DWORD)Time )
         return false;
   }
}

