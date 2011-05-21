/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdofun.y
 * author   : �������� ����, ������ ������
 * date     : 20.02.2003
 * bref     : ��������� �������, �������� � �������������������
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
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/type/range.h"
#include "rdo_lib/rdo_parser/rdo_array.h"
#include "rdo_lib/rdo_parser/type/such_as.h"
#include "rdo_lib/rdo_parser/local_variable.h"
#include "rdo_lib/rdo_parser/context/type.h"
#include "rdo_lib/rdo_parser/context/memory.h"
#include "rdo_lib/rdo_parser/variable_container.h"
#include "rdo_lib/rdo_runtime/calc/locvar.h"
#include "rdo_lib/rdo_runtime/calc/braces.h"
#include "rdo_lib/rdo_runtime/calc/statements.h"
// ===============================================================================

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

#define P_RDOVALUE(A) reinterpret_cast<PTR(RDOValue)>(A)
#define RDOVALUE(A)   (*P_RDOVALUE(A))

OPEN_RDO_PARSER_NAMESPACE
%}

%left RDO_or
%left RDO_and	
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start fun_list

%%

fun_list
	: fun_consts fun_func_seq
	| error
	{
		if (PARSER->getFUNConstants().empty())
		{
			PARSER->error().error(@1, _T("��������� �������� �������, ������������������ ��� ���������"));
		}
		else
		{
			PARSER->error().error(@1, _T("��������� �������� ������� ��� ������������������"));
		}
	}
	;

// ----------------------------------------------------------------------------
// ---------- ���������
// ----------------------------------------------------------------------------
fun_consts
	: /* empty */
	| RDO_Constant fun_const_body RDO_End
	| RDO_Constant fun_const_body
	{
		PARSER->error().error(@2, _T("����� �������� �������� ��������� �������� ����� $End"));
	}
	;

fun_const_body
	: /* empty */
	| fun_const_body fun_const_param_desc
	| fun_const_body error
	{
		PARSER->error().error(@2, _T("��������� �������� ���������"));
	}
	;

fun_const_param_desc
	: RDO_IDENTIF_COLON param_type param_value_default
	{
		PTR(RDOValue) name = P_RDOVALUE($1);
		PARSER->checkFunctionName(name->src_info());
		LPTypeInfo pParamType = PARSER->stack().pop<TypeInfo>($2);
		ASSERT(pParamType);
		LPRDOFUNConstant pConstant = rdo::Factory<RDOFUNConstant>::create(name->src_info(), pParamType, RDOVALUE($3));
		ASSERT(pConstant);
		RUNTIME->setConstValue(pConstant->getNumber(), pConstant->getDefault().value());
		$$ = PARSER->stack().push(pConstant);
	}
	| RDO_IDENTIF_COLON
	{
		PARSER->error().error(@1, _T("��������� ��� ���������"));
	}
	| RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, _T("������ �������� ���� ���������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- ������������������ � �������
// ----------------------------------------------------------------------------
fun_func_seq
	: /* empty */
	| fun_func_seq fun_func_descr
	| fun_func_seq fun_seq_descr
	| fun_func_seq RDO_Constant
	{
		PARSER->error().error(@2, _T("��������� ����� ���� ������� �������, ����� ��������� � ��������������������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- �������
// ----------------------------------------------------------------------------
fun_func_descr
	: fun_func_header fun_func_footer
	{
		LPRDOFUNFunction pFunction = PARSER->stack().pop<RDOFUNFunction>($1);
		ASSERT(pFunction);
		pFunction->end();
	}
	| fun_func_header error
	{
		LPRDOFUNFunction pFunction = PARSER->stack().pop<RDOFUNFunction>($1);
		ASSERT(pFunction);
		PARSER->error().error(@2, rdo::format(_T("��������� �������� ����� $Type � ��������� ���� ������� '%s'"), pFunction->name().c_str()));
	}
	;

fun_func_header
	: RDO_Function RDO_IDENTIF_COLON param_type param_value_default
	{
		PTR(RDOValue) name = reinterpret_cast<PTR(RDOValue)>($2);
		PARSER->checkFunctionName(name->src_info());
		LPTypeInfo pRetType = PARSER->stack().pop<TypeInfo>($3);
		ASSERT(pRetType);
		LPRDOParam pReturn = rdo::Factory<RDOParam>::create(name->src_info(), pRetType, RDOVALUE($4));
		ASSERT(pReturn);
		LPRDOFUNFunction pFunction = rdo::Factory<RDOFUNFunction>::create(name->src_info(), pReturn);
		ASSERT(pFunction);
		$$ = PARSER->stack().push(pFunction);
	}
	| RDO_Function RDO_IDENTIF_COLON error
	{
		tstring name = P_RDOVALUE($2)->value().getIdentificator();
		PARSER->error().error(@3, rdo::format(_T("��������� ��� ������������� �������� ������� '%s'"), name.c_str()));
	}
	| RDO_Function error
	{
		PARSER->error().error(@2, _T("����� ��������� ����� $Function ��������� ��� ������"));
	}
	;

fun_func_parameters
	: /* empty */
	| RDO_Parameters fun_func_params
	;

fun_func_params
	: /* empty */
	| fun_func_params RDO_IDENTIF_COLON param_type param_value_default
	{
		PTR(RDOValue)  name  = P_RDOVALUE($2);
		LPTypeInfo pType = PARSER->stack().pop<TypeInfo>($3);
		ASSERT(pType);
		LPRDOParam pParam = rdo::Factory<RDOParam>::create(name->src_info(), pType, RDOVALUE($4));
		ASSERT(pParam);
		PARSER->getLastFUNFunction()->add(pParam);
	}
	| fun_func_params RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@3, _T("��������� ��� ��������� �������"));
	}
	| fun_func_params error
	{
		PARSER->error().error(@2, _T("��������� �������� ��������� ������� � ������� <���>: <���>"));
	}
	;

fun_func_footer
	: RDO_Type '=' RDO_algorithmic fun_func_parameters alg_fun_Body alg_fun_statement alg_fun_End
	{
		LPExpression pExpressionFunBodyBrace = PARSER->stack().pop<Expression>($6);
		ASSERT(pExpressionFunBodyBrace);

		rdoRuntime::LPRDOCalc pCalcEnd = rdo::Factory<rdoRuntime::RDOCalcFunEnd>::create();
		ASSERT(pCalcEnd);

		rdoRuntime::LPRDOCalcFunBodyBrace pCalcFunBodyBrace = pExpressionFunBodyBrace->calc().object_dynamic_cast<rdoRuntime::RDOCalcFunBodyBrace>();
		ASSERT(pCalcFunBodyBrace);
		pCalcFunBodyBrace->addFunCalc(pCalcEnd);

		LPExpressionStatement pExpressionReturn = pExpressionFunBodyBrace.object_dynamic_cast<ExpressionStatement>();

		rdoRuntime::LPRDOFunCalc pCalc = pExpressionFunBodyBrace->calc().object_dynamic_cast<rdoRuntime::RDOFunCalc>();
		ASSERT(pCalc);

		LPRDOFUNFunction pFunction = PARSER->getLastFUNFunction();
		ASSERT(pFunction);
		pExpressionReturn ? pFunction->setReturnFlag(true) : pFunction->setReturnFlag(false);
		pFunction->setFunctionCalc(pCalc);
		pFunction->createAlgorithmicCalc(@5);
	}
	| RDO_Type '=' RDO_list fun_func_parameters RDO_Body fun_func_list_body RDO_End
	{
		LPRDOFUNFunction pFunction = PARSER->getLastFUNFunction();
		ASSERT(pFunction);
		pFunction->createListCalc();
	}
	| RDO_Type '=' RDO_table fun_func_parameters RDO_Body fun_func_list_body RDO_End
	{
		LPRDOFUNFunction pFunction = PARSER->getLastFUNFunction();
		ASSERT(pFunction);
		pFunction->createTableCalc(@6);
	}
	| RDO_Type '=' RDO_algorithmic fun_func_parameters alg_fun_Body alg_fun_statement error
	{
		PARSER->error().error(@7, _T("��������� �������� ����� $End"));
	}
	| RDO_Type '=' RDO_list fun_func_parameters RDO_Body fun_func_list_body error
	{
		PARSER->error().error(@7, _T("��������� �������� ����� $End"));
	}
	| RDO_Type '=' RDO_table fun_func_parameters RDO_Body fun_func_list_body error
	{
		PARSER->error().error(@7, _T("��������� �������� ����� $End"));
	}
	| RDO_Type '=' RDO_algorithmic error
	{
		PARSER->error().error(@4, _T("��������� �������� ����� $Parameters"));
	}
	| RDO_Type '=' RDO_list error
	{
		PARSER->error().error(@4, _T("��������� �������� ����� $Parameters"));
	}
	| RDO_Type '=' RDO_table error
	{
		PARSER->error().error(@4, _T("��������� �������� ����� $Parameters"));
	}
	| RDO_Type '=' error
	{
		PARSER->error().error(@3, _T("����������� ��� �������"));
	}
	| RDO_Type error
	{
		PARSER->error().error(@2, _T("����� ��������� ����� $Type ��������� ��� �������"));
	}
	;

alg_fun_statement
	: /* empty */
	{
		rdoRuntime::LPRDOCalcFunBodyBrace pCalcFunBodyBrace = rdo::Factory<rdoRuntime::RDOCalcFunBodyBrace>::create();
		ASSERT(pCalcFunBodyBrace);

		rdoRuntime::LPRDOCalc pCalcOpenBrace = rdo::Factory<rdoRuntime::RDOCalcOpenBrace>::create();
		ASSERT(pCalcOpenBrace);

		pCalcFunBodyBrace->addFunCalc(pCalcOpenBrace);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__void>::create(), RDOParserSrcInfo());
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalcFunBodyBrace, RDOParserSrcInfo());
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	| alg_fun_statement statement
	{
		LPExpression pExpressionFunBodyBrace = PARSER->stack().pop<Expression>($1);
		ASSERT(pExpressionFunBodyBrace);

		LPExpression pExpression = PARSER->stack().pop<Expression>($2);
		ASSERT(pExpression);

		rdoRuntime::LPRDOCalcFunBodyBrace pCalcFunBodyBrace = pExpressionFunBodyBrace->calc().object_dynamic_cast<rdoRuntime::RDOCalcFunBodyBrace>();
		ASSERT(pCalcFunBodyBrace);
		pCalcFunBodyBrace->addFunCalc(pExpression->calc());

		pExpressionFunBodyBrace = rdo::Factory<Expression>::create(pExpressionFunBodyBrace->typeInfo(), pCalcFunBodyBrace, RDOParserSrcInfo(@1));
		ASSERT(pExpressionFunBodyBrace);

		LPExpressionStatement pExpressionReturn = pExpression.object_dynamic_cast<ExpressionStatement>();
		if(pExpressionReturn)
		{
			pExpressionFunBodyBrace = rdo::Factory<ExpressionStatement>::create(pExpressionFunBodyBrace);
		}

		$$ = PARSER->stack().push(pExpressionFunBodyBrace);
	}
	| error
	{
		PARSER->error().error(@1, _T("�������� ��������� ��������������� �������"));
	}
	;

alg_fun_Body
	: RDO_Body
	{
		LPLocalVariableList pLocalVariableList = rdo::Factory<LocalVariableList>::create();
		ASSERT(pLocalVariableList);

		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);

		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);

		pLocalVariableListStack->push(pLocalVariableList);
	}
	;

alg_fun_End
	: RDO_End
	{
		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);

		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);

		pLocalVariableListStack->pop();
	}
	;

statement
	: empty_statement ';'
	| empty_statement error
	{
		PARSER->error().error(@1, _T("�� ������ ������ ��������� ���������� - ����� � �������"));
	}
	| equal_statement ';'
	| equal_statement error
	{
		PARSER->error().error(@1, _T("�� ������ ������ ��������� ���������� - ����� � �������"));
	}
	| local_variable_declaration ';'
	| local_variable_declaration error
	{
		PARSER->error().error(@1, _T("�� ������ ������ ��������� ���������� - ����� � �������"));
	}
	| if_statement
	| for_statement
	| return_statement ';'
	| return_statement error
	{
		PARSER->error().error(@1, _T("�� ������ ������ ��������� ���������� - ����� � �������"));
	}
	| break_statement ';'
	| break_statement error
	{
		PARSER->error().error(@1, _T("�� ������ ������ ��������� ���������� - ����� � �������"));
	}
	| open_brace statement_list close_brace
	{
		LPExpression pExpressionFunBodyBrace = PARSER->stack().pop<Expression>($2);
		ASSERT(pExpressionFunBodyBrace);

		rdoRuntime::LPRDOCalc pCalcCloseBrace = rdo::Factory<rdoRuntime::RDOCalcCloseBrace>::create();
		ASSERT(pCalcCloseBrace);

		rdoRuntime::LPRDOCalcFunBodyBrace pCalcFunBodyBrace = pExpressionFunBodyBrace->calc().object_dynamic_cast<rdoRuntime::RDOCalcFunBodyBrace>();
		ASSERT(pCalcFunBodyBrace);
		pCalcFunBodyBrace->addFunCalc(pCalcCloseBrace);

		LPExpressionStatement pExpressionReturn = pExpressionFunBodyBrace.object_dynamic_cast<ExpressionStatement>();
		if(pExpressionReturn)
		{
			pExpressionFunBodyBrace = rdo::Factory<ExpressionStatement>::create(pExpressionFunBodyBrace->typeInfo(), pCalcFunBodyBrace, RDOParserSrcInfo(@1));
			ASSERT(pExpressionFunBodyBrace);
		}
		else
		{
			pExpressionFunBodyBrace = rdo::Factory<Expression>::create(pExpressionFunBodyBrace->typeInfo(), pCalcFunBodyBrace, RDOParserSrcInfo(@1));
			ASSERT(pExpressionFunBodyBrace);
		}

		$$ = PARSER->stack().push(pExpressionFunBodyBrace);
	}
	;

open_brace
	: '{'
	{
		LPLocalVariableList pLocalVariableList = rdo::Factory<LocalVariableList>::create();
		ASSERT(pLocalVariableList);

		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);

		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);

		pLocalVariableListStack->push(pLocalVariableList);
	}
	;

close_brace
	: '}'
	{
		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);

		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);

		pLocalVariableListStack->pop();
	}
	;

statement_list
	: /* empty */
	{
		rdoRuntime::LPRDOCalcFunBodyBrace pCalcFunBodyBrace = rdo::Factory<rdoRuntime::RDOCalcFunBodyBrace>::create();
		ASSERT(pCalcFunBodyBrace);

		rdoRuntime::LPRDOCalc pCalcOpenBrace = rdo::Factory<rdoRuntime::RDOCalcOpenBrace>::create();
		ASSERT(pCalcOpenBrace);

		pCalcFunBodyBrace->addFunCalc(pCalcOpenBrace);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__void>::create(), RDOParserSrcInfo());
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalcFunBodyBrace, RDOParserSrcInfo());
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	| statement_list statement
	{
		LPExpression pExpressionFunBodyBrace = PARSER->stack().pop<Expression>($1);
		ASSERT(pExpressionFunBodyBrace);

		LPExpression pExpression = PARSER->stack().pop<Expression>($2);
		ASSERT(pExpression);

		rdoRuntime::LPRDOCalcFunBodyBrace pCalcFunBodyBrace = pExpressionFunBodyBrace->calc().object_dynamic_cast<rdoRuntime::RDOCalcFunBodyBrace>();
		ASSERT(pCalcFunBodyBrace);
		pCalcFunBodyBrace->addFunCalc(pExpression->calc());

		pExpressionFunBodyBrace = rdo::Factory<Expression>::create(pExpressionFunBodyBrace->typeInfo(), pCalcFunBodyBrace, RDOParserSrcInfo(@1));
		ASSERT(pExpressionFunBodyBrace);

		LPExpressionStatement pExpressionReturn = pExpression.object_dynamic_cast<ExpressionStatement>();
		if(pExpressionReturn)
		{
			pExpressionFunBodyBrace = rdo::Factory<ExpressionStatement>::create(pExpressionFunBodyBrace);
		}

		$$ = PARSER->stack().push(pExpressionFunBodyBrace);
	}
	;

empty_statement
	: ';'
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcNoChange>::create();
		ASSERT(pCalc);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__void>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	| error ';'
	{
		PARSER->error().error(@1, _T("������ � ����������"));
	}
	;

equal_statement
	: RDO_IDENTIF increment_or_decrement_type
	{
		tstring                paramName = RDOVALUE($1)->getIdentificator();
		rdoRuntime::EqualType  equalType = static_cast<rdoRuntime::EqualType>($2);
		LPContext pContext = PARSER->context();
		ASSERT(pContext);
		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);
		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);
		LPLocalVariable pLocalVariable = pLocalVariableListStack->findLocalVariable(paramName);
		rdoRuntime::LPRDOCalc pCalc;
		if(pLocalVariable)
		{
			switch (equalType)
			{
				case rdoRuntime::ET_INCR:
				{
					pCalc = rdo::Factory<rdoRuntime::RDOCalcSetLocalVariable<rdoRuntime::ET_INCR> >::create(paramName);
					break;
				}
				case rdoRuntime::ET_DECR:
				{
					pCalc = rdo::Factory<rdoRuntime::RDOCalcSetLocalVariable<rdoRuntime::ET_DECR> >::create(paramName);
					break;
				}
				default:
				{
					NEVER_REACH_HERE;
				}
			}
		}
		else PARSER->error().error(@1, rdo::format(_T("����������� ��������: %s"), paramName.c_str()));
		tstring oprStr;
		switch (equalType)
		{
			case rdoRuntime::ET_INCR:
			{
				oprStr = _T("++");
				break;
			}
			case rdoRuntime::ET_DECR:
			{
				oprStr = _T("--");
				break;
			}
			default:
			{
				oprStr = _T("");
				break;
			}
		}
		pCalc->setSrcText(rdo::format(_T("%s %s"), paramName.c_str(), oprStr.c_str()));
		pCalc->setSrcPos (@1.m_first_line, @1.m_first_pos, @2.m_last_line, @2.m_last_pos);

		LPExpression pExpression = rdo::Factory<Expression>::create(pLocalVariable->getTypeInfo(), pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	| RDO_IDENTIF param_equal_type fun_arithm
	{
		tstring               paramName    = RDOVALUE($1)->getIdentificator();
		rdoRuntime::EqualType equalType    = static_cast<rdoRuntime::EqualType>($2);
		LPRDOFUNArithm        pRightArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		LPContext pContext = PARSER->context();
		ASSERT(pContext);
		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);
		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);
		LPLocalVariable pLocalVariable = pLocalVariableListStack->findLocalVariable(paramName);
		rdoRuntime::LPRDOCalc pCalc;
		rdoRuntime::LPRDOCalc pCalcRight;
		if (pLocalVariable)
		{
			pCalcRight = pRightArithm->createCalc(pLocalVariable->getTypeInfo());
			switch (equalType)
			{
				case rdoRuntime::ET_NOCHANGE:
				{
					break;
				}
				case rdoRuntime::ET_EQUAL:
				{
					pCalc = rdo::Factory<rdoRuntime::RDOCalcSetLocalVariable<rdoRuntime::ET_EQUAL> >::create(paramName, pCalcRight);
					break;
				}
				case rdoRuntime::ET_PLUS:
				{
					pCalc = rdo::Factory<rdoRuntime::RDOCalcSetLocalVariable<rdoRuntime::ET_PLUS> >::create(paramName, pCalcRight);
					break;
				}
				case rdoRuntime::ET_MINUS:
				{
					pCalc = rdo::Factory<rdoRuntime::RDOCalcSetLocalVariable<rdoRuntime::ET_MINUS> >::create(paramName, pCalcRight);
					break;
				}
				case rdoRuntime::ET_MULTIPLY:
				{
					pCalc = rdo::Factory<rdoRuntime::RDOCalcSetLocalVariable<rdoRuntime::ET_MULTIPLY> >::create(paramName, pCalcRight);
					break;
				}
				case rdoRuntime::ET_DIVIDE:
				{
					pCalc = rdo::Factory<rdoRuntime::RDOCalcSetLocalVariable<rdoRuntime::ET_DIVIDE> >::create(paramName, pCalcRight);
					break;
				}
				default:
				{
					NEVER_REACH_HERE;
				}
			}
		}
		else PARSER->error().error(@1, rdo::format(_T("����������� ��������: %s"), paramName.c_str()));
		tstring oprStr;
		switch (equalType)
		{
			case rdoRuntime::ET_EQUAL:
			{
				oprStr = _T("=");
				break;
			}
			case rdoRuntime::ET_PLUS:
			{
				oprStr = _T("+=");
				break;
			}
			case rdoRuntime::ET_MINUS:
			{
				oprStr = _T("-=");
				break;
			}
			case rdoRuntime::ET_MULTIPLY:
			{
				oprStr = _T("*=");
				break;
			}
			case rdoRuntime::ET_DIVIDE:
			{
				oprStr = _T("/=");
				break;
			}
			default:
			{
				oprStr = _T("");
				break;
			}
		}
		pCalc->setSrcText(rdo::format(_T("%s %s %s"), paramName.c_str(), oprStr.c_str(), pCalcRight->src_text().c_str()));
		pCalc->setSrcPos (@1.m_first_line, @1.m_first_pos, @3.m_last_line, @3.m_last_pos);

		LPExpression pExpression = rdo::Factory<Expression>::create(pLocalVariable->getTypeInfo(), pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	| RDO_IDENTIF param_equal_type error
	{
		PARSER->error().error(@3, _T("������ � �������������� ���������"));
	}
	| RDO_IDENTIF error fun_arithm
	{
		PARSER->error().error(@2, _T("������ � ��������� ������������"));
	}
	;

local_variable_declaration
	: type_declaration init_declaration_list
	{
		LPTypeInfo pType = PARSER->stack().pop<TypeInfo>($1);
		ASSERT(pType);

		rdoRuntime::LPRDOCalc pCalc = PARSER->stack().pop<rdoRuntime::RDOCalc>($2);
		ASSERT(pCalc);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);

		PARSER->contextStack()->pop();
	}
	;

type_declaration
	: RDO_integer
	{
		LPRDOType pBaseType = rdo::Factory<RDOType__int>::create();
		ASSERT(pBaseType);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pBaseType, RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_real
	{
		LPRDOType pBaseType = rdo::Factory<RDOType__real>::create();
		ASSERT(pBaseType);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pBaseType, RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_string
	{
		LPRDOType pBaseType = rdo::Factory<RDOType__string>::create();
		ASSERT(pBaseType);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pBaseType, RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_array
	{
		LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($1);
		ASSERT(pArray);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pArray, RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_bool
	{
		LPRDOType pBaseType = rdo::Factory<RDOType__bool>::create();
		ASSERT(pBaseType);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pBaseType, RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_enum
	{
		LEXER->enumReset();
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
		ASSERT(pEnum);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_such_as
	{
		LPTypeInfo pTypeSuchAs = PARSER->stack().pop<TypeInfo>($1);
		ASSERT(pTypeSuchAs);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pTypeSuchAs->type(), RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPContext pTypeContext = rdo::Factory<TypeContext>::create(pType);
		ASSERT(pTypeContext);

		PARSER->contextStack()->push(pTypeContext);
		$$ = PARSER->stack().push(pType);
	}
	;

init_declaration_list
	: init_declaration
	{
		LPVariableContainer pVariableContainer = PARSER->stack().pop<VariableContainer>($1);
		ASSERT(pVariableContainer);

		LPLocalVariable pLocalVariable = pVariableContainer->getLocalVariable();
		ASSERT(pLocalVariable);

		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);

		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);

		pLocalVariableListStack->append(pLocalVariable);

		rdoRuntime::LPRDOCalc pCalc = pVariableContainer->getCalc();
		ASSERT(pCalc);

		rdoRuntime::LPRDOCalcLocalVariableList pCalcLocalVariableList = rdo::Factory<rdoRuntime::RDOCalcLocalVariableList>::create();
		ASSERT(pCalcLocalVariableList);

		pCalcLocalVariableList->addCalcLocalVariable(pCalc);

		$$ = PARSER->stack().push(pCalcLocalVariableList);
	}
	| init_declaration_list ',' init_declaration
	{
		LPVariableContainer pVariableContainer = PARSER->stack().pop<VariableContainer>($3);
		ASSERT(pVariableContainer);

		LPLocalVariable pLocalVariable = pVariableContainer->getLocalVariable();
		ASSERT(pLocalVariable);

		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPContextMemory pContextMemory = pContext->cast<ContextMemory>();
		ASSERT(pContextMemory);

		LPLocalVariableListStack pLocalVariableListStack = pContextMemory->getLocalMemory();
		ASSERT(pLocalVariableListStack);

		pLocalVariableListStack->append(pLocalVariable);

		rdoRuntime::LPRDOCalc pCalc = pVariableContainer->getCalc();
		ASSERT(pCalc);

		rdoRuntime::LPRDOCalcLocalVariableList pCalcLocalVariableList = PARSER->stack().pop<rdoRuntime::RDOCalcLocalVariableList>($1);
		ASSERT(pCalcLocalVariableList);

		pCalcLocalVariableList->addCalcLocalVariable(pCalc);

		$$ = PARSER->stack().push(pCalcLocalVariableList);
	}
	;

init_declaration
	: RDO_IDENTIF
	{
		RDOValue variableName = RDOVALUE($1);

		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPTypeContext pTypeContext = pContext.object_static_cast<TypeContext>();
		ASSERT(pTypeContext);

		LPTypeInfo pParam = pTypeContext->getTypeInfo();
		ASSERT(pParam);

		LPExpression pExpression = rdo::Factory<Expression>::create(
			rdo::Factory<TypeInfo>::create(
				pParam->type(),
				pParam->src_info()
			),
			rdo::Factory<rdoRuntime::RDOCalcConst>::create(variableName.value()),
			variableName.src_info()
		);
		ASSERT(pExpression);

		LPLocalVariable pLocalVariable = rdo::Factory<LocalVariable>::create(variableName, pExpression, pParam);
		ASSERT(pLocalVariable);

		rdoRuntime::LPRDOCalcCreateLocalVariable pCalcCreateLocalVariable = rdo::Factory<rdoRuntime::RDOCalcCreateLocalVariable>::create(variableName->getIdentificator());
		ASSERT(pCalcCreateLocalVariable);

		LPVariableContainer pVariableContainer = rdo::Factory<VariableContainer>::create(pCalcCreateLocalVariable, pLocalVariable);

		$$ = PARSER->stack().push(pVariableContainer);
	}
	| RDO_IDENTIF '=' fun_arithm
	{
		RDOValue variableName = RDOVALUE($1);

		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm);

		LPContext pContext = PARSER->context();
		ASSERT(pContext);

		LPTypeContext pTypeContext = pContext.object_static_cast<TypeContext>();
		ASSERT(pTypeContext);

		LPTypeInfo pParam = pTypeContext->getTypeInfo();
		ASSERT(pParam);

		LPLocalVariable pLocalVariable = rdo::Factory<LocalVariable>::create(variableName, pArithm->expression(), pParam);
		ASSERT(pLocalVariable);

		rdoRuntime::LPRDOCalcLocalVariableList pCalcLocalVariableList = rdo::Factory<rdoRuntime::RDOCalcLocalVariableList>::create();
		ASSERT(pCalcLocalVariableList);

		rdoRuntime::LPRDOCalcCreateLocalVariable pCalcCreateLocalVariable = rdo::Factory<rdoRuntime::RDOCalcCreateLocalVariable>::create(variableName->getIdentificator());
		ASSERT(pCalcCreateLocalVariable);

		rdoRuntime::LPRDOCalcInitLocalVariable pCalcSetLocalVariable = rdo::Factory<rdoRuntime::RDOCalcInitLocalVariable>::create(variableName->getIdentificator(), pArithm->calc());
		ASSERT(pCalcSetLocalVariable);

		pCalcLocalVariableList->addCalcLocalVariable(pCalcCreateLocalVariable);
		pCalcLocalVariableList->addCalcLocalVariable(pCalcSetLocalVariable);

		LPVariableContainer pVariableContainer = rdo::Factory<VariableContainer>::create(pCalcLocalVariableList, pLocalVariable);

		$$ = PARSER->stack().push(pVariableContainer);
	}
	;

if_statement
	: RDO_if '(' fun_logic ')' statement
	{
		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pCondition);

		rdoRuntime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);
		rdoRuntime::LPRDOCalcConst pConstCalc = pConditionCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>();

		LPExpression pIfExpression = PARSER->stack().pop<Expression>($5);
		ASSERT(pIfExpression);
		LPExpressionStatement pExpressionReturn = pIfExpression.object_dynamic_cast<ExpressionStatement>();

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__void>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcIf>::create(pConditionCalc, pIfExpression->calc());
		ASSERT(pCalc);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);
		if (pConstCalc && pExpressionReturn && pConstCalc->getValue().getAsBool())
		{
			pExpression = rdo::Factory<ExpressionStatement>::create(pType, pCalc, RDOParserSrcInfo(@1));
			ASSERT(pExpression);
		}
		$$ = PARSER->stack().push(pExpression);
	}
	| RDO_if '(' fun_logic ')' statement RDO_else statement
	{
		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pCondition);
		
		rdoRuntime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);
		rdoRuntime::LPRDOCalcConst pConstCalc = pConditionCalc.object_dynamic_cast<rdoRuntime::RDOCalcConst>();

		LPExpression pIfExpression = PARSER->stack().pop<Expression>($5);
		ASSERT(pIfExpression);
		LPExpressionStatement pIfExpressionReturn = pIfExpression.object_dynamic_cast<ExpressionStatement>();

		LPExpression pElseExpression = PARSER->stack().pop<Expression>($7);
		ASSERT(pElseExpression);
		LPExpressionStatement pElseExpressionReturn = pElseExpression.object_dynamic_cast<ExpressionStatement>();

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__void>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcIfElse>::create(pConditionCalc, pIfExpression->calc(), pElseExpression->calc());
		ASSERT(pCalc);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);
		if(pIfExpressionReturn&&pElseExpressionReturn)
		{
			pExpression = rdo::Factory<ExpressionStatement>::create(pType, pCalc, RDOParserSrcInfo(@1));
			ASSERT(pExpression);
		}
		else if(pConstCalc)
		{
			if(pConstCalc->getValue().getAsBool())
			{
				if(pIfExpressionReturn)
				{
					pExpression = rdo::Factory<ExpressionStatement>::create(pType, pCalc, RDOParserSrcInfo(@1));
					ASSERT(pExpression);
				}
			}
			else
			{
				if(pElseExpressionReturn)
				{
					pExpression = rdo::Factory<ExpressionStatement>::create(pType, pCalc, RDOParserSrcInfo(@1));
					ASSERT(pExpression);
				}
			}
		}

		$$ = PARSER->stack().push(pExpression);
	}
	| RDO_if error fun_logic
	{
		PARSER->error().error(@2, _T("��������� ����������� ������"));
	}
	| RDO_if '(' fun_logic error
	{
		PARSER->error().error(@4, _T("��������� ����������� ������"));
	}
	;

for_statement
	: RDO_for '(' local_variable_declaration ';' fun_logic ';' equal_statement ')' statement
	{
		LPExpression pDeclarationExpression = PARSER->stack().pop<Expression>($3);
		ASSERT(pDeclarationExpression);

		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($5);
		ASSERT(pCondition);
		
		rdoRuntime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);

		LPExpression pEqualExpression = PARSER->stack().pop<Expression>($7);
		ASSERT(pEqualExpression);

		LPExpression pStatementExpression = PARSER->stack().pop<Expression>($9);
		ASSERT(pStatementExpression);
		LPExpressionStatement pStatementExpressionReturn = pStatementExpression.object_dynamic_cast<ExpressionStatement>();

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__void>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcFor>::create(
			pDeclarationExpression->calc(),
			pConditionCalc                ,
			pEqualExpression->calc()      ,
			pStatementExpression->calc()  );
		ASSERT(pCalc);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		if(pStatementExpressionReturn)
		{
			pExpression = rdo::Factory<ExpressionStatement>::create(pType, pCalc, RDOParserSrcInfo(@1));
			ASSERT(pExpression);
		}

		$$ = PARSER->stack().push(pExpression);
	}
	| RDO_for '(' equal_statement ';' fun_logic ';' equal_statement ')' statement
	{
		LPExpression pInitExpression = PARSER->stack().pop<Expression>($3);
		ASSERT(pInitExpression);

		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($5);
		ASSERT(pCondition);
		
		rdoRuntime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);

		LPExpression pEqualExpression = PARSER->stack().pop<Expression>($7);
		ASSERT(pEqualExpression);

		LPExpression pStatementExpression = PARSER->stack().pop<Expression>($9);
		ASSERT(pStatementExpression);
		LPExpressionStatement pStatementExpressionReturn = pStatementExpression.object_dynamic_cast<ExpressionStatement>();

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__void>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcFor>::create(
			pInitExpression->calc()     ,
			pConditionCalc              ,
			pEqualExpression->calc()    ,
			pStatementExpression->calc());
		ASSERT(pCalc);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		if(pStatementExpressionReturn)
		{
			pExpression = rdo::Factory<ExpressionStatement>::create(pType, pCalc, RDOParserSrcInfo(@1));
			ASSERT(pExpression);
		}

		$$ = PARSER->stack().push(pExpression);
	}
	;

increment_or_decrement_type
	: RDO_IncrEqual
	{
		$$ = rdoRuntime::ET_INCR;
	}
	| RDO_DecrEqual
	{
		$$ = rdoRuntime::ET_DECR;
	}
	;

param_equal_type
	: RDO_set
	{
		$$ = rdoRuntime::ET_EQUAL;
	}
	| '='
	{
		$$ = rdoRuntime::ET_EQUAL;
	}
	| RDO_PlusEqual
	{
		$$ = rdoRuntime::ET_PLUS;
	}
	| RDO_MinusEqual
	{
		$$ = rdoRuntime::ET_MINUS;
	}
	| RDO_MultiplyEqual
	{
		$$ = rdoRuntime::ET_MULTIPLY;
	}
	| RDO_DivideEqual
	{
		$$ = rdoRuntime::ET_DIVIDE;
	}
	;

return_statement
	:RDO_Return fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(PARSER->getLastFUNFunction()->getReturn()->getTypeInfo());
		ASSERT(pType);

		rdoRuntime::LPRDOCalc pCalc = pArithm->createCalc(pType);
		ASSERT(pCalc);

		rdoRuntime::LPRDOCalc pCalcReturn = rdo::Factory<rdoRuntime::RDOCalcFunReturn>::create(pCalc);
		ASSERT(pCalcReturn);

		LPExpressionStatement pExpression = rdo::Factory<ExpressionStatement>::create(pType, pCalcReturn, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	;

break_statement
	:RDO_Break
	{
		rdoRuntime::LPRDOCalc pCalcBreak = rdo::Factory<rdoRuntime::RDOCalcFunBreak>::create();
		ASSERT(pCalcBreak);

		LPRDOType pBaseType = rdo::Factory<RDOType__void>::create();
		ASSERT(pBaseType);

		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pBaseType, RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalcBreak, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		$$ = PARSER->stack().push(pExpression);
	}
	;

fun_func_list_body
	: /* empty */
	| fun_func_list_body fun_func_list_value
	;

fun_func_list_value
	: RDO_IDENTIF
	{
		tstring str = P_RDOVALUE($1)->value().getIdentificator();
		LPRDOFUNFunctionListElementIdentif pValue = rdo::Factory<RDOFUNFunctionListElementIdentif>::create(RDOParserSrcInfo(@1, str));
		ASSERT(pValue);
		PARSER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = PARSER->stack().push(pValue);
	}
	| RDO_REAL_CONST
	{
		LPRDOFUNFunctionListElementReal pValue = rdo::Factory<RDOFUNFunctionListElementReal>::create(@1, P_RDOVALUE($1)->value().getDouble());
		ASSERT(pValue);
		PARSER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = PARSER->stack().push(pValue);
	}
	| RDO_INT_CONST
	{
		LPRDOFUNFunctionListElementInt pValue = rdo::Factory<RDOFUNFunctionListElementInt>::create(@1, P_RDOVALUE($1)->value().getInt());
		ASSERT(pValue);
		PARSER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = PARSER->stack().push(pValue);
	}
	| '='
	{
		LPRDOFUNFunctionListElementEq pValue = rdo::Factory<RDOFUNFunctionListElementEq>::create(@1);
		ASSERT(pValue);
		PARSER->getLastFUNFunction()->add(pValue.object_parent_cast<RDOFUNFunctionListElement>());
		$$ = PARSER->stack().push(pValue);
	}
	;

// ----------------------------------------------------------------------------
// ---------- ������������������
// ----------------------------------------------------------------------------
fun_seq_descr
	: fun_seq_uniform
	| fun_seq_exponential
	| fun_seq_normal
	| fun_seq_by_hist
	| fun_seq_enumerative
	;

fun_seq_header
	: RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type '='
	{
		PTR(RDOValue) name = P_RDOVALUE($2);
		PARSER->checkFunctionName( name->src_info() );
		LPTypeInfo pType = PARSER->stack().pop<TypeInfo>($3);
		ASSERT(pType);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = rdo::Factory<RDOFUNSequence::RDOFUNSequenceHeader>::create(pType, name->src_info());
		ASSERT(pHeader);
		$$ = PARSER->stack().push(pHeader);
	}
	| RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type '=' error
	{
		PARSER->error().error(@6, _T("����� ����� ��������� ��������� ��� ������������������"));
	}
	| RDO_Sequence RDO_IDENTIF_COLON param_type RDO_Type error
	{
		PARSER->error().error(@5, _T("����� ��������� ����� $Type ��������� ���� ��������� � ��� ������������������"));
	}
	| RDO_Sequence RDO_IDENTIF_COLON param_type error
	{
		PARSER->error().error(@4, _T("��������� �������� ����� $Type"));
	}
	| RDO_Sequence RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("����� ����� ������������������ ��������� ��� ������������� ��������"));
	}
	| RDO_Sequence error
	{
		PARSER->error().error(@1, @2, _T("����� ��������� ����� $Sequence ��������� ��� � ��� ���������� ������������������ � ������� '<���> : <���>'"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- ������� ��������� �����
// ----------------------------------------------------------------------------
fun_seq_uniform
	: fun_seq_header RDO_uniform RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceUniform>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_uniform RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceUniform>::create(pHeader, P_RDOVALUE($3)->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_uniform RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("����� ���� ��������� �������� ����� $End"));
	}
	| fun_seq_header RDO_uniform error
	{
		PARSER->error().error(@3, _T("����� ���� ������������������ ��������� ���� ���������� ��� �������� ����� $End"));
	}
	;

fun_seq_exponential
	: fun_seq_header RDO_exponential RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceExponential>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_exponential RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceExponential>::create(pHeader, P_RDOVALUE($3)->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_exponential RDO_INT_CONST error
	{
		PARSER->error().error( @4, _T("����� ���� ��������� �������� ����� $End"));
	}
	| fun_seq_header RDO_exponential error
	{
		PARSER->error().error( @3, _T("����� ���� ������������������ ��������� ���� ���������� ��� �������� ����� $End"));
	}
	;

fun_seq_normal
	: fun_seq_header RDO_normal RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceNormal>::create(pHeader);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_normal RDO_INT_CONST RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequence pSequence = rdo::Factory<RDOFUNSequenceNormal>::create(pHeader, P_RDOVALUE($3)->value().getInt());
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_header RDO_normal RDO_INT_CONST error
	{
		PARSER->error().error( @4, _T("����� ���� ��������� �������� ����� $End"));
	}
	| fun_seq_header RDO_normal error
	{
		PARSER->error().error( @3, _T("����� ���� ������������������ ��������� ���� ���������� ��� �������� ����� $End"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- �����������
// ----------------------------------------------------------------------------
fun_seq_by_hist_header
	: fun_seq_header RDO_by_hist RDO_Body
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pByHistHeader = rdo::Factory<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>::create(pHeader);
		ASSERT(pByHistHeader);
		$$ = PARSER->stack().push(pByHistHeader);
	}
	| fun_seq_header RDO_by_hist RDO_INT_CONST RDO_Body
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequence::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pByHistHeader = rdo::Factory<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>::create(pHeader, P_RDOVALUE($3)->value().getInt());
		ASSERT(pByHistHeader);
		$$ = PARSER->stack().push(pByHistHeader);
	}
	| fun_seq_header RDO_by_hist RDO_INT_CONST error
	{
		PARSER->error().error( @4, _T("����� ���� ��������� �������� ����� $Body"));
	}
	| fun_seq_header RDO_by_hist error
	{
		PARSER->error().error( @3, _T("����� ���� ������������������ ��������� ���� ���������� ��� �������� ����� $Body"));
	}
	;

fun_seq_by_hist_body_real
	: fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @3, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @3, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->getHeader()->src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistReal pSequence = rdo::Factory<RDOFUNSequenceByHistReal>::create(pHeader, RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @3, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = pSequence->getHeader();
		ASSERT(pHeader);
		if ( pHeader->getTypeInfo()->type()->typeID() == rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @3, rdo::format(_T("������������������ '%s' ���������� ��� �������������, � ��������� ���� ������ ���� ��������������"), pHeader->src_text().c_str()) );
		}
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistReal pSequence = PARSER->stack().pop<RDOFUNSequenceByHistReal>($1);
		ASSERT(pSequence);
		pSequence->addReal(RDOVALUE($2), RDOVALUE($3), RDOVALUE($4));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if (pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int)
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		PARSER->error().error( @2, @3, _T("��������� ����� ���������") );
	}
	| fun_seq_by_hist_header RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		PARSER->error().error( @2, @3, _T("��������� ����� ���������") );
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_header RDO_REAL_CONST RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������"));
	}
	| fun_seq_by_hist_header RDO_INT_CONST RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_real && pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_int )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST error
	{
		PARSER->error().error( @2, @3, _T("��������� ����� ���������") );
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST error
	{
		PARSER->error().error( @2, @3, _T("��������� ����� ���������") );
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_REAL_CONST error
	{
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_REAL_CONST error
	{
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_body_real RDO_REAL_CONST RDO_INT_CONST error
	{
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_body_real RDO_INT_CONST RDO_INT_CONST error
	{
		PARSER->error().error( @3, @4, _T("��������� ������������� �����������") );
	}
	| fun_seq_by_hist_header RDO_End
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		PARSER->error().error( pHeader->src_info(), rdo::format(_T("������������������ '%s' �� ������ ���� ������"), pHeader->src_text().c_str()) );
	}
	;

fun_seq_by_hist_body_enum
	: fun_seq_by_hist_header RDO_IDENTIF RDO_REAL_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_enum )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistEnum pSequence = rdo::Factory<RDOFUNSequenceByHistEnum>::create(pHeader, RDOVALUE($2), RDOVALUE($3));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_IDENTIF RDO_INT_CONST
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_enum )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		LPRDOFUNSequenceByHistEnum pSequence = rdo::Factory<RDOFUNSequenceByHistEnum>::create(pHeader, RDOVALUE($2), RDOVALUE($3));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_enum RDO_IDENTIF RDO_REAL_CONST
	{
		LPRDOFUNSequenceByHistEnum pSequence = PARSER->stack().pop<RDOFUNSequenceByHistEnum>($1);
		ASSERT(pSequence);
		pSequence->addEnum(RDOVALUE($2), RDOVALUE($3));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_enum RDO_IDENTIF RDO_INT_CONST
	{
		LPRDOFUNSequenceByHistEnum pSequence = PARSER->stack().pop<RDOFUNSequenceByHistEnum>($1);
		ASSERT(pSequence);
		pSequence->addEnum(RDOVALUE($2), RDOVALUE($3));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_header RDO_IDENTIF error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		if ( pHeader->getHeader()->getTypeInfo()->type()->typeID() != rdoRuntime::RDOType::t_enum )
		{
			PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
		}
		PARSER->error().error( @2, @3, rdo::format(_T("��������� ������������� ����������� ��� ��������: %s"), P_RDOVALUE($2)->value().getIdentificator().c_str()) );
	}
	| fun_seq_by_hist_body_enum RDO_IDENTIF error
	{
		PARSER->error().error( @2, @3, rdo::format(_T("��������� ������������� ����������� ��� ��������: %s"), P_RDOVALUE($2)->value().getIdentificator().c_str()) );
	}
	| fun_seq_by_hist_body_enum RDO_REAL_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
	}
	| fun_seq_by_hist_body_enum RDO_INT_CONST error
	{
		RDOFUNSequenceByHist::LPRDOFUNSequenceByHistHeader pHeader = PARSER->stack().pop<RDOFUNSequenceByHist::RDOFUNSequenceByHistHeader>($1);
		ASSERT(pHeader);
		PARSER->error().error( @2, rdo::format(_T("�������� �� ������������� ���� ������������������, ��������� �������� ������ ��������������� ����: %s"), pHeader->getHeader()->getTypeInfo()->src_info().src_text().c_str()) );
	}
	;

fun_seq_by_hist
	: fun_seq_by_hist_body_real RDO_End
	{
		LPRDOFUNSequence pSequence = PARSER->stack().pop<RDOFUNSequence>($1);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_enum RDO_End
	{
		LPRDOFUNSequence pSequence = PARSER->stack().pop<RDOFUNSequence>($1);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_by_hist_body_real error
	{
		PARSER->error().error( @2, _T("������ � �������� ������������������") );
	}
	| fun_seq_by_hist_body_enum error
	{
		PARSER->error().error( @2, _T("������ � �������� ������������������") );
	}
	| fun_seq_by_hist_body_real
	{
		PARSER->error().error( @1, _T("��������� �������� ����� $End") );
	}
	| fun_seq_by_hist_body_enum
	{
		PARSER->error().error( @1, _T("��������� �������� ����� $End") );
	}
	;

// ----------------------------------------------------------------------------
// ---------- �������� ��������
// ----------------------------------------------------------------------------
fun_seq_enumerative
	: fun_seq_enumerative_body RDO_End
	{
		LPRDOFUNSequence pSequence = PARSER->stack().pop<RDOFUNSequence>($1);
		ASSERT(pSequence);
		pSequence->createCalcs();
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_End
	{
		RDOFUNSequence::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		PARSER->error().error( pHeader->src_info(), rdo::format(_T("������������������ '%s' �� ������ ���� ������"), pHeader->src_text().c_str()) );
	}
	;

fun_seq_enumerative_header
	: fun_seq_header RDO_enumerative RDO_Body
	| fun_seq_header RDO_enumerative RDO_INT_CONST error
	{
		PARSER->error().error( @3, _T("� ������������������ ���� enumerative ��� ���� ����������") );
	}
	| fun_seq_header RDO_enumerative RDO_Parameters error
	{
		PARSER->error().error( @3, _T("� ������������������ ���� enumerative ��� ����������") );
	}
	| fun_seq_header RDO_enumerative error
	{
		PARSER->error().error( @3, _T("����� ���� ������������������ ��������� �������� ����� $Body") );
	}
	;

fun_seq_enumerative_body
	: fun_seq_enumerative_header RDO_INT_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, RDOVALUE($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_REAL_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, RDOVALUE($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_BOOL_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, RDOVALUE($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_STRING_CONST
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, RDOVALUE($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_header RDO_IDENTIF
	{
		RDOFUNSequenceEnumerative::LPRDOFUNSequenceHeader pHeader = PARSER->stack().pop<RDOFUNSequenceEnumerative::RDOFUNSequenceHeader>($1);
		ASSERT(pHeader);
		LPRDOFUNSequenceEnumerative pSequence = rdo::Factory<RDOFUNSequenceEnumerative>::create(pHeader, RDOVALUE($2));
		ASSERT(pSequence);
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_INT_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(RDOVALUE($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_REAL_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(RDOVALUE($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_BOOL_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(RDOVALUE($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_STRING_CONST
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(RDOVALUE($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body RDO_IDENTIF
	{
		LPRDOFUNSequenceEnumerative pSequence = PARSER->stack().pop<RDOFUNSequenceEnumerative>($1);
		ASSERT(pSequence);
		pSequence->addValue(RDOVALUE($2));
		$$ = PARSER->stack().push(pSequence);
	}
	| fun_seq_enumerative_body error
	{
		PARSER->error().error(@1, @2, _T("��������� ������� ������������������ ��� �������� ����� $End"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- �������� ���� ���������
// ----------------------------------------------------------------------------
param_type
	: RDO_integer param_type_range
	{
		LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
		LPTypeInfo pType;
		if (pRange)
		{
			if (pRange->getMin().typeID() != rdoRuntime::RDOType::t_int ||
			    pRange->getMax().typeID() != rdoRuntime::RDOType::t_int)
			{
				PARSER->error().error(@2, _T("�������� ������ ���� ������ ���� �������������"));
			}
			LPRDOTypeIntRange pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			ASSERT(pIntRange);
			pType = rdo::Factory<TypeInfo>::create(pIntRange, RDOParserSrcInfo(@1, @2));
		}
		else
		{
			pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__int>::create(), RDOParserSrcInfo(@1, @2));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_real param_type_range
	{
		LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
		LPTypeInfo pType;
		if (pRange)
		{
			LPRDOTypeRealRange pRealRange = rdo::Factory<RDOTypeRealRange>::create(pRange);
			ASSERT(pRealRange);
			pType = rdo::Factory<TypeInfo>::create(pRealRange, RDOParserSrcInfo(@1, @2));
		}
		else
		{
			pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__real>::create(), RDOParserSrcInfo(@1, @2));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_string
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__string>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_array
	{
		LEXER->array_cnt_rst();
		LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($1);
		ASSERT(pArray);
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pArray, RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_bool
	{
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(rdo::Factory<RDOType__bool>::create(), RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_enum
	{
		LEXER->enumReset();
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
		ASSERT(pEnum);
		LPTypeInfo pType = rdo::Factory<TypeInfo>::create(pEnum, RDOParserSrcInfo(@1));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_such_as
	{
		LPRDOTypeParamSuchAs pTypeSuchAs = PARSER->stack().pop<RDOTypeParamSuchAs>($1);
		ASSERT(pTypeSuchAs);
		LPTypeInfo pType = pTypeSuchAs.object_parent_cast<TypeInfo>();
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	;

param_type_range
	: /* empty */
	{
		$$ = PARSER->stack().push<RDOTypeRangeRange>(LPRDOTypeRangeRange());
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST error
	{
		PARSER->error().error(@4, _T("�������� ����� �������"));
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("�������� ����� �������"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST error
	{
		PARSER->error().error(@4, _T("�������� ����� �������"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("�������� ����� �������"));
	}
	| '[' RDO_REAL_CONST RDO_dblpoint error
	{
		PARSER->error().error(@4, _T("�������� ����� �������"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint error
	{
		PARSER->error().error(@4, _T("�������� ����� �������"));
	}
	| '[' error
	{
		PARSER->error().error(@2, _T("�������� ����� �������"));
	}
	;

param_type_enum
	: '(' param_type_enum_list ')'
	{
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($2);
		ASSERT(pEnum);
		$$ = PARSER->stack().push(pEnum);
	}
	| '(' param_type_enum_list error
	{
		PARSER->error().error(@2, _T("������������ ������ ������������� �������"));
	}
	;

param_type_enum_list
	: RDO_IDENTIF
	{
		LPRDOEnumType pEnum = rdo::Factory<RDOEnumType>::create();
		ASSERT(pEnum);
		pEnum->add(RDOVALUE($1));
		LEXER->enumBegin();
		$$ = PARSER->stack().push(pEnum);
	}
	| param_type_enum_list ',' RDO_IDENTIF
	{
		if (!LEXER->enumEmpty())
		{
			LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
			ASSERT(pEnum);
			pEnum->add(RDOVALUE($3));
			$$ = PARSER->stack().push(pEnum);
		}
		else
		{
			PARSER->error().error(@3, _T("������ � �������� �������� ������������� ����"));
		}
	}
	| param_type_enum_list RDO_IDENTIF
	{
		if (!LEXER->enumEmpty())
		{
			LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
			ASSERT(pEnum);
			pEnum->add(RDOVALUE($2));
			$$ = PARSER->stack().push(pEnum);
			PARSER->error().warning(@1, rdo::format(_T("��������� ������� �����: %s"), RDOVALUE($2)->getIdentificator().c_str()));
		}
		else
		{
			PARSER->error().error(@2, _T("������ � �������� �������� ������������� ����"));
		}
	}
	| param_type_enum_list ',' RDO_INT_CONST
	{
		PARSER->error().error(@3, _T("�������� ������������� ���� �� ����� ���� ������"));
	}
	| param_type_enum_list ',' RDO_REAL_CONST
	{
		PARSER->error().error(@3, _T("�������� ������������� ���� �� ����� ���� ������"));
	}
	| param_type_enum_list RDO_INT_CONST
	{
		PARSER->error().error(@2, _T("�������� ������������� ���� �� ����� ���� ������"));
	}
	| param_type_enum_list RDO_REAL_CONST
	{
		PARSER->error().error(@2, _T("�������� ������������� ���� �� ����� ���� ������"));
	}
	| RDO_INT_CONST
	{
		PARSER->error().error(@1, _T("�������� ������������� ���� �� ����� ���������� � �����"));
	}
	| RDO_REAL_CONST
	{
		PARSER->error().error(@1, _T("�������� ������������� ���� �� ����� ���������� � �����"));
	}
	;

param_type_such_as
	: RDO_such_as RDO_IDENTIF '.' RDO_IDENTIF
	{
		tstring type  = RDOVALUE($2)->getIdentificator();
		tstring param = RDOVALUE($4)->getIdentificator();
		LPRDORTPResType pResType = PARSER->findRTPResType(type);
		if (!pResType)
		{
			PARSER->error().error(@2, rdo::format(_T("������ �� ����������� ��� �������: %s"), type.c_str()));
		}
		LPRDORTPParam pRTPParam = pResType->findRTPParam(param);
		if (!pRTPParam)
		{
			PARSER->error().error(@4, rdo::format(_T("������ �� ����������� �������� �������: %s.%s"), type.c_str(), param.c_str()));
		}
		LPRDOParam pParam = pRTPParam.object_parent_cast<RDOParam>();
		ASSERT(pParam);
		LPRDOTypeParamSuchAs pTypeSuchAs = rdo::Factory<RDOTypeParamSuchAs>::create(pParam);
		ASSERT(pTypeSuchAs);
		$$ = PARSER->stack().push(pTypeSuchAs);
	}
	| RDO_such_as RDO_IDENTIF
	{
		tstring constName = RDOVALUE($2)->getIdentificator();
		LPRDOFUNConstant pConstant = PARSER->findFUNConstant(constName);
		if (!pConstant)
		{
			PARSER->error().error(@2, rdo::format(_T("������ �� �������������� ���������: %s"), constName.c_str()));
		}
		LPRDOParam pParam = pConstant.object_parent_cast<RDOParam>();
		ASSERT(pParam);
		LPRDOTypeParamSuchAs pTypeSuchAs = rdo::Factory<RDOTypeParamSuchAs>::create(pParam);
		ASSERT(pTypeSuchAs);
		$$ = PARSER->stack().push(pTypeSuchAs);
	}
	| RDO_such_as RDO_IDENTIF '.' error
	{
		tstring type = RDOVALUE($2)->getIdentificator();
		LPRDORTPResType pResType = PARSER->findRTPResType(type);
		if (!pResType)
		{
			PARSER->error().error(@2, rdo::format(_T("������ �� ����������� ��� �������: %s"), type.c_str()));
		}
		else
		{
			PARSER->error().error(@4, _T("������ ��� �������� ���������"));
		}
	}
	| RDO_such_as error
	{
		PARSER->error().error(@2, _T("����� ��������� ����� such_as ���������� ������� ��� � ��������� ������� ��� ������"));
	}
	;

param_value_default
	: /* empty */
	{
		$$ = (int)PARSER->addValue(new rdoParse::RDOValue());
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

param_value
	: RDO_INT_CONST
	{
		$$ = $1;
	}
	| RDO_REAL_CONST {
		$$ = $1;
	}
	| RDO_STRING_CONST {
		$$ = $1;
	}
	| RDO_IDENTIF {
		$$ = $1;
	}
	| RDO_BOOL_CONST {
		$$ = $1;
	}
	| param_array_value {
		$$ = $1;
	}
	;

param_type_array
	: RDO_array '<' param_type '>'
	{
		LPTypeInfo pParamType = PARSER->stack().pop<TypeInfo>($3);
		ASSERT(pParamType);
		LPRDOArrayType pArray = rdo::Factory<RDOArrayType>::create(pParamType, RDOParserSrcInfo(@1, @4));
		$$ = PARSER->stack().push(pArray);
	}
	;

param_array_value
	:	'[' array_item ']'
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($2);
		ASSERT(pArrayValue);
		$$ = (int)PARSER->addValue(new RDOValue(pArrayValue->getRArray(), RDOParserSrcInfo(@2), pArrayValue->getArrayType()->typeInfo()));
	}
	|'[' array_item error {
		PARSER->error().error(@2, _T("������ ������ ����������� �������"));
	}
	;

array_item
	: param_value
	{
		LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(RDOVALUE($1).typeInfo(), RDOParserSrcInfo(@1));
		ASSERT(pArrayType);
		LPRDOArrayValue pArrayValue = rdo::Factory<RDOArrayValue>::create(pArrayType);
		pArrayValue->insertItem(RDOVALUE($1));
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item ',' param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($3));
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($2));
		$$ = PARSER->stack().push(pArrayValue);
		PARSER->error().warning(@1, rdo::format(_T("��������� ������� �����: %s"), RDOVALUE($2)->getAsString().c_str()));
	}
	;

// ----------------------------------------------------------------------------
// ---------- ����� ��������� ������ ��� ���� �������� ���
// ----------------------------------------------------------------------------
// ---------- ���������� ���������
// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
// ---------- �������������� ���������
// ----------------------------------------------------------------------------
fun_arithm
	: RDO_INT_CONST                      { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(RDOVALUE($1))); }
	| RDO_REAL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(RDOVALUE($1))); }
	| RDO_BOOL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(RDOVALUE($1))); }
	| RDO_STRING_CONST                   { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(RDOVALUE($1))); }
	| param_array_value                  { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(RDOVALUE($1))); }
	| RDO_IDENTIF                        { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(RDOVALUE($1))); }
	| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(RDOVALUE($1), RDOVALUE($3))); }
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(RDOVALUE($1), RDOVALUE($3))); }
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
	;

// ----------------------------------------------------------------------------
// ---------- ������� � ������������������
// ----------------------------------------------------------------------------
fun_arithm_func_call
	: RDO_IDENTIF '(' ')'
	{
		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create();
		ASSERT(pFunParams);
		tstring funName = RDOVALUE($1)->getIdentificator();
		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @3);
		pFunParams->setSrcText(funName + _T("()"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' fun_arithm_func_call_pars ')'
	{
		LPRDOFUNParams pFunParams = PARSER->stack().pop<RDOFUNParams>($3);
		ASSERT(pFunParams);
		tstring funName = RDOVALUE($1)->getIdentificator();
		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @4);
		pFunParams->setSrcText(funName + _T("(") + pFunParams->src_text() + _T(")"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' error
	{
		PARSER->error().error(@3, _T("������ � ���������� �������"));
	}
	;

fun_arithm_func_call_pars
	: fun_arithm
	{
		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create();
		LPRDOFUNArithm pArithm    = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pFunParams);
		ASSERT(pArithm   );
		pFunParams->setSrcText  (pArithm->src_text());
		pFunParams->addParameter(pArithm);
		$$ = PARSER->stack().push(pFunParams);
	}
	| fun_arithm_func_call_pars ',' fun_arithm
	{
		LPRDOFUNParams pFunParams = PARSER->stack().pop<RDOFUNParams>($1);
		LPRDOFUNArithm pArithm    = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pFunParams);
		ASSERT(pArithm   );
		pFunParams->setSrcText  (pFunParams->src_text() + _T(", ") + pArithm->src_text());
		pFunParams->addParameter(pArithm);
		$$ = PARSER->stack().push(pFunParams);
	}
	| fun_arithm_func_call_pars error
	{
		PARSER->error().error(@2, _T("������ � �������������� ���������"));
	}
	| fun_arithm_func_call_pars ',' error
	{
		PARSER->error().error(@3, _T("������ � �������������� ���������"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- ��������� ���������
// ----------------------------------------------------------------------------
fun_group_keyword
	: RDO_Exist       { $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist   { $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All     { $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All { $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_group_header
	: fun_group_keyword '(' RDO_IDENTIF_COLON
	{
		PTR(RDOValue) type_name = P_RDOVALUE($3);
		$$ = PARSER->stack().push(rdo::Factory<RDOFUNGroupLogic>::create((RDOFUNGroupLogic::FunGroupType)$1, type_name->src_info()));
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

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header
	: RDO_Select '(' RDO_IDENTIF_COLON
	{
		PTR(RDOValue)  type_name = P_RDOVALUE($3);
		LPRDOFUNSelect pSelect   = rdo::Factory<RDOFUNSelect>::create(type_name->src_info());
		ASSERT(pSelect);
		pSelect->setSrcText(_T("Select(") + type_name->value().getIdentificator() + _T(": "));
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
