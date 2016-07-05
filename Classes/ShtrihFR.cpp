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

/*�����������*/
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

/*����������*/
CShtrihFR::~CShtrihFR ( void )
{
	delete m_pProt;

   delete m_pComm;
}

/*����� ����������*/
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

/*������� �������� ����� ��� �������
out: ���������� ����� ���������*/
bool CShtrihFR::XReport ( BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   return ExecuteSimpleCommand ( 0x40, data, answer, OperatorNumber );
}

/*������� �������� ����� c �����btv
out: ���������� ����� ���������*/
bool CShtrihFR::ZReport ( BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   return ExecuteSimpleCommand ( 0x41, data, answer, OperatorNumber );
}

/*������� �������� ����� c �����btv
out: ���������� ����� ���������*/
bool CShtrihFR::ReportBySection ( BYTE *OperatorNumber/* = NULL*/ )
{
	vector<BYTE> data, answer;
   return ExecuteSimpleCommand ( 0x42, data, answer, OperatorNumber );
}

/*������� ����
in: Full - ������ ��� �� ������ ������� ����*/
bool CShtrihFR::CutCheak ( bool Full, BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   if ( Full )
	   data.push_back ( 0 );
   else
   	data.push_back ( 1 );

   return ExecuteSimpleCommand ( 0x25, data, answer, OperatorNumber );
}

/*������� ������ ��������� ��
out: State - ������� ��������� ���*/
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

/*������ ������
bCheakType - ������ �� ������� �����
bCtrlType - ������ �� ����������� �����
String - ������ ��� ������*/
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

/*�������� ���������
bCheakType - ������ �� ������� �����
bCtrlType - ������ �� ����������� �����
Count - ���������� ����� ��� ��������*/
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

/*�������
in: Summ - ����� �������
out: CheakNumber - �������� ����� ���������*/
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

/*�������*/
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

/*������*/
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

/*�������� ����*/
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

/*�������� ����*/
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

/*������������� ����*/
bool CShtrihFR::CancelTicket ( BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   return ExecuteSimpleCommand ( 0x88, data, answer, OperatorNumber );
}

/*������� ���*/
bool CShtrihFR::OpenTicket ( TICKET_TYPE type, BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   data.push_back ( (BYTE)type );
   return ExecuteSimpleCommand ( 0x8D, data, answer, OperatorNumber );
}

/*������� ����������� ������*/
bool CShtrihFR::ContinuePrint ( BYTE *OperatorNumber )
{
	vector<BYTE> data, answer;
   return ExecuteSimpleCommand ( 0xB0, data, answer, OperatorNumber );
}

/*������ ���������*/
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

/*��������� ������ ���������*/
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

/*���������� ��������
in: CommandNumber - ����� �������
  	datain - �������� ��������� �������
out: answer - ��������� ������
������������ true ���� �� �������*/
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

/*��������� ������� � ������ ����� ��������� ������ �� ������ ���������*/
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
   case 0: return "��:  ������ ���";
   case 1: return "��:  ���������� ���������� �� 1, �� 2 ��� ����";
   case 2: return "��:  ����������� ��";
   case 3: return "��:  ����������� �� 2";
   case 4: return "��:  ������������ ��������� � ������� ��������� � ��";
   case 5: return "��:  ��� ����������� ������";
   case 6: return "��:  �� � ������ ������ ������";
   case 7: return "��:  ������������ ��������� � ������� ��� ������ ���������� ��";
   case 8: return "��:  ������� �� �������������� � ������ ���������� ��";
   case 9: return "��:  ������������ ����� �������";
   case 10: return "��:  ������ ������ �� BCD";
   case 11: return "��:  ���������� ������ ������ �� ��� ������ �����";
   case 17: return "��:  �� ������� ��������";
   case 18: return "��:  ��������� ����� ��� ������";
   case 19: return "��:  ������� ���� ������ ���� ��������� ������ � ��";
   case 20: return "��:  ������� ������� ������ �� �����������";
   case 21: return "��:  ����� ��� �������";
   case 22: return "��:  ����� �� �������";
   case 23: return "��:  ����� ������ ����� ������ ������ ��������� �����";
   case 24: return "��:  ���� ������ ����� ������ ���� ��������� �����";
   case 25: return "��:  ��� ������ � ��";
   case 26: return "��:  ������� ��������������� � �� �����������";
   case 27: return "��:  ��������� ����� �� ������";
   case 28: return "��:  � �������� ��������� ���� ������������ ������";
   case 29: return "��:  ���������� ��������� ������ ������� ������";
   case 30: return "��:  ������� ��������������� �� �����������";
   case 31: return "��:  ����������� ������ ���������";
   case 32: return "��:  ������������ ��������� �������� ��� ����������";
   case 33: return "��:  ���������� ����� ������ ����������� ��������� ��������";
   case 34: return "��:  �������� ����";
   case 35: return "��:  ��� ������ �����������";
   case 36: return "��:  ������� ����������� �����������";
   case 37: return "��:  ��� ����������� � ������������� �������";
   case 38: return "��:  �������� �������� ����� ������ ����� ����";
   case 43: return "��:  ���������� �������� ���������� �������";
   case 44: return "��:  ��������� ����� (��������� ������� ����������)";
   case 45: return "��:  ����� ���� �� ������ ������ ����� ������";
   case 46: return "��:  � �� ��� ����� ��� �������";
   case 48: return "��:  �� ������������, ���� ����� ������ ���������� ����������";
   case 50: return "��:  ��������� ���������� ������ �������";
   case 51: return "��:  ������������ ��������� � �������";
   case 52: return "��:  ��� ������";
   case 53: return "��:  ������������ �������� ��� ������ ����������";
   case 54: return "��:  ������������ ��������� � ������� ��� ������ ���������� ��";
   case 55: return "��:  ������� �� �������������� � ������ ���������� ��";
   case 56: return "��:  ������ � ���";
   case 57: return "��:  ���������� ������ �� ��";
   case 58: return "��:  ������������ ���������� �� ��������� � �����";
   case 59: return "��:  ������������ ���������� � �����";
   case 60: return "��:  ����: �������� ��������������� �����";
   case 61: return "��:  ����� �� ������� . �������� ����������";
   case 62: return "��:  ������������ ���������� �� ������� � �����";
   case 63: return "��:  ������������ ���������� �� ������� � �����";
   case 64: return "��:  ������������ ��������� ������";
   case 65: return "��:  ������������ ��������� ������ ���������";
   case 66: return "��:  ������������ ��������� ������ ����� 2";
   case 67: return "��:  ������������ ��������� ������ ����� 3";
   case 68: return "��:  ������������ ��������� ������ ����� 4";
   case 69: return "��:  C���� ���� ����� ������ ������ ����� ����";
   case 70: return "��:  �� ������� ���������� � �����";
   case 71: return "��:  ������������ ���������� �� ������� � �����";
   case 72: return "��:  ������������ ����� ����";
   case 73: return "��:  �������� ���������� � �������� ���� ������� ����";
   case 74: return "��:  ������ ��� . �������� ����������";
   case 75: return "��:  ����� ���� ����������";
   case 76: return "��:  ������������ ���������� �� ������� ������� � �����";
   case 77: return "��:  �������� ����������� ������� ����� ������ ����� ����";
   case 78: return "��:  ����� ��������� 24 ����";
   case 79: return "��:  �������� ������";
   case 80: return "��:  ���� ������ ���������� �������";
   case 81: return "��:  ������������ ���������� ��������� � �����";
   case 82: return "��:  ������������ ���������� �� ���� ������ 2 � �����";
   case 83: return "��:  ������������ ���������� �� ���� ������ 3 � �����";
   case 84: return "��:  ������������ ���������� �� ���� ������ 4 � �����";
   case 85: return "��:  ��� ������ . �������� ����������";
   case 86: return "��:  ��� ��������� ��� �������";
   case 87: return "��:  ����: ���������� �������� ���� �� ��������� � ��";
   case 88: return "��:  �������� ������� ����������� ������";
   case 89: return "��:  �������� ������ ������ ����������";
   case 90: return "��:  ������ ��������� ���������� � ����";
   case 91: return "��:  ������������ ��������� ��������";
   case 92: return "��:  �������� ���������� 24�";
   case 93: return "��:  ������� �� ����������";
   case 94: return "��:  ������������ ��������";
   case 95: return "��:  ������������� ���� ����";
   case 96: return "��:  ������������ ��� ���������";
   case 97: return "��:  ������������ ��������� ����";
   case 98: return "��:  ������������ ��������� ����������";
   case 99: return "��:  ������������ ��������� ������";
   case 100: return "��:  �� �����������";
   case 101: return "��:  �� ������� ����� � ������";
   case 102: return "��:  ������������ ����� � ������";
   case 103: return "��:  ������ ����� � ��";
   case 104: return "��:  �� ������� ����� �� ������� �������";
   case 105: return "��:  ������������ ����� �� ������� �������";
   case 106: return "��:  ������ ������� � ������ ������ �� I2C";
   case 107: return "��:  ��� ������� �����";
   case 108: return "��:  ��� ����������� �����";
   case 109: return "��:  �� ������� ����� �� ������";
   case 110: return "��:  ������������ ����� �� ������";
   case 111: return "��:  ������������ �� ������� � �����";
   case 112: return "��:  ������������ ��";
   case 113: return "��:  ������ ���������";
   case 114: return "��:  ������� �� �������������� � ������ ���������";
   case 115: return "��:  ������� �� �������������� � ������ ������";
   case 116: return "��:  ������ ���";
   case 117: return "��:  ������ �������";
   case 118: return "��:  ������ ��������: ��� ��������� � ��������������";
   case 119: return "��:  ������ ��������: ��� ������� � ��������";
   case 120: return "��:  ������ ��";
   case 121: return "��:  ������ ��";
   case 122: return "��:  ���� �� �������������";
   case 123: return "��:  ������ ������������";
   case 124: return "��:  �� ��������� ����";
   case 125: return "��:  �������� ������ ����";
   case 126: return "��:  �������� �������� � ���� �����";
   case 127: return "��:  ������������ ��������� ����� ����";
   case 128: return "��:  ������ ����� � ��";
   case 129: return "��:  ������ ����� � ��";
   case 130: return "��:  ������ ����� � ��";
   case 131: return "��:  ������ ����� � ��";
   case 132: return "��:  ������������ ����������";
   case 133: return "��:  ������������ �� �������� � �����";
   case 134: return "��:  ������������ �� �������� � �����";
   case 135: return "��:  ������������ �� ��������� ������ � �����";
   case 136: return "��:  ������������ �� ��������� ������� � �����";
   case 137: return "��:  ������������ �� �������� � �����";
   case 138: return "��:  ������������ �� ��������� � ����";
   case 139: return "��:  ������������ �� ������� � ����";
   case 140: return "��:  ������������� ���� �������� � ����";
   case 141: return "��:  ������������� ���� ������ � ����";
   case 142: return "��:  ������� ���� ����";
   case 143: return "��:  ����� �� ���������������";
   case 144: return "��:  ���� ��������� ������, ������������� � ����������";
   case 145: return "��:  ����� �� ������� ���� ������ ��� ������ ���������� ������";
   case 146: return "��:  ��������� �����";
   case 147: return "��:  �������������� ��� ������ �������";
   case 148: return "��:  �������� ����� �������� � ����";
   case 160: return "��:  ������ ����� � ����";
   case 161: return "��:  ���� �����������";
   case 162: return "����:  ����: ������������ ������ ��� �������� �������";
   case 163: return "����:  ������������ ��������� ����";
   case 164: return "����:  ������ ����";
   case 165: return "����:  ������ �� � ������� ����";
   case 166: return "����:  �������� ��������� ������ ����";
   case 167: return "����:  ���� �����������";
   case 168: return "����:  ����: �������� ���� � �����";
   case 169: return "����:  ����: ��� ����������� ������";
   case 170: return "����:  ������������ ���� (������������� ���� ���������)";
   case 176: return "��:  ����: ������������ � ��������� ����������";
   case 177: return "��:  ����: ������������ � ��������� �����";
   case 178: return "��:  ����: ��� ��������������";
   case 192: return "��:  �������� ���� � ������� (����������� ���� � �����)";
   case 193: return "��:  ����: �������� ����� � �������� �������� ������";
   case 194: return "��:  ���������� ���������� � ����� �������";
   case 195: return "��:  ������������ ������ ���� � ����";
   case 196: return "��:  ������������ ������� ����";
   case 197: return "��:  ����� ����������� ��������� ����";
   case 198: return "��:  ���������� �������� �����������";
   case 199: return "��:  ���� �� ������������� � ������ ������";
   default: return "";
   }
}

