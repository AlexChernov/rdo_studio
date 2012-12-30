/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_ctrl.cpp
  \author    ������� �����
  \date      12.03.2003
  \brief
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qpainter.h>
#include <QtGui/qscrollbar.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoTracerLog;
using namespace rdoStyle;

namespace rdoTracerLog
{

// --------------------------------------------------------------------------------
// -------------------- RDOLogCtrlFindInList
// --------------------------------------------------------------------------------
class RDOLogCtrlFindInList
{
	RDOLogCtrl* log;
	tstring strToFind;
	rbool matchCase;
	rbool matchWholeWord;

	rbool scan(tstring::iterator &wildCards, tstring::iterator &wildend, tstring::iterator &str, tstring::iterator &strend) const;
	rbool match(tstring::iterator &wildcards, tstring::iterator &wildend, tstring::iterator &strcomp, tstring::iterator &strend) const;
public:
	RDOLogCtrlFindInList(RDOLogCtrl* _log, tstring _strToFind, rbool _matchCase, rbool _matchWholeWord);
	rbool operator()(tstring nextstr);
};

}; // namespace rdoTracerLog

RDOLogCtrlFindInList::RDOLogCtrlFindInList(RDOLogCtrl* _log, tstring _strToFind, rbool _matchCase, rbool _matchWholeWord)
	: log(_log),
	  strToFind(_strToFind),
	  matchCase(_matchCase),
	  matchWholeWord(_matchWholeWord)
{
}

rbool RDOLogCtrlFindInList::scan(tstring::iterator &wildCards, tstring::iterator &wildend, tstring::iterator &str, tstring::iterator &strend) const
{
	// remove the '?' and '*'
	for(wildCards ++; str != strend && (*wildCards == '?' || *wildCards == '*'); wildCards ++)
		if (*wildCards == '?')
		{
			str ++;
		}
	while (*wildCards == '*')
	{
		wildCards ++;
	}

	// if str is empty and Wildcards has more characters or,
	// Wildcards is empty, return
	if (str == strend && wildCards != wildend)
	{
		return false;
	}
	if (str == strend && wildCards == wildend)
	{
		return true;
	}
	// else search substring
	else
	{
		tstring::iterator wdsCopy = wildCards;
		tstring::iterator strCopy = str;
		rbool res = 1;
		do
		{
			if (!match(wildCards, wildend, str, strend))
			{
				strCopy ++;
			}
			wildCards = wdsCopy;
			str		  = strCopy;
			while ((*wildCards != *str) && (str != strend))
			{
				str ++;
			}
			wdsCopy = wildCards;
			strCopy = str;
		}
		while ((str != strend) ? !match(wildCards, wildend, str, strend) : (res = false) != false);

		if (str == strend && wildCards == wildend)
		{
			return true;
		}

		return res;
	}
}

rbool RDOLogCtrlFindInList::match(tstring::iterator &wildcards, tstring::iterator &wildend, tstring::iterator &strcomp, tstring::iterator &strend) const
{
	rbool res = true;

	tstring strWild;
	tstring strComp;
	if (wildcards != wildend)
	{
		strWild.assign(&(*wildcards));
	}
	if (strcomp != strend)
	{
		strComp.assign(&(*strcomp));
	}
	tstring::iterator strWildb = strWild.begin();
	tstring::iterator strWilde = strWild.end();
	tstring::iterator strCompb = strComp.begin();
	tstring::iterator strCompe = strComp.end();

	//iterate and delete '?' and '*' one by one
	while(strWildb != strWilde && res && strCompb != strCompe)
	{
		if (*strWildb == '?')
		{
			strCompb ++;
		}
		else if (*strWildb == '*')
		{
			res = scan(strWildb, strWilde, strCompb, strCompe);
			strWildb --;
		}
		else
		{
			res = (*strWildb == *strCompb);
			strCompb ++;
		}
		strWildb ++;
	}
	while (*strWildb && *strWildb == '*' && res)
	{
		strWildb ++;
	}

	return res && strCompb == strCompe && strWildb == strWilde;
}

rbool RDOLogCtrlFindInList::operator()(tstring nextstr)
{
	if (!matchWholeWord && strToFind.find_first_of("*?") == tstring::npos)
	{
		strToFind.insert(0, "*");
		strToFind += "*";
	}

	tstring str = nextstr;

	if (!matchCase)
	{
		std::transform(strToFind.begin(), strToFind.end(), strToFind.begin(), tolower);
		std::transform(str.begin(), str.end(), str.begin(), tolower);
	}

	log->posFind ++;

	if (matchWholeWord)
	{
		return strToFind == str;
	}

	tstring::iterator findstrb = strToFind.begin();
	tstring::iterator findstre = strToFind.end();
	tstring::iterator strb = str.begin();
	tstring::iterator stre = str.end();
	return match(findstrb, findstre,  strb, stre);
}

// --------------------------------------------------------------------------------
// -------------------- RDOLogCtrl
// --------------------------------------------------------------------------------
RDOLogCtrl::RDOLogCtrl(PTR(QAbstractScrollArea) pParent, PTR(RDOLogStyle) pStyle)
	: parent_type(pParent)
	, m_pScrollArea(pParent)
	, lineHeight(0)
	, charWidth(0)
	, maxStrWidth(0)
	, xPos(0)
	, yPos(0)
	, xMax(0)
	, yMax(0)
	, xPageSize(0)
	, yPageSize(0)
	, clipRect(0, 0, 0, 0)
	, prevClientRect(0, 0, 0, 0)
	, newClientRect (0, 0, 0, 0)
	, prevWindowRect(0, 0, 0, 0)
	, lastViewableLine(0)
	, selectedLine(-1)
	, fullRepaintLines(0)
	, focusOnly(false)
	, logStyle(pStyle)
	, firstFoundLine(-1)
	, posFind(-1)
	, bHaveFound(false)
	, bSearchDown(true)
	, bMatchCase(false)
	, bMatchWholeWord(false)
	, drawLog(true)
	, m_prevVertSBValue(0)
	, m_prevHorzSBValue(0)
{
	if (!logStyle)
	{
		logStyle = &studioApp.getStyle()->style_trace;
	}

	connect(&getVertScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onVertScrollBarValueChanged(int)));
	connect(&getHorzScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onHorzScrollBarValueChanged(int)));

	setFocusPolicy(Qt::ClickFocus);

	setFont();
	updateScrollBars();
}

RDOLogCtrl::~RDOLogCtrl()
{
	clear();
}

REF(QScrollBar) RDOLogCtrl::getVertScrollBar()
{
	PTR(QScrollBar) pScrollBar = m_pScrollArea->verticalScrollBar();
	ASSERT(pScrollBar);
	return *pScrollBar;
}

REF(QScrollBar) RDOLogCtrl::getHorzScrollBar()
{
	PTR(QScrollBar) pScrollBar = m_pScrollArea->horizontalScrollBar();
	ASSERT(pScrollBar);
	return *pScrollBar;
}

void RDOLogCtrl::resizeEvent(QResizeEvent* pEvent)
{
	parent_type::resizeEvent(pEvent);

	newClientRect = QRect(QPoint(0, 0), pEvent->size());

	QRect newWindowRect(
		mapToGlobal(newClientRect.topLeft()),
		mapToGlobal(newClientRect.bottomRight())
	);

	int prevYPos = yPos;
	int prevXPos = xPos;
	updateScrollBars();

	rbool lastLineVisible = isFullyVisible(strings.count() - 1);
	rbool lastCharVisible = maxStrWidth == xPos + newClientRect.width() / charWidth;

	rbool fullVisibleVert = !yPos && lastLineVisible;
	rbool fullVisibleHorz = !xPos && lastCharVisible;

	rbool needShiftVert = yPos < prevYPos && !fullVisibleVert;
	rbool needShiftHorz = xPos < prevXPos && !fullVisibleHorz;

	rbool topChanged = prevWindowRect.top() != newWindowRect.top();
	int dx = newClientRect.right() - prevClientRect.right();
	int dy = newClientRect.bottom() - prevClientRect.bottom();

	QRect prevClRectBackup(prevClientRect);

	int mul = newClientRect.height() / lineHeight;
	if (mul * lineHeight < newClientRect.height())
	{
		++mul;
	}
	lastViewableLine = yPos + mul - 1;

	prevClientRect = newClientRect;
	prevWindowRect = newWindowRect;

	if (!topChanged)
	{
		update(newClientRect);

		if (dx < 0 && dy < 0)
		{
			return;
		}

		QRegion bottomRgn = dy
			? QRegion(newClientRect.left(), prevClRectBackup.bottom() - 1, newClientRect.right(), newClientRect.bottom())
			: QRegion(0, 0, 0, 0);

		//Substracting 1 pixel to remove old focus rectangle.
		QRegion rightRgn = dx
			? QRegion(prevClRectBackup.right() - 1, newClientRect.top(), newClientRect.right(), newClientRect.bottom())
			: QRegion(0, 0, 0, 0);

		QRegion invalidRgn = bottomRgn.united(rightRgn);

		if (invalidRgn.isEmpty())
		{
			invalidRgn = QRegion(newClientRect.left(), newClientRect.top(), newClientRect.right(), newClientRect.bottom());
		}
		else if (needShiftVert || needShiftHorz)
		{
			if (needShiftVert)
			{
				getVertScrollBar().setValue(lineHeight * (prevYPos - yPos));
				if (getVertScrollBar().isHidden())
				{
					getVertScrollBar().show();
				}

				if (dx)
				{
					update(rightRgn);
				}
			}

			if (needShiftHorz)
			{
				getHorzScrollBar().setValue(charWidth * (prevXPos - xPos));
				if (getHorzScrollBar().isHidden())
				{
					getHorzScrollBar().show();
				}

				if (isVisible(selectedLine))
				{
					update(getLineRect(selectedLine));
				}

				if (dy)
				{
					update(bottomRgn);
				}
			}
			return;
		}

		update(invalidRgn);
	}
	else
	{
		update(newClientRect);
	}
}

rbool RDOLogCtrl::getItemColors(int index, RDOLogColorPair* &colors) const
{
	return logStyle->getItemColors(index, colors);
}

rbool RDOLogCtrl::getItemColors(CREF(tstring) item, RDOLogColorPair* &colors) const
{
	return logStyle->getItemColors(item, colors);
}

void RDOLogCtrl::paintEvent(QPaintEvent* pEvent)
{
	mutex.Lock();

	QPainter painter(this);

	if (drawLog)
	{
		if (!pEvent->rect().isEmpty() && !pEvent->rect().isNull())
		{
			painter.setFont(m_font);

			int firstLine = max (0, yPos + pEvent->rect().top() / lineHeight);
			int mul = pEvent->rect().bottom() / lineHeight;
			if (pEvent->rect().bottom() > mul * lineHeight)
			{
				mul++;
			}
			int lastLine = (std::min)(strings.count() - 1, yPos + mul - 1);

			RDOLogColorPair* colors = NULL;

			int y = lineHeight * (-yPos + firstLine - 1);
			QRect rect(charWidth * (-xPos), y, pEvent->rect().width(), lineHeight);
			QRect textRect(
				rect.left  () + logStyle->borders->horzBorder,
				rect.top   () + logStyle->borders->vertBorder,
				rect.width () - logStyle->borders->horzBorder * 2,
				rect.height() - logStyle->borders->vertBorder * 2
			);

			StringList::const_iterator it = strings.findString(firstLine);
			for (int i = firstLine; i < lastLine + 1; i++)
			{
				if (i != selectedLine || focusOnly)
				{
					if (!getItemColors((*it), colors))
					{
						getItemColors(i, colors);
					}
				}
				else
				{
					colors = new RDOLogColorPair();
					colors->foregroundColor = palette().color(QPalette::HighlightedText);
					colors->backgroundColor = palette().color(QPalette::Highlight);
				}

				rect    .translate(0, lineHeight);
				textRect.translate(0, lineHeight);

				//Main drawing cycle
				painter.setBackgroundMode(Qt::TransparentMode);
				painter.fillRect(rect, colors->backgroundColor);
				painter.setPen  (colors->foregroundColor);
				painter.drawText(
					textRect,
					QString::fromStdString(*it)
				);
				//End of main drawing cycle :)

				if (i == selectedLine && hasFocus())
				{
					QRect focusRect(newClientRect);
					focusRect.setTop   (rect.top   ());
					focusRect.setBottom(rect.bottom());
					QStyleOptionFocusRect option;
					option.initFrom(this);
					option.backgroundColor = palette().color(QPalette::Background);
					option.rect = focusRect;
					style()->drawPrimitive(QStyle::PE_FrameFocusRect, &option, &painter, this);
				}

				++it;

				if (i == selectedLine && !focusOnly && colors)
				{
					delete colors;
					colors = NULL;
				}
			}

			getItemColors("", colors);

			painter.fillRect(
				pEvent->rect().left(),
				rect.bottom(),
				pEvent->rect().width(),
				pEvent->rect().height() - rect.bottom(),
				colors->backgroundColor
			);
		}
	}
	else
	{
		RDOLogColorPair* colors = NULL;
		getItemColors("", colors);

		painter.fillRect(
			newClientRect,
			colors->backgroundColor
		);
	}

	mutex.Unlock();

	parent_type::paintEvent(pEvent);
}

void RDOLogCtrl::onVertScrollBarValueChanged(int value)
{
	if (value < 0)
	{
		return;
	}

	int inc = value - m_prevVertSBValue;
	TRACE3("onVertScrollBarValueChanged %d, %d, %d\n", value, m_prevVertSBValue, inc);
	m_prevVertSBValue = value;
	scrollVertically(inc);
}

void RDOLogCtrl::onHorzScrollBarValueChanged(int value)
{
	if (value < 0)
	{
		return;
	}

	int inc = value - m_prevHorzSBValue;
	TRACE3("onHorzScrollBarValueChanged %d, %d, %d\n", value, m_prevVertSBValue, inc);
	m_prevHorzSBValue = value;
	scrollHorizontally(inc);
}

//! @todo qt
//void RDOLogCtrl::OnSetFocus(CWnd* pOldWnd)
//{
//	CWnd::OnSetFocus(pOldWnd);
//	repaintLine(selectedLine);
//}
//
//void RDOLogCtrl::OnKillFocus(CWnd* pNewWnd)
//{
//	CWnd::OnKillFocus(pNewWnd);
//	repaintLine(selectedLine);
//}

void RDOLogCtrl::keyPressEvent(QKeyEvent* pEvent)
{
	switch (pEvent->key())
	{
	case Qt::Key_Up:
		selectLine(selectedLine - 1);
		break;

	case Qt::Key_PageUp:
		selectLine(max (selectedLine - yPageSize, 0));
		break;

	case Qt::Key_PageDown:
		selectLine(min (selectedLine + yPageSize, strings.count() - 1));
		break;

	case Qt::Key_Down:
		selectLine(selectedLine + 1);
		break;

	case Qt::Key_Home:
		selectLine(0);
		break;

	case Qt::Key_End:
		selectLine(strings.count() - 1);
		break;

	case Qt::Key_Left:
		getHorzScrollBar().setValue(getHorzScrollBar().value() - 1);
		break;

	case Qt::Key_Right:
		getHorzScrollBar().setValue(getHorzScrollBar().value() + 1);
		break;

	default:
		break;
	}
}

void RDOLogCtrl::wheelEvent(QWheelEvent* pEvent)
{
	getVertScrollBar().setValue(getVertScrollBar().value() + (pEvent->delta() < 0 ? 1 : -1));
}

void RDOLogCtrl::mousePressEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::LeftButton)
	{
		//! @todo qt
		//	SetFocus();
		selectLine(min(yPos + pEvent->pos().y() / lineHeight, strings.count() - 1));
	}
}

void RDOLogCtrl::recalcWidth(int newMaxStrWidth)
{
	if (maxStrWidth != newMaxStrWidth)
	{
		int width = maxStrWidth * charWidth;
		int newwidth = newMaxStrWidth * charWidth + 2 * logStyle->borders->horzBorder;

		if (newwidth > width)
		{
			maxStrWidth = newwidth / charWidth;
			if (maxStrWidth * charWidth < newwidth)
			{
				++maxStrWidth;
			}
		}
	}
}

void RDOLogCtrl::updateScrollBars()
{
	xPageSize = newClientRect.width () / charWidth;
	yPageSize = newClientRect.height() / lineHeight;

	yMax = max (0, strings.count() - yPageSize);
	yPos = min (yPos, yMax);
	strings.setCursor(yPos, yMax);
	int mul = yPageSize;
	if (mul * lineHeight < newClientRect.height())
	{
		mul++;
	}
	lastViewableLine = yPos + mul - 1;

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin   = 0;

	if (drawLog)
	{
		getVertScrollBar().setMinimum (0);
		getVertScrollBar().setMaximum (strings.count() - 1);
		getVertScrollBar().setPageStep(yPageSize);
		getVertScrollBar().setValue   (yPos);

		xMax = max (0, maxStrWidth - xPageSize);
		xPos = min (xPos, xMax);

		getHorzScrollBar().setMinimum (0);
		getHorzScrollBar().setMaximum (maxStrWidth - 1);
		getHorzScrollBar().setPageStep(xPageSize);
		getHorzScrollBar().setValue   (xPos);
	}
	else
	{
		getVertScrollBar().setMinimum (0);
		getVertScrollBar().setMaximum (0);
		getVertScrollBar().setPageStep(0);
		getVertScrollBar().setValue   (0);

		xMax = max (0, maxStrWidth - xPageSize);
		xPos = min (xPos, xMax);

		getHorzScrollBar().setMinimum (0);
		getHorzScrollBar().setMaximum (0);
		getHorzScrollBar().setPageStep(0);
		getHorzScrollBar().setValue   (0);
	}
}

rbool RDOLogCtrl::scrollVertically(int inc)
{
	rbool res = false;
	if (!inc)
	{
		return res;
	}

	// If applying the vertical scrolling increment does not
	// take the scrolling position out of the scrolling range,
	// increment the scrolling position, adjust the position
	// of the scroll box, and update the window.
	if (inc == max (-yPos, min (inc, yMax - yPos)))
	{
		yPos += inc;
		strings.setCursor(yPos, yMax);
		lastViewableLine += inc;

		updateWindow();
		res = true;

		TRACE1("scrollVertically %d ok\n", inc);
	}
	else
	{
		TRACE1("scrollVertically %d error\n", inc);
	}

	return res;
}

rbool RDOLogCtrl::scrollHorizontally(int inc)
{
	rbool res = false;
	if (!inc)
	{
		return res;
	}

	// If applying the horizontal scrolling increment does not
	// take the scrolling position out of the scrolling range,
	// increment the scrolling position, adjust the position
	// of the scroll box, and update the window.
	if (inc == max (-xPos, min (inc, xMax - xPos)))
	{
		xPos += inc;

		if (isVisible(selectedLine))
		{
			update(getLineRect(selectedLine));
		}

		updateWindow();
		res = true;
	}
	return res;
}

rbool RDOLogCtrl::isVisible(int index) const
{
	return index <= lastViewableLine && index >= yPos;
}

rbool RDOLogCtrl::isFullyVisible(int index) const
{
	int lastVisible = yPos + newClientRect.height() / lineHeight - 1;
	return index <= lastVisible && index >= yPos;
}

void RDOLogCtrl::selectLine(int index)
{
	if (index < 0 || index > strings.count() - 1 || index == selectedLine)
	{
		return;
	}

	int prevSel = selectedLine;
	int inc = max (- prevSel, min (index - prevSel, strings.count() - 1 - prevSel));

	if (inc)
	{
		selectedLine += inc;

		//makeLineVisible() scrolls to the line and repaints
		//it and nearby line if scrolling occurs.
		//If no scrolling is done repaint line
		rbool needrepaint = !makeLineVisible(selectedLine);
		if (needrepaint)
		{
			repaintLine(selectedLine);
		}

		//repaintLine() repaints line only if it's visible
		repaintLine(prevSel);
	}
}

QRect RDOLogCtrl::getLineRect(int index) const
{
	QRect rect(newClientRect);
	rect.setTop((index - yPos) * lineHeight);
	rect.setBottom(min(rect.top() + lineHeight, rect.bottom()));
	return rect;
}

void RDOLogCtrl::repaintLine(int index)
{
	if (isVisible(index))
	{
		update(getLineRect(index));
		updateWindow();
	}
}

void RDOLogCtrl::updateWindow()
{
	update();
}

rbool RDOLogCtrl::makeLineVisible(int index)
{
	rbool res = false;

	if (isFullyVisible(index))
	{
		return res;
	}

	int inc;
	if (yPos < index)
	{
		int lastVisible = yPos + newClientRect.height() / lineHeight - 1;
		inc = index - lastVisible;
	}
	else
	{
		inc = index - yPos;
	}

	//Repainting nearby lines after scrolling
	fullRepaintLines = 2;

	res = scrollVertically(inc);

	fullRepaintLines = 0;

	return res;
}

void RDOLogCtrl::addStringToLog(CREF(tstring) logStr)
{
	mutex.Lock();

//! @todo qt
	//if (!hwnd)
	//	return;

	rbool prevVisible = isVisible(strings.count() - 1);

	strings.push_back(logStr);

	recalcWidth(logStr.length());

	int lastString = strings.count() - 1;

	if (drawLog)
	{
		updateScrollBars();

		fullRepaintLines = 1;

		if ( selectedLine != -1 && selectedLine == lastString - 1)
		{
			selectedLine = lastString;
			fullRepaintLines ++;
		}

		if (!isFullyVisible(lastString) && prevVisible && (!isVisible(selectedLine) || selectedLine == lastString))
		{
			//::SendMessage(m_hWnd, WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), NULL);
			scrollVertically(yMax - yPos);
		}
		else if (isVisible(lastString))
		{
			repaintLine(lastString);
			if (fullRepaintLines == 2)
			{
				repaintLine(lastString - 1);
			}
		}

		fullRepaintLines = 0;
	}
	else
	{
		if (selectedLine != -1 && selectedLine == lastString - 1)
		{
			selectedLine = lastString;
		}
	}

	mutex.Unlock();
}

CREF(RDOLogStyle) RDOLogCtrl::getStyle() const
{
	return *logStyle;
}

void RDOLogCtrl::setStyle(RDOLogStyle* style, rbool needRedraw)
{
	logStyle = style;
	setFont();

	recalcWidth(maxStrWidth);
	updateScrollBars();

	if (needRedraw)
	{
		update();
		updateWindow();
	}
}

void RDOLogCtrl::setFont()
{
	if (!logStyle)
	{
		return;
	}

	mutex.Lock();

	m_font = QFont(logStyle->font->name.c_str());
	m_font.setBold     (logStyle->theme->style & rdoStyle::RDOStyleFont::BOLD     );
	m_font.setItalic   (logStyle->theme->style & rdoStyle::RDOStyleFont::ITALIC   );
	m_font.setUnderline(logStyle->theme->style & rdoStyle::RDOStyleFont::UNDERLINE);
	m_font.setPointSize(logStyle->font->size);

	QFontMetrics fontMetrics(m_font);
	lineHeight = fontMetrics.height() + 2 * logStyle->borders->vertBorder;
	charWidth  = fontMetrics.averageCharWidth(); // fontMetrics.maxWidth()

	mutex.Unlock();
}

void RDOLogCtrl::getString(int index, tstring& str) const
{
	const_cast<CMutex&>(mutex).Lock();

	tstring res = "";

	if (index >= 0 && index < strings.count())
	{
		str.assign(*strings.findString(index));
	}

	const_cast<CMutex&>(mutex).Unlock();
}

int RDOLogCtrl::getSelectedIndex() const
{
	return selectedLine;
}

void RDOLogCtrl::getSelected(tstring& str) const
{
	getString(selectedLine, str);
}

void RDOLogCtrl::copy()
{
//! @todo qt
//	if (canCopy()) {
//		if (!OpenClipboard() || !::EmptyClipboard())
//			return;
//		tstring str;
//		getSelected(str);
//		char* ptr = (char*)::LocalAlloc(LMEM_FIXED, str.length() + 1);
//#pragma warning(disable: 4996)
//		strcpy(ptr, str.c_str());
//#pragma warning(default: 4996)
//		::SetClipboardData(CF_TEXT, ptr);
//		CloseClipboard();
//	}
}

void RDOLogCtrl::clear()
{
	mutex.Lock();

	strings.clear();

	maxStrWidth       = 0;
	lastViewableLine  = 0;
	selectedLine = -1;

	updateScrollBars();
	update();
	updateWindow();

	mutex.Unlock();
}

void RDOLogCtrl::find(int& result, rbool searchDown, rbool matchCase, rbool matchWholeWord)
{
	mutex.Lock();

	result = -1;

	tstring strtofind = findStr;

	StringList::iterator it;
	StringList::reverse_iterator it_r;

	int startPos = selectedLine + 1;
	int endPos = strings.count() - 1;
	if (!searchDown)
	{
		startPos = selectedLine - 1;
		endPos   = 0;
	}

	posFind = -1;
	if (searchDown)
	{
		it = std::find_if(
			strings.findString(startPos),
			strings.end(),
			RDOLogCtrlFindInList(this, findStr, matchCase, matchWholeWord)
		);
		if (it == strings.end())
		{
			posFind = -1;
			startPos = 0;
			endPos   = strings.count() - 1;
			it = std::find_if(
				strings.begin(),
				strings.end(),
				RDOLogCtrlFindInList(this, findStr, matchCase, matchWholeWord)
			);
		}
		if (it == strings.end())
		{
			posFind = -1;
		}
		else
		{
			posFind += startPos;
		}
	}
	else
	{
		it_r = std::find_if(
			strings.rFindString(startPos + 1),
			strings.rend(),
			RDOLogCtrlFindInList(this, findStr, matchCase, matchWholeWord)
		);
		if (it_r == strings.rend())
		{
			posFind = -1;
			startPos = strings.count() - 1;
			endPos   = 0;
			it_r = std::find_if(
				strings.rbegin(),
				strings.rend(),
				RDOLogCtrlFindInList(this, findStr, matchCase, matchWholeWord)
			);
		}
		posFind = it_r == strings.rend()
			? -1
			: startPos - posFind;
	}

	if (posFind == -1)
	{
		firstFoundLine = -1;
		bHaveFound    = false;
		result = -1;
	}
	else
	{
		bHaveFound = true;
		result = posFind;
		if (firstFoundLine == -1)
		{
			firstFoundLine = posFind;
		}
		else if (posFind == firstFoundLine)
		{
			firstFoundLine = -1;
			bHaveFound    = false;
			result = -1;
		}
	}

	mutex.Unlock();
}

void RDOLogCtrl::setText(tstring text)
{
	clear();
	while (!text.empty())
	{
		ruint pos = text.find_first_of("\r\n");
		if (pos == tstring::npos)
		{
			pos = text.length();
		}
		addStringToLog(pos ? text.substr(0, pos) : "");
		text.erase(0, pos);
		text.erase(0, text.find_first_not_of("\r\n"));
	}
}

void RDOLogCtrl::setDrawLog(rbool value)
{
	if (drawLog != value)
	{
		drawLog = value;
		updateScrollBars();
		update();
		updateWindow();
		makeLineVisible(selectedLine);
	}
}
