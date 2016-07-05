//---------------------------------------------------------------------------

#ifndef TestH
#define TestH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBUpdateSQL.hpp>
#include <IBQuery.hpp>
#include <Vcl.Menus.hpp>
#include <IBStoredProc.hpp>
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
#include "frxExportHTML.hpp"
#include "frxExportPDF.hpp"
#include "frxExportRTF.hpp"
#include <Vcl.Dialogs.hpp>
#include <IBTable.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TIBTransaction *MainIBTransaction;
	TIBUpdateSQL *IBSQLUpdate111;
	TIBDatabase *MainIBDatabase;
	TButton *Button1;
	TMemo *Memo1;
	TIBQuery *IBSQLUpdate;
	TMainMenu *MainMenu1;
	TMenuItem *KKMItem;
	TMenuItem *N1;
	TMenuItem *DBItem;
	TIBQuery *IBQuery1;
	TIBStoredProc *IBStoredProc1;
	TfrxDBDataset *frxDBDataset1;
	TfrxReport *frxReport1;
	TEdit *EditNumEnd;
	TButton *ButtonReport;
	TDBGrid *DBGrid1;
	TDataSource *DataSource1;
	TfrxPDFExport *frxPDFExport1;
	TfrxHTMLExport *frxHTMLExport1;
	TfrxRTFExport *frxRTFExport1;
	TOpenDialog *OpenDialog1;
	TButton *ButtonDBOpen;
	TEdit *EditDBName;
	TButton *ButtonDBFind;
	TfrxDBDataset *frxDBDataset2;
	TIBQuery *IBQuery2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall KKMItemClick(TObject *Sender);
	void __fastcall ButtonReportClick(TObject *Sender);
	void __fastcall ButtonDBFindClick(TObject *Sender);
	void __fastcall ButtonDBOpenClick(TObject *Sender);
private:	// User declarations
    void ShowReport(int num);
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
