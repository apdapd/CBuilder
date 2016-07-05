//---------------------------------------------------------------------------

#ifndef CommandH
#define CommandH
//---------------------------------------------------------------------------
class CAnswer
{
public:
   CAnswer ( void );
   virtual ~CAnswer ( void );

   bool Ok;
   int Error;
   int Command;
   char *Answer;
   int SizeAnswer;
   char *Operands;
   int SizeOperands;
   AnsiString DevName;
   bool Last;

   virtual void SetOperands ( char *source, int Count );
   virtual void SetAnswer ( char *source, int Count );

   __property int Sender = {read = FSender, write = FSender};

   virtual AnsiString ErrorStr ( void );
   __property AnsiString ErrorStrDefault = {read = GetErrorStrDefault};
protected:
   AnsiString GetErrorStrDefault ( void );
private:
   int FSender;
};

typedef void __fastcall (__closure *TEndAnswer)( CAnswer *Answer, const bool NextCmd );

class CCommand
{
public:
   CCommand ( void );
   virtual ~CCommand ( void );

   //true - елси надо ждать ответа
   virtual bool Exec ( void ) = 0;
   __property TEndAnswer EndAnswer = {read = FEndAnswer, write = FEndAnswer};

   __property int Sender = {read = FSender, write = FSender};

   static AnsiString GetErrorStr ( int Index );

   CAnswer *DropAnswer ( void );
protected:
   //передать ответ дальше
   virtual void CommitAnswer ( CAnswer *Answer, const bool NextCmd );
private:
   TEndAnswer FEndAnswer;
   int FSender;
   CAnswer *LastAnswer;
};

#endif
