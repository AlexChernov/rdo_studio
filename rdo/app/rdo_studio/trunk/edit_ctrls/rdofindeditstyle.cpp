/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdofindeditstyle.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdofindeditstyle.h"
#include "thirdparty/sci/SciLexer.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;
using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- RDOLogEditTheme
// --------------------------------------------------------------------------------
RDOFindEditTheme::RDOFindEditTheme(): RDOLogEditTheme()
{
	keywordColor = RGB( 0x00, 0x00, 0x00 );
	keywordStyle = RDOStyleFont::BOLD;
}

RDOFindEditTheme::~RDOFindEditTheme()
{
}

RDOFindEditTheme& RDOFindEditTheme::operator =( const RDOFindEditTheme& theme )
{
	RDOLogEditTheme::operator=( theme );

	keywordColor = theme.keywordColor;
	keywordStyle = theme.keywordStyle;

	return *this;
}

rbool RDOFindEditTheme::operator ==( const RDOFindEditTheme& theme ) const
{
	rbool flag = RDOLogEditTheme::operator==( theme );

	if ( flag ) flag &= keywordColor == theme.keywordColor &&
	                    keywordStyle == theme.keywordStyle;
	return flag;
}

rbool RDOFindEditTheme::operator !=( const RDOFindEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOFindEditTheme::load( tstring regPath )
{
	RDOLogEditTheme::load( regPath );

	regPath += "theme";
	keywordColor = AfxGetApp()->GetProfileInt( regPath.c_str(), "keywordColor", keywordColor );
	keywordStyle = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "keywordStyle", keywordStyle ));
}

void RDOFindEditTheme::save( tstring regPath ) const
{
	RDOLogEditTheme::save( regPath );

	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "keywordColor", keywordColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "keywordStyle", keywordStyle );
}

rbool RDOFindEditTheme::styleDefault( const int styleType ) const
{
	return styleType == SCE_FIND_DEFAULT;
}

rbool RDOFindEditTheme::styleUsing( const int styleType ) const
{
	return styleType == SCE_FIND_DEFAULT || styleType == SCE_FIND_KEYWORD;
}

rbool RDOFindEditTheme::styleBold( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_FIND_DEFAULT: return defaultStyle & RDOStyleFont::BOLD ? true : false;
		case SCE_FIND_KEYWORD: return keywordStyle & RDOStyleFont::BOLD ? true : false;
	}
	return false;
}

rbool RDOFindEditTheme::styleItalic( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_FIND_DEFAULT: return defaultStyle & RDOStyleFont::ITALIC ? true : false;
		case SCE_FIND_KEYWORD: return keywordStyle & RDOStyleFont::ITALIC ? true : false;
	}
	return false;
}

tstring RDOFindEditTheme::styleFGColorToHEX( const int styleType ) const
{
	switch ( styleType ) {
		case SCE_FIND_DEFAULT: return colorToHEX( defaultColor );
		case SCE_FIND_KEYWORD: return colorToHEX( keywordColor );
	}
	return RDOLogEditTheme::styleFGColorToHEX( styleType );
}

RDOFindEditTheme RDOFindEditTheme::getDefaultTheme()
{
	RDOFindEditTheme theme;
	return theme;
}

RDOFindEditTheme RDOFindEditTheme::getClassicTheme()
{
	RDOFindEditTheme theme;
	*static_cast<RDOLogEditTheme*>(&theme) = RDOLogEditTheme::getClassicTheme();

	theme.keywordColor = RGB( 0xFF, 0xFF, 0xFF );
	theme.keywordStyle = RDOStyleFont::NONE;

	return theme;
}

RDOFindEditTheme RDOFindEditTheme::getTwilightTheme()
{
	RDOFindEditTheme theme;
	*static_cast<RDOLogEditTheme*>(&theme) = RDOLogEditTheme::getTwilightTheme();

	theme.keywordColor = RGB( 0x00, 0xFF, 0xFF );
	theme.keywordStyle = RDOStyleFont::BOLD;

	return theme;
}

RDOFindEditTheme RDOFindEditTheme::getOceanTheme()
{
	RDOFindEditTheme theme;
	*static_cast<RDOLogEditTheme*>(&theme) = RDOLogEditTheme::getOceanTheme();

	theme.keywordColor = RGB( 0x00, 0x00, 0x00 );
	theme.keywordStyle = RDOStyleFont::BOLD;

	return theme;
}

// --------------------------------------------------------------------------------
// -------------------- RDOFindEditStyle
// --------------------------------------------------------------------------------
RDOFindEditStyle::RDOFindEditStyle(): RDOLogEditStyle()
{
}

RDOFindEditStyle::~RDOFindEditStyle()
{
}

void RDOFindEditStyle::initTheme()
{
	theme = new RDOFindEditTheme;
}

RDOFindEditStyle& RDOFindEditStyle::operator =( const RDOFindEditStyle& style )
{
	RDOLogEditStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<RDOFindEditTheme*>(theme) = *static_cast<RDOFindEditTheme*>(style.theme);

	return *this;
}

rbool RDOFindEditStyle::operator ==( const RDOFindEditStyle& style ) const
{
	rbool flag = RDOLogEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<RDOFindEditTheme*>(theme) == *static_cast<RDOFindEditTheme*>(style.theme);
	return flag;
}

rbool RDOFindEditStyle::operator !=( const RDOFindEditStyle& style ) const
{
	return !(*this == style);
}
