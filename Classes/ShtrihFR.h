//---------------------------------------------------------------------------
//		����� ��� ������ �� �����-��-�. �������� � ������� ��������� 1.5
//		���� ��������: 			20.12.2004
//		���� ��������������: 	22.12.2004
//		�����: 						
//---------------------------------------------------------------------------

#ifndef ShtrihFRH
#define ShtrihFRH

#include "ShtrihProtocol.h"
#include "StdCommPort.h"
#include <vector>
//---------------------------------------------------------------------------

class CShtrihFR
{
public:

	/*����� ���*/
   struct FLAGS
   {
   	bool OperationMagazine;		//����� ������������� �������
      bool CheakTape;				//����� ������� �����
      bool HightSensorUpDoc;		//������� ������ ����������� ���������
      bool LowSensorUpDoc;			//������ ������ ����������� ���������
      bool FloatingPoint;			//��������� ���.�����(0 - 0 ������, 1 - 2 �����)
      bool EKLZ;						//������� ����
      bool OpticalSensorOperationMagazine;	//���. ������ ����. �������
      bool OpticalSensorCheakTape;	//���. ������ ������� �����
      bool LeverTermogolovkiControlTape;		//����� ������������
		bool LeverTermogolovkiCheakTape;
      bool Cover;						//������ �������
      bool MoneyBox;					//�������� ����
      bool ErrorRightSensorPrinter;	//����� ������� ������� ��������
      bool ErrorLeftSensorPrinter;	//����� ������ ������� ��������
      bool EKLZIfFull;				//���� ����� ���������
      bool HightPrecission;		//����������� ��������
   };

   /*���� �������*/
   enum REGIM {
      regPrinterIsWork = 0,		//������� � ������� ���������
      regDataOut = 1,				//������ ������
      regShiftIsOpen = 2,			//�������� �����, 24 ���� �� ���������
      regShiftIsOpenTimeOver = 3,//�������� �����, 24 ���� ���������
      regShiftIsClose = 4,			//�������� �����
      regBlockWithBadNalogPassword = 5,//���������� �� ������������� ������ ���. ����������
      regWaitCommitSetDate = 6,	//�������� ������������� ����� ����
      regChangeFloatingPoint = 7,//���������� ��������� ��������� ���. �����
      regOpenDoc = 8,				//�������� ��������
      regRegimMayTechnicalZero = 9,//����� ���������� ���������������� ���������
		regTest = 10,					//�������� ������
      regPrintFullFiskalReport = 11,//������ ������� ����������� ������
      regPrintReportEKLZ = 12,	//������ ������ ����
      regWorkWithUnderDoc = 13,	//������ � ���������� ���������� ����������
      regPrintUnderDoc = 14		//������ ����������� ���������
   	 };

   /*����� ��*/
   struct REGIM_FR
   {
   	REGIM Regim;				//�����
      BYTE RegimState;			//������ ������
   };

   /*��� ����� ��*/
   enum SUBREGIM_FR
   {
		sregWait = 0,				//�� � ��������� ��������
      sregWaitPaperOut = 1,   //�� � ��������� ��������, ����������� ������
      sregPrintPaperOut = 2,	//�� � ��������� ������, �� ��������� ������
      sregWaitPrintContinue = 3,//�� ����� ��������� ������, ��� ������� ����������� ������
      sregPrintFullFiskalReport = 4, //�� �������� ������ ���������� ������
      sregPrint = 5				//�� � ��������� ������
   };

	/*��������� ��� ��������� ���*/
   struct MAIN_STATE
   {
   	WORD 				Flags;				//����� ��
      REGIM_FR 		Regim;   			//����� ��
      SUBREGIM_FR		SubRegim;			//��� ����� ��
      WORD 				CountOper;			//���������� �������� � ����
      BYTE				UBattary;			//���������� ��������� ����������
      											//�������� ���������� �������� �� 9
      BYTE				UPower;				//���������� ��������� �������
      											//�������� ���������� �������� �� 9
   	BYTE				ErrorCodeFP;		//��� ������ ��
      BYTE				ErrorCodeEKLZ;		//��� ������ ����
      
   };

	/*�����������*/
	CShtrihFR ( AnsiString CommName, DWORD BaudRate );

   /*����������*/
	~CShtrihFR ( void );

	/*����� ����������*/
   bool FindDevice ( CShtrihProtocol::DEVICE_TYPE *DevType, DWORD *BaudRate );

   /*��������� ��� �����*/
   CStdCommPort* GetCommPort ( void )
   {
   	return m_pComm;
   };

	enum ERROR_TYPE
   {
		ERR_SHORTANSWER = 0x100			//� ������ �� ���������� ����������� ������
   };

	/*������� �������� ����� ��� �������
   out: ���������� ����� ���������*/
   bool XReport ( BYTE *OperatorNumber = NULL );

	/*������� �������� ����� c �����btv
   out: ���������� ����� ���������*/
   bool ZReport ( BYTE *OperatorNumber = NULL );

 	/*������� �������� ����� c �����btv
   out: ���������� ����� ���������*/
   bool ReportBySection ( BYTE *OperatorNumber = NULL );

   /*������� ����
   in: Full - ������ ��� �� ������ ������� ����*/
   bool CutCheak ( bool Full, BYTE *OperatorNumber = NULL );

   /*������� ������ ��������� ��
   out: State - ������� ��������� ���*/
   bool GetCutState ( MAIN_STATE *State, BYTE *OperatorNumber = NULL );

   /*������ ������
   bCheakType - ������ �� ������� �����
   bCtrlType - ������ �� ����������� �����
   String - ������ ��� ������*/
   bool PrintString ( bool bCheakTape, bool bCtrlTape, AnsiString String,
   	BYTE *OperatorNumber = NULL );

   /*�������� ���������
   bCheakType - ������ �� ������� �����
   bCtrlType - ������ �� ����������� �����
   Count - ���������� ����� ��� ��������*/
	bool ListenTape ( bool bCheakTape, bool bCtrlTape, BYTE Count,
   	BYTE *OperatorNumber = NULL );

   /*�������
   in: Summ - ����� �������
   out: CheakNumber - �������� ����� ���������*/
   bool DoPayment ( __int64 Summ, WORD *CheakNumber, BYTE *OperatorNumber = NULL );


   /*�������*/
   bool Sale ( __int64 Amount, __int64 Price, BYTE DepartNumber,
   	AnsiString Text, BYTE *OperatorNumber = NULL );

   /*������� �������*/
	bool ReturnSale ( __int64 Amount, __int64 Price, BYTE DepartNumber,
   	AnsiString Text, BYTE *OperatorNumber = NULL );

   /*������*/
   bool Storno ( __int64 Amount, __int64 Price, BYTE DepartNumber,
   	AnsiString Text, BYTE *OperatorNumber = NULL );

   /*�������� ����*/
   bool CloseTicket ( __int64 Summ, AnsiString Text, __int64 *Change,
   	BYTE *OperatorNumber = NULL );

   /*�������� ����*/
   bool CloseTicket ( __int64 Summ1, __int64 Summ2, __int64 Summ3, __int64 Summ4,
   	AnsiString Text, __int64 *Change, BYTE *OperatorNumber = NULL );

   /*������������� ����*/
   bool CancelTicket ( BYTE *OperatorNumber = NULL );

   enum TICKET_TYPE { ticSale = 0, ticBay = 1, ticReturnSale = 2, ticReturnBay = 3};

   /*������� ���*/
   bool OpenTicket ( TICKET_TYPE type, BYTE *OperatorNumber = NULL );

   /*������� ����������� ������*/
   bool ContinuePrint ( BYTE *OperatorNumber = NULL );

	/*������ ���������*/
   bool CopyDoc ( BYTE *OperatorNumber = NULL );

   bool GetDeviceType ( CShtrihProtocol::DEVICE_TYPE *dev );

   /*��������� ������ ���������*/
   bool GetCheackNumber ( WORD *number );

   int GetError ( void )
   {
   	return m_ErrorCode;
   };

   AnsiString GetErrorStr ( void );

   bool IsWorking ( void )
   {
   	return m_bWorking;
   };

protected:

	/*���������� ��������
   in: CommandNumber - ����� �������
   	datain - �������� ��������� �������
   out: answer - ��������� ������
   ������������ true ���� �� �������*/
   bool ExecuteCommand ( BYTE CommandNumber, std::vector<BYTE> datain,
   	std::vector<BYTE> &answer );

   /*��������� ������� � ������ ����� ��������� ������ �� ������ ���������*/
   bool ExecuteSimpleCommand ( BYTE CommandNumber, std::vector<BYTE> datain,
   	std::vector<BYTE> &answer, BYTE *OperatorNumber = NULL );

private:

	CStdCommPort *m_pComm;				//��� ����
   CShtrihProtocol *m_pProt;			//�������� ������
   BYTE m_Password[4];

   int m_ErrorCode;
   bool m_bWorking;
};

#endif
