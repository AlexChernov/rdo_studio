/*
 * copyright: (c) RDO-Team, 2009
 * filename : model_objects_convertor.h
 * author   : �������� ����, ������ ������
 * date     : 13.06.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOCOMMON_MODEL_OBJECTS_CONVERTOR_H_
#define _RDOCOMMON_MODEL_OBJECTS_CONVERTOR_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
// ===============================================================================

#define OPEN_RDO_MODEL_OBJECTS_CONVERTOR_NAMESPACE	namespace rdoModelObjectsConvertor {
#define CLOSE_RDO_MODEL_OBJECTS_CONVERTOR_NAMESPACE	};

OPEN_RDO_MODEL_OBJECTS_CONVERTOR_NAMESPACE
	struct RDOSMRFileInfo
	{
		tstring  m_model_name;
		tstring  m_resource_file;
		tstring  m_oprIev_file;
		tstring  m_frame_file;
		tstring  m_statistic_file;
		tstring  m_results_file;
		tstring  m_trace_file;
		rbool    m_error;

		RDOSMRFileInfo()
			: m_model_name    (_T(""))
			, m_resource_file (_T(""))
			, m_oprIev_file   (_T(""))
			, m_frame_file    (_T(""))
			, m_statistic_file(_T(""))
			, m_results_file  (_T(""))
			, m_trace_file    (_T(""))
			, m_error         (false )
		{}
	};

	enum RDOFileType  { UNDEFINED = 0, PAT, RTP, RSS, OPR, FRM, FUN, DPT, SMR, PMD, PMV, TRC, ENV, PRC };

	enum RDOParseType
	{
		obNONE = 0x0000,
		obPRE  = 0x0001,
		obRTP  = 0x0002,
		obRSS  = 0x0004,
		obFUN  = 0x0008,
		obPAT  = 0x0010,
		obOPR  = 0x0020,
		obDPT  = 0x0040,
		obPMD  = 0x0080,
		obFRM  = 0x0100,
		obSMR  = 0x0200,
		obPOST = 0x0400,
		obALL  = 0x07FF
	};
CLOSE_RDO_MODEL_OBJECTS_CONVERTOR_NAMESPACE

#endif //! _RDOCOMMON_MODEL_OBJECTS_CONVERTOR_H_
