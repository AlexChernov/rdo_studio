/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_trace.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      04.10.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QAction>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_trace.h"
#include "app/rdo_studio/src/tracer/tracer.h"
// --------------------------------------------------------------------------------

DockTrace::DockTrace(PTR(QWidget) pParent)
	: DockFocusable(QString::fromStdWString(L"�����������"), pParent)
{
	PTR(context_type) pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(300, 150));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_trace.png")));

	g_pTracer->setLog(&getContext());
}

DockTrace::~DockTrace()
{}

REF(DockTrace::context_type) DockTrace::getContext()
{
	return *static_cast<PTR(context_type)>(widget());
}
