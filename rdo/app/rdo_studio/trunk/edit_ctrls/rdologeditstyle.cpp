/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologeditstyle.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdologeditstyle.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoEditCtrl;

// --------------------------------------------------------------------------------
// -------------------- LogEditTheme
// --------------------------------------------------------------------------------
LogEditTheme::LogEditTheme(): RDOBaseEditTheme()
{
	selectLineBgColor = RGB( 0x00, 0xC0, 0xEF );
}

LogEditTheme::~LogEditTheme()
{
}

LogEditTheme& LogEditTheme::operator =( const LogEditTheme& theme )
{
	RDOBaseEditTheme::operator=( theme );
	selectLineBgColor = theme.selectLineBgColor;

	return *this;
}

rbool LogEditTheme::operator ==( const LogEditTheme& theme ) const
{
	rbool flag = RDOBaseEditTheme::operator==( theme );
	if ( flag )	flag &= selectLineBgColor == theme.selectLineBgColor ? true : false;
	return flag;
}

rbool LogEditTheme::operator !=( const LogEditTheme& theme ) const
{
	return !(*this == theme);
}

void LogEditTheme::load( tstring regPath )
{
	RDOBaseEditTheme::load( regPath );

	regPath += "theme";
	selectLineBgColor = AfxGetApp()->GetProfileInt( regPath.c_str(), "selectLineBgColor", selectLineBgColor );
}

void LogEditTheme::save( tstring regPath ) const
{
	RDOBaseEditTheme::save( regPath );

	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "selectLineBgColor", selectLineBgColor );
}

LogEditTheme LogEditTheme::getDefaultTheme()
{
	LogEditTheme theme;
	return theme;
}

LogEditTheme LogEditTheme::getClassicTheme()
{
	LogEditTheme theme;
	*static_cast<RDOBaseEditTheme*>(&theme) = RDOBaseEditTheme::getClassicTheme();

	theme.selectLineBgColor = RGB( 0x00, 0xC0, 0xEF );

	return theme;
}

LogEditTheme LogEditTheme::getTwilightTheme()
{
	LogEditTheme theme;
	*static_cast<RDOBaseEditTheme*>(&theme) = RDOBaseEditTheme::getTwilightTheme();

	theme.selectLineBgColor = RGB( 0x00, 0x96, 0xBB );

	return theme;
}

LogEditTheme LogEditTheme::getOceanTheme()
{
	LogEditTheme theme;
	*static_cast<RDOBaseEditTheme*>(&theme) = RDOBaseEditTheme::getOceanTheme();

	theme.selectLineBgColor = RGB( 0xCA, 0xF4, 0xFF );

	return theme;
}

// --------------------------------------------------------------------------------
// -------------------- LogEditStyle
// --------------------------------------------------------------------------------
LogEditStyle::LogEditStyle(): RDOBaseEditStyle()
{
}

LogEditStyle::~LogEditStyle()
{
}

void LogEditStyle::initTheme()
{
	theme = new LogEditTheme;
}

LogEditStyle& LogEditStyle::operator =( const LogEditStyle& style )
{
	RDOBaseEditStyle::operator=( style );
	if ( theme && style.theme ) *static_cast<LogEditTheme*>(theme) = *static_cast<LogEditTheme*>(style.theme);

	return *this;
}

rbool LogEditStyle::operator ==( const LogEditStyle& style ) const
{
	rbool flag = RDOBaseEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *static_cast<LogEditTheme*>(theme) == *static_cast<LogEditTheme*>(style.theme);
	return flag;
}

rbool LogEditStyle::operator !=( const LogEditStyle& style ) const
{
	return !(*this == style);
}
