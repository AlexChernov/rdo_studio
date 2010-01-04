/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdofile.h
 * author   : ������ ������
 * date     : 10.05.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOFILE_H_
#define _RDOFILE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
// ===============================================================================

OPEN_RDO_NAMESPACE

class File
{
public:
	static rbool exist    (CREF(tstring) name);
	static rbool read_only(CREF(tstring) name);
	static rbool unlink   (CREF(tstring) name);
};

CLOSE_RDO_NAMESPACE

#include "rdo_common/rdofile.inl"

#endif //! _RDOFILE_H_
