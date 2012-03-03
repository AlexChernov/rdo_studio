/*!
  \copyright (c) RDO-Team, 2012
  \file      seize_release.h
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      12.02.2012
  \brief     ���������� ��������� SEIZE � RELEASE
  \indent    4T
*/

#ifndef _LIB_RUNTIME_PROCESS_SEIZE_RELEASE_H_
#define _LIB_RUNTIME_PROCESS_SEIZE_RELEASE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/noncopyable.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/process/rdoprocess_i.h"
#include "simulator/runtime/rdo_logic.h"
#include "simulator/runtime/rdo_res_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \struct  runtime_for_Seize
  \brief   ��������������� ����� ��� ��������� SEIZE � Runtime
*/
struct runtime_for_Seize
{
	LPRDOPROCResource  rss;
	int                Id_param;
	RDOValue           enum_free;
	RDOValue           enum_buzy;
	RDOValue           enum_break;
};

/*!
  \struct  parser_for_Seize
  \brief   ��������������� ����� ��� ��������� SEIZE � Parser
*/
struct parser_for_Seize
{
	int Id_res;
	int Id_param;
};

/*!
  \class   RDOPROCBlockForSeize
  \brief   ��������������� ����� ��� ����������� ��������� SEIZE
*/
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

/*!
  \class   RDOPROCSeize
  \brief   ���������� ���� SEIZE
*/
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

	virtual void transactGoIn (CREF(LPTransact) pTransact);
	virtual void transactGoOut(CREF(LPTransact) pTransact);

	DECLARE_IBaseOperation;
};

/*!
  \class   RDOPROCRelease
  \brief   ���������� ���� RELEASE
*/
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

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/process/seize_release.inl"

#endif // _LIB_RUNTIME_PROCESS_SEIZE_RELEASE_H_
