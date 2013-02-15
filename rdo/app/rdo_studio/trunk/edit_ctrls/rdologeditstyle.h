/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologeditstyle.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDIT_CTRLS_RDOLOGEDITSTYLE_H_
#define _RDO_STUDIO_EDIT_CTRLS_RDOLOGEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdobaseeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- LogEditTheme
// --------------------------------------------------------------------------------
class LogEditTheme: public RDOBaseEditTheme
{
public:
	LogEditTheme();
	virtual ~LogEditTheme();

	LogEditTheme& operator =( const LogEditTheme& theme );
	rbool operator ==( const LogEditTheme& theme ) const;
	rbool operator !=( const LogEditTheme& theme ) const;

	virtual void load( CREF(QString) regPath );
	virtual void save( CREF(QString) regPath ) const;

	QColor selectLineBgColor;

	static LogEditTheme getDefaultTheme();
	static LogEditTheme getClassicTheme();
	static LogEditTheme getTwilightTheme();
	static LogEditTheme getOceanTheme();
};

// --------------------------------------------------------------------------------
// -------------------- LogEditStyle
// --------------------------------------------------------------------------------
class LogEditStyle: public RDOBaseEditStyle
{
protected:
	virtual void initTheme();

public:
	LogEditStyle();
	virtual ~LogEditStyle();

	LogEditStyle& operator =( const LogEditStyle& style );
	rbool operator ==( const LogEditStyle& style ) const;
	rbool operator !=( const LogEditStyle& style ) const;
};

} // namespace rdoEditCtrl

#endif // _RDO_STUDIO_EDIT_CTRLS_RDOLOGEDITSTYLE_H_
