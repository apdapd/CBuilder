//---------------------------------------------------------------------------

#ifndef StGoodsH
#define StGoodsH
//---------------------------------------------------------------------------

#include "limits.h"
#include "BarCode.h"

//ограничения для мифаре карт
struct MIFARE_CARD_LIMIT
{
   bool Transit;			//транзит
   bool MayLitre;			//можно литры
   bool MayMoney;			//можно деньги
};

class StGoods
{
public:

   enum {
      KOD_PRED_SIZE = 4,
	  ALL_CODE_SIZE = 60,
//	  NAMEGSM_LENGTH = 25,
   };

  int Depart;
  long CodeGSM;
  int Volume;
  int VolumeR;
  int Divide;
  TDateTime BuyTime;
  TDateTime ReturnTime;
  int PriceGSM;     // Цена
  int ScPrice;      // Скидка с цены
  int Pay;          //
  int MoneyCount;
  UINT uTicket;
  int CardType;
  int Npost;
  int Account;
  int TicketKind;
  char NameGSM[NAMEGSM_LENGTH];
  BarCode CardCode;
  char Kod_Pred[KOD_PRED_SIZE];
  char AllCode[ALL_CODE_SIZE];
  bool InnStop;
  bool SocioExist;
  bool TicketPrint;
  bool PinAsk;
  int TerminalNumber;
  int BonusCurr;
  int BonusSumm;
  bool BonusExist;
  float BonusP;
  float MoneyP;

  MIFARE_CARD_LIMIT MifareLimit;

   StGoods ( void );

   void SetToNullAll ( void );

   void SetToNull ( void );

   StGoods* StGoods::Copy ( StGoods * NewGoods );

};

typedef StGoods CashGoods;

#endif
 