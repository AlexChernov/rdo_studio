#ifndef RDOSTUDIOCHARTDOC_H
#define RDOSTUDIOCHARTDOC_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudiodocserie.h"
#include "rdo_tracer/rdotracerserie.h"

#define WM_USER_UPDATE_CHART_VIEW WM_USER + 0x156
#define UPDATE_NEWSERIE  0x000
#define UPDATE_NEWVALUE  0x001
#define UPDATE_TIMETICKS 0x002


// ----------------------------------------------------------------------------
// ---------- RDOStudioChartDoc
// ----------------------------------------------------------------------------
class RDOTracerTimeNow;
class RDOTracerValue;
class RDOStudioDocSerie;

typedef std::list< RDOTracerTimeNow* > timesList;

class RDOStudioChartDoc : public CDocument
{
friend class RDOStudioChartView;
friend class RDOStudioChartDocInsertTime;
friend class RDOStudioChartOptionsChart;

protected:
	DECLARE_DYNCREATE(RDOStudioChartDoc)
	
	CMutex mutex;
	
	std::vector< RDOStudioDocSerie* > series;
	int getSerieIndex( RDOStudioDocSerie* serie ) const;
	COLORREF selectColor();
	RDOTracerSerieMarker selectMarker();

	timesList docTimes;
	timesList::iterator inserted_it;
	double minTimeOffset;
	int ticksCount;
	void incTimeEventsCount( RDOTracerTimeNow* time );
	bool newValueToSerieAdded( RDOTracerValue* val );

	bool previewMode;

	int getMaxMarkerSize() const;

	std::vector< HWND > views_hwnd;
	void removeFromViews( const HWND handle );
	void addToViews( const HWND handle );

	void updateChartViews( const UINT update_type ) const;

	void lock() { mutex.Lock(); };
	void unlock() { mutex.Unlock(); };

	std::string title;

public:
	//{{AFX_VIRTUAL(RDOStudioChartDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	virtual void SetTitle( LPCTSTR lpszTitle );
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioChartDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	RDOStudioChartDoc( const bool preview = false );
	virtual ~RDOStudioChartDoc();

	void addSerie( RDOTracerSerie* const serie );
	//void removeSerie( RDOTracerSerie* const serie );
	bool serieExists( const RDOTracerSerie* serie ) const;

	std::string getTitle() const { return title; };
	//void setTitle( const std::string _title );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDOSTUDIOCHARTDOC_H
