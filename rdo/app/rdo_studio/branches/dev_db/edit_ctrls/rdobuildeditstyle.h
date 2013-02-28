/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobuildeditstyle.h
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      25.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDIT_CTRLS_RDOBUILDEDITSTYLE_H_
#define _RDO_STUDIO_EDIT_CTRLS_RDOBUILDEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdologeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEditTheme
// --------------------------------------------------------------------------------
class RDOBuildEditTheme: public LogEditTheme
{
public:
	RDOBuildEditTheme();
	virtual ~RDOBuildEditTheme();

	RDOBuildEditTheme& operator =( const RDOBuildEditTheme& theme );
	rbool operator ==( const RDOBuildEditTheme& theme ) const;
	rbool operator !=( const RDOBuildEditTheme& theme ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	rbool warning;

	static RDOBuildEditTheme getDefaultTheme();
	static RDOBuildEditTheme getClassicTheme();
	static RDOBuildEditTheme getTwilightTheme();
	static RDOBuildEditTheme getOceanTheme();
};

QSettings& operator<< (QSettings& settings, const RDOBuildEditTheme& theme);
QSettings& operator>> (QSettings& settings,       RDOBuildEditTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEditStyle
// --------------------------------------------------------------------------------
class RDOBuildEditStyle: public LogEditStyle
{
public:
	RDOBuildEditStyle();
	virtual ~RDOBuildEditStyle();

	RDOBuildEditStyle& operator =( const RDOBuildEditStyle& style );
	rbool operator ==( const RDOBuildEditStyle& style ) const;
	rbool operator !=( const RDOBuildEditStyle& style ) const;
};

} // namespace rdoEditCtrl

#endif // _RDO_STUDIO_EDIT_CTRLS_RDOBUILDEDITSTYLE_H_
