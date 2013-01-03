/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      log_view.h
  \authors   ������� �����
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_CTRLS_LOG_VIEW_H_
#define _RDO_STUDIO_TRACER_CTRLS_LOG_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qwidget.h>
#include <QtGui/qevent.h>
#include <QtGui/qabstractscrollarea.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/tracer/ctrls/log_view_style.h"
#include "app/rdo_studio_mfc/src/action_activator/action_activator_widget.h"
#include "app/rdo_studio_mfc/src/dialog/find_dialog.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- LogView
// --------------------------------------------------------------------------------
class LogView: public ActionActivatorWidget
{
Q_OBJECT

public:
	LogView(PTR(QAbstractScrollArea) pParent, PTR(LogStyle) pStyle);
	virtual ~LogView();

	void  push_back (CREF(tstring) log );
	void  setText   (tstring       text);
	void  clear     ();
	void  selectLine(int index);

	rbool getFocusOnly() const;
	void  setFocusOnly(rbool value);

	CREF(LogStyle) getStyle() const;
	void setStyle(LogStyle* style, rbool needRedraw = true);

	rbool getDrawLog() const;
	void  setDrawLog(rbool value);

private:
	class StringList
	{
	public:
		typedef  std::list<tstring>            List;
		typedef  List::const_iterator          const_iterator;
		typedef  List::const_reverse_iterator  const_reverse_iterator;

		StringList();

		void push_back(CREF(tstring) value);

		const_iterator begin() const;
		const_iterator end  () const;

		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend  () const;

		void               clear    ();
		rsint              count    () const;
		tstring::size_type maxLegth () const;
		void               setCursor(rsint pos, rsint max);

		const_iterator         findString (int index) const;
		const_reverse_iterator rFindString(int index) const;

	private:
		List                m_list;
		rsint               m_count;
		List::iterator      m_cursorIt;
		rsint               m_cursor;
		tstring::size_type  m_maxLegth;

		void seek(rsint delta, REF(StringList::const_iterator) it) const;
	};

	CMutex     m_mutex;
	StringList m_strings;

	LogStyle*  m_logStyle;
	rbool getItemColors(CREF(tstring) item, LogColorPair* &colors) const;
	rbool getItemColors(int index, LogColorPair* &colors) const;

	rsint selectedLine() const;

	rsint m_lineHeight;
	rsint m_charWidth;
	rsint m_selectedLine;

	QMenu* m_pPopupMenu;

	void  setSelectedLine(rsint selectedLine);

	struct ScrollMetric
	{
		rsint position;
		rsint posMax;
		rsint pageSize;

		ScrollMetric();

		rbool applyInc(rsint delta);
	};

	struct ScrollMetricVert: public ScrollMetric
	{
		rsint lastViewableLine;

		ScrollMetricVert();
		rbool isVisible(rsint index) const;
		rbool applyInc (rsint delta);
	};

	ScrollMetric      m_SM_X;
	ScrollMetricVert  m_SM_Y;

	struct SubitemColors
	{
		typedef std::map<int, PTR(LogColorPair)> List;

		List              m_colorList;
		rbool             m_addingSubitems;
		PTR(LogColorPair) m_parentColor;

		SubitemColors()
			: m_addingSubitems(false)
			, m_parentColor   (NULL )
		{}
		SubitemColors(CREF(SubitemColors) subitemColors)
			: m_colorList     (subitemColors.m_colorList     )
			, m_addingSubitems(subitemColors.m_addingSubitems)
			, m_parentColor   (subitemColors.m_parentColor   )
		{}
	};
	SubitemColors m_subitemColors;

	QRect m_clientRect;
	QRect m_prevWindowRect;

	int m_fullRepaintLines;
	rbool m_focusOnly;

	FindDialog*          m_pFindDialog;
	FindDialog::Settings m_findSettings;
	rsint find(rbool searchDown);
	void  onFindDlgFind (CREF(FindDialog::Settings) settings);
	void  onFindDlgClose();

	tstring getString      (int index) const;
	tstring getSelected    () const;
	rbool   makeLineVisible(int index);

	void  updateScrollBars();

	rbool scrollVertically  (int pos);
	rbool scrollHorizontally(int pos);
	
	rbool isFullyVisible(int index) const;

	QRect getLineRect(int index) const;
	void  repaintLine (int index);

	void  updateWindow();

	rbool m_drawLog;

	QFont m_font;
	void setFont();

	PTR(QAbstractScrollArea) m_pScrollArea;

	typedef  ActionActivatorWidget  parent_type;
	virtual void resizeEvent    (QResizeEvent* pEvent);
	virtual void paintEvent     (QPaintEvent*  pEvent);
	virtual void keyPressEvent  (QKeyEvent*    pEvent);
	virtual void wheelEvent     (QWheelEvent*  pEvent);
	virtual void mousePressEvent(QMouseEvent*  pEvent);

	REF(QScrollBar) getVertScrollBar();
	REF(QScrollBar) getHorzScrollBar();

	void  setUpActionEditCopy(rbool activate);
	void  setUpCoordStatusBar(rbool activate);
	rbool canCopy() const;

	DECLARE_ActionActivatorWidget;

private slots:
	void onVertScrollBarValueChanged(int value);
	void onHorzScrollBarValueChanged(int value);
	void onEditCopy                 ();
	void onSearchFind               ();
	void onSearchFindNext           ();
	void onSearchFindPrevious       ();
	void onHelpContext              ();
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_CTRLS_LOG_VIEW_H_
