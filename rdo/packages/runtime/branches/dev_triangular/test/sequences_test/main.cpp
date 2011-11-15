/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   �������� ����� (impus@hotbox.ru)
  \date      2.10.2011
  \brief     ���� ������� �������������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOSequencesTest
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdofile.h"
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------

typedef std::vector<double> Container;
typedef const tstring contstr;

const long int g_seed                 = 123456789;                  //!< ���� ����������
contstr        g_fileNormalName       = _T("data_normal.txt");      //!< ���� ������
contstr        g_fileUniformName      = _T("data_uniform.txt");     //!< ���� ������
contstr        g_fileExponentialName  = _T("data_exponential.txt"); //!< ���� ������
contstr        g_fileTriangularName   = _T("data_trinagular.txt");  //!< ���� ������

const ruint    g_count                = 100000;                     //!< ���������� ������������ ������
const double   g_main                 = 10.0;                       //!< �������� ������ ����������������� � �����������
const double   g_var                  = 1.0;                        //!< �������� ������ �����������
const double   g_from                 = 1.0;                        //!< �������� ������ ������������ � ������������
const double   g_to                   = 7.0;                        //!< �������� ������ ������������ � ������������
const double   g_top                  = 5.0;                        //!< �������� ������ ������������
const ruint    g_precision            = 20;                         //!< �������� ������������� ����� ��� ������ � �����

const ruint    g_countOfExamples      = 2000;                       //!< ���������� ����� � �������
const ruint    g_countOfFree          = 39;                         //!< ����� ��������
const double   pi                     = 3.141592653;                //!< ��������������� ���������
const double   g_ksiEtalon            = 54.5722;                    //!< ��������� ��������

// --------------------------------------------------------------------------------
// -------Templates
// --------------------------------------------------------------------------------
template <class T, class F, class contstr>
void onGenerateData(F binder, contstr g_fileName)
{
	if (rdo::File::exist(g_fileName.c_str()))
		return;

	T sequence(g_seed);
	Container test(g_count);

	for (ruint i = 0; i < g_count; ++i)
	{
		test[i] = binder.operator()(&sequence);
	}

	std::ofstream stream(g_fileName.c_str());
	stream.precision(g_precision);
	STL_FOR_ALL(test, it)
	{
		stream << *it << std::endl;
	}
}

template <class T, class F, class contstr>
void onCheckData(F binder, contstr g_fileName)
{
	std::ifstream stream(g_fileName.c_str());
	BOOST_CHECK(stream.good());

	Container test(g_count);
	T sequence(g_seed);
	for (ruint i = 0; i < g_count; ++i)
	{
		test[i] = binder.operator()(&sequence);
	}

	stream.precision(g_precision);
	STL_FOR_ALL(test, it)
	{
		BOOST_CHECK(stream.good());
		tstring str;
		stream >> str;

		double val;
		BOOST_CHECK(sscanf_s(str.c_str(), _T("%lf"), &val) == 1);
		BOOST_CHECK(val == *it);
				if (val != *it)
		{
			std::cout.precision(g_precision);
			std::cout << *it << std::endl;
			std::cout << val << std::endl;
		}
	}
}

template <class T,class F>
double  area (F binder, double elem, double n, double m)
{
	double k = 1;
	double S1 = 1;
	double S2 = 0;
	ruint t = (m-n)/elem;
	while (fabs(S1-S2)/S1 > 0.01)
	{
		S2 = S1;
		S1 = 0;
		for (ruint i = 1; i < t-1; ++i)
		{
			if ((i == 0) || (i == t - 1))
				k = 0.5;
			S1 += k*(binder.operator()(i*(m-n)/t));
			k = 1;
		}
		S1 *= (m-n);
		t  *= 10;
	}
	return S1;
}

template <class T, class G, class F, class S>
void onCheckKsi(F binder, S binderSeq, double left, double right)
{
	const ruint countOfxI   = g_countOfFree*1000;	//���������� ��������, �� 1 ������ ����� �����
	const ruint areaOfI = 1/g_countOfFree;			//������� �� �������

	Container xTemp(countOfxI + 1);			//������ ��������� ��������� ��������, �� ������ ������� ����� ���� ������� ������� �������
	double elem = (right-left)/(countOfxI);	//���������� ����� ������� �� ������, ����� ���������� ����������

	for (ruint i = 0; i < countOfxI + 1; ++i)//������� ������������ �������� � ���������. �����, ��� �� ��� ������� ������ ����� (��� ��������� ��������) ���� ����������� �������� ��� �����������
	{
		xTemp[i] = elem*i;
	}

	Container x;							//��������� ��� ������ ����������, ������� ����� ������������ � �����������
	x.push_back(left);						//����� ������� ����� � ������ ������, ������ ������� ��� ������� � ��� ����� - right

	for (ruint i = 1; i < countOfxI; ++i)
	{
		if (fabs(area<T>(binder, elem, x.back(), xTemp[i]) - areaOfI) < fabs(area<T>(binder, elem, x.back(), xTemp[i+1]) - areaOfI))
		{
			x.push_back(xTemp[i]);
		}
	}
	if(x.size() == g_countOfFree)			//�������������� ������, ����� ������� �� ��������� ��������� �������� ���� ������ ��� ���������� ��� ���� �������. ����������� ���������� ����� ����� ��-�� �����. �������� ����� ���������� ��� ������� ���������� ��������� � ��������� ��������. � ��� �� �� ����
	{
		x.push_back(right);
	}
	else if(x.back() < right)				//������, ����� ��������� �������� ������������� ������ ����� ��������� ���������
	{
		x.pop_back();
		x.push_back(right);
	}

	//����, ������ � ��� ���� ������� ����������

	Container vb(g_countOfExamples);		//��������� ��� �������� �������

	G sequence(g_seed);
	for (ruint i = 0; i < g_countOfExamples; ++i)
	{
		vb[i] = binderSeq.operator()(&sequence);
	}

	Container fI(g_countOfFree);
	
	for(ruint i = 0; i < g_countOfFree - 1; ++i)
	{
		ruint freq = 0;
		for(ruint k = 0; k < g_countOfExamples; ++k)
		{
			if((vb[k] > x[i]) & (vb[k] <= x[i+1]))
			{
				++freq;
			}
		}
		fI[i] = freq;
	}

	double ksi = 0;
	double Fi = g_countOfExamples/g_countOfFree;
	for(ruint i = 0; i < g_countOfFree; ++i)
	{
		ksi += ((fI[i] - Fi)*(fI[i] - Fi))/Fi;
	}

	BOOST_CHECK(ksi < g_ksiEtalon);
}
// --------------------------------------------------------------------------------

class SequenceNormal
{
public:
	SequenceNormal(double main, double var)
		: m_main(main)
		, m_var (var )
	{}

	double get(double x) const
	{
		return 1/(sqrt(2*pi*m_var)*exp((x - m_main)*(x - m_main)/(2*m_var))); //������� ��������� ������������� �����������. �� ����������� ��� ������. ����� ����� ��� 3 �����.
	}

private:
	double m_main;
	double m_var;
};

BOOST_AUTO_TEST_SUITE(RDOSequencesTest)
/*
// --------------------------------------------------------------------------------
// -------Normal sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDONormalTestCreate)
{
	onGenerateData<rdoRuntime::RandGeneratorNormal>
		(boost::bind(&rdoRuntime::RandGeneratorNormal::next, _1, g_main, g_var), g_fileNormalName);
}
*/
BOOST_AUTO_TEST_CASE(RDONormalTestCheck)
{
/*	onCheckData<rdoRuntime::RandGeneratorNormal>
		(boost::bind(&rdoRuntime::RandGeneratorNormal::next, _1, g_main, g_var), g_fileNormalName);
*/
	SequenceNormal normal(g_main, g_var);
	onCheckKsi<SequenceNormal, rdoRuntime::RandGeneratorNormal>
		(boost::bind(&SequenceNormal::get, normal, _1),
		boost::bind(&rdoRuntime::RandGeneratorNormal::next, _1, g_main, g_var),
		g_main-4*sqrt(g_var),
		g_main+4*sqrt(g_var));
}

// --------------------------------------------------------------------------------
/*
// --------------------------------------------------------------------------------
// -------Uniform sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDOUniformTestCreate)
{
	onGenerateData<rdoRuntime::RandGeneratorUniform>
		(boost::bind(&rdoRuntime::RandGeneratorUniform::next, _1, g_from, g_to), g_fileUniformName);
}

BOOST_AUTO_TEST_CASE(RDOUniformTestCheck)
{
	onCheckData<rdoRuntime::RandGeneratorUniform>
		(boost::bind(&rdoRuntime::RandGeneratorUniform::next, _1, g_from, g_to), g_fileUniformName);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------Exponential sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDOExponentialTestCreate)
{
	onGenerateData<rdoRuntime::RandGeneratorExponential>
		(boost::bind(&rdoRuntime::RandGeneratorExponential::next, _1, g_main), g_fileExponentialName);
}

BOOST_AUTO_TEST_CASE(RDOExponentialTestCheck)
{
	onCheckData<rdoRuntime::RandGeneratorExponential>
		(boost::bind(&rdoRuntime::RandGeneratorExponential::next, _1, g_main), g_fileExponentialName);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------Triangular sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDOTriangularTestCreate)
{
	onGenerateData<rdoRuntime::RandGeneratorTriangular>
		(boost::bind(&rdoRuntime::RandGeneratorTriangular::next, _1, g_from, g_top, g_to), g_fileTriangularName);
}

BOOST_AUTO_TEST_CASE(RDOTriangularTestCheck)
{
	onCheckData<rdoRuntime::RandGeneratorTriangular>
		(boost::bind(&rdoRuntime::RandGeneratorTriangular::next, _1, g_from, g_top, g_to), g_fileTriangularName);
}
// --------------------------------------------------------------------------------
*/
BOOST_AUTO_TEST_SUITE_END()