/*!
  \copyright (c) RDO-Team, 2012
  \file      queue_depart.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     ���������� ��������� QUEUE � DEPART
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PROCESS_QUEUE_DEPART_H_
#define _LIB_RUNTIME_PROCESS_QUEUE_DEPART_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess.h"
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \struct  runtime_for_Queue
  \brief   ��������������� ����� ��� ��������� QUEUE � Runtime
*/
struct runtime_for_Queue
{
	LPRDOResource rss; 
	int           Id_param;
	RDOValue      defaultValue;
};

/*!
  \struct  parser_for_Queue
  \brief   ��������������� ����� ��� ��������� QUEUE � Parser
*/
struct parser_for_Queue
{
	int Id_res;
	int Id_param;
};

/*!
  \class   RDOPROCBlockForQueue
  \brief   ��������������� ����� ��� ����������� ��������� QUEUE
*/
class RDOPROCBlockForQueue: public RDOPROCBlock
{
protected:
	RDOPROCBlockForQueue(LPIPROCProcess process, parser_for_Queue From_Par);

	parser_for_Queue  fromParser;
	runtime_for_Queue forRes;
	void _onStart(CREF(LPRDORuntime) pRuntime);
};

/*!
  \class   RDOPROCQueue
  \brief   ���������� ���� QUEUE
*/
class RDOPROCQueue: public RDOPROCBlockForQueue, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCQueue);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlockForQueue)
	QUERY_INTERFACE       (IBaseOperation      )
QUERY_INTERFACE_END

public:
	static ruint   getDefaultValue  ();
	static tstring getQueueParamName();

private:
	RDOPROCQueue(LPIPROCProcess process, parser_for_Queue From_Par);

	DECLARE_IBaseOperation;
};

/*!
  \class   RDOPROCDepart
  \brief   ���������� ���� DEPART
*/
class RDOPROCDepart: public RDOPROCBlockForQueue, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCDepart);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlockForQueue)
	QUERY_INTERFACE       (IBaseOperation      )
QUERY_INTERFACE_END

public:
	static ruint   getDefaultValue   ();
	static tstring getDepartParamName();

private:
	RDOPROCDepart(LPIPROCProcess process, parser_for_Queue From_Par);

	DECLARE_IBaseOperation;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/process/queue_depart.inl"

#endif // _LIB_RUNTIME_PROCESS_QUEUE_DEPART_H_
