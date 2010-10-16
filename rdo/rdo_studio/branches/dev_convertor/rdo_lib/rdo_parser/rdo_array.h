/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_array.h
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_ARRAY_H_
#define _RDOPARSER_ARRAY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_array.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayType
// ----------------------------------------------------------------------------
class RDOArrayType: public RDOType
{
DECLARE_FACTORY(RDOArrayType);
public:
	rdoRuntime::LPRDOArrayType getArray() const
	{
		return m_pType.object_static_cast<rdoRuntime::RDOArrayType>();
	}

	DECLARE_IType;
	DECLARE_IModelStructure;

private:
	RDOArrayType         ();
	virtual ~RDOArrayType();
};
DECLARE_POINTER(RDOArrayType);

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_ARRAY_H_