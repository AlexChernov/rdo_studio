/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      log_view_style.cpp
  \author    ������� �����
  \date      12.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/ctrls/log_view_style.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace rdo::gui::tracer;
using namespace rdoStyle;

// --------------------------------------------------------------------------------
// -------------------- LogColorPair
// --------------------------------------------------------------------------------
LogColorPair::LogColorPair()
{
	foregroundColor = QColor( 0x00, 0x00, 0x00 );
	backgroundColor = QColor( 0xFF, 0xFF, 0xFF );
}

LogColorPair::~LogColorPair()
{
}

LogColorPair& LogColorPair::operator =( const LogColorPair& colors )
{
	foregroundColor = colors.foregroundColor;
	backgroundColor = colors.backgroundColor;

	return *this;
}

rbool LogColorPair::operator ==( const LogColorPair& colors ) const
{
	return foregroundColor == colors.foregroundColor &&
	       backgroundColor == colors.backgroundColor;
}

rbool LogColorPair::operator !=( const LogColorPair& colors ) const
{
	return !(*this == colors);
}

void LogColorPair::load( CREF(QString) groupName, QString regParam )
{
	QSettings settings;
	settings.beginGroup(groupName);
	regParam.append("_");
	foregroundColor = QColor(settings.value(QString(regParam + "foreground_color"), foregroundColor.name()).toString());
	backgroundColor = QColor(settings.value(QString(regParam + "background_color"), backgroundColor.name()).toString());
	settings.endGroup();
}

void LogColorPair::save( CREF(QString) groupName, QString regParam ) const
{
	QSettings settings;
	settings.beginGroup(groupName);
	regParam.append("_");
	settings.setValue(QString(regParam + "foreground_color"), foregroundColor.name());
	settings.setValue(QString(regParam + "background_color"), backgroundColor.name());
	settings.endGroup();
}

// --------------------------------------------------------------------------------
// -------------------- LogTheme
// --------------------------------------------------------------------------------
LogTheme::LogTheme()
{
	style = RDOStyleFont::NONE;

	es.foregroundColor = QColor( 0x8B, 0x00, 0x00 );
	es.backgroundColor = QColor( 0xFF, 0xC0, 0xCB );
	
	eb.foregroundColor = QColor( 0x34, 0x4B, 0xA2 );
	eb.backgroundColor = QColor( 0xAA, 0xE3, 0xFB );
	
	ef.foregroundColor = QColor( 0x16, 0x02, 0x50 );
	ef.backgroundColor = QColor( 0x81, 0xB0, 0xD5 );
	
	ei.foregroundColor = QColor( 0x4F, 0x29, 0x62 );
	ei.backgroundColor = QColor( 0xD0, 0xD0, 0xFF );
	
	er.foregroundColor = QColor( 0x17, 0x32, 0x47 );
	er.backgroundColor = QColor( 0xB6, 0xCB, 0xDB );
	
	rc.foregroundColor = QColor( 0x23, 0x74, 0x42 );
	rc.backgroundColor = QColor( 0x96, 0xFF, 0x96 );
	
	re.foregroundColor = QColor( 0x43, 0x5A, 0x43 );
	re.backgroundColor = QColor( 0xB4, 0xE0, 0xB4 );
	
	rk.foregroundColor = QColor( 0x00, 0x86, 0x00 );
	rk.backgroundColor = QColor( 0xD0, 0xFF, 0xD0 );
	
	v.foregroundColor  = QColor( 0x00, 0x00, 0x00 );
	v.backgroundColor  = QColor( 0xF1, 0xFB, 0xE2 );
	
	s.foregroundColor  = QColor( 0x00, 0x00, 0xFF );
	s.backgroundColor  = QColor( 0xE7, 0xF8, 0xF8 );

	dps.foregroundColor = QColor( 0x03, 0x23, 0x8B );
	dps.backgroundColor = QColor( 0xC9, 0xCD, 0xDB );

	sb.foregroundColor = QColor( 0x5A, 0x4F, 0x37 );
	sb.backgroundColor = QColor( 0xF8, 0xD6, 0x8D );
	
	so.foregroundColor = QColor( 0x4B, 0x54, 0x0E );
	so.backgroundColor = QColor( 0xE6, 0xF1, 0x98 );

	stn.foregroundColor = QColor( 0x00, 0x54, 0x72 );
	stn.backgroundColor = QColor( 0xE8, 0xE8, 0xD7 );

	std.foregroundColor = QColor( 0x69, 0x55, 0x49 );
	std.backgroundColor = stn.backgroundColor;

	str.foregroundColor = QColor( 0x8B, 0x00, 0x00 );
	str.backgroundColor = stn.backgroundColor;
	
	src.foregroundColor = rc.foregroundColor;
	src.backgroundColor = rc.backgroundColor;

	sre.foregroundColor = re.foregroundColor;
	sre.backgroundColor = re.backgroundColor;

	srk.foregroundColor = rk.foregroundColor;
	srk.backgroundColor = rk.backgroundColor;

	sd.foregroundColor = QColor( 0x54, 0x1E, 0x09 );
	sd.backgroundColor = QColor( 0xF7, 0xCF, 0xB5 );

	ses.foregroundColor = QColor( 0x54, 0x1E, 0x09 );
	ses.backgroundColor = QColor( 0xF0, 0xDE, 0xDB );
	
	sen.foregroundColor = QColor( 0xF0, 0x4B, 0x30 );
	sen.backgroundColor = ses.backgroundColor;
	
	sem.foregroundColor = QColor( 0xF0, 0x4B, 0x30 );
	sem.backgroundColor = QColor( 0xE3, 0xF0, 0xF6 );
	
	sef.foregroundColor = sem.foregroundColor;
	sef.backgroundColor = sem.backgroundColor;
	
	seu.foregroundColor = sem.foregroundColor;
	seu.backgroundColor = sem.backgroundColor;
}

LogTheme::~LogTheme()
{}

LogTheme& LogTheme::operator =( const LogTheme& theme )
{
	style        = theme.style;
	defaultColor = theme.defaultColor;
	es           = theme.es;
	eb           = theme.eb;
	ef           = theme.ef;
	ei           = theme.ei;
	er           = theme.er;
	rc           = theme.rc;
	re           = theme.re;
	rk           = theme.rk;
	v            = theme.v;
	s            = theme.s;
	dps          = theme.dps;
	sb           = theme.sb;
	so           = theme.so;
	stn          = theme.stn;
	std          = theme.std;
	str          = theme.str;
	src          = theme.src;
	sre          = theme.sre;
	srk          = theme.srk;
	sd           = theme.sd;
	ses          = theme.ses;
	sen          = theme.sen;
	sem          = theme.sem;
	sef          = theme.sef;
	seu          = theme.seu;

	return *this;
}

rbool LogTheme::operator ==( const LogTheme& theme ) const
{
	return style        == theme.style
	    && defaultColor == theme.defaultColor
	    && es           == theme.es
	    && eb           == theme.eb
	    && ef           == theme.ef
	    && ei           == theme.ei
	    && er           == theme.er
	    && rc           == theme.rc
	    && re           == theme.re
	    && rk           == theme.rk
	    && v            == theme.v
	    && s            == theme.s
	    && dps          == theme.dps
	    && sb           == theme.sb
	    && so           == theme.so
	    && stn          == theme.stn
	    && std          == theme.std
	    && str          == theme.str
	    && src          == theme.src
	    && sre          == theme.sre
	    && srk          == theme.srk
	    && sd           == theme.sd
	    && ses          == theme.ses
	    && sen          == theme.sen
	    && sem          == theme.sem
	    && sef          == theme.sef
	    && seu          == theme.seu;
}

rbool LogTheme::operator !=( const LogTheme& theme ) const
{
	return !(*this == theme);
}

void LogTheme::load( QString groupName )
{
	QSettings settings;
	groupName.append("theme");
	settings.beginGroup(groupName);
	style = static_cast<RDOStyleFont::style>(settings.value("style", style).toInt());
	settings.endGroup();
	defaultColor.load( groupName, "defaultColor" );
	es.load ( groupName, "es"  );
	eb.load ( groupName, "eb"  );
	ef.load ( groupName, "ef"  );
	ei.load ( groupName, "ei"  );
	er.load ( groupName, "er"  );
	rc.load ( groupName, "rc"  );
	re.load ( groupName, "re"  );
	rk.load ( groupName, "rk"  );
	v.load  ( groupName, "v"   );
	s.load  ( groupName, "s"   );
	dps.load( groupName, "dps" );
	sb.load ( groupName, "sb"  );
	so.load ( groupName, "so"  );
	stn.load( groupName, "stn" );
	std.load( groupName, "std" );
	str.load( groupName, "str" );
	src.load( groupName, "src" );
	sre.load( groupName, "sre" );
	srk.load( groupName, "srk" );
	sd.load ( groupName, "sd"  );
	ses.load( groupName, "ses" );
	sen.load( groupName, "sen" );
	sem.load( groupName, "sem" );
	sef.load( groupName, "sef" );
	seu.load( groupName, "seu" );
}

void LogTheme::save( QString groupName ) const
{
	QSettings settings;
	groupName.append("theme");
	settings.beginGroup(groupName);
	settings.setValue("style", style);
	settings.endGroup();
	defaultColor.save( groupName, "defaultColor" );
	es.save ( groupName, "es"  );
	eb.save ( groupName, "eb"  );
	ef.save ( groupName, "ef"  );
	ei.save ( groupName, "ei"  );
	er.save ( groupName, "er"  );
	rc.save ( groupName, "rc"  );
	re.save ( groupName, "re"  );
	rk.save ( groupName, "rk"  );
	v.save  ( groupName, "v"   );
	s.save  ( groupName, "s"   );
	dps.save( groupName, "dps" );
	sb.save ( groupName, "sb"  );
	so.save ( groupName, "so"  );
	stn.save( groupName, "stn" );
	std.save( groupName, "std" );
	str.save( groupName, "str" );
	src.save( groupName, "src" );
	sre.save( groupName, "sre" );
	srk.save( groupName, "srk" );
	sd.save ( groupName, "sd"  );
	ses.save( groupName, "ses" );
	sen.save( groupName, "sen" );
	sem.save( groupName, "sem" );
	sef.save( groupName, "sef" );
	seu.save( groupName, "seu" );
}

LogTheme LogTheme::getDefaultTheme()
{
	LogTheme theme;
	return theme;
}

// --------------------------------------------------------------------------------
// -------------------- LogBorders
// --------------------------------------------------------------------------------
LogBorders::LogBorders()
{
	vertBorder = 0;
	horzBorder = 0;
}

LogBorders::~LogBorders()
{
}

LogBorders& LogBorders::operator =( const LogBorders& borders )
{
	vertBorder = borders.vertBorder;
	horzBorder = borders.horzBorder;

	return *this;
}

rbool LogBorders::operator ==( const LogBorders& borders ) const
{
	return vertBorder == borders.vertBorder &&
	       horzBorder == borders.horzBorder;
}

rbool LogBorders::operator !=( const LogBorders& borders ) const
{
	return !(*this == borders);
}

void LogBorders::load( CREF(QString) groupName )
{
	QSettings settings;
	settings.beginGroup(groupName);
	vertBorder = settings.value("vert_border", vertBorder).toInt();
	horzBorder = settings.value("horz_border", horzBorder).toInt();
	settings.endGroup();
}

void LogBorders::save( CREF(QString) groupName ) const
{
	QSettings settings;
	settings.beginGroup(groupName);
	settings.setValue("vert_border", vertBorder);
	settings.setValue("horz_border", horzBorder);
	settings.endGroup();
}

// --------------------------------------------------------------------------------
// -------------------- LogStyle
// --------------------------------------------------------------------------------
LogStyle::LogStyle()
	: RDOStyle()
	, theme   (NULL)
	, borders (NULL)
{}

LogStyle::~LogStyle()
{
	if ( theme )   { delete theme;   theme = NULL; };
	if ( borders ) { delete borders; borders = NULL; };
}

void LogStyle::initTheme()
{
	theme = new LogTheme;
}

void LogStyle::initBorders()
{
	borders = new LogBorders;
	borders->vertBorder = 1;
	borders->horzBorder = 2;
}

rbool LogStyle::getItemColors( int index, LogColorPair* &colors ) const
{
	UNUSED(index);
	return getDefaultColor(colors);
}

rbool LogStyle::getItemColors( CREF(tstring) item, LogColorPair* &colors ) const
{
	if ( item.empty() )
		return LogStyle::getDefaultColor(colors);
	int posstart = item.find_first_not_of( ' ' );
	int posend = item.find_first_of( ' ', posstart );
	tstring key = item.substr( posstart, posend - posstart );
	rdo::trim( key );
	rbool res = true;
	if ( theme ) {
		if ( key == "ES" ) {
			colors = &theme->es;
		} else if ( key == "EB" ) {
			colors = &theme->eb;
		} else if ( key == "EF" ) {
			colors = &theme->ef;
		} else if ( key == "EI" ) {
			colors = &theme->ei;
		} else if ( key == "ER" ) {
			colors = &theme->er;
		} else if ( key == "RC" ) {
			colors = &theme->rc;
		} else if ( key == "RE" ) {
			colors = &theme->re;
		} else if ( key == "RK" ) {
			colors = &theme->rk;
		} else if ( key == "V" ) {
			colors = &theme->v;
		} else if ( key == "$Status" ) {
			colors = &theme->s;
		} else if ( key.find( "DPS" ) != tstring::npos ) {
			colors = &theme->dps;
		} else if ( key == "SB" ) {
			colors = &theme->sb;
		} else if ( key == "SO" ) {
			colors = &theme->so;
		} else if ( key == "STN" ) {
			colors = &theme->stn;
		} else if ( key == "STD" ) {
			colors = &theme->std;
		} else if ( key == "STR" ) {
			colors = &theme->str;
		} else if ( key == "SRC" ) {
			colors = &theme->src;
		} else if ( key == "SRE" ) {
			colors = &theme->sre;
		} else if ( key == "SRK" ) {
			colors = &theme->srk;
		} else if ( key == "SD" ) {
			colors = &theme->sd;
		} else if ( key == "SES" ) {
			colors = &theme->ses;
		} else if ( key == "SEN" ) {
			colors = &theme->sen;
		} else if ( key == "SEM" ) {
			colors = &theme->sem;
		} else if ( key == "SEF" ) {
			colors = &theme->sef;
		} else if ( key == "SEU" ) {
			colors = &theme->seu;
		} else {
			res = LogStyle::getDefaultColor(colors);
		}
	} else {
		res = LogStyle::getDefaultColor(colors);
	}
	return res;
}

rbool LogStyle::getDefaultColor(LogColorPair* &colors) const
{
	if (theme)
	{
		colors = &theme->defaultColor;
	}
	return false;
}

LogStyle& LogStyle::operator =( const LogStyle& style )
{
	RDOStyle::operator=( style );
	if ( theme   && style.theme )   *theme   = *style.theme;
	if ( borders && style.borders ) *borders = *style.borders;

	return *this;
}

rbool LogStyle::operator ==( const LogStyle& style ) const
{
	rbool flag = RDOStyle::operator==( style );
	if ( theme   && style.theme   && flag ) flag &= *theme   == *style.theme;
	if ( borders && style.borders && flag ) flag &= *borders == *style.borders;
	return flag;
}

rbool LogStyle::operator !=( const LogStyle& style ) const
{
	return !(*this == style);
}

void LogStyle::init( CREF(QString) _groupName )
{
	RDOStyle::init( _groupName );
	initTheme();
	initBorders();
	*font = rdoStyle::RDOStyleFont::getTracerLogFont();
}

rbool LogStyle::load()
{
	if ( RDOStyle::load() ) {
		if ( theme )   theme->load( groupName );
		if ( borders ) borders->load( groupName + "borders" );
		return true;
	}
	return false;
}

rbool LogStyle::save() const
{
	if ( RDOStyle::save() ) {
		if ( theme )   theme->save( groupName );
		if ( borders ) borders->save( groupName + "borders" );
		return true;
	}
	return false;
}
