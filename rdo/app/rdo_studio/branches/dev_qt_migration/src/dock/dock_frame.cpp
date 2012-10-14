/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_frame.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      07.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qaction.h>
#include <QtGui/qboxlayout.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock/dock_frame.h"
// --------------------------------------------------------------------------------

DockFrame::DockFrame(PTR(QWidget) pParent)
	: QDockWidget("��������", pParent)
{
	PTR(context_type) pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(150, 300));

	PTR(QVBoxLayout) pLayout = new QVBoxLayout(this);
	pLayout->setSpacing(0);
	pLayout->setContentsMargins(0, 0, 0, 0);
	pLayout->addWidget(pWidget);

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_frame.png")));
}

DockFrame::~DockFrame()
{}

REF(DockFrame::context_type) DockFrame::getContext()
{
	return *static_cast<PTR(context_type)>(widget());
}
