/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_sequence.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      13.03.2011
  \brief     ������������������
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_SEQUENCE_H_
#define _LIB_RUNTIME_CALC_SEQUENCE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/function/calc_function.h"
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ������������� ������������������ ��������������� �����
CALC(RDOCalcSeqInit)
{
DECLARE_FACTORY(RDOCalcSeqInit)
public:
	void setBase(int base);

private:
	RDOCalcSeqInit(int base, PTR(RandGenerator) gen);
	virtual ~RDOCalcSeqInit();

	int                m_base;
	PTR(RandGenerator) m_gen;

	DECLARE_ICalc;
};

//! ��������� ��������� �������� ������������������ ��������������� �����
CALC_SUB(RDOCalcSeqNext, RDOFunCalc)
{
public:
	struct Range
	{
		double  m_min;
		double  m_max;

		Range();
		Range(CREF(double) min, CREF(double) max);
	};

	rbool                   m_res_real;
	boost::optional<Range>  m_range;

protected:
	RDOCalcSeqNext();

	virtual RDOValue getNextValue(CREF(LPRDORuntime) pRuntime) = 0;
};

//! ��������������� �������������
template<class T>
class RDOCalcRandomDistribution: public RDOCalcSeqNext
{
protected:
	RDOCalcRandomDistribution(PTR(T) gen);
	virtual ~RDOCalcRandomDistribution();
	PTR(T) m_gen;

private:
	virtual REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);
};

/*!
  \def     DEFINE_RANDON_DISTRIBUTION
  \brief   ������ ��� ������ ������� �������������
*/
#define DEFINE_RANDON_DISTRIBUTION(CalcName, Distribution)                  \
CALC_SUB(RDOCalcSeqNext##CalcName, RDOCalcRandomDistribution<Distribution>) \
{                                                                           \
DECLARE_FACTORY(RDOCalcSeqNext##CalcName)                                   \
private:                                                                    \
	RDOCalcSeqNext##CalcName(PTR(Distribution) gen)                         \
		: RDOCalcRandomDistribution<Distribution>(gen)                      \
	{}                                                                      \
                                                                            \
	virtual RDOValue getNextValue(CREF(LPRDORuntime) pRuntime);             \
};

/*!
  \class   RDOCalcSeqNextUniform
  \brief   ��������� �������� ��������������� ��������, �������������� ����������
*/
DEFINE_RANDON_DISTRIBUTION(Uniform, RandGeneratorUniform);

/*!
  \class   RDOCalcSeqNextNormal
  \brief   ��������� �������� ��������������� ��������, �������������� �� ����������� ������
*/
DEFINE_RANDON_DISTRIBUTION(Normal, RandGeneratorNormal);

/*!
  \class   RDOCalcSeqNextExponential
  \brief   ��������� �������� ��������������� ��������, �������������� �� ����������������� ������
*/
DEFINE_RANDON_DISTRIBUTION(Exponential, RandGeneratorExponential);

/*!
  \class   RDOCalcSeqNextTriangular
  \brief   ��������� �������� ��������������� ��������, �������������� �� ������������ ������
*/
DEFINE_RANDON_DISTRIBUTION(Triangular, RandGeneratorTriangular);

/*!
  \class   RDOCalcSeqNextByHist
  \brief   ��������� �������� ��������������� ��������, �������������� �� �������� �����������
*/
DEFINE_RANDON_DISTRIBUTION(ByHist, RandGeneratorCommonNext);

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/calc_sequence.inl"

#endif // _LIB_RUNTIME_CALC_SEQUENCE_H_
