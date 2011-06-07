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
#include "rdo_common/rdointerface.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE
PREDECLARE_POINTER(RDORuntime);
PREDECLARE_POINTER(RDOResource);
CLOSE_RDO_RUNTIME_NAMESPACE

class IResourceType
{
public:
	virtual rdoRuntime::LPRDOResource createRes(PTR(rdoRuntime::RDORuntime) rt, ruint id, bool trace) const = 0;
};
#define DECLARE_IResourceType \
	virtual rdoRuntime::LPRDOResource createRes(PTR(rdoRuntime::RDORuntime) rt, ruint id, bool trace) const;

#endif // RDO_RESOURCE_I_H
