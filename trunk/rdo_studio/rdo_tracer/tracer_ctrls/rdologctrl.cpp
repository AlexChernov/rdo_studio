#include "stdafx.h"
#include "rdologctrl.h"
#include "../../rdostudioapp.h"
#include "../../rdostudiomainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace rdoTracerLog;
using namespace rdoEditCtrl;
using namespace std;

namespace rdoTracerLog {

// ----------------------------------------------------------------------------
// ---------- RDOLogCtrlFindInList
// ----------------------------------------------------------------------------
class RDOLogCtrlFindInList
{
	RDOLogCtrl* log;
	string strToFind;
	bool matchCase;
	bool matchWholeWord;
	
	bool scan( string::iterator &wildCards, string::iterator &wildend, string::iterator &str, string::iterator &strend ) const;
	bool match( string::iterator &wildcards, string::iterator &wildend, string::iterator &strcomp, string::iterator &strend ) const;
public:
	RDOLogCtrlFindInList( RDOLogCtrl* _log, string _strToFind, bool _matchCase, bool _matchWholeWord );
	bool operator()( string nextstr );
};

}; // namespace rdoTracerLog

RDOLogCtrlFindInList::RDOLogCtrlFindInList( RDOLogCtrl* _log, string _strToFind, bool _matchCase, bool _matchWholeWord )
	: log( _log ),
	strToFind( _strToFind ),
	matchCase( _matchCase ),
	matchWholeWord( _matchWholeWord )
{
}

bool RDOLogCtrlFindInList::scan( string::iterator &wildCards, string::iterator &wildend, string::iterator &str, string::iterator &strend ) const
{
	// remove the '?' and '*'
	for( wildCards ++; str != strend && ( *wildCards == '?' || *wildCards == '*' ); wildCards ++ )
		if ( *wildCards == '?') str ++;
	while ( *wildCards == '*') wildCards ++;
	
	// if str is empty and Wildcards has more characters or,
	// Wildcards is empty, return 
	if ( str == strend && wildCards != wildend ) return false;
	if ( str == strend && wildCards == wildend ) return true; 
	// else search substring
	else
	{
		string::iterator wdsCopy = wildCards;
		string::iterator strCopy = str;
		bool res = 1;
		do 
		{
			if ( !match( wildCards, wildend, str, strend ) ) strCopy ++;
			wildCards = wdsCopy;
			str		  = strCopy;
			while ( ( *wildCards != *str ) && ( str != strend ) ) str ++;
			wdsCopy = wildCards;
			strCopy = str;
		} while ( ( str != strend ) ? !match( wildCards, wildend, str, strend ) : ( res = false ) != false );

		if ( str == strend && wildCards == wildend ) return true;

		return res;
	}
}

bool RDOLogCtrlFindInList::match( string::iterator &wildcards, string::iterator &wildend, string::iterator &strcomp, string::iterator &strend ) const
{
	bool res = true;
	
	//iterate and delete '?' and '*' one by one
	while( wildcards != wildend && res && strcomp != strend )
	{
		if ( *wildcards == '?' )
			strcomp ++;
		else if ( *wildcards == '*' )
		{
			res = scan( wildcards, wildend, strcomp, strend );
			wildcards --;
		}
		else
		{
			res = ( *wildcards == *strcomp );
			strcomp ++;
		}
		wildcards ++;
	}
	while ( *wildcards == '*' && res )  wildcards ++;

	return res && strcomp == strend && wildcards == wildend;
}

bool RDOLogCtrlFindInList::operator()( string nextstr )
{
	if ( !matchWholeWord && strToFind.find_first_of( "*?" ) == string::npos ) {
		//strtofind.Insert( 0, "*");
		strToFind.insert( 0, "*");
		strToFind += "*";
	}

	string str = nextstr;
	
	if ( !matchCase ) {
		transform( strToFind.begin(), strToFind.end(), strToFind.begin(), tolower );
		transform( str.begin(), str.end(), str.begin(), tolower );
	}
	
	if ( matchWholeWord )
		return strToFind == str;
	log->posFind ++;
	string::iterator findstrb = strToFind.begin();
	string::iterator findstre = strToFind.end();
	string::iterator strb = str.begin();
	string::iterator stre = str.end();
	//return match( strToFind.begin(), strToFind.end(),  str.begin(), str.end() );
	return match( findstrb, findstre,  strb, stre );
}

// ----------------------------------------------------------------------------
// ---------- RDOLogCtrl
// ----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC( RDOLogCtrl, CWnd )

BEGIN_MESSAGE_MAP( RDOLogCtrl, CWnd )
	//{{AFX_MSG_MAP(RDOLogCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOLogCtrl::RDOLogCtrl( RDOLogStyle* style ):
	CWnd(),
	lineHeight( 0 ),
	charWidth( 0 ),
	maxStrWidth( 0 ),
	xPos( 0 ),
	yPos( 0 ),
	xMax( 0 ),
	yMax( 0 ),
	xPageSize( 0 ),
	yPageSize( 0 ),
	clipRect( 0, 0, 0, 0 ),
	prevClientRect( 0, 0, 0, 0 ),
	newClientRect(0, 0, 0, 0 ),
	prevWindowRect( 0, 0, 0, 0 ),
	lastViewableLine( 0 ),
	hasFocus( false ),
	selectedLine( -1 ),
	fullRepaintLines( 0 ),
	focusOnly( false ),
	logStyle( style ),
	stringsCount( 0 ),
	firstFoundLine( -1 ),
	posFind( -1 ),
	bHaveFound( false ),
	bSearchDown( true ),
	bMatchCase( false ),
	bMatchWholeWord( false )
{
	//if no style specified default style will be used
	if ( !logStyle ) {
		logStyle = &studioApp.mainFrame->style_trace;
	}
}

RDOLogCtrl::~RDOLogCtrl()
{
}

BOOL RDOLogCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CWnd::PreCreateWindow( cs ) ) return FALSE;
	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW)/*, reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL*/ );
	return TRUE;
}

int RDOLogCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CWnd::OnCreate( lpCreateStruct ) == -1 ) return -1;
	setFont( false );
	updateScrollBars();
	return 0;
}

void RDOLogCtrl::OnSize( UINT nType, int cx, int cy )
{
		//In our case OnSize() invalidates all needed rectangles.
		//Default handler invalidates all client area.
//		CWnd::OnSize(  nType, cx, cy );

		GetClientRect( &newClientRect );

		CRect newWindowRect;
		GetWindowRect( &newWindowRect );

		int prevYPos = yPos;
		int prevXPos = xPos;
		//updateScrollBars() uses newClientRect so call it
		//after setting up newClientRect
		updateScrollBars();

		//isFullyVisible() uses newClientRect so call it
		//after setting up newClientRect
		bool lastLineVisible = isFullyVisible( stringsCount - 1 );
		bool lastCharVisible = maxStrWidth == xPos + newClientRect.Width() / charWidth;
		
		bool fullVisibleVert = !yPos && lastLineVisible;
		bool fullVisibleHorz = !xPos && lastCharVisible;
		
		bool needShiftVert = yPos < prevYPos && !fullVisibleVert;
		bool needShiftHorz = xPos < prevXPos && !fullVisibleHorz;
		
		bool topChanged = prevWindowRect.top != newWindowRect.top;
		int dx = newClientRect.right - prevClientRect.right;
		int dy = newClientRect.bottom - prevClientRect.bottom;
		
		CRect prevClRectBackup;
		prevClRectBackup.CopyRect( &prevClientRect );
		
		int mul = newClientRect.Height() / lineHeight;
		if ( mul * lineHeight < newClientRect.Height() )
			mul++;
		lastViewableLine = yPos + mul - 1;

		prevClientRect = newClientRect;
		prevWindowRect = newWindowRect;

		//If top of the window changed repainting all window
		//because default OnSize() handler invalidates all
		//client rectangle.
		//If top is the same validating all client rectangle
		//and calculating invalid rectangle.
		if ( !topChanged ) {

			ValidateRect( &newClientRect );

			//If new window size is smaller than the previous
			//no repainting is needed.
			if ( dx < 0 && dy < 0 )
				return;
			
			CRgn bottomRgn;
			if ( dy )
				bottomRgn.CreateRectRgn( newClientRect.left, prevClRectBackup.bottom - 1, newClientRect.right, newClientRect.bottom );
			else
				bottomRgn.CreateRectRgn( 0, 0, 0, 0 );

			//Substracting 1 pixel to remove old focus rectangle.
			CRgn rightRgn;
			if ( dx )
				rightRgn.CreateRectRgn( prevClRectBackup.right - 1, newClientRect.top, newClientRect.right, newClientRect.bottom );
			else
				rightRgn.CreateRectRgn( 0, 0, 0, 0 );
			
			CRgn invalidRgn;
			invalidRgn.CreateRectRgn( 0, 0, 0, 0 );
			int res = invalidRgn.CombineRgn( &bottomRgn, &rightRgn, RGN_OR );

			if ( res == NULLREGION || res == ERROR ) {
				
				invalidRgn.DeleteObject();
				invalidRgn.CreateRectRgn( newClientRect.left, newClientRect.top, newClientRect.right, newClientRect.bottom );
			
			} else if ( needShiftVert || needShiftHorz ) {
				
				//If scrolled vertically to the end of log
				//and resizing forces to appear new line
				//at the top of the log.
				if ( needShiftVert ) {

					ScrollWindowEx( 0, lineHeight * ( prevYPos - yPos ),
						(CONST CRect *) NULL, NULL,
						(CRgn*) NULL, (CRect*) NULL, SW_INVALIDATE );
					if ( dx )
						InvalidateRgn( &rightRgn );
				}
				
				//If scrolled horizontally to the end of log
				//and resizing forces to appear new char
				//at the left of the log.
				if ( needShiftHorz ) {

					ScrollWindowEx( charWidth * ( prevXPos - xPos ), 0,
						(CONST CRect*)NULL, (CONST CRect*)NULL,
						(CRgn*)NULL, (CRect*)NULL, SW_INVALIDATE );
			
					if ( isVisible( selectedLine) ) {
						CRect rect;
						getLineRect( selectedLine, &rect );
						InvalidateRect( &rect );
					}
					
					if ( dy )
						InvalidateRgn( &bottomRgn );
				}
				//All needed rectangles invalidated
				//by ScrollWindowEx() so RETURN.
				return;
			}
			
			InvalidateRgn( &invalidRgn );
			
			bottomRgn.DeleteObject();
			rightRgn.DeleteObject();
			invalidRgn.DeleteObject();
		} else {
			InvalidateRect( &newClientRect );
		}
}

bool RDOLogCtrl::getItemColors( const int index,COLORREF& textColor, COLORREF& backColor ) const
{
	return logStyle->getItemColors( index, textColor, backColor );
}

bool RDOLogCtrl::getItemColors( const string& item, COLORREF& textColor, COLORREF& backColor ) const
{
	return logStyle->getItemColors( item, textColor, backColor );
}

void RDOLogCtrl::OnPaint()
{
	PAINTSTRUCT ps;
	CDC* dc = BeginPaint( &ps );

	int prevDC = dc->SaveDC();

	dc->SetBkMode( TRANSPARENT );

	if ( !IsRectEmpty( &(ps.rcPaint) ) ) {
	
		CFont*  prevFont  = dc->SelectObject( &fontLog );
		
		int firstLine = max ( 0, yPos + ps.rcPaint.top / lineHeight );
		int mul = ps.rcPaint.bottom / lineHeight;
		if ( ps.rcPaint.bottom > mul * lineHeight ) mul++;
		int lastLine = min ( stringsCount - 1, yPos + mul - 1 );

		COLORREF back;
		COLORREF front;

		int y = lineHeight * ( -yPos + firstLine - 1 );
		CRect rect( charWidth * ( -xPos ), y, ps.rcPaint.right, y + lineHeight );
		stringList::const_iterator it = findString( firstLine );
		for ( int i = firstLine; i < lastLine + 1; i++ ) {

			if ( i != selectedLine || focusOnly ) {
				if ( !( getItemColors( (*it), front, back ) || getItemColors( i, front, back ) ) ) {
					front = dc->GetTextColor();
					back  = dc->GetBkColor();
				}
			} else {
				front = ::GetSysColor( COLOR_HIGHLIGHTTEXT );
				back  = ::GetSysColor( COLOR_HIGHLIGHT );
			}

			int backdc = dc->SaveDC();

			CBrush brush ( back );
			CPen pen( PS_SOLID, 1, back );
			dc->SelectObject( &pen );
			dc->SelectObject( &brush );

			rect.OffsetRect( 0, lineHeight );

			dc->Rectangle( &rect );
			
			dc->SetTextColor( front );

			rect.left += logStyle->horzBorder;

			dc->DrawText( (*it).c_str(), rect, DT_SINGLELINE | DT_LEFT | DT_VCENTER );

			rect.left -= logStyle->horzBorder;
			
			dc->RestoreDC( backdc );

			if ( i == selectedLine && hasFocus ) {
				CRect focusRect;
				focusRect.CopyRect( &newClientRect );
				focusRect.top = rect.top;
				focusRect.bottom = rect.bottom;
				dc->DrawFocusRect( &focusRect );
			}

			it++;
		}

		dc->FillSolidRect( ps.rcPaint.left, rect.bottom, ps.rcPaint.right, ps.rcPaint.bottom, dc->GetBkColor() );
	}
	
	dc->RestoreDC( prevDC );

	EndPaint( &ps );
}

void RDOLogCtrl::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	int inc;
	SCROLLINFO si;
	si.cbSize = sizeof( si );

	switch( nSBCode ) {
		case SB_PAGEUP:
			inc = -xPageSize;
			break; 

		case SB_PAGEDOWN:
			inc = xPageSize;
			break;

		case SB_LINEUP:
			inc = -1;
			break;

		case SB_LINEDOWN:
			inc = 1;
			break;

		case SB_THUMBTRACK: {
			GetScrollInfo( SB_HORZ, &si, SIF_TRACKPOS );
			inc = si.nTrackPos - xPos;
			break;
		}
		default:
			inc = 0;
	}

	scrollHorizontally( inc );
}

void RDOLogCtrl::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	int inc;
	SCROLLINFO si;
	si.cbSize = sizeof( si );

	switch( nSBCode ) {
		case SB_TOP:
			inc = -yPos;
			break;
		case SB_PAGEUP:
			inc = min( -1, -yPageSize );
			break; 

		case SB_PAGEDOWN:
			inc = max( 1, yPageSize );
			break;

		case SB_LINEUP:
			inc = -1;
			break;

		case SB_LINEDOWN:
			inc = 1;
			break;

		case SB_THUMBTRACK: {
			GetScrollInfo( SB_VERT, &si, SIF_TRACKPOS );
			inc = si.nTrackPos - yPos;
			break;
		}
		case SB_BOTTOM:
			inc = yMax - yPos;
			break;
		default:
			inc = 0;
	}

	scrollVertically( inc );
}

BOOL RDOLogCtrl::OnEraseBkgnd(CDC* pDC) 
{
  return TRUE;
}

void RDOLogCtrl::OnSetFocus( CWnd* pOldWnd )
{
	CWnd::OnSetFocus( pOldWnd );
	hasFocus = true;
	repaintLine( selectedLine );
}

void RDOLogCtrl::OnKillFocus( CWnd* pNewWnd )
{
	CWnd::OnKillFocus( pNewWnd );
	hasFocus = false;
	repaintLine( selectedLine );
}

void RDOLogCtrl::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	WORD scrollNotify = 0xFFFF;
	UINT msg = WM_VSCROLL;
	
	switch ( nChar ) {
	
		case VK_UP:
			selectLine( selectedLine - 1 );
			break;

		case VK_PRIOR:
			selectLine( max ( selectedLine - yPageSize, 0 ) );
			break;

		case VK_NEXT:
			selectLine( min ( selectedLine + yPageSize, stringsCount - 1 ) );
			break;

		case VK_DOWN:
			selectLine( selectedLine + 1 );
			break;

		case VK_HOME:
			selectLine( 0 );
			break;

		case VK_END:
			selectLine( stringsCount - 1 );
			break;

		case VK_LEFT: {
			scrollNotify = SB_LINEUP;
			msg = WM_HSCROLL;
			break;
		}
		
		case VK_RIGHT: {
			scrollNotify = SB_LINEDOWN;
			msg = WM_HSCROLL;
			break;
		}
		default:
			break;
	}

	if (scrollNotify != -1) 
		::SendMessage( m_hWnd, msg, MAKELONG(scrollNotify, 0), NULL );
	
}

BOOL RDOLogCtrl::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	WORD scrollNotify = 0xFFFF;
	
	if ( zDelta < 0 )
		scrollNotify = SB_LINEDOWN;
	else
		scrollNotify = SB_LINEUP;
	
	::SendMessage( m_hWnd, WM_VSCROLL, MAKELONG(scrollNotify, 0), NULL );
	
	return TRUE;
}

void RDOLogCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
	CWnd::OnLButtonDown( nFlags, point );
	SetFocus();
	selectLine( min( yPos + point.y / lineHeight, stringsCount - 1 ) );
}

void RDOLogCtrl::recalcWidth( const int newMaxStrWidth )
{
	if ( maxStrWidth != newMaxStrWidth ) {
		int width = maxStrWidth * charWidth;
		int newwidth = newMaxStrWidth * charWidth + 2 * logStyle->horzBorder;

		if ( newwidth > width ) {
			maxStrWidth = newwidth / charWidth;
			if ( maxStrWidth * charWidth < newwidth )
				maxStrWidth++;
		}
	}
}

void RDOLogCtrl::updateScrollBars()
{
	xPageSize = newClientRect.Width() / charWidth;
	yPageSize = newClientRect.Height() / lineHeight;

	yMax = max ( 0, stringsCount - yPageSize );
	yPos = min ( yPos, yMax );
	int mul = yPageSize;
	if ( mul * lineHeight < newClientRect.Height() )
		mul++;
	lastViewableLine = yPos + mul - 1;

	SCROLLINFO si;
	si.cbSize = sizeof( si );
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS; 
	si.nMin   = 0; 
	
	si.nMax   = stringsCount - 1; 
	si.nPage  = yPageSize; 
	si.nPos   = yPos; 
	SetScrollInfo( SB_VERT, &si, TRUE );
	
	xMax = max ( 0, maxStrWidth - xPageSize );
	xPos = min ( xPos, xMax ); 
	
	si.nMax   = maxStrWidth - 1; 
	si.nPage  = xPageSize; 
	si.nPos   = xPos; 
	SetScrollInfo( SB_HORZ, &si, TRUE );
}

bool RDOLogCtrl::scrollVertically( int inc )
{
	bool res = false;
	if ( !inc ) return res;

	// If applying the vertical scrolling increment does not
	// take the scrolling position out of the scrolling range, 
	// increment the scrolling position, adjust the position 
	// of the scroll box, and update the window.
	if ( inc = max ( -yPos, min ( inc, yMax - yPos ) ) ) {
		yPos += inc;
		lastViewableLine += inc;
		
		CRect rect;
		rect.CopyRect( &newClientRect );

		if ( fullRepaintLines )
			if ( inc > 0 )
				rect.bottom = ( rect.Height() / lineHeight - fullRepaintLines + 1 ) * lineHeight;
			else
				rect.top = ( fullRepaintLines - 1 )  * lineHeight;
		
		ScrollWindowEx( 0, -lineHeight * inc,
			(CONST CRect *) NULL, &rect,
			(CRgn*) NULL, NULL, SW_INVALIDATE );
		/*ScrollWindowEx( 0, -lineHeight * inc,
			(CONST CRect *) NULL, NULL,
			(CRgn*) NULL, (CRect*) NULL, 0 );*/

		SCROLLINFO si;
		si.cbSize = sizeof( si );
		si.fMask  = SIF_POS;
		si.nPos   = yPos;
		SetScrollInfo( SB_VERT, &si, TRUE );

		if ( fullRepaintLines ) {
			rect.CopyRect( &newClientRect );
			if ( inc > 0 ) {
				rect.top = ( rect.Height() / lineHeight - fullRepaintLines + 1 ) * lineHeight;
			} else {
				rect.bottom = ( fullRepaintLines - 1 ) * lineHeight;
			}
			InvalidateRect( &rect );
		}
		
		/*rect.CopyRect( &newClientRect );
		if ( inc > 0 ) {
			rect.top = rect.Height() - inc * lineHeight;
			if ( fullRepaintLines )
				rect.top = ( rect.top / lineHeight - fullRepaintLines + 1) * lineHeight;
		} else {
			rect.bottom = (-inc) * lineHeight;
			if ( fullRepaintLines )
				rect.bottom = ( rect.bottom / lineHeight + fullRepaintLines - 1) * lineHeight;
		}
		
		InvalidateRect( &rect );*/
		UpdateWindow ();
		res = true;
	}
	return res;
}

bool RDOLogCtrl::scrollHorizontally( int inc )
{
	bool res = false;
	if ( !inc ) return res;

	// If applying the horizontal scrolling increment does not 
	// take the scrolling position out of the scrolling range, 
	// increment the scrolling position, adjust the position 
	// of the scroll box, and update the window.
	if ( inc = max ( -xPos, min ( inc, xMax - xPos ) ) ) {
		xPos += inc;
		
		ScrollWindowEx( -charWidth * inc, 0,
			(CONST CRect*)NULL, (CONST CRect*)NULL,
			(CRgn*)NULL, (CRect*)NULL, SW_INVALIDATE );
		
		if ( isVisible( selectedLine) ) {
			CRect rect;
			getLineRect( selectedLine, &rect );
			InvalidateRect( &rect );
		}
		
		SCROLLINFO si;
		si.cbSize = sizeof( si );
		si.fMask  = SIF_POS;
		si.nPos   = xPos;
		SetScrollInfo( SB_HORZ, &si, TRUE );

		UpdateWindow();
		res = true;
	}
	return res;
}

bool RDOLogCtrl::isVisible( const int index ) const
{
	return index <= lastViewableLine && index >= yPos;
}

bool RDOLogCtrl::isFullyVisible( const int index ) const
{
	int lastVisible = yPos + newClientRect.Height() / lineHeight - 1;
	return index <= lastVisible && index >= yPos;
}

void RDOLogCtrl::selectLine( const int index )
{
	if ( index < 0 || index > stringsCount - 1 || index == selectedLine )
		return;
	int prevSel = selectedLine;
	int inc = max ( - prevSel, min ( index - prevSel, stringsCount - 1 - prevSel ) );

	if ( inc ) {
		selectedLine += inc;
		
		//makeLineVisible() scrolls to the line and repaints
		//it and nearby line if scrolling occurs.
		//If no scrolling is done repaint line
		bool needrepaint = !makeLineVisible( selectedLine );
		if ( needrepaint )
			repaintLine( selectedLine );

		//repaintLine() repaints line only if it's visible
		repaintLine( prevSel );
	}
	CWnd* parent = GetParent();
	if ( parent )
		::SendMessage( parent->m_hWnd, WM_LOGSELCHANGE, (WPARAM)prevSel, (LPARAM)selectedLine );
}

void RDOLogCtrl::getLineRect( const int index, CRect* rect ) const
{
	rect->CopyRect( &newClientRect );
	rect->top = ( index - yPos ) * lineHeight;
	rect->bottom = min( rect->top + lineHeight, rect->bottom );
}

void RDOLogCtrl::repaintLine ( const int index )
{
	if ( isVisible( index ) ) {
		CRect rect;
		getLineRect( index, &rect );
		InvalidateRect( &rect );
		UpdateWindow();
	}
}

bool RDOLogCtrl::makeLineVisible( const int index )
{
	bool res = false;
	
	if ( isFullyVisible( index ) )
		return res;

	int inc;
	if ( yPos < index ) {
		int lastVisible = yPos + newClientRect.Height() / lineHeight - 1;
		inc = index - lastVisible;
	} else
		inc = index - yPos;
	
	//Repainting nearby lines after scrolling
	fullRepaintLines = 2;
	
	res = scrollVertically( inc );
	
	fullRepaintLines = 0;
	
	return res;
}

void RDOLogCtrl::addStringToLog( const string& logStr )
{
	if ( GetSafeHwnd() ) {
		bool prevVisible = isVisible( stringsCount - 1 );

		strings.push_back( logStr );
		stringsCount ++;

		recalcWidth( logStr.length() );

		updateScrollBars();

		int lastString = stringsCount - 1;

		fullRepaintLines = 1;

		if (  selectedLine != -1 && selectedLine == lastString - 1 ) {
			selectedLine = lastString;
			fullRepaintLines ++;
		}

		if ( !isFullyVisible( lastString ) && prevVisible && ( !isVisible( selectedLine ) || selectedLine == lastString ) )
			::SendMessage( m_hWnd, WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), NULL );
		else if ( isVisible( lastString ) ) {
			repaintLine( lastString );
			if ( fullRepaintLines == 2 )
				repaintLine( lastString - 1 );
		}

		fullRepaintLines = 0;
	}
}

const stringList& RDOLogCtrl::getLogStrings() const
{
	return strings;
}

const RDOLogStyle& RDOLogCtrl::getStyle() const
{
	return (*logStyle);
}

void RDOLogCtrl::setStyle( RDOLogStyle* style, const bool needRedraw )
{
	logStyle = style;
	setFont( false );
	
	recalcWidth( maxStrWidth );
	updateScrollBars();
	
	if ( needRedraw ) {
		Invalidate();
		UpdateWindow();
	}
}

void RDOLogCtrl::setFont( const bool needRedraw )
{
	if ( !logStyle ) return;

	if ( fontLog.m_hObject )
		if ( !fontLog.DeleteObject() )
			return;
	
	LOGFONT lf;
	memset( &lf, 0, sizeof(lf) );
	// The negative is to allow for leading
	lf.lfHeight    = -MulDiv( logStyle->font->size, ::GetDeviceCaps( GetDC()->m_hDC, LOGPIXELSY ), 72 );
	lf.lfWeight    = logStyle->style & RDOFS_BOLD ? FW_BOLD : FW_NORMAL;
	lf.lfItalic    = logStyle->style & RDOFS_ITALIC;
	lf.lfUnderline = logStyle->style & RDOFS_UNDERLINE;
	lf.lfCharSet   = logStyle->font->characterSet;
	strcpy( lf.lfFaceName, logStyle->font->name.c_str() );

	if ( !fontLog.CreateFontIndirect( &lf ) )
		return;
	
	TEXTMETRIC tm;
	if ( m_hWnd ) {
		CDC* dc = GetDC();
		CFont* oldFont = dc->SelectObject( &fontLog );
		dc->GetTextMetrics( &tm );
		lineHeight = tm.tmHeight + 2 * logStyle->vertBorder;
		charWidth  = tm.tmAveCharWidth/*tm.tmMaxCharWidth*/;
		dc->SelectObject( oldFont );
	}

	
	if ( needRedraw ) {
		Invalidate();   
		UpdateWindow();
	}
}

string RDOLogCtrl::getString( const int index ) const
{
	if ( index >= 0 && index < stringsCount ) {
		return (*const_findString( index ));
	} else {
		return "";
	}
}

int RDOLogCtrl::getSelectedIndex() const
{
	return selectedLine;
}

string RDOLogCtrl::getSelected() const
{
	return getString( selectedLine );
}

void RDOLogCtrl::copy()
{
	if ( canCopy() ) {
		if ( !OpenClipboard() || !::EmptyClipboard() )
			return;
		string str = getSelected();
		char* ptr = (char*)::LocalAlloc( LMEM_FIXED, str.length() + 1 );
		strcpy( ptr, str.c_str() );
		::SetClipboardData( CF_TEXT, ptr );
		CloseClipboard();
	}
}

void RDOLogCtrl::clear()
{
	strings.clear();
	stringsCount      = 0;
	maxStrWidth       = 0;
	lastViewableLine  = 0;
	selectedLine = -1;
	updateScrollBars();
	Invalidate();
	UpdateWindow();
}

stringList::iterator RDOLogCtrl::findString( int index )
{
	stringList::iterator res = strings.begin();
	while ( index-- )
		res ++;
	return res;
}

stringList::reverse_iterator RDOLogCtrl::reverse_findString( int index )
{
	return findString( index );
}

stringList::const_iterator RDOLogCtrl::const_findString( int index ) const
{
	stringList::const_iterator res = strings.begin();
	while ( index-- )
		res ++;
	return res;
}

stringList::const_reverse_iterator RDOLogCtrl::const_reverse_findString( int index ) const
{
	return const_findString( index );
}

int RDOLogCtrl::find( const bool searchDown, const bool matchCase, const bool matchWholeWord )
{
	string strtofind = findStr;
	
	stringList::iterator it;
	stringList::reverse_iterator it_r;

	int startPos = selectedLine + 1;
	int endPos = stringsCount - 1;
	if ( !searchDown ) {
		startPos = selectedLine - 1;
		endPos   = 0;
	}

	posFind = -1;
	if ( searchDown ) {
		it = find_if(
				findString( startPos ),
				strings.end(),
				RDOLogCtrlFindInList( this, findStr, matchCase, matchWholeWord )
			);
		if ( it == strings.end() ) {
			posFind = -1;
			startPos = 0;
			endPos   = stringsCount - 1;
			it = find_if(
				strings.begin(),
				strings.end(),
				RDOLogCtrlFindInList( this, findStr, matchCase, matchWholeWord )
			);
		}
		if ( it == strings.end() )
			posFind = -1;
		else
			posFind += startPos;
	} else {
		it_r = find_if(
				reverse_findString( startPos + 1 ),
				strings.rend(),
				RDOLogCtrlFindInList( this, findStr, matchCase, matchWholeWord )
			);
		if ( it_r == strings.rend() ) {
			posFind = -1;
			startPos = stringsCount - 1;
			endPos   = 0;
			it_r = find_if(
				strings.rbegin(),
				strings.rend(),
				RDOLogCtrlFindInList( this, findStr, matchCase, matchWholeWord )
			);
		}
		if ( it_r == strings.rend() )
			posFind = -1;
		else
			posFind = startPos - posFind;
	}
	
	if ( posFind == -1 ) {
		firstFoundLine = -1;
		bHaveFound    = false;
		return -1;
	} else {
		if ( firstFoundLine == -1 ) {
			firstFoundLine = posFind;
		} else if ( posFind == firstFoundLine ) {
			firstFoundLine = -1;
			bHaveFound    = false;
			return -1;
		}
		bHaveFound = true;
		return posFind;
	}
	return -1;
}
