/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_random_distribution.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      11.04.2008
  \brief     ������� ��������������� �����
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_
#define _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <math.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! ������ ������������� ��������������� ����� � ����� ���
#define RDO_NATIVE_U01

//! ������� ����������� ������������� [0, 1)
class RandGenerator
{
public:
	//! ������� ������, ����� ������ ����������� ���������� ��� ������ �� \ref RDOCalcSeqInit::~RDOCalcSeqInit()
	virtual ~RandGenerator();

	//! ��������� ���� ����������
	//! @param seed - ���� ����������
	void setSeed(long int seed = 123456789);

	//! @brief  ��������� ������-���������� ����� �� �������� [0, 1) �� ������������ ������ �������������
	/*! @note   ���� ��� ����� ������ � ����������� ������, ������ �������������.
		         69069 - �� �����, ��������� �������� ���������� ��� ������ � ������� (���������).
		         +1    - ��������� ����� ����� ��������� �� ����.
		         ������� ���������� (long double)UINT_MAX + 1*/
	//! @result ������-��������� �����, �������� � �������� [0, 1)
	double u01();

protected:

#ifdef RDO_NATIVE_U01
	//! @param  m_seed  ���� ����������
	ruint m_seed;
#else
	long int m_seed;
#endif
	RandGenerator(long int seed = 123456789);
};

//! ����������� ������������� [0, 1)
class RandGeneratorUniform: public RandGenerator
{
public:
	//! @param   seed   ���� ���������� ��������������� �����
	RandGeneratorUniform(long int seed = 123456789);

	//! @brief          ��������� ����� �� ������������������ �� ������������ ������
	//! @param   from   ������ ������� ��������� �������������
	//! @param   to     ������� ������� ��������� �������������
	//! @note           ������� ����������� ������������� �������������� ����������� �� ������ ��������� � ��������� ������������ ����� �������
	//! @return         ����� �� ��������� �������������
	double next(double from, double to);
};

//! ���������������� �������������
class RandGeneratorExponential: public RandGenerator
{
public:
	//! @param   seed   ���� ���������� ��������������� �����
	RandGeneratorExponential(long int seed = 123456789);

	//! @brief          ��������� ����� �� ������������������ �� ���������������� ������
	//! @param   math   �������������� �������� ������ �������������
	//! @result         ����� �� 0 �� �������������
	double next(double math);
};

//! ���������� �������������
class RandGeneratorNormal: public RandGenerator
{
public:
	//! @param   seed   ���� ���������� ��������������� �����
	RandGeneratorNormal(long int seed = 123456789);

	//! @brief          ��������� ����� �� ������������������ �� ����������� ������
	//! @param   av     �������������� �������� ������������������
	//! @param   var    �������������������� ����������
	//! @note           ������������ ������������, �������������� ������ ��������� (����� �� ���������� �� ����������������)
	//! @result         ����� �� ����� ������������ �� �������������. �� �� ����� ���� ����� ����� �� ��������� av+-3*var
	double next(double av, double var);
};

//! ����������� �������������
class RandGeneratorTriangular: public RandGenerator
{
public:
	//! @param   seed   ���� ���������� ��������������� �����
	RandGeneratorTriangular(long int seed = 123456789);

	//! @brief         ��������� ����� �� ������������������ �� ������������ ������
	//! @param   from  ����� ������� ��������� �������������
	//! @param   top   ����� ��� �������� ������������
	//! @param   to    ������ ������� ��������� �������������
	/*! @note          ��� ���������� ������������ ������������ ����� ��� �������� ������������.
	                   ���� �������� ������ �� ��� �����: ����� � ������ �� ������� ������������.
	                   � ������� �������� ������������ ������������� ������������, � ������� �� ������ ����� ������ ��������������� �����.
	                   ����� ����� �������� ������� �� �������� ������������ ������������� ������������ �����.
	                   ����� ��� ���������� (-1), �������������� (�����������). ������������ �����, ��������� �� ���������� �� ������ ��������� �� ������� ������������*/
	//! result         ����� �� ��������� �������������
	double next(double from, double top, double to);
};

//! ����� ����� ��� by_hist � enumerative
class RandGeneratorCommonNext
{
public:
	virtual ~RandGeneratorCommonNext();
	virtual RDOValue next() = 0;

protected:
	RandGeneratorCommonNext();
};

//! ������� ����� ��� ����������
class RandGeneratorByHist: public RandGeneratorUniform, public RandGeneratorCommonNext
{
protected:
	double summ;
	//! @param  seed  ���� ���������� �����
	RandGeneratorByHist(long int seed = 123456789);
};

//! ����������� ������������ �����
class RandGeneratorByHistReal: public RandGeneratorByHist
{
public:
	//! @param   seed   ���� ���������� �����
	RandGeneratorByHistReal(long int seed = 123456789);

	//! @brief        ���������� �������� � ������������������ by_hist
	//! @param  from  ������ ������� ���������
	//! @param  to    ������� ������� ���������
	//! @param  freq  ������������� ������� ��������� ������������������ � �������� ��������
	//! @note         ���������� ����� � ��������� ���� vector. ������������ ������������� ������
	void addValues(double from, double to, double freq);

private:
	std::vector< double > m_from;
	std::vector< double > m_to;
	std::vector< double > m_freq;

	//! @brief   ��������� �������� ������������������ by_hist (Real)
	/*! @note    ������������ ����� �� ������������ ������ �� 0 �� ����� ���� ������.
	             ������������ ����� ��������, ��� ���������� �������� ����� ���� ���������� ������ � �������� �������� �������� ��������������� �����
	             ������������ �����, ���������� �� ������������ ������ �� ��������� � ���� ���������� �������*/
	//! @return  �������� �� ����� ����� �� ���� ������ �� ����� ������.
	virtual RDOValue next();
};

//! ����������� ������������� ����
class RandGeneratorByHistEnum: public RandGeneratorByHist
{
public:
	//! @param   seed   ���� ���������� �����
	RandGeneratorByHistEnum(long int seed = 123456789);

	//! @brief        ���������� �������� � ������������������ by_hist
	//! @param  val   ��� ������������� ����
	//! @param  freq  ������������� ������� ��������� ������������������ � �������� ��������
	//! @note         ���������� ����� � ��������� ���� vector. ������������ ������������� ������
	void addValues(RDOValue val, double freq);

private:
	std::vector< RDOValue > m_vals;
	std::vector< double >   m_freq;

	//! @brief   ��������� �������� ������������������ by_hist (Enum)
	/*! @note    ������������ ����� �� ������������ ������ �� 0 �� ����� ���� ������.
	             ������������ ����� ��������, ��� ���������� �������� ����� ���� ���������� ������ � �������� �������� �������� ��������������� �����
	             ������������ �����, �������������� ���� ����������� ������.*/
	//! @result  �������� �� ����� ����� �� ���� ������ �� ����� ������.
	virtual RDOValue next();
};

//! �������� ��������
class RandGeneratorEnumerative: public RandGeneratorCommonNext
{
public:
	RandGeneratorEnumerative();
	virtual ~RandGeneratorEnumerative();

	//! @brief       ���������� �������� � ������������������ enumerative
	//! @param  val  ��� ������������ ��������� ��� ��� �������� � ����������� � ����� �������� ������������������
	//! @note        ���������� �������� � ��������� ���� vector
	void addValue(RDOValue val);

private:
	std::vector<RDOValue>            m_vals;
	std::vector<RDOValue>::size_type m_curr;

	//! @brief   ��������� �������� �� ��������� ��������� Enum
	//! @note    ���������� ��������� �������� �� ��������� ������������ ���������. ��� ��������� ������ - ������� � ������
	//! @return  ���� �� ������������� ��������
	virtual RDOValue next();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_random_distribution.inl"

#endif // _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_
