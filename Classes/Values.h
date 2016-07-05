//---------------------------------------------------------------------------

#ifndef ValuesH
#define ValuesH
//---------------------------------------------------------------------------

//класс переменных для фискальной печати
class CValues
{
public:
   CValues ( void );
   ~CValues ( void );

   __property AnsiString Value[AnsiString Index] = {read = GetValue,
      write = SetValue};
   __property AnsiString ValueByInt[int Index] = {read = GetValueInt,
      write = SetValueInt};
   __property int CountValues = {read = GetCountValues};

   void Clear ( void );
   void DeleteValue ( AnsiString Name );
   AnsiString GetNameValue ( int Index );

   bool IsExistValue ( AnsiString Name );
   bool IsExistValue ( int Number );
protected:
   int Find ( AnsiString Name );
   void SetValue ( AnsiString Index, AnsiString Value );
   AnsiString GetValue ( AnsiString Index );
   void SetValueInt ( int Index, AnsiString Value );
   AnsiString GetValueInt ( int Index );

   int GetCountValues ( void );
private:
   TStringList *Names, *Values;
};

#endif
 