/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      dock_build.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      30.09.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qaction.h>
#include <QtWidgets/qmessagebox.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dock/dock_build.h"
#include "app/rdo_studio/src/application.h"
#include "simulator/report/file_message.h"
#include "simulator/report/build_edit_line_info.h"
// --------------------------------------------------------------------------------

DockBuild::DockBuild(PTR(QWidget) pParent)
	: DockFocusable(QString::fromStdWString(L"����������"), pParent)
{
	PTR(context_type) pWidget = new context_type(this);
	pWidget->setMinimumSize(QSize(300, 150));

	setWidget(pWidget);

	toggleViewAction()->setIcon(QIcon(QString::fromUtf8(":/images/images/dock_build.png")));
}

DockBuild::~DockBuild()
{}

void DockBuild::appendString(CREF(QString) str)
{
	PTR(rdo::simulation::report::BuildEditLineInfo) pLine = new rdo::simulation::report::BuildEditLineInfo(str.toLocal8Bit().constData());
	getContext().appendLine(pLine);
}

void DockBuild::appendString(CREF(rdo::simulation::report::FileMessage) message)
{
	QString qMessage = QString::fromLocal8Bit(message.getText().c_str());
	if (qMessage.contains(QString::fromStdWString(L"��������� ������������ �����������")))
	{
		QMessageBox::critical(studioApp.getMainWnd(), QString::fromStdWString(L"������������ �����������"), qMessage);
		return;
	}

	if (message.getType() == rdo::simulation::report::FileMessage::MT_ERROR || (message.getType() == rdo::simulation::report::FileMessage::MT_WARNING && static_cast<PTR(rdoEditCtrl::RDOBuildEditTheme)>(studioApp.getStyle()->style_build.theme)->warning))
	{
		PTR(rdo::simulation::report::BuildEditLineInfo) pLine = new rdo::simulation::report::BuildEditLineInfo(message);
		getContext().appendLine(pLine);
	}
}

void DockBuild::clear()
{
	getContext().clearAll();
}

REF(DockBuild::context_type) DockBuild::getContext()
{
	return *static_cast<PTR(context_type)>(widget());
}