//---------------------------------------------------------------------------

#include <vcl.h>
#include <iterator>
#pragma hdrstop

#include "ShtrihProtocol.h"

using namespace std;
//---------------------------------------------------------------------------

const int STX = 0x2;
const int ENQ = 0x5;
const int ACK = 0x6;
const int NAK = 0x15;

#pragma package(smart_init)

/*�����������*/
CShtrihProtocol::CShtrihProtocol ( CStdCommPort *pPort )
{
	m_pPort = pPort;
   m_Error = errOk;
}

/*��������� ���-�����*/
void CShtrihProtocol::ConfigurePort ( CStdCommPort *pPort )
{
	pPort->SetBaudRate ( 9600 );
   pPort->SetByteSize ( 8 );
   pPort->SetStopBits ( ONESTOPBIT );

   COMMTIMEOUTS t;
   t.ReadIntervalTimeout = 10;
   t.ReadTotalTimeoutMultiplier = 100;
   t.ReadTotalTimeoutConstant = 10;
   t.WriteTotalTimeoutMultiplier = 100;
   t.WriteTotalTimeoutConstant = 10;
}

/*������ �������� ���������
out: ��������� true ���� �� ���� ������ �����
  	ErrorCode - ������ � �������� ���������
   answer - ��������� ������
��� ������� ������ ����� ��� ������ ����� ������ ������� GetError*/
bool CShtrihProtocol::ReadAnswer (
//  	BYTE *ErrorCode,
   std::vector<BYTE> &answer )
{
	vector<BYTE> data;
   bool bContinue = true;

   try
   {
		while ( bContinue )
   	{
      	DWORD t1, t2;
//         t1 = GetTickCount();
	   	if ( m_pPort->GetDataTimeLen ( data, 5000, 500, 1 ) == 0 )
	   	{
				m_Error = errLink;
	   	   return false;
		  	}
//         t2 = GetTickCount();
//         ShowMessage ( t2 - t1 );

	   	if ( data[0] == STX )//���� �� ������� ����������� �� ���������� ��������� �������
	   	{
   	   	bContinue = false;
   		}
	   }

   	if ( m_pPort->GetDataTimeLen ( data, 5000, 500, 1 ) == 0 )
	   {
			m_Error = errTimeOut;
      	return false;
	   }

   	if ( m_pPort->GetDataTimeLen ( data, 500, 500, data[1] + 1 ) < data[1] + 1 )
	   {
   		m_Error = errTimeOut;
      	return false;
	   }
   }catch ( ECommError &E )
   {
   	m_Error = errCommPort;
      return false;
   }

//   BYTE csumm = GetCheckSumm ( data.begin() + 1, data.end() - 1 );
//   BYTE rsumm = *(data.end() - 1);
   if ( GetCheckSumm ( data.begin() + 1, data.end() - 1 ) != *(data.end() - 1) )
   {
   	m_Error = errCheckSumm;
      return false;
   }

	if ( data.size() >= 3 )
   {
//   	*ErrorCode = data[2];
      copy ( data.begin() + 3, data.end() - 1, back_inserter(answer) );
   }

   return true;
}

/*������� � ������ �������� ���������
out: ��������� true ���� �� ���� ������ �����
  	ErrorCode - ������ � �������� ���������
   answer - ��������� ������
��� ������� ������ ����� ��� ������ ����� ������ ������� GetError*/
bool CShtrihProtocol::WaitAnswer ( std::vector<BYTE> &answer )
{
	int j = 0;
	while ( true )
	{
		if ( ReadAnswer ( answer ) )
   	{
			return SendACK();
	   }else
	   {
   		if ( m_Error == errLink || m_Error == errCommPort )
      		return false;
	      else
   	   {
         	if ( j >= 10 )
            	return false;

				if ( ! SendNAK() )
            	return false;

           	BYTE Simbol;
           	if ( ! SendENQ ( &Simbol ) )
            	return false;

      	}
	   }
      j++;
   }
}

/*�������� �������*/
bool CShtrihProtocol::SendMessage ( BYTE CommandNumber,
	std::vector<BYTE> data )
{
	BYTE Simbol = ACK;

   if ( ! SendENQ ( &Simbol ) )
   	return false;

   while ( Simbol == ACK )
   {
      vector<BYTE> startanswer;
      WaitAnswer ( startanswer );

	   if ( ! SendENQ ( &Simbol ) )
   		return false;
   }

   vector<BYTE> cmd;
   cmd.push_back ( STX );
   cmd.push_back ( 1 + data.size() );
   cmd.push_back ( CommandNumber );
   copy ( data.begin(), data.end(), back_inserter(cmd) );
   BYTE CheckSumm = 0;
	vector<BYTE>::iterator iter;
   for ( iter = cmd.begin() + 1; iter != cmd.end(); iter++ )
   	CheckSumm = CheckSumm ^ *iter;
   cmd.push_back ( CheckSumm );

//   bool bContinue = true;
	int j = 0;
   while ( j < 10 )
	{
	   try
   	{
			m_pPort->SendData ( cmd );
	   }catch ( ECommError &E )
   	{
   		m_Error = errCommPort;
	      return false;
   	}

		if ( WaitACK() )
      	return true;

      j++;
   }

   return false; 
}

/*�������� ��� ����������*/
bool CShtrihProtocol::GetDeviceType ( int *ErrorNumber, CShtrihProtocol::DEVICE_TYPE *DevType )
{
	*ErrorNumber = 0;
   vector<BYTE> data, answer, readdata;
   if ( SendMessage ( 0xFC, data ) )
   {
   	if ( WaitAnswer ( readdata ) )
      {
      	if ( readdata.empty() )
         {
         	m_Error = errLength;
            return false;
         }

         *ErrorNumber = readdata[0];

         copy ( readdata.begin() + 1, readdata.end(), back_inserter ( answer ) );

         if ( *ErrorNumber == 0 )
         {
		   	DevType->DevType = answer[0];
		   DevType->DevSubType = answer[1];
		   DevType->Version = answer[2];
			DevType->SubVersion = answer[3];
	      	DevType->Model = answer[4];
			  DevType->Language = answer[5];

			  AnsiString S = "";
			vector<BYTE>::iterator iter;
		   for ( iter = answer.begin() + 6; iter != answer.end(); iter++ )
				S  += (char) *iter;
		   DevType->Name = S;
//		   DevType->Name = AnsiString ( (char*)(answer.begin() + 6), answer.size() - 6 );
			return true;
         }
      }
   }

   return false;
}

/*���������� ����� �����-����������
���������� true ���� ���������� ������� � � ����������
  	ErrorType ������������ ������
   DevType ������������ ���������� �� ����������
   BaudRate ������������ �������� ����������*/
bool CShtrihProtocol::FindDeviceType ( int *ErrorNumber,
	CShtrihProtocol::DEVICE_TYPE *DevType, DWORD *BaudRate )
{
   DWORD br[] = {2400, 4800, 9600, 19200, 38400, 57600, 115200};
	for ( int i = 0; i < 7; i++ )
   {
   	Sleep ( 2000 );
	   m_pPort->CloseCommPort();
	   try
   	{
   		m_pPort->SetBaudRate ( br[i] );
	      m_pPort->OpenCommPort ();
//         m_pPort->
         if ( GetDeviceType ( ErrorNumber, DevType ) )
         {
         	*BaudRate = br[i];
         	return true;
         }
         if ( GetDeviceType ( ErrorNumber, DevType ) )
         {
         	*BaudRate = br[i];
         	return true;
         }
		}catch ( ECommError )
      {
      }
   }

   return false;
}

AnsiString CShtrihProtocol::GetErrorStr ( ERROR_TYPE err )
{
	switch ( err )
   {
   case errOk: return "��� ������";
   case errLink: return "��� ����� � �����������";
   case errTimeOut: return "����� �������� �������";
   case errLength: return "�������� ����� ���������";
   case errCheckSumm: return "�������� ����������� �����";
   case errCommPort: return "������ ��� �����";
   default: return "����������� ������";
   }
}

/*���������� ��������� ������*/
CShtrihProtocol::ERROR_TYPE CShtrihProtocol::GetError ( void )
{
 	return m_Error;
};

AnsiString CShtrihProtocol::GetErrorStr ( void )
{
 	return GetErrorStr ( GetError() );
};

/*��������� ����������� �����*/
BYTE CShtrihProtocol::GetCheckSumm ( std::vector<BYTE>::iterator start,
	std::vector<BYTE>::iterator end )
{
	BYTE res = 0;
	vector<BYTE>::iterator iter;
   for ( iter = start; iter != end; iter++ )
		res = res ^ *iter;
   return res;
}

/*�������� ENQ � ��� �����
���������� true - ���� ����� ����������(NAK ��� ENQ)
Simbol - �������� ����������� �������*/
bool CShtrihProtocol::SendENQ ( BYTE *Simbol )
{
	vector<BYTE> data;
	try
   {
   	m_pPort->PutByte ( ENQ );

      while ( true )
      {
      	data.clear();
	      if ( m_pPort->GetDataTimeLen ( data, 500, 500, 1 ) == 0 )
   	   {
      		m_Error = errLink;
         	return false;
	      }

   	   if ( data[0] == ACK || data[0] == NAK )
      	{
		      *Simbol = data[0];
      		return true;
   	   }
      }
   }catch ( ECommError &E )
   {
   	m_Error = errCommPort;
      return false;
   }
}

/*�������� �������������(ACK)
true - ���� �� �������*/
bool CShtrihProtocol::SendACK ( void )
{
	try
   {
   	m_pPort->PutByte ( ACK );
   	return true;
   }catch ( ECommError &E )
   {
   	m_Error = errCommPort;
      return false;
   }
}

/*�������� ��������� �� ������(NAK)*/
bool CShtrihProtocol::SendNAK ( void )
{
	try
   {
   	m_pPort->PutByte ( NAK );
   	return true;
   }catch ( ECommError &E )
   {
   	m_Error = errCommPort;
      return false;
   }
}

/*������� �������������*/
bool CShtrihProtocol::WaitACK ( void )
{
	vector<BYTE> v;
	while ( true )
   {
   	v.clear();
      try
      {
			if ( m_pPort->GetDataTimeLen ( v, 500, 500, 1 ) == 0 )
         {
         	m_Error = errLink;
            return false;
         }

         if ( v[0] == ACK )
         	return true;

      }catch ( ECommError &E )
      {
      	m_Error = errCommPort;
         return false;
      }
   }
}
