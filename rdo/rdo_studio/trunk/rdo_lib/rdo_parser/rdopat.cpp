#include "rdo_lib/rdo_parser/pch.h"
#include "rdo_lib/rdo_parser/rdopat.h"
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"

namespace rdoParse 
{

int patlex(PTR(YYSTYPE) lpval, PTR(YYLTYPE) llocp, PTR(void) lexer)
{
	LEXER->m_lpval = lpval;
	LEXER->m_lploc = llocp;
	return LEXER->yylex();
}

void paterror(PTR(char) mes)
{}

// ----------------------------------------------------------------------------
// ---------- RDOPATPattern
// ----------------------------------------------------------------------------
RDOPATPattern::RDOPATPattern( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _name_src_info ),
	commonChoice( NULL ),
	patRuntime( NULL ),
	currRelRes( NULL ),
	current_rel_res_index( 0 ),
	useCommonChoice( false )
{
	const RDOPATPattern* pattern = parser()->findPATPattern( src_info().src_text() );
	if ( pattern ) {
		parser()->error().push_only( src_info(), rdo::format("������� '%s' ��� ����������", name().c_str()) );
		parser()->error().push_only( pattern->src_info(), "��. ������ �����������" );
		parser()->error().push_done();
//		parser()->error().error( "Pattern with name: " + name + " already exist" );
	}
	parser()->insertPATPattern( this );
}

std::string RDOPATPattern::StatusToStr( rdoRuntime::RDOResource::ConvertStatus value )
{
	switch ( value ) {
		case rdoRuntime::RDOResource::CS_Keep    : return "Keep";
		case rdoRuntime::RDOResource::CS_Create  : return "Create";
		case rdoRuntime::RDOResource::CS_Erase   : return "Erase";
		case rdoRuntime::RDOResource::CS_NonExist: return "NonExist";
		case rdoRuntime::RDOResource::CS_NoChange: return "NoChange";
	}
	return ""; // unreachable code
}

rdoRuntime::RDOResource::ConvertStatus RDOPATPattern::StrToStatus( const std::string& value, const YYLTYPE& convertor_pos )
{
	if ( value == "Keep" || value == "keep" ) {
		return rdoRuntime::RDOResource::CS_Keep;
	} else if ( value == "Create" || value == "create" ) {
		return rdoRuntime::RDOResource::CS_Create;
	} else if ( value == "Erase" || value == "erase" ) {
		return rdoRuntime::RDOResource::CS_Erase;
	} else if ( value == "NonExist" || value == "nonexist" ) {
		return rdoRuntime::RDOResource::CS_NonExist;
	} else if ( value == "NoChange" || value == "nochange" ) {
		return rdoRuntime::RDOResource::CS_NoChange;
	} else {
		parser()->error().error( convertor_pos, rdo::format("�������� ������ ����������: %s", value.c_str()) );
//		parser()->error().error( "Wrong converter status: " + value );
	}
	return rdoRuntime::RDOResource::CS_Keep; // unreachable code
}

void RDOPATPattern::beforeRelRensert( const RDOParserSrcInfo& rel_info )
{
	if ( findRelevantResource( rel_info.src_text() ) ) {
		parser()->error().error( rel_info, rdo::format("����������� ������ '%s' ��� ���������", rel_info.src_text().c_str()) );
	}
}

void RDOPATPattern::rel_res_insert( RDORelevantResource* rel_res )
{
	switch ( getType() )
	{
		case PT_IE:
		{
			static_cast<rdoRuntime::RDOPatternIrregEvent*>(getPatRuntime())->addConvertorStatus( rel_res->begin );
			break;
		}
		case PT_Rule:
		{
			static_cast<rdoRuntime::RDOPatternRule*>(getPatRuntime())->addConvertorStatus( rel_res->begin );
			break;
		}
		case PT_Operation:
		{
			static_cast<rdoRuntime::RDOPatternOperation*>(getPatRuntime())->addConvertorBeginStatus( rel_res->begin );
			break;
		}
		case PT_Keyboard:
		{
			static_cast<rdoRuntime::RDOPatternKeyboard*>(getPatRuntime())->addConvertorBeginStatus( rel_res->begin );
			break;
		}
		default:
		{
			parser()->error().error( src_info(), "����������� ��� �������" );
		}
	}
	relRes.push_back( rel_res );
}

void RDOPATPattern::addRelResConvert( bool trace, RDOPATParamSet* parSet, const YYLTYPE& convertor_pos, const YYLTYPE& trace_pos )
{
	if ( parSet->convert_status == rdoRuntime::RDOResource::CS_NoChange || parSet->convert_status == rdoRuntime::RDOResource::CS_NonExist ) {
		parser()->error().error( convertor_pos, getErrorMessage_NotNeedConvertor(parSet) );
	}

	if ( parSet->convert_status == rdoRuntime::RDOResource::CS_Create ) {
		addParamSetCalc( parSet, createRelRes( parSet, trace ) );
	} else {
		if ( trace ) {
			parser()->error().error( trace_pos, "������� ����������� � ������ ����� �������� ������ ��� ������������ �������" );
		}
	}

	int size = parSet->m_params.size();
	if ( !size && parSet->convert_status == rdoRuntime::RDOResource::CS_Keep ) {
		parser()->error().warning( convertor_pos, getWarningMessage_EmptyConvertor(parSet) );
	}
	for ( int i = 0; i < size; i++ ) {
		int parNumb = parSet->m_params.at(i).m_paramID;
		RDOFUNArithm* currArithm = parSet->m_params.at(i).m_rightArithm;
		if ( currArithm ) { // NULL == NoChange
			LPRDORTPParam param = parSet->getRelRes()->getType()->getParams().at(parNumb);
			rdoRuntime::RDOCalc* rightValue = currArithm->createCalc( param->getParamType().get() );
			rdoRuntime::RDOCalc* calc = NULL;
			switch (parSet->m_params.at(i).m_equalType)
			{
			case rdoRuntime::ET_NOCHANGE: break;
			case rdoRuntime::ET_EQUAL   : calc = new rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_EQUAL   >(parser()->runtime(), parSet->getRelRes()->rel_res_id, parNumb, rightValue); break;
			case rdoRuntime::ET_PLUS    : calc = new rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_PLUS    >(parser()->runtime(), parSet->getRelRes()->rel_res_id, parNumb, rightValue); break;
			case rdoRuntime::ET_MINUS   : calc = new rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_MINUS   >(parser()->runtime(), parSet->getRelRes()->rel_res_id, parNumb, rightValue); break;
			case rdoRuntime::ET_MULTIPLY: calc = new rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_MULTIPLY>(parser()->runtime(), parSet->getRelRes()->rel_res_id, parNumb, rightValue); break;
			case rdoRuntime::ET_DIVIDE  : calc = new rdoRuntime::RDOSetRelParamCalc<rdoRuntime::ET_DIVIDE  >(parser()->runtime(), parSet->getRelRes()->rel_res_id, parNumb, rightValue); break;
			default: NEVER_REACH_HERE;
			}
			if (calc)
			{
				// �������� �� ��������
				switch ( param->getParamType()->type()->typeID() )
				{
					case rdoRuntime::RDOType::t_int:
					{
						const RDORTPIntParamType* param_type = static_cast<const RDORTPIntParamType*>(param->getType());
						if ( param_type->getDiap().isExist() )
						{
							calc = new rdoRuntime::RDOSetRelParamDiapCalc( parser()->runtime(), parSet->getRelRes()->rel_res_id, parNumb, param_type->getDiap().getMin(), param_type->getDiap().getMax(), calc );
						}
						break;
					}
					case rdoRuntime::RDOType::t_real:
					{
						const RDORTPRealParamType* param_type = static_cast<const RDORTPRealParamType*>(param->getType());
						if ( param_type->getDiap().isExist() )
						{
							calc = new rdoRuntime::RDOSetRelParamDiapCalc( parser()->runtime(), parSet->getRelRes()->rel_res_id, parNumb, param_type->getDiap().getMin(), param_type->getDiap().getMax(), calc );
						}
						break;
					}
				}
				calc->setSrcText( parSet->m_params.at(i).m_name + " set " + rightValue->src_text() );
				addParamSetCalc( parSet, calc );
			}
		}
	}
}

void RDOPATPattern::addParamSetCalc( const RDOPATParamSet* const parSet, rdoRuntime::RDOCalc* calc )
{
	switch ( getType() )
	{
		case PT_IE:
		{
			static_cast<rdoRuntime::RDOPatternIrregEvent*>(getPatRuntime())->addConvertorCalc( calc );
			break;
		}
		case PT_Rule:
		{
			static_cast<rdoRuntime::RDOPatternRule*>(getPatRuntime())->addConvertorCalc( calc );
			break;
		}
		case PT_Operation:
		{
			static_cast<rdoRuntime::RDOPatternOperation*>(getPatRuntime())->addConvertorBeginCalc( calc );
			break;
		}
		case PT_Keyboard:
		{
			static_cast<rdoRuntime::RDOPatternKeyboard*>(getPatRuntime())->addConvertorBeginCalc( calc );
			break;
		}
		default:
		{
			parser()->error().error( src_info(), "����������� ��� �������" );
		}
	}
}

std::string RDOPATPattern::getPatternId() const
{ 
	return patRuntime->traceId(); 
}

void RDOPATPattern::writeModelStructure( std::ostream& stream ) const
{
	stream << getPatternId() << " " << name() << " " << getModelStructureLetter() << " " << relRes.size();
	for ( unsigned int i = 0; i < relRes.size(); i++ ) {
		stream << " " << relRes.at(i)->getType()->getNumber();
	}
	stream << std::endl;
}

const RDOFUNFunctionParam* RDOPATPattern::findPATPatternParam( const std::string& paramName ) const
{
	std::vector< RDOFUNFunctionParam* >::const_iterator it = std::find_if(params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return NULL;

	return (*it);
}

const RDORelevantResource* RDOPATPattern::findRelevantResource( const std::string& resName ) const
{
	std::vector< RDORelevantResource* >::const_iterator it = std::find_if( relRes.begin(), relRes.end(), compareName<RDORelevantResource>(resName) );
	return it != relRes.end() ? (*it) : NULL;
}

int RDOPATPattern::findPATPatternParamNum( const std::string& paramName ) const
{
	std::vector< RDOFUNFunctionParam* >::const_iterator it = std::find_if( params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName) );
	return it != params.end() ? it - params.begin() : -1;
}

int RDOPATPattern::findRelevantResourceNum( const std::string& resName ) const
{
	std::vector< RDORelevantResource* >::const_iterator it = std::find_if( relRes.begin(), relRes.end(), compareName<RDORelevantResource>(resName) );
	return it != relRes.end() ? it - relRes.begin() : -1;
}

void RDOPATPattern::add( RDOFUNFunctionParam* const _param )
{
	const RDOFUNFunctionParam* param = findPATPatternParam(_param->name());
	if ( param ) {
		parser()->error().push_only( _param->src_info(), rdo::format("�������� '%s' ��� ���������", _param->src_text().c_str()) );
		parser()->error().push_only( param->src_info(), "��. ������ �����������" );
		parser()->error().push_done();
//		parser()->error().error("Second appearance of the same parameter name: " + _param->getName());
	}
	params.push_back( _param ); 
}

void RDOPATPattern::setCommonChoiceFirst()
{
	useCommonChoice = true;
	commonChoice    = NULL;
//	parser()->error().error( "�������� ������, �.�. � ���������� �� ������� ������������ commonChoice" );
}

void RDOPATPattern::setCommonChoiceWithMin( RDOFUNArithm* arithm )
{
	useCommonChoice  = true;
	useCommonWithMax = false;
	commonChoice     = arithm;
}

void RDOPATPattern::setCommonChoiceWithMax( RDOFUNArithm* arithm )
{
	useCommonChoice  = true;
	useCommonWithMax = true;
	commonChoice     = arithm;
}

void RDOPATPattern::setTime( RDOFUNArithm* arithm )
{ 
	switch ( getType() )
	{
		case PT_IE:
		{
			static_cast<rdoRuntime::RDOPatternIrregEvent*>(getPatRuntime())->setTime( arithm->createCalc(NULL) );
			break;
		}
		case PT_Operation:
		{
			static_cast<rdoRuntime::RDOPatternOperation*>(getPatRuntime())->setTime( arithm->createCalc(NULL) );
			break;
		}
		case PT_Keyboard:
		{
			static_cast<rdoRuntime::RDOPatternKeyboard*>(getPatRuntime())->setTime( arithm->createCalc(NULL) );
			break;
		}
		default:
		{
			parser()->error().error( src_info(), rdo::format("��� ������� ���� %s ����������� ������������� ��������� �������", typeToString(getType()).c_str()) );
		}
	}
}

void RDOPATPattern::addChoiceFromCalc( rdoRuntime::RDOCalc* calc )
{
	switch ( getType() )
	{
		case PT_IE:
		{
			static_cast<rdoRuntime::RDOPatternIrregEvent*>(getPatRuntime())->addPreSelectRelRes( calc );
			break;
		}
		case PT_Rule:
		{
			static_cast<rdoRuntime::RDOPatternRule*>(getPatRuntime())->addChoiceFromCalc( calc );
			break;
		}
		case PT_Operation:
		{
			static_cast<rdoRuntime::RDOPatternOperation*>(getPatRuntime())->addChoiceFromCalc( calc );
			break;
		}
		case PT_Keyboard:
		{
			static_cast<rdoRuntime::RDOPatternKeyboard*>(getPatRuntime())->addChoiceFromCalc( calc );
			break;
		}
		default:
		{
			parser()->error().error( src_info(), rdo::format("��� ������� ���� %s ����������� ������������� �������� ������", typeToString(getType()).c_str()) );
		}
	}
}

void RDOPATPattern::addRelResBody( const RDOParserSrcInfo& body_name )
{ 
	std::vector< RDORelevantResource* >::const_iterator it = std::find_if( relRes.begin(), relRes.end(), compareName<RDORelevantResource>(body_name.src_text()) );
	if ( it == relRes.end() ) {
		parser()->error().error( body_name.src_info(), rdo::format("����������� ����������� ������: %s", body_name.src_text().c_str()) );
//		parser()->error().error( "Name of relevant resource expected instead of: " + body_name.src_text() );
	}
	if ( findRelevantResourceNum( body_name.src_text() ) != current_rel_res_index ) {
		std::string rel_res_waiting = current_rel_res_index < relRes.size() ? relRes[current_rel_res_index]->name().c_str() : "";
		parser()->error().error( body_name.src_info(), rdo::format("��������� �������� ������������ ������� '%s', ������ ���� �������: %s", rel_res_waiting.c_str(), body_name.src_text().c_str()) );
	}
	if ( (*it)->alreadyHaveConverter ) {
		parser()->error().error( body_name.src_info(), rdo::format("����������� ������ ��� ������������: %s", body_name.src_text().c_str()) );
//		parser()->error().error( "\"" + body_name.src_text() + "\" relevant resource has converter block already" );
	}
	currRelRes = (*it);
	currRelRes->body_name = body_name;
	currRelRes->alreadyHaveConverter = true;
	current_rel_res_index++;
}

void RDOPATPattern::addRelResUsage( RDOPATChoiceFrom* choice_from, RDOPATChoiceOrder* choice_order )
{
	if ( !useCommonChoice ) {
		if ( choice_order->type == rdoRuntime::RDOSelectResourceCalc::order_empty ) {
			if ( (currRelRes->begin != rdoRuntime::RDOResource::CS_Create) && (currRelRes->end != rdoRuntime::RDOResource::CS_Create) ) {
//				choice_order->type = rdoRuntime::RDOSelectResourceCalc::order_first;
			}
		} else if ( currRelRes->isDirect() ) {
			parser()->error().warning( choice_order->src_info(), rdo::format("������� ������ '%s' ������������ ������� '%s' �� ����� ������, �.�. ����������� ������ ��������� ����� ���, � �� ���, � �� ����� ���� ������ � �����-���� ������ ��������",choice_order->src_text().c_str(), currRelRes->name().c_str()) );
		}
	} else {
		if ( choice_order->type != rdoRuntime::RDOSelectResourceCalc::order_empty ) {
			parser()->error().push_only( choice_order->src_info(), "������ ������� ������ ������ ������������ �������, �.�. ������������ ������ ��� ���� ����������� �������� ������, ��������� �� ��������� ����� $Body" );
			if ( commonChoice ) {
				parser()->error().push_only( commonChoice->src_info(), rdo::format("��. '%s'", commonChoice->src_text().c_str()) );
			}
			parser()->error().push_done();
//			parser()->error().error( "Cannot use both common choice and choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
		}
	}

	if ( (currRelRes->begin == rdoRuntime::RDOResource::CS_Create) || (currRelRes->end == rdoRuntime::RDOResource::CS_Create) ) {
		if ( choice_from->type != RDOPATChoiceFrom::ch_empty ) {
			parser()->error().error( choice_from->src_info(), "����������� ������ ���������, ��� ���� ������ ������������ Choice from ��� Choice NoCheck" );
//			parser()->error().error( "Cannot use choice when create \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
		}
		if ( choice_order->type != rdoRuntime::RDOSelectResourceCalc::order_empty ) {
			parser()->error().error( choice_order->src_info(), rdo::format("����������� ������ ���������, ��� ���� ������ ������������ ������� ������ '%s'", choice_order->asString().c_str()) );
		}
	}

	currRelRes->choice_from  = choice_from;
	currRelRes->choice_order = choice_order;
}

void RDOPATPattern::end()
{
	int size = relRes.size();
	for ( int i = 0; i < size; i++ ) {
		RDORelevantResource* currRelRes = relRes.at( i );
		if ( !currRelRes->alreadyHaveConverter ) {
			// TODO: � ������ ������ ������� warning ? ��������, ���� ������� ���������� ����������� ������� ���. �������.
			parser()->error().error( currRelRes->src_info(), rdo::format("����������� ������ '%s' �� ������������ � ������� '%s'", currRelRes->name().c_str(), name().c_str()) );
		}
		patRuntime->addPreSelectRelRes( currRelRes->createPreSelectRelResCalc() );
	}

	if ( useCommonChoice ) {
		// first
		// �������� �����������, � ��� ������������ first
		if ( commonChoice == NULL ) { // first
			std::vector< rdoRuntime::RDOSelectResourceCommon* > resSelectors;
			for ( int i = 0; i < size; i++ ) {
				if ( relRes.at(i)->begin == rdoRuntime::RDOResource::CS_Keep || relRes.at(i)->begin == rdoRuntime::RDOResource::CS_Erase || relRes.at(i)->begin == rdoRuntime::RDOResource::CS_NoChange ) {
					resSelectors.push_back( relRes.at(i)->createSelectResourceCommonChoiceCalc() );
				}
			}
			rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOSelectResourceCommonCalc( parser()->runtime(), resSelectors, useCommonWithMax, NULL );
			calc->setSrcInfo( src_info() );
			calc->setSrcText( "first" );
			addChoiceFromCalc( calc );
		} else {
			// with_min / with_max
			std::vector< rdoRuntime::RDOSelectResourceCommon* > resSelectors;
			for ( int i = 0; i < size; i++ ) {
				if ( relRes.at(i)->begin == rdoRuntime::RDOResource::CS_Keep || relRes.at(i)->begin == rdoRuntime::RDOResource::CS_Erase || relRes.at(i)->begin == rdoRuntime::RDOResource::CS_NoChange ) {
					resSelectors.push_back( relRes.at(i)->createSelectResourceCommonChoiceCalc() );
				}
			}
			rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOSelectResourceCommonCalc( parser()->runtime(), resSelectors, useCommonWithMax, commonChoice->createCalc() );
			calc->setSrcInfo( commonChoice->src_info() );
			addChoiceFromCalc( calc );
		}
	} else {
		for ( int i = 0; i < size; i++ )
		{
			rdoRuntime::RDOCalc* calc = relRes.at(i)->createSelectResourceChoiceCalc();
			addChoiceFromCalc( calc );
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternIrregEvent
// ----------------------------------------------------------------------------
RDOPatternIrregEvent::RDOPatternIrregEvent( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool trace ):
	RDOPATPattern( _parser, _name_src_info )
{ 
	patRuntime = new rdoRuntime::RDOPatternIrregEvent( parser()->runtime(), trace ); 
	patRuntime->setTraceID( parser()->getPAT_id() );
}

void RDOPatternIrregEvent::addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResource::ConvertStatus beg, const YYLTYPE& convertor_pos )
{
	beforeRelRensert( rel_info );
	if ( beg == rdoRuntime::RDOResource::CS_NonExist || beg == rdoRuntime::RDOResource::CS_NoChange ) {
		parser()->error().error( convertor_pos, "������� ����������� NonExist � NoChange �� ����� ���� ������������ � ������������ �������" );
	}

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = parser()->findRSSResource( type_info.src_text() );
	if ( res ) {
		if ( beg == rdoRuntime::RDOResource::CS_Create ) {
			parser()->error().error( type_info, rdo::format("��� �������� ������� '%s' ��������� ������� ��� ���, �� ������ ������ ������ (%s)", rel_info.src_text().c_str(), type_info.src_text().c_str()) );
		}
		if ( beg == rdoRuntime::RDOResource::CS_Erase ) {
			parser()->error().error( convertor_pos, "������� ������� � ������������ ������� ������" );
		}
		relRes = new RDORelevantResourceDirect( this, rel_info, rel_res_count(), res, beg );
		rel_res_insert( relRes );
	} else {
		const RDORTPResType* const type = parser()->findRTPResType( type_info.src_text() );
		if ( !type ) {
			parser()->error().error( type_info, rdo::format("����������� ��� �������: %s", type_info.src_text().c_str()) );
//			parser()->error().error( "Unknown resource name or type: " + *type_info.src_text() );
		}
		if ( beg == rdoRuntime::RDOResource::CS_Create && !type->isTemporary() ) {
			parser()->error().error( type_info, rdo::format("��� ������� '%s' ����������. ����������� ��������� �� ���� ������� ������", type_info.src_text().c_str()) );
		}
		if ( beg == rdoRuntime::RDOResource::CS_Keep || beg == rdoRuntime::RDOResource::CS_Erase ) {
			parser()->error().error( convertor_pos, "������� ����������� Keep � Erase ����� ���� ������������ � ������������ ������� � ���������� � ���� �������, �� �� ���� �������" );
//			parser()->error().error( "Can use Keep and Erase status with resource name only (not with resource type) in irregular event" );
		}
		relRes = new RDORelevantResourceByType( this, rel_info, rel_res_count(), type, beg );
		rel_res_insert( relRes );
	}
	if ( relRes->begin == rdoRuntime::RDOResource::CS_Erase ) {
		rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOCalcEraseRes( parser()->runtime(), relRes->rel_res_id, relRes->name() );
		calc->setSrcInfo( rel_info );
		calc->setSrcText( rdo::format("�������� ���������� ������� %s", rel_info.src_text().c_str()) );
		static_cast<rdoRuntime::RDOPatternIrregEvent*>(getPatRuntime())->addEraseCalc( calc );
	}
}

void RDOPatternIrregEvent::addRelResUsage( RDOPATChoiceFrom* choice_from, RDOPATChoiceOrder* choice_order )
{
	if ( choice_from->type != RDOPATChoiceFrom::ch_empty ) {
		parser()->error().error( choice_from->src_info(), "����������� ������� ������������� ������� ������ �������� � ������� Choice from ��� Choice NoCheck" );
//		parser()->error().error( "Unexpected choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + getName() + "\"" );
	}
	if ( choice_order->type != rdoRuntime::RDOSelectResourceCalc::order_empty ) {
		parser()->error().error( choice_from->src_info(), rdo::format("��� ����������� �������� ������������� ������� ������ ������������ ������� ������ '%s'", choice_order->asString().c_str()) );
	}
	currRelRes->choice_from  = choice_from;
	currRelRes->choice_order = choice_order;
}

rdoRuntime::RDOCalc* RDOPATPattern::createRelRes( const RDOPATParamSet* const parSet, bool trace ) const
{
	std::vector< rdoRuntime::RDOValue > params_default;
	std::vector< const RDORTPParam* >::const_iterator it = currRelRes->getType()->getParams().begin();
	while ( it != currRelRes->getType()->getParams().end() ) {
		if ( !(*it)->getType()->getDV().isExist() ) {
			params_default.push_back( rdoRuntime::RDOValue(0) );
			bool set_found = false;
			RDOPATParamSet::ParamList::const_iterator set_it = parSet->m_params.begin();
			while (set_it != parSet->m_params.end())
			{
				if ((*it)->name() == set_it->m_name && set_it->m_rightArithm)
				{
					set_found = true;
					break;
				}
				set_it++;
			}
			if ( !set_found ) {
				parser()->error().error( parSet->src_info(), rdo::format("��� �������� ������� ���������� ���������� ��� ��� ���������. �� ������� ����������� ���������: %s", (*it)->name().c_str()));
			}
		} else {
			params_default.push_back( (*it)->getType()->getDefaultValue( (*it)->getType()->getDV().value() ) );
		}
		it++;
	}
	rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOCalcCreateEmptyResource( parser()->runtime(), currRelRes->getType()->getNumber(), trace, params_default, currRelRes->rel_res_id );
	calc->setSrcInfo( currRelRes->src_info() );
	calc->setSrcText( rdo::format("�������� ���������� ������� %s", currRelRes->name().c_str()) );
	return calc;
}

std::string RDOPatternIrregEvent::getErrorMessage_NotNeedConvertor( const RDOPATParamSet* const parSet )
{
	return rdo::format("��� ������������ ������� '%s' �� ��������� ��������� (Convert_event), �.�. ��� ������: %s", parSet->getRelRes()->name().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str());
}

std::string RDOPatternIrregEvent::getWarningMessage_EmptyConvertor( const RDOPATParamSet* const parSet )
{
	return rdo::format("��� ������������ ������� '%s' ������ ������ ��������� (Convert_event), ���� ��� ������: %s", parSet->getRelRes()->name().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str());
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternRule
// ----------------------------------------------------------------------------
RDOPatternRule::RDOPatternRule( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool trace ):
	RDOPATPattern( _parser, _name_src_info )
{ 
//	parser()->runtime()->addRuntimeRule((RDOPatternRule *)(patRuntime = new RDOPatternRule(parser()->runtime(), _trace))); 
	patRuntime = new rdoRuntime::RDOPatternRule( parser()->runtime(), trace );
	patRuntime->setTraceID( parser()->getPAT_id() );
}

void RDOPatternRule::addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResource::ConvertStatus beg, const YYLTYPE& convertor_pos )
{
	beforeRelRensert( rel_info );
	if ( beg == rdoRuntime::RDOResource::CS_NonExist ) {
		parser()->error().error( convertor_pos, rdo::format("������ ������������ ������ ���������� '%s' � ������������� �������", RDOPATPattern::StatusToStr(beg).c_str()) );
//		parser()->error().error( "Cannot use NonExist status in rule" );
	}

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = parser()->findRSSResource( type_info.src_text() );
	if ( res ) {
		if ( beg == rdoRuntime::RDOResource::CS_Create ) {
			parser()->error().error( type_info, rdo::format("��� �������� ������� '%s' ��������� ������� ��� ���, �� ������ ������ ������ (%s)", rel_info.src_text().c_str(), type_info.src_text().c_str()) );
//			parser()->error().error( "Cannot use Create status for resource, only for resource type" );
		}
		if ( beg == rdoRuntime::RDOResource::CS_Erase ) {
			parser()->error().error( convertor_pos, rdo::format("������������ ������ ���������� ��� �������: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
//			parser()->error().error( "Wrong converter status for resource of permanent type" );
		}
		relRes = new RDORelevantResourceDirect( this, rel_info, rel_res_count(), res, beg );
		rel_res_insert( relRes );

	} else {
		const RDORTPResType* const type = parser()->findRTPResType( type_info.src_text() );
		if ( !type ) {
			parser()->error().error( type_info, rdo::format("����������� ��� �������: %s", type_info.src_text().c_str()) );
//			parser()->error().error( "Unknown resource name or type: " + *type_info.src_text() );
		}
		if ( !type->isTemporary() && (beg == rdoRuntime::RDOResource::CS_Create || beg == rdoRuntime::RDOResource::CS_Erase ) ) {
			parser()->error().error( type_info, rdo::format("��� �������� ��� �������� ������� ��� ��� ������ ���� ��������� (temporary), � �� ���������� (permanent), ��� � '%s'", type_info.src_text().c_str()) );
		}
		relRes = new RDORelevantResourceByType( this, rel_info, rel_res_count(), type, beg );
		rel_res_insert( relRes );
	}
	if ( relRes->begin == rdoRuntime::RDOResource::CS_Erase ) {
		rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOCalcEraseRes( parser()->runtime(), relRes->rel_res_id, relRes->name() );
		calc->setSrcInfo( rel_info );
		calc->setSrcText( rdo::format("�������� ���������� ������� %s", rel_info.src_text().c_str()) );
		static_cast<rdoRuntime::RDOPatternRule*>(getPatRuntime())->addEraseCalc( calc );
	}
}

std::string RDOPatternRule::getErrorMessage_NotNeedConvertor( const RDOPATParamSet* const parSet )
{
	return rdo::format("��� ������������ ������� '%s' �� ��������� ��������� (Convert_rule), �.�. ��� ������: %s", parSet->getRelRes()->name().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str());
}

std::string RDOPatternRule::getWarningMessage_EmptyConvertor( const RDOPATParamSet* const parSet )
{
	return rdo::format("��� ������������ ������� '%s' ������ ������ ��������� (Convert_rule), ���� ��� ������: %s", parSet->getRelRes()->name().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str());
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternOperation
// ----------------------------------------------------------------------------
RDOPatternOperation::RDOPatternOperation( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool trace ):
	RDOPATPattern( _parser, _name_src_info )
{ 
	patRuntime = new rdoRuntime::RDOPatternOperation( parser()->runtime(), trace );
	patRuntime->setTraceID( parser()->getPAT_id() );
}

RDOPatternOperation::RDOPatternOperation( RDOParser* _parser, bool trace, const RDOParserSrcInfo& _name_src_info ):
	RDOPATPattern( _parser, _name_src_info )
{
}

void RDOPatternOperation::rel_res_insert( RDORelevantResource* rel_res )
{
	RDOPATPattern::rel_res_insert( rel_res );
	static_cast<rdoRuntime::RDOPatternOperation*>(getPatRuntime())->addConvertorEndStatus( rel_res->end );
}

void RDOPatternOperation::addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResource::ConvertStatus beg, const YYLTYPE& convertor_pos )
{
	parser()->error().error( convertor_pos, "���������� ������ �������" );
}

void RDOPatternOperation::addRelRes( const RDOParserSrcInfo& rel_info, const RDOParserSrcInfo& type_info, rdoRuntime::RDOResource::ConvertStatus beg, rdoRuntime::RDOResource::ConvertStatus end, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos )
{
	beforeRelRensert( rel_info );
	switch ( beg ) {
		case rdoRuntime::RDOResource::CS_Keep:
			if ( end != rdoRuntime::RDOResource::CS_Keep && end != rdoRuntime::RDOResource::CS_Erase && end != rdoRuntime::RDOResource::CS_NoChange ) {
				parser()->error().error( convertor_end_pos, rdo::format("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser()->error().error( "Wrong second converter status" );
			}
			break;
		case rdoRuntime::RDOResource::CS_Create:
			if ( end != rdoRuntime::RDOResource::CS_Keep && end != rdoRuntime::RDOResource::CS_Erase && end != rdoRuntime::RDOResource::CS_NoChange ) {
				parser()->error().error( convertor_end_pos, rdo::format("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser()->error().error( "Wrong second converter status" );
			}
			break;
		case rdoRuntime::RDOResource::CS_Erase:
			if ( end != rdoRuntime::RDOResource::CS_NonExist ) {
				parser()->error().error( convertor_end_pos, rdo::format("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser()->error().error( "Wrong second converter status" );
			}
			break;
		case rdoRuntime::RDOResource::CS_NonExist:
			if ( end != rdoRuntime::RDOResource::CS_Create ) {
				parser()->error().error( convertor_end_pos, rdo::format("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser()->error().error( "Wrong second converter status" );
			}
			break;
		case rdoRuntime::RDOResource::CS_NoChange:
			if ( end != rdoRuntime::RDOResource::CS_Keep && end != rdoRuntime::RDOResource::CS_Erase && end != rdoRuntime::RDOResource::CS_NoChange ) {
				parser()->error().error( convertor_end_pos, rdo::format("������ ���������� ����� ����������� �� �������� ���������� ������: %s � %s", RDOPATPattern::StatusToStr(beg).c_str(), RDOPATPattern::StatusToStr(end).c_str()) );
//				parser()->error().error( "Wrong second converter status" );
			}
			break;
	};

	RDORelevantResource* relRes     = NULL;
	const RDORSSResource* const res = parser()->findRSSResource( type_info.src_text() );
	if ( res ) {
		if ( beg == rdoRuntime::RDOResource::CS_Create ) {
			parser()->error().error( convertor_begin_pos, rdo::format("��� �������� ������� '%s' ��������� ������� ��� ���, �� ������ ������ ������ (%s)", rel_info.src_text().c_str(), type_info.src_text().c_str()) );
		}
		if ( end == rdoRuntime::RDOResource::CS_Create ) {
			parser()->error().error( convertor_end_pos, rdo::format("��� �������� ������� '%s' ��������� ������� ��� ���, �� ������ ������ ������ (%s)", rel_info.src_text().c_str(), type_info.src_text().c_str()) );
		}
		if ( beg == rdoRuntime::RDOResource::CS_Erase || beg == rdoRuntime::RDOResource::CS_NonExist ) {
			parser()->error().error( convertor_begin_pos, rdo::format("������������ ������ ���������� ������ ��� �������: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
		}
		if ( end == rdoRuntime::RDOResource::CS_Erase || end == rdoRuntime::RDOResource::CS_NonExist ) {
			parser()->error().error( convertor_end_pos, rdo::format("������������ ������ ���������� ����� ��� �������: %s", RDOPATPattern::StatusToStr(end).c_str()) );
		}
		relRes = new RDORelevantResourceDirect( this, rel_info, rel_res_count(), res, beg, end );
		rel_res_insert( relRes );

	} else {
		const RDORTPResType* const type = parser()->findRTPResType( type_info.src_text() );
		if ( !type ) {
			parser()->error().error( type_info, rdo::format("����������� ��� �������: %s", type_info.src_text().c_str()) );
//			parser()->error().error( "Unknown resource name or type: " + *type_info.src_text() );
		}
		if ( type->isPermanent() ) {
			if ( beg == rdoRuntime::RDOResource::CS_Create || beg == rdoRuntime::RDOResource::CS_Erase || beg == rdoRuntime::RDOResource::CS_NonExist ) {
				parser()->error().error( convertor_begin_pos, rdo::format("������������ ������ ���������� ������ ��� ����������� ����: %s", RDOPATPattern::StatusToStr(beg).c_str()) );
			}
			if ( end == rdoRuntime::RDOResource::CS_Create || end == rdoRuntime::RDOResource::CS_Erase || end == rdoRuntime::RDOResource::CS_NonExist ) {
				parser()->error().error( convertor_end_pos, rdo::format("������������ ������ ���������� ����� ��� ����������� ����: %s", RDOPATPattern::StatusToStr(end).c_str()) );
			}
		}
		relRes = new RDORelevantResourceByType( this, rel_info, rel_res_count(), type, beg, end );
		rel_res_insert( relRes );
	}
	if ( relRes->begin == rdoRuntime::RDOResource::CS_Erase ) {
		rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOCalcEraseRes( parser()->runtime(), relRes->rel_res_id, relRes->name() );
		calc->setSrcInfo( rel_info );
		calc->setSrcText( rdo::format("�������� ���������� ������� %s", rel_info.src_text().c_str()) );
		static_cast<rdoRuntime::RDOPatternOperation*>(getPatRuntime())->addEraseBeginCalc( calc );
	}
	if ( relRes->end == rdoRuntime::RDOResource::CS_Erase ) {
		rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOCalcEraseRes( parser()->runtime(), relRes->rel_res_id, relRes->name() );
		calc->setSrcInfo( rel_info );
		calc->setSrcText( rdo::format("�������� ���������� ������� %s", rel_info.src_text().c_str()) );
		static_cast<rdoRuntime::RDOPatternOperation*>(getPatRuntime())->addEraseEndCalc( calc );
	}
}

void RDOPatternOperation::addRelResConvertBeginEnd( bool trace_begin, RDOPATParamSet* parSet_begin, bool trace_end, RDOPATParamSet* parSet_end, const YYLTYPE& convertor_begin_pos, const YYLTYPE& convertor_end_pos, const YYLTYPE& trace_begin_pos, const YYLTYPE& trace_end_pos )
{
	if ( parSet_begin ) {
		addRelResConvert( trace_begin, parSet_begin, convertor_begin_pos, trace_begin_pos );
	}
	if ( parSet_end ) {
		addRelResConvert( trace_end, parSet_end, convertor_end_pos, trace_end_pos );
	}
}

void RDOPatternOperation::addParamSetCalc( const RDOPATParamSet* const parSet, rdoRuntime::RDOCalc* calc )
{
	switch (parSet->getRelRes()->getConvertorType(parSet))
	{
		case convert_begin: static_cast<PTR(rdoRuntime::RDOPatternOperation)>(getPatRuntime())->addConvertorBeginCalc(calc); break;
		case convert_end  : static_cast<PTR(rdoRuntime::RDOPatternOperation)>(getPatRuntime())->addConvertorEndCalc  (calc); break;
		default           : NEVER_REACH_HERE;
	}
}

std::string RDOPatternOperation::getErrorMessage_NotNeedConvertor( const RDOPATParamSet* const parSet )
{
	switch ( parSet->getRelRes()->getConvertorType(parSet) ) {
		case convert_begin: return rdo::format("��� ������������ ������� '%s' �� ��������� ��������� ������ (Convert_begin), �.�. ��� ������: %s", parSet->getRelRes()->name().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str()); break;
		case convert_end  : return rdo::format("��� ������������ ������� '%s' �� ��������� ��������� ����� (Convert_end), �.�. ��� ������: %s", parSet->getRelRes()->name().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str()); break;
	}
	return "���������� ������ �������";
}

std::string RDOPatternOperation::getWarningMessage_EmptyConvertor( const RDOPATParamSet* const parSet )
{
	switch ( parSet->getRelRes()->getConvertorType(parSet) ) {
		case convert_begin: return rdo::format("��� ������������ ������� '%s' ������ ������ ��������� ������ (Convert_begin), ���� ��� ������: %s", parSet->getRelRes()->name().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str()); break;
		case convert_end  : return rdo::format("��� ������������ ������� '%s' ������ ������ ��������� ����� (Convert_end), ���� ��� ������: %s", parSet->getRelRes()->name().c_str(), RDOPATPattern::StatusToStr(parSet->convert_status).c_str()); break;
	}
	return "���������� ������ �������";
}

// ----------------------------------------------------------------------------
// ---------- RDOPatternKeyboard
// ----------------------------------------------------------------------------
RDOPatternKeyboard::RDOPatternKeyboard( RDOParser* _parser, const RDOParserSrcInfo& _name_src_info, bool _trace ):
	RDOPatternOperation( _parser, _trace, _name_src_info )
{
	patRuntime = new rdoRuntime::RDOPatternKeyboard( parser()->runtime(), _trace ); 
	patRuntime->setTraceID( parser()->getPAT_id() );
}

// ----------------------------------------------------------------------------
// ---------- RDORelevantResource
// ----------------------------------------------------------------------------
RDOPATParamSet* RDORelevantResource::createParamSet()
{
	if ( !param_set_begin ) {
		param_set_begin = new RDOPATParamSet( this, begin );
		return param_set_begin;
	} else if ( !param_set_end ) {
		param_set_end = new RDOPATParamSet( this, end );
		return param_set_end;
	}
	parser()->error().error( src_info(), "���������� ������ �������" );
	return NULL;
}

void RDORelevantResource::deleteParamSetBegin()
{
	if ( param_set_begin ) {
		delete param_set_begin;
		param_set_begin = NULL;
	}
}

rdoRuntime::RDOCalc* RDORelevantResource::getChoiceCalc() const
{
	if ( choice_from && choice_from->type == rdoParse::RDOPATChoiceFrom::ch_from ) {
		return choice_from->logic->getCalc( rdoRuntime::RDOType::t_int );
	}
	return NULL;
}

rdoRuntime::RDOCalc* RDORelevantResource::getSelectCalc() const
{
	if ( choice_order && choice_order->arithm ) {
		return choice_order->arithm->createCalc( NULL );
	}
	return NULL;
}

rdoRuntime::RDOSelectResourceCalc::Type RDORelevantResource::getSelectType() const
{
	return choice_order ? choice_order->type : rdoRuntime::RDOSelectResourceCalc::order_empty;
}

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceDirect - �� ����� �������
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalc* RDORelevantResourceDirect::createPreSelectRelResCalc()
{
	rdoRuntime::RDOSelectResourceDirectCalc* calc = new rdoRuntime::RDOSelectResourceDirectCalc( parser()->runtime(), rel_res_id, res->getID(), NULL, NULL );
	calc->setSrcInfo( src_info() );
	calc->setSrcText( "��������������� ����� ���. ������� " + calc->src_text() );
	return calc;
}

rdoRuntime::RDOCalc* RDORelevantResourceDirect::createSelectResourceChoiceCalc()
{
	rdoRuntime::RDOSelectResourceDirectCalc* calc = new rdoRuntime::RDOSelectResourceDirectCalc( parser()->runtime(), rel_res_id, res->getID(), getChoiceCalc(), getSelectCalc(), getSelectType() );
	calc->setSrcInfo( choice_from->src_info() );
	return calc;
}

rdoRuntime::RDOCalc* RDORelevantResourceDirect::createSelectFirstResourceChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCalc( parser()->runtime(), rel_res_id, res->getID(), getChoiceCalc() );
}

rdoRuntime::RDOSelectResourceCommon* RDORelevantResourceDirect::createSelectResourceCommonChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceDirectCommonCalc( parser()->runtime(), rel_res_id, res->getID(), getChoiceCalc() );
}

const RDORTPResType* const RDORelevantResourceDirect::getType() const 
{ 
	return res->getType(); 
}

// ----------------------------------------------------------------------------
// ---------- RDORelevantResourceByType - �� ����� ����
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalc* RDORelevantResourceByType::createPreSelectRelResCalc()
{
	if ( (begin != rdoRuntime::RDOResource::CS_Create) && (end != rdoRuntime::RDOResource::CS_Create) ) {
		rdoRuntime::RDOSelectResourceByTypeCalc* calc = new rdoRuntime::RDOSelectResourceByTypeCalc( parser()->runtime(), rel_res_id, type->getNumber(), NULL, NULL );
		calc->setSrcInfo( src_info() );
		calc->setSrcText( "��������������� ����� ���. ������� " + calc->src_text() );
		return calc;
	} else {
		if ( begin == rdoRuntime::RDOResource::CS_NonExist ) {
			return new rdoRuntime::RDOSelectResourceNonExistCalc( parser()->runtime(), rel_res_id );
		} else {
			rdoRuntime::RDOCalcConst* calc = new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );
			calc->setSrcInfo( src_info() );
			calc->setSrcText( "��������������� ����� ���. ������� ����� ��������� " + calc->src_text() );
			return calc;
		}
	}
}

rdoRuntime::RDOCalc* RDORelevantResourceByType::createSelectResourceChoiceCalc()
{
	if ( (begin != rdoRuntime::RDOResource::CS_Create) && (end != rdoRuntime::RDOResource::CS_Create) ) {
		rdoRuntime::RDOSelectResourceByTypeCalc* calc = new rdoRuntime::RDOSelectResourceByTypeCalc( parser()->runtime(), rel_res_id, type->getNumber(), getChoiceCalc(), getSelectCalc(), getSelectType() );
		calc->setSrcInfo( choice_from->src_info() );
		return calc;
	} else {
		if ( begin == rdoRuntime::RDOResource::CS_NonExist ) {
			return new rdoRuntime::RDOSelectResourceNonExistCalc( parser()->runtime(), rel_res_id );
		} else {
			rdoRuntime::RDOCalcConst* calc = new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );
			calc->setSrcInfo( src_info() );
			calc->setSrcText( "����� ��������� ���. ������� " + calc->src_text() );
			return calc;
		}
	}
}

rdoRuntime::RDOCalc* RDORelevantResourceByType::createSelectFirstResourceChoiceCalc()
{
	if ( (begin != rdoRuntime::RDOResource::CS_Create) && (end != rdoRuntime::RDOResource::CS_Create) ) {
		return new rdoRuntime::RDOSelectResourceByTypeCalc( parser()->runtime(), rel_res_id, type->getNumber(), getChoiceCalc() );
	} else {
		if ( begin == rdoRuntime::RDOResource::CS_NonExist ) {
			return new rdoRuntime::RDOSelectResourceNonExistCalc( parser()->runtime(), rel_res_id );
		} else {
			return new rdoRuntime::RDOCalcConst( parser()->runtime(), 1 );
		}
	}
}

rdoRuntime::RDOSelectResourceCommon* RDORelevantResourceByType::createSelectResourceCommonChoiceCalc()
{
	return new rdoRuntime::RDOSelectResourceByTypeCommonCalc( parser()->runtime(), rel_res_id, type->getNumber(), getChoiceCalc() );
}

// ----------------------------------------------------------------------------
// ---------- RDOPATParamSet - ��� ��������� set ��� ������ ���. �������
// ----------------------------------------------------------------------------
void RDOPATParamSet::addSet(CREF(std::string) paramName, CREF(YYLTYPE) param_name_pos, rdoRuntime::EqualType equalType, PTR(RDOFUNArithm) rightArithm)
{
	switch (equalType)
	{
	case rdoRuntime::ET_NOCHANGE:
	case rdoRuntime::ET_EQUAL   :
	case rdoRuntime::ET_PLUS    :
	case rdoRuntime::ET_MINUS   :
	case rdoRuntime::ET_MULTIPLY:
	case rdoRuntime::ET_DIVIDE  : break;
	default                     : parser()->error().error(param_name_pos, _T("����������� �������� ������������"));
	}

	if ( m_params.empty() ) {
		setSrcText( paramName + (rightArithm ? " set " + rightArithm->src_text() : " NoChange") );
		setSrcPos( param_name_pos );
	} else {
		setSrcText( src_text() + "\n" + paramName + (rightArithm ? " set " + rightArithm->src_text() : " NoChange") );
	}
	const RDORTPParam* param = getRelRes()->getType()->findRTPParam( paramName );
	if ( !param ) {
		parser()->error().error( param_name_pos, rdo::format("����������� ��������: %s", paramName.c_str()) );
	}
	if ( isExist(paramName) ) {
		parser()->error().error( RDOParserSrcInfo(param_name_pos), rdo::format("�������� '%s' ��� ������������", paramName.c_str()) );
//		parser()->error().warning( RDOParserSrcInfo(param_name_pos), rdo::format("�������� '%s' ��� ���������� � ����������. � ����������� ������� ��������� �������� ���������", paramName.c_str()) );
	}
	if ( rightArithm ) {
		param->getType()->checkParamType( rightArithm );
	}
	m_params.push_back(Param(paramName, getRelRes()->getType()->getRTPParamNumber(paramName), equalType, rightArithm));
}

} // namespace rdoParse
