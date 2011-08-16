/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopokaz_group_i.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      04.03.2011
  \brief     ��������� ������ ���������� �����������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_POKAZ_GROUP_I_H_
#define _LIB_RUNTIME_POKAZ_GROUP_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/rdointerface.h"
#include "rdo_lib/rdo_runtime/rdo_simulator.h"
#include "rdo_lib/rdo_runtime/rdopokaz_i.h"
// --------------------------------------------------------------------------------

/*!
  \interface IPokazGroup
  \brief     ��������� ������ ���������� �����������
*/
class IPokazGroup
{
public:
	virtual void onStart (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void onStop  (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual void onAppend(CREF(LPIPokaz)                 pResult ) = 0;
};

/*!
  \def     DECLARE_IPokazGroup
  \brief   ������ ��� ���������� ������� ������ ���������� �����������
*/
#define DECLARE_IPokazGroup                                 \
	void onStart (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	void onStop  (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	void onAppend(CREF(LPIPokaz)                 pResult );

#endif // _LIB_RUNTIME_POKAZ_GROUP_I_H_
