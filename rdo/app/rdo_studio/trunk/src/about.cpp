/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/about.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      18.08.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/about.h"
// --------------------------------------------------------------------------------

About::About(PTR(QWidget) pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
	, m_ui   (new Ui::AboutDialog)
{
	m_ui->setupUi(this);

	m_ui->version->setText(QString("RAO-studio   %1%2-version 1.1 (build %3)")

#ifdef RDO_MT
		.arg("mt")
#else
		.arg("st")
#endif

#ifdef RDOSIM_COMPATIBLE
		.arg("-comp")
#else
		.arg("")
#endif

		.arg(1234)
	);

	if (pParent)
	{
		move(pParent->frameGeometry().center() - frameGeometry().center());
	}
}
