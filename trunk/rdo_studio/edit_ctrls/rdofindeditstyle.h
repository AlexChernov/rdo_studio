#ifndef RDOFINDEDITSTYLE_H
#define RDOFINDEDITSTYLE_H
#pragma once

#include "rdologeditstyle.h"

// ----------------------------------------------------------------------------
// ---------- RDOFindEditTheme
// ----------------------------------------------------------------------------
class RDOFindEditTheme: public RDOLogEditTheme
{
public:
	RDOFindEditTheme();
	virtual ~RDOFindEditTheme();

	RDOFindEditTheme& operator =( const RDOFindEditTheme& theme );
	bool operator ==( const RDOFindEditTheme& theme ) const;
	bool operator !=( const RDOFindEditTheme& theme ) const;

	virtual void load( string regPath );
	virtual void save( string regPath ) const;

	COLORREF identifierColor;
	COLORREF keywordColor;

	rdoBaseEdit::RDOFontStyle identifierStyle;
	rdoBaseEdit::RDOFontStyle keywordStyle;

	virtual bool styleDefault( const int styleType ) const;
	virtual bool styleUsing( const int styleType ) const;
	virtual bool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual bool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual string styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;
};

// ----------------------------------------------------------------------------
// ---------- RDOFindEditStyle
// ----------------------------------------------------------------------------
class RDOFindEditStyle: public RDOLogEditStyle
{
protected:
	virtual void initTheme();

public:
	RDOFindEditStyle();
	virtual ~RDOFindEditStyle();

	RDOFindEditStyle& operator =( const RDOFindEditStyle& style );
	bool operator ==( const RDOFindEditStyle& style ) const;
	bool operator !=( const RDOFindEditStyle& style ) const;
};

#endif // RDOFINDEDITSTYLE_H
