/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoproc_opr.y
 * author   : �������� ����, ������ ������
 * date     : 24.03.2008
 * bref     : ��������� ���������. based on rdodpt_opr.y
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
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdodpt.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdorss.h"

#include "rdo_lib/rdo_mbuilder/rdo_resources.h"
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

%start prc_main

%%

// ----------------------------------------------------------------------------
// ---------- General part
// ----------------------------------------------------------------------------
prc_main
	: /* empty */
	| prc_main dpt_process_end
	;

// ----------------------------------------------------------------------------
// ---------- Process
// ----------------------------------------------------------------------------
dpt_process
	: dpt_process_header dpt_process_input
	;

dpt_process_header
	: dpt_process_begin dpt_process_condition dpt_process_prior
	;

dpt_process_begin
	: RDO_Process
	{
		LPRDOPROCProcess pProcess = PARSER->getLastPROCProcess();
		if (pProcess && !pProcess->closed())
		{
			PARSER->error().error(pProcess->src_info(), _T("�������� ���������� ���� $Process"));
		}
		pProcess = rdo::Factory<RDOPROCProcess>::create(RDOParserSrcInfo(@1, _T("Process")));
		ASSERT(pProcess);
	}
	;

dpt_process_condition
	: /* empty */
	{
		LPRDOPROCProcess pProcess = PARSER->getLastPROCProcess();
		ASSERT(pProcess);
		pProcess->setCondition();
	}
	| RDO_Condition fun_logic
	{
		LPRDOPROCProcess pProcess = PARSER->getLastPROCProcess();
		ASSERT(pProcess);
		pProcess->setCondition(PARSER->stack().pop<RDOFUNLogic>($2));
	}
	| RDO_Condition RDO_NoCheck
	{
		LPRDOPROCProcess pProcess = PARSER->getLastPROCProcess();
		ASSERT(pProcess);
		pProcess->setCondition();
	}
	| RDO_Condition error
	{
		PARSER->error().error(@2, @2, _T("����� ��������� ����� $Condition ��������� ������� ��������� ��������"));
	}
	| error
	{
		PARSER->error().error(@1, _T("��������� �������� ����� $Condition"));
	}
	;

dpt_process_prior
	: /* empty */
	| RDO_Priority fun_arithm
	{
		if (!PARSER->getLastPROCProcess()->setPrior(PARSER->stack().pop<RDOFUNArithm>($2)))
		{
			PARSER->error().error(@2, _T("������� ���� �� ����� ����� ���������"));
		}
	}
	| RDO_Priority error
	{
		PARSER->error().error(@1, @2, _T("������ �������� ���������� ����� �������� �������"))
	}
	| error
	{
		PARSER->error().error(@1, @1, _T("��������� �������� ����� $Priority"))
	}
	;

dpt_process_input
	: /* empty */
	| dpt_process_input dpt_process_line
	;

dpt_process_line
	: RDO_IDENTIF
	{
		PARSER->error().error(@1, rdo::format(_T("����������� �������� '%s'"), P_RDOVALUE($1)->value().getIdentificator().c_str()));
	}
	| RDO_GENERATE fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		int time = pArithm->createCalc()->calcValue(RUNTIME).getInt();
		tstring rtp_name       = _T("���������");
		tstring rtp_param_name = _T("�����_��������");

		// �������� ������ ���� ����� ��������
		rdoMBuilder::RDOResTypeList rtpList(PARSER);
		// ����� ��� �������, ���� ��� ���, �� �������
		if (!rtpList[rtp_name].exist())
		{
			// �������� ��� �������
			rdoMBuilder::RDOResType rtp(rtp_name);
			// ������� �������� �����_��������
			rtp.m_params.append(rdoMBuilder::RDOResType::Param(rtp_param_name, rdo::Factory<RDOType__real>::create()));
			// ������� ��� �������
			if (!rtpList.append(rtp))
			{
				PARSER->error().error(@2, rdo::format(_T("������ �������� ���� �������: %s"), rtp_name.c_str()));
			}
			rdoRuntime::RDOPROCTransact::typeID = rtp.id();
		}
		else
		{
			// ��� ������, �������� ��� �� ������� ������������� ���������
			CREF(rdoMBuilder::RDOResType) rtp = rtpList[rtp_name];
			if (!rtp.m_params[rtp_param_name].exist())
			{
				PARSER->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' ��� ���������� ��������� '%s'"), rtp.name().c_str(), rtp_param_name.c_str()));
			}
			// �������� ����, ���� ��������� �� ���
			if (rtp.m_params[rtp_param_name].typeID() != rdoRuntime::RDOType::t_real)
			{
				PARSER->error().error(rtp.src_info(), rdo::format(_T("� ���� ������� '%s' �������� '%s' �� �������� ������������ �����"), rtp.name().c_str(), rtp_param_name.c_str()));
			}
			rdoRuntime::RDOPROCTransact::typeID = rtp.id();
		}
		LPRDOPROCOperator pBlock = rdo::Factory<RDOPROCGenerate>::create(PARSER->getLastPROCProcess(), _T("GENERATE"), pArithm->createCalc());
		ASSERT(pBlock);
		$$ = PARSER->stack().push(pBlock);
	}
	| RDO_GENERATE fun_arithm error
	{
		PARSER->error().error(@2, _T("������ � �������������� ���������"));
	}
	| RDO_TERMINATE dpt_term_param
	{
		LPRDOPROCOperator pBlock = PARSER->stack().pop<RDOPROCOperator>($2);
		ASSERT(pBlock);
		$$ = PARSER->stack().push(pBlock);
	}
	| RDO_TERMINATE error
	{
		PARSER->error().error(@1, _T("������ � ��������� ��������� TERMINATE"));
	}
	| RDO_ADVANCE fun_arithm
	{
		LPRDOPROCOperator pBlock = rdo::Factory<RDOPROCAdvance>::create(PARSER->getLastPROCProcess(), _T("ADVANCE"), PARSER->stack().pop<RDOFUNArithm>($2)->createCalc());
		ASSERT(pBlock);
		$$ = PARSER->stack().push(pBlock);
	}
	| RDO_ADVANCE fun_arithm error
	{
		PARSER->error().error(@2, _T("������ � �������������� ���������"));
	}
	| RDO_QUEUE dpt_queue_param
	{
		TRACE(_T("QUEUE dpt_queue_param\n"));
		LPRDOPROCQueue pQueue = PARSER->stack().pop<RDOPROCQueue>($2);
		ASSERT(pQueue);
		pQueue->createRuntime();
		$$ = PARSER->stack().push(pQueue);
	}
	| RDO_QUEUE error
	{
		PARSER->error().error(@1, _T("��������� ��� ������� ��� ����� ���������� �� �������"));
	}
	| RDO_DEPART dpt_depart_param
	{
		TRACE(_T("DEPART dpt_depart_param\n"));
		LPRDOPROCDepart pDepart = PARSER->stack().pop<RDOPROCDepart>($2);
		ASSERT(pDepart);
		pDepart->createRuntime();
		$$ = PARSER->stack().push(pDepart);
	}
	| RDO_DEPART error
	{
		PARSER->error().error(@1, _T("��������� ��� ������� ��� ����� ���������� �� �������"));
	}
	| RDO_SEIZE dpt_seize_param
	{
		TRACE(_T("SEIZE dpt_seize_param\n"));
		LPRDOPROCSeize pSeize = PARSER->stack().pop<RDOPROCSeize>($2);
		ASSERT(pSeize);
		pSeize->createRuntime();
		$$ = PARSER->stack().push(pSeize);
	}
	| RDO_SEIZE error
	{
		PARSER->error().error(@1, rdo::format(_T("��������� ������ ��������, ������������ � ����, ����� �������")));
	}
	| RDO_RELEASE dpt_release_param
	{
		TRACE(_T("RELEASE dpt_release_param\n"));
		LPRDOPROCRelease pRelease = PARSER->stack().pop<RDOPROCRelease>($2);
		ASSERT(pRelease);
		pRelease->createRuntime();
		$$ = PARSER->stack().push(pRelease);
	}
	| RDO_RELEASE error
	{
		PARSER->error().error(@1, rdo::format(_T("��������� ������ ��������, ������������ � ����, ����� �������")));
	}
	| RDO_ASSIGN dpt_assign_param
	{
	}
	| RDO_ASSIGN error
	{
		PARSER->error().error(@1, rdo::format(_T("��������� ������ ��������� ���������")));
	}
	;

dpt_queue_param
	: RDO_IDENTIF
	{
		tstring res_name = P_RDOVALUE($1)->value().getIdentificator();
		TRACE1(_T("%s _good\n"), res_name.c_str());
		LPRDOPROCQueue pQueue = rdo::Factory<RDOPROCQueue>::create(PARSER->getLastPROCProcess(), _T("QUEUE"));
		ASSERT(pQueue);
		pQueue->setResource(res_name);
		$$ = PARSER->stack().push(pQueue);
	}
	| RDO_IDENTIF error
	{
		PARSER->error().error(@1, _T("������ � ����� �������"))
	}
	;

dpt_depart_param
	: RDO_IDENTIF
	{
		tstring res_name = P_RDOVALUE($1)->value().getIdentificator();
		TRACE1(_T("%s _good\n"), res_name.c_str());
		LPRDOPROCDepart pDepart = rdo::Factory<RDOPROCDepart>::create(PARSER->getLastPROCProcess(), _T("DEPART"));
		ASSERT(pDepart);
		pDepart->setResource(res_name);
		$$ = PARSER->stack().push(pDepart);
	}
	| RDO_IDENTIF error 
	{
		PARSER->error().error(@1, _T("������ � ����� �������"))
	}
	;

dpt_term_param
	: /* empty */
	{
		LPRDOPROCOperator pBlock = rdo::Factory<RDOPROCTerminate>::create(PARSER->getLastPROCProcess(), _T("TERMINATE"), 0);
		ASSERT(pBlock);
		$$ = PARSER->stack().push(pBlock);
	}
	| fun_arithm
	{
		LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pArithm);
		if (pArithm->createCalc()->calcValue(RUNTIME).type()->typeID()==rdoRuntime::RDOType::t_int)
		{
			int term = pArithm->createCalc()->calcValue(RUNTIME).getInt();
			LPRDOPROCOperator pBlock = rdo::Factory<RDOPROCTerminate>::create(PARSER->getLastPROCProcess(), _T("TERMINATE"), term);
			ASSERT(pBlock);
			$$ = PARSER->stack().push(pBlock);
		}
		else
		{
			PARSER->error().error(@1, _T("������, ��� ��������� TERMINATE ����� ������������ ������ ����� ��������"));
		}
	}
	| fun_arithm  error
	{
		PARSER->error().error(@1, _T("������, ����� ��������� TERMINATE ����� ���� ������� ������ ���� ����� ������������� �����"))
	}
	;

dpt_seize_param
	: RDO_IDENTIF
	{
		tstring        res_name = P_RDOVALUE($1)->value().getIdentificator().c_str();
		LPRDOPROCSeize pSeize   = rdo::Factory<RDOPROCSeize>::create(PARSER->getLastPROCProcess(), _T("SEIZE"));
		ASSERT(pSeize);
		pSeize->addResource(res_name);
		$$ = PARSER->stack().push(pSeize);
	}
	| dpt_seize_param ',' RDO_IDENTIF
	{
		LPRDOPROCSeize pSeize = PARSER->stack().pop<RDOPROCSeize>($1);
		ASSERT(pSeize);
		tstring res_name = P_RDOVALUE($3)->value().getIdentificator().c_str();
		pSeize->addResource(res_name);
		$$ = PARSER->stack().push(pSeize);
	}
	| dpt_seize_param error
	{
		PARSER->error().error(@1, _T("������ � ����� �������"));
	}
	;

dpt_release_param
	: RDO_IDENTIF
	{
		tstring          res_name = P_RDOVALUE($1)->value().getIdentificator().c_str();
		LPRDOPROCRelease pRelease = rdo::Factory<RDOPROCRelease>::create(PARSER->getLastPROCProcess(), _T("RELEASE"));
		ASSERT(pRelease);
		pRelease->addResource(res_name);
		$$ = PARSER->stack().push(pRelease);
	}
	| dpt_release_param ',' RDO_IDENTIF
	{
		LPRDOPROCRelease pRelease = PARSER->stack().pop<RDOPROCRelease>($1);
		ASSERT(pRelease);
		tstring res_name = P_RDOVALUE($3)->value().getIdentificator().c_str();
		pRelease->addResource(res_name);
		$$ = PARSER->stack().push(pRelease);
	}
	| dpt_release_param error
	{
		PARSER->error().error(@1, _T("������ � ����� �������"));
	}
	;

dpt_assign_param
	: RDO_IDENTIF '.' RDO_IDENTIF '=' fun_arithm
	{
		tstring res   = P_RDOVALUE($1)->value().getIdentificator();
		tstring param = P_RDOVALUE($3)->value().getIdentificator();
		CREF(RDOParserSrcInfo) info = @1;
		rdoMBuilder::RDOResType rtp;

		// �������� ������ ���� ��������
		rdoMBuilder::RDOResourceList rssList(PARSER);

		// ���� ������ ����������, ����� ��� ��� � ��������� ������� ���������
		if (rssList[res].exist())
		{
			rtp = rssList[res].getType();
			if(!rtp.m_params[param].exist())
			{
				PARSER->error().error(@1, rdo::format(_T("������ �� ����������� �������� �������: %s.%s"), res.c_str(), param.c_str()));
			}
		
			LPRDOFUNArithm pArithm = PARSER->stack().pop<RDOFUNArithm>($5);
			if (pArithm)
			{
				LPRDORSSResource pResource = PARSER->findRSSResource(res);
				ASSERT(pResource);
				LPRDORTPResType pResType = pResource->getType();
				LPRDORTPParam   pParam   = pResType->findRTPParam(param);
				pArithm->checkParamType(pParam->getType());
				LPRDOPROCOperator pBlock = rdo::Factory<RDOPROCAssign>::create(PARSER->getLastPROCProcess(), _T("ASSIGN"), pArithm->createCalc(pParam->getType()), pResource->getID(), rtp.m_params[param].id());
				ASSERT(pBlock);
				$$ = PARSER->stack().push(pBlock);
			}
		}
		else
		{
			PARSER->error().error(@1, rdo::format(_T("������ �� ����������� ������: %s"), res.c_str()));
		}
	}
	;

dpt_process_end
	: dpt_process RDO_End
	{
		PARSER->getLastPROCProcess()->end();
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
