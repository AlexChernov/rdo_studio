#include "stdafx.h"
#include "rdotracertrace.h"
#include "rdotracerrestype.h"
#include "rdotracerresource.h"
#include "rdotracerpattern.h"
#include "rdotraceroperation.h"
#include "rdotracerresult.h"
#include "rdotracervalues.h"
#include "./tracer_ctrls/rdotracerlogctrl.h"
#include "./tracer_ctrls/rdotracertreectrl.h"
#include "../rdostudiochartdoc.h"
#include "../rdostudiochartview.h"
#include "../rdostudioapp.h"
#include "../rdostudiochildfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace rdoTracer;
using namespace rdoTracerLog;
using namespace std;

RDOTracer tracer;

void CALLBACK traceTimerFunc( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
	tracer.getTraceString();
}

// ----------------------------------------------------------------------------
// ---------- RDOTracer
// ----------------------------------------------------------------------------

RDOTracer::RDOTracer():
	log( NULL ),
	tree( NULL ),
	traceFile( NULL ),
	timerID( 0 ),
	tracing( false ),
	clipboardFormat( 0 ),
	chartDocTemplate( NULL ),
	eventIndex( 0 )/*,
	minTimeDifference( 1.7E+308 )*/
{
}

RDOTracer::~RDOTracer()
{
	stopTrace();
	deleteTrace();
}

void RDOTracer::cleanupObjects()
{
	if ( timerID ) {
		::KillTimer( NULL, timerID );
		timerID = 0;
	}
	if ( traceFile ) {
		delete traceFile;
		traceFile = NULL;
	}
}

void RDOTracer::getStructureData()
{
	string res;
	if ( !traceFile )
		return;
	CString str;
	while ( traceFile->ReadString( str ) && str != "$Tracing" ) {
		res = str;
		parseStructureData( res );
	}
	return;
}

void RDOTracer::parseStructureData( string& structure )
{
	CString str;
	string s = "Model_name";
	int startpos = 0;
	int pos = structure.find( s );
	if ( pos != string::npos ) {
		startpos = structure.find( "=", pos + s.length() ) + 1;
		modelName = structure.substr( startpos );
		trim( modelName );
		tree->setModelName( modelName );
	}

	if ( structure.find( "$Resource_type" ) != string::npos ) {
		do {
			traceFile->ReadString( str );
			s = str;
			if ( !s.empty() )
				addResourceType( s );
		} while ( s != "" );
	}
	
	if ( structure.find( "$Resources" ) != string::npos ) {
		do {
			traceFile->ReadString( str );
			s = str;
			if ( !s.empty() )
				addResource( s );
		} while ( s != "" );
	}

	if ( structure.find( "$Pattern" ) != string::npos ) {
		do {
			traceFile->ReadString( str );
			s = str;
			if ( !s.empty() )
				addPattern( s );
		} while ( s != "" );
	}

	if ( structure.find( "$Activities" ) != string::npos ) {
		do {
			traceFile->ReadString( str );
			s = str;
			if ( !s.empty() )
				addOperation( s );
		} while ( s != "" );
		do {
			traceFile->ReadString( str );
			s = str;
			if ( !s.empty() )
				addIrregularEvent( s );
		} while ( s != "" );
	}

	if ( structure.find( "$Watching" ) != string::npos ) {
		do {
			traceFile->ReadString( str );
			s = str;
			if ( !s.empty() )
				addResult( s );
		} while ( s != "" );
	}
}

void RDOTracer::addResourceType( string& s )
{
	RDOTracerResType* type = new RDOTracerResType( RDOTK_PERMANENT );
	int pos = s.find( ' ' );
	int endpos = s.rfind( ' ' );
	type->Name = s.substr( pos, endpos - pos );
	trim( type->Name );
	
	string pcountstr = s.substr( endpos );
	trim( pcountstr );
	int paramcount = atoi( pcountstr.c_str() );
	CString str;
	string par;
	for ( int i = 0; i < paramcount; i++ ) {
		traceFile->ReadString( str );
		par = str;
		pos = par.rfind( ' ' );
		string partype = par.substr( pos );
		trim( partype );
		int enumcount = atoi( partype.c_str() );
		RDOTracerResParamType parType;
		if ( enumcount )
			parType = RDOPT_ENUMERATIVE;
		else if ( partype == "I" )
			parType = RDOPT_INTEGER;
		else if ( partype == "R" )
			parType = RDOPT_REAL;
		RDOTracerResParamInfo* param = new RDOTracerResParamInfo( parType );
		pos = par.find( ' ', 2 );
		endpos = par.find( ' ', pos + 1 );
		param->Name = par.substr( pos, endpos - pos );
		trim( param->Name );
		
		if ( parType == RDOPT_ENUMERATIVE ) {
			string en;
			for ( int j = 0; j < enumcount; j++ ) {
				traceFile->ReadString( str );
				en = str;
				pos = en.rfind( ' ' );
				string enname = en.substr( pos );
				trim( enname );
				param->addEnumValue( enname );
			}
		}
		type->addParamInfo( param );
	}
	
	resTypes.push_back( type );
	tree->addResourceType( type );
}

void RDOTracer::addResource( string& s )
{
	int pos = s.find( ' ' );
	int endpos = s.rfind( ' ' );
	string rtpstr = s.substr( endpos );
	trim( rtpstr );
	string name = s.substr( pos, endpos - pos );
	trim( name );
	RDOTracerResource* res = new RDOTracerResource( resTypes.at( atoi( rtpstr.c_str() ) - 1 ), name );
	rtpstr = getNextValue( s );
	res->id = atoi( rtpstr.c_str() );
	
	resources.push_back( res );
	tree->addResource( res );
}

void RDOTracer::addPattern( string& s )
{
	string str = s;
	string part = getNextValue( str );
	string name = getNextValue( str );
	part = getNextValue( str );
	RDOTracerPatternKind kind;
	if ( part == "A" )
		kind = RDOPK_OPERATION;
	else if ( part == "I" )
		kind = RDOPK_IRREGULAREVENT;
	else if ( part == "R" )
		kind = RDOPK_RULE;
	else if ( part == "K" )
		kind = RDOPK_KEYBOARD;
	RDOTracerPattern* pat = new RDOTracerPattern( kind );
	pat->Name = name;
	patterns.push_back( pat );
	tree->addPattern( pat );
}

void RDOTracer::addOperation( string& s )
{
	int pos = s.find( ' ' );
	int endpos = s.rfind( ' ' );
	string patstr = s.substr( endpos );
	trim( patstr );
	RDOTracerOperation* opr = new RDOTracerOperation( patterns.at( atoi( patstr.c_str() ) -1 ) );

	patstr = s.substr( pos, endpos - pos );
	trim( patstr );
	opr->setName( patstr );
	
	operations.push_back( opr );
	tree->addOperation( opr );
}

void RDOTracer::addIrregularEvent( string& s )
{
	addOperation( s );
}

void RDOTracer::addResult( string& s )
{
	string str = s;
	string name = getNextValue( str );
	string part = getNextValue( str );
	int resid = atoi( part.c_str() );
	part = getNextValue( str );
	RDOTracerResultKind resKind;
	if ( part == "watch_par" )
		resKind = RDORK_WATCHPAR;
	else if ( part == "watch_state" )
		resKind = RDORK_WATCHSTATE;
	else if ( part == "watch_quant" )
		resKind = RDORK_WATCHQUANT;
	else if ( part == "watch_value" )
		resKind = RDORK_WATCHVALUE;
	RDOTracerResult* res = new RDOTracerResult( resKind );
	res->setName( name );
	res->id = resid;
	results.push_back( res );
	tree->addResult( res );
}

string RDOTracer::getNextString()
{
	CString res;
	if ( !traceFile->ReadString( res ) )
		res = "Unexpected EOF";
	//log->addStringToLog( res );
	return res;
}

void RDOTracer::dispathNextString( string& line )
{
	if ( line.empty() )
		return;
	if ( line == "Unexpected EOF" ) {
		statusStr = line;
		doStopTrace();
		return;
	}
	if ( line.find( "$Status", 0 ) != string::npos ) {
		line.erase( 0, 10 );
		statusStr = line;
		return;
	}
	if ( line.find( "DPS_MM", 0 ) != string::npos ) {
		doStopTrace();
		return;
	}

	string key = getNextValue( line );
	RDOTracerTimeNow* timeNow;
	if ( key != "SO" && key.find( "ST" ) == string::npos && key != "SD" && key.find( "SE" ) == string::npos )
		timeNow = addTime( getNextValue( line ) );
	else
		timeNow = timeList.back();

	if ( key == "ES" ) {
	} else if ( key == "EB" ) {
		startAction( line, timeNow );
	} else if ( key == "EF" ) {
		accomplishAction( line, timeNow );
	} else if ( key == "EI" ) {
		irregularEvent( line, timeNow );
	} else if ( key == "ER" ) {
		productionRule( line, timeNow );
	} else if ( key == "RC" || key == "SRC" ) {
		resourceCreation( line, timeNow );
	} else if ( key == "RE" || key == "SRE" ) {
		resourceElimination( line, timeNow );
	} else if ( key == "RK" || key == "SRK" ) {
		resourceChanging( line, timeNow );
	} else if ( key == "V" ) {
		resultChanging( line, timeNow );
	}/* else if ( key == _T("$Status") ) {
		textColor = s.foregroundColor;
		backColor = s.backgroundColor;
	} else if ( key.Find( "DPS", 0) != -1 ) {
		textColor = dps.foregroundColor;
		backColor = dps.backgroundColor;
	} else if ( key == _T("SB") ) {
		textColor = sb.foregroundColor;
		backColor = sb.backgroundColor;
	} else if ( key == _T("SO") ) {
		textColor = so.foregroundColor;
		backColor = so.backgroundColor;
	} else if ( key == _T("STN") ) {
		textColor = stn.foregroundColor;
		backColor = stn.backgroundColor;
	} else if ( key == _T("STD") ) {
		textColor = std.foregroundColor;
		backColor = std.backgroundColor;
	} else if ( key == _T("STR") ) {
		textColor = str.foregroundColor;
		backColor = str.backgroundColor;
	} else if ( key == _T("SRK") ) {
		textColor = srk.foregroundColor;
		backColor = srk.backgroundColor;
	} else if ( key == _T("SRC") ) {
		textColor = src.foregroundColor;
		backColor = src.backgroundColor;
	} else if ( key == _T("SRE") ) {
		textColor = sre.foregroundColor;
		backColor = sre.backgroundColor;
	} else if ( key == _T("SRK") ) {
		textColor = srk.foregroundColor;
		backColor = srk.backgroundColor;
	} else if ( key == _T("SD") ) {
		textColor = sd.foregroundColor;
		backColor = sd.backgroundColor;
	} else if ( key == _T("SES") ) {
		textColor = ses.foregroundColor;
		backColor = ses.backgroundColor;
	} else if ( key == _T("SEN") ) {
		textColor = sen.foregroundColor;
		backColor = sen.backgroundColor;
	} else if ( key == _T("SEM") ) {
		textColor = sem.foregroundColor;
		backColor = sem.backgroundColor;
	} else if ( key == _T("SEF") ) {
		textColor = sef.foregroundColor;
		backColor = sef.backgroundColor;
	} else if ( key == _T("SEU") ) {
		textColor = seu.foregroundColor;
		backColor = seu.backgroundColor;
	}*/

	//graphicsPanel->addToLog( _T("... Done\r\n\r\n") );
	updateCharts();
}

string RDOTracer::getNextValue( string& line )
{
	int posstart = line.find_first_not_of( ' ' );
	int posend = line.find_first_of( ' ', posstart );
	string res = line.substr( posstart, posend - posstart );
	line.erase( 0, posend + 1 );
	trim( res );
	return res;
}

RDOTracerTimeNow* RDOTracer::addTime( string& time )
{
	double val = atof( time.c_str() );
	bool empty = timeList.empty();
	RDOTracerTimeNow* last = NULL;
	if ( !empty ) last = timeList.back();
	if ( empty || last->time != val ) {
		RDOTracerTimeNow* timeNow = new RDOTracerTimeNow( val );
		timeList.push_back( timeNow );
		eventIndex = 0;
	} else {
		last->eventCount ++;
		eventIndex ++;
		for ( vector< RDOStudioChartDoc* >::iterator it = charts.begin(); it != charts.end(); it++ ) {
			(*it)->incTimeEventsCount( last );
		}
	}
	return timeList.back();
}

RDOTracerOperation* RDOTracer::getOperation( string& line )
{
	getNextValue( line );
	return operations.at( atoi( getNextValue( line ).c_str() ) - 1 );
}

void RDOTracer::startAction( string& line, RDOTracerTimeNow* const time  )
{
	getOperation( line )->start( time, eventIndex );
}

void RDOTracer::accomplishAction( string& line, RDOTracerTimeNow* const time  )
{
	getOperation( line )->accomplish( time, eventIndex );
}

void RDOTracer::irregularEvent( string& line, RDOTracerTimeNow* const time  )
{
}

void RDOTracer::productionRule( string& line, RDOTracerTimeNow* const time  )
{
}

RDOTracerResource* RDOTracer::getResource( string& line )
{
	getNextValue( line );
	RDOTracerResource* res = NULL;
	int findid = atoi( getNextValue( line ).c_str() );
	for ( vector< RDOTracerResource* >::iterator it = resources.begin(); it != resources.end(); it++ ) {
		if ( (*it)->id == findid && !(*it)->isErased() ) {
			res = *it;
			break;
		}
	}
	return res;
}

void RDOTracer::resourceCreation( string& line, RDOTracerTimeNow* const time  )
{
	RDOTracerResType* type = resTypes.at( atoi( getNextValue( line ).c_str() ) - 1 );
	int id = atoi( getNextValue( line ).c_str() );
	RDOTracerResource* res = new RDOTracerResource( type, format( "%s #%d", type->Name.c_str(), id ) );
	res->id = id;
	res->setParams( line, time, eventIndex );

	resources.push_back( res );
	tree->addResource( res );
}

void RDOTracer::resourceElimination( string& line, RDOTracerTimeNow* const time  )
{
	RDOTracerResource* res = getResource( line );
	res->setParams( line, time, eventIndex, true );
	res->setErased( true );
	tree->updateResource( res );
}

void RDOTracer::resourceChanging( string& line, RDOTracerTimeNow* const time  )
{
	RDOTracerResource* res = getResource( line );
	res->setParams( line, time, eventIndex );
}

void RDOTracer::resultChanging( string& line, RDOTracerTimeNow* const time  )
{
}

void RDOTracer::deleteTrace()
{
	int count = resources.size();
	for ( int i = 0; i < count; i++ ) {
		delete resources.at( i );
	}
	resources.clear();
	count = resTypes.size();
	for ( i = 0; i < count; i++ ) {
		delete resTypes.at( i );
	}
	resTypes.clear();
	count = operations.size();
	for ( i = 0; i < count; i++ ) {
		delete operations.at( i );
	}
	operations.clear();
	count = irregularEvents.size();
	for ( i = 0; i < count; i++ ) {
		delete irregularEvents.at( i );
	}
	irregularEvents.clear();
	count = patterns.size();
	for ( i = 0; i < count; i++ ) {
		delete patterns.at( i );
	}
	patterns.clear();
	count = results.size();
	for ( i = 0; i < count; i++ ) {
		delete results.at( i );
	}
	results.clear();
	/*count = timeVector.size();
	for ( i = 0; i < count; i++ ) {
		delete timeVector.at( i );
	}
	timeVector.clear();*/
	list< RDOTracerTimeNow* >::iterator it = timeList.begin();
	while ( it != timeList.end() ) {
		delete *it;
		it++;
	}
	timeList.clear();
	charts.clear();
}

void RDOTracer::clear()
{
	deleteTrace();
	tree->clear();
}

CMultiDocTemplate* RDOTracer::createDocTemplate()
{
	chartDocTemplate = new CMultiDocTemplate( IDR_CHARTTYPE, RUNTIME_CLASS(RDOStudioChartDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioChartView) );
	return chartDocTemplate;
}

RDOTracerLogCtrl* RDOTracer::createLog()
{
	log = new RDOTracerLogCtrl();
	return log;
}

RDOTracerTreeCtrl* RDOTracer::createTree()
{
	tree = new RDOTracerTreeCtrl();
	return tree;
}

void RDOTracer::startTrace()
{
	string filter = _T("Trace files (*.trc)|*.TRC||");

	CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter.c_str() );
	if ( dlg.DoModal() != IDOK )
		return;
	clear();
	log->startTrace();
	startTime = CTime::GetCurrentTime();

	traceFile = new CStdioFile( dlg.GetPathName(), CFile::modeRead | CFile::shareDenyNone );
	
	try {
		getModelStructure();
		timerID = ::SetTimer( NULL, 0, 1, traceTimerFunc );
		tracing = true;
	} catch ( CException* ) {
		if (traceFile) {
			delete traceFile;
			traceFile = NULL;
			tracing = false;
		}
		throw;
	}
}

void RDOTracer::getModelStructure()
{
	getStructureData();
}

void RDOTracer::getTraceString()
{
	string str = getNextString();
	log->addStringToLog( str );
	dispathNextString( str );
}

void RDOTracer::stopTrace()
{
	statusStr = "Trace stopped by user";
	doStopTrace();
}

void RDOTracer::doStopTrace()
{
	cleanupObjects();
	stopTime = CTime::GetCurrentTime();
	tracing = false;
}
	
const bool RDOTracer::isTracing() const
{
	return tracing;
}

RDOStudioChartDoc* RDOTracer::createNewChart()
{
	RDOStudioChartDoc* doc = NULL;
	if ( chartDocTemplate ) {
		doc = (RDOStudioChartDoc*)chartDocTemplate->OpenDocumentFile( NULL );
	}
	return doc;
}

RDOStudioChartDoc* RDOTracer::addSerieToChart( RDOTracerSerie* const serie, RDOStudioChartDoc* chart )
{
	if ( !chart )
		chart = createNewChart();
	if( chart )
		chart->addSerie( serie );
	return chart;
}

void RDOTracer::addChart( RDOStudioChartDoc* const chart )
{
	charts.push_back( chart );
}

void RDOTracer::removeChart( RDOStudioChartDoc* chart )
{
	for ( vector< RDOStudioChartDoc* >::iterator it = charts.begin(); it != charts.end(); it++ ) {
		if ( (*it) == chart ) {
			charts.erase( it );
			break;
		}
	}
}

void RDOTracer::updateCharts()
{
	for ( vector< RDOStudioChartDoc* >::iterator it = charts.begin(); it != charts.end(); it++ ) {
		(*it)->UpdateAllViews( NULL );
	}
}

RDOTracerTimeNow* RDOTracer::getTime( const int index ) const
{
	//return timeVector.at( index );
	if ( index >= timeList.size() || index < 0 )
		return NULL;
	list< RDOTracerTimeNow* >::const_iterator it = timeList.begin();
	for ( int i = 0; i < index; i++ )
		it++;
	return (*it);
}