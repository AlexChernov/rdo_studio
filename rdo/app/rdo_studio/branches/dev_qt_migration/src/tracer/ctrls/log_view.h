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
	void  selectLine(rsint index);

	rbool getFocusOnly() const;
	void  setFocusOnly(rbool value);

	CREF(LogStyle) getStyle() const;
	void setStyle(LogStyle* style, rbool needRedraw = true);

	rbool getDrawLog() const;
	void  setDrawLog(rbool value);

private:
	typedef  ActionActivatorWidget  parent_type;

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

		const_iterator         findString (rsint index) const;
		const_reverse_iterator rFindString(rsint index) const;

	private:
		List                m_list;
		rsint               m_count;
		List::iterator      m_cursorIt;
		rsint               m_cursor;
		tstring::size_type  m_maxLegth;

		void seek(rsint delta, REF(StringList::const_iterator) it) const;
	};

	struct SubitemColors
	{
		typedef std::map<rsint, PTR(LogColorPair)> List;

		List              m_colorList;
		rbool             m_addingSubitems;
		PTR(LogColorPair) m_parentColor;

		SubitemColors();
		SubitemColors(CREF(SubitemColors) subitemColors);
	};

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

	QAbstractScrollArea* m_pScrollArea;
	StringList           m_strings;
	LogStyle*            m_logStyle;
	rsint                m_lineHeight;
	rsint                m_charWidth;
	rsint                m_selectedLine;
	QMenu*               m_pPopupMenu;
	ScrollMetric         m_SM_X;
	ScrollMetricVert     m_SM_Y;
	SubitemColors        m_subitemColors;
	QRect                m_clientRect;
	QRect                m_prevWindowRect;
	rsint                m_fullRepaintLines;
	rbool                m_focusOnly;
	FindDialog*          m_pFindDialog;
	FindDialog::Settings m_findSettings;
	rbool                m_drawLog;
	QFont                m_font;

	rbool getItemColors(CREF(tstring) item, LogColorPair* &colors) const;
	rbool getItemColors(rsint index, LogColorPair* &colors) const;

	rsint selectedLine() const;
	void  setSelectedLine(rsint selectedLine);

	tstring     getString           (rsint index) const;
	tstring     getSelected         () const;

	QScrollBar& getVertScrollBar    ();
	QScrollBar& getHorzScrollBar    ();
	void        updateScrollBars    ();
	rbool       scrollVertically    (rsint pos);
	rbool       scrollHorizontally  (rsint pos);

	rbool       makeLineVisible     (rsint index);
	rbool       isFullyVisible      (rsint index) const;
	QRect       getLineRect         (rsint index) const;
	void        repaintLine         (rsint index);

	void        setFont             ();
	void        updateActionFind    (rbool activated);
	void        updateActionEditCopy(rbool activated);
	void        updateCoordStatusBar(rbool activated);
	rbool       canCopy             () const;

	rsint       find                (rbool searchDown);
	void        onFindDlgFind       (CREF(FindDialog::Settings) settings);
	void        onFindDlgClose      ();

	virtual void resizeEvent    (QResizeEvent* pEvent);
	virtual void paintEvent     (QPaintEvent*  pEvent);
	virtual void keyPressEvent  (QKeyEvent*    pEvent);
	virtual void wheelEvent     (QWheelEvent*  pEvent);
	virtual void mousePressEvent(QMouseEvent*  pEvent);

	virtual void onUpdateActions(rbool activated);

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
