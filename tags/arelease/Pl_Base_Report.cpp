//---------------------------------------------------------------------------
#include <vcl.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "Pl_Base_Report.h"
#include "Pl_Form_ReportPreview.h"
#include "Report_Consts.h"

#include "DrR_Exception.h"
#pragma package(smart_init)

extern bool PrintDialogShowed;

__fastcall TPlBaseReport::TPlBaseReport():
           TObject(),
           FPrinter(Printer()),
           Previewing(false),
           PreviewForm(NULL),
           PixelsPerMmX(0),
           PixelsPerMmY(0),
           DisplayPixelsPerMmX(0),
           DisplayPixelsPerMmY(0),
           PrinterPageWidth(0),
           PrinterPageHeight(0),
           OffsetLeft(0),
           OffsetTop(0),
           OffsetRight(0),
           OffsetBottom(0),
           KWidth(0),
           PageHeightMm(0),
           PageWidthMm(0),
           DrawTo(NULL),
           ShadowWidth(4),
           ReportBorderWidth(10),
           PreviewCantWidth(1),
           PreviewCantColor(clBlack),
           ShadowColor(clBlack),
           Decrement(ShadowWidth + 2*ReportBorderWidth + 2*PreviewCantWidth),
           RoundFonts(true),
           PrinterOffsetLeft(0),
           PrinterOffsetTop(0),
           PrinterOffsetRight(0),
           PrinterOffsetBottom(0),
           Pages(0),
           Page(0),
           LastY(0),
           DrawMode(dm_Init),
           Title(""),
           HeaderHeight(0),
           FooterHeight(0),
           PrintHeader(true),
           PrintFooter(true),
           PrintPrintDate(true),
           PrintPageNumbers(true),
           BlackWhite(false),
           FullWidth(0),
           RealFontSize(0),
           ReportData(NULL)
{
  OpenReportDataSource();
  GetPrinterParams();
  OffsetTop    = PrinterOffsetTop;
  OffsetLeft   = PrinterOffsetLeft;
  OffsetRight  = PrinterOffsetRight;
  OffsetBottom = PrinterOffsetBottom;
  GetDisplayParams();
  Init();
  FullWidth = DisplayPixelsPerMmX*PageWidthMm;
}

__fastcall TPlBaseReport::~TPlBaseReport()
{
  if (PreviewForm)
    delete PreviewForm;
}

void TPlBaseReport::OpenReportDataSource()
{
}

void TPlBaseReport::LoadReportParams(AnsiString IniSection, TPrinterOrientation DefaultOrientation)
{
  TIniFile* ini = NULL;
  AnsiString inifullnamepath = ExtractFilePath(Application->ExeName) + "\\" + LoadStr(sIniFileName);
  try {
    ini = new TIniFile(inifullnamepath);
    TPrinterOrientation orientation = (TPrinterOrientation)ini->ReadInteger(IniSection, "Orientation", (int)DefaultOrientation);
    if (orientation == poPortrait) {
      Orientation = poLandscape;
      Orientation = orientation;
    } else {
      Orientation = poPortrait;
      Orientation = orientation;
    }
    OffsetLeft = ini->ReadFloat(IniSection, "LeftOffset", PrinterOffsetLeft + 5);
    OffsetTop = ini->ReadFloat(IniSection, "TopOffset", PrinterOffsetTop + 5);
    OffsetRight = ini->ReadFloat(IniSection, "RightOffset", PrinterOffsetRight + 5);
    OffsetBottom = ini->ReadFloat(IniSection, "BottomOffset", PrinterOffsetBottom + 5);
    PrintHeader = ini->ReadBool(IniSection, "PrintHeader", true);
    PrintPrintDate = ini->ReadBool(IniSection, "PrintPrintDate", true);
    PrintPageNumbers = ini->ReadBool(IniSection, "PrintPageNumbers", true);
    PrintFooter = ini->ReadBool(IniSection, "PrintFooter", true);
    ShadowWidth = ini->ReadInteger(IniSection, "ShadowWidth", 4);
    ReportBorderWidth = ini->ReadInteger(IniSection, "ReportBorderWidth", 10);
    PreviewCantWidth = ini->ReadInteger(IniSection, "PreviewCantWidth", 1);
    ShadowColor = (TColor)ini->ReadInteger(IniSection, "ShadowColor", (int)clBlack);
    PreviewCantColor = (TColor)ini->ReadInteger(IniSection, "PreviewCantColor", (int)clBlack);
    RoundFonts = ini->ReadBool(IniSection, "RoundFonts", true);
    BlackWhite = ini->ReadBool(IniSection, "BlackWhite", false);
    delete ini;
  } catch (Exception &e) {
    if (ini)
      delete ini;
    Orientation = poLandscape;
    Orientation = poPortrait;
    OffsetLeft = PrinterOffsetLeft + 1.5;
    OffsetTop = PrinterOffsetTop + 1.5;
    OffsetRight = PrinterOffsetRight + 1.5;
    OffsetBottom = PrinterOffsetBottom + 1.5;
  }
}

void TPlBaseReport::WriteReportParams(AnsiString IniSection)
{
  TIniFile* ini = NULL;
  AnsiString inifullnamepath = ExtractFilePath(Application->ExeName) + "\\" + LoadStr(sIniFileName);
  try {
    ini = new TIniFile(inifullnamepath);
    ini->WriteInteger(IniSection, "Orientation", Orientation);
    ini->WriteFloat(IniSection, "LeftOffset", OffsetLeft);
    ini->WriteFloat(IniSection, "TopOffset", OffsetTop);
    ini->WriteFloat(IniSection, "RightOffset", OffsetRight);
    ini->WriteFloat(IniSection, "BottomOffset", OffsetBottom);
    ini->WriteBool(IniSection, "PrintHeader", PrintHeader);
    ini->WriteBool(IniSection, "PrintPrintDate", PrintPrintDate);
    ini->WriteBool(IniSection, "PrintPageNumbers", PrintPageNumbers);
    ini->WriteBool(IniSection, "PrintFooter", PrintFooter);
    ini->WriteInteger(IniSection, "ShadowWidth", ShadowWidth);
    ini->WriteInteger(IniSection, "ReportBorderWidth", ReportBorderWidth);
    ini->WriteInteger(IniSection, "PreviewCantWidth", PreviewCantWidth);
    ini->WriteInteger(IniSection, "ShadowColor", ShadowColor);
    ini->WriteInteger(IniSection, "PreviewCantColor", PreviewCantColor);
    ini->WriteBool(IniSection, "RoundFonts", RoundFonts);
    ini->WriteBool(IniSection, "BlackWhite", BlackWhite);
    delete ini;
  } catch (Exception &e) {
    if (ini)
      delete ini;
  }
}

void TPlBaseReport::Preview()
{
  DrawMode = dm_Preview;
  try {
    PreviewForm = new TPreviewForm();
    if (PreviewForm) {
      PreviewForm->Caption = Title;
      PreviewForm->Report = this;
      PreviewForm->CanResize = true;
      PreviewForm->ShowModal();
    }
    Previewing = false;
    delete PreviewForm;
    PreviewForm = NULL;
  } catch (Exception& e) {
    if (PreviewForm) {
      delete PreviewForm;
      PreviewForm = NULL;
    }
  }
}

void TPlBaseReport::Print(bool ShowPrintDialog)
{
  TCursor oldcur = Screen->Cursor;
  TDrawReportMode oldmode = DrawMode;
  int copies = 1;
  bool print = true;
  TPrintDialog* dlg = NULL;
  try {
    if (ShowPrintDialog) {
      dlg = new TPrintDialog(NULL);
      if (dlg && dlg->Execute()) {
        PrintDialogShowed = true;
        FPrinter = Printer();
        if (!(FPrinter && FPrinter->Printers && FPrinter->Printers->Count))
          throw RubberError(LoadStr(sNoPrintersError));
        Orientation = FPrinter->Orientation;
        copies = dlg->Copies;
      } else
        print = false;
      delete dlg;
      dlg = NULL;
    }
    if (print) {
      Init();
      FullWidth = DisplayPixelsPerMmX*PageWidthMm;
      CorrectOffsets();
      if (Previewing)
        PreviewForm->ReportImage->Picture->Bitmap->Canvas->FillRect(Rect(0, 0, PreviewForm->ReportImage->Picture->Bitmap->Width, PreviewForm->ReportImage->Picture->Bitmap->Height));
      DrawMode = dm_Print;
      FPrinter->Title = Title;
      FPrinter->BeginDoc();
      DrawTo = FPrinter->Canvas;
      for (int i = 0; i < copies; i++) {
        if (i)
          FPrinter->NewPage();
        Page = 1;
        DrawReport(0);
      }
     FPrinter->EndDoc();
    }
    DrawMode = oldmode;
    if (Previewing) {
        PreviewForm->ResizeReport();
        PreviewForm->SetStatusButtons();
    }
    Screen->Cursor = oldcur;
  } catch (Exception& e) {
    if (FPrinter->Printing)
      FPrinter->EndDoc();
    if (dlg)
      delete dlg;
    Screen->Cursor = oldcur;
    DrawMode = oldmode;
    throw;
  }
}

// ����������� ���������������� ���������
void TPlBaseReport::UpdateReportPreview(const int PreviewWidth, const int PreviewHeight, const int PageNumber)
{
  GetPrinterParams();
  ResetPreviewParams();
  Graphics::TBitmap* ReportBitmap = NULL;
  TCursor oldcur = Screen->Cursor;
  try {
    ReportBitmap = new Graphics::TBitmap();
    ReportBitmap->Height = 1;
    ReportBitmap->Width = 1;
    ReportBitmap->Canvas->Pixels[0][0] = clWhite;
    if (BlackWhite)
      ReportBitmap->Mask(clWhite);
// ����� ������� ������ � ���-�� ����� �� ��������� �� X � Y
    ReportBitmap->Width = PreviewWidth;
    PixelsPerMmX = (double)ReportBitmap->Width / (double)PageWidthMm;
    ReportBitmap->Height = PreviewHeight;
    PixelsPerMmY = (double)ReportBitmap->Height / (double)PageHeightMm;
// ���� ������
    DrawTo = ReportBitmap->Canvas;
// ������ ��������
    DrawReportPage(PageNumber);
// ������ ���������� �������
    DrawPrintableArea();
// ������ ������� ������
    RectAtMm(0, 0, PageWidthMm - OffsetLeft - OffsetRight, PageHeightMm - OffsetTop - OffsetBottom, clWhite, bsClear, clBlack, psDot, 1);
    PreviewForm->ReportImage->Picture->Bitmap = ReportBitmap;
    delete ReportBitmap;
    ReportBitmap = NULL;
    Screen->Cursor = oldcur;
  } catch (Exception& e) {
    Screen->Cursor = oldcur;
    if (ReportBitmap)
      delete ReportBitmap;
  }
}

void TPlBaseReport::DrawReport(int PageNumber)
{
}

void TPlBaseReport::DrawReportPage(int PageNumber)
{
  Page = 1;
  DrawReport(PageNumber);
}

void TPlBaseReport::Init()
{
  TCanvas* oldcanvas = DrawTo;
  TDrawReportMode oldmode = DrawMode;
  TCursor oldcur = Screen->Cursor;
  DrawMode = dm_Init;
  GetPrinterParams();
  Graphics::TBitmap* InitBitmap = NULL;
  try {
    InitBitmap = new Graphics::TBitmap();
    InitBitmap->Height = 1;
    InitBitmap->Width = 1;
    InitBitmap->Canvas->Pixels[0][0] = clWhite;
    InitBitmap->Mask(clWhite);
// ���� ������
    DrawTo = InitBitmap->Canvas;
// ������� ������ ������
    TFontStyles style;
    style.Clear();
    SetFont("Arial", style, 10);
    AnsiString date = Now().DateString();
    AnsiString str = LoadStr(sPrinted) + LoadStr(sPages);
    TVarRec v[] = { date, 1, 5};
    AnsiString teststr = Format(str, v, ARRAYSIZE(v) - 1);
    FooterHeight = MmTextHeight(teststr) + FooterOffset;
    Pages = 1;
    Page = 0;
// ��������� ���� �����
    DrawReport(0);
    delete InitBitmap;
    InitBitmap = NULL;
    DrawTo = oldcanvas;
    DrawMode = oldmode;
    Screen->Cursor = oldcur;
  } catch (Exception& e) {
    Screen->Cursor = oldcur;
    if (InitBitmap)
      delete InitBitmap;
    DrawTo = oldcanvas;
    DrawMode = oldmode;
  }
}

void TPlBaseReport::GetPrinterParams()
{
  if (FPrinter && FPrinter->Printers && FPrinter->Printers->Count) {
    try {
      PrinterPageWidth = FPrinter->PageWidth;
      PrinterPageHeight = FPrinter->PageHeight;
      PixelsPerMmX = GetDeviceCaps(FPrinter->Handle, LOGPIXELSX)/Inch;
      PixelsPerMmY = GetDeviceCaps(FPrinter->Handle, LOGPIXELSY)/Inch;
      PrinterOffsetLeft = RoundTwoDigits(GetDeviceCaps(FPrinter->Handle, PHYSICALOFFSETX)/PixelsPerMmX);
      PrinterOffsetTop = RoundTwoDigits(GetDeviceCaps(FPrinter->Handle, PHYSICALOFFSETY)/PixelsPerMmY);
      PageWidthMm = RoundDoubleToInt(GetDeviceCaps(FPrinter->Handle, PHYSICALWIDTH)/PixelsPerMmX);
      PageHeightMm = RoundDoubleToInt(GetDeviceCaps(FPrinter->Handle, PHYSICALHEIGHT)/PixelsPerMmY);
      PrinterOffsetRight = RoundTwoDigits(PageWidthMm - GetDeviceCaps(FPrinter->Handle, HORZSIZE) - PrinterOffsetLeft);
      PrinterOffsetBottom = RoundTwoDigits(PageHeightMm - GetDeviceCaps(FPrinter->Handle, VERTSIZE) - PrinterOffsetTop) + 0.2;
      KWidth = (float)PageHeightMm / (float)PageWidthMm;
    } catch (Exception& e) {
      throw;
    }
  } else {
// ��������� �� Epson Stylus ����� ���-���� �������� ������
    bool port = (FOrientation == poPortrait);
    PrinterPageWidth = port ? 1488 : 2104;
    PrinterPageHeight = port ? 2104 : 1488;
    PixelsPerMmX = 180/Inch;
    PixelsPerMmY = 180/Inch;
    PrinterOffsetLeft = port ? 22/PixelsPerMmX : 65/PixelsPerMmX;
    PrinterOffsetTop = port ? 65/PixelsPerMmY : 22/PixelsPerMmY;
    PageWidthMm = port ? 210 : 297;
    PageHeightMm = port ? 297 : 210;
    int physwidth = port ? 203 : 272;
    int physheight = port ? 272 : 203;
    PrinterOffsetRight = RoundTwoDigits(PageWidthMm - physwidth - PrinterOffsetLeft);
    PrinterOffsetBottom = RoundTwoDigits(PageHeightMm - physheight - PrinterOffsetTop) + 0.2;
    KWidth = (float)PageHeightMm / (float)PageWidthMm;
  }
}

void TPlBaseReport::GetDisplayParams()
{
    HDC display = NULL;
    try {
      display = CreateDC("DISPLAY", NULL, NULL, NULL);
      DisplayPixelsPerMmX = GetDeviceCaps(display, LOGPIXELSX)/Inch;
      DisplayPixelsPerMmY = GetDeviceCaps(display, LOGPIXELSY)/Inch;
      DeleteDC(display);
      display = NULL;
    } catch (Exception& e) {
      if (display)
        DeleteDC(display);
// ������� (�������� ������ ��� 15' ���������) ����� ���-���� �������� ������
      DisplayPixelsPerMmX = 96/Inch;
      DisplayPixelsPerMmY = 96/Inch;
    }
}

// ������ ������ � ����������� �� DrawTo
double TPlBaseReport::MmTextWidth(const AnsiString Text)
{
  int oldsize = DrawTo->Font->Size;
  double oldppmmX = PixelsPerMmX;
  double oldppmmY = PixelsPerMmY;

  if (FPrinter && FPrinter->Printers && FPrinter->Printers->Count) {
    PixelsPerMmX = GetDeviceCaps(FPrinter->Handle, LOGPIXELSX)/Inch;
    PixelsPerMmY = GetDeviceCaps(FPrinter->Handle, LOGPIXELSY)/Inch;
  } else {
    PixelsPerMmX = 180/Inch;
    PixelsPerMmY = 180/Inch;
  }

  double size = RealFontSize*(double)PixelsPerMmY/(double)DisplayPixelsPerMmY;
  DrawTo->Font->Size = RoundFonts ? RoundDoubleToInt(size) : size;
  double res = (double)DrawTo->TextWidth(Text)/PixelsPerMmX;
  PixelsPerMmX = oldppmmX;
  PixelsPerMmY = oldppmmY;
  DrawTo->Font->Size = oldsize;
  return res;
}

// ������ ������ � ����������� �� DrawTo
double TPlBaseReport::MmTextHeight(const AnsiString Text)
{
  int oldsize = DrawTo->Font->Size;
  double oldppmmX = PixelsPerMmX;
  double oldppmmY = PixelsPerMmY;

  if (FPrinter && FPrinter->Printers && FPrinter->Printers->Count) {
    PixelsPerMmX = GetDeviceCaps(FPrinter->Handle, LOGPIXELSX)/Inch;
    PixelsPerMmY = GetDeviceCaps(FPrinter->Handle, LOGPIXELSY)/Inch;
  } else {
    PixelsPerMmX = 180/Inch;
    PixelsPerMmY = 180/Inch;
  }

  double size = RealFontSize*(double)PixelsPerMmY/(double)DisplayPixelsPerMmY;
  DrawTo->Font->Size = RoundFonts ? RoundDoubleToInt(size) : size;
  double res = (double)DrawTo->TextHeight(Text)/PixelsPerMmY;
  PixelsPerMmX = oldppmmX;
  PixelsPerMmY = oldppmmY;
  DrawTo->Font->Size = oldsize;
  return res;
}

// ������� ����� � X,Y (� �����������) �� DrawTo
void TPlBaseReport::TextAtMm(double X, double Y, const AnsiString Text)
{
  DrawTo->TextOut((X + OffsetLeft)*PixelsPerMmX, (Y + OffsetTop)*PixelsPerMmY, Text);
}

// ������� ����� � �������� (X1, X2), (Y1, Y2) �� DrawTo � ��������� ����.
// ���������� ������, ���������� ������� � ��.
void TPlBaseReport::TextInRectMm(double X1, double Y1, double X2, double Y2, const AnsiString Text, UINT Format, TTextDimentions* Dimentions)
{
  TRect rect = TRect((X1 + OffsetLeft)*PixelsPerMmX, (Y1 + OffsetTop)*PixelsPerMmY, (X2 + OffsetLeft)*PixelsPerMmX, (Y2 + OffsetTop)*PixelsPerMmY);
  TRect calcrect = rect;

  DrawText(DrawTo->Handle, Text.c_str(), Text.Length(), &calcrect, Format | DT_CALCRECT);

// ���� Y2 = 0, �� ��������� � ������������ ��������
  if (!Y2)
    rect.bottom = calcrect.bottom;
// DT_VCENTER �������� ������ ��� ����� ������. ����� �������� ��� ������
  if (Dimentions) {
    if ((rect.bottom - rect.top) > (calcrect.bottom - calcrect.top))
      Dimentions->TextHeight = (rect.bottom - rect.top)/PixelsPerMmY;
    else
      Dimentions->TextHeight = (calcrect.bottom - calcrect.top)/PixelsPerMmY;
      
    Dimentions->TextWidth = (calcrect.right - calcrect.left)/PixelsPerMmX;
  }
  if (Format & DT_VCENTER) {
    rect.top += ((rect.bottom - rect.top) - (calcrect.bottom - calcrect.top))/2;
  }
  DrawText(DrawTo->Handle, Text.c_str(), Text.Length(), &rect, Format);
  //return textheight;
}

// ������� ����� � (X1,Y1)-(X2,Y2) (� �����������) �� DrawTo
void TPlBaseReport::LineAtMm(double X1, double Y1, double X2, double Y2, const int PenWidth, const TPenStyle Pen)
{
  TPenStyle oldstyle = DrawTo->Pen->Style;
  int oldwidth = DrawTo->Pen->Width;
  DrawTo->Pen->Style = Pen;
  DrawTo->Pen->Width = PenWidth;
  DrawTo->MoveTo((X1 + OffsetLeft)*PixelsPerMmX, (Y1 + OffsetTop)*PixelsPerMmY);
  DrawTo->LineTo((X2 + OffsetLeft)*PixelsPerMmX, (Y2 + OffsetTop)*PixelsPerMmY);
  DrawTo->Pen->Style = oldstyle;
  DrawTo->Pen->Width = oldwidth;
}

// ��������� ������ DrawTo (� ������ ��������� � �������� �������� �������� �������)
void TPlBaseReport::SetFont(const AnsiString FontName, const TFontStyles FontStyle, double FontSize)
{
  RealFontSize = FontSize;
  if ((DrawMode == dm_Init || Previewing) & (DrawMode != dm_Print)) {
    double ScaledSize = FontSize*(PixelsPerMmY*Inch);
    if (DrawMode == dm_Init) {
        ScaledSize /= ((double)DisplayPixelsPerMmY*Inch);
    } else {
      if (Previewing)
        ScaledSize /= ((double)PreviewForm->PixelsPerInch);
    }
    if (RoundFonts)
      FontSize = RoundDoubleToInt(ScaledSize);
    else
      FontSize = ScaledSize;
  }
  DrawTo->Font->Name = FontName;
  DrawTo->Font->Style = FontStyle;
  DrawTo->Font->Size = FontSize;
}

// ������� ��������������� ��������� (X1,Y1), (X2,Y2) (� �����������) �� DrawTo
void TPlBaseReport::RectAtMm(double X1, double Y1, double X2, double Y2, TColor BrushColor, TBrushStyle BrushStyle, TColor PenColor, TPenStyle PenStyle, int PenWidth)
{
  TBrushStyle oldbrstyle = DrawTo->Brush->Style;
  TPenStyle oldpenstyle = DrawTo->Pen->Style;
  TColor oldpencolor = DrawTo->Pen->Color;
  TColor oldbrcolor =  DrawTo->Brush->Color;
  int oldpenwidth = DrawTo->Pen->Width;
  DrawTo->Brush->Color = BrushColor;
  DrawTo->Brush->Style = BrushStyle;
  DrawTo->Pen->Color = PenColor;
  DrawTo->Pen->Style = PenStyle;
  DrawTo->Pen->Width = PenWidth;
  DrawTo->Rectangle((X1 + OffsetLeft)*PixelsPerMmX, (Y1 + OffsetTop)*PixelsPerMmY, (X2 + OffsetLeft)*PixelsPerMmX, (Y2 + OffsetTop)*PixelsPerMmY);
  DrawTo->Brush->Color = oldbrcolor;
  DrawTo->Brush->Style = oldbrstyle;
  DrawTo->Pen->Color = oldpencolor;
  DrawTo->Pen->Style = oldpenstyle;
  DrawTo->Pen->Width = oldpenwidth;
}

void TPlBaseReport::EllipseAtMm(double X1, double Y1, double X2, double Y2, TColor BrushColor, TBrushStyle BrushStyle, TColor PenColor, TPenStyle PenStyle, int PenWidth)
{
  TBrushStyle oldbrstyle = DrawTo->Brush->Style;
  TPenStyle oldpenstyle = DrawTo->Pen->Style;
  TColor oldpencolor = DrawTo->Pen->Color;
  TColor oldbrcolor =  DrawTo->Brush->Color;
  int oldpenwidth = DrawTo->Pen->Width;
  DrawTo->Brush->Color = BrushColor;
  DrawTo->Brush->Style = BrushStyle;
  DrawTo->Pen->Color = PenColor;
  DrawTo->Pen->Style = PenStyle;
  DrawTo->Pen->Width = PenWidth;
  DrawTo->Ellipse((X1 + OffsetLeft)*PixelsPerMmX, (Y1 + OffsetTop)*PixelsPerMmY, (X2 + OffsetLeft)*PixelsPerMmX, (Y2 + OffsetTop)*PixelsPerMmY);
  DrawTo->Brush->Color = oldbrcolor;
  DrawTo->Brush->Style = oldbrstyle;
  DrawTo->Pen->Color = oldpencolor;
  DrawTo->Pen->Style = oldpenstyle;
  DrawTo->Pen->Width = oldpenwidth;
}


// ���������� ������ �� ������� �����
void TPlBaseReport::ResetPreviewParams()
{
  PreviewForm->Shadow->Brush->Color = ShadowColor;
  PreviewForm->Border->Pen->Color = PreviewCantColor;
  PreviewForm->Border->Pen->Width = PreviewCantWidth;
  Decrement = ShadowWidth + 2*ReportBorderWidth + 2*PreviewCantWidth;
}

// ���������� ����� double �� ������
int TPlBaseReport::RoundDoubleToInt(double Value)
{
  int res = Value;
  if ((Value - res) >= 0.5)
    res++;
  return res;
}

// ���������� ����� double �� ���� ������ ����� �������
double TPlBaseReport::RoundTwoDigits(double Value)
{
  return ((float)RoundDoubleToInt(Value*100))/100;
}

// ���������� ���������� ������� ������ ��������
void TPlBaseReport::DrawPrintableArea()
{
  TBrushStyle oldbrstyle = DrawTo->Brush->Style;
  TPenStyle oldpenstyle = DrawTo->Pen->Style;
  TColor oldpencolor = DrawTo->Pen->Color;
  TColor oldbrcolor =  DrawTo->Brush->Color;
  int oldpenwidth = DrawTo->Pen->Width;
  DrawTo->Brush->Style = bsClear;
  DrawTo->Pen->Color = clBlack;
  DrawTo->Pen->Style = psDot;
  DrawTo->Pen->Width = 1;
  DrawTo->Rectangle(RoundDoubleToInt(PrinterOffsetLeft*PixelsPerMmX), RoundDoubleToInt(PrinterOffsetTop*PixelsPerMmY), RoundDoubleToInt((PageWidthMm - PrinterOffsetRight)*PixelsPerMmX), RoundDoubleToInt((PageHeightMm - PrinterOffsetBottom)*PixelsPerMmY));
  DrawTo->Brush->Style = oldbrstyle;
  DrawTo->Pen->Style = oldpenstyle;
  DrawTo->Pen->Color = oldpencolor;
  DrawTo->Brush->Color = oldbrcolor;
  DrawTo->Pen->Width = oldpenwidth;
}

// ������� ����� ��������. ��������� ��� ��������� ������� (�������������, ������, ��������)
void TPlBaseReport::BeginNewPage()
{
  // ���������� �����
  DrawFooter();
  LastY = 0;
  switch (DrawMode) {
    case dm_Init: {
      Pages++;
      break;
    }
    case dm_Preview: {
      RectAtMm(-OffsetLeft - 5, -OffsetTop - 5, PageWidthMm - OffsetLeft + 5, PageHeightMm - OffsetTop + 5, clWhite, bsSolid, clWhite, psSolid, 1);
      Page++;
      break;
    }
    case dm_Print: {
      FPrinter->NewPage();
      Page++;
      break;
    }
  }
// ���������� �����
  DrawHeader();
// ����� ������� ������ DrawFooter � DrawHeader ������������ ��������� ������
// �� ���� ���������, � ������ - �� ���� ����� ������. ���� ���� �� ������ -
// ������� DrawHeader ������� �� DrawReport � ����� ������
}

void TPlBaseReport::SetOrientation(TPrinterOrientation Value)
{
  if (FOrientation != Value) {
    FOrientation = Value;
    if (!(FPrinter && FPrinter->Printers && FPrinter->Printers->Count)) {
// ���� ��� �������� ����� �������� ������
      switch (FOrientation) {
        case poPortrait: {
          PageWidthMm = 210;
          PageHeightMm = 297;
          break;
        }
        case poLandscape: {
          PageWidthMm = 297;
          PageHeightMm = 210;
          break;
        }
      }
// ���� ��� �������� ����� �������� ������
    } else {
      FPrinter->Orientation = FOrientation;
    }
    Init();
    FullWidth = DisplayPixelsPerMmX*PageWidthMm;
    if (Previewing){
      DrawMode = dm_Preview;
      PreviewForm->Page = Pages;
      PreviewForm->Page = 1;
    }
  }
}

// ��������� �������� ����������� (������)
void TPlBaseReport::DrawHeader()
{
  if (!PrintHeader) {
    HeaderHeight = 0;
    return;
  }
  TFontStyles oldstyles = DrawTo->Font->Style;
  AnsiString oldname = DrawTo->Font->Name;
  int oldsize = DrawTo->Font->Size;
  TColor oldcolor = DrawTo->Pen->Color;
  TFontStyles style;
  style.Clear();
  style << fsItalic;
  SetFont("Arial", style, 11);
  /*double width = MmTextWidth(Title);
  TextAtMm(PageWidthMm - width - OffsetLeft - OffsetRight - 3, 0, Title);
  double height = MmTextHeight(Title);
  height += 1;*/
  TTextDimentions dim;
  TextInRectMm(3, LastY + 1, PageWidthMm - OffsetLeft - OffsetRight - 3, 0, Title, DT_RIGHT | DT_VCENTER, &dim);
  dim.TextHeight += 2;
  DrawTo->Pen->Color = clBlack;
  LineAtMm(3, dim.TextHeight, PageWidthMm - OffsetRight - OffsetLeft - 3, dim.TextHeight, 1, psSolid);
  HeaderHeight = dim.TextHeight;
  LastY = HeaderHeight + HeaderOffset;
  DrawTo->Font->Style = oldstyles;
  DrawTo->Font->Name = oldname;
  DrawTo->Font->Size = oldsize;
  DrawTo->Pen->Color = oldcolor;
}

// ��������� ������� ����������� (������)
void TPlBaseReport::DrawFooter()
{
  if (!PrintFooter) {
    FooterHeight = 0;
    return;
  }
  TFontStyles oldstyles = DrawTo->Font->Style;
  AnsiString oldname = DrawTo->Font->Name;
  int oldsize = DrawTo->Font->Size;
  TFontStyles style;
  style.Clear();
  SetFont("Arial", style, 10);
  int top = PageHeightMm - FooterOffset - OffsetTop - OffsetBottom;

  TTextDimentions dim;

  if (PrintPrintDate) {
    AnsiString str = LoadStr(sPrinted);
    AnsiString date = Now().FormatString("dd.mm.yyyy");
    TVarRec v[] = { date };
    str = Format(str, v, ARRAYSIZE(v) - 1);
    TextInRectMm(3, top - 1, PageWidthMm - OffsetLeft - OffsetRight - 3, 0, str, DT_LEFT | DT_VCENTER, &dim);
//    TextAtMm(3, top, str);
  }

  if (PrintPageNumbers) {
    AnsiString str = LoadStr(sPages);
    TVarRec v1[] = {Page, Pages};
    str = Format(str, v1, ARRAYSIZE(v1) - 1);
    //double width = MmTextWidth(str);
    TextInRectMm(3, top - 1, PageWidthMm - OffsetLeft - OffsetRight - 3, 0, str, DT_RIGHT | DT_VCENTER, &dim);
    //TextAtMm(PageWidthMm - width - OffsetLeft - OffsetRight - 3, top, str);
  }

  top -= 2;
  DrawTo->Pen->Color = clBlack;
  LineAtMm(3, top, PageWidthMm - OffsetRight - OffsetLeft - 3, top, 1, psSolid);
  DrawTo->Font->Style = oldstyles;
  DrawTo->Font->Name = oldname;
  DrawTo->Font->Size = oldsize;
}

void TPlBaseReport::CorrectOffsets()
{
  if (OffsetLeft < PrinterOffsetLeft)
    OffsetLeft += (PrinterOffsetLeft - OffsetLeft);
  if (OffsetTop < PrinterOffsetTop)
    OffsetTop += (PrinterOffsetTop - OffsetTop);
  if (OffsetRight < PrinterOffsetRight)
    OffsetRight += (PrinterOffsetRight - OffsetRight);
  if (OffsetBottom < PrinterOffsetBottom)
    OffsetBottom += (PrinterOffsetBottom - OffsetBottom);
}
