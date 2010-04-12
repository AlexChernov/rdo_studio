/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_param.h
 * author   : ������ ������
 * date     : 12.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_TYPE_PARAM_H_
#define _RDOPARSER_TYPE_PARAM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdo_type.h"
#include "rdo_lib/rdo_parser/rdo_value.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_model_interface.h"
#include "rdo_common/rdosmart_ptr.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeParam
// ----------------------------------------------------------------------------
class RDOTypeParam: public RDOParserSrcInfo, public IModelStructure, public rdo::smart_ptr_counter_reference
{
DECLARE_FACTORY(RDOTypeParam);
public:
	LPRDOType type      ()                     const;
	RDOValue  value_cast(CREF(RDOValue) value) const;

	CREF(RDOValue) default() const;

	DECLARE_IModelStructure;

private:
	RDOTypeParam(CREF(LPRDOType) type, CREF(RDOValue) default, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOTypeParam();

	LPRDOType m_type;
	RDOValue  m_default;
};
DECLARE_POINTER(RDOTypeParam);

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_PARAM_H_
