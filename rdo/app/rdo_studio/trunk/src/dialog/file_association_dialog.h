/*!
  \copyright (c) RDO-Team, 2013
  \file      file_association_dialog.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      10.02.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_FILE_ASSOCIATION_DIALOG_H_
#define _RDO_STUDIO_FILE_ASSOCIATION_DIALOG_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QDialog>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/projects/common/bin/rdo_studio/generated/ui_file_association_dialog.h"
// --------------------------------------------------------------------------------

class FileAssociationDialog
	: public QDialog
	, public Ui::FileAssociationDialog
{
Q_OBJECT

public:
	FileAssociationDialog(PTR(QWidget) pParent);
	virtual ~FileAssociationDialog();
};

#endif // _RDO_STUDIO_FILE_ASSOCIATION_DIALOG_H_
