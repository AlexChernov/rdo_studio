/**
 @file    rdo_resource_i.h
 @authors ����� �������
 @date    05.06.2011
 @brief   ��������� ��� ����� ��������
 @indent  4T
 @details ���� �������� ������������ �� ���� �������, ������� ��������� ����� createRes ��� �������� ��������
 */

#ifndef RDO_RESOURCE_I_H
#define RDO_RESOURCE_I_H

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
//#include "rdo_lib/rdo_runtime/rdo_resource.h"
//#include "rdo_lib/rdo_runtime/rdo_runtime.h"
//#include "rdo_common/rdointerface.h"
// ===============================================================================

PREDECLARE_POINTER(RDORuntime);
PREDECLARE_POINTER(RDOResource);

class IResourceType
{
public:
	virtual PTR(RDOResource) createRes(PTR(RDORuntime) rt, ruint id, bool trace) const = 0;
};
#define DECLARE_IResourceType \
	virtual PTR(RDOResource) createRes(PTR(RDORuntime) rt, ruint id, bool trace) const;

#endif // RDO_RESOURCE_I_H
