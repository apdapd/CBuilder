/******************************************************************************/
/***          Generated by IBExpert 2011.04.25 27.06.2016 10:45:06          ***/
/******************************************************************************/

SET SQL DIALECT 3;

SET NAMES WIN1251;

CREATE DATABASE 'D:\Test.GDB'
USER 'sysdba' PASSWORD 'masterkey'
PAGE_SIZE 4096
DEFAULT CHARACTER SET WIN1251;



/******************************************************************************/
/***                                Domains                                 ***/
/******************************************************************************/

CREATE DOMAIN ACCOUNT AS
INTEGER;

CREATE DOMAIN AMOUNT AS
INTEGER
NOT NULL;

CREATE DOMAIN BLACK AS
CHAR(1);

CREATE DOMAIN CARDCODE AS
VARCHAR(11);

CREATE DOMAIN CHANNEL AS
SMALLINT
NOT NULL;

CREATE DOMAIN CHAR10 AS
VARCHAR(10);

CREATE DOMAIN CHAR20 AS
VARCHAR(20);

CREATE DOMAIN CHAR25 AS
VARCHAR(25)
NOT NULL;

CREATE DOMAIN CHAR30 AS
VARCHAR(30);

CREATE DOMAIN CHAR60 AS
VARCHAR(60)
NOT NULL;

CREATE DOMAIN CODFUEL AS
INTEGER
NOT NULL;

CREATE DOMAIN COD_CH AS
SMALLINT;

CREATE DOMAIN COD_G AS
SMALLINT;

CREATE DOMAIN COD_S AS
SMALLINT;

CREATE DOMAIN COD_Y AS
SMALLINT;

CREATE DOMAIN CONDIT AS
INTEGER;

CREATE DOMAIN DATETIME AS
TIMESTAMP;

CREATE DOMAIN DEPART AS
INTEGER
NOT NULL;

CREATE DOMAIN FIO AS
VARCHAR(40)
NOT NULL;

CREATE DOMAIN GASATERIA AS
SMALLINT
NOT NULL;

CREATE DOMAIN GRAN AS
INTEGER
NOT NULL;

CREATE DOMAIN HIGHT AS
INTEGER;

CREATE DOMAIN INTG AS
INTEGER;

CREATE DOMAIN KILOS AS
INTEGER;

CREATE DOMAIN KIND AS
SMALLINT;

CREATE DOMAIN KLIENT AS
VARCHAR(6);

CREATE DOMAIN KOD_CITY AS
VARCHAR(7);

CREATE DOMAIN KOD_PRED AS
CHAR(4);

CREATE DOMAIN LEVEL_ AS
SMALLINT
NOT NULL;

CREATE DOMAIN MARK AS
CHAR(1);

CREATE DOMAIN NAME AS
VARCHAR(18);

CREATE DOMAIN NAME_CH AS
CHAR(20)
NOT NULL;

CREATE DOMAIN NAME_G AS
CHAR(20)
NOT NULL;

CREATE DOMAIN NAME_S AS
CHAR(20)
NOT NULL;

CREATE DOMAIN NAME_Y AS
CHAR(20)
NOT NULL;

CREATE DOMAIN NCITY AS
SMALLINT;

CREATE DOMAIN NCOMP AS
SMALLINT;

CREATE DOMAIN NOMCHK AS
INTEGER;

CREATE DOMAIN NOTE AS
VARCHAR(50);

CREATE DOMAIN NPOST AS
INTEGER;

CREATE DOMAIN NUM AS
SMALLINT;

CREATE DOMAIN NUMEND AS
INTEGER
NOT NULL;

CREATE DOMAIN NUMOUT AS
NUMERIC(15,0);

CREATE DOMAIN ONLYTIME AS
TIME;

CREATE DOMAIN PAY AS
INTEGER
DEFAULT 0
NOT NULL;

CREATE DOMAIN PIN AS
SMALLINT;

CREATE DOMAIN PRICE AS
INTEGER
default 0
NOT NULL;

CREATE DOMAIN QUALIFICATION AS
SMALLINT
NOT NULL;

CREATE DOMAIN QUANT AS
SMALLINT;

CREATE DOMAIN RESERVOIR AS
INTEGER
NOT NULL;

CREATE DOMAIN R_VOL AS
DOUBLE PRECISION;

CREATE DOMAIN SENDER AS
INTEGER
NOT NULL;

CREATE DOMAIN STAT AS
SMALLINT
DEFAULT -2
NOT NULL;

CREATE DOMAIN SUMMA AS
INTEGER;

CREATE DOMAIN SYSON AS
SMALLINT
DEFAULT 0
NOT NULL;

CREATE DOMAIN TEMPERATURE AS
INTEGER;

CREATE DOMAIN TERMKIND AS
SMALLINT;

CREATE DOMAIN THICKNESS AS
FLOAT;

CREATE DOMAIN VALUE_ AS
INTEGER
NOT NULL;



/******************************************************************************/
/***                               Generators                               ***/
/******************************************************************************/

CREATE GENERATOR NVER;
SET GENERATOR NVER TO 20110412;

CREATE GENERATOR PROTMESS_CODRECORD_GEN;
SET GENERATOR PROTMESS_CODRECORD_GEN TO 0;

CREATE GENERATOR TOV_ALL_CODRECORD_GEN1;
SET GENERATOR TOV_ALL_CODRECORD_GEN1 TO 0;



SET TERM ^ ; 



/******************************************************************************/
/***                           Stored Procedures                            ***/
/******************************************************************************/

CREATE PROCEDURE BNALPROC (
    NUMEND1 INTEGER)
RETURNS (
    CODG1 INTEGER,
    FUELNAME VARCHAR(18),
    S_AMOUNT FLOAT,
    KINDNAME VARCHAR(28))
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE CHECKSER (
    SER CHAR(3))
RETURNS (
    RET INTEGER)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE DAYEND (
    NUMEND1 INTEGER)
RETURNS (
    DEPART INTEGER,
    CODFUEL INTEGER,
    AMOUNT FLOAT,
    PAY FLOAT,
    SCPAY FLOAT,
    KINDNAME VARCHAR(28))
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE DSPROC (
    NUMEND1 INTEGER)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE FILLFUELREMS (
    NUMEND INTEGER)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE FILLFUELREMS1 (
    NUMEND INTEGER)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE FILLGOODSMOVING (
    NUMEND INTEGER)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE FS_ITOG (
    PNUM1 INTEGER,
    PNUM2 INTEGER,
    COD1 INTEGER,
    COD2 INTEGER)
RETURNS (
    CODFUEL INTEGER,
    NAME CHAR(20),
    PAY DOUBLE PRECISION,
    PAY_L DOUBLE PRECISION,
    PAY_N DOUBLE PRECISION,
    PAY_P DOUBLE PRECISION,
    PAY_UR DOUBLE PRECISION,
    PAY_T DOUBLE PRECISION,
    PAY_UL DOUBLE PRECISION,
    AMOUNT DOUBLE PRECISION,
    AMOUNT_L DOUBLE PRECISION,
    AMOUNT_N DOUBLE PRECISION,
    AMOUNT_P DOUBLE PRECISION,
    AMOUNT_T DOUBLE PRECISION,
    AMOUNT_UR DOUBLE PRECISION,
    AMOUNT_UL DOUBLE PRECISION)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE GETPISTOLS
RETURNS (
    DEPART INTEGER,
    RESERVOIRID INTEGER,
    STATE INTEGER,
    CODFUEL INTEGER)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE INS_CURRGSM (
    RESERVOIR1 INTEGER,
    AMOUNT1 INTEGER,
    THICKNESS1 FLOAT,
    CODFUEL1 INTEGER,
    NUMEND1 INTEGER)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE INS_CURRPHAS (
    CODFUEL1 INTEGER,
    AMOUNT1 INTEGER,
    PRICE1 INTEGER,
    NUMEND1 INTEGER)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE INSDIM (
    NUMEND1 INTEGER)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE ISPULTSYS
RETURNS (
    PULTSYS INTEGER)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE JOB_ADD_NEW (
    TYPEJOB INTEGER,
    DATESTART TIMESTAMP)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE JOB_AVERAGE_TRY (
    TYPEJOB INTEGER,
    DATELASTTRY TIMESTAMP)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE JOB_CLOSE (
    TYPEJOB INTEGER,
    RESULT INTEGER)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE JOB_CURRENT (
    TYPEJOB INTEGER)
RETURNS (
    ID INTEGER,
    DATESTART TIMESTAMP,
    STATE INTEGER,
    RESULT INTEGER,
    DATELASTTRY TIMESTAMP,
    TRYCOUNT INTEGER)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE JOB_SET_STATE (
    TYPEJOB INTEGER,
    NEWSTATE INTEGER)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE KLIENTPUMP (
    KOD_CITY1 VARCHAR(7),
    KLIENT1 VARCHAR(6),
    KOD_PRED1 CHAR(4),
    PIN1 SMALLINT)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE MAKEENDCHANGE (
    NUMEND INTEGER)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE MONEY_STACKER_INC (
    C INTEGER,
    NOM INTEGER,
    TERMNUMBER INTEGER)
RETURNS (
    CN INTEGER)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE MONEY_STACKER_INKASS (
    TERMNUMBER INTEGER)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE NALPROC (
    NUMEND1 INTEGER)
RETURNS (
    S_AMOUNT FLOAT,
    PRICE1 NUMERIC(5,2),
    S_PAY NUMERIC(5,2),
    CODG INTEGER,
    FUELNAME VARCHAR(18))
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE NUMENDDETAIL (
    NUMEND1 INTEGER)
RETURNS (
    RESERV1 INTEGER,
    CODFUEL1 INTEGER,
    VOL_R_CARD INTEGER,
    VOL_L_CARD INTEGER,
    VOL_U_CARD INTEGER,
    VOL_TAL INTEGER,
    VOL_OTHER INTEGER,
    VOL_CASH INTEGER)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE NUMENDREPORT (
    NUMEND1 INTEGER)
RETURNS (
    RESERV1 INTEGER,
    CODFUEL1 INTEGER,
    DEPART1 INTEGER,
    BEGC INTEGER,
    ENDC INTEGER,
    OSTATOK1 INTEGER,
    OSTATOK1_WEIGHT INTEGER,
    PRIHOD1 INTEGER,
    PRIHOD1_WEIGHT INTEGER,
    RASHOD1 INTEGER,
    OSTATOK2 INTEGER,
    HIGHT1 INTEGER,
    THICKNESS1 NUMERIC(4,3),
    TEMPR INTEGER)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE NUMENDREPORT1 (
    NUMEND1 INTEGER)
RETURNS (
    RESERV1 INTEGER,
    CODFUEL1 INTEGER,
    DEPART1 INTEGER,
    BEGC INTEGER,
    ENDC INTEGER,
    OSTATOK1 INTEGER,
    OSTATOK1_WEIGHT INTEGER,
    PRIHOD1 INTEGER,
    PRIHOD1_WEIGHT INTEGER,
    RASHOD1 INTEGER,
    OSTATOK2 INTEGER,
    HIGHT1 INTEGER,
    THICKNESS1 NUMERIC(4,3),
    TEMPR INTEGER)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE NUMENDREPTOT (
    NUMEND1 INTEGER)
RETURNS (
    RESERV1 INTEGER,
    CODFUEL1 INTEGER,
    OSTATOK1 NUMERIC(8,2),
    PRIHOD1 NUMERIC(8,2),
    RASHOD1 NUMERIC(8,2),
    OSTATOK2 NUMERIC(8,2))
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE NUMENFDETAIL (
    NUMEND1 INTEGER)
RETURNS (
    CODFUEL1 INTEGER,
    VOL_R_CARD INTEGER,
    VOL_L_CARD INTEGER,
    VOL_U_CARD INTEGER,
    VOL_TAL INTEGER,
    VOL_OTHER INTEGER,
    VOL_CASH INTEGER,
    OST2 DOUBLE PRECISION,
    PRIHOD DOUBLE PRECISION,
    RASHOD DOUBLE PRECISION,
    FUELNAME VARCHAR(18))
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE OSTATOK (
    NUMEND1 INTEGER)
RETURNS (
    FUELNAME VARCHAR(18),
    RESERV1 INTEGER,
    DEADBAL INTEGER,
    AMOUNT1 FLOAT,
    MASSA FLOAT)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE PHASREP (
    NUMEND1 INTEGER)
RETURNS (
    FUELNAME VARCHAR(256),
    PRICE1 DOUBLE PRECISION,
    PRIHOD DOUBLE PRECISION,
    RASHOD DOUBLE PRECISION,
    OST2 DOUBLE PRECISION,
    OST1 DOUBLE PRECISION)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE PHASREP_MOVING (
    NUMEND1 INTEGER)
RETURNS (
    CODFUEL INTEGER,
    PRICE1 DOUBLE PRECISION,
    PRIHOD DOUBLE PRECISION,
    RASHOD DOUBLE PRECISION,
    OST2 DOUBLE PRECISION)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE POKUPUP (
    KOD_CITY1 VARCHAR(7),
    KLIENT1 VARCHAR(6),
    PAY1 INTEGER,
    AMOUNT1 INTEGER,
    TRSTOP INTEGER)
AS
BEGIN
  EXIT;
END^





CREATE PROCEDURE PRIHODPH (
    NUMEND1 INTEGER)
RETURNS (
    SENDERNAME VARCHAR(50),
    COD INTEGER,
    FUELNAME VARCHAR(18),
    AMOUNT1 INTEGER,
    PRICE1 FLOAT)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE PRINT_ID_PROC
RETURNS (
    ID INTEGER)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE PV_ITOG (
    PNCOMP INTEGER,
    PNUM_B INTEGER,
    PNUM_E INTEGER,
    COD_F INTEGER,
    NKIND INTEGER)
RETURNS (
    NUMEND INTEGER,
    PRICE DOUBLE PRECISION,
    PAY DOUBLE PRECISION,
    AMOUNT DOUBLE PRECISION)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE PV_PRICE (
    PNCOMP INTEGER,
    PNUM_B INTEGER,
    PNUM_E INTEGER,
    COD_F INTEGER,
    NKIND INTEGER)
RETURNS (
    PRICE DOUBLE PRECISION)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE PV_PROT (
    PNCOMP INTEGER,
    PNUM_B INTEGER,
    PNUM_E INTEGER)
RETURNS (
    NUMEND INTEGER,
    DATE_END DATE)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE PV_PRT (
    PNCOMP INTEGER,
    PNUM_B INTEGER,
    PNUM_E INTEGER,
    COD_F INTEGER,
    NKIND INTEGER)
RETURNS (
    NUMEND INTEGER,
    DATE_END DATE,
    PRICE DOUBLE PRECISION)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE PV_TOV (
    PNCOMP INTEGER,
    PNUM_B INTEGER,
    PNUM_E INTEGER,
    COD_F INTEGER,
    NKIND INTEGER)
RETURNS (
    PRICE DOUBLE PRECISION,
    NUMEND INTEGER,
    PAY DOUBLE PRECISION,
    AMOUNT DOUBLE PRECISION)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE REPRICE (
    CODFUEL1 INTEGER,
    NEWPRICE INTEGER,
    ADDAMOUNT INTEGER,
    NUMEND INTEGER)
RETURNS (
    CODREC INTEGER)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE SHOWREST (
    NUMEND INTEGER)
RETURNS (
    RESERVID INTEGER,
    RECEPTVOL DOUBLE PRECISION,
    EXCEPSEVOL DOUBLE PRECISION,
    VOLUME_E DOUBLE PRECISION,
    VOLUME_B DOUBLE PRECISION,
    SDV_DAY DOUBLE PRECISION,
    SDV_MON DOUBLE PRECISION,
    NAME VARCHAR(26))
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE TALOST (
    NUMEND INTEGER)
RETURNS (
    SUMRET INTEGER)
AS
BEGIN
  SUSPEND;
END^





CREATE PROCEDURE TRANSPASSW
AS
BEGIN
  EXIT;
END^






SET TERM ; ^



/******************************************************************************/
/***                                 Tables                                 ***/
/******************************************************************************/


CREATE TABLE FUEL (
    CODFUEL  CODFUEL,
    NAME     NAME NOT NULL,
    NUM      NUM,
    F_STOP   SMALLINT,
    RATE     SMALLINT,
    EXCISE   SMALLINT,
    COD_G    COD_G,
    COD_S    COD_S,
    COD_1    COD_CH,
    COD_2    COD_CH,
    COD_3    COD_CH,
    COD_4    COD_CH,
    COD_5    COD_CH
);


CREATE TABLE PROTMESS (
    CODRECORD  INTEGER NOT NULL,
    DATETIME   DATETIME,
    MESKIND    SMALLINT,
    MODULNUM   SMALLINT,
    ERRNUM     SMALLINT,
    MESSTRING  VARCHAR(500),
    NUMEND     NUMEND NOT NULL,
    NUMOUT     NUMOUT
);

CREATE TABLE PROTOCOL (
    NUMEND    NUMEND NOT NULL,
    DATE_BEG  DATETIME NOT NULL,
    DATE_END  DATETIME,
    NUMOUT    NUMOUT
);


CREATE TABLE TECHKIND (
    KIND         KIND NOT NULL,
    NAME         CHAR(28),
    TICKETPRINT  SMALLINT,
    NCOMP        NCOMP
);


CREATE TABLE TOV_ALL (
    CODRECORD  INTEGER NOT NULL,
    NOMCHK     NOMCHK,
    DEPART     SMALLINT,
    DATEPAY    DATETIME,
    NUM        NUM,
    NUMEND     NUMEND NOT NULL,
    CODFUEL    CODFUEL NOT NULL,
    AMOUNT     AMOUNT,
    ACCOUNT    ACCOUNT,
    NPOST      INTEGER,
    PRICE      PRICE,
    PAY        PRICE,
    SCPAY      PAY,
    KIND       KIND,
    DIVIDE     SMALLINT NOT NULL,
    CARDCODE   VARCHAR(25),
    KOD_CITY   KOD_CITY,
    KLIENT     KLIENT,
    KOD_PRED   KOD_PRED,
    AMOUNTR    INTG,
    NUMOUT     NUMOUT,
    TERMKIND   TERMKIND,
    BONUSCURR  INTEGER,
    BONUSSUMM  INTEGER
);



/******************************************************************************/
/***                              Primary Keys                              ***/
/******************************************************************************/

ALTER TABLE FUEL ADD CONSTRAINT PK_FUEL PRIMARY KEY (CODFUEL);
ALTER TABLE PROTMESS ADD CONSTRAINT PK_PROTMESS PRIMARY KEY (CODRECORD);
ALTER TABLE PROTOCOL ADD CONSTRAINT PK_PROTOCOL PRIMARY KEY (NUMEND);
ALTER TABLE TECHKIND ADD CONSTRAINT PK_TECHKIND PRIMARY KEY (KIND);
ALTER TABLE TOV_ALL ADD CONSTRAINT PK_TOV_ALL PRIMARY KEY (CODRECORD);



/******************************************************************************/
/***                                Indices                                 ***/
/******************************************************************************/

CREATE INDEX IDXTOVALLBYFUEL ON TOV_ALL (CODFUEL);
CREATE DESCENDING INDEX IDXTOVALLBYNUMEND ON TOV_ALL (NUMEND);
CREATE INDEX IDX_TOV_ALL ON TOV_ALL (NUMEND);
CREATE INDEX IDX_TOV_ALL1 ON TOV_ALL (KOD_CITY, KLIENT);

/******************************************************************************/
/***                                Triggers                                ***/
/******************************************************************************/


SET TERM ^ ;



/******************************************************************************/
/***                          Triggers for tables                           ***/
/******************************************************************************/



/* Trigger: AI_PROTMESS_CODRECORD */
CREATE TRIGGER AI_PROTMESS_CODRECORD FOR PROTMESS
ACTIVE BEFORE INSERT POSITION 0
AS
BEGIN
      NEW.CODRECORD = GEN_ID(PROTMESS_CODRECORD_GEN, 1);
END
^

/* Trigger: AI_TOV_ALL_CODRECORD */
CREATE TRIGGER AI_TOV_ALL_CODRECORD FOR TOV_ALL
ACTIVE BEFORE INSERT POSITION 0
AS
BEGIN
  NEW.CODRECORD = GEN_ID(TOV_ALL_CODRECORD_GEN1, 1);
END
^

SET TERM ; ^



/******************************************************************************/
/***                           Stored Procedures                            ***/
/******************************************************************************/


SET TERM ^ ;


ALTER PROCEDURE DAYEND (
    NUMEND1 INTEGER)
RETURNS (
    DEPART INTEGER,
    CODFUEL INTEGER,
    AMOUNT FLOAT,
    PAY FLOAT,
    SCPAY FLOAT,
    KINDNAME VARCHAR(28))
AS
declare variable KIND integer;
begin
  /* Procedure Text */
 for
  select
    t1.depart,
    t1.codfuel,
    sum(t1.amount*1.0 /t1.divide) amount,
    sum(t1.pay*0.01) pay,
    sum(t1.scpay*0.01) scpay,
    t1.kind
/*    t2.name*/

  from tov_all t1
/*   left join techkind t2 on (t1.kind=t2.kind)*/

  where  ( numend  =  :numend1)
  group by t1.depart, t1.codfuel, t1.kind
  order by t1.depart, t1.codfuel, t1.kind
  into :depart, :codfuel, :amount, :pay, :scpay, :kind
  do
  begin
   select name from techkind
   where kind = :kind
   into :kindname;
   suspend;
  end
end^




SET TERM ; ^


/******************************************************************************/
/***                              Descriptions                              ***/
/******************************************************************************/

