/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofrm.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOFRM_H_
#define _RDOFRM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdoframe.h"
#include "simulator/compiler/parser/rdo_object.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/context/context.h"
#include "simulator/compiler/parser/context/context_find_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

int  frmparse(PTR(void) lexer);
int  frmlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void frmerror(PTR(char) mes);

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
CLASS(RDOFRMFrame):
	    INSTANCE_OF      (RDOParserSrcInfo)
	AND INSTANCE_OF      (Context         )
	AND IMPLEMENTATION_OF(IContextFind    )
{
DECLARE_FACTORY(RDOFRMFrame);
public:
	CREF(tstring)                   name () const  { return src_info().src_text(); }
	CREF(rdoRuntime::LPRDOFRMFrame) frame() const  { return m_pFrame;              }
	void                            end  ();

private:
	RDOFRMFrame(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic = NULL);

	rdoRuntime::LPRDOFRMFrame m_pFrame;

	DECLARE_IContextFind;
};
DECLARE_POINTER(RDOFRMFrame);

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOFRM_H_
