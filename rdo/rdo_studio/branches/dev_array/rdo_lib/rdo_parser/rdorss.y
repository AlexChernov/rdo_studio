/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdorss.y
 * author   : �������� ����, ������ ������
 * date     : 20.02.2003
 * bref     : ��������� ��������
 * indent   : 4T
 */

%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token RDO_Resource_type
%token RDO_permanent
%token RDO_Parameters
%token RDO_integer
%token RDO_real
%token RDO_End
%token RDO_temporary
%token RDO_IDENTIF
%token RDO_INT_CONST
%token RDO_REAL_CONST
%token RDO_such_as
%token RDO_dblpoint
%token RDO_Resources
%token RDO_trace
%token RDO_no_trace
%token RDO_IDENTIF_COLON
%token RDO_Constant
%token RDO_Body
%token RDO_Function
%token RDO_Type
%token RDO_algorithmic
%token RDO_table
%token RDO_list
%token RDO_Exist
%token RDO_Not_Exist
%token RDO_For_All
%token RDO_Not_For_All
%token RDO_neq
%token RDO_leq
%token RDO_geq
%token RDO_NoCheck
%token RDO_Calculate_if
%token RDO_or
%token RDO_and
%token RDO_Sequence
%token RDO_uniform
%token RDO_exponential
%token RDO_normal
%token RDO_by_hist
%token RDO_enumerative

%token RDO_Pattern
%token RDO_operation
%token RDO_rule
%token RDO_keyboard
%token RDO_Relevant_resources
%token RDO_Keep
%token RDO_Create
%token RDO_Erase
%token RDO_NonExist
%token RDO_IDENTIF_NoChange
%token RDO_Time
%token RDO_Choice
%token RDO_from
%token RDO_first
%token RDO_Convert_begin
%token RDO_Convert_end
%token RDO_Convert_rule
%token RDO_Convert_event
%token RDO_with_max
%token RDO_with_min
%token RDO_set
%token RDO_IDENTIF_NoChange_NoChange
%token RDO_Operations

%token RDO_Results
%token RDO_watch_par
%token RDO_watch_state
%token RDO_watch_quant
%token RDO_watch_value
%token RDO_get_value

%token RDO_Show_mode
%token RDO_Frame_number
%token RDO_Show_rate
%token RDO_Run_StartTime
%token RDO_Trace_StartTime
%token RDO_Trace_EndTime
%token RDO_Terminate_if
%token RDO_Break_point
%token RDO_Seed
%token RDO_NoShow
%token RDO_Monitor
%token RDO_Animation
%token RDO_NoChange

%token RDO_Decision_point
%token RDO_search
%token RDO_trace_stat
%token RDO_trace_tops
%token RDO_trace_all
%token RDO_Condition
%token RDO_Term_condition
%token RDO_Evaluate_by
%token RDO_Compare_tops
%token RDO_NO
%token RDO_YES
%token RDO_Activities
%token RDO_value_before
%token RDO_value_after
%token RDO_some
%token RDO_Process
%token RDO_SEIZE
%token RDO_GENERATE
%token RDO_TERMINATE
%token RDO_ADVANCE
%token RDO_RELEASE
%token RDO_if
%token RDO_for
%token RDO_Return
%token RDO_Break
%token RDO_result
%token RDO_CF
%token RDO_Priority
%token RDO_prior
%token RDO_Parent
%token RDO_PlusEqual
%token RDO_MinusEqual
%token RDO_MultiplyEqual
%token RDO_DivideEqual
%token RDO_array
%token RDO_event
%token RDO_Planning
%token RDO_else
%token RDO_IncrEqual
%token RDO_DecrEqual
%token RDO_Stopping
%token RDO_Start
%token RDO_Stop
%token RDO_WatchStart
%token RDO_WatchStop

%token RDO_Frame
%token RDO_Show_if
%token RDO_Back_picture
%token RDO_Show
%token RDO_frm_cell
%token RDO_text
%token RDO_bitmap
%token RDO_s_bmp
%token RDO_rect
%token RDO_r_rect
%token RDO_line
%token RDO_ellipse
%token RDO_triang
%token RDO_active
%token RDO_ruler
%token RDO_space
%token RDO_color_transparent
%token RDO_color_last
%token RDO_color_white
%token RDO_color_black
%token RDO_color_red
%token RDO_color_green
%token RDO_color_blue
%token RDO_color_cyan
%token RDO_color_magenta
%token RDO_color_yellow
%token RDO_color_gray

%token RDO_IDENTIF_RELRES
%token RDO_typedef
%token RDO_enum

%token RDO_STRING_CONST
%token RDO_STRING_CONST_BAD
%token RDO_IDENTIF_BAD
%token RDO_Select
%token RDO_Size
%token RDO_Empty
%token RDO_not
%token RDO_UMINUS
%token RDO_string
%token RDO_bool
%token RDO_BOOL_CONST
%token RDO_Fuzzy
%token RDO_Fuzzy_Term
%token RDO_eq
%token RDO_External_Model
%token RDO_QUEUE
%token RDO_DEPART
%token RDO_ASSIGN

%{
// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdorss.h"
#include "rdo_lib/rdo_parser/rdortp.h"
// ===============================================================================

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

#define P_RDOVALUE(A) reinterpret_cast<PTR(RDOValue)>(A)
#define RDOVALUE(A)   (*P_RDOVALUE(A))

OPEN_RDO_PARSER_NAMESPACE
%}

%left RDO_IDENTIF
%left ':'

%start rss_main

%%

rss_main
	: /* empty */
	| rss_resources_begin rss_resources rss_resources_end
	| rss_resources_begin rss_resources
	{
		PARSER->error().error(@2, _T("����� �������� ���� �������� ��������� �������� ����� $End"));
	}
	| error
	{
		if (!PARSER->isHaveKWResources())
		{
			PARSER->error().error(@1, _T("��������� �������� ����� $Resources"));
		}
		else
		{
			if (PARSER->isHaveKWResourcesEnd())
			{
				PARSER->error().error(@1, _T("������� ��� ����������"));
			}
			else
			{
				PARSER->error().error(@1, rdoSimulator::RDOSyntaxError::UNKNOWN);
			}
		}
	}
	;

rss_resources_begin
	: RDO_Resources
	{
		PARSER->setHaveKWResources(true);
	}
	;

rss_resources_end
	: RDO_End
	{
		PARSER->setHaveKWResourcesEnd(true);
	}
	;

rss_resources
	: /* empty */
	| rss_resources rss_res_descr
	;

rss_res_descr
	: rss_res_type rss_trace rss_values
	{
		LPRDORSSResource pResource = PARSER->stack().pop<RDORSSResource>($1);
		ASSERT(pResource);
		if (!pResource->defined())
		{
			PARSER->error().error(@3, rdo::format(_T("������ �� ��� ��������� �������: %s"), pResource->name().c_str()));
		}
		pResource->setTrace($2 != 0);
	}
	;

rss_res_type
	: RDO_IDENTIF_COLON RDO_IDENTIF
	{
		PTR(RDOValue) name = P_RDOVALUE($1);
		PTR(RDOValue) type = P_RDOVALUE($2);
		LPRDORTPResType pResType = PARSER->findRTPResType(type->value().getIdentificator());
		if (!pResType)
		{
			PARSER->error().error(@2, rdo::format(_T("����������� ��� �������: %s"), type->value().getIdentificator().c_str()));
		}
		LPRDORSSResource pResourceExist = PARSER->findRSSResource(name->value().getIdentificator());
		if (pResourceExist)
		{
			PARSER->error().push_only(name->src_info(), rdo::format(_T("������ '%s' ��� ����������"), name->value().getIdentificator().c_str()));
			PARSER->error().push_only(pResourceExist->src_info(), _T("��. ������ �����������"));
			PARSER->error().push_done();
		}
		LPRDORSSResource pResource = rdo::Factory<RDORSSResource>::create(PARSER, name->src_info(), pResType);
		$$ = PARSER->stack().push(pResource);
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, _T("��������� ��� �������"));
	}
	| ':'
	{
		PARSER->error().error(@1, _T("����� ���������� ��������� ��� �������"));
	}
	| error
	{
		PARSER->error().error(@1, _T("��������� ��� �������"));
	}
	;

rss_trace
	: /* empty */  {$$ = 0;}
	| RDO_trace	   {$$ = 1;}
	| RDO_no_trace {$$ = 0;}
	;

rss_values
	: /* empty */
	| rss_values rss_value
	;

rss_value
	: '*'               {PARSER->getLastRSSResource()->addParam(RDOValue(RDOParserSrcInfo(@1, _T("*"))));}
	| RDO_INT_CONST     {PARSER->getLastRSSResource()->addParam(RDOVALUE($1));}
	| RDO_REAL_CONST    {PARSER->getLastRSSResource()->addParam(RDOVALUE($1));}
	| RDO_BOOL_CONST    {PARSER->getLastRSSResource()->addParam(RDOVALUE($1));}
	| RDO_STRING_CONST  {PARSER->getLastRSSResource()->addParam(RDOVALUE($1));}
	| RDO_IDENTIF       {PARSER->getLastRSSResource()->addParam(RDOVALUE($1));}
	| param_array_value {PARSER->getLastRSSResource()->addParam(RDOVALUE($1));}
	| error
	{
		PARSER->error().error(@1, rdo::format(_T("������������ �������� ���������: %s"), LEXER->YYText()));
	}
	;

param_value
	: RDO_INT_CONST
	{
		$$ = $1;
	}
	| RDO_REAL_CONST
	{
		$$ = $1;
	}
	| RDO_STRING_CONST
	{
		$$ = $1;
	}
	| RDO_IDENTIF
	{
		$$ = $1;
	}
	| RDO_BOOL_CONST
	{
		$$ = $1;
	}
	| param_array_value
	{
		$$ = $1;
	}
	;

param_array_value
	: '[' array_enumeration ']'
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($2);
		ASSERT(pArrayValue);
		RDOParserSrcInfo srcInfo(@1, @3, pArrayValue->getAsString());
		pArrayValue->setSrcInfo(srcInfo);
		pArrayValue->getArrayType()->setSrcInfo(srcInfo);
		$$ = (int)PARSER->addValue(new RDOValue(pArrayValue));
	}
	| '[' array_enumeration error
	{
		PARSER->error().error(@2, _T("������ ������ ����������� �������"));
	}
	;

array_enumeration
	: param_value
	{
		LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(RDOVALUE($1).type(), RDOVALUE($1).src_info());
		ASSERT(pArrayType);
		LPRDOArrayValue pArrayValue = rdo::Factory<RDOArrayValue>::create(pArrayType);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($1));
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_enumeration ',' param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($3));
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_enumeration param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($2));
		$$ = PARSER->stack().push(pArrayValue);
		PARSER->error().warning(@1, rdo::format(_T("��������� ������� �����: %s"), RDOVALUE($2)->getAsString().c_str()));
	}
	;

// ----------------------------------------------------------------------------

%%

CLOSE_RDO_PARSER_NAMESPACE
