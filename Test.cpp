//---------------------------------------------------------------------------

#include <vcl.h>
#include <Registry.hpp>
#pragma hdrstop

#include "Test.h"
#include "UnitKKM.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "frxExportHTML"
#pragma link "frxExportPDF"
#pragma link "frxExportRTF"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
AnsiString __fastcall Regread(AnsiString folder,AnsiString Key,int mode)
{
 AnsiString S="";
 int p,p1;
 TRegistry *Registry = new TRegistry;
 try
 {
  Registry->RootKey = HKEY_LOCAL_MACHINE;
  if(Registry->OpenKey(folder,false))
  {
   if(mode==0)
    S = Registry->ReadString(Key);
   if(mode==1)
    Registry->WriteString("ServerN",Key);
   if(mode==2)
    Registry->WriteString("DB",Key);

  }
  Registry->CloseKey();
 }
 __finally
 {
  delete Registry;
 }
 return S;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
 AnsiString NameConnect = "D:\\Test.gdb";
//Regread("SOFTWARE\\Санго Плюс\\arm","Database",0);
 try
  {

/*
   Memo1->Text = "Идёт выполнение";
   Button1->Visible = false;

   MainIBDatabase->Close();
   MainIBDatabase->DatabaseName = NameConnect;
   MainIBDatabase->Open();
   MainIBTransaction->StartTransaction();


   IBSQLUpdate->Close();

   AnsiString Stt = "select max(numend) numend from CURRENT_PHAS ";

   IBSQLUpdate->SQL->Clear();
   IBSQLUpdate->SQL->Add(Stt);
   IBSQLUpdate->Open();
   int nn = IBSQLUpdate->FieldByName("numend")->AsInteger;

   IBSQLUpdate->Close();

   Stt = Format("delete from CURRENT_PHAS where numend = %d", OPENARRAY(TVarRec,((int)nn)));

   IBSQLUpdate->SQL->Clear();
   IBSQLUpdate->SQL->Add(Stt);
   IBSQLUpdate->ExecSQL(); //ExecQuery();
   IBSQLUpdate->Close();

   MainIBTransaction->Commit();
   MainIBDatabase->Connected = false;
   Close();
*/
  }
 catch (Exception &exception)
  {
   MessageBox( NULL, exception.Message.c_str(), L"Ошибка", MB_OK );
   MainIBTransaction->Rollback();
   MainIBDatabase->Connected = false;
  }
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::KKMItemClick(TObject *Sender)
{
 FormKKM->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonReportClick(TObject *Sender)
{
 int i = EditNumEnd->Text.ToInt();
 ShowReport(i);
}
//---------------------------------------------------------------------------
void TForm1::ShowReport(int num)
{
 IBQuery2->Close();
 IBQuery2->ParamByName("numend")->AsInteger = num;
 IBQuery2->Open();

// AnsiString FName = "Report1.fr3";
// if (FileExists(FName))
//   frxReport1->LoadFromFile(FName);

 frxReport1->ShowReport();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonDBFindClick(TObject *Sender)
{
  OpenDialog1->Filter = "БД (*.gdb; *.fdb) |*.gdb; *.fdb";
  if (OpenDialog1->Execute())
	// First, check if the file exists.
	if (FileExists(OpenDialog1->FileName))
	  // If it exists, load the data into the memo box.
	  EditDBName->Text = OpenDialog1->FileName;
	else
	  // Otherwise, throw an exception.
	  throw(Exception("File does not exist."));

}
//---------------------------------------------------------------------------


void __fastcall TForm1::ButtonDBOpenClick(TObject *Sender)
{
 try
  {
//   Memo1->Text = "Идёт выполнение";
//   Button1->Visible = false;
   AnsiString NameConnect = EditDBName->Text;
   MainIBDatabase->Close();
   MainIBDatabase->DatabaseName = NameConnect;
   MainIBDatabase->Open();
  }
 catch (Exception &exception)
  {
   MessageBox( NULL, exception.Message.c_str(), L"Ошибка", MB_OK );
//   MainIBTransaction->Rollback();
//   MainIBDatabase->Connected = false;
  }

}
//---------------------------------------------------------------------------

