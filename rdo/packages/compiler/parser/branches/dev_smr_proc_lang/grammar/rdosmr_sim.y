/*!
  \copyright (c) RDO-Team, 2011
  \file      rdosmr_sim.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     Синтаксис описания информации о прогоне (режим анимации, инициализация списка событий, терминальное условие и т.д.)
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
%token RDO_Run_Count

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
%token RDO_Multithreading

%token RDO_Frame
%token RDO_Back_picture
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
%token RDO_Select
%token RDO_Size
%token RDO_Empty
%token RDO_Select_Array
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

%token RDO_Sprite
%token RDO_sprite_call

%{
// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdosmr.h"
#include "simulator/compiler/parser/rdofun.h"
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/type/range.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/calc/calc_event.h"
#include "simulator/runtime/calc/calc_array.h"
#include "simulator/runtime/calc/resource/calc_resource.h"
// --------------------------------------------------------------------------------

#define PARSER  LEXER->parser()

OPEN_RDO_PARSER_NAMESPACE
%}

%left RDO_or
%left RDO_and
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start smr_main

%%

// --------------------------------------------------------------------------------
// --------------------smr_main
// --------------------------------------------------------------------------------
smr_main
	: smr_launch
	| smr_launch_set
	;

smr_launch_set
	: /* empty */
	| smr_launch_set smr_launch_single_of_set
	;

smr_launch_single_of_set
	: '{' smr_launch '}'
	{
		PARSER->pushToContainer();
		PARSER->foundEndOfNextRun();
	}
	| '{' smr_launch error
	{
		PARSER->error().error(@3, rdo::format("Очередной прогон из серии должен заканчиваться фигурной скобкой"));
	}
	| error
	{
		PARSER->error().error(@1, rdo::format("Очередной прогон из серии должен начинаться с фигурной скобки"));
	}
	;

smr_launch
	: /* empty */
	| smr_launch smr_launch_line
	;

smr_launch_line
	: smr_launch_line_terminate_if
	| smr_launch_line_break_point
	| smr_launch_line_event_planning
	| smr_launch_line_run_startTime
	| smr_launch_line_trace_endTime
	| smr_launch_line_trace_startTime
	| smr_launch_line_arithm
	| smr_launch_line_show_mode
	| smr_launch_line_show_rate
	| smr_launch_line_frame_number
	| smr_launch_line_seed
	| smr_launch_line_external_model
	| error
	{
		PARSER->error().error(@1, rdo::format("Неизвестная инструкция"));
	}
	;

smr_launch_line_event_planning
	: RDO_IDENTIF '.' RDO_Planning '(' arithm_list ')'
	{
		if (PARSER->check())
		{
			tstring    eventName          = PARSER->stack().pop<RDOValue>($2)->value().getIdentificator();
			LPArithmContainer pArithmList = PARSER->stack().pop<ArithmContainer>($5);
			LPRDOEvent pEvent             = PARSER->findEvent(eventName);
			if (!pEvent)
			{
				PARSER->error().error(@2, rdo::format("Попытка запланировать неизвестное событие: %s", eventName.c_str()));
			}

			ArithmContainer::Container::const_iterator arithmIt = pArithmList->getContainer().begin();
			if (arithmIt == pArithmList->getContainer().end())
			{
				PARSER->error().error(@1, rdo::format("Не указано время планирования события: %s", eventName.c_str()));
			}

			LPRDOFUNArithm pTimeArithm = *arithmIt;
			ASSERT(pTimeArithm);
			++arithmIt;

			LPArithmContainer pParamList = rdo::Factory<ArithmContainer>::create();
			ASSERT(pParamList);

			while (arithmIt != pArithmList->getContainer().end())
			{
				pParamList->addItem(*arithmIt);
				++arithmIt;
			}

			rdo::runtime::LPRDOCalc pCalcTime = pTimeArithm->createCalc();
			pCalcTime->setSrcInfo(pTimeArithm->src_info());
			ASSERT(pCalcTime);

			//
			//LPIBaseOperation pBaseOperation = pEvent->getRuntimeEvent();
			//ASSERT(pBaseOperation);

			rdo::runtime::LPRDOCalcEventPlan pEventPlan = rdo::Factory<rdo::runtime::RDOCalcEventPlan>::create(pCalcTime);
			ASSERT(pEventPlan);
			pEventPlan->setSrcInfo(RDOParserSrcInfo(@1, @5, rdo::format("Планирование события %s в момент времени %s", eventName.c_str(), pCalcTime->srcInfo().src_text().c_str())));

			pEvent->setParamList(pParamList);
			//pEventPlan->setEvent(pBaseOperation); связывание событий произойдет в castInitToRuntime
			pEvent->attachCalc(pEventPlan);

			LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
			ASSERT(pSMR);
			pSMR->pushEventPlan(pEventPlan);
			//pEvent->setInitCalc(pEventPlan); сохранить калк, который выполняет планирование событий перед запуском в RDOSMR
		}
	}
	;

smr_launch_line_external_model
	: RDO_External_Model RDO_IDENTIF '=' RDO_IDENTIF
	{
#ifndef CORBA_ENABLE
		PARSER->error().error(@1, @4, "Данная версия РДО не поддерживает CORBA");
#endif
	}
	;

smr_launch_line_show_mode
	: RDO_Show_mode                  '=' smr_show_mode
	{
		LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
		ASSERT(pSMR);
		pSMR->setShowMode((rdo::service::simulation::ShowMode)$3);
	}
	| RDO_Show_mode '=' error
	{
		PARSER->error().error(@2, @3, "Ожидается режим анимации");
	}
	| RDO_Show_mode error
	{
		PARSER->error().error(@2, "Ожидается '='");
	}
	;

smr_launch_line_frame_number
	: RDO_Frame_number '=' RDO_INT_CONST
	{
		if (PARSER->check())
		{
			LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
			ASSERT(pSMR);
			pSMR->setFrameNumber(PARSER->stack().pop<RDOValue>($3)->value().getInt(), @3);
		}
	}
	| RDO_Frame_number '=' error
	{
		PARSER->error().error(@2, @3, "Ожидается начальный номер кадра");
	}
	| RDO_Frame_number error
	{
		PARSER->error().error(@2, "Ожидается '='");
	}
	;

smr_launch_line_show_rate
	: RDO_Show_rate '=' RDO_REAL_CONST
	{
		LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
		ASSERT(pSMR);
		pSMR->setShowRate(PARSER->stack().pop<RDOValue>($3)->value().getDouble(), @3);
	}
	| RDO_Show_rate '=' RDO_INT_CONST
	{
		LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
		ASSERT(pSMR);
		pSMR->setShowRate(PARSER->stack().pop<RDOValue>($3)->value().getInt(), @3);
	}
	| RDO_Show_rate '=' error
	{
		PARSER->error().error(@2, @3, "Ожидается масштабный коэффициент");
	}
	| RDO_Show_rate error
	{
		PARSER->error().error(@1, "Ожидается '='");
	}
	;

smr_launch_line_run_startTime
	: RDO_Run_StartTime '=' RDO_REAL_CONST
	{
		if (PARSER->check())
		{
			LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
			ASSERT(pSMR);
			pSMR->setRunStartTime(PARSER->stack().pop<RDOValue>($3)->value().getDouble(), @3);
		}
	}
	| RDO_Run_StartTime '=' RDO_INT_CONST
	{
		if (PARSER->check())
		{
			LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
			ASSERT(pSMR);
			pSMR->setRunStartTime(PARSER->stack().pop<RDOValue>($3)->value().getInt(), @3);
		}
	}
	| RDO_Run_StartTime '=' error
	{
		PARSER->error().error(@2, @3, "Ожидается начальное модельное время");
	}
	| RDO_Run_StartTime error
	{
		PARSER->error().error(@1, "Ожидается '='");
	}
	;

smr_launch_line_trace_startTime
	: RDO_Trace_StartTime '=' RDO_REAL_CONST
	{
		LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
		ASSERT(pSMR);
		pSMR->setTraceStartTime(PARSER->stack().pop<RDOValue>($3)->value().getDouble(), @3);
	}
	| RDO_Trace_StartTime '=' RDO_INT_CONST
	{
		if (PARSER->check())
		{
			LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
			ASSERT(pSMR);
			pSMR->setTraceStartTime(PARSER->stack().pop<RDOValue>($3)->value().getInt(), @3);
		}
	}
	| RDO_Trace_StartTime '=' error
	{
		PARSER->error().error(@2, @3, "Ожидается начальное время трассировки");
	}
	| RDO_Trace_StartTime error
	{
		PARSER->error().error(@1, "Ожидается '='");
	}
	;

smr_launch_line_trace_endTime
	: RDO_Trace_EndTime '=' RDO_REAL_CONST
	{
		if (PARSER->check())
		{
			LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
			ASSERT(pSMR);
			pSMR->setTraceEndTime(PARSER->stack().pop<RDOValue>($3)->value().getDouble(), @3);
		}
	}
	| RDO_Trace_EndTime '=' RDO_INT_CONST
	{
		if (PARSER->check())
		{
			LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
			ASSERT(pSMR);
			pSMR->setTraceEndTime(PARSER->stack().pop<RDOValue>($3)->value().getInt(), @3);
		}
	}
	| RDO_Trace_EndTime '=' error
	{
		PARSER->error().error(@2, @3, "Ожидается конечное время трассировки");
	}
	| RDO_Trace_EndTime error
	{
		PARSER->error().error(@1, "Ожидается '='");
	}
	;

smr_launch_line_terminate_if
	: RDO_Terminate_if fun_logic
	{
		if (PARSER->check())
		{
			LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
			ASSERT(pLogic);

			LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
			ASSERT(pSMR);

			pSMR->setTerminateIf(pLogic);
		}
	}
	| RDO_Terminate_if error
	{
		PARSER->error().error(@1, @2, "Ошибка логического выражения в терминальном условии");
	}
	;

smr_launch_line_break_point
	: RDO_Break_point RDO_IDENTIF fun_logic
	{
		if (PARSER->check())
		{
			LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($3);
			ASSERT(pLogic);

			LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
			ASSERT(pSMR);

			pSMR->insertBreakPoint(PARSER->stack().pop<RDOValue>($2)->src_info(), pLogic);
		}
	}
	| RDO_Break_point RDO_IDENTIF error
	{
		PARSER->error().error(@3, "Ошибка логического выражения в точке останова");
	}
	| RDO_Break_point error
	{
		PARSER->error().error(@1, @2, "Ожидается имя точки останова");
	}
	;

smr_launch_line_arithm
	: RDO_IDENTIF '=' fun_arithm
	{
		if (PARSER->check())
		{
			LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($3);
			ASSERT(pArithm);

			LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
			ASSERT(pSMR);

			pSMR->setConstValue(PARSER->stack().pop<RDOValue>($1)->src_info(), pArithm);
		}
	}
	| RDO_IDENTIF '=' error
	{
		PARSER->error().error(@2, @3, "Ошибка в арифметическом выражении");
	}
	| RDO_IDENTIF error
	{
		PARSER->error().error(@1, "Ожидается '='");
	}
	| RDO_IDENTIF '.' RDO_IDENTIF '=' fun_arithm
	{
		if (PARSER->check())
		{
			LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($5);
			ASSERT(pArithm);

			LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
			ASSERT(pSMR);

			pSMR->setResParValue(PARSER->stack().pop<RDOValue>($1)->src_info(), PARSER->stack().pop<RDOValue>($3)->src_info(), pArithm);
		}
	}
	| RDO_IDENTIF '.' RDO_IDENTIF '=' error
	{
		PARSER->error().error(@4, @5, "Ошибка в арифметическом выражении");
	}
	| RDO_IDENTIF '.' RDO_IDENTIF error
	{
		PARSER->error().error(@3, "Ожидается '='");
	}
	| RDO_IDENTIF '.' error
	{
		tstring name = PARSER->stack().pop<RDOValue>($1)->value().getIdentificator();
		LPRDORSSResource pResource = PARSER->findRSSResource(name);
		if (pResource)
		{
			PARSER->error().error(@2, @3, "Ожидается параметр");
		}
		else
		{
			LPRDOFUNSequence pSequence = PARSER->findFUNSequence(name);
			if (pSequence)
			{
				PARSER->error().error(@2, @3, "Ожидается ключевое слово Seed");
			}
			else
			{
				PARSER->error().error(@1, "Неизвестный параметр или последовательность");
			}
		}
	}
	;

smr_launch_line_seed
	: RDO_IDENTIF '.' RDO_Seed '=' RDO_INT_CONST
	{
		if (PARSER->check())
		{
			LPRDOSMR pSMR = PARSER->getSMR(PARSER->m_currentRunNumber);
			ASSERT(pSMR);
			pSMR->setSeed(PARSER->stack().pop<RDOValue>($1)->src_info(), PARSER->stack().pop<RDOValue>($5)->value().getInt());
		}
	}
	| RDO_IDENTIF '.' RDO_Seed '=' error
	{
		PARSER->error().error(@4, @5, "Ожидается база генератора");
	}
	| RDO_IDENTIF '.' RDO_Seed error
	{
		PARSER->error().error(@3, "Ожидается '='");
	}
	;

// --------------------------------------------------------------------------------
// --------------------ShowMode
// --------------------------------------------------------------------------------
smr_show_mode
	: RDO_NoShow
	{
		$$ = rdo::service::simulation::SM_NoShow;
	}
	| RDO_Monitor
	{
		$$ = rdo::service::simulation::SM_Monitor;
	}
	| RDO_Animation
	{
		$$ = rdo::service::simulation::SM_Animation;
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Общие составные токены для всех объектов РДО
// --------------------------------------------------------------------------------
// -------------------- Описание переменной
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
		PARSER->error().error(@2, "Массив должен закрываться скобкой");
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
		PARSER->error().warning(@1, rdo::format("Пропущена запятая перед: %s", pValue->value().getAsString().c_str()));
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
			PARSER->error().error(src_info, "Не указано значение по умолчанию");
		}
		else
		{
			PARSER->error().error(src_info, "Неверное значение по умолчанию");
		}
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Логические выражения
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
		pLogic->setSrcText("[" + pLogic->src_text() + "]");
		$$ = PARSER->stack().push(pLogic);
	}
	| '(' fun_logic ')'
	{
		LPRDOFUNLogic pLogic = PARSER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText("(" + pLogic->src_text() + ")");
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
		PARSER->error().error(@2, "Ожидается закрывающаяся скобка");
	}
	| '(' fun_logic error
	{
		PARSER->error().error(@2, "Ожидается закрывающаяся скобка");
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Арифметические выражения
// --------------------------------------------------------------------------------
fun_arithm
	: RDO_INT_CONST                      { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_REAL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_BOOL_CONST                     { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_STRING_CONST                   { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| param_array_value                  { $$ = PARSER->stack().push(RDOFUNArithm::generateByConst(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_IDENTIF                        { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_IDENTIF_RELRES                 { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1))); }
	| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1), PARSER->stack().pop<RDOValue>($3))); }
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = PARSER->stack().push(RDOFUNArithm::generateByIdentificator(PARSER->stack().pop<RDOValue>($1), PARSER->stack().pop<RDOValue>($3))); }
	| '*' 
	{
		LPRDOValue pValue = rdo::Factory<RDOValue>::create(RDOParserSrcInfo(@1, "*"));
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
		pArithm->setSrcText("(" + pArithm->src_text() + ")");
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

		rdo::runtime::LPRDOCalc pCalc;
		if (pArithm->typeInfo()->type().object_dynamic_cast<RDOArrayType>())
		{
			pCalc = rdo::Factory<rdo::runtime::RDOCalcArraySize>::create(pArithm->calc());
			ASSERT(pCalc);
		}
		else
		{
			PARSER->error().error(@1, rdo::format("'%s' не является массивом.", pValue->value().getIdentificator().c_str()));
		}

		LPTypeInfo pType = rdo::Factory<TypeInfo>::delegate<RDOType__int>(RDOParserSrcInfo(@1));
		ASSERT(pType);

		LPExpression pExpression = rdo::Factory<Expression>::create(pType, pCalc, RDOParserSrcInfo(@1));
		ASSERT(pExpression);

		LPRDOFUNArithm pArithmArraySize = rdo::Factory<RDOFUNArithm>::create(pExpression);
		ASSERT(pArithmArraySize);

		$$ = PARSER->stack().push(pArithmArraySize);
	}
	| RDO_IDENTIF '[' fun_arithm ']' '.' RDO_IDENTIF
	{
		LPRDOValue pArrayValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pArrayValue);

		LPRDOFUNArithm pArrayArithm = RDOFUNArithm::generateByIdentificator(pArrayValue);
		ASSERT(pArrayArithm);

		LPRDOArrayType pArrayType = pArrayArithm->typeInfo()->type().object_dynamic_cast<RDOArrayType>();
		if (!pArrayType)
		{
			PARSER->error().error(@1, rdo::format("'%s' не является массивом"
				, pArrayValue->value().getIdentificator().c_str())
			);
		}

		LPRDORTPResType pResType = pArrayType->getItemType()->type().object_dynamic_cast<RDORTPResType>();
		if (!pResType)
		{
			PARSER->error().error(@1, rdo::format("'%s' не является массивом ресурсов"
				, pArrayValue->value().getIdentificator().c_str())
			);
		}

		LPRDOFUNArithm pArrayIndex = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArrayIndex);

		LPRDOValue pParamName = PARSER->stack().pop<RDOValue>($6);
		ASSERT(pParamName);

		rsint paramIndex = pResType->getRTPParamNumber(pParamName->value().getAsString());

		if (paramIndex == RDORTPResType::UNDEFINED_PARAM)
		{
			PARSER->error().error(@6, rdo::format("'%s' не является параметром ресурса '%s'"
				, pParamName->value().getAsString().c_str()
				, pResType->name().c_str())
			);
		}

		rdo::runtime::LPRDOCalc pArrayItem = rdo::Factory<rdo::runtime::RDOCalcArrayItem>::create(
			pArrayArithm->calc(),
			pArrayIndex->calc()
		);
		ASSERT(pArrayItem);

		rdo::runtime::LPRDOCalc pParamValue = rdo::Factory<rdo::runtime::RDOCalcGetResourceParam>::create(
			pArrayItem, paramIndex
		);
		ASSERT(pParamValue);

		LPExpression pParamExpression = rdo::Factory<Expression>::create(
			pResType->getParams()[paramIndex]->getTypeInfo(),
			pParamValue,
			RDOParserSrcInfo(@6)
		);
		ASSERT(pParamExpression);

		LPRDOFUNArithm pParamArithm = rdo::Factory<RDOFUNArithm>::create(pParamExpression);
		ASSERT(pParamArithm);

		$$ = PARSER->stack().push(pParamArithm);
	}
	| RDO_IDENTIF '[' fun_arithm ']'
	{
		LPRDOValue pValue = PARSER->stack().pop<RDOValue>($1);
		ASSERT(pValue);

		LPRDOFUNArithm pArithm = RDOFUNArithm::generateByIdentificator(pValue);
		ASSERT(pArithm);

		LPRDOFUNArithm pArithmInd = PARSER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithmInd);

		LPRDOType pType = pArithm->typeInfo()->type();
		ASSERT(pType);

		LPRDOArrayType pArrayType = pType.object_dynamic_cast<RDOArrayType>();
		if (!pArrayType)
		{
			PARSER->error().error(@1, rdo::format("'%s' не является массивом.", pValue->value().getIdentificator().c_str()));
		}

		rdo::runtime::LPRDOCalc pCalc = rdo::Factory<rdo::runtime::RDOCalcArrayItem>::create(pArithm->calc(), pArithmInd->calc());
		ASSERT(pCalc);

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
// -------------------- Функции и последовательности
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
		pFunParams->setSrcText(funName + "(" + pArithmContainer->src_text() + ")");
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' error
	{
		PARSER->error().error(@3, "Ошибка в параметрах функции");
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
		pArithmContainer->setSrcText(pArithmContainer->src_text() + ", " + pArithm->src_text());
		pArithmContainer->addItem   (pArithm);
		$$ = PARSER->stack().push(pArithmContainer);
	}
	| arithm_list_body ',' error
	{
		PARSER->error().error(@3, "Ошибка в арифметическом выражении");
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Групповые выражения
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
		PARSER->error().error(@3, "Ожидается имя типа");
	}
	| fun_group_keyword error
	{
		PARSER->error().error(@1, "После имени функции ожидается октрывающаяся скобка");
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
		LPRDOFUNLogic pTrueLogic = RDOFUNLogic::generateTrue(RDOParserSrcInfo(@2, "NoCheck"));
		ASSERT(pTrueLogic);
		$$ = PARSER->stack().push(pGroupFun->createFunLogic(pTrueLogic));
	}
	| fun_group_header fun_logic error
	{
		PARSER->error().error(@2, "Ожидается закрывающаяся скобка");
	}
	| fun_group_header RDO_NoCheck error
	{
		PARSER->error().error(@2, "Ожидается закрывающаяся скобка");
	}
	| fun_group_header error
	{
		PARSER->error().error(@1, @2, "Ошибка в логическом выражении");
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
		pSelect->setSrcText("Select(" + pValue->value().getIdentificator() + ": ");
		$$ = PARSER->stack().push(pSelect);
	}
	| RDO_Select '(' error
	{
		PARSER->error().error(@3, "Ожидается имя типа");
	}
	| RDO_Select error
	{
		PARSER->error().error(@1, "Ожидается октрывающаяся скобка");
	}
	;

fun_select_body
	: fun_select_header fun_logic ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = PARSER->stack().pop<RDOFUNLogic> ($2);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcText(pSelect->src_text() + pLogic->src_text() + ")");
		pSelect->initSelect(pLogic);
		$$ = PARSER->stack().push(pSelect);
	}
	| fun_select_header RDO_NoCheck ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		RDOParserSrcInfo info(@2, "NoCheck");
		pSelect->setSrcText(pSelect->src_text() + info.src_text() + ")");
		LPRDOFUNLogic pTrueLogic = RDOFUNLogic::generateTrue(info);
		ASSERT(pTrueLogic);
		pSelect->initSelect(pTrueLogic);
		$$ = PARSER->stack().push(pSelect);
	}
	| fun_select_header fun_logic error
	{
		PARSER->error().error(@2, "Ожидается закрывающаяся скобка");
	}
	| fun_select_header RDO_NoCheck error
	{
		PARSER->error().error(@2, "Ожидается закрывающаяся скобка");
	}
	| fun_select_header error
	{
		PARSER->error().error(@1, @2, "Ошибка в логическом выражении");
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
		PARSER->error().error(@4, @5, "Ошибка в логическом выражении");
	}
	| fun_select_body '.' fun_select_keyword error
	{
		PARSER->error().error(@3, "Ожидается октрывающаяся скобка");
	}
	| fun_select_body '.' RDO_Empty '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo emptyInfo(@3, @5, "Empty()");
		LPRDOFUNLogic pLogic = pSelect->createFunSelectEmpty(emptyInfo);
		ASSERT(pLogic);
		$$ = PARSER->stack().push(pLogic);
	}
	| fun_select_body '.' RDO_Empty '(' error
	{
		PARSER->error().error(@4, "Ожидается закрывающаяся скобка");
	}
	| fun_select_body '.' RDO_Empty error
	{
		PARSER->error().error(@3, "Ожидается октрывающаяся скобка");
	}
	| fun_select_body '.' error
	{
		PARSER->error().error(@2, @3, "Ожидается метод списка ресурсов");
	}
	| fun_select_body error
	{
		PARSER->error().error(@1, "Ожидается '.' (точка) для вызова метода списка ресурсов");
	}
	;

fun_select_arithm
	: fun_select_body '.' RDO_Size '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo sizeInfo(@3, @5, "Size()");
		LPRDOFUNArithm pArithm = pSelect->createFunSelectSize(sizeInfo);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| fun_select_body '.' RDO_Size error
	{
		PARSER->error().error(@3, "Ожидается октрывающаяся скобка");
	}
	| fun_select_body '.' RDO_Size '(' error
	{
		PARSER->error().error(@4, "Ожидается закрывающаяся скобка");
	}
	| fun_select_body '.' RDO_Select_Array '(' ')'
	{
		LPRDOFUNSelect pSelect = PARSER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo arrayInfo(@3, @5, "Array()");
		LPRDOFUNArithm pArithm = pSelect->createFunSelectArray(arrayInfo);
		ASSERT(pArithm);
		$$ = PARSER->stack().push(pArithm);
	}
	| fun_select_body '.' RDO_Select_Array error
	{
		PARSER->error().error(@3, "Ожидается октрывающаяся скобка");
	}
	| fun_select_body '.' RDO_Select_Array '(' error
	{
		PARSER->error().error(@4, "Ожидается закрывающаяся скобка");
	}
	;

%%

CLOSE_RDO_PARSER_NAMESPACE
