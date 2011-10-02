/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   �������� ����� (impus@hotbox.ru)
  \date      2.10.2011
  \brief     ���� ����������� ������
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#define BOOST_TEST_MODULE RDOTriangularTest
#include <iostream>
#include <fstream>
#include <list>
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdofile.h"
#include "simulator/runtime/rdo_random_distribution.h"
// --------------------------------------------------------------------------------

const long int g_seed     = 123456789;                  //!< ���� ����������
const tstring  g_fileName = _T("data_normal.txt");  //!< ���� ������
const ruint    g_count    = 100000;                     //!< ���������� ������������ ������
const double   g_from     = 1.0;                        //!< �������� ������
const double   g_to       = 7.0;                        //!< �������� ������

typedef std::list <double> Container;

BOOST_AUTO_TEST_SUITE(RDOTriangularTest)

BOOST_AUTO_TEST_CASE(RDOTriangularTestCreate)
{
}