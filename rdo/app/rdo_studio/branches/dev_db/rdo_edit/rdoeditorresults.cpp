/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorresults.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/rdo_edit/rdoeditorresults.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_frm.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditor;

// --------------------------------------------------------------------------------
// -------------------- RDOEditorResults
// ---------------------------------------------------------------------------
RDOEditorResults::RDOEditorResults(PTR(QWidget) pParent)
	: super(pParent)
	, EditWithReadOnlyPopupMenu(pParent)
{
	kw0 = "TRUE FALSE";
	kw1 = "";
	kw2 = "";
	kw3 = "";

	setReadOnly( true );
}

RDOEditorResults::~RDOEditorResults()
{}

void RDOEditorResults::setEditorStyle(PTR(RDOEditorResultsStyle) pStyle)
{
	super::setEditorStyle(pStyle);
}

void RDOEditorResults::onHelpContext()
{
	tstring keyword = getCurrentOrSelectedWord();
	tstring s = getAllKW();

	if (s.find_first_of(keyword) == tstring::npos || keyword.empty())
	{
		keyword = _T("pmv");
	}

	QByteArray ba;
	ba.append("activateKeyword ");
	ba.append(keyword.c_str());
	ba.append("\n");
	studioApp.callQtAssistant(ba);
}

void RDOEditorResults::mousePressEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::LeftButton)
	{
		super::mousePressEvent(pEvent);
	}
	else if (pEvent->button() == Qt::RightButton)
	{
		m_pPopupMenu->exec(pEvent->globalPos());
	}
}
