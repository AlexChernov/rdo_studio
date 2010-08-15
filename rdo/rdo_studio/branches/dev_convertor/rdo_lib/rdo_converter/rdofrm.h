/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdofrm.h
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOFRM_H_
#define _RDOFRM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdoframe.h"
#include "rdo_lib/rdo_converter/rdo_object.h"
#include "rdo_lib/rdo_converter/rdofun.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

int  frmparse(PTR(void) lexer);
int  frmlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void frmerror(PTR(char) mes);

// ----------------------------------------------------------------------------
// ---------- RDOFRMFrame
// ----------------------------------------------------------------------------
OBJECT(RDOFRMFrame) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFRMFrame);
public:
	CREF(tstring)                name () const  { return src_info().src_text(); }
	PTR(rdoRuntime::RDOFRMFrame) frame()        { return m_pFrame;              }

private:
	RDOFRMFrame(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic = NULL);

	PTR(rdoRuntime::RDOFRMFrame) m_pFrame;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _RDOFRM_H_
