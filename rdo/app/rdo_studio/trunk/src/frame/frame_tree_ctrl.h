/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      frame_tree_ctrl.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      28.03.2003
  \brief     ������ ������ ��������
  \indent    4T
*/

#ifndef _RDO_STUDIO_FRAME_TREE_CTRL_H_
#define _RDO_STUDIO_FRAME_TREE_CTRL_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QTreeWidget>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/action_activator/action_activator.h"
// --------------------------------------------------------------------------------

class FrameTreeCtrl
	: public QTreeWidget
	, public ActionActivator
{
Q_OBJECT

public:
	FrameTreeCtrl(PTR(QWidget) pParent);
	virtual ~FrameTreeCtrl();

	PTR(QTreeWidgetItem) insertFrame(CREF(QString) name);

	void clear();

private:
	typedef  QTreeWidget  parent_type;

	PTR(QTreeWidgetItem) m_pRootItem;

	virtual void focusInEvent   (QFocusEvent* pEvent);
	virtual void focusOutEvent  (QFocusEvent* pEvent);
	virtual void onUpdateActions(rbool activated);

private slots:
	void onHelpContext();
};

#endif // _RDO_STUDIO_FRAME_TREE_CTRL_H_
