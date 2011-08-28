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

#ifndef _LIB_COMMON_RDOANIMATION_H_
#define _LIB_COMMON_RDOANIMATION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
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

	private:
		Type m_type;
	};

	//! \brief   ����
	//! \details ������ RGB-���� � ������� ������������
	STRUCT(RDOColor)
	{
		rbyte m_r;           //!< ������� ������������ �����
		rbyte m_g;           //!< ������ ������������ �����
		rbyte m_b;           //!< ����� ������������ �����
		rbool m_transparent; //!< ������� ������������

		//! \details ������ ���������� ����. �������� ������ ��������� ������������.
		RDOColor();

		//! ����������� �����
		//! \param color - ����, �� ������ �������� �������� �����
		RDOColor(CREF(RDOColor) color);

		//! ������ ����
		//! \param r           - ������� ������������ �����
		//! \param g           - ������ ������������ �����
		//! \param b           - ����� ������������ �����
		//! \param transparent - ������� ������������
		RDOColor(rbyte r, rbyte g, rbyte b, rbool transparent = false);
	};

	//! \brief   ���� ��������
	//! \details ������������ ��� �������� ������ ��������� ����� � ���� �������� ��������
	STRUCT(RDOColoredElement)
	{
		RDOColor m_background; //!< ���� ����
		RDOColor m_foreground; //!< ���� ��������� �����

		//! ����������� �����
		//! \param color - ����, �� ������ �������� �������� �����
		RDOColoredElement(CREF(RDOColoredElement) color);

		//! ������ ���� ��������
		//! \param background - ���� ����
		//! \param foreground - ���� ��������� �����
		RDOColoredElement(CREF(RDOColor) background, CREF(RDOColor) foreground);
	};

	//! \brief   �����
	//! \details ������ ������� ��������� - ����� ������� ����
	STRUCT(RDOPoint)
	{
		double m_x; //!< ���������� x
		double m_y; //!< ���������� y

		//! ����������� �����
		//! \param point - �����, �� ������ ������� �������� �����
		RDOPoint(CREF(RDOPoint) point);

		//! ������ �����
		//! \param x - ���������� x
		//! \param y - ���������� y
		RDOPoint(double x, double y);
	};

	//! ������
	STRUCT(RDOSize)
	{
		double m_width;  //!< ������
		double m_height; //!< ������

		//! \details ������ ������ ������� ��������
		RDOSize();

		//! ����������� �����
		//! \param size - ������, �� ������ ������� �������� �����
		RDOSize(CREF(RDOSize) size);

		//! ������ ������
		//! \param width  - ������
		//! \param height - ������
		RDOSize(double width, double height);
	};

	//! ��������� � ������������ �������
	STRUCT(RDOBoundedElement)
	{
		RDOPoint  m_point; //!< ���������� ��������
		RDOSize   m_size;  //!< ������ ��������

		//! ����������� �����
		//! \param rect - ������������, �� ������ ������� �������� �����
		RDOBoundedElement(CREF(RDOBoundedElement) rect);

		//! ������ ��������� � ������������ �������
		//! \param point - ���������� ��������
		//! \param size  - ������ ��������
		RDOBoundedElement(CREF(RDOPoint) point, CREF(RDOSize) size);
	};

	//! ������ ����������
	STRUCT(RDORadius)
	{
		double m_radius; //!< ������

		//! ����������� �����
		//! \param radius - ������, �� ������ ������� �������� �����
		RDORadius(CREF(RDORadius) radius);

		//! ������ ������
		//! \param radius - �������� �������
		RDORadius(double radius);
	};

//! ���������� ������ ��������
#define FRAME_ITEM(A) STRUCT_PARENT_OF(A, FrameItem)

	//! \brief   �����
	//! \details ������������ ��� ���������� ������� �������� <tt>text[x, y, ������, ������, ����_����, ����, [������������] ����������]</tt>
	FRAME_ITEM(RDOTextElement) IS  IMPLEMENTATION_OF(RDOBoundedElement)
	                           AND IMPLEMENTATION_OF(RDOColoredElement)
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
		RDOTextElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color, CREF(tstring) text, TextAlign align);
	};

	//! \brief   �������
	//! \details ������������ ��� ���������� ������� �������� <tt>line[x1, y1, x2, y2, ����]</tt>
	FRAME_ITEM(RDOLineElement)
	{
		RDOPoint  m_point1; //!< ��������� �����
		RDOPoint  m_point2; //!< �������� �����
		RDOColor  m_color;  //!< ���� �������

		//! ������ �������
		//! \param point1 - ��������� �����
		//! \param point2 - �������� �����
		//! \param color  - ���� �������
		RDOLineElement(CREF(RDOPoint) point1, CREF(RDOPoint) point2, CREF(RDOColor) color);
	};

	//! \brief   �����������
	//! \details ������������ ��� ���������� ������� �������� <tt>triang[x1, y1, x2, y2, x3, y3, ����_����, ����]</tt>
	FRAME_ITEM(RDOTriangElement) IS IMPLEMENTATION_OF(RDOColoredElement)
	{
		RDOPoint  m_point1; //!< ������ ����� ������������
		RDOPoint  m_point2; //!< ������ ����� ������������
		RDOPoint  m_point3; //!< ������ ����� ������������

		//! ������ �����������
		//! \param point1 - ������ ����� ������������
		//! \param point2 - ������ ����� ������������
		//! \param point3 - ������ ����� ������������
		//! \param color  - ���� ������������
		RDOTriangElement(CREF(RDOPoint) point1, CREF(RDOPoint) point2, CREF(RDOPoint) point3, CREF(RDOColoredElement) color);
	};

	//! \brief   �������������
	//! \details ������������ ��� ���������� ������� �������� <tt>rect[x, y, ������, ������, ����_����, ����]</tt>
	FRAME_ITEM(RDORectElement) IS  IMPLEMENTATION_OF(RDOBoundedElement)
	                           AND IMPLEMENTATION_OF(RDOColoredElement)
	{
		//! ������ �������������
		//! \param rect  - ���������� � ������ ������
		//! \param color - ���� ������
		RDORectElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color);
	};

	//! \brief   ������������� �� ����������� ������
	//! \details ������������ ��� ���������� ������� �������� <tt>r_rect[x, y, ������, ������, ����_����, ����]</tt>
	FRAME_ITEM(RDORRectElement) IS  IMPLEMENTATION_OF(RDOBoundedElement)
	                            AND IMPLEMENTATION_OF(RDOColoredElement)
	{
		//! ������ �������������
		//! \param rect  - ���������� � ������ ������
		//! \param color - ���� ������
		RDORRectElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color);
	};

	//! \brief   ����������
	//! \details ������������ ��� ���������� ������� �������� <tt>circle[x, y, ������, ����_����, ����]</tt>
	FRAME_ITEM(RDOCircleElement) IS IMPLEMENTATION_OF(RDOColoredElement)
	{
		RDOPoint  m_center; //!< ����� ����������
		RDORadius m_radius; //!< ������ ����������

		//! ������ ����������
		//! \param center - ����� ����������
		//! \param radius - ������ ����������
		//! \param color  - ���� ����������
		RDOCircleElement(CREF(RDOPoint) center, CREF(RDORadius) radius, CREF(RDOColoredElement) color);
	};

	//! \brief   ������
	//! \details ������������ ��� ���������� ������� �������� <tt>ellipse[x, y, ������, ������, ����_����, ����]</tt>
	FRAME_ITEM(RDOEllipseElement) IS  IMPLEMENTATION_OF(RDOBoundedElement)
	                              AND IMPLEMENTATION_OF(RDOColoredElement)
	{
		//! ������ ������
		//! \param rect  - ���������� � ������ ������
		//! \param color - ���� ������
		RDOEllipseElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color);
	};

	//! \brief   ��������
	//! \details ������������ ��� ���������� ������� �������� <tt>bitmap[x, y, ���_�����_�������_����� [, ���_�����_�����]]</tt>
	FRAME_ITEM(RDOBmpElement)
	{
		RDOPoint  m_point;     //!< ���������� ������ �������� ���� ��������
		tstring   m_bmp_name;  //!< ��� ����� ��������
		tstring   m_mask_name; //!< ��� ����� ����� ��������

		//! ������ ��������
		//! \param point     - ���������� ������ �������� ���� ��������
		//! \param bmp_name  - ��� ����� ��������
		//! \param mask_name - ��� ����� ����� ��������, ����� �������������
		RDOBmpElement(CREF(RDOPoint) point, CREF(tstring) bmp_name, CREF(tstring) mask_name = _T(""));

		//! \details ���������� \b true, ���� ����� �������
		rbool hasMask() const;
	};

	//! \brief   �������������� ��������
	//! \details ������������ ��� ���������� ������� �������� <tt>s_bmp[x, y, ������, ������, ���_�����_�������_����� [, ���_�����_�����]]</tt>
	FRAME_ITEM(RDOSBmpElement) IS IMPLEMENTATION_OF(RDOBoundedElement)
	{
		tstring  m_bmp_name;  //!< ��� ����� ��������
		tstring  m_mask_name; //!< ��� ����� ����� ��������

		//! ������ ��������
		//! \param rect      - ���������� � ������ ������
		//! \param bmp_name  - ��� ����� ��������
		//! \param mask_name - ��� ����� ����� ��������, ����� �������������
		RDOSBmpElement(CREF(RDOBoundedElement) rect, CREF(tstring) bmp_name, CREF(tstring) mask_name = _T(""));

		//! \details ���������� \b true, ���� ����� �������
		rbool hasMask() const;
	};

	//! \brief   �������� �������
	//! \details ������������ ��� ���������� ������� �������� <tt>active ���_������������_��������[x, y, ������, ������]</tt>
	FRAME_ITEM(RDOActiveElement) IS IMPLEMENTATION_OF(RDOBoundedElement)
	{
		tstring  m_opr_name; //!< ��� ������������ ��������

		//! ������ �������� �������
		//! \param rect      - ���������� � ������ �������
		//! \param opr_name  - ��� ������������ ��������
		RDOActiveElement(CREF(RDOBoundedElement) rect, CREF(tstring) opr_name);
	};

	//! \brief   ������ �������
	//! \details ����� �� ������������
	FRAME_ITEM(RDONullElement)
	{
		//! \details ������ ������ �������
		RDONullElement();
	};

	//! \brief   �����
	//! \details �������� ����������� ��������� ��������.
	//!          ������ $Frame, �� �������� ������ �� ��������, ������� ���������� �������, �.�. Show_if ��� ���������.
	STRUCT(RDOFrame)
	{
		typedef  std::vector<PTR(FrameItem)>  Elements; //!< ��� ���������� ��������� ��������

		RDOColor  m_bgColor;     //!< ���� ���� ������
		tstring   m_bgImageName; //!< ��� ������� ��������
		RDOSize   m_size;        //!< ������ ������
		Elements  m_elements;    //!< ������ ��������� ��������


		//! \details ������� ��� ��������
		~RDOFrame();

		//! \details ���������� \b true, ���� ������� ������� ��������
		rbool hasBgImage() const;
	};

CLOSE_RDO_ANIMATION_NAMESPACE

#include "rdo_common/rdoanimation.inl"

#endif //! _LIB_COMMON_RDOANIMATION_H_
