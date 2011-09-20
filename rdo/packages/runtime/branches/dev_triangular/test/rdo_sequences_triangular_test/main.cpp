/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \authors   �������� ����� (impus@hotbox.ru)
  \date      12.09.2011
  \brief     ���� ������������ ������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOTriangularTest
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <numeric>
#include <map>
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
#include <boost/assign/std/map.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/map.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------
using namespace rdoRuntime;

BOOST_AUTO_TEST_CASE(RDOTriangularTest)
{
	RandGeneratorTriangular triang(123456789);				//������ ������, ��������� ����� �� ������������ ������

	typedef std::map<int, double> Container;				//��� ����� �������� �������� ���������� map
	Container m_base, m_forTest;							//������ ��� ����������

	std::ofstream  testFile("Test.txt", std::ios::binary);	//�������� ����, ������� ����� ������ ���������� ������.

	for (int i = 0; i < 100000; i++)						//��������� ���������� �������, ������������� ����������� ��������
	{
		std::pair<Container::iterator, bool> result =
			m_forTest.insert(Container::value_type(i, triang.next(1,5,7)));	//� ���������� �������� ������� ������ (���������� ����� � ����������� �����)
	}
	
	
	
	//���� ���� ������������� ������������ ��� ������ ���������� �����. ������ � ��������. ������� �� ����, ���
	//�������� ������ � ����
/*
	Container::const_iterator it = m_forTest.begin();
	while (it != m_forTest.end())
	{
	//char aaa[] = std::cout << it->first << ";" << it->second <<std::endl;
		testFile.write(aaa,sizeof(aaa));
	}
*/
	testFile.close();
	int i = 1;
}


