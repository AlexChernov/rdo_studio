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
	if ( transact->block )
	{
	Iterator it = std::find( begin(), end(), transact->block );
	//���� � ��������� ���� ����
		if ( it != end() ) {
		//����� ���� ����
		RDOPROCBlock* block = static_cast<RDOPROCBlock*>(*it);
		//������� ������������ �������� � ������ ��� ����������
		std::list< RDOPROCTransact* >::iterator it_res = std::find( block->transacts.begin(), block->transacts.end(), transact );
			//���� �������� ������
			if ( it_res != block->transacts.end() ) {
			//������� ��� �� ������ ���������� ����� �����
			block->TransactGoOut( *it_res );
			}
			else{
				//������ ����� ����� �� ����� ������, ��������� advance ������� ��������� �� ����
//				getRuntime()->error( "���������� ������: ���������� �������� (�������� ������� ���� ����)" );
			}
			//��������� � ���������� �����
			it++;
			//����� ���� ����
			block = static_cast<RDOPROCBlock*>(*it);	
				
			//���� ��������� ���� ����������
			if ( it != end() ) {
			transact->block = block;
			//���������� � ����� ������ ����� ����� ������������ ��������
			block->TransactGoIn( transact );
			}
			//���� � �� �������� ����� ���� ����������� �������� ��� ���������
			else {
			//---------���� � ���� ���� ��������, ��� it-1 = ��������� ���� ��� ���������, 
			//---------� ���� ������ ��������� ����� ������� ���� ��������?????????
				//getRuntime()->error( "������ ���������� ��������, ���� � ������� �� ��� - ���������" );
			}
		} 
		else {
			//getRuntime()->error( "�� ���������� ������ �����, � �������� �������� ��������" );
		}
	} 
	else 
	{
		//getRuntime()->error( "� ��������� �� �������� ������� ����" );
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

RDOPROCBlock* RDOPROCTransact::getBlock()
{
return this->block;
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


void RDOPROCBlock::TransactGoIn( RDOPROCTransact* _transact )
{
	transacts.push_back( _transact );
}

void RDOPROCBlock::TransactGoOut( RDOPROCTransact* _transact )
{
	transacts.remove( _transact );
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
	//this->TransactGoIn(transact);
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
// ---------- RDOPROCQueue
// ----------------------------------------------------------------------------
RDOPROCQueue::RDOPROCQueue( RDOPROCProcess* _process, parser_for_Queue From_Par):
	RDOPROCBlock( _process ),
	fromParser  ( From_Par )
{
}
void RDOPROCQueue::onStart( RDOSimulator* sim )
{
	int Id_res = fromParser.Id_res;
	int Id_param = fromParser.Id_param;
	RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( Id_res );
	forRes.Id_param = Id_param;
	forRes.rss = static_cast<RDOPROCResource*>(res);
	forRes.defaultValue = RDOValue( RDOPROCQueue::getDefaultValue() );
}

bool RDOPROCQueue::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) 
	{
		RDOValue i = forRes.rss->getParam( forRes.Id_param );
		RDOValue j = RDOValue( int (1) );
		forRes.rss->setParam( forRes.Id_param, i + j );	
		return true;
	}
	else
	{
	return false;
	}
}

RDOBaseOperation::BOResult RDOPROCQueue::onDoOperation( RDOSimulator* sim )
{
	TRACE( "%7.1f QUEUE\n", sim->getCurrentTime() );
	transacts.front()->next();
	return RDOBaseOperation::BOR_done;
}

// ----------------------------------------------------------------------------
// ---------- RDOPROCBlockForSeize
// ----------------------------------------------------------------------------
RDOPROCBlockForSeize::RDOPROCBlockForSeize( RDOPROCProcess* _process, parser_for_Seize From_Par ):
	RDOPROCBlock( _process ),
	fromParser ( From_Par)
{
}

void RDOPROCBlockForSeize::onStart( RDOSimulator* sim )
{
	// todo: ���� ����������� ������ �������, ��������� ������� ����� ������ ������ ��,
	// ��� ��� DPT search, �� ������������� ��������� ���� ����� ������ � checkOperation
	int Id_res = fromParser.Id_res;
	int Id_param = fromParser.Id_param;
	RDOResource* res = static_cast<RDORuntime*>(sim)->getResourceByID( Id_res );
	forRes.Id_param = Id_param;
	forRes.rss = static_cast<RDOPROCResource*>(res);
	forRes.enum_free = RDOValue( forRes.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumFree() );
	forRes.enum_buzy = RDOValue( forRes.rss->getParam(Id_param).getEnum(), RDOPROCBlockForSeize::getStateEnumBuzy() );
}


// ----------------------------------------------------------------------------
// ---------- RDOPROCSeize
// ----------------------------------------------------------------------------
bool RDOPROCSeize::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
	int idBlocksTransact = transacts.front()->getTraceID();	
	int idResourcesTransact = forRes.rss->transacts.front()->getTraceID();
		if( idBlocksTransact == idResourcesTransact ){
		RDOTrace* tracer = static_cast<RDORuntime*>( sim)->getTracer();
			if ( !tracer->isNull() ) {
			tracer->getOStream() << forRes.rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
			}
			// �������� ������, ���� ��������
			if ( forRes.rss->getParam(forRes.Id_param) == forRes.enum_free ) {
			forRes.rss->setParam(forRes.Id_param, forRes.enum_buzy);	
			TRACE( "%7.1f SEIZE-%d\n", sim->getCurrentTime() ,index );
			return true;
			} 
			else {
			return false;			
			}
		}
		else {
		return false;			
		}
	}
return false;
}
		
RDOBaseOperation::BOResult RDOPROCSeize::onDoOperation( RDOSimulator* sim )
{
transacts.front()->next();
return RDOBaseOperation::BOR_done;
}

void RDOPROCSeize::TransactGoIn( RDOPROCTransact* _transact )
{
	forRes.rss->transacts.push_back( _transact );
	RDOPROCBlockForSeize::TransactGoIn( _transact );
}

void RDOPROCSeize::TransactGoOut( RDOPROCTransact* _transact )
{
	forRes.rss->transacts.remove( _transact );
	RDOPROCBlockForSeize::TransactGoOut( _transact );
}
// ----------------------------------------------------------------------------
// ---------- RDOPROCRelease
// ----------------------------------------------------------------------------
bool RDOPROCRelease::onCheckCondition( RDOSimulator* sim )
{
	if ( !transacts.empty() ) {
	RDOTrace* tracer = static_cast<RDORuntime*>(sim)->getTracer();
		if ( !tracer->isNull() ) {
		tracer->getOStream() << forRes.rss->traceResourceState('\0', static_cast<RDORuntime*>(sim)) << tracer->getEOL();
		}
		// �����
		if ( forRes.rss->getParam(forRes.Id_param) == forRes.enum_buzy ) {
		forRes.rss->setParam(forRes.Id_param, forRes.enum_free);
		return true;
		}
	}
return false;
}

RDOBaseOperation::BOResult RDOPROCRelease::onDoOperation( RDOSimulator* sim )
{
	TRACE( "%7.1f RELEASE\n", sim->getCurrentTime() );
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
	} 
	else
		if ( !leave_list.empty() ) {
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
		return RDOBaseOperation::BOR_done;
	} 
	else	if ( !leave_list.empty() ) {
			double tnow = sim->getCurrentTime();
			std::list< LeaveTr >::iterator it = leave_list.begin();
				while ( it != leave_list.end() ) {
					if ( tnow >= it->timeLeave ) {
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
