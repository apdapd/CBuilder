//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "StGoods.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

StGoods::StGoods ( void )
{
   SetToNullAll();
   Depart = 0;
}

void StGoods::SetToNullAll ( void )
{
   SetToNull();
   for(int i = 0; i < NAMEGSM_LENGTH; ++i)
      NameGSM[i] = 0;
   Divide = 1000;
   CodeGSM = 0;
}

void StGoods::SetToNull ( void )
{
   CardCode.SetToNull();
   memset ( Kod_Pred, 0, KOD_PRED_SIZE );
   Volume = 0;
   VolumeR = 0;
   BuyTime = 0;
   ReturnTime = 0;
   PriceGSM = 0;
   ScPrice = 0;
   Pay = 0;
   MoneyCount = 0;
   uTicket = 0;
   CardType = 0;
   Npost = 0;
   Account = 0;
   TicketKind = 0;
   InnStop = false;
   SocioExist = false;
   PinAsk     = true;
   TicketPrint= true;
   AllCode[0] = 0;
   TerminalNumber = 0;
   BonusCurr = 0;
   BonusSumm = 0;
   BonusExist = false;
   BonusP = 0;

   BuyTime = Now();
   ReturnTime = Now();
}

StGoods* StGoods::Copy ( StGoods * NewGoods )
{
   if(NewGoods)
   {
      NewGoods->Depart     = Depart;
      NewGoods->CodeGSM    = CodeGSM;
      NewGoods->Volume     = Volume;
      NewGoods->VolumeR    = VolumeR;
      NewGoods->Divide     = Divide;
      NewGoods->BuyTime    = BuyTime;
      NewGoods->ReturnTime = ReturnTime;
      NewGoods->PriceGSM   = PriceGSM;
      NewGoods->ScPrice    = ScPrice;
      NewGoods->Pay        = Pay;
      NewGoods->MoneyCount = MoneyCount;
      NewGoods->uTicket    = uTicket;
      NewGoods->CardType   = CardType;
      NewGoods->Npost      = Npost;
      NewGoods->Account    = Account;
      NewGoods->TicketKind = TicketKind;
      NewGoods->CardCode   = CardCode;
      NewGoods->InnStop    = InnStop;
      NewGoods->SocioExist = SocioExist;
      NewGoods->TicketPrint= TicketPrint;
      NewGoods->PinAsk     = PinAsk;
      NewGoods->TerminalNumber = TerminalNumber;
      NewGoods->BonusCurr = BonusCurr;
      NewGoods->BonusSumm = BonusSumm;
      NewGoods->BonusExist = BonusExist;
      NewGoods->BonusP    = BonusP;
      NewGoods->MoneyP    = MoneyP;


      for(int i = 0; i < NAMEGSM_LENGTH; ++i)
        NewGoods->NameGSM[i] = NameGSM[i];

      for(int i = 0; i < ALL_CODE_SIZE; ++i)
        NewGoods->AllCode[i] = AllCode[i];

      for(int i = 0; i < KOD_PRED_SIZE; ++i)
        NewGoods->Kod_Pred[i] = Kod_Pred[i];
   }
   return NewGoods;
}
