#include "stdafx.h"
#include "rdologeditstyle.h"
#include "sci/SciLexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOLogEditTheme
// ----------------------------------------------------------------------------
RDOLogEditTheme::RDOLogEditTheme(): RDOBaseEditTheme()
{
	selectLineBgColor = RGB( 0x00, 0xC0, 0xEF );
}

RDOLogEditTheme::~RDOLogEditTheme()
{
}

RDOLogEditTheme& RDOLogEditTheme::operator =( const RDOLogEditTheme& theme )
{
	selectLineBgColor = theme.selectLineBgColor;

	return *this;
}

bool RDOLogEditTheme::operator ==( const RDOLogEditTheme& theme ) const
{
	return selectLineBgColor == theme.selectLineBgColor;
}

bool RDOLogEditTheme::operator !=( const RDOLogEditTheme& theme ) const
{
	return !(*this == theme);
}

void RDOLogEditTheme::load( string regPath )
{
	RDOBaseEditTheme::load( regPath );

	regPath += "theme";
	selectLineBgColor = AfxGetApp()->GetProfileInt( regPath.c_str(), "selectLineBgColor", selectLineBgColor );
}

void RDOLogEditTheme::save( string regPath ) const
{
	RDOBaseEditTheme::save( regPath );

	regPath += "theme";
	AfxGetApp()->WriteProfileInt( regPath.c_str(), "selectLineBgColor", selectLineBgColor );
}

// ----------------------------------------------------------------------------
// ---------- RDOLogEditStyle
// ----------------------------------------------------------------------------
RDOLogEditStyle::RDOLogEditStyle(): RDOBaseEditStyle()
{
}

RDOLogEditStyle::~RDOLogEditStyle()
{
}

void RDOLogEditStyle::initTheme()
{
	theme = new RDOLogEditTheme;
}

RDOLogEditStyle& RDOLogEditStyle::operator =( const RDOLogEditStyle& style )
{
	RDOBaseEditStyle::operator=( style );
	if ( theme && style.theme ) *(RDOLogEditTheme*)theme = *(RDOLogEditTheme*)style.theme;

	return *this;
}

bool RDOLogEditStyle::operator ==( const RDOLogEditStyle& style ) const
{
	bool flag = RDOBaseEditStyle::operator==( style );
	if ( theme && style.theme && flag ) flag &= *(RDOLogEditTheme*)theme == *(RDOLogEditTheme*)style.theme;
	return flag;
}

bool RDOLogEditStyle::operator !=( const RDOLogEditStyle& style ) const
{
	return !(*this == style);
}
