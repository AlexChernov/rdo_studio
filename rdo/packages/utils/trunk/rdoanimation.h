/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoanimation.h
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ������ ������ (kopninandrey@gmail.com)
  \date      13.06.2009
  \brief     �������� �������� ��������, ������� ���������� �� ���������� � GUI ��� ���������
  \indent    4T
*/

#ifndef _UTILS_RDOANIMATION_H_
#define _UTILS_RDOANIMATION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_ANIMATION_NAMESPACE

	//! ������� ����� ��������� ��������
	STRUCT(FrameItem)
	{
		//! \details ��� ��������
		enum Type
		{
			FIT_NULL   , //!< ������
			FIT_TEXT   , //!< �����
			FIT_LINE   , //!< �����
			FIT_TRIANG , //!< �����������
			FIT_RECT   , //!< �������������
			FIT_R_RECT , //!< ������������� �� ����������� ������
			FIT_CIRCLE , //!< ����������
			FIT_ELLIPSE, //!< ������
			FIT_BMP    , //!< ��������
			FIT_S_BMP  , //!< ����������������� ��������
			FIT_ACTIVE   //!< ������� ����������
		};

		//! \param type - ��� ��������
		FrameItem(Type type);

		//! \details ���������� ��� ��������
		Type getType() const;

		rbool operator== (CREF(FrameItem) item) const
		{
			return m_type == item.m_type;
		}

	private:
		Type m_type;
	};

	//! \brief   ����
	//! \details ������ RGB-���� � ������� ������������
	STRUCT(Color)
	{
		rbyte m_r;           //!< ������� ������������ �����
		rbyte m_g;           //!< ������ ������������ �����
		rbyte m_b;           //!< ����� ������������ �����
		rbool m_transparent; //!< ������� ������������

		//! \details ������ ���������� ����. �������� ������ ��������� ������������.
		Color();

		//! ����������� �����
		//! \param color - ����, �� ������ �������� �������� �����
		Color(CREF(Color) color);

		//! ������ ����
		//! \param r           - ������� ������������ �����
		//! \param g           - ������ ������������ �����
		//! \param b           - ����� ������������ �����
		//! \param transparent - ������� ������������
		Color(rbyte r, rbyte g, rbyte b, rbool transparent = false);

		rbool operator== (CREF(Color) color) const
		{
			return m_r == color.m_r && m_g == color.m_g && m_b == color.m_b && m_transparent == color.m_transparent;
		}
	};

	//! \brief   ���� ��������
	//! \details ������������ ��� �������� ������ ��������� ����� � ���� �������� ��������
	STRUCT(ColoredElement)
	{
		Color m_background; //!< ���� ����
		Color m_foreground; //!< ���� ��������� �����

		//! ����������� �����
		//! \param color - ����, �� ������ �������� �������� �����
		ColoredElement(CREF(ColoredElement) color);

		//! ������ ���� ��������
		//! \param background - ���� ����
		//! \param foreground - ���� ��������� �����
		ColoredElement(CREF(Color) background, CREF(Color) foreground);

		rbool operator== (CREF(ColoredElement) element) const
		{
			return m_background == element.m_background && m_foreground == element.m_foreground;
		}
	};

	//! \brief   �����
	//! \details ������ ������� ��������� - ����� ������� ����
	STRUCT(Point)
	{
		double m_x; //!< ���������� x
		double m_y; //!< ���������� y

		//! ����������� �����
		//! \param point - �����, �� ������ ������� �������� �����
		Point(CREF(Point) point);

		//! ������ �����
		//! \param x - ���������� x
		//! \param y - ���������� y
		Point(double x, double y);

		rbool operator== (CREF(Point) point) const
		{
			return m_x == point.m_x && m_y == point.m_y;
		}
	};

	//! ������
	STRUCT(Size)
	{
		double m_width;  //!< ������
		double m_height; //!< ������

		//! \details ������ ������ ������� ��������
		Size();

		//! ����������� �����
		//! \param size - ������, �� ������ ������� �������� �����
		Size(CREF(Size) size);

		//! ������ ������
		//! \param width  - ������
		//! \param height - ������
		Size(double width, double height);

		rbool operator== (CREF(Size) size) const
		{
			return m_width == size.m_width && m_height == size.m_height;
		}
	};

	//! ��������� � ������������ �������
	STRUCT(BoundedElement)
	{
		Point  m_point; //!< ���������� ��������
		Size   m_size;  //!< ������ ��������

		//! ����������� �����
		//! \param rect - ������������, �� ������ ������� �������� �����
		BoundedElement(CREF(BoundedElement) rect);

		//! ������ ��������� � ������������ �������
		//! \param point - ���������� ��������
		//! \param size  - ������ ��������
		BoundedElement(CREF(Point) point, CREF(Size) size);

		rbool operator== (CREF(BoundedElement) element) const
		{
			return m_point == element.m_point && m_size == element.m_size;
		}
	};

	//! ������ ����������
	STRUCT(Radius)
	{
		double m_radius; //!< ������

		//! ����������� �����
		//! \param radius - ������, �� ������ ������� �������� �����
		Radius(CREF(Radius) radius);

		//! ������ ������
		//! \param radius - �������� �������
		Radius(double radius);

		rbool operator== (CREF(Radius) radius) const
		{
			return m_radius == radius.m_radius;
		}
	};

//! ���������� ������ ��������
#define FRAME_ITEM(A) STRUCT_PARENT_OF(A, FrameItem)

	//! \brief   �����
	//! \details ������������ ��� ���������� ������� �������� <tt>text[x, y, ������, ������, ����_����, ����, [������������] ����������]</tt>
	FRAME_ITEM(TextElement) IS  IMPLEMENTATION_OF(BoundedElement)
	                        AND IMPLEMENTATION_OF(ColoredElement)
	{
		//! \details ��� ������������ ������
		enum TextAlign
		{
			TETA_LEFT,   //!< �� ������ ����
			TETA_RIGHT,  //!< �� ������� ����
			TETA_CENTER  //!< �� ������
		};

		tstring    m_text;  //!< ������ ������
		TextAlign  m_align; //!< ��� ������������

		//! ������ ������ ������
		//! \param rect  - ������� ������
		//! \param color - ���� ������
		//! \param text  - ������
		//! \param align - ������������
		TextElement(CREF(BoundedElement) rect, CREF(ColoredElement) color, CREF(tstring) text, TextAlign align);
	};

	//! \brief   �������
	//! \details ������������ ��� ���������� ������� �������� <tt>line[x1, y1, x2, y2, ����]</tt>
	FRAME_ITEM(LineElement)
	{
		Point  m_point1; //!< ��������� �����
		Point  m_point2; //!< �������� �����
		Color  m_color;  //!< ���� �������

		//! ������ �������
		//! \param point1 - ��������� �����
		//! \param point2 - �������� �����
		//! \param color  - ���� �������
		LineElement(CREF(Point) point1, CREF(Point) point2, CREF(Color) color);
	
		rbool operator== (CREF(LineElement) element) const
		{
			return m_point1 == element.m_point1 && m_point2 == element.m_point2 && m_color == element.m_color;
		}
	};

	//! \brief   �����������
	//! \details ������������ ��� ���������� ������� �������� <tt>triang[x1, y1, x2, y2, x3, y3, ����_����, ����]</tt>
	FRAME_ITEM(TriangElement) IS IMPLEMENTATION_OF(ColoredElement)
	{
		Point  m_point1; //!< ������ ����� ������������
		Point  m_point2; //!< ������ ����� ������������
		Point  m_point3; //!< ������ ����� ������������

		//! ������ �����������
		//! \param point1 - ������ ����� ������������
		//! \param point2 - ������ ����� ������������
		//! \param point3 - ������ ����� ������������
		//! \param color  - ���� ������������
		TriangElement(CREF(Point) point1, CREF(Point) point2, CREF(Point) point3, CREF(ColoredElement) color);
	};

	//! \brief   �������������
	//! \details ������������ ��� ���������� ������� �������� <tt>rect[x, y, ������, ������, ����_����, ����]</tt>
	FRAME_ITEM(RectElement) IS  IMPLEMENTATION_OF(BoundedElement)
	                        AND IMPLEMENTATION_OF(ColoredElement)
	{
		//! ������ �������������
		//! \param rect  - ���������� � ������ ������
		//! \param color - ���� ������
		RectElement(CREF(BoundedElement) rect, CREF(ColoredElement) color);
	};

	//! \brief   ������������� �� ����������� ������
	//! \details ������������ ��� ���������� ������� �������� <tt>r_rect[x, y, ������, ������, ����_����, ����]</tt>
	FRAME_ITEM(RoundRectElement) IS  IMPLEMENTATION_OF(BoundedElement)
	                             AND IMPLEMENTATION_OF(ColoredElement)
	{
		//! ������ �������������
		//! \param rect  - ���������� � ������ ������
		//! \param color - ���� ������
		RoundRectElement(CREF(BoundedElement) rect, CREF(ColoredElement) color);
	};

	//! \brief   ����������
	//! \details ������������ ��� ���������� ������� �������� <tt>circle[x, y, ������, ����_����, ����]</tt>
	FRAME_ITEM(CircleElement) IS IMPLEMENTATION_OF(ColoredElement)
	{
		Point  m_center; //!< ����� ����������
		Radius m_radius; //!< ������ ����������

		//! ������ ����������
		//! \param center - ����� ����������
		//! \param radius - ������ ����������
		//! \param color  - ���� ����������
		CircleElement(CREF(Point) center, CREF(Radius) radius, CREF(ColoredElement) color);
	};

	//! \brief   ������
	//! \details ������������ ��� ���������� ������� �������� <tt>ellipse[x, y, ������, ������, ����_����, ����]</tt>
	FRAME_ITEM(EllipseElement) IS  IMPLEMENTATION_OF(BoundedElement)
	                           AND IMPLEMENTATION_OF(ColoredElement)
	{
		//! ������ ������
		//! \param rect  - ���������� � ������ ������
		//! \param color - ���� ������
		EllipseElement(CREF(BoundedElement) rect, CREF(ColoredElement) color);
	};

	//! \brief   ��������
	//! \details ������������ ��� ���������� ������� �������� <tt>bitmap[x, y, ���_�����_�������_����� [, ���_�����_�����]]</tt>
	FRAME_ITEM(BmpElement)
	{
		Point    m_point;     //!< ���������� ������ �������� ���� ��������
		tstring  m_bmp_name;  //!< ��� ����� ��������
		tstring  m_mask_name; //!< ��� ����� ����� ��������

		//! ������ ��������
		//! \param point     - ���������� ������ �������� ���� ��������
		//! \param bmp_name  - ��� ����� ��������
		//! \param mask_name - ��� ����� ����� ��������, ����� �������������
		BmpElement(CREF(Point) point, CREF(tstring) bmp_name, CREF(tstring) mask_name = _T(""));

		//! \details ���������� \b true, ���� ����� �������
		rbool hasMask() const;
	};

	//! \brief   �������������� ��������
	//! \details ������������ ��� ���������� ������� �������� <tt>s_bmp[x, y, ������, ������, ���_�����_�������_����� [, ���_�����_�����]]</tt>
	FRAME_ITEM(ScaledBmpElement) IS IMPLEMENTATION_OF(BoundedElement)
	{
		tstring  m_bmp_name;  //!< ��� ����� ��������
		tstring  m_mask_name; //!< ��� ����� ����� ��������

		//! ������ ��������
		//! \param rect      - ���������� � ������ ������
		//! \param bmp_name  - ��� ����� ��������
		//! \param mask_name - ��� ����� ����� ��������, ����� �������������
		ScaledBmpElement(CREF(BoundedElement) rect, CREF(tstring) bmp_name, CREF(tstring) mask_name = _T(""));

		//! \details ���������� \b true, ���� ����� �������
		rbool hasMask() const;
	};

	//! \brief   �������� �������
	//! \details ������������ ��� ���������� ������� �������� <tt>active ���_������������_��������[x, y, ������, ������]</tt>
	FRAME_ITEM(ActiveElement) IS IMPLEMENTATION_OF(BoundedElement)
	{
		tstring  m_opr_name; //!< ��� ������������ ��������

		//! ������ �������� �������
		//! \param rect      - ���������� � ������ �������
		//! \param opr_name  - ��� ������������ ��������
		ActiveElement(CREF(BoundedElement) rect, CREF(tstring) opr_name);
	};

	//! \brief   ������ �������
	//! \details ����� �� ������������
	FRAME_ITEM(NullElement)
	{
		//! \details ������ ������ �������
		NullElement();
	};

	//! \brief   �����
	//! \details �������� ����������� ��������� ��������.
	//!          ������ $Frame, �� �������� ������ �� ��������, ������� ���������� �������, �.�. Show_if ��� ���������.
	STRUCT(Frame)
	{
		typedef  std::vector<PTR(FrameItem)>  Elements; //!< ��� ���������� ��������� ��������

		Color     m_bgColor;     //!< ���� ���� ������
		tstring   m_bgImageName; //!< ��� ������� ��������
		Size      m_size;        //!< ������ ������
		Elements  m_elements;    //!< ������ ��������� ��������

		//! \details ������� ��� ��������
		~Frame();

		//! \details ���������� \b true, ���� ������� ������� ��������
		rbool hasBgImage() const;
	};

CLOSE_RDO_ANIMATION_NAMESPACE

#include "utils/rdoanimation.inl"

#endif // _UTILS_RDOANIMATION_H_
