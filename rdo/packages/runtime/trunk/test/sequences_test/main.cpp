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
#include "utils/platform.h"
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
	#define  __SCANF  sscanf_s
#else  // not COMPILER_VISUAL_STUDIO
	#define  __SCANF  sscanf
#endif // COMPILER_VISUAL_STUDIO

typedef std::vector<double> Container;
typedef std::vector<ruint>  ContainerInt;
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
const ruint    g_countOfR          = 39;                         //!< ����� ��������
const double   pi                     = 3.141592653;                //!< ��������������� ���������
const double   g_ksiEtalon            = 50.9985;                    //!< ��������� ��������. 95% ����������� ����, ��� ��� ������������� ��� ����� ����� �������������

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
	test.reserve(g_count);

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

	Container test;
	test.reserve(g_count);
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
		BOOST_CHECK(__SCANF(str.c_str(), _T("%lf"), &val) == 1);
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
double  area (F binder, double n, double m)
{
	double k = 1;
	double S1 = 1;
	double S2 = 0;
	ruint t = 10;
	while (fabs(S1-S2)/S1 > 0.01)
	{
		S2 = S1;
		S1 = 0;
		for (ruint g = 0; g < t + 1; ++g)
		{
			if ((g == 0) || (g == t - 1))
				k = 0.5;
			S1 += k*(binder.operator()(n + g*(m-n)/t));
			k = 1;
		}
		S1 *= (m-n)/t;
		t  *= 10;
	}
	return S1;
}

template <class T, class G, class F, class S>
void onCheckKsi(F binder, S binderSeq, double left, double right)
{
	Container x(g_countOfR + 1);
	//x.reserve(g_countOfR + 1);
	double elem = (right-left)/(g_countOfR*1.0);	//���������� ����� ������� �� ������

	for (ruint i = 0; i < g_countOfR + 1; ++i)
	{
		x[i] = left + elem*i;
	}

	Container vb(g_countOfExamples);				//��������� ��� �������� �������
	//vb.reserve(g_countOfExamples);

	G sequence(g_seed);								//�������
	for (ruint i = 0; i < g_countOfExamples; ++i)
	{
		vb[i] = binderSeq.operator()(&sequence);
	}

	Container f_vb(g_countOfR);						//��������� ��� ��������� ���������� ��������� �� ��������

	for(ruint i = 0; i < g_countOfR; ++i)			//���������� ���������� ��������� �� ��������
	{
		ruint freq = 0;
		for(ruint k = 0; k < g_countOfExamples; ++k)
		{
			if((vb[k] > x[i]) & (vb[k] <= x[i+1]))
			{
				++freq;
			}
		}
		f_vb[i] = freq;
	}

	ContainerInt F_etalon(g_countOfR);
	//Fi.reserve(g_countOfR);

	for (ruint i = 0; i < g_countOfR; ++i)
	{
		F_etalon[i] = g_countOfExamples*area<T>(binder, x[i], x[i+1]);
	}

	double sum = 0;
	double summ= 0;
	for(ruint i = 0; i < g_countOfR; ++i)
	{
		sum += F_etalon[i];
		summ += f_vb[i];
	}

	double ksi = 0;
	for(ruint i = 0; i < g_countOfR; ++i)
	{
		ksi += (f_vb[i] - F_etalon[i])*(f_vb[i] - F_etalon[i])/F_etalon[i];
	}

	BOOST_CHECK(ksi <= g_ksiEtalon);
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
		return 1/(sqrt(2*pi)*m_var*exp((x - m_main)*(x - m_main)/(2*m_var*m_var))); //������� ��������� ������������� �����������. �� ����������� ��� ������. ����� ����� ��� 3 �����.
	}

private:
	double m_main;
	double m_var;
};

class SequenceExponential
{
public:
	SequenceExponential(double main)
		: m_main(main)
	{}

	double get(double x) const
	{
		return 1/(m_main*exp(x/m_main)); 
	}

private:
	double m_main;
};

class SequenceUniform
{
public:
	SequenceUniform(double min, double max)
		: m_min(min)
		, m_max(max)
	{}

	double get(double x) const
	{
		return 1/(m_max-m_min); 
	}

private:
	double m_min;
	double m_max;
};

class SequenceTriangular
{
public:
	SequenceTriangular(double min, double top, double max)
		: m_min(min-top)
		, m_top(top)
		, m_max(max-top)
	{}

	double get(double x) const
	{
		x -= m_top;
		double temp;
		if (x < 0)
		{
			temp = -2*x/((m_max - m_min)*m_min) + 2/(m_max - m_min);
		}
		else
		{
			temp = -2*x/((m_max - m_min)*m_max) + 2/(m_max - m_min);
		}
		return temp; 
	}

private:
	double m_min;
	double m_top;
	double m_max;
};

BOOST_AUTO_TEST_SUITE(RDOSequencesTest)

// --------------------------------------------------------------------------------
// -------Normal sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDONormalTestCreate)
{
	//onGenerateData<rdoRuntime::RandGeneratorNormal>
	//	(boost::bind(&rdoRuntime::RandGeneratorNormal::next, _1, g_main, g_var), g_fileNormalName);
}

BOOST_AUTO_TEST_CASE(RDONormalTestCheck)
{
	//onCheckData<rdoRuntime::RandGeneratorNormal>
	//	(boost::bind(&rdoRuntime::RandGeneratorNormal::next, _1, g_main, g_var), g_fileNormalName);

	SequenceNormal normal(g_main, g_var);
	onCheckKsi<SequenceNormal, rdoRuntime::RandGeneratorNormal>
		(boost::bind(&SequenceNormal::get, normal, _1),
		boost::bind(&rdoRuntime::RandGeneratorNormal::next, _1, g_main, g_var),
		g_main-3.2*sqrt(g_var),
		g_main+3.2*sqrt(g_var));
}

// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------Uniform sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDOUniformTestCreate)
{
	//onGenerateData<rdoRuntime::RandGeneratorUniform>
	//	(boost::bind(&rdoRuntime::RandGeneratorUniform::next, _1, g_from, g_to), g_fileUniformName);
}

BOOST_AUTO_TEST_CASE(RDOUniformTestCheck)
{
	//onCheckData<rdoRuntime::RandGeneratorUniform>
	//	(boost::bind(&rdoRuntime::RandGeneratorUniform::next, _1, g_from, g_to), g_fileUniformName);

	SequenceUniform uniform(g_from, g_to);
	onCheckKsi<SequenceUniform, rdoRuntime::RandGeneratorUniform>
		(boost::bind(&SequenceUniform::get, uniform, _1),
		boost::bind(&rdoRuntime::RandGeneratorUniform::next, _1, g_from, g_to),
		g_from,
		g_to);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------Exponential sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDOExponentialTestCreate)
{
	//onGenerateData<rdoRuntime::RandGeneratorExponential>
	//	(boost::bind(&rdoRuntime::RandGeneratorExponential::next, _1, g_main), g_fileExponentialName);
}

BOOST_AUTO_TEST_CASE(RDOExponentialTestCheck)
{
	//onCheckData<rdoRuntime::RandGeneratorExponential>
	//	(boost::bind(&rdoRuntime::RandGeneratorExponential::next, _1, g_main), g_fileExponentialName);

	SequenceExponential exponential(g_main);
	onCheckKsi<SequenceExponential, rdoRuntime::RandGeneratorExponential>
		(boost::bind(&SequenceExponential::get, exponential, _1),
		boost::bind(&rdoRuntime::RandGeneratorExponential::next, _1, g_main),
		0,
		5*g_main);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------Triangular sequence
// --------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(RDOTriangularTestCreate)
{
	//onGenerateData<rdoRuntime::RandGeneratorTriangular>
	//	(boost::bind(&rdoRuntime::RandGeneratorTriangular::next, _1, g_from, g_top, g_to), g_fileTriangularName);
}

BOOST_AUTO_TEST_CASE(RDOTriangularTestCheck)
{
	//onCheckData<rdoRuntime::RandGeneratorTriangular>
	//	(boost::bind(&rdoRuntime::RandGeneratorTriangular::next, _1, g_from, g_top, g_to), g_fileTriangularName);

	SequenceTriangular triangular(g_from, g_top, g_to);
	onCheckKsi<SequenceTriangular, rdoRuntime::RandGeneratorTriangular>
		(boost::bind(&SequenceTriangular::get, triangular, _1),
		boost::bind(&rdoRuntime::RandGeneratorTriangular::next, _1, g_from, g_top, g_to),
		g_from,
		g_to);
}
// --------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
