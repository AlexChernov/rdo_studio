/*
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_priority_i.h
 * @authors   ������ ������
 * @date      01.08.2009
 * @brief     ��������� IPriority
 * @indent    4T
 *********************************************************************************/

#ifndef _RDO_PRIORITY_I_H_
#define _RDO_PRIORITY_I_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/rdointerface.h"
// ********************************************************************************

class IPriority
{
public:
	virtual rdoRuntime::LPRDOCalc getPrior()                                   = 0;
	virtual rbool                 setPrior(CREF(rdoRuntime::LPRDOCalc) pPrior) = 0;
};
#define DECLARE_IPriority \
	virtual rdoRuntime::LPRDOCalc getPrior(); \
	virtual rbool                 setPrior(CREF(rdoRuntime::LPRDOCalc) pPrior);

#endif // _RDO_PRIORITY_I_H_
