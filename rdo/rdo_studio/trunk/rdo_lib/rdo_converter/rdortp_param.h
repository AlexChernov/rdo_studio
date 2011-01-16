/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdortp_param.h
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_RDORTP_PARAM_H_
#define _CONVERTOR_RDORTP_PARAM_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdo_type_param.h"
#include "rdo_lib/rdo_runtime/rdo_model_interface.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORTPParam
// ----------------------------------------------------------------------------
PREDECLARE_POINTER(RDORTPResType);

OBJECT(RDORTPParam)
	IS INSTANCE_OF       (RDOParserSrcInfo)
	AND IMPLEMENTATION_OF(IModelStructure )
	AND IMPLEMENTATION_OF(IName           )
{
DECLARE_FACTORY(RDORTPParam);
public:
	LPRDORTPResType getResType  () const { ASSERT(m_pResourceType); return m_pResourceType; }
	LPRDOTypeParam  getParamType() const { ASSERT(m_paramType);     return m_paramType;     }
	CREF(RDOValue)  getDefault  () const { return getParamType()->default();                }

	DECLARE_IModelStructure;
	DECLARE_IName;

protected:
	LPRDORTPResType m_pResourceType;
	LPRDOTypeParam  m_paramType;

private:
	RDORTPParam(CREF(LPRDORTPResType) pResourceType, CREF(LPRDOTypeParam) pParamType, CREF(RDOParserSrcInfo) src_info);
	virtual ~RDORTPParam();
};
DECLARE_POINTER(RDORTPParam);

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_RDORTP_PARAM_H_
