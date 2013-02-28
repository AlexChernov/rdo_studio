/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_view_style.h
  \author    ������� �����
  \date      28.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_CHART_VIEW_STYLE_H_
#define _RDO_STUDIO_TRACER_CHART_VIEW_STYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- ChartViewTheme
// --------------------------------------------------------------------------------
class ChartViewTheme: public rdoStyle::RDOStyleTheme
{
public:
	ChartViewTheme();
	virtual ~ChartViewTheme();

	ChartViewTheme& operator =(const ChartViewTheme& theme);
	rbool operator ==(const ChartViewTheme& theme) const;
	rbool operator !=(const ChartViewTheme& theme) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor axisFgColor;
	QColor titleFGColor;
	QColor legendFgColor;
	QColor chartBgColor;
	QColor timeBgColor;

	rdoStyle::RDOStyleFont::style titleStyle;
	rdoStyle::RDOStyleFont::style legendStyle;

	static ChartViewTheme getDefaultTheme();
};

QSettings& operator<< (QSettings& settings, const ChartViewTheme& theme);
QSettings& operator>> (QSettings& settings,       ChartViewTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- ChartViewFontsTicks
// --------------------------------------------------------------------------------
class ChartViewFontsTicks
{
public:
	ChartViewFontsTicks();
	virtual ~ChartViewFontsTicks();

	ChartViewFontsTicks& operator =(const ChartViewFontsTicks& fonts_ticks);
	rbool operator ==(const ChartViewFontsTicks& fonts_ticks) const;
	rbool operator !=(const ChartViewFontsTicks& fonts_ticks) const;

	void load(QSettings& settings);
	void save(QSettings& settings) const;

	int titleFontSize;
	int legendFontSize;
	int tickWidth;
};

QSettings& operator<< (QSettings& settings, const ChartViewFontsTicks& fonts_ticks);
QSettings& operator>> (QSettings& settings,       ChartViewFontsTicks& fonts_ticks);

// --------------------------------------------------------------------------------
// -------------------- ChartViewStyle
// --------------------------------------------------------------------------------
class ChartViewStyle: public rdoStyle::RDOStyleWithTheme
{
public:
	ChartViewStyle();
	virtual ~ChartViewStyle();

	ChartViewStyle& operator =(const ChartViewStyle& style);
	rbool operator ==(const ChartViewStyle& style) const;
	rbool operator !=(const ChartViewStyle& style) const;

	void init(CREF(QString) _groupName = "");
	rbool load();
	rbool save() const;

	ChartViewTheme* getTheme()
	{
		return static_cast<ChartViewTheme*>(theme);
	}

	ChartViewFontsTicks* pFontsTicks;
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_CHART_VIEW_STYLE_H_
