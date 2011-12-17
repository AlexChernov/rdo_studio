/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_function_internal.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     ����������� ������� �����
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_FUNCTION_INTERNAL_H_
#define _LIB_RUNTIME_CALC_FUNCTION_INTERNAL_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdomacros.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class std_fun_one_param {};
class std_fun_two_param {};

/*!
  \class   std_fun1
  \brief   ������� � ����� ����������
*/
template <class RT, class P1>
class std_fun1
{
public:
	typedef  RT                 return_type;
	typedef  P1                 arg1_type;
	typedef  RT                 (*function_type)(P1);
	typedef  std_fun_one_param  param_count;
};

/*!
  \class   std_fun2
  \brief   ������� � ����� �����������
*/
template <class RT, class P1, class P2>
class std_fun2
{
public:
	typedef  RT                 return_type;
	typedef  P1                 arg1_type;
	typedef  P2                 arg2_type;
	typedef  RT                 (*function_type)(P1, P2);
	typedef  std_fun_two_param  param_count;
};

template <class T>
struct GetParam         { static T      getParam(CREF(LPRDORuntime) pRuntime, ruint paramNumber); };

template <>
struct GetParam<double> { static double getParam(CREF(LPRDORuntime) pRuntime, ruint paramNumber); };

template <>
struct GetParam<int>    { static int    getParam(CREF(LPRDORuntime) pRuntime, ruint paramNumber); };

/*!
  \class   RDOFunCalcStd
  \brief   ������� �� ������������ ���� std C++
*/
template <class F>
class RDOFunCalcStd: public RDOFunCalc
{
public:
	typedef typename F::function_type function_type;

	RDOFunCalcStd(function_type pFunction);

private:
	function_type m_pFunction;

	REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);

	void calc(CREF(LPRDORuntime) pRuntime, std_fun_one_param);
	void calc(CREF(LPRDORuntime) pRuntime, std_fun_two_param);
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/calc_internal_function.inl"

#endif // _LIB_RUNTIME_CALC_FUNCTION_INTERNAL_H_
