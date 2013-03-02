/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdofindedit.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDIT_CTRLS_RDOFINDEDIT_H_
#define _RDO_STUDIO_EDIT_CTRLS_RDOFINDEDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdologedit.h"
#include "app/rdo_studio/edit_ctrls/rdofindeditstyle.h"
#include "app/rdo_studio/edit_ctrls/editwithreadonlypopupmenu.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

// --------------------------------------------------------------------------------
// -------------------- RDOFindEdit
// --------------------------------------------------------------------------------
class RDOFindEdit
	: public LogEdit
	, public EditWithReadOnlyPopupMenu
{
public:
	RDOFindEdit(PTR(QWidget) pParent);
	virtual ~RDOFindEdit();

	void setEditorStyle(RDOFindEditStyle* pStyle);

	void setKeyword(CREF(QString) keyword, const rbool matchCase = false) const;

private:
	typedef  LogEdit  super;

	virtual void mousePressEvent(QMouseEvent* pEvent);
	virtual void onHelpContext();
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_EDIT_CTRLS_RDOFINDEDIT_H_
