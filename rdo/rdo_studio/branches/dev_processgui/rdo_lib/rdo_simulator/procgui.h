/*
 * copyright: (c) RDO-Team, 2011
 * filename : procgui.h
 * author   : ������� ������
 * date     : 22.04.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _PROCGUI_H_
#define _PROCGUI_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
// ===============================================================================

// --------------------------------------------------------------------
// ---------- ProcGUIBlock
// --------------------------------------------------------------------
class ProcGUIBlock
{
public:
	ProcGUIBlock(PTR(rdoParse::RDOParser) pParser, PTR(rdoRuntime::RDORuntime) pRuntime);
	virtual ~ProcGUIBlock();

	void Create();

private:
	PTR(rdoParse::RDOParser)     m_pParser;
	PTR(rdoRuntime::RDORuntime)  m_pRuntime;
	LPILogic                     l_runtime;
	LPIPROCBlock                 b_runtime;
};

#endif //! _PROCGUI_H_
