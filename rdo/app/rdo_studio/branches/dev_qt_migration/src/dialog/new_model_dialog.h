/*!
  \copyright (c) RDO-Team, 2013
  \file      new_model_dialog.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      04.01.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_NEW_MODEL_DIALOG_H_
#define _RDO_STUDIO_NEW_MODEL_DIALOG_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qdialog.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/projects/common/bin/rdo_studio/generated/ui_new_model_dialog.h"
// --------------------------------------------------------------------------------

class NewModelDialog
	: public  QDialog
	, public  Ui::NewModelDialog
{
Q_OBJECT

public:
	NewModelDialog(PTR(QWidget) pParent);
	virtual ~NewModelDialog();

private:
	QFont    m_font;
	QFont    m_fontRed;
	QPalette m_palette;
	QPalette m_paletteRed;

	void setPath(CREF(QString) path);

private slots:
	void updateUI();
	void onPathButtonClicked();
};

#endif // _RDO_STUDIO_NEW_MODEL_DIALOG_H_
