/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdo_type_param_suchas.h
 * author   : ������ ������
 * date     : 09.01.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _CONVERTOR_RDO_TYPE_PARAM_SUCHAS_H_
#define _CONVERTOR_RDO_TYPE_PARAM_SUCHAS_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdo_type_param.h"
#include "rdo_lib/rdo_converter/param.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOTypeParamSuchAs
// ----------------------------------------------------------------------------
CLASS(RDOTypeParamSuchAs): INSTANCE_OF(RDOTypeParam)
{
DECLARE_FACTORY(RDOTypeParamSuchAs);
public:
	CREF(LPRDOParam) getParam() const;

private:
	RDOTypeParamSuchAs(CREF(LPRDOParam) pParam);
	virtual ~RDOTypeParamSuchAs();

	LPRDOParam m_pParam;
};
DECLARE_POINTER(RDOTypeParamSuchAs);

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _CONVERTOR_RDO_TYPE_PARAM_SUCHAS_H_
