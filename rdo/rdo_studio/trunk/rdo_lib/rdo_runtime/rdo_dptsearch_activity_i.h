/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_dptsearch_activity_i.h
 * @author    ������ ������
 * @date      08.08.2009
 * @brief     ��������� ���������� ����� �������� ������� DPTSearch
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_DPTSEARCH_ACTIVITY_I_H_
#define _LIB_RUNTIME_DPTSEARCH_ACTIVITY_I_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdointerface.h"
#include "rdo_lib/rdo_runtime/rdo_rule_i.h"
#include "rdo_lib/rdo_runtime/rdo_runtime_interface_registrator.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

class RDOSimulator;

CLOSE_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @interface IDPTSearchActivity
 * @brief     ��������� ���������� ����� �������� ������� DPTSearch
 *********************************************************************************/
class IDPTSearchActivity
{
public:
	/**
	 * @enum  ValueTime
	 * @brief ����� ���������� ������� ��������� ����
	 * @var   vt_before �� ���������� ����������
	 * @var   vt_after  ����� ���������� ����������
	 */
	enum ValueTime
	{
		vt_before,
		vt_after
	};

	virtual REF(LPIRule) rule     ()                                        = 0;
	virtual double       cost     (CREF(rdoRuntime::LPRDORuntime) pRuntime) = 0;
	virtual ValueTime    valueTime() const                                  = 0;
};

/******************************************************************************//**
 * @def       DECLARE_IDPTSearchActivity
 * @brief     ���������� ���������� ���������� ����� �������� ������� DPTSearch
 *********************************************************************************/
#define DECLARE_IDPTSearchActivity                                           \
	virtual REF(LPIRule) rule     ();                                        \
	virtual double       cost     (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual ValueTime    valueTime() const;

#endif // _LIB_RUNTIME_DPTSEARCH_ACTIVITY_I_H_
