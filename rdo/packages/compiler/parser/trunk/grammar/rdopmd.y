/*!
  \copyright (c) RDO-Team, 2011
  \file      rdopmd.y
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     ��������� ���������� �����������
  \indent    4T
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
%token RDO_triangular
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

%token RDO_ProcessStart
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
%token RDO_circle
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
// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdopmd.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/runtime/rdocalc.h"
#include "simulator/runtime/calc/array.h"
// --------------------------------------------------------------------------------

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

OPEN_RDO_PARSER_NAMESPACE
%}

%left RDO_or
%left RDO_and
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%%

pmd_main
	: /* empty */
	| pmd_main pmd_result_group
	;

pmd_result_group_name
	: /* empty */
	{
		LPRDOResultGroup pResultGroup = PARSER->findResultGroup(_T(""));
		if (!pResultGroup)
		{
			pResultGroup = rdo::Factory<RDOResultGroup>::create();
			ASSERT(pResultGroup);
			pResultGroup->init(RDOParserSrcInfo());
		}
		PARSER->contextStack()->push(pResultGroup);
	}
	| RDO_IDENTIF
	{
		LPRDOResultGroup pResultGroup = rdo::Factory<RDOResultGroup>::create();
		ASSERT(pResultGroup);
		pResultGroup->init(RDOParserSrcInfo(@1, PARSER->stack().pop<RDOValue>($1)->value().getIdentificator()));
		PARSER->contextStack()->push(pResultGroup);
	}
	;

pmd_result_group
	: RDO_Results pmd_result_group_name pmd_body RDO_End
	{
		PARSER->contextStack()->pop();
	}
	| RDO_Results pmd_result_group_name pmd_body error
	{
		PARSER->contextStack()->pop();
		PARSER->error().error(@3, _T("��������� �������� ����� $End"));
	}
	| error
	{
		YYLTYPE pos( @1 );
		pos.m_last_line = pos.m_first_line;
		pos.m_last_pos  = pos.m_first_pos;
		pos.m_last_seek = pos.m_first_seek;
		PARSER->error().error(pos, _T("��������� �������� ����� $Results"));
	}
	;

pmd_body
	: /* empty */
	| pmd_body pmd_pokaz
	{
		LPRDOPMDPokaz pResult = PARSER->stack().pop<RDOPMDPokaz>($2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	;

pmd_trace
	: /* empty */
	{
		$$ = 0;
	}
	| RDO_trace
	{
		$$ = 1;
	}
	| RDO_no_trace
	{
		$$ = 0;
	}
	;

pmd_pokaz_watch_quant_begin
	: RDO_IDENTIF_COLON pmd_trace RDO_watch_quant RDO_IDENTIF
	{
		LPRDOValue pName = PARSER->stack().pop<RDOValue>($1);
		LPRDOValue pType = PARSER->stack().pop<RDOValue>($4);
		ASSERT(pName);
		ASSERT(pType);

		LPRDOPMDWatchQuant pWatchQuant = rdo::Factory<RDOPMDWatchQuant>::create(pName->src_info(), pType->src_info());
		ASSERT(pWatchQuant);
		pWatchQuant->init($2 != 0, pType->src_info());
		$$ = PARSER->stack().push(pWatchQuant);
	}
	;

pmd_pokaz_watch_value_begin
	: RDO_IDENTIF_COLON pmd_trace RDO_watch_value RDO_IDENTIF
	{
		LPRDOValue pName = PARSER->stack().pop<RDOValue>($1);
		LPRDOValue pType = PARSER->stack().pop<RDOValue>($4);
		ASSERT(pName);
		ASSERT(pType);

		LPRDOPMDWatchValue pWatchValue = rdo::Factory<RDOPMDWatchValue>::create(pName->src_info(), pType->src_info());
		ASSERT(pWatchValue);
		pWatchValue->init($2 != 0, pType->src_info());
		$$ = PARSER->stack().push(pWatchValue);
	}
	;

pmd_pokaz
	: RDO_IDENTIF_COLON pmd_trace RDO_watch_par RDO_IDENTIF '.' RDO_IDENTIF
	{
		LPRDOPMDWatchPar pPokaz = rdo::Factory<RDOPMDWatchPar>::create(PARSER->stack().pop<RDOValue>($1)->src_info());
		ASSERT(pPokaz);
		pPokaz->init($2 != 0, PARSER->stack().pop<RDOValue>($4)->src_info(), PARSER->stack().pop<RDOValue>($6)->src_info());
		$$ = PARSER->stack().push(pPokaz);
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_par RDO_IDENTIF '.' error
	{
		PARSER->error().error(@5, @6, _T("��������� ��� ���������"));
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_par RDO_IDENTIF error
	{
		PARSER->error().error(@4, _T("��������� '.'"));
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_par error
	{
		PARSER->error().error(@3, @4, _T("����� ��������� ����� watch_par ��������� ��� �������"));
	}
	| RDO_IDENTIF_COLON pmd_trace error
	{
		PARSER->error().error(@2, @3, _T("��������� ��� ����������"));
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@1, @2, _T("��������� ������� ����������� ��� ��� ����������"));
	}
	| RDO_IDENTIF error
	{
		PARSER->error().error(@1, @2, _T("��������� ':'"));
	}
	| error
	{
		PARSER->error().error(@1, _T("��������� ��� ����������"));
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_state fun_logic
	{
		LPRDOPMDWatchState pPokaz = rdo::Factory<RDOPMDWatchState>::create(PARSER->stack().pop<RDOValue>($1)->src_info());
		ASSERT(pPokaz);
		pPokaz->init($2 != 0, PARSER->stack().pop<RDOFUNLogic>($4));
		$$ = PARSER->stack().push(pPokaz);
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_state error
	{
		PARSER->error().error(@3, @4, _T("����� ��������� ����� watch_state ��������� ���������� ���������"));
	}
	| pmd_pokaz_watch_quant_begin fun_logic
	{
		LPRDOPMDWatchQuant pWatchQuant = PARSER->stack().pop<RDOPMDWatchQuant>($1);
		ASSERT(pWatchQuant);
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pWatchQuant->setLogic(pLogic);
		$$ = PARSER->stack().push(pWatchQuant);
	}
	| pmd_pokaz_watch_quant_begin RDO_NoCheck
	{
		LPRDOPMDWatchQuant pWatchQuant = PARSER->stack().pop<RDOPMDWatchQuant>($1);
		ASSERT(pWatchQuant);
		pWatchQuant->setLogicNoCheck();
		$$ = PARSER->stack().push(pWatchQuant);
	}
	| pmd_pokaz_watch_quant_begin error
	{
		PARSER->error().error(@1, @2, _T("����� ����� ���� ��������� ���������� ���������"));
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_quant error
	{
		PARSER->error().error(@3, @4, _T("����� ��������� ����� watch_quant ��������� ��� �������"));
	}
	| pmd_pokaz_watch_value_begin fun_logic fun_arithm
	{
		LPRDOPMDWatchValue pWatchValue = PARSER->stack().pop<RDOPMDWatchValue>($1);
		ASSERT(pWatchValue);
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);
		pWatchValue->setLogic(pLogic, pArithm);
		$$ = PARSER->stack().push(pWatchValue);
	}
	| pmd_pokaz_watch_value_begin RDO_NoCheck fun_arithm
	{
		LPRDOPMDWatchValue pWatchValue = PARSER->stack().pop<RDOPMDWatchValue>($1);
		ASSERT(pWatchValue);
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);
		pWatchValue->setLogicNoCheck(pArithm);
		$$ = PARSER->stack().push(pWatchValue);
	}
	| pmd_pokaz_watch_value_begin fun_logic error
	{
		PARSER->error().error(@2, @3, _T("����� ����������� ��������� �������������� ���������"));
	}
	| pmd_pokaz_watch_value_begin RDO_NoCheck error
	{
		PARSER->error().error(@2, @3, _T("����� ����������� ��������� �������������� ���������"));
	}
	| pmd_pokaz_watch_value_begin error
	{
		PARSER->error().error(@1, @2, _T("����� ����� ���� ��������� ���������� ���������"));
	}
	| RDO_IDENTIF_COLON pmd_trace RDO_watch_value error
	{
		PARSER->error().error(@3, @4, _T("����� ��������� ����� watch_value ��������� ��� �������"));
	}
	| RDO_IDENTIF_COLON RDO_get_value fun_arithm
	{
		LPRDOPMDGetValue pPokaz = rdo::Factory<RDOPMDGetValue>::create(PARSER->stack().pop<RDOValue>($1)->src_info());
		ASSERT(pPokaz);
		pPokaz->init(PARSER->stack().pop<RDOFUNArithm>($3));
		$$ = PARSER->stack().push(pPokaz);
	}
	| RDO_IDENTIF_COLON RDO_get_value error
	{
		PARSER->error().error(@2, @3, _T("����� ��������� ����� get_value ��������� �������������� ���������"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- ����� ��������� ������ ��� ���� �������� ���
// --------------------------------------------------------------------------------
// -------------------- �������� ����������
// --------------------------------------------------------------------------------
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
	: '[' array_item ']'
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($2);
		ASSERT(pArrayValue);
		RDOParserSrcInfo srcInfo(@1, @3, pArrayValue->getAsString());
		pArrayValue->setSrcInfo(srcInfo);
		pArrayValue->getArrayType()->setSrcInfo(srcInfo);
		$$ = PARSER->stack().push(rdo::Factory<RDOValue>::create(pArrayValue->getArrayType()->typeInfo(), pArrayValue));
	}
	| '[' array_item error
	{
		PARSER->error().error(@2, _T("������ ������ ����������� �������"));
	}
	;

array_item
	: param_value
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pValue);
		LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(pValue->typeInfo(), RDOParserSrcInfo(@1));
		ASSERT(pArrayType);
		LPRDOArrayValue pArrayValue = rdo::Factory<RDOArrayValue>::create(pArrayType);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(pValue);
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item ',' param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		pArrayValue->insertItem(pValue);
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($2);
		ASSERT(pValue);
		pArrayValue->insertItem(pValue);
		$$ = PARSER->stack().push(pArrayValue);
		PARSER->error().warning(@1, rdo::format(_T("��������� ������� �����: %s"), pValue->value().getAsString().c_str()));
	}
	;

param_value_default
	: /* empty */
	{
		LPRDOValue pValue = rdo::Factory<RDOValue>::create();
		ASSERT(pValue);
		$$ = PARSER->stack().push(pValue);
	}
	| '=' param_value
	{
		$$ = $2;
	}
	| '=' error
	{
		RDOParserSrcInfo src_info(@1, @2, true);
		if (src_info.src_pos().point())
		{
			PARSER->error().error(src_info, _T("�� ������� �������� ��-���������"));
		}
		else
		{
			PARSER->error().error(src_info, _T("�������� �������� ��-���������"));
		}
	}
	;

// --------------------------------------------------------------------------------
// -------------------- ���������� ���������
// --------------------------------------------------------------------------------
fun_logic_eq
	: RDO_eq { $$ = RDO_eq; }
	;

fun_logic
	: fun_arithm fun_logic_eq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator ==(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm RDO_neq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator !=(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '<' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator <(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '>' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator >(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm RDO_leq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator <=(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm RDO_geq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator >=(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_logic RDO_and fun_logic
	{
		LPRDOFUNLogic pLogic1 = PARSER->stack().pop<RDOFUNLogic>($1);
		LPRDOFUNLogic pLogic2 = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pLogic1);
		ASSERT(pLogic2);
		LPRDOFUNLogic pResult = pLogic1->operator &&(pLogic2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_logic RDO_or fun_logic
	{
		LPRDOFUNLogic pLogic1 = PARSER->stack().pop<RDOFUNLogic>($1);
		LPRDOFUNLogic pLogic2 = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pLogic1);
		ASSERT(pLogic2);
		LPRDOFUNLogic pResult = pLogic1->operator ||(pLogic2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pArithm);
		LPRDOFUNLogic pResult = rdo::Factory<RDOFUNLogic>::create(pArithm);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_group
	| fun_select_logic
	| '[' fun_logic ']'
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText(_T("[") + pLogic->src_text() + _T("]"));
		$$ = PARSER->stack().push(pLogic);
	}
	| '(' fun_logic ')'
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText(_T("(") + pLogic->src_text() + _T(")"));
		$$ = PARSER->stack().push(pLogic);
	}
	| RDO_not fun_logic
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		RDOParserSrcInfo src_info(@1, @2);
		LPRDOFUNLogic pLogicNot = pLogic->operator_not(src_info.src_pos());
		ASSERT(pLogicNot);
		$$ = PARSER->stack().push(pLogicNot);
	}
	| '[' fun_logic error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| '(' fun_logic error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- �������������� ���������
// --------------------------------------------------------------------------------
fun_arithm
	: RDO_INT_CONST                      { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_REAL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_BOOL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_STRING_CONST                   { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| param_array_value                  { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_IDENTIF                        { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1), PARSER->stack().pop<RDOValue>($3))); }
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1), PARSER->stack().pop<RDOValue>($3))); }
	| '*' 
	{
		LPRDOValue pValue = rdo::Factory<RDOValue>::create(RDOParserSrcInfo(@1, _T("*")));
		ASSERT(pValue);
		LPExpression pExpression = rdo::Factory<Expression>::create(pValue);
		ASSERT(pExpression);
		LPRDOFUNArithm pArithm = rdo::Factory<RDOFUNArithm>::create(pExpression);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| fun_arithm '+' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator +(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '-' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator -(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '*' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator *(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm '/' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = PARSER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator /(pArithm2);
		ASSERT(pResult);
		$$ = PARSER->stack().push(pResult);
	}
	| fun_arithm_func_call
	| fun_select_arithm
	| '(' fun_arithm ')'
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		pArithm->setSrcPos (@1, @3);
		pArithm->setSrcText(_T("(") + pArithm->src_text() + _T(")"));
		$$ = PARSER->stack().push(pArithm);
	}
	| '-' fun_arithm %prec RDO_UMINUS
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		RDOParserSrcInfo info;
		info.setSrcPos(@1, @2);
		$$ = PARSER->stack().push(pArithm->uminus(info.src_pos()));
	}
	| RDO_IDENTIF '.' RDO_Size
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pValue);

		LPRDOFUNArithm pArithm = RDOFUNArithm::generateByIdentificator(pValue);
		ASSERT(pArithm);

		rdoRuntime::LPRDOCalc pCalc;
		if (pArithm->typeInfo()->type().object_dynamic_cast<RDOArrayType>())
		{
			pCalc = rdo::Factory<rdoRuntime::RDOCalcArraySize>::create(pArithm->calc());
			ASSERT(pCalc);
		}
		else
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' �� �������� ��������."), pValue->value().getIdentificator().c_str()));
		}

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__int>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		LPRDOFUNArithm pArithmArraySize = rdo::Factory<RDOFUNArithm>::create(pExpression);
		ASSERT(pArithmArraySize);

		$$ = PARSER->stack().push(pArithmArraySize);
	}
	| RDO_IDENTIF '[' fun_arithm ']'
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pValue);

		LPRDOFUNArithm pArithm = RDOFUNArithm::generateByIdentificator(pValue);
		ASSERT(pArithm);

		LPRDOFUNArithm pArithmInd = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithmInd);

		if (pArithm->typeInfo()->type().object_dynamic_cast<RDOArrayType>())
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' �� �������� ��������."), pValue->value().getIdentificator().c_str()));
		}

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcArrayItem>::create(pArithm->calc(), pArithmInd->calc());
		ASSERT(pCalc);

		LPRDOType pType = pArithm->typeInfo()->type();
		ASSERT(pType);
		LPRDOArrayType pArrayType = pType.object_dynamic_cast<RDOArrayType>();
		ASSERT(pArrayType);

		LPTypeInfo pItemType = pArrayType->getItemType();
		ASSERT(pItemType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pItemType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		LPRDOFUNArithm pArithmArrayItem = rdo::Factory<RDOFUNArithm>::create(pExpression);
		ASSERT(pArithmArrayItem);

		$$ = PARSER->stack().push(pArithmArrayItem);
	}
	;

// --------------------------------------------------------------------------------
// -------------------- ������� � ������������������
// --------------------------------------------------------------------------------
fun_arithm_func_call
	: RDO_IDENTIF '(' arithm_list ')'
	{
		tstring funName                    = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		LPArithmContainer pArithmContainer = PARSER->stack().pop<ArithmContainer>($3);
		ASSERT(pArithmContainer);

		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create(pArithmContainer);
		ASSERT(pFunParams);

		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @4);
		pFunParams->setSrcText(funName + _T("(") + pArithmContainer->src_text() + _T(")"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' error
	{
		PARSER->error().error(@3, _T("������ � ���������� �������"));
	}
	;

arithm_list
	: /* empty */
	{
		LPArithmContainer pArithmContainer = rdo::Factory<ArithmContainer>::create();
		ASSERT(pArithmContainer);
		$$ = PARSER->stack().push(pArithmContainer);
	}
	| arithm_list_body
	;

arithm_list_body
	: fun_arithm
	{
		LPArithmContainer pArithmContainer = rdo::Factory<ArithmContainer>::create();
		LPRDOFUNArithm    pArithm          = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT (pArithmContainer);
		ASSERT (pArithm);
		pArithmContainer->setSrcText(pArithm->src_text());
		pArithmContainer->addItem   (pArithm);
		$$ = PARSER->stack().push(pArithmContainer);
	}
	| arithm_list_body ',' fun_arithm
	{
		LPArithmContainer pArithmContainer = PARSER->stack().pop<ArithmContainer>($1);
		LPRDOFUNArithm    pArithm          = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT (pArithmContainer);
		ASSERT (pArithm);
		pArithmContainer->setSrcText(pArithmContainer->src_text() + _T(", ") + pArithm->src_text());
		pArithmContainer->addItem   (pArithm);
		$$ = PARSER->stack().push(pArithmContainer);
	}
	| arithm_list_body ',' error
	{
		PARSER->error().error(@3, _T("������ � �������������� ���������"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- ��������� ���������
// --------------------------------------------------------------------------------
fun_group_keyword
	: RDO_Exist       { $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist   { $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All     { $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All { $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_group_header
	: fun_group_keyword '(' RDO_IDENTIF_COLON
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		$$ = PARSER->stack().push(rdo::Factory<RDOFUNGroupLogic>::create((RDOFUNGroupLogic::FunGroupType)$1, pValue->src_info()));
	}
	| fun_group_keyword '(' error
	{
		PARSER->error().error(@3, _T("��������� ��� ����"));
	}
	| fun_group_keyword error
	{
		PARSER->error().error(@1, _T("����� ����� ������� ��������� ������������� ������"));
	}
	;

fun_group
	: fun_group_header fun_logic ')'
	{
		LPRDOFUNGroupLogic pGroupFun = PARSER->stack().pop<RDOFUNGroupLogic>($1);
		LPRDOFUNLogic      pLogic    = PARSER->stack().pop<RDOFUNLogic>     ($2);
		ASSERT(pGroupFun);
		ASSERT(pLogic   );
		pGroupFun->setSrcPos(@1, @3);
		$$ = PARSER->stack().push(pGroupFun->createFunLogic(pLogic));
	}
	| fun_group_header RDO_NoCheck ')'
	{
		LPRDOFUNGroupLogic pGroupFun = PARSER->stack().pop<RDOFUNGroupLogic>($1);
		ASSERT(pGroupFun);
		pGroupFun->setSrcPos(@1, @3);
		LPRDOFUNLogic pTrueLogic = RDOFUNLogic::generateTrue(RDOParserSrcInfo(@2, _T("NoCheck")));
		ASSERT(pTrueLogic);
		$$ = PARSER->stack().push(pGroupFun->createFunLogic(pTrueLogic));
	}
	| fun_group_header fun_logic error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| fun_group_header RDO_NoCheck error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| fun_group_header error
	{
		PARSER->error().error(@1, @2, _T("������ � ���������� ���������"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Select
// --------------------------------------------------------------------------------
fun_select_header
	: RDO_Select '(' RDO_IDENTIF_COLON
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($3);
		ASSERT(pValue);
		LPRDOFUNSelect pSelect   = rdo::Factory<RDOFUNSelect>::create(pValue->src_info());
		ASSERT(pSelect);
		pSelect->setSrcText(_T("Select(") + pValue->value().getIdentificator() + _T(": "));
		$$ = PARSER->stack().push(pSelect);
	}
	| RDO_Select '(' error
	{
		PARSER->error().error(@3, _T("��������� ��� ����"));
	}
	| RDO_Select error
	{
		PARSER->error().error(@1, _T("��������� ������������� ������"));
	}
	;

fun_select_body
	: fun_select_header fun_logic ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = PARSER->stack().pop<RDOFUNLogic> ($2);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcText(pSelect->src_text() + pLogic->src_text() + _T(")"));
		pSelect->initSelect(pLogic);
		$$ = PARSER->stack().push(pSelect);
	}
	| fun_select_header RDO_NoCheck ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		RDOParserSrcInfo info(@2, _T("NoCheck"));
		pSelect->setSrcText(pSelect->src_text() + info.src_text() + _T(")"));
		LPRDOFUNLogic pTrueLogic = RDOFUNLogic::generateTrue(info);
		ASSERT(pTrueLogic);
		pSelect->initSelect(pTrueLogic);
		$$ = PARSER->stack().push(pSelect);
	}
	| fun_select_header fun_logic error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| fun_select_header RDO_NoCheck error
	{
		PARSER->error().error(@2, _T("��������� ������������� ������"));
	}
	| fun_select_header error
	{
		PARSER->error().error(@1, @2, _T("������ � ���������� ���������"));
	}
	;

fun_select_keyword
	: RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_select_logic
	: fun_select_body '.' fun_select_keyword '(' fun_logic ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = PARSER->stack().pop<RDOFUNLogic> ($5);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcPos(@1, @6);
		LPRDOFUNLogic pLogicSelect = pSelect->createFunSelectGroup((RDOFUNGroupLogic::FunGroupType)$3, pLogic);
		ASSERT(pLogicSelect);
		$$ = PARSER->stack().push(pLogicSelect);
	}
	| fun_select_body '.' fun_select_keyword '(' error
	{
		PARSER->error().error(@4, @5, _T("������ � ���������� ���������"));
	}
	| fun_select_body '.' fun_select_keyword error
	{
		PARSER->error().error(@3, _T("��������� ������������� ������"));
	}
	| fun_select_body '.' RDO_Empty '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo emptyInfo(@3, @5, _T("Empty()"));
		LPRDOFUNLogic pLogic = pSelect->createFunSelectEmpty(emptyInfo);
		ASSERT(pLogic);
		$$ = PARSER->stack().push(pLogic);
	}
	| fun_select_body '.' RDO_Empty '(' error
	{
		PARSER->error().error(@4, _T("��������� ������������� ������"));
	}
	| fun_select_body '.' RDO_Empty error
	{
		PARSER->error().error(@3, _T("��������� ������������� ������"));
	}
	| fun_select_body '.' error
	{
		PARSER->error().error(@2, @3, _T("��������� ����� ������ ��������"));
	}
	| fun_select_body error
	{
		PARSER->error().error(@1, _T("��������� '.' (�����) ��� ������ ������ ������ ��������"));
	}
	;

fun_select_arithm
	: fun_select_body '.' RDO_Size '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo sizeInfo(@3, @5, _T("Size()"));
		LPRDOFUNArithm pArithm = pSelect->createFunSelectSize(sizeInfo);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| fun_select_body '.' RDO_Size error
	{
		PARSER->error().error(@3, _T("��������� ������������� ������"));
	}
	| fun_select_body '.' RDO_Size '(' error
	{
		PARSER->error().error(@4, _T("��������� ������������� ������"));
	}
	;

%%

CLOSE_RDO_PARSER_NAMESPACE
