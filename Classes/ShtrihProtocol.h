//---------------------------------------------------------------------------
//	 	����� ��������� ��� ������ � ��������������� ���������� ��� "�����-���"
//			�������� ������ 1.1
//		���� ��������:				15.10.2004
//		���� ��������������:		20.12.2004
//		�����:
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
   	errOk,				//��� ������
   	errLink,				//��� �����
      errTimeOut,			//����� ����� ��������
      errLength,			//�������� �����
      errCheckSumm,		//�������� ����������� �����
      errCommPort		//������ ��� �����
//      errBadSimbol		//�������� ������
   };

	/*�����������*/
	CShtrihProtocol ( CStdCommPort *pPort );

   /*��������� ���-�����*/
   static void ConfigurePort ( CStdCommPort *pPort );

   /*������ �������� ���������
   out: ��������� true ���� �� ���� ������ �����
   	ErrorCode - ������ � �������� ���������
      answer - ��������� ������
   ��� ������� ������ ����� ��� ������ ����� ������ ������� GetError*/
   bool ReadAnswer (
//   	BYTE *ErrorCode,
      std::vector<BYTE> &answer );

   /*������� � ������ �������� ���������
   out: ��������� true ���� �� ���� ������ �����
   	ErrorCode - ������ � �������� ���������
      answer - ��������� ������
   ��� ������� ������ ����� ��� ������ ����� ������ ������� GetError*/
   bool WaitAnswer ( std::vector<BYTE> &answer );

   /*�������� �������*/
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

   /*�������� ��� ����������
   ���������� true - ���� �� �������.
   ���������� false - ���� ��������� ������.
   	ErrorNumber = 0, ���� ��� ������ ����� � ������ ����� �������� ���������
      	GetError.
      ���� ErrorNumber != 0 - �� ��� ������������ ������*/
   bool GetDeviceType ( int *ErrorNumber, CShtrihProtocol::DEVICE_TYPE *DevType );

	/*���������� ����� �����-����������
   ���������� true ���� ���������� ������� � � ����������
   	ErrorType ������������ ������
      DevType ������������ ���������� �� ����������
      BaudRate ������������ �������� ����������*/
   bool FindDeviceType ( int *ErrorNumber, CShtrihProtocol::DEVICE_TYPE *DevType,
   	DWORD *BaudRate );

	/*���������� ��������� ������*/
   ERROR_TYPE GetError ( void );

   AnsiString GetErrorStr ( ERROR_TYPE err );
   AnsiString GetErrorStr ( void );

protected:

   /*��������� ����������� �����*/
   BYTE GetCheckSumm ( std::vector<BYTE>::iterator start,
   	std::vector<BYTE>::iterator end );

	/*�������� ENQ � ��� �����
   ���������� true - ���� ����� ����������(NAK ��� ENQ)
   Simbol - �������� ����������� �������*/
   bool SendENQ ( BYTE *Simbol );

   /*�������� �������������(ACK)
   true - ���� �� �������*/
   bool SendACK ( void );

   /*�������� ��������� �� ������(NAK)*/
   bool SendNAK ( void );

   /*������� �������������*/
   bool WaitACK ( void );

private:

	CStdCommPort *m_pPort;

   ERROR_TYPE m_Error;
};

#endif
