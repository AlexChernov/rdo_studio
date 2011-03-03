/*
 * copyright: (c) RDO-Team, 2010
 * filename : context.h
 * author   : ������ ������
 * date     : 06.06.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_CONTEXT_H_
#define _RDOPARSER_CONTEXT_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/rdomacros.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"

#include "rdo_lib/rdo_parser/namespace.h"
#include "rdo_lib/rdo_parser/context/stack.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Context
// ----------------------------------------------------------------------------
OBJECT(Context)
{
DECLARE_FACTORY(Context);
friend void ContextStack::push(LPContext pContext);

public:
	template <class T>
	rdo::intrusive_ptr<T> cast();

	LPContext             find  (CREF(tstring) name) const;
	rdoRuntime::LPRDOCalc create(CREF(tstring) name);

protected:
	Context();
	virtual ~Context();

private:
	LPContextStack m_pContextStack;

	void setContextStack(CREF(LPContextStack) pContextStack);
};

CLOSE_RDO_PARSER_NAMESPACE

#include "rdo_lib/rdo_parser/context/context.inl"

#endif //! _RDOPARSER_CONTEXT_H_
