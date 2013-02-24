/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_base.h
  \author    ������� �����
  \date      01.04.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_BASE_H_
#define _RDO_STUDIO_TRACER_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtCore/qstring.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "utils/smart_ptr/intrusive_ptr.h"
#include "kernel/rdothread.h"
#include "kernel/rdokernel.h"
#include "app/rdo_studio/resource.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

PREDECLARE_POINTER(TracerResourceType);
PREDECLARE_POINTER(TracerResource);
PREDECLARE_POINTER(TracerPattern);
PREDECLARE_POINTER(TracerOperationBase);
PREDECLARE_POINTER(TracerEvent);
PREDECLARE_POINTER(TracerResult);
PREDECLARE_POINTER(TracerSerie);
class TracerTimeNow;
class TracerResourceParamInfo;
class ChartTree;
class RDOStudioChartDoc;
class LogMainWnd;

class TracerBase: public RDOThreadGUI
{
public:
	void setLog (PTR(LogMainWnd) pTracerLog);
	void setTree(PTR(ChartTree) pTreeCtrl);

	void startTrace();
	void    getModelStructure(rdo::textstream& stream);
	void    getTraceString(tstring trace_string);
	tstring getNextValue(REF(tstring) line);
	void registerClipboardFormat();
	UINT const getClipboardFormat() const;
	RDOStudioChartDoc* createNewChart();
	void addChart(RDOStudioChartDoc* const pDocument);
	void removeChart(RDOStudioChartDoc* pDocument);
	RDOStudioChartDoc* addSerieToChart(CREF(LPTracerSerie) pSerie, RDOStudioChartDoc* pDocument = NULL);
	void updateChartsStyles() const;
	void clear();
	void setModelName(CREF(QString) name) const;
	void setDrawTrace(const rbool value);
	rbool getDrawTrace() const;

protected:
	TracerBase(CREF(tstring) _thread_name, RDOKernelGUI* _kernel_gui);
	virtual ~TracerBase();

private:
	LogMainWnd* m_pLog;
	ChartTree*  m_pChartTree;

	TracerResourceParamInfo* getParam(rdo::textstream& stream);
	TracerResourceParamInfo* getParamType(rdo::textstream& stream);

	typedef  std::vector<LPTracerResourceType>        ResourceTypeList;
	typedef  std::vector<LPTracerResource>       ResourceList;
	typedef  std::vector<LPTracerPattern>        PatternList;
	typedef  std::vector<LPTracerOperationBase>  OperationBaseList;
	typedef  std::vector<LPTracerEvent>          EventList;
	typedef  std::vector<LPTracerResult>         ResultList;
	typedef  std::list<TracerTimeNow*>           TimeList;
	typedef  std::vector<RDOStudioChartDoc*>     DocumentList;

	ResourceTypeList  m_resourceTypeList;
	ResourceList      m_resourceList;
	PatternList       m_patternList;
	OperationBaseList m_operationList;
	EventList         m_eventList;
	ResultList        m_resultList;

	void addResourceType(REF(tstring) s, rdo::textstream& stream);
	void addResource    (REF(tstring) s, rdo::textstream& stream);
	void addPattern     (REF(tstring) s, rdo::textstream& stream);
	void addOperation   (REF(tstring) s, rdo::textstream& stream);
	void addResult      (REF(tstring) s, rdo::textstream& stream);

	void dispatchNextString(REF(tstring) line);

	TracerTimeNow* addTime(CREF(tstring) time);
	int m_eventIndex;

	LPTracerOperationBase getOperation(REF(tstring) line);void startAction(REF(tstring) line, TracerTimeNow* const pTime);
	void accomplishAction(REF(tstring) line, TracerTimeNow* const pTime);
	void irregularEvent(REF(tstring) line, TracerTimeNow* const pTime);
	void productionRule(REF(tstring) line, TracerTimeNow* const pTime);

	LPTracerResource getResource(REF(tstring) line);
	LPTracerResource resourceCreation(REF(tstring) line, TracerTimeNow* const pTime);
	LPTracerResource resourceElimination(REF(tstring) line, TracerTimeNow* const pTime);
	enum TracerResUpdateAction
	{
		RUA_NONE, RUA_ADD, RUA_UPDATE
	};
	TracerResUpdateAction m_updateAction;
	LPTracerResource      m_pResource;
	LPTracerResource resourceChanging(REF(tstring) line, TracerTimeNow* const pTime);

	LPTracerResult getResult(REF(tstring) line);void resultChanging(REF(tstring) line, TracerTimeNow* const pTime);

	TimeList m_timeList;

	void clearCharts();
	void deleteTrace();

	UINT         m_clipboardFormat;
	DocumentList m_documentList;
	rbool        m_drawTrace;
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_BASE_H_
