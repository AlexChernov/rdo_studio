/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser_rdo.h
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_RDO_H_
#define _RDOPARSER_RDO_H_

// ====================================================================== INCLUDES
#include <iostream>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser_base.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOParserRDOItem
// ----------------------------------------------------------------------------
PREDECLARE_POINTER(RDOParser);

class RDOParserRDOItem: public RDOParserItem
{
DECLARE_FACTORY(RDOParserRDOItem);
public:
	virtual void  parse         (CREF(LPRDOParser) pParser);
	virtual ruint lexer_loc_line();
	virtual ruint lexer_loc_pos ();

protected:
	RDOParserRDOItem(rdoModelObjects::RDOFileType _type, t_bison_parse_fun _parser_fun, t_bison_error_fun _error_fun, t_flex_lexer_fun _lexer_fun, StreamFrom from = sf_repository);
	virtual ~RDOParserRDOItem();

	PTR(RDOLexer)  m_pLexer;
	YYLTYPE        m_loc;

private:
	void parse(CREF(LPRDOParser) pParser, REF(std::istream) in_stream);
	PTR(RDOLexer) getLexer(CREF(LPRDOParser) pParser, PTR(std::istream) in_stream, PTR(std::ostream) out_stream);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserRSS
// ----------------------------------------------------------------------------
class RDOParserRSS: public RDOParserRDOItem
{
DECLARE_FACTORY(RDOParserRSS);
private:
	RDOParserRSS(StreamFrom from = sf_repository);
	virtual void parse(CREF(LPRDOParser) pParser);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserRSSPost
// ----------------------------------------------------------------------------
class RDOParserRSSPost: public RDOParserItem
{
DECLARE_FACTORY(RDOParserRSSPost);
private:
	RDOParserRSSPost()
		: RDOParserItem(rdoModelObjects::RSS, NULL, NULL, NULL)
	{}
	virtual void parse(CREF(LPRDOParser) pParser);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSMRPost
// ----------------------------------------------------------------------------
class RDOParserSMRPost: public RDOParserItem
{
DECLARE_FACTORY(RDOParserSMRPost);
private:
	RDOParserSMRPost();
	virtual void parse(CREF(LPRDOParser) pParser);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserEVNPost
// ----------------------------------------------------------------------------
class RDOParserEVNPost: public RDOParserItem
{
DECLARE_FACTORY(RDOParserEVNPost);
private:
	RDOParserEVNPost()
		: RDOParserItem(rdoModelObjects::PAT, NULL, NULL, NULL)
	{}
	virtual void parse(CREF(LPRDOParser) pParser);
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSTDFUN
// ----------------------------------------------------------------------------
class RDOParserSTDFUN: public RDOParserItem
{
DECLARE_FACTORY(RDOParserSTDFUN);
private:
	RDOParserSTDFUN()
		: RDOParserItem(rdoModelObjects::FUN, NULL, NULL, NULL)
	{}
	virtual void parse(CREF(LPRDOParser) pParser);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_RDO_H_
