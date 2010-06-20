#include "rdo_lib/rdo_parser/pch.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_parser/rdofrm.h"
#include "rdo_lib/rdo_parser/rdodpt.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_lib/rdo_runtime/rdo_random_distribution.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdoframe.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"

namespace rdoParse 
{

int funlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void funerror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOFUNDoubleToIntByResult
// ----------------------------------------------------------------------------
void RDOFUNDoubleToIntByResult::roundCalc()
{
	STL_FOR_ALL(CalcList, m_int_or_double, it)
	{
		(*it)->needRound();
	}
}

// ----------------------------------------------------------------------------
// ---------- ����� �������� ��� ��������� ���������� � �������������� ���������
// ----------------------------------------------------------------------------
#define CREATE_CALC( CALC, OPR ) \
rdoRuntime::LPRDOCalcConst pConstCalc1 = m_pCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>(); \
rdoRuntime::LPRDOCalcConst pConstCalc2 = second.m_pCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>(); \
rdoRuntime::LPRDOCalc pNewCalc; \
if (pConstCalc1 && pConstCalc2) \
{ \
	pNewCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(pConstCalc1->calcValue(parser()->runtime()) OPR pConstCalc2->calcValue(parser()->runtime())).object_cast<rdoRuntime::RDOCalc>(); \
	pNewCalc->setSrcInfo(rdoRuntime::RDOCalc##CALC::getStaticSrcInfo(pConstCalc1.object_cast<rdoRuntime::RDOCalc>(), pConstCalc2.object_cast<rdoRuntime::RDOCalc>())); \
} \
else \
{ \
	pNewCalc = rdo::Factory<rdoRuntime::RDOCalc##CALC>::create(m_pCalc, second.m_pCalc).object_cast<rdoRuntime::RDOCalc>(); \
}

#define RETURN_LOGIC() \
RDOFUNLogic* logic = new RDOFUNLogic( this, pNewCalc ); \
logic->setSrcInfo( pNewCalc->src_info() ); \
logic->m_int_or_double.insert( m_int_or_double, second.m_int_or_double ); \
return logic;

#define GENERATE_LOGIC( CALC, OPR ) \
CREATE_CALC( CALC, OPR ); \
RETURN_LOGIC();

#define CAST_ARITHM_VALUE( OPR, ERROR ) \
try \
{ \
	if ( beforeCastValue(second) == CR_CONTINUE ) \
	{ \
		value().value() OPR second.value().value(); \
	} \
} \
catch ( rdoRuntime::RDOValueException& ) \
{ \
	parser()->error().error( second, rdo::format(ERROR, type()->name().c_str(), second.type()->name().c_str()) ); \
}

#define GET_ARITHM_PRE_TYPE() \
LPRDOType pNewType = getPreType( second );

#define GENERATE_ARITHM_CALC( CALC, OPR, ERROR ) \
CAST_ARITHM_VALUE( OPR, ERROR ) \
GET_ARITHM_PRE_TYPE() \
CREATE_CALC( CALC, OPR )

#define RETURN_ARITHM() \
RDOFUNArithm* arithm = new RDOFUNArithm( this, RDOValue(pNewCalc->calcValue(parser()->runtime()), pNewType, pNewCalc->src_info()), pNewCalc ); \
arithm->m_int_or_double.insert( m_int_or_double, second.m_int_or_double ); \
return arithm;

#define GENERATE_ARITHM( CALC, OPR, ERROR ) \
GENERATE_ARITHM_CALC( CALC, OPR, ERROR ) \
RETURN_ARITHM()

#define GENERATE_LOGIC_FROM_ARITHM( CALC, OPR, ERROR ) \
GENERATE_ARITHM_CALC( CALC, OPR, ERROR ) \
RETURN_LOGIC()

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic( const RDOFUNArithm& arithm ):
	RDOParserObject( &arithm ),
	RDOParserSrcInfo( arithm.src_info() ),
	m_pCalc( NULL )
{
	switch ( arithm.typeID() )
	{
		case rdoRuntime::RDOType::t_bool: m_pCalc = arithm.calc(); break;
	}
	if ( !m_pCalc )
	{
		parser()->error().error( src_info(), rdo::format("��������� ���������� ���������, �������: %s", src_text().c_str()) );
	}
}

RDOFUNLogic::RDOFUNLogic( const RDOParserObject* _parent, CREF(rdoRuntime::LPRDOCalc) pCalc, bool hide_warning ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo(),
	m_pCalc(pCalc)
{
	if ( m_pCalc )
	{
		m_pCalc->setSrcFileType( src_filetype() );
	}
	if ( !hide_warning )
	{
		rdoRuntime::LPRDOCalcConst pConstCalc = m_pCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>();
		if ( pConstCalc )
		{
			if ( pConstCalc->calcValue( parser()->runtime() ).getAsBool() )
			{
				parser()->error().warning( pConstCalc->src_info(), rdo::format("���������� ��������� ������ �������: %s", pConstCalc->src_text().c_str()) );
			}
			else
			{
				parser()->error().warning( pConstCalc->src_info(), rdo::format("���������� ��������� ������ �����: %s", pConstCalc->src_text().c_str()) );
			}
		}
	}
}

rdoRuntime::LPRDOCalc RDOFUNLogic::getCalc( rdoRuntime::RDOType::TypeID _id )
{
	if ( _id != rdoRuntime::RDOType::t_real )
	{
		m_int_or_double.roundCalc();
	}
	return m_pCalc;
}

RDOFUNLogic* RDOFUNLogic::operator &&( const RDOFUNLogic& second )
{
	GENERATE_LOGIC( And, && );
}

RDOFUNLogic* RDOFUNLogic::operator ||( const RDOFUNLogic& second )
{
	GENERATE_LOGIC( Or, || );
}

RDOFUNLogic* RDOFUNLogic::operator_not()
{
	rdoRuntime::LPRDOCalc pNewCalc = rdo::Factory<rdoRuntime::RDOCalcNot>::create(m_pCalc).object_cast<rdoRuntime::RDOCalc>();
	RDOFUNLogic* logic = new RDOFUNLogic(this, pNewCalc);
	logic->setSrcInfo(pNewCalc->src_info());
	logic->m_int_or_double.insert(m_int_or_double);
	return logic;
}

void RDOFUNLogic::setSrcInfo( const RDOParserSrcInfo& src_info )
{
	RDOParserSrcInfo::setSrcInfo( src_info );
	if ( m_pCalc ) m_pCalc->setSrcInfo( src_info );
}

void RDOFUNLogic::setSrcPos( const RDOSrcInfo::Position& _pos )
{
	RDOParserSrcInfo::setSrcPos( _pos );
	if ( m_pCalc ) m_pCalc->setSrcPos( _pos );
}

void RDOFUNLogic::setSrcText( const std::string& value )
{
	RDOParserSrcInfo::setSrcText( value );
	if ( m_pCalc ) m_pCalc->setSrcText( value );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNArithm
// ----------------------------------------------------------------------------
RDOFUNArithm::RDOFUNArithm( RDOParser* parser, const RDOValue& value, CREF(rdoRuntime::LPRDOCalc) pCalc ):
	RDOParserObject( parser ),
	m_value( value ),
	m_pCalc(pCalc)
{
	setSrcInfo( m_value.src_info() );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* parent, const RDOValue& value, CREF(rdoRuntime::LPRDOCalc) pCalc ):
	RDOParserObject( parent ),
	m_value( value ),
	m_pCalc(pCalc)
{
	setSrcInfo( m_value.src_info() );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, const RDOValue& value ):
	RDOParserObject( _parser ),
	m_pCalc( NULL )
{
	init( value );
}

RDOFUNArithm::RDOFUNArithm( const RDOFUNArithm* parent, const RDOValue& value ):
	RDOParserObject( parent ),
	m_pCalc( NULL )
{
	init( value );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* parser, const RDOValue& res_name, const RDOValue& par_name ):
	RDOParserObject( parser ),
	m_pCalc( NULL )
{
	init( res_name, par_name );
}

RDOFUNArithm::RDOFUNArithm( const RDOFUNArithm* parent, const RDOValue& res_name, const RDOValue& par_name ):
	RDOParserObject( parent ),
	m_pCalc( NULL )
{
	init( res_name, par_name );
}

void RDOFUNArithm::init(CREF(RDOValue) value)
{
	setSrcInfo( value.src_info() );

	if ( value.constant() )
	{
		m_value = value;
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_value.value()).object_cast<rdoRuntime::RDOCalc>();
		m_pCalc->setSrcInfo( src_info() );
		return;
	}
	//if ( value->getIdentificator() == "Time_now" || value->getIdentificator() == "time_now" || value->getIdentificator() == "���������_�����" || value->getIdentificator() == "���������_�����" )
	//{
	//	m_value = g_real.cast<RDOType>();
	//	m_pCalc = new rdoRuntime::RDOCalcGetTimeNow( parser()->runtime() );
	//	m_pCalc->setSrcInfo( src_info() );
	//	return;
	//}
	//else
	if ( value->getIdentificator() == "Terminate_counter" || value->getIdentificator() == "terminate_counter" )
	{
		m_value = g_int.object_cast<RDOType>();
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetTermNow>::create().object_cast<rdoRuntime::RDOCalc>();
		m_pCalc->setSrcInfo( src_info() );
		return;
	}
	else if ( value->getIdentificator() == "Seconds" || value->getIdentificator() == "seconds" )
	{
		m_value = g_real.object_cast<RDOType>();
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetSeconds>::create().object_cast<rdoRuntime::RDOCalc>();
		m_pCalc->setSrcInfo( src_info() );
		return;
	}

	//! ���� �������� ������������ �������
	if (parser()->getFileToParse() == rdoModelObjects::PAT)
	{
		CPTR(RDOPATPattern) pPattern = parser()->getLastPATPattern();
		if (pPattern && pPattern->m_pCurrRelRes)
		{
			LPRDORTPParam pParam = pPattern->m_pCurrRelRes->getType()->findRTPParam(value->getIdentificator());
			if (pParam)
			{
				RDOValue paramName(pPattern->m_pCurrRelRes->m_bodySrcInfo);
				init(paramName, value);
				return;
			}
		}
	}

	// ���� ��������� �������� ��� ������� �� �����
	const RDOFUNFunctionParam* param = NULL;
	switch ( parser()->getFileToParse() )
	{
		case rdoModelObjects::PAT: param = parser()->getLastPATPattern()->findPATPatternParam( value->getIdentificator() ); break;
		case rdoModelObjects::FUN: param = parser()->getLastFUNFunction()->findFUNFunctionParam( value->getIdentificator() ); break;
	}

	// ���� ��������� �� �����
	const RDOFUNConstant* cons = parser()->findFUNConstant( value->getIdentificator() );

	if ( cons && param )
	{
		parser()->error().error( src_info(), rdo::format("��� ��������� ������� ��������� � ������ ���������: %s", value->getIdentificator().c_str()) );
	}

	if ( cons )
	{
		m_value = cons->getType()->type();
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetConst>::create(cons->getNumber()).object_cast<rdoRuntime::RDOCalc>();
		m_pCalc->setSrcInfo( src_info() );
		return;
	}

	// ���� ������������������ �� �����
	const RDOFUNSequence* seq = parser()->findFUNSequence( value->getIdentificator() );
	if ( seq && param )
	{
		parser()->error().error( src_info(), rdo::format("��� ��������� ������� ��������� � ������ ������������������: %s", value->getIdentificator().c_str()) );
	}

	if ( seq )
	{
		RDOFUNParams tmp( seq );
		RDOFUNArithm* arithm = tmp.createSeqCall( value->getIdentificator() );
		arithm->setSrcInfo( src_info() );
		m_value = arithm->value();
		m_pCalc  = arithm->m_pCalc;
		return;
	}

	if ( param )
	{
		// ��� ��������
		m_value = param->getType()->type();
		switch ( parser()->getFileToParse() )
		{
		case rdoModelObjects::PAT: m_pCalc = rdo::Factory<rdoRuntime::RDOCalcPatParam>::create (parser()->getLastPATPattern()->findPATPatternParamNum(value->getIdentificator())).object_cast<rdoRuntime::RDOCalc>(); break;
		case rdoModelObjects::FUN: m_pCalc = rdo::Factory<rdoRuntime::RDOCalcFuncParam>::create(parser()->getLastFUNFunction()->findFUNFunctionParamNum(value->getIdentificator()), param->src_info()).object_cast<rdoRuntime::RDOCalc>(); break;
		}
		if ( m_pCalc ) m_pCalc->setSrcInfo( src_info() );
		return;
	}

	//! ��������, ��� ��� �������� ������������� ����, ������ ���� � ���� �������� ����� ����������� � ������
	//! ������������ �����, ������� ����� ������ �� ��� ������� - ������
	{ErrorBlockMonicker errorBlockMonicker;
		CREF(RDOParser::PreCastTypeList) typeList = parser()->getPreCastTypeList();
		STL_FOR_ALL_CONST(RDOParser::PreCastTypeList, typeList, it)
		{
			RDOValue try_cast_value = (*it)->value_cast(value);
			if (try_cast_value.defined())
			{
				m_value = value;
				return;
			}
		}
	}

	parser()->error().error( value.src_info(), rdo::format("����������� �������������: %s", value->getIdentificator().c_str()) );
}

void RDOFUNArithm::init(CREF(RDOValue) res_name, CREF(RDOValue) par_name)
{
	setSrcInfo( res_name.src_info(), ".", par_name.src_info() );
	const RDORSSResource* const res = parser()->findRSSResource( res_name->getIdentificator() ); 
	if ( res )
	{
		// ��� ������ � �������� RSS
		unsigned int parNumb = res->getType()->getRTPParamNumber( par_name->getIdentificator() );
		if ( parNumb == RDORTPResType::UNDEFINED_PARAM )
		{
			parser()->error().error( par_name.src_info(), rdo::format("����������� �������� �������: %s", par_name->getIdentificator().c_str()) );
		}
		if ( res->getType()->isPermanent() )
		{
			m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetResParam>::create(res->getID(), parNumb).object_cast<rdoRuntime::RDOCalc>();
		}
		else if ( res->getType()->isTemporary() && parser()->getFileToParse() == rdoModelObjects::FRM )
		{
			m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetTempResParamFRM>::create(res->getID(), parNumb).object_cast<rdoRuntime::RDOCalc>();
		}
		else
		{
			parser()->error().error( res_name.src_info(), rdo::format("������ ������������ ��������� ������: %s", res_name->getIdentificator().c_str()) );
		}
		m_pCalc->setSrcInfo( src_info() );
		m_value = res->getType()->findRTPParam( par_name->getIdentificator() )->getParamType()->type();
		return;
	}
	// ��� �� ������, �� ��������, ������ ������ ��������� �������
	else if ( !parser()->getFUNGroupStack().empty() && parser()->getFUNGroupStack().back()->resType->name() == res_name->getIdentificator() )
	{
		// ��� ������ ������ ��������� �������
		RDOFUNGroup* currGroup = parser()->getFUNGroupStack().back();
		unsigned int parNumb = currGroup->resType->getRTPParamNumber( par_name->getIdentificator() );
		if ( parNumb == RDORTPResType::UNDEFINED_PARAM )
		{
			parser()->error().error( par_name.src_info(), rdo::format("����������� �������� �������: %s", par_name->getIdentificator().c_str()) );
		}
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetGroupResParam>::create(parNumb).object_cast<rdoRuntime::RDOCalc>();
		m_pCalc->setSrcInfo( src_info() );
		m_value = currGroup->resType->findRTPParam( par_name->getIdentificator() )->getParamType()->type();
		return;
	}
	else
	{
		// ��������, ��� ����������� ������
		switch ( parser()->getFileToParse() )
		{
			case rdoModelObjects::PAT:
			{
				if ( parser()->getLastPATPattern() && parser()->getLastPATPattern()->findRelevantResource( res_name->getIdentificator() ) )
				{
					// ��� ����������� ������ ���-�� � �������� (with_min-common-choice, $Time, $Body)
					RDOPATPattern* pat = parser()->getLastPATPattern();
					const RDORelevantResource* const rel = pat->findRelevantResource( res_name->getIdentificator() );
					if ( !pat->m_pCurrRelRes )
					{
						// ������ with_min-common-choice ��� $Time
						if ( rel->m_statusBegin == rdoRuntime::RDOResource::CS_NonExist || rel->m_statusBegin == rdoRuntime::RDOResource::CS_Create )
						{
							parser()->error().error( res_name.src_info(), rdo::format("����������� ������ �� ����� ���� �����������, �.�. �� ��� �� ����������: %s", rel->name().c_str()) );
						}
					}
					else
					{
						// ������ $Body
						// ��������� ������������� ��������������������� ���.������� (rel) � Choice from ������ ���.������� (pat->m_pCurrRelRes)
						if ( pat->m_pCurrRelRes->isChoiceFromState() )
						{
							if ( !rel->m_alreadyHaveConverter && !rel->isDirect() )
							{
								parser()->error().error( res_name.src_info(), rdo::format("����������� ������ �����������: %s. ��� ������ ������������ � �������� ������ ������ �������� �� ��� ������������ Choice from", rel->name().c_str()) );
							}
							if ( rel->m_statusBegin == rdoRuntime::RDOResource::CS_NonExist )
							{
								parser()->error().error( res_name.src_info(), rdo::format("����������� ������ � ������ �������� �� ���������� (NonExist): %s", rel->name().c_str()) );
							}
							if ( rel->m_statusBegin == rdoRuntime::RDOResource::CS_Create )
							{
								parser()->error().error( res_name.src_info(), rdo::format("����� ����� �������� (Create) ����������� ������ '%s' ����� ������������ ������ � �����������, �� �� � ������� ������", rel->name().c_str()) );
							}
						}
						// ��������� ������������� ���������� ���.������� ������ ���������� ������� ���.�������
						if ( rel->getType()->isTemporary() )
						{
							// � ���������� ������
							if ( pat->m_pCurrRelRes->m_currentState == RDORelevantResource::convertBegin )
							{
								if ( rel->m_statusBegin == rdoRuntime::RDOResource::CS_Create && !rel->m_alreadyHaveConverter )
								{
									parser()->error().error( res_name.src_info(), rdo::format("����������� ������ ������ ������������ �� ��� �������� (Create): %s", rel->name().c_str()) );
								}
								if ( rel->m_statusBegin == rdoRuntime::RDOResource::CS_Erase && rel->m_alreadyHaveConverter )
								{
									parser()->error().error( res_name.src_info(), rdo::format("����������� ������ ������ ������������ ����� �������� (Erase): %s", rel->name().c_str()) );
								}
								if ( rel->m_statusBegin == rdoRuntime::RDOResource::CS_NonExist )
								{
									parser()->error().error( res_name.src_info(), rdo::format("����������� ������ �� ���������� � ���� ���������� (NonExist): %s", rel->name().c_str()) );
								}
							}
							// � ���������� �����
							if ( pat->m_pCurrRelRes->m_currentState == RDORelevantResource::convertEnd )
							{
								if ( rel->m_statusEnd == rdoRuntime::RDOResource::CS_Create && !rel->m_alreadyHaveConverter )
								{
									parser()->error().error( res_name.src_info(), rdo::format("����������� ������ ������ ������������ �� ��� �������� (Create): %s", rel->name().c_str()) );
								}
								if ( rel->m_statusEnd == rdoRuntime::RDOResource::CS_Erase && rel->m_alreadyHaveConverter )
								{
									parser()->error().error( res_name.src_info(), rdo::format("����������� ������ ������ ������������ ����� �������� (Erase): %s", rel->name().c_str()) );
								}
								if ( rel->m_statusEnd == rdoRuntime::RDOResource::CS_NonExist )
								{
									parser()->error().error( res_name.src_info(), rdo::format("����������� ������ �� ���������� � ���� ���������� (NonExist): %s", rel->name().c_str()) );
								}
							}
						}
						// ��������� ������������� ��� �� ������������������� (������ ��� Create) ��������� ���. ������� � ��� �� ����������
						LPRDORTPParam param = pat->m_pCurrRelRes->getType()->findRTPParam( par_name->getIdentificator() );
						if ( param && pat->m_pCurrRelRes->name() == res_name->getIdentificator() )
						{
							// � ���������� ������
							if (pat->m_pCurrRelRes->m_currentState == RDORelevantResource::convertBegin && pat->m_pCurrRelRes->m_statusBegin == rdoRuntime::RDOResource::CS_Create)
							{
								if (!pat->m_pCurrRelRes->getParamSetList().find(par_name->getIdentificator()))
								{
									if ( !param->getDefault().defined() )
									{
										parser()->error().error( par_name.src_info(), rdo::format("�������� '%s' ��� �� ���������, ��� ���������� ��������� �������� � ������� ���������� ��� ������� �������� ��-��������� � ���� �������", par_name->getIdentificator().c_str()) );
									}
								}
							}
							// � ���������� �����
							if (pat->m_pCurrRelRes->m_currentState == RDORelevantResource::convertEnd && pat->m_pCurrRelRes->m_statusEnd == rdoRuntime::RDOResource::CS_Create)
							{
								if (!pat->m_pCurrRelRes->getParamSetList().find( par_name->getIdentificator()))
								{
									if ( !param->getDefault().defined() ) {
										parser()->error().error( par_name.src_info(), rdo::format("�������� '%s' ��� �� ���������, ��� ���������� ��������� �������� � ������� ���������� ��� ������� �������� ��-��������� � ���� �������", par_name->getIdentificator().c_str()) );
									}
								}
							}
						}
					}
					unsigned int parNumb = rel->getType()->getRTPParamNumber( par_name->getIdentificator() );
					if ( parNumb == RDORTPResType::UNDEFINED_PARAM )
					{
						parser()->error().error( par_name.src_info(), rdo::format("����������� �������� �������: %s", par_name->getIdentificator().c_str()) );
					}
					m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetRelevantResParam>::create(pat->findRelevantResourceNum(res_name->getIdentificator()), parNumb).object_cast<rdoRuntime::RDOCalc>();
					m_pCalc->setSrcInfo( src_info() );
					m_value = rel->getType()->findRTPParam( par_name->getIdentificator() )->getParamType()->type();
					return;
				}
				break;
			}
			case rdoModelObjects::DPT:
			{
				if ( parser()->isCurrentDPTSearch() && parser()->getLastDPTSearch()->getLastActivity() )
				{
					const RDOPATPattern* rule = parser()->getLastDPTSearch()->getLastActivity()->pattern();
					if ( rule && rule->findRelevantResource( res_name->getIdentificator() ) ) {
						// ��� ������, ������� ������������ � DPT (condition, term_condition, evaluate_by, value before, value after)
						const RDORelevantResource* const rel = rule->findRelevantResource( res_name->getIdentificator() );
						int relResNumb = rule->findRelevantResourceNum( res_name->getIdentificator() );
						unsigned int parNumb = rel->getType()->getRTPParamNumber( par_name->getIdentificator() );
						if ( parNumb == RDORTPResType::UNDEFINED_PARAM )
						{
							parser()->error().error( par_name.src_info(), rdo::format("����������� �������� �������: %s", par_name->getIdentificator().c_str()) );
						}
						m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetRelevantResParam>::create(relResNumb, parNumb).object_cast<rdoRuntime::RDOCalc>();
						m_pCalc->setSrcInfo( src_info() );
						m_value = rel->getType()->findRTPParam( par_name->getIdentificator() )->getParamType()->type();
						return;
					}
				}
				if ( parser()->isCurrentDPTPrior() && parser()->getLastDPTPrior()->getLastActivity() )
				{
					const RDOPATPattern* activity = parser()->getLastDPTPrior()->getLastActivity()->pattern();
					if ( activity && activity->findRelevantResource( res_name->getIdentificator() ) ) {
						// ��� ������, ������� ������������ � ��������� ���������� ���������� DPTPrior
						const RDORelevantResource* const rel = activity->findRelevantResource( res_name->getIdentificator() );
						int relResNumb = activity->findRelevantResourceNum( res_name->getIdentificator() );
						unsigned int parNumb = rel->getType()->getRTPParamNumber( par_name->getIdentificator() );
						if ( parNumb == RDORTPResType::UNDEFINED_PARAM )
						{
							parser()->error().error( par_name.src_info(), rdo::format("����������� �������� �������: %s", par_name->getIdentificator().c_str()) );
						}
						m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetRelevantResParam>::create(relResNumb, parNumb).object_cast<rdoRuntime::RDOCalc>();
						m_pCalc->setSrcInfo( src_info() );
						m_value = rel->getType()->findRTPParam( par_name->getIdentificator() )->getParamType()->type();
						return;
					}
				}
				break;
			}
			case rdoModelObjects::FRM:
			{
				if ( parser()->getLastFRMFrame() && parser()->getLastFRMFrame()->frame()->getLastShow() && parser()->getLastFRMFrame()->frame()->getLastShow()->isShowIf() )
				{
					m_pCalc = rdo::Factory<rdoRuntime::RDOCalcGetUnknowResParam>::create(res_name->getIdentificator(), par_name->getIdentificator()).object_cast<rdoRuntime::RDOCalc>();
					m_pCalc->setSrcInfo( src_info() );
					return;
				}
				break;
			}
		}
	}
	parser()->error().error( res_name.src_info(), rdo::format("����������� ������: %s", res_name->getIdentificator().c_str()) );
}

RDOFUNArithm::CastResult RDOFUNArithm::beforeCastValue(REF(RDOFUNArithm) second)
{
	if ( typeID() == rdoRuntime::RDOType::t_enum && second.typeID() == rdoRuntime::RDOType::t_identificator )
	{
		second.m_value = RDOValue(
			type()->value_cast(
				second.value(),
				second.src_info(),
				src_info()
			).value(),
			enumType().object_cast<RDOType>(),
			second.m_value.src_info()
		);
		second.m_pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(second.m_value.value()).object_cast<rdoRuntime::RDOCalc>();
		second.m_pCalc->setSrcInfo( second.src_info() );
		return CR_DONE;
	}
	else if ( typeID() == rdoRuntime::RDOType::t_identificator && second.typeID() == rdoRuntime::RDOType::t_enum )
	{
		m_value = RDOValue(
			second.type()->value_cast(
				value(),
				src_info(),
				second.src_info()
			).value(),
			second.enumType().object_cast<RDOType>(),
			m_value.src_info()
		);
		m_pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_value.value()).object_cast<rdoRuntime::RDOCalc>();
		m_pCalc->setSrcInfo( src_info() );
		return CR_DONE;
	}
	return CR_CONTINUE;
}

LPRDOType RDOFUNArithm::getPreType(CREF(RDOFUNArithm) second)
{
	if ( typeID() == rdoRuntime::RDOType::t_unknow )
	{
		parser()->error().error( src_info(), rdo::format("���������� ������ �������: �������������� �������� ��������������� ���������: %s", src_text().c_str()) );
	}
	if ( second.typeID() == rdoRuntime::RDOType::t_unknow )
	{
		parser()->error().error( second.src_info(), rdo::format("���������� ������ �������: �������������� �������� ��������������� ���������: %s", second.src_text().c_str()) );
	}
	if ( typeID() == rdoRuntime::RDOType::t_identificator )
	{
		parser()->error().error( src_info(), rdo::format("����������� �������������: %s", value()->getIdentificator().c_str()) );
	}
	if ( second.typeID() == rdoRuntime::RDOType::t_identificator )
	{
		parser()->error().error( second.src_info(), rdo::format("����������� �������������: %s", second.value()->getIdentificator().c_str()) );
	}

	//! TODO: ������� ��� ���������� src_info(), ��������� � �������� ������������
	return type()->type_cast(second.type(), second.src_info(), src_info(), src_info());
}

RDOFUNArithm* RDOFUNArithm::operator+ ( RDOFUNArithm& second )
{
	GENERATE_ARITHM( Plus, +, "�� �� ���� � ������� %s � %s" );
}

RDOFUNArithm* RDOFUNArithm::operator- ( RDOFUNArithm& second )
{
	GENERATE_ARITHM( Minus, -, "������ �� %s ������� %s" );
}

RDOFUNArithm* RDOFUNArithm::operator* ( RDOFUNArithm& second )
{
	GENERATE_ARITHM( Mult, *, "������ %s �������� �� %s" );
}

RDOFUNArithm* RDOFUNArithm::operator/ ( RDOFUNArithm& second )
{
	GENERATE_ARITHM_CALC( Div, /, "������ %s ��������� �� %s" );
	// TODO: ������� ������������� � ����� ��� �������. � ��� ������ � ���������� � �.�. ?
	// �����: � ���������� ���� ������ ���� �����, ������ ��������� ���
	if ( pNewType->type().typeID() == rdoRuntime::RDOType::t_int )
	{
		rdoRuntime::LPRDOCalc pNewCalc_div = pNewCalc;
		pNewCalc = rdo::Factory<rdoRuntime::RDOCalcDoubleToIntByResult>::create(pNewCalc_div).object_cast<rdoRuntime::RDOCalc>();
		pNewCalc->setSrcInfo(pNewCalc_div->src_info());
	}
	RDOFUNArithm* arithm = new RDOFUNArithm( this, RDOValue(pNewType, pNewCalc->src_info()), pNewCalc );
	if ( pNewType->type().typeID() == rdoRuntime::RDOType::t_int )
	{
		rdoRuntime::LPRDOCalcDoubleToIntByResult pResult = pNewCalc.object_cast<rdoRuntime::RDOCalcDoubleToIntByResult>();
		arithm->m_int_or_double.push_back(pResult);
	}
	arithm->m_int_or_double.insert( m_int_or_double, second.m_int_or_double );
	return arithm;
}

RDOFUNLogic* RDOFUNArithm::operator< ( RDOFUNArithm& second )
{
	GENERATE_LOGIC_FROM_ARITHM( IsLess, <, "������ ���������� %s � %s" );
}

RDOFUNLogic* RDOFUNArithm::operator> ( RDOFUNArithm& second )
{
	GENERATE_LOGIC_FROM_ARITHM( IsGreater, >, "������ ���������� %s � %s" );
}

RDOFUNLogic* RDOFUNArithm::operator<= ( RDOFUNArithm& second )
{
	GENERATE_LOGIC_FROM_ARITHM( IsLEQ, <=, "������ ���������� %s � %s" );
}

RDOFUNLogic* RDOFUNArithm::operator>= ( RDOFUNArithm& second )
{
	GENERATE_LOGIC_FROM_ARITHM( IsGEQ, >, "������ ���������� %s � %s" );
}

RDOFUNLogic* RDOFUNArithm::operator== ( RDOFUNArithm& second )
{
	GENERATE_LOGIC_FROM_ARITHM( IsEqual, ==, "������ ���������� %s � %s" );
}

RDOFUNLogic* RDOFUNArithm::operator!= ( RDOFUNArithm& second )
{
	GENERATE_LOGIC_FROM_ARITHM( IsNotEqual, !=, "������ ���������� %s � %s" );
}

void RDOFUNArithm::checkParamType(CREF(LPRDOTypeParam) pType)
{
	pType->type()->type_cast(type(), src_info(), pType->src_info(), src_info());
	rdoRuntime::LPRDOCalcConst pConstCalc = calc().object_dynamic_cast<rdoRuntime::RDOCalcConst>();
	if (pConstCalc)
	{
		rdoRuntime::RDOValue value = pConstCalc->calcValue(parser()->runtime());
		pType->value_cast(RDOValue(value, type(), src_info()));
	}
}

rdoRuntime::LPRDOCalc RDOFUNArithm::createCalc(CREF(LPRDOTypeParam) forType)
{
	if ( typeID() != rdoRuntime::RDOType::t_identificator )
	{
		if ( forType == NULL )
		{
			return m_pCalc;
		}
		if ( forType->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			if ( forType->type()->typeID() == rdoRuntime::RDOType::t_enum )
			{
				m_int_or_double.roundCalc();
			}
			return m_pCalc;
		}
		else
		{
			m_int_or_double.roundCalc();
			rdoRuntime::LPRDOCalc pNewCalc = rdo::Factory<rdoRuntime::RDOCalcDoubleToInt>::create(m_pCalc).object_cast<rdoRuntime::RDOCalc>();
			pNewCalc->setSrcInfo(src_info());
			return pNewCalc;
		}
	}
	else if ( typeID() == rdoRuntime::RDOType::t_identificator && !forType )
	{
		parser()->error().error( src_info(), rdo::format( "����������� �������������: %s", value()->getAsString().c_str()) );
	}

	if ( !forType )
	{
		parser()->error().error( src_info(), "����������� ��� ���������" );
	}

	rdoRuntime::LPRDOCalc pNewCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(forType->value_cast(RDOValue(src_info())).value()).object_cast<rdoRuntime::RDOCalc>(); // TODO ����� _m_str
	pNewCalc->setSrcInfo( src_info() );
	return pNewCalc;
}

void RDOFUNArithm::setSrcInfo( const RDOParserSrcInfo& src_info )
{
	RDOParserSrcInfo::setSrcInfo( src_info );
	if ( m_pCalc ) m_pCalc->setSrcInfo( src_info );
}

void RDOFUNArithm::setSrcPos( const RDOSrcInfo::Position& _pos )
{
	RDOParserSrcInfo::setSrcPos( _pos );
	if ( m_pCalc ) m_pCalc->setSrcPos( _pos );
}

void RDOFUNArithm::setSrcText( const std::string& value )
{
	RDOParserSrcInfo::setSrcText( value );
	if ( m_pCalc ) m_pCalc->setSrcText( src_info().src_text() );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNConstant
// ----------------------------------------------------------------------------
RDOFUNConstant::RDOFUNConstant(PTR(RDOParser) _parser, CREF(tstring) name, CREF(LPRDOTypeParam) type)
	: RDOParserObject (_parser                  )
	, RDOParserSrcInfo(type->src_info()        )
	, m_name          (name                    )
	, m_type          (type                    )
	, m_number        (_parser->getFUNCONST_id())
{
	parser()->insertFUNConstant(this);
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNParams
// ----------------------------------------------------------------------------
// ���������, � �������� ������� ����������
// ----------------------------------------------------------------------------
RDOFUNArithm* RDOFUNParams::createCall( const std::string& funName ) const
{
	const RDOFUNFunction* const func = parser()->findFUNFunction( funName );
	if ( !func ) {
		return createSeqCall( funName );
	}

	int nParams = func->getParams().size();
	if ( nParams != params.size() ) {
		parser()->error().error( src_info(), rdo::format("�������� ���������� ���������� �������: %s", funName.c_str()) );
	}

	rdoRuntime::LPRDOCalcFunctionCall pFuncCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(func->getFunctionCalc());
	const_cast<RDOFUNFunction*>(func)->insertPostLinked(pFuncCall);
	pFuncCall->setSrcInfo( src_info() );
	for ( int i = 0; i < nParams; i++ )
	{
		LPRDOTypeParam funcParam = func->getParams()[i]->getType();
		PTR(RDOFUNArithm) arithm = params[i];
		arithm->checkParamType(funcParam);
		pFuncCall->addParameter(funcParam->type()->calc_cast(arithm->createCalc(funcParam), arithm->type()));
	}

	RDOFUNArithm* res = new RDOFUNArithm(this, RDOValue(func->getType()->type(), src_pos()), pFuncCall.object_cast<rdoRuntime::RDOCalc>());
	res->setSrcInfo( src_info() );

	return res;
}

RDOFUNArithm* RDOFUNParams::createSeqCall( const std::string& seqName ) const
{
	const RDOFUNSequence* const seq = parser()->findFUNSequence( seqName );
	if ( !seq ) {
		parser()->error().error( funseq_name, rdo::format("�������������� ������� ��� ������������������: %s", seqName.c_str()) );
	}
	RDOFUNArithm* arithm = seq->createCallCalc( this, src_info() );
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- ������������������
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequence
// ----------------------------------------------------------------------------
RDOFUNSequence::RDOFUNSequence( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _header->src_info() ),
	header( _header ),
	init_calc( NULL ),
	next_calc( NULL ),
	base( _base )
{
	header->reparent( this );
	parser()->insertFUNSequence( this );
}

void RDOFUNSequence::initResult()
{
	initCalcSrcInfo();
	next_calc->m_res_real = true;
	switch (header->getType()->type()->typeID())
	{
		case rdoRuntime::RDOType::t_int:
		{
			LPRDOTypeIntRange pRange = header->getType()->type().object_dynamic_cast<RDOTypeIntRange>();
			if (pRange)
			{
				next_calc->m_diap     = true;
				next_calc->m_diap_min = pRange->range()->getMin().value().getDouble();
				next_calc->m_diap_max = pRange->range()->getMax().value().getDouble();
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			next_calc->m_res_real = true;
			LPRDOTypeRealRange pRange = header->getType()->type().object_dynamic_cast<RDOTypeRealRange>();
			if (pRange)
			{
				next_calc->m_diap     = true;
				next_calc->m_diap_min = pRange->range()->getMin().value().getDouble();
				next_calc->m_diap_max = pRange->range()->getMax().value().getDouble();
			}
			break;
		}
	}
}

void RDOFUNSequence::initCalcSrcInfo()
{
	if ( init_calc ) {
		init_calc->setSrcInfo( header->src_info() );
		init_calc->setSrcText( "��������� ���� ������������������ " + init_calc->src_text() );
	}
	next_calc->setSrcInfo( header->src_info() );
}

// ----------------------------------------------------------------------------
// ---------- ������� ��������� �����
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceUniform
// ----------------------------------------------------------------------------
RDOFUNSequenceUniform::RDOFUNSequenceUniform( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if (header->getType()->type()->typeID() == rdoRuntime::RDOType::t_enum)
	{
		parser()->error().error( src_info(), "������������������ ���� uniform �� ����� ���������� �������� ������������� ����" );
	}
}

void RDOFUNSequenceUniform::createCalcs()
{
	rdoRuntime::RandGeneratorUniform* gen = new rdoRuntime::RandGeneratorUniform();
	init_calc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(base, gen);
	parser()->runtime()->addInitCalc( init_calc.object_cast<rdoRuntime::RDOCalc>() );
	next_calc = rdo::Factory<rdoRuntime::RDOCalcSeqNextUniform>::create(gen).object_cast<rdoRuntime::RDOCalcSeqNext>();
	initResult();
}

RDOFUNArithm* RDOFUNSequenceUniform::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& _src_info ) const
{
	if ( param->params.size() != 2 ) {
		parser()->error().error( _src_info, rdo::format("��� ������������ ������ ������������� '%s' ����� ������� ��� ���������: ����������� � ������������ ������� ���������", name().c_str()) );
	}

	rdoRuntime::LPRDOCalcFunctionCall funcCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(next_calc.object_cast<rdoRuntime::RDOFunCalc>());
	LPRDOTypeParam realType = rdo::Factory<RDOTypeParam>::create(g_real.object_cast<RDOType>(), RDOValue(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc arg1 = param->params[0]->createCalc( realType );
	rdoRuntime::LPRDOCalc arg2 = param->params[1]->createCalc( realType );

	funcCall->addParameter( arg1 );
	funcCall->addParameter( arg2 );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceUniform*>(this), RDOValue(header->getType()->type(), param->src_pos()), funcCall.object_cast<rdoRuntime::RDOCalc>() );
	arithm->setSrcInfo( _src_info );
	if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
		parser()->error().error( src_info(), "���������� ������ �������" );
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceExponential
// ----------------------------------------------------------------------------
RDOFUNSequenceExponential::RDOFUNSequenceExponential( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->getType()->type()->typeID() != rdoRuntime::RDOType::t_int && header->getType()->type()->typeID() != rdoRuntime::RDOType::t_real )
	{
		parser()->error().error( _header->getType()->src_info(), rdo::format("������������������ '%s' ����� ���������� �������� ������ ������ ��� ������������� ����", src_text().c_str()) );
	}
}

void RDOFUNSequenceExponential::createCalcs()
{
	rdoRuntime::RandGeneratorExponential* gen = new rdoRuntime::RandGeneratorExponential();
	init_calc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(base, gen);
	parser()->runtime()->addInitCalc( init_calc.object_cast<rdoRuntime::RDOCalc>() );
	next_calc = rdo::Factory<rdoRuntime::RDOCalcSeqNextExponential>::create(gen).object_cast<rdoRuntime::RDOCalcSeqNext>();
	initResult();
}

RDOFUNArithm* RDOFUNSequenceExponential::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& _src_info ) const
{
	if ( param->params.size() != 1 ) {
		parser()->error().error( _src_info, rdo::format("��� ����������������� ������ ������������� '%s' ����� ������� ���� ��������: �������������� ��������", name().c_str()) );
	}

	rdoRuntime::LPRDOCalcFunctionCall funcCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(next_calc.object_cast<rdoRuntime::RDOFunCalc>());
	RDOFUNArithm *arithm1 = param->params[0];
	LPRDOTypeParam realType = rdo::Factory<RDOTypeParam>::create(g_real.object_cast<RDOType>(), RDOValue(), RDOParserSrcInfo());
	rdoRuntime::LPRDOCalc arg1 = arithm1->createCalc( realType );

	funcCall->addParameter( arg1 );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceExponential*>(this), RDOValue(header->getType()->type(), param->src_pos()), funcCall.object_cast<rdoRuntime::RDOCalc>() );
	arithm->setSrcInfo( _src_info );
	if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
		parser()->error().error( src_info(), "���������� ������ �������" );
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceNormal
// ----------------------------------------------------------------------------
RDOFUNSequenceNormal::RDOFUNSequenceNormal( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->getType()->type()->typeID() == rdoRuntime::RDOType::t_enum )
	{
		parser()->error().error( src_info(), "������������������ ���� normal �� ����� ���������� �������� ������������� ����" );
	}
}

void RDOFUNSequenceNormal::createCalcs()
{
	rdoRuntime::RandGeneratorNormal* gen = new rdoRuntime::RandGeneratorNormal();
	init_calc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(base, gen);
	parser()->runtime()->addInitCalc( init_calc.object_cast<rdoRuntime::RDOCalc>() );
	next_calc = rdo::Factory<rdoRuntime::RDOCalcSeqNextNormal>::create(gen).object_cast<rdoRuntime::RDOCalcSeqNext>();
	initResult();
}

RDOFUNArithm* RDOFUNSequenceNormal::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& _src_info ) const
{
	if ( param->params.size() != 2 ) {
		parser()->error().error( _src_info, rdo::format("��� ����������� ������ ������������� '%s' ����� ������� ��� ���������: �������������� �������� � ������� �������������� ����������", name().c_str()) );
	}

	rdoRuntime::LPRDOCalcFunctionCall funcCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(next_calc.object_cast<rdoRuntime::RDOFunCalc>());
	LPRDOTypeParam realType = rdo::Factory<RDOTypeParam>::create(g_real.object_cast<RDOType>(), RDOValue(), RDOParserSrcInfo());
	RDOFUNArithm *arithm1 = param->params[0];
	RDOFUNArithm *arithm2 = param->params[1];
	rdoRuntime::LPRDOCalc arg1 = arithm1->createCalc( realType );
	rdoRuntime::LPRDOCalc arg2 = arithm2->createCalc( realType );

	funcCall->addParameter( arg1 );
	funcCall->addParameter( arg2 );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceNormal*>(this), RDOValue(header->getType()->type(), param->src_pos()), funcCall.object_cast<rdoRuntime::RDOCalc>() );
	arithm->setSrcInfo( _src_info );
	if ( arithm->typeID() == rdoRuntime::RDOType::t_enum ) {
		parser()->error().error( src_info(), "���������� ������ �������" );
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- �����������
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHist
// ----------------------------------------------------------------------------
RDOFUNArithm* RDOFUNSequenceByHist::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& _src_info ) const
{
	if ( param->params.size() != 0 ) {
		parser()->error().error( _src_info, rdo::format("����������� '%s' ������ ���������� ��� ����������", name().c_str()) );
	}

	rdoRuntime::LPRDOCalcFunctionCall funcCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(next_calc.object_cast<rdoRuntime::RDOFunCalc>());

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceByHist*>(this), RDOValue(header->getType()->type(), param->src_pos()), funcCall.object_cast<rdoRuntime::RDOCalc>() );
	arithm->setSrcInfo( _src_info );
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistReal
// ----------------------------------------------------------------------------
void RDOFUNSequenceByHistReal::addReal( const RDOValue& from, const RDOValue& to, const RDOValue& freq )
{
	if ( !m_to.empty() && from.value() != m_to.back() )
	{
		parser()->error().error( from, "������ ��������� ������ ��������� � ���������� �����������" );
	}
	if ( from.value() > to.value() )
	{
		parser()->error().error( to, "������ ��������� ������ ���� ������ ��� �����" );
	}
	if ( freq.value() == 0 )
	{
		parser()->error().warning( freq, "������������� ����������� ������ ���� ������ ����" );
	}
	else if ( freq.value() < 0 )
	{
		parser()->error().error( freq, "������������� ����������� ������ ���� ������ ����" );
	}
	header->getType()->value_cast( from );
	header->getType()->value_cast( to );
	m_from.push_back( from.value() );
	m_to.push_back( to.value() );
	m_freq.push_back( freq.value() );
}

void RDOFUNSequenceByHistReal::createCalcs()
{
	rdoRuntime::RandGeneratorByHistReal* gen = new rdoRuntime::RandGeneratorByHistReal();
	int size = m_from.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValues( m_from[i].getDouble(), m_to[i].getDouble(), m_freq[i].getDouble() );
	}

	init_calc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(base, gen);
	parser()->runtime()->addInitCalc( init_calc.object_cast<rdoRuntime::RDOCalc>() );
	next_calc = rdo::Factory<rdoRuntime::RDOCalcSeqNextByHist>::create(gen).object_cast<rdoRuntime::RDOCalcSeqNext>();
	initResult();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistEnum
// ----------------------------------------------------------------------------
void RDOFUNSequenceByHistEnum::addEnum( const RDOValue& value, const RDOValue& freq )
{
	if ( freq.value() == 0 )
	{
		parser()->error().warning( freq, "������������� ����������� ������ ���� ������ ����" );
	}
	else if ( freq.value() < 0 )
	{
		parser()->error().error( freq, "������������� ����������� ������ ���� ������ ����" );
	}
	header->getType()->value_cast( value );
	rdoRuntime::RDOValue enum_id = header->getType()->value_cast( value ).value();
	if ( std::find( m_values.begin(), m_values.end(), enum_id ) != m_values.end() )
	{
		parser()->error().error( value, rdo::format("������������ �������� ���������� ������: %s", value->getIdentificator().c_str()) );
	}
	m_values.push_back( enum_id );
	m_freq.push_back( freq.value() );
}

void RDOFUNSequenceByHistEnum::createCalcs()
{
	rdoRuntime::RandGeneratorByHistEnum* gen = new rdoRuntime::RandGeneratorByHistEnum();
	unsigned int size = m_values.size();
	for ( unsigned int i = 0; i < size; i++ )
	{
		gen->addValues( m_values[i], m_freq[i].getDouble() );
	}
	init_calc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(base, gen);
	parser()->runtime()->addInitCalc( init_calc.object_cast<rdoRuntime::RDOCalc>() );
	next_calc = rdo::Factory<rdoRuntime::RDOCalcSeqNextByHist>::create(gen).object_cast<rdoRuntime::RDOCalcSeqNext>();
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- �������� ��������
// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
RDOFUNArithm* RDOFUNSequenceEnumerative::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& _src_info ) const
{
	if ( param->params.size() != 0 )
	{
		parser()->error().error( _src_info, rdo::format("������������ '%s' ������ ���������� ��� ����������", name().c_str()) );
	}

	rdoRuntime::LPRDOCalcFunctionCall funcCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(next_calc.object_cast<rdoRuntime::RDOFunCalc>());

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceEnumerative*>(this), RDOValue(header->getType()->type(), param->src_pos()), funcCall.object_cast<rdoRuntime::RDOCalc>() );
	arithm->setSrcInfo( _src_info );
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerative::createCalcs()
{
	rdoRuntime::RandGeneratorEnumerative* gen = new rdoRuntime::RandGeneratorEnumerative();
	for ( unsigned int i = 0; i < m_values.size(); i++ )
	{
		gen->addValue( m_values[i].value() );
	}
	next_calc = rdo::Factory<rdoRuntime::RDOCalcSeqNextByHist>::create(gen).object_cast<rdoRuntime::RDOCalcSeqNext>();
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- �������
// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElement
// ----------------------------------------------------------------------------
rdoRuntime::LPRDOCalcIsEqual RDOFUNFunctionListElement::createIsEqualCalc(CREF(LPRDOTypeParam) retType, CREF(rdoRuntime::LPRDOCalcFuncParam) funcParam, const RDOParserSrcInfo& _src_pos ) const
{
	rdoRuntime::LPRDOCalcConst constCalc = createResultCalc( retType, _src_pos );
	return rdo::Factory<rdoRuntime::RDOCalcIsEqual>::create(funcParam.object_cast<rdoRuntime::RDOCalc>(), constCalc.object_cast<rdoRuntime::RDOCalc>());
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementIdentif
// ----------------------------------------------------------------------------
rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementIdentif::createResultCalc(CREF(LPRDOTypeParam) retType, const RDOParserSrcInfo& _src_pos ) const
{
	rdoRuntime::LPRDOCalcConst const_calc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(retType->value_cast(RDOValue(RDOParserSrcInfo(_src_pos.getPosAsYY(), src_text()))).value());
	const_calc->setSrcInfo( _src_pos );
	return const_calc;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementReal
// ----------------------------------------------------------------------------
rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementReal::createResultCalc(CREF(LPRDOTypeParam) retType, const RDOParserSrcInfo& _src_pos ) const
{
	rdoRuntime::LPRDOCalcConst const_calc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(retType->value_cast(RDOValue(value, g_real.object_cast<RDOType>(), _src_pos)).value());
	const_calc->setSrcInfo( _src_pos );
	return const_calc;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementInt
// ----------------------------------------------------------------------------
rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementInt::createResultCalc(CREF(LPRDOTypeParam) retType, const RDOParserSrcInfo& _src_pos ) const
{
	rdoRuntime::LPRDOCalcConst const_calc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(retType->value_cast(RDOValue(value, g_int.object_cast<RDOType>(), _src_pos)).value());
	const_calc->setSrcInfo( _src_pos );
	return const_calc;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementEq
// ----------------------------------------------------------------------------
rdoRuntime::LPRDOCalcConst RDOFUNFunctionListElementEq::createResultCalc(CREF(LPRDOTypeParam) retType, const RDOParserSrcInfo& _src_pos ) const
{
	parser()->error().error( _src_pos, "���������� ������ �������: RDOFUNFunctionListElementEq::createResultCalc" );
	return NULL;	// unreachable code
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNCalculateIf
// ----------------------------------------------------------------------------
RDOFUNCalculateIf::RDOFUNCalculateIf( const RDOParserObject* _parent, RDOFUNLogic* _condition, RDOFUNArithm* _action ):
	RDOParserObject( _parent ),
	condition( _condition ),
	action( _action )
{
	action->checkParamType(parser()->getLastFUNFunction()->getType());
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunction
// ----------------------------------------------------------------------------
RDOFUNFunction::RDOFUNFunction( RDOParser* _parser, const RDOParserSrcInfo& _src_info, CREF(LPRDOTypeParam) _retType ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	retType( _retType ),
	functionCalc( NULL )
{
	parser()->insertFUNFunction( this );
}

RDOFUNFunction::RDOFUNFunction( RDOParser* _parser, const std::string& _name, CREF(LPRDOTypeParam) _retType ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _name ),
	retType( _retType ),
	functionCalc( NULL )
{
	parser()->insertFUNFunction( this );
}

void RDOFUNFunction::setFunctionCalc(CREF(rdoRuntime::LPRDOFunCalc) calc)
{
	functionCalc = calc;
	if ( functionCalc->src_empty() )
	{
		functionCalc->setSrcInfo( src_info() );
	}
	STL_FOR_ALL(PostLinkedList, post_linked, it)
	{
		(*it)->setFunctionCalc( getFunctionCalc() );
	}
}

const RDOFUNFunctionParam* const RDOFUNFunction::findFUNFunctionParam( const std::string& paramName ) const 
{
	std::vector< const RDOFUNFunctionParam* >::const_iterator it = std::find_if( params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName) );
	return it != params.end() ? *it : NULL;
}

int RDOFUNFunction::findFUNFunctionParamNum( const std::string& paramName ) const
{
	std::vector< const RDOFUNFunctionParam* >::const_iterator it = std::find_if( params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName) );
	return it != params.end() ? it - params.begin() : -1;
}

void RDOFUNFunction::add( const RDOFUNFunctionParam* const _param )
{ 
	const RDOFUNFunctionParam* const param = findFUNFunctionParam( _param->name() );
	if ( param ) {
		parser()->error().push_only( _param->src_info(), rdo::format("�������� ��� ����������: %s", _param->name().c_str()) );
		parser()->error().push_only( param->src_info(), "��. ������ �����������" );
		parser()->error().push_done();
	}
	params.push_back( _param ); 
}

void RDOFUNFunction::add( const RDOFUNFunctionListElement* const _param )
{
	elements.push_back( _param ); 
}

void RDOFUNFunction::add( const RDOFUNCalculateIf* const _calculateIf )
{
	calculateIf.push_back( _calculateIf );
}

void RDOFUNFunction::createListCalc()
{
	if (!getType()->default().defined())
	{
		parser()->error().error(getType()->src_info(), rdo::format("������� '%s' ������ ����� �������� ��-���������", name().c_str()) );
	}
	if ( params.empty() ) {
		parser()->error().error( src_info(), rdo::format("������� '%s' ������ ����� ���������, �.�. � ��� list", name().c_str()) );
	}
	if ( elements.empty() ) {
		parser()->error().warning( src_info(), rdo::format("������� '%s' �� �������� ������", name().c_str()) );
	}

	rdoRuntime::LPRDOCalcConst default_value_calc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(getType()->value_cast(getType()->default()).value());
	default_value_calc->setSrcInfo(getType()->default().src_info());
	rdoRuntime::LPRDOFunListCalc fun_calc = rdo::Factory<rdoRuntime::RDOFunListCalc>::create(default_value_calc);
	fun_calc->setSrcInfo( src_info() );
	const RDOFUNFunctionListElement* arg_last = NULL;
	std::vector< const RDOFUNFunctionListElement* >::const_iterator elem_it = elements.begin();
	while ( elem_it != elements.end() ) {
		rdoRuntime::LPRDOCalc case_calc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(true).object_cast<rdoRuntime::RDOCalc>();
		case_calc->setSrcInfo( (*elem_it)->src_info() );
		int currParam_number = 0;
		std::vector< const RDOFUNFunctionParam* >::const_iterator param_it = params.begin();
		while ( param_it != params.end() ) {
			const RDOFUNFunctionParam* const param = *param_it;
			if ( elem_it == elements.end() ) {
				elem_it--;
				parser()->error().error( (*elem_it)->src_info(), rdo::format("��������� �������� ��� ��������� '%s'", param->name().c_str()) );
			}
			const RDOFUNFunctionListElement* elem = *elem_it;
			if ( elem->isEquivalence() ) {
				if ( (param_it + 1) == params.end() ) {
					parser()->error().error( elem->src_info(), rdo::format("����� ������ ��������� ��������� �������� ��� ���������: %s",param->name().c_str()) );
				} else {
					std::string _m_str = (*param_it)->src_text();
					param_it++;
					while ( param_it != params.end() ) {
						_m_str += ", ";
						_m_str += (*param_it)->src_text();
						param_it++;
					}
					parser()->error().error( elem->src_info(), rdo::format("����� ������ ��������� ��������� �������� ��� ����������: %s", _m_str.c_str()) );
				}
			}
			rdoRuntime::LPRDOCalcFuncParam funcParam    = rdo::Factory<rdoRuntime::RDOCalcFuncParam>::create(currParam_number, param->src_info());
			rdoRuntime::LPRDOCalcIsEqual   compare_calc = elem->createIsEqualCalc( param->getType(), funcParam, elem->src_info() );
			rdoRuntime::LPRDOCalc          and_calc     = rdo::Factory<rdoRuntime::RDOCalcAnd>::create(case_calc, compare_calc.object_cast<rdoRuntime::RDOCalc>()).object_cast<rdoRuntime::RDOCalc>();
			case_calc = and_calc;
			elem_it++;
			param_it++;
			currParam_number++;
		}
		if ( elem_it == elements.end() ) {
			elem_it--;
			param_it--;
			parser()->error().error( (*elem_it)->src_info(), rdo::format("����� �������� ��� ��������� '%s' ��������� ���� ���������", (*param_it)->name().c_str()) );
		}
		if ( !(*elem_it)->isEquivalence() ) {
			parser()->error().error( (*elem_it)->src_info(), rdo::format("������� ����� ��������, ������ '%s' ��������� ���� ���������", (*elem_it)->src_text().c_str()) );
		}
		elem_it++;
		if ( elem_it == elements.end() ) {
			elem_it--;
			parser()->error().error( (*elem_it)->src_info(), rdo::format("����� ����� ��������� ��������� �������� ������� '%s'", name().c_str()) );
		}
		rdoRuntime::LPRDOCalcConst result_calc = (*elem_it)->createResultCalc(getType(), (*elem_it)->src_info());
		fun_calc->addCase( case_calc, result_calc );
		elem_it++;
	}
	setFunctionCalc( fun_calc.object_cast<rdoRuntime::RDOFunCalc>() );
}

void RDOFUNFunction::createTableCalc( const YYLTYPE& _elements_pos )
{
	std::vector< const RDOFUNFunctionListElement* >::const_iterator it = elements.begin();
	while ( it != elements.end() ) {
		if ( (*it)->isEquivalence() ) {
			parser()->error().error( (*it)->src_info(), "������ '=' ���������� � ��������� �������" );
		}
		it++;
	}
	int param_cnt = params.size();
	int range = 1;
	rdoRuntime::LPRDOCalc calc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(0).object_cast<rdoRuntime::RDOCalc>();
	calc->setSrcInfo( src_info() );
	calc->setSrcText( "0" );
	for ( int currParam = 0; currParam < param_cnt; currParam++ ) {
		const RDOFUNFunctionParam* const param  = params.at(currParam);
		rdoRuntime::LPRDOCalcFuncParam funcParam = rdo::Factory<rdoRuntime::RDOCalcFuncParam>::create(currParam, param->src_info());
		rdoRuntime::LPRDOCalc val2 = funcParam.object_cast<rdoRuntime::RDOCalc>();
		if ( param->getType()->type()->typeID() != rdoRuntime::RDOType::t_enum ) {
			rdoRuntime::LPRDOCalcConst const1 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
			const1->setSrcInfo( param->src_info() );
			val2 = rdo::Factory<rdoRuntime::RDOCalcMinus>::create(val2, const1.object_cast<rdoRuntime::RDOCalc>()).object_cast<rdoRuntime::RDOCalc>();
		}
		rdoRuntime::LPRDOCalcConst const2 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(range);
		const2->setSrcInfo( param->src_info() );
		rdoRuntime::LPRDOCalcMult mult = rdo::Factory<rdoRuntime::RDOCalcMultEnumSafe>::create(const2.object_cast<rdoRuntime::RDOCalc>(), val2).object_cast<rdoRuntime::RDOCalcMult>();
		rdoRuntime::LPRDOCalcPlus add  = rdo::Factory<rdoRuntime::RDOCalcPlusEnumSafe>::create(mult.object_cast<rdoRuntime::RDOCalc>(), calc).object_cast<rdoRuntime::RDOCalcPlus>();

		rbool found = false;
		switch (param->getType()->type()->typeID())
		{
		case rdoRuntime::RDOType::t_int:
			if (dynamic_cast<PTR(RDOTypeIntRange)>(param->getType()->type().get()))
			{
				LPRDOTypeIntRange pRange = param->getType()->type().object_cast<RDOTypeIntRange>();
				if (pRange->range()->getMax().value().getInt() != 1)
				{
					parser()->error().error(pRange->range()->src_info(), rdo::format(_T("����������� �������� ��������� ������ ���� 1, ������� �������� [%d..%d]"), pRange->range()->getMin().value().getInt(), pRange->range()->getMax().value().getInt()));
				}
				range *= pRange->range()->getMax().value().getInt() - pRange->range()->getMin().value().getInt() + 1;
			}
			else
			{
				parser()->error().error(param->getType()->src_info(), _T("��� ��������� ��������� ������� ������ ���� ����� ���������� ��������"));
			}
			break;
		case rdoRuntime::RDOType::t_enum:
			range *= param->getType()->type().object_cast<RDOEnumType>()->getEnums().getValues().size();
			break;
		default:
			parser()->error().error(param->getType()->src_info(), _T("�������� ��������� ������� ����� ���� ������ ��� ������������� ����"));
		}
		calc = add.object_cast<rdoRuntime::RDOCalc>();
	}

	if ( elements.size() != range ) {
		parser()->error().error( _elements_pos, rdo::format("�������� ���������� ��������� ��������� ������� '%s': %d, ��������� %d", name().c_str(), elements.size(), range) );
	}

	rdoRuntime::LPRDOFuncTableCalc fun_calc = rdo::Factory<rdoRuntime::RDOFuncTableCalc>::create(calc);
	fun_calc->setSrcInfo( src_info() );
	for ( int currElem = 0; currElem < range; currElem++ ) {
		const RDOFUNFunctionListElement* const elem = elements.at(currElem);
		rdoRuntime::LPRDOCalcConst result_calc = elem->createResultCalc(getType(), elem->src_info());
		fun_calc->addResultCalc( result_calc );
	}
	setFunctionCalc( fun_calc.object_cast<rdoRuntime::RDOFunCalc>() );
}

void RDOFUNFunction::createAlgorithmicCalc( const RDOParserSrcInfo& _body_src_info )
{
	rdoRuntime::LPRDOFunAlgorithmicCalc fun_calc;
	// ������ �� �������
	switch (getType()->type()->typeID())
	{
		case rdoRuntime::RDOType::t_int:
		{
			LPRDOTypeIntRange pRange = getType()->type().object_dynamic_cast<RDOTypeIntRange>();
			if (pRange)
			{
				fun_calc = rdo::Factory<rdoRuntime::RDOFunAlgorithmicDiapCalc>::create(pRange->range()->getMin().value(), pRange->range()->getMax().value()).object_cast<rdoRuntime::RDOFunAlgorithmicCalc>();
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			LPRDOTypeRealRange pRange = getType()->type().object_dynamic_cast<RDOTypeRealRange>();
			if (pRange)
			{
				fun_calc = rdo::Factory<rdoRuntime::RDOFunAlgorithmicDiapCalc>::create(pRange->range()->getMin().value(), pRange->range()->getMax().value()).object_cast<rdoRuntime::RDOFunAlgorithmicCalc>();
			}
			break;
		}
	}
	if ( !fun_calc )
	{
		fun_calc = rdo::Factory<rdoRuntime::RDOFunAlgorithmicCalc>::create();
	}
	fun_calc->setSrcInfo( src_info() );
	bool default_flag = false;
	bool true_const   = false;
	rdoRuntime::LPRDOCalcConst calc_cond_const;
	int size = calculateIf.size();
	int cnt = 0;
	for ( int i = 0; i < size; i++ ) {
		rdoRuntime::LPRDOCalc logic_calc = calculateIf[i]->condition->getCalc(getType()->type()->typeID());
		rdoRuntime::LPRDOCalcConst calc_cond_last = logic_calc.object_dynamic_cast<rdoRuntime::RDOCalcConst>();
		if ( true_const ) {
			parser()->error().warning( calculateIf[i]->condition->src_info(), rdo::format("������� �� ������������: %s", calculateIf[i]->condition->src_text().c_str()) );
			parser()->error().warning( calc_cond_const->src_info(), rdo::format("��������� ������� ������� �������: %s", calc_cond_const->src_text().c_str()) );
		} else if ( !calc_cond_last || calc_cond_last->calcValue( parser()->runtime() ).getAsBool() ) {
			// ���������� ������ false-������� ���������� ���������
			fun_calc->addCalcIf( logic_calc, calculateIf[i]->action->createCalc(getType()) );
			cnt++;
		}
		if ( !default_flag && calc_cond_last && calc_cond_last->calcValue( parser()->runtime() ).getAsBool() ) {
			true_const   = true;
			default_flag = true;
			calc_cond_const = calc_cond_last;
		}
	}
	if ( !cnt ) {
		parser()->error().warning( src_info(), rdo::format("����������� ���� ������� '%s'", name().c_str()) );
	}
	if ( !true_const ) {
		if (getType()->default().defined())
		{
			rdoRuntime::LPRDOCalcConst calc_cond = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
			rdoRuntime::LPRDOCalcConst calc_act  = rdo::Factory<rdoRuntime::RDOCalcConst>::create(getType()->value_cast(getType()->default()).value());
			calc_cond->setSrcInfo( getType()->src_info() );
			calc_act->setSrcInfo( getType()->src_info() );
			fun_calc->addCalcIf( calc_cond.object_cast<rdoRuntime::RDOCalc>(), calc_act.object_cast<rdoRuntime::RDOCalc>() );
			default_flag = true;
		}
	}
	if ( !default_flag )
	{
		//! ��������� �������������� �������� ��-���������, ���� ��� �� ������ � ����� ����
		rdoRuntime::LPRDOCalcConst calc_cond = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
		rdoRuntime::LPRDOCalcConst calc_act  = rdo::Factory<rdoRuntime::RDOCalcConst>::create(getType()->type()->get_default().value());
		calc_cond->setSrcInfo( getType()->src_info() );
		calc_act->setSrcInfo( getType()->src_info() );
		fun_calc->addCalcIf( calc_cond.object_cast<rdoRuntime::RDOCalc>(), calc_act.object_cast<rdoRuntime::RDOCalc>() );
		parser()->error().warning( src_info(), rdo::format("��� ������� '%s' ������������ �������� ��-���������", name().c_str()) );
	}
	setFunctionCalc( fun_calc.object_cast<rdoRuntime::RDOFunCalc>() );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroup
// ----------------------------------------------------------------------------
RDOFUNGroup::RDOFUNGroup( RDOParser* _parser, const RDOParserSrcInfo& _res_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _res_info )
{
	init( _res_info );
}

RDOFUNGroup::RDOFUNGroup( const RDOParserObject* _parent, const RDOParserSrcInfo& _res_info ):
	RDOParserObject( _parent ),
	RDOParserSrcInfo( _res_info )
{
	init( _res_info );
}

void RDOFUNGroup::init( const RDOParserSrcInfo& _res_info )
{
	resType = parser()->findRTPResType( _res_info.src_text() );
	if ( !resType ) {
		parser()->error().error( _res_info, rdo::format("����������� ��� �������: %s", _res_info.src_text().c_str()) );
	}
	parser()->insertFUNGroup( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroupLogic
// ----------------------------------------------------------------------------
RDOFUNLogic* RDOFUNGroupLogic::createFunLogic( RDOFUNLogic* cond )
{
	rdoRuntime::LPRDOFunCalcGroup calc;
	switch ( funType ) {
		case fgt_exist    : setSrcText( "Exist(" + resType->name() + ": " + cond->src_text() + ")" );     calc = rdo::Factory<rdoRuntime::RDOFunCalcExist    >::create(resType->getNumber(), cond->getCalc()).object_cast<rdoRuntime::RDOFunCalcGroup>(); break;
		case fgt_notexist : setSrcText( "NotExist(" + resType->name() + ": " + cond->src_text() + ")" );  calc = rdo::Factory<rdoRuntime::RDOFunCalcNotExist >::create(resType->getNumber(), cond->getCalc()).object_cast<rdoRuntime::RDOFunCalcGroup>(); break;
		case fgt_forall   : setSrcText( "ForAll(" + resType->name() + ": " + cond->src_text() + ")" );    calc = rdo::Factory<rdoRuntime::RDOFunCalcForAll   >::create(resType->getNumber(), cond->getCalc()).object_cast<rdoRuntime::RDOFunCalcGroup>(); break;
		case fgt_notforall: setSrcText( "NotForAll(" + resType->name() + ": " + cond->src_text() + ")" ); calc = rdo::Factory<rdoRuntime::RDOFunCalcNotForAll>::create(resType->getNumber(), cond->getCalc()).object_cast<rdoRuntime::RDOFunCalcGroup>(); break;
		default: parser()->error().error( src_info(), "���������� ������: �������������� ��� �������" );
	}
	parser()->getFUNGroupStack().pop_back();
	RDOFUNLogic* logic = new RDOFUNLogic( this, calc.object_cast<rdoRuntime::RDOCalc>() );
	logic->setSrcInfo( src_info() );
	return logic;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSelect
// ----------------------------------------------------------------------------
// ��� Select ��� ������� �� ���� � �������
void RDOFUNSelect::initSelect( const RDOFUNLogic* cond )
{
	select = rdo::Factory<rdoRuntime::RDOFunCalcSelect>::create(resType->getNumber(), const_cast<RDOFUNLogic*>(cond)->getCalc());
	select->setSrcInfo( cond->src_info() );
}

// ��������� ������� ��� �������� Select'�
RDOFUNLogic* RDOFUNSelect::createFunSelectGroup( RDOFUNGroupLogic::FunGroupType funType, RDOFUNLogic* cond )
{
	ASSERT(cond);
	rdoRuntime::LPRDOFunCalcSelectBase calc;
	switch ( funType ) {
		case RDOFUNGroupLogic::fgt_exist    : setSrcText( src_text() + ".Exist(" + cond->src_text() + ")" );     calc = rdo::Factory<rdoRuntime::RDOFunCalcSelectExist    >::create(select, cond->getCalc()).object_cast<rdoRuntime::RDOFunCalcSelectBase>(); break;
		case RDOFUNGroupLogic::fgt_notexist : setSrcText( src_text() + ".NotExist(" + cond->src_text() + ")" );  calc = rdo::Factory<rdoRuntime::RDOFunCalcSelectNotExist >::create(select, cond->getCalc()).object_cast<rdoRuntime::RDOFunCalcSelectBase>(); break;
		case RDOFUNGroupLogic::fgt_forall   : setSrcText( src_text() + ".ForAll(" + cond->src_text() + ")" );    calc = rdo::Factory<rdoRuntime::RDOFunCalcSelectForAll   >::create(select, cond->getCalc()).object_cast<rdoRuntime::RDOFunCalcSelectBase>(); break;
		case RDOFUNGroupLogic::fgt_notforall: setSrcText( src_text() + ".NotForAll(" + cond->src_text() + ")" ); calc = rdo::Factory<rdoRuntime::RDOFunCalcSelectNotForAll>::create(select, cond->getCalc()).object_cast<rdoRuntime::RDOFunCalcSelectBase>(); break;
		default: parser()->error().error(cond->src_info(), _T("���������� ������: ����������� ����� ��� ������ ��������"));
	}
	parser()->getFUNGroupStack().pop_back();
	RDOFUNLogic* logic = new RDOFUNLogic( this, calc.object_cast<rdoRuntime::RDOCalc>() );
	logic->setSrcInfo( src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNSelect::createFunSelectEmpty( const RDOParserSrcInfo& _empty_info )
{
	setSrcText( src_text() + "." + _empty_info.src_text() );
	parser()->getFUNGroupStack().pop_back();
	RDOFUNLogic* logic = new RDOFUNLogic( this, rdo::Factory<rdoRuntime::RDOFunCalcSelectEmpty>::create(select).object_cast<rdoRuntime::RDOCalc>() );
	logic->setSrcInfo( _empty_info );
	return logic;
}

RDOFUNArithm* RDOFUNSelect::createFunSelectSize( const RDOParserSrcInfo& _size_info )
{
	setSrcText( src_text() + "." + _size_info.src_text() );
	parser()->getFUNGroupStack().pop_back();
	RDOFUNArithm* arithm = new RDOFUNArithm( this, RDOValue(g_int.object_cast<RDOType>(), _size_info), rdo::Factory<rdoRuntime::RDOFunCalcSelectSize>::create(select).object_cast<rdoRuntime::RDOCalc>() );
	arithm->setSrcInfo( _size_info );
	return arithm;
}

} // namespace rdoParse
