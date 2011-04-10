/*
 * copyright: (c) RDO-Team, 2011
 * filename : type_param_suchas.h
 * author   : ������ ������
 * date     : 09.01.2011
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_TYPE_PARAM_SUCHAS_H_
#define _RDOPARSER_TYPE_PARAM_SUCHAS_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/type/type_param.h"
#include "rdo_lib/rdo_parser/param.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

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

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_TYPE_PARAM_SUCHAS_H_
