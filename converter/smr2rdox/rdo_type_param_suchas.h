#ifndef _CONVERTOR_RDO_TYPE_PARAM_SUCHAS_H_
#define _CONVERTOR_RDO_TYPE_PARAM_SUCHAS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_type_param.h"
#include "converter/smr2rdox/param.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeParamSuchAs
// --------------------------------------------------------------------------------
class RDOTypeParamSuchAs: public RDOTypeParam
{
DECLARE_FACTORY(RDOTypeParamSuchAs);
public:
	const LPRDOParam& getParam() const;

private:
	RDOTypeParamSuchAs(const LPRDOParam& pParam);
	virtual ~RDOTypeParamSuchAs();

	LPRDOParam m_pParam;
};
DECLARE_POINTER(RDOTypeParamSuchAs);

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDO_TYPE_PARAM_SUCHAS_H_
