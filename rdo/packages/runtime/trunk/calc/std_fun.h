/*!
  \copyright (c) RDO-Team, 2011
  \file      std_fun.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     ����������� ������� �����
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_STD_FUN_H_
#define _LIB_RUNTIME_CALC_STD_FUN_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdocalc.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class   std_fun1
  \brief   ������� � ����� ����������
*/
template <class RT, class P1>
class std_fun1
{
public:
	enum { arity = 1 };

	typedef RT return_type;
	typedef P1 arg1_type;
	typedef RT (*function_type)(P1);
};

/*!
  \class   std_fun2
  \brief   ������� � ����� �����������
*/
template <class RT, class P1, class P2>
class std_fun2
{
public:
	enum { arity = 2 };

	typedef RT return_type;
	typedef P1 arg1_type;
	typedef P2 arg2_type;
	typedef RT (*function_type)(P1, P2);
};

/*!
  \class   RDOFunCalcStd
  \brief   ������� �� ������������ ���� std C++
*/
template <class F>
class RDOFunCalcStd: public RDOFunCalc
{
public:
	typedef typename F::function_type function_type;

	RDOFunCalcStd(CREF(function_type) pFunction);

private:
	function_type m_pFunction;

	REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);

	template <int paramCount>
	FORCEINLINE void calc(CREF(LPRDORuntime) pRuntime);

	/// @todo ��������� ����������� �������-������ � std_fun.inl
	template <>
	FORCEINLINE void calc<1>(CREF(LPRDORuntime) pRuntime)
	{
		m_value = m_pFunction(getParam<F::arg1_type>(pRuntime, 0));
	}

	template <>
	FORCEINLINE void calc<2>(CREF(LPRDORuntime) pRuntime)
	{
		m_value = m_pFunction(getParam<F::arg1_type>(pRuntime, 0), getParam<F::arg2_type>(pRuntime, 1));
	}

	template <class T>
	FORCEINLINE T getParam(CREF(LPRDORuntime) pRuntime, ruint paramNumber);

	template <>
	FORCEINLINE double getParam<double>(CREF(LPRDORuntime) pRuntime, ruint paramNumber)
	{
		return pRuntime->getFuncArgument(paramNumber).getDouble();
	}

	template <>
	FORCEINLINE int getParam<int>(CREF(LPRDORuntime) pRuntime, ruint paramNumber)
	{
		return pRuntime->getFuncArgument(paramNumber).getInt();
	}
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/std_fun.inl"

#endif // _LIB_RUNTIME_CALC_STD_FUN_H_
