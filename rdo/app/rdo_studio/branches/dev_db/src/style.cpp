/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/style.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      27.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/style.h"
#include "thirdparty/scintilla/include/Scintilla.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- RDOStyleFont
// --------------------------------------------------------------------------------
RDOStyleFont::RDOStyleFont()
{
	name         = "Courier New";
	size         = 10;
	codepage     = 0;
	characterSet = 1;
	if ( PRIMARYLANGID(GetSystemDefaultLangID()) == LANG_RUSSIAN ) {
		characterSet = SC_CHARSET_CYRILLIC;
	}
}

RDOStyleFont::~RDOStyleFont()
{
}

RDOStyleFont& RDOStyleFont::operator =( const RDOStyleFont& font )
{
	name         = font.name;
	size         = font.size;
	codepage     = font.codepage;
	characterSet = font.characterSet;

	return *this;
}

rbool RDOStyleFont::operator ==( const RDOStyleFont& font ) const
{
	return name         == font.name &&
	       size         == font.size &&
	       codepage     == font.codepage &&
	       characterSet == font.characterSet;
}

rbool RDOStyleFont::operator !=( const RDOStyleFont& font ) const
{
	return !(*this == font);
}

void RDOStyleFont::load( CREF(QString) groupName )
{
	QSettings settings;
	settings.beginGroup(groupName + "font");
	name         = settings.value("name", QString::fromLocal8Bit(name.c_str())).toString().toLocal8Bit().constData();
	size         = settings.value("size", size).toInt();
	codepage     = settings.value("codepage", codepage).toInt();
	characterSet = settings.value("character_set", characterSet).toInt();
	settings.endGroup();
	if (characterSet == RUSSIAN_CHARSET)
	{
		characterSet = SC_CHARSET_CYRILLIC;
	}
}

void RDOStyleFont::save( CREF(QString) groupName ) const
{
	QSettings settings;
	settings.beginGroup(groupName +"font");
	settings.setValue("name", QString::fromLocal8Bit(name.c_str()));
	settings.setValue("size", size);
	settings.setValue("codepage", codepage);
	settings.setValue("character_set", characterSet);
	settings.endGroup();
}

RDOStyleFont RDOStyleFont::getDefaultFont()
{
	RDOStyleFont font;
	return font;
}

RDOStyleFont RDOStyleFont::getClassicFont()
{
	RDOStyleFont font;

	font.name = "Fixedsys";

	return font;
}

RDOStyleFont RDOStyleFont::getTracerLogFont()
{
	RDOStyleFont font;

	font.name = "Courier";

	return font;
}

RDOStyleFont RDOStyleFont::getChartViewFont()
{
	RDOStyleFont font;

	font.name = "Tahoma";

	return font;
}

RDOStyleFont RDOStyleFont::getFrameFont()
{
	RDOStyleFont font;

	font.name = _T("Verdana");
	font.size = 8;

	return font;
}

// --------------------------------------------------------------------------------
// -------------------- RDOStyleTheme
// --------------------------------------------------------------------------------
RDOStyleTheme::RDOStyleTheme()
{
	defaultColor    = QColor( 0x00, 0x00, 0x00 );
	backgroundColor = QColor( 0xFF, 0xFF, 0xFF );

	defaultStyle = RDOStyleFont::NONE;
}

RDOStyleTheme::~RDOStyleTheme()
{
}

RDOStyleTheme& RDOStyleTheme::operator =( const RDOStyleTheme& theme )
{
	defaultColor    = theme.defaultColor;
	backgroundColor = theme.backgroundColor;

	defaultStyle = theme.defaultStyle;

	return *this;
}

rbool RDOStyleTheme::operator ==( const RDOStyleTheme& theme ) const
{
	return defaultColor    == theme.defaultColor &&
	       backgroundColor == theme.backgroundColor &&

	       defaultStyle == theme.defaultStyle;
}

rbool RDOStyleTheme::operator !=( const RDOStyleTheme& theme ) const
{
	return !(*this == theme);
}

void RDOStyleTheme::load( CREF(QString) groupName )
{
	QSettings settings;
	settings.beginGroup(groupName + "theme");
	settings >> *this;
	settings.endGroup();
}

void RDOStyleTheme::save( CREF(QString) groupName ) const
{
	QSettings settings;
	settings.beginGroup(groupName + "theme");
	settings << *this;
	settings.endGroup();
}

namespace rdoStyle
{

QSettings& operator<< (QSettings& settings, const RDOStyleTheme& theme)
{
	settings.setValue("default_color", theme.defaultColor.name());
	settings.setValue("background_color", theme.backgroundColor.name());
	settings.setValue("default_style", theme.defaultStyle);

	return settings;
}

QSettings& operator>> (QSettings& settings, RDOStyleTheme& theme)
{
	theme.defaultColor    = QColor(settings.value("default_color", QColor( 0x00, 0x00, 0x00 ).name()).toString());
	theme.backgroundColor = QColor(settings.value("background_color", QColor( 0xFF, 0xFF, 0xFF ).name()).toString());
	theme.defaultStyle    = static_cast<RDOStyleFont::style>(settings.value("default_style", RDOStyleFont::NONE).toInt());

	return settings;
}

} // namespace rdoStyle


// --------------------------------------------------------------------------------
// -------------------- RDOStyle
// --------------------------------------------------------------------------------
RDOStyle::RDOStyle():
	groupName( "" ),
	font( NULL )
{
}

RDOStyle::~RDOStyle()
{
	if ( font ) { delete font; font = NULL; };
}

void RDOStyle::initFont()
{
	font = new RDOStyleFont;
}

RDOStyle& RDOStyle::operator =( const RDOStyle& style )
{
	if ( font && style.font ) *font = *style.font;

	return *this;
}

rbool RDOStyle::operator ==( const RDOStyle& style ) const
{
	rbool flag = true;
	if ( font && style.font && flag ) flag &= *font == *style.font;
	return flag;
}

rbool RDOStyle::operator !=( const RDOStyle& style ) const
{
	return !(*this == style);
}

void RDOStyle::init( CREF(QString) _groupName )
{
	groupName = _groupName;
	if (!groupName.isEmpty()) 
	{
		groupName.prepend("style/");
		if(groupName.lastIndexOf("/") != groupName.length() - 1)
		{
			groupName.append("/");
		}
	}
	initFont();
}

rbool RDOStyle::load()
{
	if ( !groupName.isEmpty() ) {
		if ( font ) font->load( groupName );
		return true;
	}
	return false;
}

rbool RDOStyle::save() const
{
	if ( !groupName.isEmpty() ) {
		if ( font ) font->save( groupName );
		return true;
	}
	return false;
}

// --------------------------------------------------------------------------------
// -------------------- RDOStyleWithTheme
// --------------------------------------------------------------------------------
RDOStyleWithTheme::RDOStyleWithTheme():
	RDOStyle(),
	theme( NULL )
{
}

RDOStyleWithTheme::~RDOStyleWithTheme()
{
	if ( theme ) { delete theme; theme = NULL; };
}

void RDOStyleWithTheme::initTheme()
{
	theme = new RDOStyleTheme;
}

RDOStyleWithTheme& RDOStyleWithTheme::operator =( const RDOStyleWithTheme& style )
{
	RDOStyle::operator=( style );
	if ( theme && style.theme ) *theme = *style.theme;

	return *this;
}

rbool RDOStyleWithTheme::operator ==( const RDOStyleWithTheme& style ) const
{
	rbool flag = RDOStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *theme == *style.theme;
	return flag;
}

rbool RDOStyleWithTheme::operator !=( const RDOStyleWithTheme& style ) const
{
	return !(*this == style);
}

void RDOStyleWithTheme::init( CREF(QString) _groupName )
{
	RDOStyle::init( _groupName );
	initTheme();
}

rbool RDOStyleWithTheme::load()
{
	if ( RDOStyle::load() ) {
		if ( theme ) theme->load( groupName );
		return true;
	}
	return false;
}

rbool RDOStyleWithTheme::save() const
{
	if ( RDOStyle::save() ) {
		if ( theme ) theme->save( groupName );
		return true;
	}
	return false;
}
