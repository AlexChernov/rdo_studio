/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofile.cpp
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      07.11.2020
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#include <Windows.h>
#endif // COMPILER_VISUAL_STUDIO
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdofile.h"
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

rbool File::read_only(CREF(tstring) name)
{
#ifdef COMPILER_VISUAL_STUDIO
	return _access(name.c_str(), 04) == 0 && _access(name.c_str(), 06) == -1;
#endif  // COMPILER_VISUAL_STUDIO
#ifdef COMPILER_GCC
	return access(name.c_str(), R_OK) == 0 && access(name.c_str(), W_OK) == -1;
#endif // COMPILER_GCC
}

rbool File::splitpath(CREF(tstring) name, REF(tstring) fileDir, REF(tstring) fileName, REF(tstring) fileExt)
{
	boost::filesystem::path from(name);
	fileDir = from.parent_path().string();
	fileName = from.stem().string();
	fileExt  = from.extension().string();
	return true;
}

tstring File::getTempFileName()
{
#ifdef COMPILER_VISUAL_STUDIO
	const ruint BUFSIZE = 4096;
	char lpPathBuffer[BUFSIZE];

	if (::GetTempPath(BUFSIZE, lpPathBuffer) == 0)
	{
		return tstring();
	}
	char szTempName[MAX_PATH];
	if (::GetTempFileName(lpPathBuffer, NULL, 0, szTempName) == 0)
	{
		return tstring();
	}
	return szTempName;
#endif // COMPILER_VISUAL_STUDIO
#ifdef COMPILER_GCC
	//! @todo check random
	boost::uuids::random_generator random_gen;
	tstring tempFileName = tstring(_T("/tmp/rdo_temp_file_num_")) + boost::uuids::to_string(random_gen());
	create(tempFileName);
	return tempFileName;
#endif // COMPILER_GCC
}

rbool File::trimLeft(CREF(tstring) name)
{
	tstring tempFileName = getTempFileName();

	std::ofstream tempStream(tempFileName.c_str(), std::ios::trunc | std::ios::binary);
	std::ifstream fileStream(name.c_str(), std::ios::binary);

	if (!tempStream.good() || !fileStream.good())
	{
		return false;
	}

	rbool empty = true;
	while (!fileStream.eof())
	{
		char byte;
		fileStream.get(byte);

		if (empty)
		{
			if (byte != _T(' ') && byte != _T('\t') && byte != _T('\n') && byte != _T('\r'))
			{
				empty = false;
			}
		}

		if (!empty)
		{
			tempStream.write(&byte, 1);
		}
	}

	tempStream.close();
	fileStream.close();

	boost::filesystem::path from(tempFileName);
	boost::filesystem::path to  (name);

	try
	{
		if (!boost::filesystem::remove(to))
		{
			return false;
		}
		boost::filesystem::rename(from, to);
	}
	catch (CREF(boost::system::error_code))
	{
		return false;
	}

	return true;
}

CLOSE_RDO_NAMESPACE
