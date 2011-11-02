/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoevn.y
  \authors   ���� ���������
  \authors   ������ ������ (rdo@rk9.bmstu.ru)
  \authors   ����� ������� (dluschan@rk9.bmstu.ru)
  \date      12.06.2010
  \brief     ��������� �������
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
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/compiler/parser/type/range.h"
#include "simulator/compiler/parser/rdo_array.h"
#include "simulator/compiler/parser/type/such_as.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/calc/event_plan.h"
#include "simulator/runtime/calc/process_control.h"
#include "simulator/runtime/calc/braces.h"
#include "simulator/runtime/calc/statements.h"
#include "simulator/runtime/calc/array.h"
// --------------------------------------------------------------------------------

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

%%

pat_main
	: /* empty */
	| pat_main pat_pattern
	| error
	{
		PARSER->error().error(@1, _T("����������� ������"));
	}
	;

pat_header
	: RDO_Pattern RDO_IDENTIF_COLON RDO_event pat_trace
	{
		PTR(RDOValue) name = P_RDOVALUE($2);
		LPRDOPATPattern pPattern = rdo::Factory<RDOPatternEvent>::create(name->src_info(), $4 != 0);
		ASSERT(pPattern);
		$$ = PARSER->stack().push(pPattern);
	}
	| RDO_Pattern error
	{
		PARSER->error().error(@2, _T("��������� ��� �������"));
	}
	| RDO_Pattern RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("��������� ��� �������"));
	}
	;

pat_trace
	: /* empty */	{ $$ = 0; }
	| RDO_trace		{ $$ = 1; }
	| RDO_no_trace	{ $$ = 0; }
	;

pat_params_begin
	: pat_header RDO_Parameters
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		$$ = PARSER->stack().push(pPattern);
	}
	;

pat_params
	: pat_params_begin RDO_IDENTIF_COLON param_type param_value_default
	{
		LPRDOPATPattern pPattern   = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		PTR(RDOValue)   param_name = P_RDOVALUE($2);
		LPTypeInfo      pType      = PARSER->stack().pop<TypeInfo>($3);
		LPRDOParam      pParam     = rdo::Factory<RDOParam>::create(param_name->src_info(), pType, RDOVALUE($4));
		pPattern->add(pParam);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params RDO_IDENTIF_COLON param_type param_value_default
	{
		LPRDOPATPattern pPattern   = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		PTR(RDOValue)   param_name = P_RDOVALUE($2);
		LPTypeInfo      pType      = PARSER->stack().pop<TypeInfo>($3);
		LPRDOParam      pParam     = rdo::Factory<RDOParam>::create(param_name->src_info(), pType, RDOVALUE($4));
		pPattern->add(pParam);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_begin error
	{
		if (@1.m_last_line != @2.m_last_line)
		{
			PARSER->error().error(@2, _T("��������� ��� ��������� �������"));
		}
		else
		{
			PARSER->error().error(@2, rdo::format(_T("��������� ��� ��������� �������, �������: %s"), LEXER->YYText()));
		}
	}
	| pat_params_begin RDO_IDENTIF error
	{
		if (@2.m_last_line != @3.m_last_line)
		{
			PARSER->error().error(@2, @3, _T("��������� ���������"));
		}
		else
		{
			PARSER->error().error(@2, @3, rdo::format(_T("��������� ���������, �������: %s"), LEXER->YYText()));
		}
	}
	| pat_params_begin RDO_IDENTIF_COLON error
	{
		if (@2.m_last_line != @3.m_last_line)
		{
			PARSER->error().error(@2, @3, _T("��������� ��� ��������� �������"));
		}
		else
		{
			PARSER->error().error(@2, @3, rdo::format(_T("��������� ��� ��������� �������, �������: %s"), LEXER->YYText()));
		}
	}
	| pat_params error
	{
		if (@1.m_last_line != @2.m_last_line)
		{
			PARSER->error().error(@2, _T("��������� ��� ��������� �������"));
		}
		else
		{
			PARSER->error().error(@2, rdo::format(_T("��������� ��� ��������� �������, �������: %s"), LEXER->YYText()));
		}
	}
	| pat_params RDO_IDENTIF error
	{
		if (@2.m_last_line != @3.m_last_line)
		{
			PARSER->error().error(@2, @3, _T("��������� ���������"));
		}
		else
		{
			PARSER->error().error(@2, @3, rdo::format(_T("��������� ���������, �������: %s"), LEXER->YYText()));
		}
	}
	| pat_params RDO_IDENTIF_COLON error
	{
		if (@2.m_last_line != @3.m_last_line)
		{
			PARSER->error().error(@2, @3, _T("��������� ��� ��������� �������"));
		}
		else
		{
			PARSER->error().error(@2, @3, rdo::format(_T("��������� ��� ��������� �������, �������: %s"), LEXER->YYText()));
		}
	}
	;

pat_params_end
	: pat_params RDO_Relevant_resources
	{
		$$ = $1;
	}
	| pat_header RDO_Relevant_resources
	{
		$$ = $1;
	}
	| pat_header error
	{
		PARSER->error().error(@2, _T("��������� �������� ����� $Relevant_resources"));
	}
	;

pat_rel_res
	: pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv
	{
		// ��������� ��� ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@5, _T("� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv pat_conv
	{
		// ��������� ��� ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@5, _T("� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv
	{
		// ��������� ��� ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Event: 
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				pPattern->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, @4);
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv
	{
		// ��������� ��� ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Event: 
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				pPattern->addRelRes(rel_name->src_info(), type_name->src_info(), (rdoRuntime::RDOResource::ConvertStatus)$4, @4);
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv
	{
		// ��������� ��� ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, (rdoRuntime::RDOResource::ConvertStatus)$4, convertor_pos, @4);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange pat_conv
	{
		// ��������� ��� ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, (rdoRuntime::RDOResource::ConvertStatus)$4, convertor_pos, @4);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange
	{
		// ��������� ��� ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_begin_pos = @3;
				tstring str = LEXER->YYText();
				rdo::toLower(str);
				tstring::size_type first_nochange = str.find(_T("nochange"));
				ruint i = 0;
				for (;;)
				{
					if (str[i] == '\n')
					{
						convertor_begin_pos.m_first_line++;
						convertor_begin_pos.m_first_seek++;
						convertor_begin_pos.m_first_pos = 0;
					}
					else if (str[i] != '\r')
					{
						convertor_begin_pos.m_first_pos++;
						convertor_begin_pos.m_first_seek++;
					}
					i++;
					if (i == first_nochange)
						break;
				}
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				YYLTYPE convertor_end_pos = @3;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@3, _T("� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@3, _T("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange_NoChange
	{
		// ��������� ��� ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_begin_pos = @3;
				tstring str = LEXER->YYText();
				rdo::toLower(str);
				tstring::size_type first_nochange = str.find(_T("nochange"));
				ruint i = 0;
				for (;;)
				{
					if (str[i] == '\n')
					{
						convertor_begin_pos.m_first_line++;
						convertor_begin_pos.m_first_seek++;
						convertor_begin_pos.m_first_pos = 0;
					}
					else if (str[i] != '\r')
					{
						convertor_begin_pos.m_first_pos++;
						convertor_begin_pos.m_first_seek++;
					}
					i++;
					if (i == first_nochange)
						break;
				}
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				YYLTYPE convertor_end_pos = @3;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@3, _T("� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@3, _T("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange
	{
		// ��������� ��� ie,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@3, rdo::format(_T("������ ������� ���������� ������ (%s), ��������� ������ ���������� �����, ������ ��� � �������� ���� ������� �����"), RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).c_str()));
				break;
			}
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, convertor_pos);
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange
	{
		// ��������� ��� ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@3, rdo::format(_T("������ ������� ���������� ������ (%s), ��������� ������ ���������� �����, ������ ��� � �������� ���� ������� �����"), RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).c_str()));
				break;
			}
			case RDOPATPattern::PT_Event: 
			case RDOPATPattern::PT_Rule :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				YYLTYPE convertor_pos = @3;
				convertor_pos.m_first_line = convertor_pos.m_last_line;
				convertor_pos.m_first_pos  = convertor_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_pos.m_first_seek = convertor_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern->addRelRes(rel_name->src_info(), type_name->src_info(), rdoRuntime::RDOResource::CS_NoChange, convertor_pos);
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange
	{
		// ��������� ��� ie,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				tstring convert_begin = RDOVALUE($4)->getIdentificator();
				YYLTYPE convertor_begin_pos = @4;
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + convert_begin.length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + convert_begin.length();
				YYLTYPE convertor_end_pos = @4;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), pPattern->StrToStatus(convert_begin, convertor_begin_pos), rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF RDO_IDENTIF_NoChange
	{
		// ��������� ��� ie,event,rule,opr,key
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PTR(RDOValue) rel_name  = P_RDOVALUE($2);
				PTR(RDOValue) type_name = P_RDOVALUE($3);
				tstring convert_begin = RDOVALUE($4)->getIdentificator();
				YYLTYPE convertor_begin_pos = @4;
				convertor_begin_pos.m_last_line = convertor_begin_pos.m_first_line;
				convertor_begin_pos.m_last_pos  = convertor_begin_pos.m_first_pos  + convert_begin.length();
				convertor_begin_pos.m_last_seek = convertor_begin_pos.m_first_seek + convert_begin.length();
				YYLTYPE convertor_end_pos = @4;
				convertor_end_pos.m_first_line = convertor_end_pos.m_last_line;
				convertor_end_pos.m_first_pos  = convertor_end_pos.m_last_pos  - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				convertor_end_pos.m_first_seek = convertor_end_pos.m_last_seek - RDOPATPattern::StatusToStr(rdoRuntime::RDOResource::CS_NoChange).length();
				pPattern.object_static_cast<RDOPatternOperation>()->addRelRes(rel_name->src_info(), type_name->src_info(), pPattern->StrToStatus(convert_begin, convertor_begin_pos), rdoRuntime::RDOResource::CS_NoChange, convertor_begin_pos, convertor_end_pos);
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("� �������������� ������� ��� ������� �����, � ������ � ������� ������� ����������"));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_params_end error
	{
		PARSER->error().error(@2, _T("������ � �������� ����������� ��������"));
	}
	| pat_rel_res error
	{
		PARSER->error().error(@2, _T("������ � �������� ����������� ��������"));
	}
	| pat_params_end RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("��������� ��������� (��� ���� ��� �������)"));
	}
	| pat_rel_res RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, @3, _T("��������� ��������� (��� ���� ��� �������)"));
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF error
	{
		if (PARSER->getLastPATPattern()->isHaveConvertEnd())
		{
			PARSER->error().error(@3, @4, _T("��������� ������ ���������� ������"));
		}
		else
		{
			PARSER->error().error(@3, @4, _T("��������� ������ ����������"));
		}
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF error
	{
		if (PARSER->getLastPATPattern()->isHaveConvertEnd())
		{
			PARSER->error().error(@3, @4, _T("��������� ������ ���������� ������"));
		}
		else
		{
			PARSER->error().error(@3, @4, _T("��������� ������ ����������"));
		}
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error
	{
		switch (PARSER->getLastPATPattern()->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@5, _T("��������� ������ ������ (first/with_min/with_max) ��� $Body"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@5, _T("��������� ������ ������ (first/with_min/with_max) ��� $Body"));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@4, @5, rdo::format(_T("��������� ������ ���������� �����, �������: %s"), LEXER->YYText()));
				break;
			}
		}
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF pat_conv error
	{
		switch (PARSER->getLastPATPattern()->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@5, _T("��������� ������ ������ (first/with_min/with_max) ��� $Body"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@5, _T("��������� ������ ������ (first/with_min/with_max) ��� $Body"));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@4, @5, rdo::format(_T("��������� ������ ���������� �����, �������: %s"), LEXER->YYText()));
				break;
			}
		}
	}
	| pat_params_end RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error
	{
		switch (PARSER->getLastPATPattern()->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("��������� ������ ������ (first/with_min/with_max) ��� $Body"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("��������� ������ ������ (first/with_min/with_max) ��� $Body"));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@3, @4, rdo::format(_T("��������� ������ ���������� �����, �������: %s"), LEXER->YYText()));
				break;
			}
		}
	}
	| pat_rel_res RDO_IDENTIF_COLON RDO_IDENTIF_NoChange error
	{
		switch (PARSER->getLastPATPattern()->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@4, _T("��������� ������ ������ (first/with_min/with_max) ��� $Body"));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@4, _T("��������� ������ ������ (first/with_min/with_max) ��� $Body"));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@3, @4, rdo::format(_T("��������� ������ ���������� �����, �������: %s"), LEXER->YYText()));
				break;
			}
		}
	}
	;

pat_conv
	: RDO_Keep			{ $$ = rdoRuntime::RDOResource::CS_Keep;     }
	| RDO_Create		{ $$ = rdoRuntime::RDOResource::CS_Create;   }
	| RDO_Erase			{ $$ = rdoRuntime::RDOResource::CS_Erase;    }
	| RDO_NonExist		{ $$ = rdoRuntime::RDOResource::CS_NonExist; }
	;

pat_common_choice
	: pat_rel_res
	| pat_rel_res RDO_first
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		if (pPattern->getType() == RDOPATPattern::PT_Event)
		{
			PARSER->error().error(@2, _T("� �������� �� ������������ ������ ������ ����������� ��������"));
		}
		else
		{
			pPattern->setCommonChoiceFirst();
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_with_min fun_arithm
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		if (pPattern->getType() == RDOPATPattern::PT_Event)
		{
			PARSER->error().error(@2, _T("� �������� �� ������������ ������ ������ ����������� ��������"));
		}
		else
		{
			LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
			ASSERT(pArithm);
			pArithm->setSrcPos (@2, @3);
			pArithm->setSrcText(_T("with_min ") + pArithm->src_text());
			pPattern->setCommonChoiceWithMin(pArithm);
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_with_max fun_arithm
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		if ( pPattern->getType() == RDOPATPattern::PT_Event)
		{
			PARSER->error().error(@2, _T("� �������� �� ������������ ������ ������ ����������� ��������"));
		}
		else
		{
			LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
			ASSERT(pArithm);
			pArithm->setSrcPos (@2, @3);
			pArithm->setSrcText(_T("with_max ") + pArithm->src_text());
			pPattern->setCommonChoiceWithMax(pArithm);
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_rel_res RDO_with_min error
	{
		PARSER->error().error(@3, _T("������ � �������������� ���������"));
	}
	| pat_rel_res RDO_with_max error
	{
		PARSER->error().error(@3, _T("������ � �������������� ���������"));
	}
	;

pat_time
	: pat_common_choice RDO_Body
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_common_choice error
	{
		PARSER->error().error(@1, @2, _T("��������� �������� ����� $Body"));
	}
	| pat_common_choice error
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		switch (pPattern->getType())
		{
			case RDOPATPattern::PT_Rule:
			{
				PARSER->error().error(@2, rdo::format(_T("��������� $Body, �������: %s"), LEXER->YYText()));
				break;
			}
			case RDOPATPattern::PT_Event:
			{
				PARSER->error().error(@2, rdo::format(_T("��������� $Body, �������: %s"), LEXER->YYText()));
				break;
			}
			case RDOPATPattern::PT_Operation:
			case RDOPATPattern::PT_Keyboard :
			{
				PARSER->error().error(@2, rdo::format(_T("��������� $Time, �������: %s"), LEXER->YYText()));
				break;
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	;

pat_body
	: pat_time RDO_IDENTIF_RELRES
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		tstring         name     = RDOVALUE($2)->getIdentificator();
		pPattern->addRelResBody(RDOParserSrcInfo(@2, name));
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_convert RDO_IDENTIF_RELRES
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		tstring         name    = RDOVALUE($2)->getIdentificator();
		pPattern->addRelResBody(RDOParserSrcInfo(@2, name));
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_time error
	{
		tstring str(LEXER->YYText());
		PARSER->error().error(@2, rdo::format(_T("����������� ����������� ������: %s"), str.c_str()));
	}
	| pat_convert error
	{
		tstring str(LEXER->YYText());
		PARSER->error().error(@2, rdo::format(_T("����������� ����������� ������: %s"), str.c_str()));
	}
	;

pat_res_usage
	: pat_body pat_choice pat_order
	{
		LPRDOPATChoiceFrom pChoiceFrom = PARSER->stack().pop<RDOPATChoiceFrom>($2);
		ASSERT(pChoiceFrom);
		pChoiceFrom->setSrcPos(@2);

		LPRDOPATChoiceOrder pChoiceOrder = PARSER->stack().pop<RDOPATChoiceOrder>($3);
		pChoiceOrder->setSrcPos(@3);

		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		pPattern->addRelResUsage(pChoiceFrom, pChoiceOrder);
		$$ = PARSER->stack().push(pPattern);
	}
	;

pat_choice
	: /* empty */
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceEmpty;
		LPRDOPATChoiceFrom pChoiceFrom = rdo::Factory<RDOPATChoiceFrom>::create(RDOParserSrcInfo(_T("Choice NoCheck")), RDOPATChoiceFrom::ch_empty);
		ASSERT(pChoiceFrom);
		$$ = PARSER->stack().push(pChoiceFrom);
	}
	| pat_choice_nocheck
	{
		LPRDOPATChoiceFrom pChoiceFrom = rdo::Factory<RDOPATChoiceFrom>::create(RDOParserSrcInfo(_T("Choice NoCheck")), RDOPATChoiceFrom::ch_nocheck);
		ASSERT(pChoiceFrom);
		$$ = PARSER->stack().push(pChoiceFrom);
	}
	| pat_choice_from fun_logic
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		LPRDOPATChoiceFrom pChoiceFrom = rdo::Factory<RDOPATChoiceFrom>::create(RDOParserSrcInfo(_T("Choice from ") + pLogic->src_text()), RDOPATChoiceFrom::ch_from, pLogic);
		ASSERT(pChoiceFrom);
		$$ = PARSER->stack().push(pChoiceFrom);
	}
	| pat_choice_from error
	{
		PARSER->error().error(@2, _T("������ � ���������� ���������"));
	}
	;

pat_choice_nocheck
	: RDO_Choice RDO_NoCheck
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceNoCheck;
	}
	;

pat_choice_from
	: RDO_Choice RDO_from
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceFrom;
	}
	;

pat_order
	: /* empty */
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceOrderEmpty;
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(), rdoRuntime::RDOSelectResourceCalc::order_empty);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_first
	{
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(_T("first")), rdoRuntime::RDOSelectResourceCalc::order_first);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_with_min fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(_T("with_min ") + pArithm->src_text()), rdoRuntime::RDOSelectResourceCalc::order_with_min, pArithm);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_with_max fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		LPRDOPATChoiceOrder pChoiceOrder = rdo::Factory<RDOPATChoiceOrder>::create(RDOParserSrcInfo(_T("with_max ") + pArithm->src_text()), rdoRuntime::RDOSelectResourceCalc::order_with_max, pArithm);
		ASSERT(pChoiceOrder);
		$$ = PARSER->stack().push(pChoiceOrder);
	}
	| pat_choice_with_min error
	{
		PARSER->error().error(@2, _T("������ � �������������� ���������"));
	}
	| pat_choice_with_max error
	{
		PARSER->error().error(@2, _T("������ � �������������� ���������"));
	}
	;

pat_choice_first
	: RDO_first
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceOrderFirst;
	}
	;

pat_choice_with_min
	: RDO_with_min
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceOrderWithMin;
	}
	;

pat_choice_with_max
	: RDO_with_max
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::choiceOrderWithMax;
	}
	;

pat_convert
	: pat_res_usage
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		LPRDORelevantResource rel_res  = pPattern->m_pCurrRelRes;
		tstring str;
		if (rel_res->m_pChoiceOrder->m_type != rdoRuntime::RDOSelectResourceCalc::order_empty)
		{
			str = _T("����� ����� ��������� ����� ") + rel_res->m_pChoiceOrder->asString();
		}
		else if (rel_res->m_pChoiceFrom->m_type != RDOPATChoiceFrom::ch_empty)
		{
			str = _T("����� ����� ������� ������");
		}
		else
		{
			str = _T("����� ����� �����");
		}
		if (rel_res->m_statusBegin != rdoRuntime::RDOResource::CS_NoChange && rel_res->m_statusBegin != rdoRuntime::RDOResource::CS_Erase && rel_res->m_statusBegin != rdoRuntime::RDOResource::CS_NonExist)
		{
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Event:
				{
					PARSER->error().error(@1, rdo::format(_T("%s ��������� �������� ����� Convert_event ��� ������������ ������� '%s', �.�. ��� ������ '%s', �� �������: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->m_statusBegin).c_str(), LEXER->YYText()));
					break;
				}
				case RDOPATPattern::PT_Rule:
				{
					PARSER->error().error(@1, rdo::format(_T("%s ��������� �������� ����� Convert_rule ��� ������������ ������� '%s', �.�. ��� ������ '%s', �� �������: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->m_statusBegin).c_str(), LEXER->YYText()));
					break;
				}
				case RDOPATPattern::PT_Operation:
				case RDOPATPattern::PT_Keyboard :
				{
					PARSER->error().error(@1, rdo::format(_T("%s ��������� �������� ����� Convert_begin ��� ������������ ������� '%s', �.�. ��� ������ '%s', �� �������: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->m_statusBegin).c_str(), LEXER->YYText()));
					break;
				}
			}
		}
		if (rel_res->m_statusEnd != rdoRuntime::RDOResource::CS_NoChange && rel_res->m_statusEnd != rdoRuntime::RDOResource::CS_Erase && rel_res->m_statusEnd != rdoRuntime::RDOResource::CS_NonExist)
		{
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Event:
				case RDOPATPattern::PT_Rule:
				{
					PARSER->error().error(@1, _T("���������� ������"));
					break;
				}
				case RDOPATPattern::PT_Operation:
				case RDOPATPattern::PT_Keyboard :
				{
					PARSER->error().error(@1, rdo::format(_T("%s ��������� �������� ����� Convert_end ��� ������������ ������� '%s', �.�. ��� ������ '%s', �� �������: %s"), str.c_str(), rel_res->name().c_str(), RDOPATPattern::StatusToStr(rel_res->m_statusBegin).c_str(), LEXER->YYText()));
					break;
				}
			}
		}
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_begin pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Operation && pPattern->getType() != RDOPATPattern::PT_Keyboard)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Event:
				{
					type = _T("�������"); 
					break;
				}
				case RDOPATPattern::PT_Rule:
				{
					type = _T("������������� �������");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("�������� ����� Convert_begin ����� ���� ������������ � ������������ ��� ������������ ��������, �� �� � %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($4);
		pPattern.object_static_cast<RDOPatternOperation>()->addRelResConvertBeginEnd($3 != 0, pCmdList, false, NULL, @2, @2, @3, @3);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_end pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Operation && pPattern->getType() != RDOPATPattern::PT_Keyboard)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Event:
				{
					type = _T("�������");
					break;
				}
				case RDOPATPattern::PT_Rule:
				{
					type = _T("������������� �������");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("�������� ����� Convert_end ����� ���� ������������ � ������������ � ������������ ��������, �� �� � %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($4);
		pPattern.object_static_cast<RDOPatternOperation>()->addRelResConvertBeginEnd(false, NULL, $3 != 0, pCmdList, @2, @2, @3, @3);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_begin pat_trace pat_convert_cmd convert_end pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Operation && pPattern->getType() != RDOPATPattern::PT_Keyboard)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Event:
				{
					type = _T("�������");
					break;
				}
				case RDOPATPattern::PT_Rule:
				{
					type = _T("������������� �������");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("�������� ����� Convert_begin � Convert_end ����� ���� ������������ � ������������ � ������������ ��������, �� �� � %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdListBegin = PARSER->stack().pop<ConvertCmdList>($4);
		LPConvertCmdList pCmdListEnd   = PARSER->stack().pop<ConvertCmdList>($7);
		pPattern.object_static_cast<RDOPatternOperation>()->addRelResConvertBeginEnd($3 != 0, pCmdListBegin, $6 != 0, pCmdListEnd, @2, @5, @3, @6);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_rule pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		if (pPattern->getType() != RDOPATPattern::PT_Rule)
		{
			tstring type = _T("");
			switch (pPattern->getType())
			{
				case RDOPATPattern::PT_Event:
				{
					type = _T("�������");
					break;
				}
				case RDOPATPattern::PT_Operation:
				{
					type = _T("��������");
					break;
				}
				case RDOPATPattern::PT_Keyboard :
				{
					type = _T("������������ ��������");
					break;
				}
			}
			PARSER->error().error(@2, rdo::format(_T("�������� ����� Convert_rule ����� ���� ������������ � ������������� �������, �� �� � %s '%s'"), type.c_str(), pPattern->name().c_str()));
		}
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($4);
		ASSERT(pPattern->m_pCurrRelRes);
		pPattern->addRelResConvert($3 != 0, pCmdList, @2, @3, pPattern->m_pCurrRelRes->m_statusBegin);
		$$ = PARSER->stack().push(pPattern);
	}
	| pat_res_usage convert_event pat_trace pat_convert_cmd
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		ASSERT(pPattern);
		LPConvertCmdList pCmdList = PARSER->stack().pop<ConvertCmdList>($4);
		ASSERT(pPattern->m_pCurrRelRes);
		pPattern->addRelResConvert($3 != 0, pCmdList, @2, @3, pPattern->m_pCurrRelRes->m_statusBegin);
		$$ = PARSER->stack().push(pPattern);
	}
	;

convert_rule
	: RDO_Convert_rule
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertBegin;
	}
	;

convert_event
	: RDO_Convert_event
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertBegin;
	}
	;

convert_begin
	: RDO_Convert_begin
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertBegin;
	}
	;

convert_end
	:	RDO_Convert_end
	{
		PARSER->getLastPATPattern()->m_pCurrRelRes->m_currentState = RDORelevantResource::convertEnd;
	}
	;

pat_convert_cmd
	: /* empty */
	{
		LPConvertCmdList pCmdList = rdo::Factory<ConvertCmdList>::create();
		LPRDORelevantResource pRelRes = PARSER->getLastPATPattern()->m_pCurrRelRes;
		ASSERT(pRelRes);
		$$ = PARSER->stack().push(pCmdList);
	}
	| pat_convert_cmd statement
	{
		LPConvertCmdList      pCmdList = PARSER->stack().pop<ConvertCmdList>($1);
		rdoRuntime::LPRDOCalc pCalc    = PARSER->stack().pop<rdoRuntime::RDOCalc>($2);
		pCmdList->insertCommand(pCalc);
		$$ = PARSER->stack().push(pCmdList);
	}
	;

statement
	: empty_statement
	| nochange_statement
	| equal_statement
	| stopping_statement
	| planning_statement
	| process_input_statement
	| if_statement
	| '{' statement_list '}'
	{
		rdoRuntime::LPRDOCalcBodyBrace pCalcBodyBrace = PARSER->stack().pop<rdoRuntime::RDOCalcBodyBrace>($2);
		ASSERT(pCalcBodyBrace);
		rdoRuntime::LPRDOCalc pCalc = pCalcBodyBrace;
		ASSERT(pCalc);
		$$ = PARSER->stack().push(pCalc);
	}
	;

statement_list
	: /* empty */
	{
		rdoRuntime::LPRDOCalcBodyBrace pCalcBodyBrace = rdo::Factory<rdoRuntime::RDOCalcBodyBrace>::create();
		ASSERT(pCalcBodyBrace);
		$$ = PARSER->stack().push(pCalcBodyBrace);
	}
	| statement_list statement
	{
		rdoRuntime::LPRDOCalcBodyBrace pCalcBodyBrace = PARSER->stack().pop<rdoRuntime::RDOCalcBodyBrace>($1);
		ASSERT(pCalcBodyBrace);

		rdoRuntime::LPRDOCalc     pCalc     = PARSER->stack().pop<rdoRuntime::RDOCalc>($2);
		ASSERT(pCalc);

		pCalcBodyBrace->addCalc(pCalc);

		$$ = PARSER->stack().push(pCalcBodyBrace);
	}
	;

empty_statement
	: ';'
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcNoChange>::create();
		ASSERT(pCalc);
		$$ = PARSER->stack().push(pCalc);
	}
	| error ';'
	{
		PARSER->error().error(@1, _T("������ � ����������"));
	}
	;

nochange_statement
	: RDO_IDENTIF_NoChange ';'
	{
		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcNoChange>::create();
		ASSERT(pCalc);
		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF_NoChange error
	{
		PARSER->error().error(@2, _T("�� ������ ������ ��������� ���������� - ����� � �������"));
	}
	;

equal_statement
	: RDO_IDENTIF increment_or_decrement_type ';'
	{
		tstring               paramName = RDOVALUE($1)->getIdentificator();
		rdoRuntime::EqualType equalType = static_cast<rdoRuntime::EqualType>($2);
		LPRDORelevantResource pRelRes   = PARSER->getLastPATPattern()->m_pCurrRelRes;
		ASSERT(pRelRes);
		LPRDORTPParam pParam = pRelRes->getType()->findRTPParam(paramName);
		if (!pParam)
		{
			PARSER->error().error(@1, rdo::format(_T("����������� ��������: %s"), paramName.c_str()));
		}
		rdoRuntime::LPRDOCalc pCalc;
		switch (equalType)
		{
			case rdoRuntime::ET_INCR:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelResParamCalc<rdoRuntime::ET_INCR> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName));
				break;
			}
			case rdoRuntime::ET_DECR:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelResParamCalc<rdoRuntime::ET_DECR> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName));
				break;
			}
			default:
			{
				NEVER_REACH_HERE;
			}
		}
		ASSERT(pCalc);
		//! �������� �� ��������
		/// @todo ��������� �����������������
		if (dynamic_cast<PTR(RDOTypeIntRange)>(pParam->getTypeInfo().get()))
		{
			LPRDOTypeIntRange pRange = pParam->getTypeInfo()->type().object_static_cast<RDOTypeIntRange>();
			pCalc = rdo::Factory<rdoRuntime::RDOSetRelResParamDiapCalc>::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pRange->range()->getMin().value(), pRange->range()->getMax().value(), pCalc);
		}
		else if (dynamic_cast<PTR(RDOTypeRealRange)>(pParam->getTypeInfo().get()))
		{
			LPRDOTypeRealRange pRange = pParam->getTypeInfo()->type().object_static_cast<RDOTypeRealRange>();
			pCalc = rdo::Factory<rdoRuntime::RDOSetRelResParamDiapCalc>::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pRange->range()->getMin().value(), pRange->range()->getMax().value(), pCalc);
		}
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

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF param_equal_type fun_arithm ';'
	{
		tstring               paramName    = RDOVALUE($1)->getIdentificator();
		rdoRuntime::EqualType equalType    = static_cast<rdoRuntime::EqualType>($2);
		LPRDOFUNArithm        pRightArithm = PARSER->stack().pop<RDOFUNArithm>($3);
		LPRDORelevantResource pRelRes      = PARSER->getLastPATPattern()->m_pCurrRelRes;
		ASSERT(pRelRes);
		LPRDORTPParam pParam = pRelRes->getType()->findRTPParam(paramName);
		if (!pParam)
		{
			PARSER->error().error(@1, rdo::format(_T("����������� ��������: %s"), paramName.c_str()));
		}
		rdoRuntime::LPRDOCalc pCalcRight = pRightArithm->createCalc(pParam->getTypeInfo());
		rdoRuntime::LPRDOCalc pCalc;
		switch (equalType)
		{
			case rdoRuntime::ET_NOCHANGE:
			{
				break;
			}
			case rdoRuntime::ET_EQUAL:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelResParamCalc<rdoRuntime::ET_EQUAL> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				ASSERT(pCalc);
				pCalc->setSrcText(rdo::format(_T("%s.%s"), pRelRes->src_text().c_str(), paramName.c_str()));
				pCalc->setSrcPos(@1.m_first_line, @1.m_first_pos, @1.m_last_line, @1.m_last_pos);

				LPExpression pExpressionLeft = rdo::Factory<Expression>::create(
					pParam->getTypeInfo(),
					pCalc,
					pCalc->src_info()
				);
				ASSERT(pExpressionLeft);

				LPRDOFUNArithm pArithmLeft = rdo::Factory<RDOFUNArithm>::create(pExpressionLeft);
				ASSERT(pArithmLeft);
				pArithmLeft->setEqual(pRightArithm);

				pRelRes->getParamSetList().insert(pParam);
				break;
			}
			case rdoRuntime::ET_PLUS:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelResParamCalc<rdoRuntime::ET_PLUS> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				break;
			}
			case rdoRuntime::ET_MINUS:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelResParamCalc<rdoRuntime::ET_MINUS> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				break;
			}
			case rdoRuntime::ET_MULTIPLY:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelResParamCalc<rdoRuntime::ET_MULTIPLY> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				break;
			}
			case rdoRuntime::ET_DIVIDE:
			{
				pCalc = rdo::Factory<rdoRuntime::RDOSetRelResParamCalc<rdoRuntime::ET_DIVIDE> >::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pCalcRight);
				break;
			}
			default:
			{
				NEVER_REACH_HERE;
			}
		}
		ASSERT(pCalc);
		//! �������� �� ��������
		/// @todo ��������� �����������������
		if (dynamic_cast<PTR(RDOTypeIntRange)>(pParam->getTypeInfo().get()))
		{
			LPRDOTypeIntRange pRange = pParam->getTypeInfo()->type().object_static_cast<RDOTypeIntRange>();
			pCalc = rdo::Factory<rdoRuntime::RDOSetRelResParamDiapCalc>::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pRange->range()->getMin().value(), pRange->range()->getMax().value(), pCalc);
		}
		else if (dynamic_cast<PTR(RDOTypeRealRange)>(pParam->getTypeInfo().get()))
		{
			LPRDOTypeRealRange pRange = pParam->getTypeInfo()->type().object_static_cast<RDOTypeRealRange>();
			pCalc = rdo::Factory<rdoRuntime::RDOSetRelResParamDiapCalc>::create(pRelRes->m_relResID, pRelRes->getType()->getRTPParamNumber(paramName), pRange->range()->getMin().value(), pRange->range()->getMax().value(), pCalc);
		}
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

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF param_equal_type error
	{
		PARSER->error().error(@3, _T("������ � �������������� ���������"));
	}
	| RDO_IDENTIF param_equal_type fun_arithm error
	{
		PARSER->error().error(@4, _T("�� ������ ������ ��������� ���������� - ����� � �������"));
	}
	| RDO_IDENTIF error fun_arithm
	{
		PARSER->error().error(@2, _T("������ � ��������� ������������"));
	}
	;

stopping_statement
	: RDO_IDENTIF '.' RDO_Stopping '(' ')' ';'
	{
		tstring           eventName   = RDOVALUE($1)->getIdentificator();
		LPRDOEvent        pEvent      = PARSER->findEvent(eventName);
		if (!pEvent)
		{
			PARSER->error().error(@1, rdo::format(_T("������� ���������� ����������� �������: %s"), eventName.c_str()));
		}

		rdoRuntime::LPRDOCalcEventStop pCalc = rdo::Factory<rdoRuntime::RDOCalcEventStop>::create();
		ASSERT(pCalc);
		pEvent->attachCalc(pCalc);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF '.' RDO_Stopping '(' ')' error
	{
		PARSER->error().error(@4, _T("�� ������ ������ ��������� ���������� - ����� � �������"));
	}
	;

process_input_statement
	: RDO_IDENTIF '.' RDO_ProcessStart '(' RDO_IDENTIF_RELRES ')' ';'
	{
		tstring          processName  = RDOVALUE($1)->getIdentificator();
		LPRDOPROCProcess pProcess     = PARSER->findPROCProcess(processName);
		if (!pProcess)
		{
			PARSER->error().error(@1, rdo::format(_T("������� ��������� ����������� �������: %s"), processName.c_str()));
		}

		LPIPROCBlock pBlock = (*(pProcess->getBlockList().begin()))->getRuntimeBlock();
		ASSERT(pBlock);

		tstring relResName = RDOVALUE($5)->getIdentificator();

		LPRDOPATPattern pPattern = PARSER->getLastPATPattern();
		ASSERT(pPattern);
		/*��-�� ������������� RDO_IDENTIF_RELRES findRelevantResource() ������ ������� ������*/
		LPRDORelevantResource pRelRes = pPattern->findRelevantResource(relResName);
		tstring relResTypeName = pRelRes->getType()->name();

		if (!pProcess->checkTransactType(relResTypeName))
		{
			PARSER->error().error(@1, rdo::format(_T("������� %s ������� � �������� ���������� ������� ���� %s, � �� %s"), processName.c_str(), _T("true_resTypeName"), relResTypeName.c_str()));
		}

		rdoRuntime::LPRDOCalcProcessControl pCalc = rdo::Factory<rdoRuntime::RDOCalcProcessControl>::create(pBlock, pRelRes->m_relResID);
		ASSERT(pCalc);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF '.' RDO_ProcessStart '(' error ')' ';'
	{
		PARSER->error().error(@5, _T("� �������� ��������� �������� ����� ���������� ������ ���������� ������"));
	}
	;

planning_statement
	: RDO_IDENTIF '.' RDO_Planning '(' fun_arithm ')' '(' arithm_list ')' ';'
	{
		tstring        eventName   = RDOVALUE($1)->getIdentificator();
		LPRDOFUNArithm pTimeArithm = PARSER->stack().pop<RDOFUNArithm>($5);
		LPRDOEvent     pEvent      = PARSER->findEvent(eventName);
		if (!pEvent)
		{
			PARSER->error().error(@1, rdo::format(_T("������� ������������� ����������� �������: %s"), eventName.c_str()));
		}
		LPArithmContainer pParamList = PARSER->stack().pop<ArithmContainer>($8);
		PARSER->findEvent(eventName)->addParamList(pParamList);

		rdoRuntime::LPRDOCalc pCalcTime = pTimeArithm->createCalc(NULL);
		ASSERT(pCalcTime);

		rdoRuntime::LPRDOCalcEventPlan pCalc = rdo::Factory<rdoRuntime::RDOCalcEventPlan>::create(pCalcTime);
		ASSERT(pCalc);
		pEvent->attachCalc(pCalc);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_IDENTIF '.' RDO_Planning '(' fun_arithm ')' '(' arithm_list ')' error
	{
		PARSER->error().error(@7, _T("�� ������ ������ ��������� ���������� - ����� � �������"));
	}
	| RDO_IDENTIF '.' RDO_Planning '(' error
	{
		PARSER->error().error(@5, _T("������ � �������������� ���������"));
	}
	| RDO_IDENTIF '.' RDO_Planning error
	{
		PARSER->error().error(@4, _T("��������� ����������� ������"));
	}
	| RDO_IDENTIF '.' RDO_Planning '(' fun_arithm')' '(' arithm_list error
	{
		PARSER->error().error(@6, _T("��������� ����������� ������"));
	}
	;

if_statement
	: RDO_if '(' fun_logic ')' statement
	{
		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pCondition);

		rdoRuntime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);

		rdoRuntime::LPRDOCalc pStatementCalc = PARSER->stack().pop<rdoRuntime::RDOCalc>($5);
		ASSERT(pStatementCalc);

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcIf>::create(pConditionCalc, pStatementCalc);
		ASSERT(pCalc);

		$$ = PARSER->stack().push(pCalc);
	}
	| RDO_if '(' fun_logic ')' statement RDO_else statement
	{
		LPRDOFUNLogic pCondition = PARSER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pCondition);
		
		rdoRuntime::LPRDOCalc pConditionCalc = pCondition->getCalc();
		ASSERT(pConditionCalc);

		rdoRuntime::LPRDOCalc pIfStatementCalc = PARSER->stack().pop<rdoRuntime::RDOCalc>($5);
		ASSERT(pIfStatementCalc);

		rdoRuntime::LPRDOCalc pElseStatementCalc = PARSER->stack().pop<rdoRuntime::RDOCalc>($7);
		ASSERT(pElseStatementCalc);

		rdoRuntime::LPRDOCalc pCalc = rdo::Factory<rdoRuntime::RDOCalcIfElse>::create(pConditionCalc, pIfStatementCalc, pElseStatementCalc);
		ASSERT(pCalc);
		
		$$ = PARSER->stack().push(pCalc);
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

pat_pattern
	: pat_convert RDO_End
	{
		LPRDOPATPattern pPattern = PARSER->stack().pop<RDOPATPattern>($1);
		pPattern->end();
		$$ = PARSER->stack().push(pPattern);
	}
	;

// --------------------------------------------------------------------------------
// -------------------- �������� ���� ���������
// --------------------------------------------------------------------------------
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

param_type_array
	: RDO_array '<' param_type '>'
	{
		LPTypeInfo pParamType = PARSER->stack().pop<TypeInfo>($3);
		ASSERT(pParamType);
		LPRDOArrayType pArray = rdo::Factory<RDOArrayType>::create(pParamType, RDOParserSrcInfo(@1, @4));
		$$ = PARSER->stack().push(pArray);
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
		$$ = (int)PARSER->addValue(new RDOValue(pArrayValue));
	}
	| '[' array_item error
	{
		PARSER->error().error(@2, _T("������ ������ ����������� �������"));
	}
	;

array_item
	: param_value
	{
		LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(RDOVALUE($1).typeInfo(), RDOParserSrcInfo(@1));
		ASSERT(pArrayType);
		LPRDOArrayValue pArrayValue = rdo::Factory<RDOArrayValue>::create(pArrayType);
		ASSERT(pArrayValue);
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
	| RDO_IDENTIF '.' RDO_Size
	{
		LPRDOFUNArithm pArithm = RDOFUNArithm::generateByIdentificator(RDOVALUE($1));
		rdoRuntime::LPRDOCalc pCalc;
		if(pArithm->typeID() == rdoRuntime::RDOType::t_array)
		{
			pCalc = rdo::Factory<rdoRuntime::RDOCalcArraySize>::create(pArithm->calc());
			ASSERT(pCalc);
		}
		else
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' �� �������� ��������."), P_RDOVALUE($1)->value().getIdentificator().c_str()));
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
		LPRDOFUNArithm pArithm = RDOFUNArithm::generateByIdentificator(RDOVALUE($1));
		LPRDOFUNArithm pArithmInd = PARSER->stack().pop<RDOFUNArithm>($3);
		if (pArithm->typeID() != rdoRuntime::RDOType::t_array)
		{
			PARSER->error().error(@1, rdo::format(_T("'%s' �� �������� ��������."), P_RDOVALUE($1)->value().getIdentificator().c_str()));
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
		tstring funName                    = RDOVALUE($1)->getIdentificator();
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
	{};

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

// --------------------------------------------------------------------------------
// -------------------- Select
// --------------------------------------------------------------------------------
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
