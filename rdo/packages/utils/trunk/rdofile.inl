/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofile.inl
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     
  \indent    4T
*/

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#ifdef COMPILER_VISUAL_STUDIO
	#include <io.h>
#endif // COMPILER_VISUAL_STUDIO

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

inline rbool File::create(CREF(tstring) name)
{
	return create(name, _T(""));
}

inline rbool File::create(CREF(tstring) name, CREF(tstring) content)
{
	boost::filesystem::fstream file(name.c_str(), std::ios::out | std::ios::binary);
	file << content.c_str() << std::endl;
	file.close();
	return true;
}

inline rbool File::exist(CREF(tstring) name)
{
	return  boost::filesystem::exists(name.c_str());
}

inline rbool File::unlink(CREF(tstring) name)
{
	return boost::filesystem::remove(name.c_str());
}

CLOSE_RDO_NAMESPACE
