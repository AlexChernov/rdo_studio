#include "stdafx.h"
#include "rdotracerfilereader.h"
#include "../resource.h"
#include "rdotracerapp.h"
#include "./trace_files/rdotracer.h"
#include "./trace_files/rdotracerexception.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

RDOTracerFileReader trace_reader;

// ----------------------------------------------------------------------------
// ---------- RDOTracerFileReader
// ----------------------------------------------------------------------------
RDOTracerFileReader::RDOTracerFileReader()
	: tracing( false ),
	  trace_file( NULL ),
	  structure( NULL ),
	  delay( 30 )
{
}

RDOTracerFileReader::~RDOTracerFileReader()
{
	stopTrace();
}

// sting is declared here to avoid memory leaks
// when killing a thread
string str;

UINT RunningThreadControllingFunction( LPVOID pParam )
{
	trace_reader.trace_file = new ifstream;
	trace_reader.structure = new stringstream;
	try {
		if ( !trace_reader.trace_file || !trace_reader.structure )
			throw RDOTracerException( format( IDS_TRACEINITERROR ).c_str() );;

		trace_reader.trace_file->open( trace_reader.file_name.c_str() );

		do {
			*(trace_reader.trace_file) >> str;
		} while( str != "Model_name" && !trace_reader.trace_file->eof() );
		if ( str != "Model_name" && trace_reader.trace_file->eof() )
			throw RDOTracerException( format( IDS_MODELNAMEERROR ).c_str() );
		*(trace_reader.trace_file) >> str;
		*(trace_reader.trace_file) >> trace_reader.model_name;
		rdoTracerApp.mainFrame->setModelName( trace_reader.model_name );

		do {
			*(trace_reader.trace_file) >> str;
		} while( str != "$Resource_type" && !trace_reader.trace_file->eof() );
		if ( str != "$Resource_type" && trace_reader.trace_file->eof() )
			throw RDOTracerException( format( IDS_MODELSTRUCTERROR ).c_str() );

		do {
			*(trace_reader.structure) << str << " ";
			*(trace_reader.trace_file) >> str;
		} while ( str != "$Tracing" && !trace_reader.trace_file->eof() );
		if ( str != "$Tracing" && trace_reader.trace_file->eof() )
			throw RDOTracerException( format( IDS_TRACINGERROR ).c_str() );

		trace_reader.getNextLine();

		tracer->beforeModelStartNotify();

		trace_reader.tracing = true;

		while ( !trace_reader.trace_file->eof() ) {
			tracer->traceStringNotify( trace_reader.getNextLine() );
			if ( rdoTracerApp.mainFrame->getShowMode() != SM_NoShow && !trace_reader.trace_file->eof() )
				::Sleep( trace_reader.delay );
		}
	} catch ( RDOTracerException &e ) {
		//AfxMessageBox( format( IDS_FILEREADEREXCEPTION, trace_reader.file_name.c_str(), trace_reader.model_name.c_str(), e.getMessage().c_str() ).c_str() );
		::MessageBox( rdoTracerApp.mainFrame->GetSafeHwnd(), format( IDS_FILEREADEREXCEPTION, trace_reader.file_name.c_str(), trace_reader.model_name.c_str(), e.getMessage().c_str() ).c_str(), NULL, MB_ICONEXCLAMATION | MB_OK );
	}

	trace_reader.th = NULL;

	if ( trace_reader.trace_file ) {
		if ( trace_reader.trace_file->is_open() ) {
			trace_reader.trace_file->close();
		}
		delete trace_reader.trace_file;
		trace_reader.trace_file = NULL;
	}

	if ( trace_reader.structure ) {
		delete trace_reader.structure;
		trace_reader.structure = NULL;
	}

	trace_reader.tracing = false;

	tracer->modelStoppedNotify();

	return 0;
}

string RDOTracerFileReader::getNextLine()
{
	if ( trace_file && trace_file->is_open() ) {
		char str[4096];
		::FillMemory( &str[0], 4096, '\n' );
		trace_file->getline( &str[0], 4096 );
		return str;
	} else
		return "";

}

void RDOTracerFileReader::startTrace()
{
	string filter = format( IDS_TRACE_FILTER );

	CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter.c_str() );
	if ( dlg.DoModal() != IDOK )
		return;

	file_name = dlg.GetPathName();
	rdoTracerApp.mainFrame->getTab()->deleteAllItems();

	th = AfxBeginThread( RunningThreadControllingFunction, (LPVOID)NULL );
}

void RDOTracerFileReader::stopTrace()
{
	if( th != NULL ) {
		::TerminateThread(th->m_hThread, -1);
		delete th;
		th = NULL;
		
		if ( trace_file ) {
			if ( trace_file->is_open() ) {
				trace_file->close();
			}
			delete trace_file; trace_file = NULL;
		}

		if ( structure ) {
			delete structure; structure = NULL;
		}
		
		tracing = false;

		tracer->modelStoppedNotify();
	}

}

void RDOTracerFileReader::pauseTrace()
{
	th->SuspendThread();
}

void RDOTracerFileReader::unpauseTrace()
{
	th->ResumeThread();
}