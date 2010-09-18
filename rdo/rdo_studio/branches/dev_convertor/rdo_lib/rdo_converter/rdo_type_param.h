/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_type_param.h
 * author   : ������ ������
 * date     : 12.02.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_RDOCONVERTER_TYPE_PARAM_H_
#define _CONVERTOR_RDOCONVERTER_TYPE_PARAM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdo_type.h"
#include "rdo_lib/rdo_converter/rdo_value.h"
#include "rdo_lib/rdo_converter/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_model_interface.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeParam
// ----------------------------------------------------------------------------
OBJECT(RDOTypeParam)
	IS INSTANCE_OF       (RDOParserSrcInfo)
	AND IMPLEMENTATION_OF(IModelStructure )
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

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_RDOCONVERTER_TYPE_PARAM_H_
