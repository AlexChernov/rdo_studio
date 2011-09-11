/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_logic_i.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      30.07.2009
  \brief     ��������� ILogic
  \indent    4T
*/

#ifndef _LIB_RUNTIME_LOGIC_I_H_
#define _LIB_RUNTIME_LOGIC_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdointerface.h"
#include "rdo_lib/rdo_runtime/rdo_runtime_interface_registrator.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE
class RDOSimulator;
CLOSE_RDO_RUNTIME_NAMESPACE

/*!
  \interface ILogic
  \brief     ������ � ��� - ������� ����������� ������ ��� ����������� IBaseOperationContainer
*/
class ILogic
{
public:
	virtual void init           (CREF(rdoRuntime::LPRDORuntime) pRuntime  ) = 0;
	virtual void setCondition   (CREF(rdoRuntime::LPRDOCalc)    pCondition) = 0;
};

#define DECLARE_ILogic                                                       \
	virtual void init           (CREF(rdoRuntime::LPRDORuntime) pRuntime  ); \
	virtual void setCondition   (CREF(rdoRuntime::LPRDOCalc)    pCondition);

/*!
  \interface IBaseOperationContainer
  \brief     ������������� ���������� �����������. ��. ������� "�����������"
*/
class IBaseOperationContainer
{
public:
	typedef  LPIBaseOperation      Item;
	typedef  std::vector<Item>     List;
	typedef  List::iterator        Iterator;
	typedef  List::const_iterator  CIterator;

	virtual Iterator              begin ()                = 0;
	virtual Iterator              end   ()                = 0;
	virtual CIterator             begin () const          = 0;
	virtual CIterator             end   () const          = 0;
	virtual void                  append(CREF(Item) item) = 0;
	virtual rbool                 empty () const          = 0;
	virtual REF(LPIBaseOperation) back  ()                = 0;
	virtual void                  clear ()                = 0;
};

#define DECLARE_IBaseOperationContainer                    \
	virtual Iterator              begin ();                \
	virtual Iterator              end   ();                \
	virtual CIterator             begin () const;          \
	virtual CIterator             end   () const;          \
	virtual void                  append(CREF(Item) item); \
	virtual rbool                 empty () const;          \
	virtual REF(LPIBaseOperation) back  ();                \
	virtual void                  clear ();

#endif // _LIB_RUNTIME_LOGIC_I_H_
