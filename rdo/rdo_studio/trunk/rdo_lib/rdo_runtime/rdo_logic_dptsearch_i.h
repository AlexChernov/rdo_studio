/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_logic_dptsearch_i.h
 * @authors   ������ ������
 * @date      08.08.2009
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_LOGIC_DPTSEARCH_I_
#define _LIB_RUNTIME_LOGIC_DPTSEARCH_I_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdointerface.h"
// ********************************************************************************

/******************************************************************************//**
 * @interface IDPTSearchLogic
 * @brief     unknown
 *********************************************************************************/
class IDPTSearchLogic
{
public:
	virtual void addActivity(LPIDPTSearchActivity activity) = 0;
};

/******************************************************************************//**
 * @def     DECLARE_IDPTSearchLogic
 * @brief   unknown
 *********************************************************************************/
#define DECLARE_IDPTSearchLogic \
	virtual void addActivity(LPIDPTSearchActivity activity);

#endif // _LIB_RUNTIME_LOGIC_DPTSEARCH_I_
