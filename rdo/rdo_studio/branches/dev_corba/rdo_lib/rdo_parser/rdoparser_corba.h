#ifndef RDOPARSER_CORBA_H
#define RDOPARSER_CORBA_H

#include "rdoparser_rdo.h"

#ifdef CORBA_ENABLE

namespace rdoParse
{

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRTP
// ----------------------------------------------------------------------------
class RDOParserCorbaRTP: public RDOParserRDOItem
{
public:
	RDOParserCorbaRTP( RDOParser* _parser ): RDOParserRDOItem( _parser, rdoModelObjects::RTP, NULL, NULL, NULL ) {};
	virtual void parse();
};

// ----------------------------------------------------------------------------
// ---------- RDOParserCorbaRSS
// ----------------------------------------------------------------------------
class RDOParserCorbaRSS: public RDOParserRDOItem
{
public:
	RDOParserCorbaRSS( RDOParser* _parser ): RDOParserRDOItem( _parser, rdoModelObjects::RSS, NULL, NULL, NULL ) {};
	virtual void parse();
};

} // namespace rdoParse

#endif // CORBA_ENABLE
#endif // RDOPARSER_CORBA_H
