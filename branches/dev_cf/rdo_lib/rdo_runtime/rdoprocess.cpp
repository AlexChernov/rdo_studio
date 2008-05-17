#include "pch.h"
#include "rdoprocess.h"
#include "rdocalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDOPROCProcess
// ----------------------------------------------------------------------------
RDOPROCProcess::RDOPROCProcess( const std::string& _name, RDOSimulator* sim ):
	RDOLogic( sim ),
	name( _name ),
	parent( NULL )
{
}

void RDOPROCProcess::insertChild( RDOPROCProcess* value )
{
	if ( value ) {
		child.push_back( value );
		value->parent = this;
	}
}

void RDOPROCProcess::next( RDOPROCTransact* transact )
{
	Iterator it = std::find( begin(), end(), transact->block );
	if ( it != end() ) {
		RDOPROCBlock* block = static_cast<RDOPROCBlock*>(*it);
		std::list< RDOPROCTransact* >::iterator it_res = std::find( block->transacts.begin(), block->transacts.end(), transact );
		if ( it_res != block->transacts.end() ) {
			block->transacts.erase( it_res );
		}
		it++;
		if ( it != end() ) {
			transact->block = static_cast<RDOPROCBlock*>(*it);
			static_cast<RDOPROCBlock*>(*it)->transacts.push_back( transact );
		} else {
			// run-time error
		}
	} else {
		// run-time error
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTransact
// ----------------------------------------------------------------------------
int RDOPROCTransact::typeID = -1;

RDOPROCTransact::RDOPROCTransact( RDOSimulator* sim, RDOPROCBlock* _block ):
	RDOResource( static_cast<RDORuntime*>(sim), -1, typeID, true ),
	block( _block )
{
	static_cast<RDORuntime*>(sim)->insertNewResource( this );
	setTrace( true );
	m_temporary = true;
	m_state     = RDOResource::CS_Create;
	m_params.push_back( sim->getCurrentTime() );
}

void RDOPROCTransact::next()
{
	block->process->next( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCResource
// ----------------------------------------------------------------------------
RDOPROCResource::RDOPROCResource( RDORuntime* _runtime, int _number, unsigned int type, bool _trace ):
	RDOResource( _runtime, _number, type, _trace )
{
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlock
// ----------------------------------------------------------------------------
RDOPROCBlock::RDOPROCBlock( RDOPROCProcess* _process ):
	RDOBaseOperation( _process ),
	process( _process )
{
	process->append( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCGenerate
// ----------------------------------------------------------------------------
void RDOPROCGenerate::onStart( RDOSimulator* sim )
{
	calcNextTimeInterval( sim );
}

bool RDOPROCGenerate::onCheckCondition( RDOSimulator* sim )
{
	return sim->getCurrentTime() >= timeNext ? true : false;
}

RDOBaseOperation::BOResult RDOPROCGenerate::onDoOperation( RDOSimulator* sim )
{
	TRACE( "%7.1f GENERATE\n", sim->getCurrentTime() );
	calcNextTimeInterval( sim );
	RDOPROCTransact* transact = new RDOPROCTransact( sim, this );
	RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
	if ( !tracer->isNull() ) {
		tracer->getOStream() << transact->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
	}
	transact->next();
	return RDOBaseOperation::BOR_done;
}

void RDOPROCGenerate::calcNextTimeInterval( RDOSimulator* sim )
{
	sim->addTimePoint( timeNext = timeCalc->calcValue( static_cast<RDORuntime*>(sim) ).getDouble() + sim->getCurrentTime(), process, this );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize( RDOPROCProcess* _process, int _rss_id ):
	RDOPROCBlock( _process ),
	rss( NULL ),
	rss_id( _rss_id )
{
}

void RDOPROCBlockForSeize::onStart( RDOSimulator* sim )
{
	// todo: ���� ����������� ������ �������, ��������� ������� ����� ������ ������ ��,
	// ��� ��� DPT search, �� ������������� ��������� ���� ����� ������ � onCheckCondition
	rss = static_cast<RDORuntime*>(sim)->getResourceByID( rss_id );
	enum_free = RDOValue( rss->getParam(0).getEnum(), RDOPROCBlockForSeize::getStateEnumFree() );
	enum_buzy = RDOValue( rss->getParam(0).getEnum(), RDOPROCBlockForSeize::getStateEnumBuzy() );
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
bool RDOPROCSeize::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
		RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
		if ( !tracer->isNull() ) {
			tracer->getOStream() << rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
		}
		// ��������
		if ( rss->getParam(0) == enum_free ) {
			return true;
		} else {
			TRACE( "%7.1f SEIZE CANNOT\n", sim->getCurrentTime() );
		}
	}
	return false;
}

RDOBaseOperation::BOResult RDOPROCSeize::onDoOperation( RDOSimulator* sim )
{
	TRACE( "%7.1f SEIZE\n", sim->getCurrentTime() );
	rss->setParam(0, enum_buzy);
	transacts.front()->next();
	return RDOBaseOperation::BOR_done;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
bool RDOPROCRelease::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
		RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
		if ( !tracer->isNull() ) {
			tracer->getOStream() << rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
		}
		// �����
		if ( rss->getParam(0) == enum_buzy ) {
			return true;
		} else {
			TRACE( "%7.1f RELEASE CANNOT\n", sim->getCurrentTime() );
		}
	}
	return false;
}

RDOBaseOperation::BOResult RDOPROCRelease::onDoOperation( RDOSimulator* sim )
{
	TRACE( "%7.1f RELEASE\n", sim->getCurrentTime() );
	rss->setParam(0, enum_free);
	transacts.front()->next();
	return RDOBaseOperation::BOR_done;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCAdvance
// ----------------------------------------------------------------------------
bool RDOPROCAdvance::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
		return true;
	} else if ( !leave_list.empty() ) {
		double tnow = sim->getCurrentTime();
		std::list< LeaveTr >::iterator it = leave_list.begin();
		while ( it != leave_list.end() ) {
			if ( tnow >= it->timeLeave ) {
				return true;
			}
			it++;
		}
	}
	return false;
}

RDOBaseOperation::BOResult RDOPROCAdvance::onDoOperation( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
		TRACE( "%7.1f ADVANCE BEGIN\n", sim->getCurrentTime() );
		double timeLeave = delayCalc->calcValue( static_cast<RDORuntime*>(sim) ).getDouble() + sim->getCurrentTime();
		leave_list.push_back( LeaveTr(transacts.front(), timeLeave) );
		transacts.erase( transacts.begin() );
		sim->addTimePoint( timeLeave, process, this );
//		RDOTrace* tracer = sim->getTracer();
//		if ( !tracer->isNull() ) {
//			tracer->getOStream() << res->traceResourceState('\0', sim) << tracer->getEOL();
//		}
		return RDOBaseOperation::BOR_done;
	} else if ( !leave_list.empty() ) {
		double tnow = sim->getCurrentTime();
		std::list< LeaveTr >::iterator it = leave_list.begin();
		while ( it != leave_list.end() ) {
			if ( tnow >= it->timeLeave ) {
//				RDOTrace* tracer = sim->getTracer();
//				if ( !tracer->isNull() ) {
//					tracer->getOStream() << res->traceResourceState('\0', sim) << tracer->getEOL();
//				}
				TRACE( "%7.1f ADVANCE END\n", it->timeLeave );
				it->transact->next();
				leave_list.erase( it );
				return RDOBaseOperation::BOR_planned_and_run;
			}
			it++;
		}
	}
	return RDOBaseOperation::BOR_cant_run;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCTerminate
// ----------------------------------------------------------------------------
bool RDOPROCTerminate::onCheckCondition( RDOSimulator* sim )
{
	return !transacts.empty() ? true : false;
}

RDOBaseOperation::BOResult RDOPROCTerminate::onDoOperation( RDOSimulator* sim )
{
	TRACE( "%7.1f TERMINATE\n", sim->getCurrentTime() );
	RDOPROCTransact* transact = transacts.front();
	transact->setState( RDOResource::CS_Erase );
	RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
	if ( !tracer->isNull() ) {
		tracer->getOStream() << transact->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
	}
	static_cast<RDORuntime*>(sim)->onEraseRes( transact->getTraceID(), NULL );
	transacts.erase( transacts.begin() );
	return RDOBaseOperation::BOR_done;
}

} // rdoRuntime
