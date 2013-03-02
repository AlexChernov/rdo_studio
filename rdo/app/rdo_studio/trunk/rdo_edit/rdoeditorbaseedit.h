/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorbaseedit.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDIT_H_
#define _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDIT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdobaseedit.h"
#include "app/rdo_studio/rdo_edit/rdoeditoreditstyle.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class RDOEditorBaseEdit: public EditBase
{
public:
	RDOEditorBaseEdit(PTR(QWidget) pParent);
	virtual ~RDOEditorBaseEdit();

	void setEditorStyle(PTR(RDOEditorBaseEditStyle) pStyle);

	void replaceCurrent(CREF(tstring) str, int changePosValue = -1) const;

protected:
	tstring kw0;
	tstring kw1;
	tstring kw2;
	tstring kw3;
	tstring getAllKW() const;

	static tstring convertToLexer(CREF(tstring) kw);

private:
	typedef  EditBase  super;
};

}}} // namespace rdo::gui::editor

#endif // _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDIT_H_
