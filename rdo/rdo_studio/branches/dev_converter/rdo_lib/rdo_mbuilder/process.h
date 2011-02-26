/*
 * copyright: (c) RDO-Team, 2010
 * filename : process.h
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _MBUILDER_PROCESS_H_
#define _MBUILDER_PROCESS_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_mbuilder/namespace.h"
#include "rdo_lib/rdo_mbuilder/rdo_resources.h"
// ===============================================================================

OPEN_MBUILDER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- BlockForQueue
// ----------------------------------------------------------------------------
class BlockForQueue
{
public:
	static rbool      checkType (RDOResType rtp,         CREF(rdoParse::RDOParserSrcInfo) info    );
	static void       createRes (RDOResType rtp,         CREF(tstring)                    res_name);
	static RDOResType createType(CREF(tstring) rtp_name, CREF(rdoParse::RDOParserSrcInfo) info    );
};

// ----------------------------------------------------------------------------
// ---------- BlockForSeize
// ----------------------------------------------------------------------------
class BlockForSeize
{
public:
	static rbool      checkType  (RDOResType    rtp,      CREF(rdoParse::RDOParserSrcInfo) info    );
	static void       createRes  (RDOResType    rtp,      CREF(tstring)                    res_name);
	static void       reobjectRes(RDOResType    rtp,      CREF(tstring)                    res_name);
	static RDOResType createType (CREF(tstring) rtp_name, CREF(rdoParse::RDOParserSrcInfo) info    );
};

CLOSE_MBUILDER_NAMESPACE

#endif //! _MBUILDER_PROCESS_H_
