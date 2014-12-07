#ifndef _UI_QT_ANIMATION_AREA_H_
#define _UI_QT_ANIMATION_AREA_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <map>
#include <QRect>
#include <QString>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
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
			typedef  std::map<QString, Area>  AreaList;

		} // namespace animation
	} // namespace gui
} // namespace rdo

#endif // _UI_QT_ANIMATION_AREA_H_
