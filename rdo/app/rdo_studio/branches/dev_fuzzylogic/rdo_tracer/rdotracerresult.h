/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerresult.h
  \author    ������� �����
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_RDOTRACERRESULT_H_
#define _RDO_STUDIO_MFC_TRACER_RDOTRACERRESULT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerserie.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOTracerResult
// --------------------------------------------------------------------------------
enum RDOTracerResultKind {
	RDORK_UNDEFINED  = ~0,
	RDORK_WATCHPAR   = 0,
	RDORK_WATCHSTATE,
	RDORK_WATCHQUANT,
	RDORK_WATCHVALUE
};

class RDOTracerResult: public RDOTracerSerie
{
protected:
	RDOTracerResultKind resultKind;
	tstring Name;
public:
	RDOTracerResult( const RDOTracerResultKind kind );
	virtual ~RDOTracerResult();

	int id;
	RDOTracerResultKind getResultKind() const { return resultKind; };
	void setName( CREF(tstring) name ) { Name = name; if ( title.empty() ) title = Name; };
	const tstring getName() const { return Name; };
	virtual void getCaptions( std::vector<tstring> &captions, const int val_count ) const;
	void setValue( tstring& line, RDOTracerTimeNow* const time, const int eventIndex );
};

#endif // _RDO_STUDIO_MFC_TRACER_RDOTRACERRESULT_H_
