/**
 @file    rdo_res_type_i.h
 @authors ����� �������
 @date    05.06.2011
 @brief   ��������� ��� ����� ��������
 @indent  4T
 @details ���� �������� ������������ �� ���� �������, ������� ��������� ����� createRes ��� �������� ��������
 */

#ifndef RDO_RES_TYPE_I_H
#define RDO_RES_TYPE_I_H

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/interface_ptr.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime);
PREDECLARE_POINTER(RDOResource);

OBJECT_INTERFACE(IResourceType)
{
DECLARE_FACTORY(IResourceType);
public:
	virtual rdoRuntime::LPRDOResource createRes(PTR(rdoRuntime::RDORuntime) runtime, CREF(std::vector<RDOValue>) paramsCalcs, rbool trace, rbool permanentFlag) = 0;

protected:
	IResourceType()          {}
	virtual ~IResourceType() {}
};
#define DECLARE_IResourceType \
	rdoRuntime::LPRDOResource createRes(PTR(rdoRuntime::RDORuntime) runtime, CREF(std::vector<RDOValue>) paramsCalcs, rbool trace, rbool permanentFlag);

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // RDO_RES_TYPE_I_H
