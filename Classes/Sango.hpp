#ifndef MySan
#define MySan

#include "limits.h"

#define SBRProt    0
#define PRGProt    1

#define WM_QUITMESSAGE   WM_USER + 2
#define WM_SWITCHMESSAGE WM_USER + 3

#define DCNothing  0
#define DCStart    1
#define DCStop     2
#define DCPause    3
#define DCContinue 4
#define DCAbort    5
#define DCPauseDep 31

#define DepIns   1
#define DepEdit  0
#define DepDel  -1

#define DEAllOK           0 // Без ошибок
#define DEEngActiveOver 101 //
#define DEFirstImpOver  102 //
#define DEAllImpOver    103 //
#define DELastImpOver   104 //

#define PuOK       1
#define PuNO       0
#define PuW       100

#define ProtMess   0
#define POError    2
#define SBRError   3
#define HardError  4
#define OpError    5
#define MessageOnly 6

#include "ErrInfo.h"
#include "SanDep.h"
#include "TechInfo.h"
#include "BarCode.h"
#include "StGoods.h"
#include "CashList.h"

#endif
