#ifndef RDOTRACERLOGSTYLE_H
#define RDOTRACERLOGSTYLE_H
#pragma once

#include "rdologstyle.h"

class RDOStudioOptionsStylesAndColors;

namespace rdoTracerLog {

// ----------------------------------------------------------------------------
// ---------- RDOTracerLogStyle
// ----------------------------------------------------------------------------
typedef struct {
	COLORREF foregroundColor;
	COLORREF backgroundColor;
} RDOColorPair;


class RDOTracerLogStyle : public RDOLogStyle
{
friend class RDOStudioOptionsStylesAndColors;

protected:
	RDOColorPair es;
	RDOColorPair eb;
	RDOColorPair ef;
	RDOColorPair ei;
	RDOColorPair er;
	RDOColorPair rc;
	RDOColorPair re;
	RDOColorPair rk;
	RDOColorPair v;
	RDOColorPair s;
	RDOColorPair dps;
	RDOColorPair sb;
	RDOColorPair so;
	RDOColorPair stn;
	RDOColorPair std;
	RDOColorPair str;
	RDOColorPair src;
	RDOColorPair sre;
	RDOColorPair srk;
	RDOColorPair sd;
	RDOColorPair ses;
	RDOColorPair sen;
	RDOColorPair sem;
	RDOColorPair sef;
	RDOColorPair seu;

public:
	RDOTracerLogStyle();
	virtual ~RDOTracerLogStyle();

	bool getItemColors( const std::string& item, COLORREF& textColor, COLORREF& backColor ) const;

	RDOTracerLogStyle& operator =( const RDOTracerLogStyle& style );
	bool operator ==( const RDOTracerLogStyle& style ) const;
	bool operator !=( const RDOTracerLogStyle& style ) const;

	static RDOTracerLogStyle getDefaultStyle();

	virtual bool load();
	virtual bool save() const;
};

}; // namespace rdoTracerLog

#endif // RDOTRACERLOGSTYLE_H
