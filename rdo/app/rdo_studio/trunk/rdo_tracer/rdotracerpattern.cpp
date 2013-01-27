/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotracerpattern.cpp
  \author    ������� �����
  \date      11.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerpattern.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- TracerPattern
// --------------------------------------------------------------------------------
TracerPattern::TracerPattern( const TracerPatternKind kind ) :
	ChartTreeItem(),
	patKind( kind )
{
}

TracerPattern::~TracerPattern()
{
}