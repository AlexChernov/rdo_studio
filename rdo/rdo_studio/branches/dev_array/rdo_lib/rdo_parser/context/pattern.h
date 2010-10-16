/*
 * copyright: (c) RDO-Team, 2010
 * filename : pattern.h
 * author   : ������ ������
 * date     : 16.10.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONEXT_PATTERN_H_
#define _CONEXT_PATTERN_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/context/context.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- ContextPattern
// ----------------------------------------------------------------------------
class ContextPattern: public Context
{
DECLARE_FACTORY(ContextPattern);
private:
	ContextPattern();

	LPLocalVariableList m_pLocalVariableList;

	DECLARE_IContext;
};
DECLARE_POINTER(ContextPattern);

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _CONEXT_PATTERN_H_
