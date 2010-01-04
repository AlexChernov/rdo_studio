/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_dptsearch_interface.h
 * author   : ������ ������
 * date     : 08.08.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_DPTSEARCH_INTERFACE_H_
#define _RDO_LOGIC_DPTSEARCH_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdointerface.h"
// ===============================================================================

class IDPTSearchLogic
{
public:
	virtual void addActivity(LPIDPTSearchActivity activity) = 0;
};
#define DECLARE_IDPTSearchLogic \
	virtual void addActivity(LPIDPTSearchActivity activity);

#endif //! _RDO_LOGIC_DPTSEARCH_INTERFACE_H_
