/*!
  \copyright (c) RDO-Team, 2012-2012
  \file      bitmap.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      15.07.2012
  \brief     �������������� ������� ������ � ����������
  \indent    4T
*/

#ifndef _UI_QT_BITMAP_H_
#define _UI_QT_BITMAP_H_

// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <QtGui/qpixmap.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdotypes.h"
// --------------------------------------------------------------------------------

namespace rdo
{
	namespace gui
	{
		class Bitmap
		{
		public:
			static QPixmap transparent(CREF(QPixmap) bitmap,      CREF(QPixmap) mask);
			static rbool   transparent( REF(QImage)  inOutBitmap, CREF(QImage)  mask);
		};

		typedef  std::map<tstring, QPixmap>  BitmapList;

	} // namespace gui
} // namespace rdo

#endif // _UI_GDIPLUS_BITMAP_H_
