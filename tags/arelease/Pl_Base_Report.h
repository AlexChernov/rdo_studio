//---------------------------------------------------------------------------
#ifndef Pl_Base_ReportH
#define Pl_Base_ReportH
//---------------------------------------------------------------------------
#include <Printers.hpp>
#include <db.hpp>
//---------------------------------------------------------------------------
typedef enum {
  dm_Init    = 0,
  dm_Preview = 1,
  dm_Print   = 2
} TDrawReportMode;

typedef struct {
  double TextWidth;
  double TextHeight;
} TTextDimentions;


//---------------------------------------------------------------------------
class TPlBaseReport :  public TObject
{
friend class TPreviewForm;
friend class TPreviewOptionsForm;
private:
  double RealFontSize;
  void GetPrinterParams();

  TPreviewForm* PreviewForm;
  int ShadowWidth;
  int ReportBorderWidth;
  int PreviewCantWidth;
  int Decrement;

  TColor PreviewCantColor;
  TColor ShadowColor;
  bool RoundFonts;
  bool BlackWhite;
  void ResetPreviewParams();
  void DrawPrintableArea();

  TPrinterOrientation FOrientation;
  void SetOrientation(TPrinterOrientation Value);

  double DisplayPixelsPerMmX;
  double DisplayPixelsPerMmY;
  void GetDisplayParams();
protected:
  TPrinter* FPrinter;

  TDataSet* ReportData;
  virtual void OpenReportDataSource();

  TDrawReportMode DrawMode;
  TCanvas* DrawTo;

  void LoadReportParams(AnsiString IniSection, TPrinterOrientation DefaultOrientation);
  void WriteReportParams(AnsiString IniSection);

  __property TPrinterOrientation Orientation = { read=FOrientation, write=SetOrientation};
  int PageHeightMm;
  int PageWidthMm;

  float KWidth;
  double PixelsPerMmX;
  double PixelsPerMmY;

// � ��.
  double PrinterOffsetLeft;
  double PrinterOffsetTop;
  double PrinterOffsetRight;
  double PrinterOffsetBottom;
// � ��.

// � ��.
  double OffsetLeft;
  double OffsetTop;
  double OffsetRight;
  double OffsetBottom;
  double HeaderHeight;
  double FooterHeight;
// � ��.

  int PrinterPageWidth;
  int PrinterPageHeight;

  bool PrintHeader;
  bool PrintFooter;
  bool PrintPrintDate;
  bool PrintPageNumbers;

  int Pages;
  int Page;
  double LastY;

  virtual void DrawReport(int PageNumber);
  virtual void DrawReportPage(int PageNumber);
  virtual void Init();

  int RoundDoubleToInt(double Value);
  double RoundTwoDigits(double Value);

  double MmTextWidth(const AnsiString Text);
  double MmTextHeight(const AnsiString Text);
  void TextAtMm(double X, double Y, const AnsiString Text);
  void TextInRectMm(double X1, double Y1, double X2, double Y2, const AnsiString Text, UINT Format, TTextDimentions* Dimentions);
  void LineAtMm(double X1, double Y1, double X2, double Y2, const int PenWidth, const TPenStyle Pen);
  void RectAtMm(double X1, double Y1, double X2, double Y2, TColor BrushColor, TBrushStyle BrushStyle, TColor PenColor, TPenStyle PenStyle, int PenWidth);
  void EllipseAtMm(double X1, double Y1, double X2, double Y2, TColor BrushColor, TBrushStyle BrushStyle, TColor PenColor, TPenStyle PenStyle, int PenWidth);
  void SetFont(const AnsiString FontName, const TFontStyles FontStyle, double FontSize);
  void BeginNewPage();
  void CorrectOffsets();
  virtual void DrawHeader();
  virtual void DrawFooter();
public:
  int FullWidth;
  bool Previewing;
  AnsiString Title;
  __fastcall TPlBaseReport();
  __fastcall ~TPlBaseReport();
  void UpdateReportPreview(const int PreviewWidth, const int PreviewHeight, const int PageNumber);
  void Preview();
  void Print(bool ShowPrintDialog);
};
#endif

