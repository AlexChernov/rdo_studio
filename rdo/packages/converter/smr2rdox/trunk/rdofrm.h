/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofrm.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDOFRM_H_
#define _CONVERTOR_RDOFRM_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdoframe.h"
#include "converter/smr2rdox/rdo_object.h"
#include "converter/smr2rdox/rdofun.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_NAMESPACE

int  cnv_frmparse(PTR(void) lexer);
int  cnv_frmlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void cnv_frmerror(PTR(char) message);

// --------------------------------------------------------------------------------
// -------------------- RDOFRMFrame
// --------------------------------------------------------------------------------
OBJECT(RDOFRMFrame) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOFRMFrame);
public:
	CREF(tstring)                   name () const  { return src_info().src_text(); }
	CREF(rdoRuntime::LPRDOFRMFrame) frame() const  { return m_pFrame;              }

private:
	RDOFRMFrame(CREF(RDOParserSrcInfo) src_info, LPRDOFUNLogic pLogic = NULL);

	rdoRuntime::LPRDOFRMFrame m_pFrame;
};

CLOSE_RDO_CONVERTER_NAMESPACE

#endif // _CONVERTOR_RDOFRM_H_
