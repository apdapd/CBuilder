object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 538
  ClientWidth = 829
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsMDIForm
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 567
    Top = 55
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 0
    Visible = False
    OnClick = Button1Click
  end
  object Memo1: TMemo
    Left = 320
    Top = 8
    Width = 241
    Height = 65
    Lines.Strings = (
      #1058#1077#1089#1090' '#1088#1072#1073#1086#1090#1099)
    TabOrder = 1
    Visible = False
  end
  object EditNumEnd: TEdit
    Left = 104
    Top = 416
    Width = 49
    Height = 21
    TabOrder = 2
    Text = '1858'
  end
  object ButtonReport: TButton
    Left = 200
    Top = 414
    Width = 75
    Height = 25
    Caption = 'ButtonReport'
    TabOrder = 3
    OnClick = ButtonReportClick
  end
  object DBGrid1: TDBGrid
    Left = 567
    Top = 104
    Width = 217
    Height = 73
    DataSource = DataSource1
    TabOrder = 4
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    Visible = False
  end
  object ButtonDBOpen: TButton
    Left = 264
    Top = 230
    Width = 105
    Height = 25
    Caption = #1057#1086#1077#1076#1080#1085#1077#1085#1080#1077
    TabOrder = 5
    OnClick = ButtonDBOpenClick
  end
  object EditDBName: TEdit
    Left = 40
    Top = 232
    Width = 218
    Height = 21
    TabOrder = 6
    Text = 'D:\Test.gdb'
  end
  object ButtonDBFind: TButton
    Left = 64
    Top = 190
    Width = 170
    Height = 25
    Caption = #1055#1086#1080#1089#1082' '#1092#1072#1081#1083#1072' '#1041#1044
    TabOrder = 7
    OnClick = ButtonDBFindClick
  end
  object MainIBTransaction: TIBTransaction
    DefaultDatabase = MainIBDatabase
    Left = 96
    Top = 8
  end
  object IBSQLUpdate111: TIBUpdateSQL
    Left = 776
    Top = 8
  end
  object MainIBDatabase: TIBDatabase
    DatabaseName = 'D:\Test.GDB'
    Params.Strings = (
      'user_name=sysdba'
      'password=masterkey'
      'lc_ctype=WIN1251')
    LoginPrompt = False
    DefaultTransaction = MainIBTransaction
    ServerType = 'IBServer'
    Left = 32
    Top = 8
  end
  object IBSQLUpdate: TIBQuery
    Database = MainIBDatabase
    Transaction = MainIBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    ParamCheck = True
    Left = 712
    Top = 8
  end
  object MainMenu1: TMainMenu
    OwnerDraw = True
    Left = 176
    object N1: TMenuItem
      Caption = '1'
      object KKMItem: TMenuItem
        Caption = #1050#1050#1052
        OnClick = KKMItemClick
      end
      object DBItem: TMenuItem
        Caption = #1041#1072#1079#1072
      end
    end
  end
  object IBQuery1: TIBQuery
    Database = MainIBDatabase
    Transaction = MainIBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    ParamCheck = True
    SQL.Strings = (
      'select '
      '    tov_all.depart,'
      '    tov_all.codfuel,'
      '    sum(tov_all.amount*1.0 /tov_all.divide) amount, '
      '    sum(tov_all.pay*0.01) pay,'
      '    sum(tov_all.scpay*0.01) scpay,'
      '    tov_all.kind'
      ''
      'from tov_all'
      'where  ( numend  =  :numend)'
      'group by tov_all.depart, tov_all.codfuel, tov_all.kind'
      'order by tov_all.depart, tov_all.codfuel, tov_all.kind')
    Left = 24
    Top = 488
    ParamData = <
      item
        DataType = ftInteger
        Name = 'numend'
        ParamType = ptInput
      end>
  end
  object IBStoredProc1: TIBStoredProc
    Database = MainIBDatabase
    Transaction = MainIBTransaction
    StoredProcName = 'DAYEND'
    Left = 112
    Top = 488
    ParamData = <
      item
        DataType = ftInteger
        Name = 'DEPART'
        ParamType = ptOutput
      end
      item
        DataType = ftInteger
        Name = 'CODFUEL'
        ParamType = ptOutput
      end
      item
        DataType = ftFloat
        Name = 'AMOUNT'
        ParamType = ptOutput
      end
      item
        DataType = ftFloat
        Name = 'PAY'
        ParamType = ptOutput
      end
      item
        DataType = ftFloat
        Name = 'SCPAY'
        ParamType = ptOutput
      end
      item
        DataType = ftString
        Name = 'KINDNAME'
        ParamType = ptOutput
      end
      item
        DataType = ftInteger
        Name = 'NUMEND1'
        ParamType = ptInput
      end>
  end
  object frxDBDataset1: TfrxDBDataset
    UserName = 'Group'
    CloseDataSource = False
    DataSet = IBQuery2
    BCDToCurrency = False
    Left = 344
    Top = 488
  end
  object frxReport1: TfrxReport
    Version = '4.12.13'
    DotMatrixReport = False
    IniFile = '\Software\Fast Reports'
    PreviewOptions.Buttons = [pbPrint, pbLoad, pbSave, pbExport, pbZoom, pbFind, pbOutline, pbPageSetup, pbTools, pbEdit, pbNavigator, pbExportQuick]
    PreviewOptions.Zoom = 1.000000000000000000
    PrintOptions.Printer = 'Default'
    PrintOptions.PrintOnSheet = 0
    ReportOptions.CreateDate = 42546.498874432900000000
    ReportOptions.LastChange = 42548.581740601850000000
    ScriptLanguage = 'PascalScript'
    ScriptText.Strings = (
      ''
      'begin'
      ''
      'end.')
    Left = 480
    Top = 480
    Datasets = <
      item
        DataSet = frxDBDataset1
        DataSetName = 'Group'
      end>
    Variables = <>
    Style = <>
    object Data: TfrxDataPage
      Height = 1000.000000000000000000
      Width = 1000.000000000000000000
    end
    object Page1: TfrxReportPage
      PaperWidth = 210.000000000000000000
      PaperHeight = 297.000000000000000000
      PaperSize = 9
      LeftMargin = 10.000000000000000000
      RightMargin = 10.000000000000000000
      TopMargin = 10.000000000000000000
      BottomMargin = 10.000000000000000000
      object ReportTitle1: TfrxReportTitle
        Height = 22.677180000000000000
        Top = 18.897650000000000000
        Width = 718.110700000000000000
      end
      object MasterData1: TfrxMasterData
        Height = 22.677180000000000000
        Top = 147.401670000000000000
        Width = 718.110700000000000000
        DataSet = frxDBDataset1
        DataSetName = 'Group'
        RowCount = 0
        object frxDBDataset1AMOUNT: TfrxMemoView
          Left = 139.842610000000000000
          Width = 102.047310000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'AMOUNT'
          DataSet = frxDBDataset1
          DataSetName = 'Group'
          Memo.UTF8W = (
            '[Group."AMOUNT"]')
        end
        object frxDBDataset1PAY: TfrxMemoView
          Left = 313.700990000000000000
          Width = 94.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'PAY'
          DataSet = frxDBDataset1
          DataSetName = 'Group'
          Memo.UTF8W = (
            '[Group."PAY"]')
        end
        object frxDBDataset1SCPAY: TfrxMemoView
          Left = 423.307360000000000000
          Width = 79.370130000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'SCPAY'
          DataSet = frxDBDataset1
          DataSetName = 'Group'
          Memo.UTF8W = (
            '[Group."SCPAY"]')
        end
        object frxDBDataset1KIND: TfrxMemoView
          Left = 525.354670000000000000
          Top = 3.779530000000000000
          Width = 192.756030000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'KINDNAME'
          DataSet = frxDBDataset1
          DataSetName = 'Group'
          Memo.UTF8W = (
            '[Group."KINDNAME"]')
        end
      end
      object PageFooter1: TfrxPageFooter
        Height = 22.677180000000000000
        Top = 275.905690000000000000
        Width = 718.110700000000000000
        object Memo1: TfrxMemoView
          Left = 642.520100000000000000
          Width = 75.590600000000000000
          Height = 18.897650000000000000
          ShowHint = False
          HAlign = haRight
          Memo.UTF8W = (
            '[Page#]')
        end
      end
      object GroupHeader1: TfrxGroupHeader
        Height = 22.677180000000000000
        Top = 102.047310000000000000
        Width = 718.110700000000000000
        Condition = 'Group."DEPART"'
        object frxDBDataset1DEPART: TfrxMemoView
          Left = 49.133890000000000000
          Width = 79.370130000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'DEPART'
          DataSet = frxDBDataset1
          DataSetName = 'Group'
          Memo.UTF8W = (
            '[Group."DEPART"]')
        end
        object Memo4: TfrxMemoView
          Left = 11.338590000000000000
          Width = 30.236240000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Memo.UTF8W = (
            #1058#1056#1050)
        end
      end
      object GroupFooter1: TfrxGroupFooter
        Height = 22.677180000000000000
        Top = 192.756030000000000000
        Width = 718.110700000000000000
        object Memo2: TfrxMemoView
          Left = 136.063080000000000000
          Width = 105.826840000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Memo.UTF8W = (
            '[SUM(<Group."AMOUNT">,MasterData1)]')
        end
        object Memo5: TfrxMemoView
          Left = 11.338590000000000000
          Width = 49.133890000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Memo.UTF8W = (
            #1048#1090#1086#1075#1086)
        end
        object Memo3: TfrxMemoView
          Left = 313.700990000000000000
          Width = 94.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Memo.UTF8W = (
            '[SUM(<Group."PAY">,MasterData1)]')
        end
      end
    end
  end
  object DataSource1: TDataSource
    DataSet = IBQuery1
    Left = 768
    Top = 184
  end
  object frxPDFExport1: TfrxPDFExport
    UseFileCache = True
    ShowProgress = True
    OverwritePrompt = False
    DataOnly = False
    PrintOptimized = False
    Outline = False
    Background = False
    HTMLTags = True
    Author = 'FastReport'
    Subject = 'FastReport PDF export'
    ProtectionFlags = [ePrint, eModify, eCopy, eAnnot]
    HideToolbar = False
    HideMenubar = False
    HideWindowUI = False
    FitWindow = False
    CenterWindow = False
    PrintScaling = False
    Left = 560
    Top = 480
  end
  object frxHTMLExport1: TfrxHTMLExport
    UseFileCache = True
    ShowProgress = True
    OverwritePrompt = False
    DataOnly = False
    FixedWidth = True
    Background = False
    Centered = False
    EmptyLines = True
    Print = False
    PictureType = gpPNG
    Left = 640
    Top = 480
  end
  object frxRTFExport1: TfrxRTFExport
    UseFileCache = True
    ShowProgress = True
    OverwritePrompt = False
    DataOnly = False
    PictureType = gpPNG
    Wysiwyg = True
    Creator = 'FastReport'
    SuppressPageHeadersFooters = False
    HeaderFooterMode = hfText
    AutoSize = False
    Left = 728
    Top = 480
  end
  object OpenDialog1: TOpenDialog
    FileName = 'D:\Test.gdb'
    InitialDir = 'D:\'
    Left = 40
    Top = 136
  end
  object frxDBDataset2: TfrxDBDataset
    UserName = 'Fuel'
    CloseDataSource = False
    DataSet = IBQuery1
    BCDToCurrency = False
    Left = 408
    Top = 480
  end
  object IBQuery2: TIBQuery
    Database = MainIBDatabase
    Transaction = MainIBTransaction
    BufferChunks = 1000
    CachedUpdates = False
    ParamCheck = True
    SQL.Strings = (
      'select * from dayend(:numend)')
    Left = 208
    Top = 488
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'numend'
        ParamType = ptUnknown
      end>
  end
end
