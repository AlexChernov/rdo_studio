#include "stdafx.h"
#include "rdostudiochartoptions.h"
#include "rdostudiochartview.h"
#include "rdostudiochartdoc.h"
#include "rdostudioapp.h"
#include "htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartOptionsChart
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioChartOptionsChart, CPropertyPage)
	//{{AFX_MSG_MAP(RDOStudioChartOptionsChart)
	ON_EN_CHANGE(IDC_CHART_TITLE_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_DRAW_LEGEND_CHECK, OnUpdateModify)
	ON_CBN_SELCHANGE(IDC_AXIS_SERIES, OnUpdateModify)
	ON_EN_CHANGE(IDC_IDC_VAL_COUNT_Y_EDIT, OnUpdateModify)
	ON_EN_CHANGE(IDC_IDC_VAL_COUNT_X_EDIT, OnUpdateModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioChartOptionsChart::RDOStudioChartOptionsChart( RDOStudioChartOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet )
{

	//{{AFX_DATA_INIT(RDOStudioChartOptionsChart)
	m_ValCountX = sheet->view->valueCountX;
	m_ValCountY = sheet->view->valueCountY;
	m_ChartTitle = sheet->view->GetDocument()->getTitle().c_str();
	m_AxisSerie = -1;
	//}}AFX_DATA_INIT

	m_psp.dwFlags |= PSP_HASHELP;
}

RDOStudioChartOptionsChart::~RDOStudioChartOptionsChart()
{
}

void RDOStudioChartOptionsChart::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(RDOStudioChartOptionsChart)
	DDX_Control(pDX, IDC_AXIS_SERIES, m_AxisSerieCombo);
	DDX_Control(pDX, IDC_DRAW_LEGEND_CHECK, m_DrawLegend);
	DDX_Text(pDX, IDC_IDC_VAL_COUNT_X_EDIT, m_ValCountX);
	DDV_MinMaxInt(pDX, m_ValCountX, 2, 100);
	DDX_Text(pDX, IDC_IDC_VAL_COUNT_Y_EDIT, m_ValCountY);
	DDV_MinMaxInt(pDX, m_ValCountY, 2, 100);
	DDX_Text(pDX, IDC_CHART_TITLE_EDIT, m_ChartTitle);
	DDX_CBIndex(pDX, IDC_AXIS_SERIES, m_AxisSerie);
	//}}AFX_DATA_MAP
}

BOOL RDOStudioChartOptionsChart::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	RDOStudioChartDoc* doc = sheet->view->GetDocument();

	for ( vector< RDOStudioDocSerie* >::iterator it = doc->series.begin(); it != doc->series.end(); it++ ) {
		m_AxisSerieCombo.AddString( (*it)->serie->getTitle().c_str() );
	}
	
	m_AxisSerieCombo.SetCurSel( sheet->view->GetDocument()->getSerieIndex( sheet->view->yAxis ) );

	m_DrawLegend.SetCheck( sheet->view->needDrawLegend );

	return TRUE;
}

void RDOStudioChartOptionsChart::OnOK() 
{
	sheet->apply();
	CPropertyPage::OnOK();
}

BOOL RDOStudioChartOptionsChart::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if ( reinterpret_cast<LPNMHDR>(lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void RDOStudioChartOptionsChart::OnUpdateModify() 
{
	UpdateData();
	
	RDOStudioChartView* view = sheet->view;
	bool legend = m_DrawLegend.GetCheck() ? true : false;

	SetModified( legend != view->needDrawLegend || m_ValCountX != view->valueCountX || m_ValCountY != view->valueCountY || m_ChartTitle != view->GetDocument()->getTitle().c_str() || m_AxisSerie != view->GetDocument()->getSerieIndex( view->yAxis ) );
}

void RDOStudioChartOptionsChart::apply() const
{
	if ( m_AxisSerie != -1 )
		sheet->view->yAxis = sheet->view->GetDocument()->series.at( m_AxisSerie );
	else
		sheet->view->yAxis = NULL;
	bool legend = m_DrawLegend.GetCheck() ? true : false;
	sheet->view->needDrawLegend = legend;
	sheet->view->valueCountX = m_ValCountX;
	sheet->view->valueCountY = m_ValCountY;
	sheet->view->GetDocument()->SetTitle( m_ChartTitle );
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartOptionsSeries
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioChartOptionsSeries, CPropertyPage)
	//{{AFX_MSG_MAP(RDOStudioChartOptionsSeries)
	ON_EN_CHANGE(IDC_SERIE_TITLE_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_COLOR_BUTTON, OnColorButton)
	ON_CBN_SELCHANGE(IDC_SERIES_COMBO, OnSelchangeSeriesCombo)
	ON_CBN_SELCHANGE(IDC_COLOR_COMBO, OnUpdateModify)
	ON_CBN_SELCHANGE(IDC_MARKER_COMBO, OnUpdateModify)
	ON_BN_CLICKED(IDC_MARKER_CHECK, OnUpdateModify)
	ON_EN_CHANGE(IDC_MARKER_SIZE_EDIT, OnUpdateModify)
	ON_BN_CLICKED(IDC_LEGEND_CHECK, OnUpdateModify)
	ON_BN_CLICKED(IDC_TRANSP_MARKER_CHECK, OnUpdateModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioChartOptionsSeries::RDOStudioChartOptionsSeries( RDOStudioChartOptions& _sheet ):
	CPropertyPage( IDD ),
	sheet( &_sheet ),
	serie( NULL )
{

	//{{AFX_DATA_INIT(RDOStudioChartOptionsSeries)
	m_sizeMarker = 2;
	//}}AFX_DATA_INIT

	m_psp.dwFlags |= PSP_HASHELP;
}

RDOStudioChartOptionsSeries::~RDOStudioChartOptionsSeries()
{
}

bool RDOStudioChartOptionsSeries::getModified() const
{
	bool res = false;
	if ( serie ) {
		CString title;
		m_SerieTitle.GetWindowText( title );
		bool marker = m_DrawMarker.GetCheck() ? true : false;
		bool legend = m_DrawInLegend.GetCheck() ? true : false;
		bool transp = m_TranspMarker.GetCheck() ? true : false;
		res = title != serie->docSerieTitle.c_str() || ColorCB.getCurrentColor() != serie->color || m_Marker.GetCurSel() != serie->marker || marker != serie->needDrawMarker || m_sizeMarker != serie->marker_size || legend != serie->showInLegend || transp != serie->transparentMarker;
	}
	return res;
}

void RDOStudioChartOptionsSeries::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(RDOStudioChartOptionsSeries)
	DDX_Control(pDX, IDC_TRANSP_MARKER_CHECK, m_TranspMarker);
	DDX_Control(pDX, IDC_SERIES_COMBO, m_SerieCombo);
	DDX_Control(pDX, IDC_LEGEND_CHECK, m_DrawInLegend);
	DDX_Control(pDX, IDC_MARKER_SIZE_EDIT, m_MarkerSize);
	DDX_Control(pDX, IDC_MARKER_CHECK, m_DrawMarker);
	DDX_Control(pDX, IDC_MARKER_COMBO, m_Marker);
	DDX_Control(pDX, IDC_SERIE_TITLE_EDIT, m_SerieTitle);
	DDX_Text(pDX, IDC_MARKER_SIZE_EDIT, m_sizeMarker);
	DDV_MinMaxInt(pDX, m_sizeMarker, 2, 6);
	//}}AFX_DATA_MAP

	DDX_Control( pDX, IDC_COLOR_COMBO, ColorCB );
}

BOOL RDOStudioChartOptionsSeries::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	int itemHeight = static_cast<CComboBox*>(GetDlgItem( IDC_SERIES_COMBO ))->GetItemHeight( -1 );
	ColorCB.setItemHeight( itemHeight );
	ColorCB.insertBaseColors();

	RDOStudioChartDoc* doc = sheet->view->GetDocument();

	for ( vector< RDOStudioDocSerie* >::iterator it = doc->series.begin(); it != doc->series.end(); it++ ) {
		m_SerieCombo.AddString( (*it)->serie->getTitle().c_str() );
	}
	
	m_SerieCombo.SetCurSel( 0 );
	OnSelchangeSeriesCombo();

	return TRUE;
}

void RDOStudioChartOptionsSeries::OnOK() 
{
	sheet->apply();
	CPropertyPage::OnOK();
}

BOOL RDOStudioChartOptionsSeries::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if ( reinterpret_cast<LPNMHDR>(lParam)->code == PSN_HELP ) {
		sheet->onHelpButton();
		return true;
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void RDOStudioChartOptionsSeries::OnUpdateModify() 
{
	if ( serie ) {
		UpdateData();
		SetModified( getModified() );
	}
}

void RDOStudioChartOptionsSeries::apply() const
{
	if ( serie ) {
		CString title;
		m_SerieTitle.GetWindowText( title );
		serie->docSerieTitle = title;
		serie->color = ColorCB.getCurrentColor();
		serie->marker = static_cast<RDOTracerSerieMarker>(m_Marker.GetCurSel());
		bool marker = m_DrawMarker.GetCheck() ? true : false;
		serie->needDrawMarker = marker;
		serie->marker_size = m_sizeMarker;
		bool legend = m_DrawInLegend.GetCheck() ? true : false;
		serie->showInLegend = legend;
		bool transp = m_TranspMarker.GetCheck() ? true : false;
		serie->transparentMarker = transp;
	}
}

void RDOStudioChartOptionsSeries::restoreValues()
{
	if ( serie ) {
		m_SerieTitle.SetWindowText( serie->docSerieTitle.c_str() );
		ColorCB.insertColor( serie->color );
		ColorCB.setCurrentColor( serie->color );
		m_Marker.SetCurSel( serie->marker );
		m_DrawMarker.SetCheck( serie->needDrawMarker );
		m_MarkerSize.SetWindowText( format( "%d", serie->marker_size).c_str() );
		m_DrawInLegend.SetCheck( serie->showInLegend );
		m_TranspMarker.SetCheck( serie->transparentMarker );
	}
}

void RDOStudioChartOptionsSeries::OnSelchangeSeriesCombo() 
{
	int ser = m_SerieCombo.GetCurSel();
	bool enable = ser != -1;
	m_SerieTitle.EnableWindow( enable );
	ColorCB.EnableWindow( enable );
	m_Marker.EnableWindow( enable );
	m_DrawMarker.EnableWindow( enable );
	m_MarkerSize.EnableWindow( enable );
	m_DrawInLegend.EnableWindow( enable );
	m_TranspMarker.EnableWindow( enable );
	if ( enable ) {
		if ( serie && getModified() ) {
			int res = AfxGetMainWnd()->MessageBox( format( IDS_CHART_OPTIONS_APPLY ).c_str(), NULL, MB_ICONQUESTION | MB_YESNOCANCEL );
			bool continue_flag = true;  
			switch ( res ) {
				case IDYES   : sheet->apply(); break;
				case IDNO    : restoreValues(); break;
				case IDCANCEL: {
					continue_flag = false;
					m_SerieCombo.SetCurSel( sheet->view->GetDocument()->getSerieIndex( serie ) );
					break;
				}
			}
			if ( !continue_flag ) return;
		}
		serie = sheet->view->GetDocument()->series.at( ser );
		restoreValues();
	} else {
		serie = NULL;
	}
}

void RDOStudioChartOptionsSeries::OnColorButton() 
{
	CColorDialog dlg( ColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		ColorCB.insertColor( dlg.GetColor() );
		OnUpdateModify();
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartOptions
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioChartOptions, CPropertySheet)
	//{{AFX_MSG_MAP(RDOStudioChartOptions)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioChartOptions::RDOStudioChartOptions( RDOStudioChartView* _view ):
	CPropertySheet(),
	chart( NULL ),
	view( _view )
{
	SetTitle( format( IDS_CHART_OPTIONS ).c_str() );

	chart  = new RDOStudioChartOptionsChart( *this );
	series = new RDOStudioChartOptionsSeries( *this );

	AddPage( chart );
	AddPage( series );

	m_psh.dwFlags |= PSH_USECALLBACK | PSH_HASHELP;
	m_psh.pfnCallback = AddContextHelpProc;
}

RDOStudioChartOptions::~RDOStudioChartOptions()
{
	if ( chart )  { delete chart; chart = NULL; }
	if ( series ) { delete series; series = NULL; }
}

void RDOStudioChartOptions::apply() const
{
	chart->apply();
	series->apply();
	view->GetDocument()->UpdateAllViews( NULL );
}

int CALLBACK RDOStudioChartOptions::AddContextHelpProc(HWND hwnd, UINT message, LPARAM /*lParam*/)
{
	switch (message) {
		case PSCB_INITIALIZED: {
			LONG style = ::GetWindowLong( hwnd, GWL_EXSTYLE );
			style |= WS_EX_CONTEXTHELP;
			::SetWindowLong( hwnd, GWL_EXSTYLE, style );
		}
	}
	return true;
}

void RDOStudioChartOptions::onHelpButton()
{
	/*string filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return;

	CPropertyPage* page = GetActivePage( );
	if ( page == editor ) {
		filename += "::/html/work_options.htm#editor";
	} else if ( page == tabs ) {
		filename += "::/html/work_options.htm#tabs";
	} else if ( page == styles ) {
		filename += "::/html/work_options.htm#styles";
	}
	HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_DISPLAY_TOPIC, NULL );*/
}

BOOL RDOStudioChartOptions::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	string filename = studioApp.getFullHelpFileName();
	if ( filename.empty() ) return TRUE;

	if ( pHelpInfo->iContextType == HELPINFO_WINDOW )
		return HtmlHelp( ::GetDesktopWindow(), filename.c_str(), HH_HELP_CONTEXT, pHelpInfo->dwContextId) != NULL;
	return TRUE;
}
