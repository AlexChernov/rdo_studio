#include "stdafx.h"
#include "rdotracerresource.h"
#include "rdotracerrestype.h"
#include "rdotracervalues.h"
#include "rdotracer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOTracerResParam
// ----------------------------------------------------------------------------
RDOTracerResParam::RDOTracerResParam( RDOTracerResource* const res ) :
	RDOTracerSerie( RDOST_RESPARAM ),
	resource( res )
{
}

RDOTracerResParam::~RDOTracerResParam()
{
}

RDOTracerResParamInfo* RDOTracerResParam::getParamInfo() const
{
	int index = resource->getParamIndex( this );
	if ( index != -1 )
		return resource->getType()->getParamInfo( index );
	else
		return NULL;
}

void RDOTracerResParam::getCaptions( std::vector<std::string> &captions, const int val_count ) const
{
	const_cast<CMutex&>(mutex).Lock();

	switch( getParamInfo()->getParamType() ) {
		case RDOPT_INTEGER: {
			RDOTracerSerie::getCaptionsInt( captions, val_count );
			break;
		}
		case RDOPT_REAL: {
			RDOTracerSerie::getCaptionsDouble( captions, val_count );
			break;
		}
		case RDOPT_ENUMERATIVE: {
			RDOTracerSerie::getCaptions( captions, val_count );
			int delta = getParamInfo()->getEnumCount();
			minValue = 0;
			maxValue = delta - 1;
			int real_val_count = val_count;
			if ( delta > real_val_count ) {
				while ( (int)(( delta - 1 ) / ( real_val_count - 1 )) != ( (double)(delta - 1) / ( real_val_count - 1 ) ) )
					real_val_count--;
			} else {
				real_val_count = delta;
			}
			int valo = minValue;
			int valoffset = ( delta - 1 ) / ( real_val_count - 1 );
			for ( int i = 0; i < real_val_count; i++ ) {
				captions.push_back( getParamInfo()->getEnumValue( valo ) );
				valo += valoffset;
			}
			break;
		}
	}

	const_cast<CMutex&>(mutex).Unlock();
}

// ----------------------------------------------------------------------------
// ---------- RDOTracerResource
// ----------------------------------------------------------------------------
RDOTracerResource::RDOTracerResource( RDOTracerResType* const type, std::string& name ) :
	RDOTracerTreeItem(),
	erased( false ),
	resType( type ),
	Name( name ),
	id( 0 )
{
	mutex.Lock();

	int count = resType->getParamsCount();
	for ( int i = 0; i < count; i++ ) {
		addParam( new RDOTracerResParam( this ) );
	}

	mutex.Unlock();
}

RDOTracerResource::~RDOTracerResource()
{
	mutex.Lock();

	int count = params.size();
	for ( int i = 0; i < count; i++ ) {
		delete params.at( i );
	}

	mutex.Unlock();
}

void RDOTracerResource::addParam( RDOTracerResParam* const value )
{
	mutex.Lock();

	value->setTitle( Name + "." + resType->getParamInfo( params.size() )->Name );
	params.push_back( value );

	mutex.Unlock();
}

RDOTracerResParam* RDOTracerResource::getParam( const int index ) const
{
	if ( index >= params.size() || index < 0 )
		return NULL;
	return params.at( index );
}

int RDOTracerResource::getParamIndex( const RDOTracerResParam* const param ) const
{
	int count = params.size();
	for ( int i = 0; i < count; i++ ) {
		if ( params.at( i ) == param )
			return i;
	}
	return -1;
}

void RDOTracerResource::setParams( std::string& line, RDOTracerTimeNow* const time, const int eventIndex, const bool erasing )
{
	int count = params.size();
	for ( int i = 0; i < count; i++ ) {
		RDOTracerValue* prevval;
		params.at( i )->getLastValue( prevval );
		double newval = erasing ? prevval->value : atof( tracer->getNextValue( line ).c_str() );
		if ( !prevval || erasing || prevval->value != newval ) {
			RDOTracerValue* newvalue = new RDOTracerValue( time, eventIndex );
			newvalue->value = newval;
			params.at( i )->addValue( newvalue );
		}
	}
}

void RDOTracerResource::setErased( const bool value )
{
	if ( erased != value )
		erased = value;
}
