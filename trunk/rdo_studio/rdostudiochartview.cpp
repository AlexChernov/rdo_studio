#include "stdafx.h"
#include "rdostudiochartview.h"
#include "rdostudiochartdoc.h"
#include "./rdo_tracer/rdotracertrace.h"
#include "./rdo_tracer/rdotracervalues.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioChartView
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioChartView, CView)

BEGIN_MESSAGE_MAP(RDOStudioChartView, CView)
	//{{AFX_MSG_MAP(RDOStudioChartView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_CHART_TIMEWRAP, OnChartTimewrap)
	ON_UPDATE_COMMAND_UI(ID_CHART_TIMEWRAP, OnUpdateChartTimewrap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioChartView::RDOStudioChartView()
	: CView(),
	bmpRect( 0, 0, 0, 0 ),
	newClientRect( 0, 0, 0, 0 ),
	dragedSerie( NULL ),
	valueCountX( 5 ),
	valueCountY( 5 ),
	yaxis( NULL ),
	tickWidth( 5 ),
	timeColor( RGB( 0xE7, 0xF8, 0xF8 ) ),
	timeWrap( true ),
	timeRange( 0 ),
	timeScale( 0 ),
	drawFromX( NULL ),
	drawFromXEventIndex( 0 ),
	drawToX( NULL ),
	drawToXEventIndex( 0 ),
	pixelsToChart( 0 ),
	xAxisOffset( 0 ),
	yAxisOffset( 0 )
{
}

RDOStudioChartView::~RDOStudioChartView()
{
}

BOOL RDOStudioChartView::PreCreateWindow(CREATESTRUCT& cs) 
{
//	return CView::PreCreateWindow(cs);
	if ( !CView::PreCreateWindow( cs ) ) return FALSE;
	cs.style = WS_CHILD | WS_VISIBLE /*| WS_HSCROLL | WS_VSCROLL*/ | WS_TABSTOP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)/*, reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL*/ );
	return TRUE;
}

void RDOStudioChartView::prepareDrawing( CDC &dc, CRect& chartRect )
{
/*	timeRange = trace.getMaxTime()->time;
	
	if ( timeRange > 0 ) {
		CRect backuprect;
		backuprect.CopyRect( &chartRect );
		calcYAxisOffset( dc );
		chartRect.left += yAxisOffset;
		calcXAxisOffset( dc );
		chartRect.right -= xAxisOffset;
		timeScale = timeWrap ? chartRect.Width() / timeRange : ( chartRect.Width() - tickWidth * trace.getMaxTime()->overallCount ) / timeRange;
		if ( trace.getMinTimeDifference() * timeScale < 5 ) {
			chartRect.CopyRect( &backuprect );
			timeScale = 5 / trace.getMinTimeDifference();
			
			drawToX = trace.getMaxTime();
			drawToXEventIndex = trace.getMaxTime()->eventCount;
			
			calcYAxisOffset( dc );
			chartRect.left += yAxisOffset;
			calcXAxisOffset( dc );
			chartRect.right -= xAxisOffset;
			
			int count = trace.getTimesCount();
			int x = chartRect.right;
			RDOTracerTimeNow* timenow = NULL;
			RDOTracerTimeNow* prev = trace.getMaxTime();
			int offset = timeWrap ? 0 : tickWidth * prev->eventCount;
			x -= offset;
			for ( int i = count - 2; i >= 0; i-- ) {
				timenow = trace.getTime( i );
				offset = ( prev->time - timenow->time ) * timeScale;
				if ( !timeWrap )
					offset += tickWidth * timenow->eventCount;
				x -= offset;
				if ( x <= chartRect.left ) {
					drawFromX = timenow;
					pixelsToChart = chartRect.left - x;
					drawFromXEventIndex = ( pixelsToChart ) / tickWidth;
					break;
				}
				prev = timenow;
			}
			
			timeRange = drawToX->time - drawFromX->time;
		} else {
			drawFromX = trace.getTime( 0 );
			drawFromXEventIndex = 0;
			drawToX = trace.getMaxTime();
			drawToXEventIndex = trace.getMaxTime()->eventCount;
		}
	} else {
		timeScale = 0;
		drawFromX = trace.getTime( 0 );
		drawFromXEventIndex = 0;
		drawToX = trace.getTime( 0 );
		drawToXEventIndex = trace.getTime( 0 )->eventCount;
		calcYAxisOffset( dc );
		chartRect.left += yAxisOffset;
		calcXAxisOffset( dc );
		chartRect.right -= xAxisOffset;
	}*/
}

void RDOStudioChartView::calcYAxisOffset( CDC &dc )
{
	CSize size;
	//temporary
	string str = format( "%.3f", 0 );
	size = dc.GetTextExtent( str.c_str() );
	yAxisOffset = size.cx + 2 + 3;
}

void RDOStudioChartView::calcXAxisOffset( CDC &dc )
{
	CSize size;
	//temporary
	string str = format( "%.3f", trace.getMaxTime()->time );
	size = dc.GetTextExtent( str.c_str() );
	xAxisOffset = size.cx;
}

void RDOStudioChartView::drawYAxis( CDC &dc, CRect& chartRect, const RDOTracerSerie* axisValues)
{
	CRect tmprect;
	CSize size;

	tmprect.CopyRect( &chartRect );
	tmprect.left -= yAxisOffset;
	tmprect.right = tmprect.left + yAxisOffset;
	if ( axisValues ) {
		string formatstr = "%.3f";
		/*if ( yaxis->getSerieKind() == RDOST_OPERATION )
			formatstr = "%d";
		if ( yaxis->getSerieKind() == RDOST_RESPARAM ) {
			RDOTracerResParam* resparam = (RDOTracerResParam*)yaxis;
			RDOTracerResParamType type = resparam->getParamInfo()->getParamType();
			if ( type == RDOPT_INTEGER )
				formatstr = "%d";
			else if ( type == RDOPT_ENUMERATIVE )
				//format = "%s";
				formatstr = "%d";
		}*/
		RDOTracerValue* start = NULL;
		RDOTracerValue* end = NULL;
		axisValues->getFromToValues( drawFromX, drawFromXEventIndex, drawToX, drawToXEventIndex, start, end );
		double min = 0;
		double max = 0;
		axisValues->getMinMaxValues( start, end, min, max );
		double valoffset = 0;
		if ( min != max )
			valoffset = ( max - min ) / ( valueCountY - 1 );
		int heightoffset = chartRect.Height() / ( valueCountY - 1 );
		double valo = min ? min : 0;
		int y = chartRect.bottom;
		//int maxy = 0;
		for ( int i = 0; i < valueCountY; i++ ) {
			string str = format( formatstr.c_str(), valo );
			size = dc.GetTextExtent( str.c_str() );
			//if ( size.cx > maxy )
			//	maxy = size.cx;
			tmprect.top = y - size.cy / 2;
			tmprect.bottom += size.cy / 2;
			dc.DrawText( str.c_str(), tmprect, DT_LEFT );
			valo += valoffset;
			y -= heightoffset;
		}

		//chartRect.left += ( maxy + 2 );
		
		y = chartRect.bottom;
		for ( i = 1; i < valueCountY - 1; i++ ) {
			y -= heightoffset;
			dc.MoveTo( chartRect.left, y );
			dc.LineTo( chartRect.left + 3, y );
		}
		//chartRect.left += 3;
	}
}

void RDOStudioChartView::drawXAxis( CDC &dc, CRect& chartRect )
{
	CRect tmprect;
	CSize size;
	/*tmprect.top = chartRect.bottom;
	tmprect.left = chartRect.left;
	tmprect.bottom = newClientRect.bottom;
	tmprect.right = chartRect.right;*/
	tmprect.CopyRect( &chartRect );
	
	tmprect.top = chartRect.bottom;
	tmprect.left = chartRect.left;
	tmprect.bottom = newClientRect.bottom;
	tmprect.right = chartRect.right + xAxisOffset;
	
	string str = format( "%.3f", drawFromX->time );
	dc.DrawText( str.c_str(), tmprect, DT_LEFT );
	//CSize size = dc.GetTextExtent( str.c_str() );
	//rect.left += size.cx / 2;
	if ( timeRange > 0 ) {
		//rect.right -= size.cx / 2;
		//chartRect.right -= size.cx;
		int x = chartRect.left;
		int widthoffset = chartRect.Width() / ( valueCountX - 1 );
		double valoffset = timeRange / ( valueCountX - 1 );
		double tempo = drawFromX->time;
		for ( int i = 1; i < valueCountX - 1; i++ ) {
			tempo += valoffset;
			x += widthoffset;
			dc.MoveTo( x, chartRect.bottom );
			dc.LineTo( x, chartRect.bottom - 3 );
			str = format( "%.3f", tempo );
			//size = dc.GetTextExtent( str.c_str() );
			//tmprect.left = x - size.cx / 2;
			tmprect.left = x;
			dc.DrawText( str.c_str(), tmprect, DT_LEFT );
		}
		str = format( "%.3f", drawToX->time );
		//size = dc.GetTextExtent( str.c_str() );
		//tmprect.left += widthoffset;
		//tmprect.right = tmprect.left + size.cx;
		dc.DrawText( str.c_str(), tmprect, DT_RIGHT );
	}
	chartRect.bottom -= 2;
}

void RDOStudioChartView::drawGrid(	CDC &dc, CRect& chartRect )
{
	dc.Rectangle( chartRect );
	chartRect.InflateRect( -1, -1 );

	if ( !timeWrap ) {
		int count = trace.getTimesCount();
		CRect tmprect;
		tmprect.CopyRect( &chartRect );
		RDOTracerTimeNow* lasttime = NULL;
		for ( int i = 0; i < count; i++ ) {
			RDOTracerTimeNow* timeNow = trace.getTime( i );
			if ( timeNow->eventCount ) {
				int ticks = ( lasttime ) ? lasttime->overallCount : 0;
				tmprect.left = chartRect.left + min( timeScale * timeNow->time + tickWidth * ticks, chartRect.right );
				tmprect.right = min( tmprect.left + tickWidth * timeNow->eventCount, chartRect.right );
				dc.FillSolidRect( &tmprect, timeColor );
			}
			lasttime = timeNow;
		}
	}
}

void RDOStudioChartView::OnDraw(CDC* pDC)
{
	
	
	RDOStudioChartDoc* pDoc = GetDocument();

	
	//temporary
	if ( pDoc->series.size() )
		yaxis = pDoc->series.at( 0 ).getSerie();
	//temporary

	GetClientRect( &newClientRect );
	CRect rect;
	rect.CopyRect( &newClientRect );

	if ( newClientRect.Width() > bmpRect.Width() || newClientRect.Height() > bmpRect.Height() )
	{
		if ( bmp.GetSafeHandle() )
			bmp.DeleteObject();
		bmp.CreateCompatibleBitmap( pDC, newClientRect.Width(), newClientRect.Height() );
		bmpRect = newClientRect;
	}

	CDC dc;
	dc.CreateCompatibleDC( pDC );
	CBitmap* pOldBitmap = dc.SelectObject( &bmp );
	

	int oldBkMode = dc.SetBkMode( TRANSPARENT );

	CBrush brush( RGB( 255, 255, 255 ) );
	CBrush* pOldBrush = dc.SelectObject( &brush );
	CPen penBlack;
	penBlack.CreatePen( PS_SOLID, 0, RGB( 0, 0, 0 ) );
	CPen* pOldPen = dc.SelectObject( &penBlack );
	
	dc.FillSolidRect( newClientRect, RGB( 255, 255, 255 ) );

	string str = pDoc->GetTitle();
	dc.DrawText( str.c_str(), rect, DT_CENTER | DT_CALCRECT );
	dc.DrawText( str.c_str(), newClientRect, DT_CENTER );
	int height = rect.Height();
	rect.top = height;
	rect.bottom = newClientRect.Height() - height;
	rect.left = 5;
	rect.right = newClientRect.right - 5;
	
	if ( rect.Height() > 0 && rect.Width() > 0 ) {

		prepareDrawing( dc, rect );
		
		//temporary
		/*timeRange = trace.getMaxTime()->time;
		drawToX = timeRange;

		rect.left += calcYAxisOffset();
		rect.right -= calcXAxisOffset();*/
		
		//drawYAxis( dc, rect, yaxis );
		
		//drawXAxis( dc, rect );

		/*if ( timeWrap )
			timeScale = ( timerange > 0 ) ? rect.Width() / timerange : 0;
		else
			timeScale = ( timerange > 0 ) ? ( rect.Width() - tickWidth * trace.getMaxTime()->overallCount ) / timerange : 0;*/

		drawGrid( dc, rect );

		int count = pDoc->series.size();
		for ( int i = 0; i < count; i++ ) {
			pDoc->series.at( i ).getSerie()->drawSerie( this, dc, rect, pDoc->series.at( i ).getColor() );
		}
	}

	pDC->BitBlt( 0, 0, newClientRect.Width(), newClientRect.Height(), &dc, 0, 0, SRCCOPY );
	dc.SelectObject( pOldBrush );
	dc.SelectObject( pOldPen );
	dc.SetBkMode( oldBkMode );
	dc.SelectObject( pOldBitmap );
}

BOOL RDOStudioChartView::OnPreparePrinting( CPrintInfo* pInfo )
{
	// default preparation
	return DoPreparePrinting( pInfo );
}

void RDOStudioChartView::OnBeginPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
	// TODO: add extra initialization before printing
}

void RDOStudioChartView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

DROPEFFECT RDOStudioChartView::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	HGLOBAL glb = NULL;
	UINT format = trace.getClipboardFormat();
	if ( pDataObject->IsDataAvailable( format ) ) {
		glb = pDataObject->GetGlobalData( format );
		if ( glb ) {
			dragedSerie = *(RDOTracerSerie**)::GlobalLock( glb );
			::GlobalUnlock( glb );
			::GlobalFree( glb );
		}
		if ( !GetDocument()->serieExists( dragedSerie ) )
			return DROPEFFECT_COPY;
		else
			dragedSerie = NULL;
	}
	return DROPEFFECT_NONE;
}

void RDOStudioChartView::OnDragLeave()
{
	dragedSerie = NULL;
}

DROPEFFECT RDOStudioChartView::OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	if ( pDataObject->IsDataAvailable( trace.getClipboardFormat() ) && dragedSerie )
		return DROPEFFECT_COPY;
	else
		return DROPEFFECT_NONE;
}

BOOL RDOStudioChartView::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point )
{
	GetDocument()->addSerie( dragedSerie );
	dragedSerie = NULL;
	return TRUE;
}

int RDOStudioChartView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( !target.Register( this ) )
		return -1;
	return CView::OnCreate( lpCreateStruct );
}

#ifdef _DEBUG
void RDOStudioChartView::AssertValid() const
{
	CView::AssertValid();
}

void RDOStudioChartView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

RDOStudioChartDoc* RDOStudioChartView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(RDOStudioChartDoc)));
	return (RDOStudioChartDoc*)m_pDocument;
}
#endif

BOOL RDOStudioChartView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void RDOStudioChartView::OnSize(UINT nType, int cx, int cy) 
{

}

void RDOStudioChartView::OnChartTimewrap() 
{
	timeWrap = !timeWrap;
	Invalidate();
	UpdateWindow();
}

void RDOStudioChartView::OnUpdateChartTimewrap(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( timeWrap );
}
