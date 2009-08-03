/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_logic_interface.h
 * author   : ������ ������
 * date     : 30.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_LOGIC_INTERFACE_H_
#define _RDO_LOGIC_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <namespace.h>
#include <rdointerface.h>
#include "rdo_runtime_interface_registrator.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE
class RDOCalc;
CLOSE_RDO_RUNTIME_NAMESPACE

class ILogic
{
public:
	virtual void setCondition   (PTR (rdoRuntime::RDOCalc) calc ) = 0;
	virtual void appendOperation(CREF(LPIBaseOperation)    opr  ) = 0;
	virtual void appendLogic    (CREF(LPIBaseOperation)    logic) = 0;
};
#define DECLARE_ILogic \
	virtual void setCondition   (PTR (rdoRuntime::RDOCalc) calc ); \
	virtual void appendOperation(CREF(LPIBaseOperation)    opr  ); \
	virtual void appendLogic    (CREF(LPIBaseOperation)    logic);

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
};
#define DECLARE_IBaseOperationContainer                    \
	virtual Iterator              begin ();                \
	virtual Iterator              end   ();                \
	virtual CIterator             begin () const;          \
	virtual CIterator             end   () const;          \
	virtual void                  append(CREF(Item) item); \
	virtual rbool                 empty () const;          \
	virtual REF(LPIBaseOperation) back  ();

#endif //! _RDO_LOGIC_INTERFACE_H_
