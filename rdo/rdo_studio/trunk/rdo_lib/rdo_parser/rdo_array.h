/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdo_array.h
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_VALUE_ARRAY_H_
#define _RDOPARSER_VALUE_ARRAY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/type/array.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOArrayValue
// ----------------------------------------------------------------------------
OBJECT(RDOArrayValue) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOArrayValue);
public:
	typedef std::vector<RDOValue> Container;

	void insertItem(CREF(RDOValue) value);

	CREF(LPRDOArrayType) getArrayType() const;
	 REF(LPRDOArrayType) getArrayType();
	rdoRuntime::RDOValue getRArray   () const;
	tstring              getAsString () const;
	CREF(Container)      getContainer() const;

private:
	RDOArrayValue(CREF(LPRDOArrayType) pArrayType);
	virtual ~RDOArrayValue();

	Container      m_container;
	LPRDOArrayType m_pArrayType;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_VALUE_ARRAY_H_
