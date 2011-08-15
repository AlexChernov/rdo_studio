/******************************************************************************//**
 * @copyright (c) RDO-Team, 2006
 * @file      rdoprocess.h
 * @authors   ������ ������, ����� �������, etc.
 * @date      11.06.2006
 * @brief     ����������� ��������� ���
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_PROCESS_H_
#define _LIB_RUNTIME_PROCESS_H_

// **************************************************************************** PCH
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdoprocess_i.h"
#include "rdo_lib/rdo_runtime/rdo_logic.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @class   RDOPROCBlock
 * @brief   ������� ����� ��� ���������� ������ (����������) ���
 *********************************************************************************/
class RDOPROCBlock: public IPROCBlock, public IInit, CAST_TO_UNKNOWN
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPROCBlock)
	QUERY_INTERFACE(IInit     )
QUERY_INTERFACE_END

friend class RDOPROCTransact;
friend class RDOPROCProcess;
friend class RDOPROCResource;

protected:
	LPIPROCProcess  m_process;
	TransactList    m_transacts;

	RDOPROCBlock(LPIPROCProcess process);
	virtual ~RDOPROCBlock();

	DECLARE_IPROCBlock;
	DECLARE_IInit;
};

/******************************************************************************//**
 * @class   RDOPROCProcess
 * @brief   ������� � ���
 *********************************************************************************/
class RDOPROCProcess: public RDOLogicSimple, public IPROCProcess, public RDOPatternPrior
{
DEFINE_IFACTORY(RDOPROCProcess)
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE       (IPROCProcess)
	QUERY_INTERFACE_PARENT(RDOLogic    )
	QUERY_INTERFACE_PARENT(RDOPatternPrior)
QUERY_INTERFACE_END
friend class RDOPROCBlock;

public:
	DECLARE_IPROCProcess;

protected:
	tstring                    m_name;
	LPIPROCProcess             m_parent;
	std::list<LPIPROCProcess>  m_child;

private:
	RDOPROCProcess(CREF(tstring) _name, CREF(LPRDORuntime) pRuntime);
	LPRDOResourceType            m_transactType;
};

PREDECLARE_POINTER(RDOPROCResource);

/******************************************************************************//**
 * @class   RDOPROCTransact
 * @brief   �������� � ���
 *********************************************************************************/
CLASS_PARENT_OF(RDOPROCTransact, RDOResource)
{
DECLARE_FACTORY(RDOPROCTransact);
public:
	LPRDOPROCResource getRes();
	void setRes(CREF(LPRDOPROCResource) pResource);
	REF(LPIPROCBlock) getBlock();
	void setBlock(CREF(LPIPROCBlock) block);

	void next();
	virtual LPRDOResource clone(CREF(LPRDORuntime) pRuntime) const;

private:
	RDOPROCTransact(CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool permanentFlag);
	virtual ~RDOPROCTransact();

	LPIPROCBlock       m_block;
	LPRDOPROCResource  m_res;
};

/******************************************************************************//**
 * @class   RDOPROCResource
 * @brief   ���������� ������ ��� - ����������� ��������� � ���������
 *********************************************************************************/
CLASS_PARENT_OF(RDOPROCResource, RDOResource)
{
DECLARE_FACTORY(RDOPROCResource);
friend class RDOPROCSeize;
friend class RDOPROCRelease;

public:
	tstring whoAreYou();
	virtual LPRDOResource clone(CREF(LPRDORuntime) pRuntime) const;

protected:
	std::list<LPRDOPROCTransact> transacts;

private:
	RDOPROCResource(CREF(LPRDORuntime) pRuntime, CREF(std::vector<RDOValue>) paramsCalcs, LPIResourceType pResType, ruint resID, ruint typeID, rbool trace, rbool permanentFlag);
	virtual ~RDOPROCResource();
};

/******************************************************************************//**
 * @class   RDOPROCGenerate
 * @brief   ���������� ���� GENERATE
 *********************************************************************************/
class RDOPROCGenerate: public RDOPROCBlock, public IBaseOperation 
{
DEFINE_IFACTORY(RDOPROCGenerate);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

public:
	void calcNextTimeInterval(CREF(LPRDORuntime) pRuntime);

private:
	RDOPROCGenerate(LPIPROCProcess process, CREF(LPRDOCalc) pTime, int maxTransCount = 0);

	double     timeNext;
	LPRDOCalc  pTimeCalc;
	int        m_maxTransCount;
	int        m_TransCount;

	DECLARE_IBaseOperation;
};

/******************************************************************************//**
 * @struct  runtime_for_Queue
 * @brief   ��������������� ����� ��� ��������� QUEUE � Runtime
 *********************************************************************************/
struct runtime_for_Queue
{
	LPRDOResource rss; 
	int           Id_param;
	RDOValue      defaultValue;
};

/******************************************************************************//**
 * @struct  parser_for_Queue
 * @brief   ��������������� ����� ��� ��������� QUEUE � Parser
 *********************************************************************************/
struct parser_for_Queue
{
	int Id_res;
	int Id_param;
};

/******************************************************************************//**
 * @class   RDOPROCBlockForQueue
 * @brief   ��������������� ����� ��� ����������� ��������� QUEUE
 *********************************************************************************/
class RDOPROCBlockForQueue: public RDOPROCBlock
{
protected:
	RDOPROCBlockForQueue(LPIPROCProcess process, parser_for_Queue From_Par);

	parser_for_Queue  fromParser;
	runtime_for_Queue forRes;
	void _onStart(CREF(LPRDORuntime) pRuntime);
};

/******************************************************************************//**
 * @class   RDOPROCQueue
 * @brief   ���������� ���� QUEUE
 *********************************************************************************/
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

/******************************************************************************//**
 * @class   RDOPROCDepart
 * @brief   ���������� ���� DEPART
 *********************************************************************************/
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

/******************************************************************************//**
 * @struct  runtime_for_Seize
 * @brief   ��������������� ����� ��� ��������� SEIZE � Runtime
 *********************************************************************************/
struct runtime_for_Seize
{
	LPRDOPROCResource  rss;
	int                Id_param;
	RDOValue           enum_free;
	RDOValue           enum_buzy;
	RDOValue           enum_break;
};

/******************************************************************************//**
 * @struct  parser_for_Seize
 * @brief   ��������������� ����� ��� ��������� SEIZE � Parser
 *********************************************************************************/
struct parser_for_Seize
{
	int Id_res;
	int Id_param;
};

/******************************************************************************//**
 * @class   RDOPROCBlockForSeize
 * @brief   ��������������� ����� ��� ����������� ��������� SEIZE
 *********************************************************************************/
class RDOPROCBlockForSeize: public RDOPROCBlock
{
public:
	static tstring getStateParamName();
	static tstring getStateEnumFree();
	static tstring getStateEnumBuzy();

protected:
	RDOPROCBlockForSeize(LPIPROCProcess process, std::vector<parser_for_Seize> From_Par);

	std::vector<runtime_for_Seize> forRes;
	std::vector<parser_for_Seize>  fromParser;

	void _onStart(CREF(LPRDORuntime) pRuntime);
};

/******************************************************************************//**
 * @class   RDOPROCSeize
 * @brief   ���������� ���� SEIZE
 *********************************************************************************/
class RDOPROCSeize: public RDOPROCBlockForSeize, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCSeize);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlockForSeize)
	QUERY_INTERFACE       (IBaseOperation      )
QUERY_INTERFACE_END

private:
	RDOPROCSeize(LPIPROCProcess process, std::vector <parser_for_Seize> From_Par);

	ruint index;

	virtual void transactGoIn (PTR(RDOPROCTransact) _transact);
	virtual void transactGoOut(PTR(RDOPROCTransact) _transact);

	DECLARE_IBaseOperation;
};

/******************************************************************************//**
 * @class   RDOPROCRelease
 * @brief   ���������� ���� RELEASE
 *********************************************************************************/
class RDOPROCRelease: public RDOPROCBlockForSeize, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCRelease);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlockForSeize)
	QUERY_INTERFACE       (IBaseOperation      )
QUERY_INTERFACE_END

private:
	RDOPROCRelease(LPIPROCProcess process, std::vector <parser_for_Seize> From_Par);

	ruint index;

	DECLARE_IBaseOperation;
};

/******************************************************************************//**
 * @class   RDOPROCAdvance
 * @brief   ���������� ���� ADVANCE
 *********************************************************************************/
class RDOPROCAdvance: public RDOPROCBlock, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCAdvance);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

protected:
	LPRDOCalc pDelayCalc;

	/**
	 * @struct  LeaveTr
	 * @brief   ����� ����� ���������
	 * @todo    ��� ���?
	 */
	struct LeaveTr
	{
		LPRDOPROCTransact transact;
		double            timeLeave;
		LeaveTr(CREF(LPRDOPROCTransact) _transact, double _timeLeave);
	};
	std::list<LeaveTr> leave_list;

private:
	RDOPROCAdvance(LPIPROCProcess process, CREF(LPRDOCalc) _pDelayCalc);

	DECLARE_IBaseOperation;
};

/******************************************************************************//**
 * @class   RDOPROCTerminate
 * @brief   ���������� ���� TERMINATE
 *********************************************************************************/
class RDOPROCTerminate: public RDOPROCBlock, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCTerminate);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

public:
	int getTerm();

private:
	RDOPROCTerminate(LPIPROCProcess process, ruint _term);
	const ruint term; 
	DECLARE_IBaseOperation;
};

/******************************************************************************//**
 * @class   RDOPROCAssign
 * @brief   ���������� ���� ASSIGN
 *********************************************************************************/
class RDOPROCAssign: public RDOPROCBlock, public IBaseOperation
{
DEFINE_IFACTORY(RDOPROCAssign);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(RDOPROCBlock  )
	QUERY_INTERFACE       (IBaseOperation)
QUERY_INTERFACE_END

private:
	RDOPROCAssign(LPIPROCProcess process, CREF(LPRDOCalc) pValue, int Id_res, int Id_param);

	LPRDOCalc pParamValue;
	int       t_resId;
	int       t_parId;

	DECLARE_IBaseOperation;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdoprocess.inl"

#endif // _LIB_RUNTIME_PROCESS_H_
