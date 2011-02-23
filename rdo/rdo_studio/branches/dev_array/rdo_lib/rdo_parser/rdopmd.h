/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdopmd.h
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPMD_PMD_H_
#define _RDOPMD_PMD_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_lib/rdo_runtime/rdopokaz.h"
#include "rdo_lib/rdo_parser/rdo_object.h"
#include "rdo_lib/rdo_parser/rdofun.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

int  pmdparse(PTR(void) lexer);
int  pmdlex  (PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer);
void pmderror(PTR(char) mes);

// ----------------------------------------------------------------------------
// ---------- RDOPMDPokaz
// ----------------------------------------------------------------------------
OBJECT(RDOPMDPokaz) IS INSTANCE_OF(RDOParserSrcInfo)
{
DECLARE_FACTORY(RDOPMDPokaz);
public:
	CREF(tstring) name() const { return src_text(); }

protected:
	RDOPMDPokaz(CREF(RDOParserSrcInfo) src_info);
	virtual ~RDOPMDPokaz();

	void endOfCreation(CREF(LPIPokaz) pPokaz);

	LPIPokaz m_pPokaz;
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchPar
// ----------------------------------------------------------------------------
class RDOPMDWatchPar: public RDOPMDPokaz
{
DECLARE_FACTORY(RDOPMDWatchPar);
private:
	RDOPMDWatchPar(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_src_info, CREF(RDOParserSrcInfo) par_src_info);
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchState
// ----------------------------------------------------------------------------
class RDOPMDWatchState: public RDOPMDPokaz
{
DECLARE_FACTORY(RDOPMDWatchState);
private:
	RDOPMDWatchState(CREF(RDOParserSrcInfo) src_info, rbool trace, LPRDOFUNLogic pLogic);
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchTemp
// ----------------------------------------------------------------------------
class RDOPMDWatchTemp: public RDOPMDPokaz
{
protected:
	RDOPMDWatchTemp(CREF(RDOParserSrcInfo) src_info, CREF(RDOParserSrcInfo) res_type_src_info);
};

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchQuant
// ----------------------------------------------------------------------------
class RDOPMDWatchQuant: public RDOPMDWatchTemp
{
DECLARE_FACTORY(RDOPMDWatchQuant);
public:
	void setLogic       (REF(LPRDOFUNLogic) pLogic);
	void setLogicNoCheck();

private:
	RDOPMDWatchQuant(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_type_src_info);
};
DECLARE_POINTER(RDOPMDWatchQuant);

// ----------------------------------------------------------------------------
// ---------- RDOPMDWatchValue
// ----------------------------------------------------------------------------
class RDOPMDWatchValue: public RDOPMDWatchTemp
{
DECLARE_FACTORY(RDOPMDWatchValue);
public:
	void setLogic       (REF(LPRDOFUNLogic) pLogic, REF(LPRDOFUNArithm) pArithm);
	void setLogicNoCheck(REF(LPRDOFUNArithm) pArithm);

private:
	RDOPMDWatchValue(CREF(RDOParserSrcInfo) src_info, rbool trace, CREF(RDOParserSrcInfo) res_type_src_info);
};
DECLARE_POINTER(RDOPMDWatchValue);

// ----------------------------------------------------------------------------
// ---------- RDOPMDGetValue
// ----------------------------------------------------------------------------
class RDOPMDGetValue: public RDOPMDPokaz
{
DECLARE_FACTORY(RDOPMDGetValue);
private:
	RDOPMDGetValue(CREF(RDOParserSrcInfo) src_info, LPRDOFUNArithm pArithm);
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPMD_PMD_H_
