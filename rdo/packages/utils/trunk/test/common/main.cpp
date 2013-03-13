/*!
  \copyright (c) RDO-Team, 2011
  \file      main.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     Тест common-библиотеки
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <boost/regex.hpp>
#define BOOST_TEST_MODULE RDOCommon_Test
#include <boost/test/included/unit_test.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "utils/rdofile.h"
#include "utils/rdotime.h"
#include "utils/rdolocale.h"
// --------------------------------------------------------------------------------

const tstring s_testFileName("test_file");
const ruint64 s_createTestLocalTime = 129557633912040000;

BOOST_AUTO_TEST_SUITE(RDOCommon_Test)

BOOST_AUTO_TEST_CASE(RDOCommon_FileCreate)
{
	rdo::locale::init();
	BOOST_CHECK(rdo::File::create(s_testFileName));
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileExist)
{
	BOOST_CHECK(rdo::File::exist(s_testFileName));
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileReadOnly)
{
	BOOST_CHECK(!rdo::File::read_only(s_testFileName));
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileRemove)
{
	BOOST_CHECK(rdo::File::unlink(s_testFileName));
}

#ifdef OST_LINUX
BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitPathInLinux)
{
	tstring file("/rdo/run and space/files/project.smr");
	tstring dir;
	tstring name;
	tstring ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == "/rdo/run and space/files/");
	BOOST_CHECK(name == "project");
	BOOST_CHECK(ext  == ".smr");
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitByRootPathInLinux)
{
	tstring file("/project.smr");
	tstring dir;
	tstring name;
	tstring ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == "/");
	BOOST_CHECK(name == "project");
	BOOST_CHECK(ext  == ".smr");
}
#endif

#ifdef OST_WINDOWS
BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitPathInWindows)
{
	tstring file("C:/rdo/русский пробел/files/проект.smr");
	tstring dir;
	tstring name;
	tstring ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == "C:\\rdo\\русский пробел\\files\\");
	BOOST_CHECK(name == "проект");
	BOOST_CHECK(ext  == ".smr");
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitByRootPathInWindows)
{
	tstring file("C:/проект.smr");
	tstring dir;
	tstring name;
	tstring ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == "C:\\");
	BOOST_CHECK(name == "проект");
	BOOST_CHECK(ext  == ".smr");
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitPathInWindows_BackSlash)
{
	tstring file("C:\\rdo\\русский пробел\\files\\проект.smr");
	tstring dir;
	tstring name;
	tstring ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == "C:\\rdo\\русский пробел\\files\\");
	BOOST_CHECK(name == "проект");
	BOOST_CHECK(ext  == ".smr");
}

BOOST_AUTO_TEST_CASE(RDOCommon_FileSplitByRootPathInWindows_BackSlash)
{
	tstring file("C:\\проект.smr");
	tstring dir;
	tstring name;
	tstring ext;

	BOOST_CHECK(rdo::File::splitpath(file, dir, name, ext));
	BOOST_CHECK(dir  == "C:\\");
	BOOST_CHECK(name == "проект");
	BOOST_CHECK(ext  == ".smr");
}
#endif // #endif

BOOST_AUTO_TEST_CASE(RDOCommon_GetTempFile)
{
	std::set<tstring> name_set;
	for(int i = 0; i < 15; ++i)
	{
		tstring file_name = rdo::File::getTempFileName();
		BOOST_CHECK(name_set.find(file_name) == name_set.end());
		name_set.insert(file_name);
	}
}

BOOST_AUTO_TEST_CASE(RDOCommon_Time)
{
	rdo::Time timeValue = rdo::Time::local();
	BOOST_CHECK(timeValue > s_createTestLocalTime);
	std::cout << "Today:  " << timeValue.asString().c_str() << "  is not it?";
}

BOOST_AUTO_TEST_SUITE_END() // RDOCommon_Test
