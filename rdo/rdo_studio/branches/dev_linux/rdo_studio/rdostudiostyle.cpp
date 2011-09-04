#include "rdo_studio/stdafx.h"
#include "rdo_studio/rdostudiostyle.h"

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
		characterSet = RUSSIAN_CHARSET;
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

bool RDOStyleFont::operator ==( const RDOStyleFont& font ) const
{
	return name         == font.name &&
	       size         == font.size &&
	       codepage     == font.codepage &&
	       characterSet == font.characterSet;
}

bool RDOStyleFont::operator !=( const RDOStyleFont& font ) const
{
	return !(*this == font);
}

void RDOStyleFont::load( std::string regPath )
{
	regPath += "font";
	name         = AfxGetApp()->GetProfileString( regPath.c_str(), "name", name.c_str() );
	size         = AfxGetApp()->GetProfileInt( regPath.c_str(), "size", size );
	codepage     = AfxGetApp()->GetProfileInt( regPath.c_str(), "codepage", codepage );
	characterSet = AfxGetApp()->GetProfileInt( regPath.c_str(), "characterSet", characterSet );
}

void RDOStyleFont::save( std::string regPath ) const
{
	regPath += "font";
	AfxGetApp()->WriteProfileString( regPath.c_str(), "name", name.c_str() );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "size", size );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "codepage", codepage );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "characterSet", characterSet );
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

	font.name = "System";

	return font;
}

// --------------------------------------------------------------------------------
// -------------------- RDOStyleTheme
// --------------------------------------------------------------------------------
RDOStyleTheme::RDOStyleTheme()
{
	defaultColor    = RGB( 0x00, 0x00, 0x00 );
	backgroundColor = RGB( 0xFF, 0xFF, 0xFF );

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

bool RDOStyleTheme::operator ==( const RDOStyleTheme& theme ) const
{
	return defaultColor    == theme.defaultColor &&
	       backgroundColor == theme.backgroundColor &&

	       defaultStyle == theme.defaultStyle;
}

bool RDOStyleTheme::operator !=( const RDOStyleTheme& theme ) const
{
	return !(*this == theme);
}

void RDOStyleTheme::load( std::string regPath )
{
	regPath += "theme";
	defaultColor    = AfxGetApp()->GetProfileInt( regPath.c_str(), "defaultColor", defaultColor );
	backgroundColor = AfxGetApp()->GetProfileInt( regPath.c_str(), "backgroundColor", backgroundColor );
	defaultStyle    = static_cast<RDOStyleFont::style>(AfxGetApp()->GetProfileInt( regPath.c_str(), "defaultStyle", defaultStyle ));
}

void RDOStyleTheme::save( std::string regPath ) const
{
	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "defaultColor", defaultColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "backgroundColor", backgroundColor );
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "defaultStyle", defaultStyle );
}

// --------------------------------------------------------------------------------
// -------------------- RDOStyle
// --------------------------------------------------------------------------------
RDOStyle::RDOStyle():
	regPath( "" ),
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

bool RDOStyle::operator ==( const RDOStyle& style ) const
{
	bool flag = true;
	if ( font && style.font && flag ) flag &= *font == *style.font;
	return flag;
}

bool RDOStyle::operator !=( const RDOStyle& style ) const
{
	return !(*this == style);
}

void RDOStyle::init( const std::string& _regPath )
{
	regPath = _regPath;
	rdo::trim( regPath );
	if ( !regPath.empty() ) {
		if ( regPath.find_last_of( '\\' ) != regPath.length() - 1 ) {
			regPath += '\\';
		}
		regPath = "style\\" + regPath;
	}
	initFont();
}

bool RDOStyle::load()
{
	if ( !regPath.empty() ) {
		if ( font ) font->load( regPath );
		return true;
	}
	return false;
}

bool RDOStyle::save() const
{
	if ( !regPath.empty() ) {
		if ( font ) font->save( regPath );
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

bool RDOStyleWithTheme::operator ==( const RDOStyleWithTheme& style ) const
{
	bool flag = RDOStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *theme == *style.theme;
	return flag;
}

bool RDOStyleWithTheme::operator !=( const RDOStyleWithTheme& style ) const
{
	return !(*this == style);
}

void RDOStyleWithTheme::init( const std::string& _regPath )
{
	RDOStyle::init( _regPath );
	initTheme();
}

bool RDOStyleWithTheme::load()
{
	if ( RDOStyle::load() ) {
		if ( theme ) theme->load( regPath );
		return true;
	}
	return false;
}

bool RDOStyleWithTheme::save() const
{
	if ( RDOStyle::save() ) {
		if ( theme ) theme->save( regPath );
		return true;
	}
	return false;
}
