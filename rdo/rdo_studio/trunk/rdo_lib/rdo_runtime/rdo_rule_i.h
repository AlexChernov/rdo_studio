/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_rule_i.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      01.08.2009
  \brief     ��������� ������������� ������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RULE_I_H_
#define _LIB_RUNTIME_RULE_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdointerface.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime);

CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \interface IRule
  \brief     ��������� �������������� �������
*/
class IRule
{
public:
	virtual void  onBeforeChoiceFrom(CREF(rdoRuntime::LPRDORuntime) pRuntime)                 = 0;
	virtual rbool choiceFrom        (CREF(rdoRuntime::LPRDORuntime) pRuntime)                 = 0;
	virtual void  onBeforeRule      (CREF(rdoRuntime::LPRDORuntime) pRuntime)                 = 0;
	virtual void  convertRule       (CREF(rdoRuntime::LPRDORuntime) pRuntime)                 = 0;
	virtual void  onAfterRule       (CREF(rdoRuntime::LPRDORuntime) pRuntime, rbool inSearch) = 0;
};

/*!
  \def       DECLARE_IRule
  \brief     ���������� ���������� �������������� �������
*/
#define DECLARE_IRule \
	virtual void  onBeforeChoiceFrom(CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual rbool choiceFrom        (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual void  onBeforeRule      (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual void  convertRule       (CREF(rdoRuntime::LPRDORuntime) pRuntime); \
	virtual void  onAfterRule       (CREF(rdoRuntime::LPRDORuntime) pRuntime, rbool inSearch);

#endif // _LIB_RUNTIME_RULE_I_H_
