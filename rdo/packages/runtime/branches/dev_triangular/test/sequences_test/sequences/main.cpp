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
#include <list>
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdofile.h"
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------

typedef std::list<double> Container;
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

// --------------------------------------------------------------------------------
// -------Templates
// --------------------------------------------------------------------------------
template <class T, class F, class contstr>
void onGenerateData(F binder, contstr g_fileName)
{
	if (rdo::File::exist(g_fileName.c_str()))
		return;

	T sequence(g_seed);
	Container test;

	for (ruint i = 0; i < g_count; ++i)
	{
		test.push_back(binder.operator()(&sequence));
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

	Container test;
	T sequence(g_seed);
	for (ruint i = 0; i < g_count; ++i)
	{
		test.push_back(binder.operator()(&sequence));
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
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(RDOSequencesTest)

// --------------------------------------------------------------------------------
// -------Normal sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDONormalTestCreate)
{
	onGenerateData<rdoRuntime::RandGeneratorNormal>
		(boost::bind(&rdoRuntime::RandGeneratorNormal::next, _1, g_main, g_var), g_fileNormalName);
}

BOOST_AUTO_TEST_CASE(RDONormalTestCheck)
{
	onCheckData<rdoRuntime::RandGeneratorNormal>
		(boost::bind(&rdoRuntime::RandGeneratorNormal::next, _1, g_main, g_var), g_fileNormalName);
}
// --------------------------------------------------------------------------------

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

BOOST_AUTO_TEST_SUITE_END()