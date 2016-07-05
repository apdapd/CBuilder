object FormProt: TFormProt
  Left = 472
  Top = 261
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Протокол работы принтера'
  ClientHeight = 261
  ClientWidth = 269
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Memo: TMemo
    Left = 0
    Top = 0
    Width = 269
    Height = 233
    Align = alTop
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
    OnKeyDown = MemoKeyDown
  end
  object BtnClose: TButton
    Left = 0
    Top = 232
    Width = 265
    Height = 25
    Caption = 'Закрыть'
    ModalResult = 1
    TabOrder = 1
  end
end
