object FormKKM: TFormKKM
  Left = 0
  Top = 0
  Caption = 'FormKKM'
  ClientHeight = 393
  ClientWidth = 763
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsMDIForm
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object EdtComm: TEdit
    Left = 80
    Top = 8
    Width = 49
    Height = 21
    TabOrder = 0
    Text = 'COM1'
  end
  object ButtonStart: TButton
    Left = 288
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Start'
    TabOrder = 1
    OnClick = ButtonStartClick
  end
  object EdtBaudRate: TEdit
    Left = 161
    Top = 8
    Width = 48
    Height = 21
    TabOrder = 2
    Text = '19200'
  end
  object ButtonZReport: TButton
    Left = 56
    Top = 104
    Width = 97
    Height = 25
    Caption = 'Z-'#1086#1090#1095#1105#1090
    TabOrder = 3
    OnClick = ButtonZReportClick
  end
  object ButtonXReport: TButton
    Left = 56
    Top = 152
    Width = 97
    Height = 25
    Caption = 'X-'#1086#1090#1095#1105#1090
    TabOrder = 4
    OnClick = ButtonXReportClick
  end
end
