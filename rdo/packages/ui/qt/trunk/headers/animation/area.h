/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      ui/qt/headers/animation/area.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      15.07.2012
  \brief     �������� ������� ���� ��������
  \indent    4T
*/

#ifndef _UI_QT_ANIMATION_AREA_H_
#define _UI_QT_ANIMATION_AREA_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <QtCore/qrect.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace gui
	{
		namespace animation
		{
			struct Area
			{
				QRect  m_rect;
			};
			typedef  std::map<tstring, Area>  AreaList;

		} // namespace animation
	} // namespace gui
} // namespace rdo

#endif // _UI_QT_ANIMATION_AREA_H_
