/*!
  \copyright (c) RDO-Team, 2012
  \file      rdolocale.h
  \authors   ��������� ������� (lord.tiran@gmail.com)
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      20.10.2012
  \brief     ��������� ������ ��� ���
  \indent    4T
*/

#ifndef _UTILS_LOCALE_H_
#define _UTILS_LOCALE_H_

// ----------------------------------------------------------------------- PLATFORM
#include "utils/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <boost/locale.hpp>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo
{
	void setup_locale();

	namespace locale
	{
		void init();

		class generator
		{
		public:
			generator();

			std::locale get(const std::string& name);

			std::locale system();
			std::locale cp1251();
			std::locale utf8  ();

		private:
			boost::locale::generator m_generator;
		};

		generator& list();
		std::string convert(const std::string& txt, const std::locale& to, const std::locale& from = std::locale());

	} // namespace locale
} // namespace rdo

#endif // _UTILS_LOCALE_H_
