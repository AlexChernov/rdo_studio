/*!
  \copyright (c) RDO-Team, 2009-2012
  \file      main.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      01.05.2012
  \brief     ���� ��� ������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOCalc_Test
#include <boost/test/included/unit_test.hpp>
#include <boost/tuple/tuple.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/runtime/calc/procedural/calc_const.h"
#include "simulator/runtime/calc/procedural/calc_statement.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

BOOST_AUTO_TEST_SUITE(RDOCalc_Test)

static const double const1 = 10.5;
static const double const2 = 11.4;

typedef  boost::tuple<LPRDORuntime, LPRDOCalc, LPRDOCalc>  CalcPair;

template <class F>
RDOValue calc(F binder, CREF(CalcPair) calcPair)
{
	BOOST_CHECK(calcPair.get<0>());
	BOOST_CHECK(calcPair.get<1>());
	BOOST_CHECK(calcPair.get<2>());

	return binder.operator()(calcPair.get<1>()->calcValue(calcPair.get<0>()), calcPair.get<2>()->calcValue(calcPair.get<0>()));
}

CalcPair prepair()
{
	LPRDORuntime pRuntime = rdo::Factory<RDORuntime>::create();
	BOOST_CHECK(pRuntime);

	LPRDOCalc pLeft = rdo::Factory<RDOCalcConst>::create(RDOValue(const1));
	BOOST_CHECK(pLeft);

	LPRDOCalc pRight = rdo::Factory<RDOCalcConst>::create(RDOValue(const2));
	BOOST_CHECK(pRight);

	return CalcPair(pRuntime, pLeft, pRight);
}

BOOST_AUTO_TEST_CASE(RDOCalc_MultDouble)
{
	RDOValue result = calc(boost::bind(&RDOValue::operator*, _1, _2), prepair());
	BOOST_CHECK(result.getDouble() == const1 * const2);
}

BOOST_AUTO_TEST_CASE(RDOCalc_DivDouble)
{
	RDOValue result = calc(boost::bind(&RDOValue::operator/, _1, _2), prepair());
	BOOST_CHECK(result.getDouble() == const1 / const2);
}

BOOST_AUTO_TEST_CASE(RDOCalc_PlusDouble)
{
	RDOValue result = calc(boost::bind(&RDOValue::operator+, _1, _2), prepair());
	BOOST_CHECK(result.getDouble() == const1 + const2);
}

BOOST_AUTO_TEST_CASE(RDOCalc_MinusDouble)
{
	RDOValue result = calc(boost::bind(&RDOValue::operator-, _1, _2), prepair());
	BOOST_CHECK(result.getDouble() == const1 - const2);
}

BOOST_AUTO_TEST_CASE(RDOCalc_RecursSimulator)
{
	struct RecursCalcSimulator
	{
		struct Param
		{
			Param(int value)
				: m_value(value)
			{}

			//! �����, ����� ��� ��� int, � �� int&
			//! ����� ���������� ����� ������� ����� �� 1
			//! � ������ 1 � 120
			int value() 
			{
				return m_value;
			}

			Param& dec()
			{
				--m_value;
				return *this;
			}

		private:
			int m_value;
		};

		int funError(Param& param)
		{
			if (param.value() == 1)
			{
				return 1;
			}
			else
			{
				return funError(param.dec()) * param.value();
			}
		}

		int funOk(Param& param)
		{
			if (param.value() == 1)
			{
				return 1;
			}
			else
			{
				return param.value() * funOk(param.dec());
			}
		}
	};

	RecursCalcSimulator calc1;
	RecursCalcSimulator calc2;
	RecursCalcSimulator::Param param1(5);
	RecursCalcSimulator::Param param2(5);

	int resultError = calc1.funError(param1);
	int resultOk    = calc2.funOk   (param2);
	BOOST_CHECK(resultError == 1);
	BOOST_CHECK(resultOk    == 120);
}

BOOST_AUTO_TEST_SUITE_END() // RDOCalc_Test

CLOSE_RDO_RUNTIME_NAMESPACE
