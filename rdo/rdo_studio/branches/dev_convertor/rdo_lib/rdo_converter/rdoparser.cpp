/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoparser.cpp
 * author   : �������� ����, ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/rdoparser.h"
#include "rdo_lib/rdo_converter/rdoparser_rdo.h"
#include "rdo_lib/rdo_converter/rdofun.h"
#include "rdo_lib/rdo_converter/rdorss.h"
#include "rdo_lib/rdo_converter/context/global.h"
#include "rdo_lib/rdo_converter/rdo_common/model_objects_convertor.h"
#include "rdo_lib/rdo_converter/update/update_i.h"
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdofile.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Converter
// ----------------------------------------------------------------------------
Converter::ParserList Converter::s_parserStack;

#define DECLARE_PARSER_OBJECT_CONTAINER_NONAME(NAME) \
void Converter::insert##NAME(LPRDO##NAME value) \
{ \
	howIsIt<LPRDO##NAME>(); \
	m_all##NAME.push_back(value); \
}

#define DECLARE_PARSER_OBJECT_CONTAINER(NAME) \
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(NAME) \
const LPRDO##NAME Converter::find##NAME(CREF(tstring) name) const \
{ \
	NAME##List::const_iterator it = std::find_if(m_all##NAME.begin(), m_all##NAME.end(), compareName<RDO##NAME>(name)); \
	return it != m_all##NAME.end() ? *it : NULL; \
} \
rbool Converter::remove##NAME(const LPRDO##NAME item) \
{ \
	NAME##List::iterator it = std::find(m_all##NAME.begin(), m_all##NAME.end(), item); \
	if (it == m_all##NAME.end()) \
		return false; \
	m_all##NAME.erase(it); \
	return true; \
}

DECLARE_PARSER_OBJECT_CONTAINER(PATPattern     );
DECLARE_PARSER_OBJECT_CONTAINER(RTPResType     );
DECLARE_PARSER_OBJECT_CONTAINER(RSSResource    );
DECLARE_PARSER_OBJECT_CONTAINER(OPROperation   );
DECLARE_PARSER_OBJECT_CONTAINER(FRMFrame       );
DECLARE_PARSER_OBJECT_CONTAINER(FUNConstant    );
DECLARE_PARSER_OBJECT_CONTAINER(FUNFunction    );
DECLARE_PARSER_OBJECT_CONTAINER(FUNSequence    );
DECLARE_PARSER_OBJECT_CONTAINER(DPTSearch      );
DECLARE_PARSER_OBJECT_CONTAINER(DPTSome        );
DECLARE_PARSER_OBJECT_CONTAINER(DPTPrior       );
DECLARE_PARSER_OBJECT_CONTAINER(DPTFreeActivity);
DECLARE_PARSER_OBJECT_CONTAINER(PMDPokaz       );

DECLARE_PARSER_OBJECT_CONTAINER_NONAME(FUNGroup   );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(DPTFree    );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(PROCProcess);
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(Operations );

rdoModelObjectsConvertor::RDOFileType Converter::getFileToParse()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_pParserItem ? s_parserStack.back()->m_pParserItem->m_type : rdoModelObjectsConvertor::PAT;
}

ruint Converter::lexer_loc_line()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_pParserItem ? s_parserStack.back()->m_pParserItem->lexer_loc_line() : ~0;
}

ruint Converter::lexer_loc_pos()
{
	return !s_parserStack.empty() && s_parserStack.back()->m_pParserItem ? s_parserStack.back()->m_pParserItem->lexer_loc_pos() : 0;
}

PTR(Converter) Converter::s_converter()
{
	return !s_parserStack.empty() ? s_parserStack.back() : NULL;
}

Converter::Converter()
	: m_pParserItem         (NULL )
	, m_have_kw_Resources   (false)
	, m_have_kw_ResourcesEnd(false)
	, m_pattern             (false)
{
	s_parserStack.push_back(this);
	m_runtime.memory_insert(sizeof(Converter));
	m_runtime.init();
	m_contextStack.push(rdo::Factory<ContextGlobal>::create());
}

Converter::~Converter()
{
	m_runtime.deinit();
	rdo::deleteAllObjects(m_allValues);
	m_movementObjectList.clear();
	s_parserStack.remove(this);
}

REF(ContextStack) Converter::contextStack()
{
	return m_contextStack;
}

LPContext Converter::context() const
{
	return m_contextStack.top();
}

void Converter::insertDocUpdate(CREF(LPDocUpdate) pDocUpdate)
{
	ASSERT(m_pParserItem);
	m_pParserItem->insertDocUpdate(pDocUpdate);
}

rbool Converter::isCurrentDPTSearch()
{
	return getLastDPTSearch() && !getLastDPTSearch()->closed() ? true : false;
}

rbool Converter::isCurrentDPTPrior()
{
	return getLastDPTPrior() ? true : false;
}

void Converter::insertChanges(CREF(tstring) name, CREF(tstring) value)
{
	m_changes.push_back(Changes(name, value));
}

tstring Converter::getChanges() const
{
	rdo::textstream stream;
	stream << _T("$Changes") << std::endl;
	ruint changes_max_length = 0;
	STL_FOR_ALL_CONST(m_changes, change_it)
	{
		if (change_it->m_name.length() > changes_max_length)
		{
			changes_max_length = change_it->m_name.length();
		}
	}
	STL_FOR_ALL_CONST(m_changes, change_it)
	{
		stream << _T("  ") << change_it->m_name;
		for (ruint i = change_it->m_name.length(); i < changes_max_length; i++)
		{
			stream << _T(" ");
		}
		stream << _T("  = ") << change_it->m_value << std::endl;
	}
	return stream.str();
}

tstring Converter::getModelStructure()
{
	rdo::textstream modelStructure;

	// $Changes
	modelStructure << getChanges();

	// RTP
	modelStructure << std::endl << std::endl << _T("$Resource_type") << std::endl;
	STL_FOR_ALL_CONST(m_allRTPResType, rtp_it)
	{
		(*rtp_it)->writeModelStructure(modelStructure);
	}

	// RSS
	modelStructure << std::endl << _T("$Resources") << std::endl;
	STL_FOR_ALL_CONST(m_allRSSResource, rss_it)
	{
		(*rss_it)->writeModelStructure(modelStructure);
	}

	// PAT
	modelStructure << std::endl << _T("$Pattern") << std::endl;
	STL_FOR_ALL_CONST(m_allPATPattern, pat_it)
	{
		(*pat_it)->writeModelStructure(modelStructure);
	}

	// OPR/DPT
	ruint counter = 1;
	modelStructure << std::endl << _T("$Activities") << std::endl;
	modelStructure << m_runtime.writeActivitiesStructure(counter);

	// DPT only
	for (ruint i = 0; i < m_allDPTSearch.size(); i++)
	{
		for (ruint j = 0; j < m_allDPTSearch.at(i)->getActivities().size(); j++)
		{
			LPRDODPTSearchActivity pSearchActivity = m_allDPTSearch.at(i)->getActivities().at(j);
			ASSERT(pSearchActivity);
			modelStructure << counter++ << _T(" ") << pSearchActivity->name() << _T(" ") << pSearchActivity->pattern()->getPatternId() << std::endl;
		}
	}

	// PMD
	modelStructure << std::endl << _T("$Watching") << std::endl;
	ruint watching_max_length = 0;
	STL_FOR_ALL_CONST(m_runtime.getPokaz(), watching_it)
	{
		LPITrace          trace     = *watching_it;
		LPIName           name      = trace;
		LPIModelStructure structure = trace;
		if (trace && name && structure)
		{
			if (trace->traceable() && name->name().length() > watching_max_length)
			{
				watching_max_length = name->name().length();
			}
		}
	}
	STL_FOR_ALL_CONST(m_runtime.getPokaz(), watching_it)
	{
		LPITrace          trace     = *watching_it;
		LPIName           name      = trace;
		LPIModelStructure structure = trace;
		if (trace && name && structure)
		{
			if (trace->traceable())
			{
				modelStructure << _T("  ") << name->name();
				for (ruint i = name->name().length(); i < watching_max_length + 2; i++)
					modelStructure << _T(" ");

				structure->writeModelStructure(modelStructure);
			}
		}
	}

	return modelStructure.str();
}

CREF(RDOParserSMRInfo::FileList) RDOParserSMRInfo::getFileList() const
{
	return m_fileList;
}

void RDOParserSMRInfo::insertFileName(rdoModelObjectsConvertor::RDOFileType type,
                                      CREF(tstring)                         modelPath,
                                      CREF(tstring)                         modelName,
                                      CREF(tstring)                         smrFileName,
                                      CREF(tstring)                         nameFromSMR,
                                      CREF(tstring)                         fileExt
)
{
	CREF(tstring) fileName = !nameFromSMR.empty() ? nameFromSMR : (!modelName.empty() ? modelName : smrFileName);
	if (fileName.empty())
		return;

	tstring fullFileName = rdo::format(_T("%s%s.%s"), modelPath.c_str(), fileName.c_str(), fileExt.c_str());

	if (rdo::File::exist(fullFileName))
	{
		m_fileList[type] = fullFileName;
	}
}

rbool RDOParserSMRInfo::parseSMR(CREF(tstring) smrFullFileName, REF(rdoModelObjectsConvertor::RDOSMRFileInfo) smrInfo)
{
	std::ifstream stream(smrFullFileName.c_str());
	if (!stream.is_open())
		return false;

	RDOParserContainer::Iterator it = begin();
	ASSERT(it != end());

	m_pParserItem = it->second;
	it->second->parse(this, stream);
	m_pParserItem = NULL;

#ifdef _DEBUG
	++it;
	ASSERT(it == end());
#endif

	if (!hasSMR())
		return false;

	tstring smrFilePath, smrFileName, smrFileExt;
	if (!rdo::File::splitpath(smrFullFileName, smrFilePath, smrFileName, smrFileExt))
		return false;

	tstring modelName = getSMR()->getFile(_T("Model_name"));

	insertFileName(rdoModelObjectsConvertor::PAT, smrFilePath, modelName, smrFileName, modelName,                               _T("pat"));
	insertFileName(rdoModelObjectsConvertor::RTP, smrFilePath, modelName, smrFileName, modelName,                               _T("rtp"));
	insertFileName(rdoModelObjectsConvertor::RSS, smrFilePath, modelName, smrFileName, getSMR()->getFile(_T("Resource_file")),  _T("rss"));
	insertFileName(rdoModelObjectsConvertor::OPR, smrFilePath, modelName, smrFileName, getSMR()->getFile(_T("OprIev_file")  ),  _T("opr"));
	insertFileName(rdoModelObjectsConvertor::FRM, smrFilePath, modelName, smrFileName, getSMR()->getFile(_T("Frame_file")   ),  _T("frm"));
	insertFileName(rdoModelObjectsConvertor::FUN, smrFilePath, modelName, smrFileName, modelName,                               _T("fun"));
	insertFileName(rdoModelObjectsConvertor::DPT, smrFilePath, modelName, smrFileName, modelName,                               _T("dpt"));
	insertFileName(rdoModelObjectsConvertor::PMD, smrFilePath, modelName, smrFileName, getSMR()->getFile(_T("Statistic_file")), _T("pmd"));
	insertFileName(rdoModelObjectsConvertor::SMR, smrFilePath, modelName, smrFileName, smrFileName,                             _T("smr"));
	insertFileName(rdoModelObjectsConvertor::PMV, smrFilePath, modelName, smrFileName, getSMR()->getFile(_T("Results_file")  ), _T("pmv"));
	insertFileName(rdoModelObjectsConvertor::TRC, smrFilePath, modelName, smrFileName, getSMR()->getFile(_T("Trace_file")    ), _T("trc"));

	return true;
}

RDOParserModel::Result RDOParserModel::convert(CREF(tstring) smrFullFileName)
{
	RDOParserSMRInfo::FileList fileList;
	{
		std::auto_ptr<RDOParserSMRInfo> pSMRParser(new RDOParserSMRInfo());
		rdoModelObjectsConvertor::RDOSMRFileInfo smrInfo;

		try
		{
			if (!pSMRParser->parseSMR(smrFullFileName, smrInfo))
				return CNV_NONE;
		}
		catch (REF(rdoParse::RDOSyntaxException))
		{
			return CNV_NONE;
		}
		catch (REF(rdoRuntime::RDORuntimeException))
		{
			return CNV_NONE;
		}
		fileList = pSMRParser->getFileList();
		if (fileList.empty())
		{
			return CNV_NONE;
		}
	}

	try
	{
		RDOParserContainer::Iterator it = begin();
		while (it != end())
		{
			m_pParserItem = it->second;
			if (m_pParserItem->needStream())
			{
				RDOParserSMRInfo::FileList::const_iterator it = fileList.find(m_pParserItem->m_type);
				if (it != fileList.end())
				{
					std::ifstream stream(it->second.c_str(), ios::binary);
					m_pParserItem->parse(this, stream);
				}
			}
			else
			{
				m_pParserItem->parse(this);
			}
			m_pParserItem = NULL;
			++it;
		}
	}
	catch (REF(rdoConverter::RDOSyntaxException))
	{
		return CNV_NONE;
	}
	catch (REF(rdoRuntime::RDORuntimeException))
	{
		return CNV_NONE;
	}
	catch (...)
	{
		return CNV_NONE;
	}

	error().clear();

	try
	{
		BOOST_AUTO(fileIt, fileList.begin());
		boost::filesystem::path fullPath(fileIt->second);
		fullPath.remove_filename();

		boost::posix_time::ptime time(boost::posix_time::second_clock::local_time());
		std::stringstream backupDirName;
		backupDirName << fullPath.directory_string()
		              << boost::format(_T("backup %1$04d-%2$02d-%3$02d %4$02d-%5$02d-%6$02d"))
		                 % time.date().year ()
		                 % time.date().month()
		                 % time.date().day  ()
		                 % time.time_of_day().hours  ()
		                 % time.time_of_day().minutes()
		                 % time.time_of_day().seconds();

		boost::filesystem::path backupPath(backupDirName.str());

		try
		{
			if (!boost::filesystem::create_directory(backupPath))
			{
				YYLTYPE pos;
				pos.m_last_line = 0;
				pos.m_last_pos  = 0;
				error().error(RDOParserSrcInfo(pos), rdo::format(_T("������ �������� backup-���������� '%s': ��� ����������\n"), backupPath.directory_string().c_str()));
			}
		}
		catch (CREF(boost::filesystem::basic_filesystem_error<boost::filesystem::path>) ex)
		{
			tstring message = ex.what();
			if (message.find(_T("boost")) == 0)
			{
				BOOST_AUTO(pos, message.find(_T(' ')));
				if (pos != tstring::npos)
				{
					message = message.substr(pos + 1);
				}
			}
			YYLTYPE pos;
			pos.m_last_line = 0;
			pos.m_last_pos  = 0;
			error().error(RDOParserSrcInfo(pos), rdo::format(_T("������ �������� backup-���������� '%s': %s\n"), backupPath.directory_string().c_str(), message.c_str()));
		}

		STL_FOR_ALL(fileList, it)
		{
			boost::filesystem::path from(it->second);
			boost::filesystem::path to  (backupPath.directory_string() + boost::filesystem::slash<boost::filesystem::path>::value + from.filename());
			boost::filesystem::rename(from, to);
			it->second = to.file_string();
		}
	}
	catch (REF(rdoConverter::RDOSyntaxException))
	{
		return CNV_ERROR;
	}
	catch (REF(rdoRuntime::RDORuntimeException))
	{
		return CNV_ERROR;
	}
	catch (...)
	{
		return CNV_ERROR;
	}

	RDOParserContainer::Iterator it = begin();
	while (it != end())
	{
		LPRDOParserItem pParserItem = it->second;
		ASSERT(pParserItem);
		if (pParserItem->needStream())
		{
			RDOParserSMRInfo::FileList::const_iterator it = fileList.find(pParserItem->m_type);
			if (it != fileList.end())
			{
				std::ifstream streamIn (it->second.c_str(), ios::binary);
				ASSERT(streamIn.good());

				std::ofstream streamOut(_T("C:\\Users\\������\\Documents\\1.txt"), ios::trunc | ios::binary);
				pParserItem->convert(this, streamIn, streamOut);
				streamOut.close();
			}
		}
		++it;
	}

	return CNV_OK;
}

void Converter::checkFunctionName(CREF(RDOParserSrcInfo) src_info)
{
	LPRDOFUNConstant pConstant = findFUNConstant(src_info.src_text());
	if (pConstant)
	{
		error().push_only(src_info, rdo::format(_T("��������� '%s' ��� ����������"), src_info.src_text().c_str()));
//		parser->error(_T("Second appearance of the same constant name: ") + *(_cons->getName()));
		error().push_only(pConstant->src_info(), _T("��. ������ �����������"));
		error().push_done();
	}
	LPRDOFUNSequence pSequence = findFUNSequence(src_info.src_text());
	if (pSequence)
	{
		error().push_only(src_info, rdo::format(_T("������������������ '%s' ��� ����������"), src_info.src_text().c_str()));
		error().push_only(pSequence->src_info(), _T("��. ������ �����������"));
		error().push_done();
	}
	LPRDOFUNFunction pFunction = findFUNFunction(src_info.src_text());
	if (pFunction)
	{
		error().push_only(src_info, rdo::format(_T("������� '%s' ��� ����������"), src_info.src_text().c_str()));
		error().push_only(pFunction->src_info(), _T("��. ������ �����������"));
		error().push_done();
	}
}

void Converter::checkActivityName(CREF(RDOParserSrcInfo) src_info)
{
	STL_FOR_ALL_CONST(getDPTSearchs(), it_search)
	{
		RDODPTSearch::ActivityList::const_iterator it_search_act = std::find_if((*it_search)->getActivities().begin(), (*it_search)->getActivities().end(), compareName<RDODPTSearchActivity>(src_info.src_text()));
		if (it_search_act != (*it_search)->getActivities().end())
		{
			error().push_only(src_info, rdo::format(_T("���������� '%s' ��� ����������"), src_info.src_text().c_str()));
			error().push_only((*it_search_act)->src_info(), _T("��. ������ �����������"));
			error().push_done();
//			error(_T("Activity name: ") + *_name + _T(" already defined"));
		}
	}
	STL_FOR_ALL_CONST(getDPTSomes(), it_some)
	{
		RDODPTSome::ActivityList::const_iterator it_some_act = std::find_if((*it_some)->getActivities().begin(), (*it_some)->getActivities().end(), compareName<RDODPTSomeActivity>(src_info.src_text()));
		if (it_some_act != (*it_some)->getActivities().end())
		{
			error().push_only(src_info, rdo::format(_T("���������� '%s' ��� ����������"), src_info.src_text().c_str()));
			error().push_only((*it_some_act)->src_info(), _T("��. ������ �����������"));
			error().push_done();
		}
	}
	STL_FOR_ALL_CONST(getDPTPriors(), it_prior)
	{
		RDODPTPrior::ActivityList::const_iterator it_prior_act = std::find_if((*it_prior)->getActivities().begin(), (*it_prior)->getActivities().end(), compareName<RDODPTPriorActivity>(src_info.src_text()));
		if (it_prior_act != (*it_prior)->getActivities().end())
		{
			error().push_only(src_info, rdo::format(_T("���������� '%s' ��� ����������"), src_info.src_text().c_str()));
			error().push_only((*it_prior_act)->src_info(), _T("��. ������ �����������"));
			error().push_done();
		}
	}
	LPRDODPTFreeActivity pFreeActivity = findDPTFreeActivity(src_info.src_text());
	if (pFreeActivity)
	{
		error().push_only(src_info, rdo::format(_T("���������� '%s' ��� ����������"), src_info.src_text().c_str()));
		error().push_only(pFreeActivity->src_info(), _T("��. ������ �����������"));
		error().push_done();
	}
	LPRDOOPROperation pOperation = findOPROperation(src_info.src_text());
	if (pOperation)
	{
		error().push_only(src_info, rdo::format(_T("�������� '%s' ��� ����������"), src_info.src_text().c_str()));
		error().push_only(pOperation->src_info(), _T("��. ������ �����������"));
		error().push_done();
	}
}

void Converter::checkDPTName(CREF(RDOParserSrcInfo) src_info)
{
	if (src_info.src_text().empty())
	{
		// ��������� ��� �������� � ��������� ������ �����������
		return;
	}
	DPTSearchList::const_iterator search_it = std::find_if(getDPTSearchs().begin(), getDPTSearchs().end(), compareName<RDODPTSearch>(src_info.src_text()));
	if (search_it != getDPTSearchs().end())
	{
		error().push_only(src_info, rdo::format(_T("����� '%s' ��� ����������"), src_info.src_text().c_str()));
		error().push_only((*search_it)->src_info(), _T("��. ������ �����������"));
		error().push_done();
//		error(src_info, _T("DPT name: ") + src_info.src_text() + _T(" already defined"));
	}
	DPTSomeList::const_iterator some_it = std::find_if(getDPTSomes().begin(), getDPTSomes().end(), compareName<RDODPTSome>(src_info.src_text()));
	if (some_it != getDPTSomes().end())
	{
		error().push_only(src_info, rdo::format(_T("����� '%s' ��� ����������"), src_info.src_text().c_str()));
		error().push_only((*some_it)->src_info(), _T("��. ������ �����������"));
		error().push_done();
	}
}

CLOSE_RDO_CONVERTER_NAMESPACE
