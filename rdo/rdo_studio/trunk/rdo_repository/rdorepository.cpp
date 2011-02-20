/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdorepository.cpp
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_repository/rdorepository.h"
#include "rdo_kernel/rdokernel.h"
#include "rdo_lib/rdo_simulator/rdosimwin.h"
#include "rdo_common/rdofile.h"
#include "rdo_common/rdotime.h"
// ===============================================================================

OPEN_RDO_REPOSITORY_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOThreadRepository
// ----------------------------------------------------------------------------
RDOThreadRepository::RDOThreadRepository()
	: RDOThreadMT    (_T("RDOThreadRepository"))
	, m_modelName    (_T("")                   )
	, m_modelPath    (_T("")                   )
	, m_hasModel     (false                    )
	, m_realOnlyInDlg(false                    )
{
	notifies.push_back(RT_STUDIO_MODEL_NEW                  );
	notifies.push_back(RT_STUDIO_MODEL_OPEN                 );
	notifies.push_back(RT_STUDIO_MODEL_CLOSE                );
	notifies.push_back(RT_STUDIO_MODEL_SAVE                 );
	notifies.push_back(RT_STUDIO_MODEL_SAVE_AS              );
	notifies.push_back(RT_REPOSITORY_MODEL_GET_FILEINFO     );
	notifies.push_back(RT_REPOSITORY_LOAD                   );
	notifies.push_back(RT_REPOSITORY_SAVE                   );
	notifies.push_back(RT_REPOSITORY_LOAD_BINARY            );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_OK           );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_BY_USER      );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
	notifies.push_back(RT_RUNTIME_MODEL_START_BEFORE        );
	notifies.push_back(RT_RUNTIME_TRACE_STRING              );

	m_files.resize(12);
	m_files[rdoModelObjects::RTP].m_extention = _T(".rtp");
	m_files[rdoModelObjects::RSS].m_extention = _T(".rss");
	m_files[rdoModelObjects::EVN].m_extention = _T(".evn");
	m_files[rdoModelObjects::PAT].m_extention = _T(".pat");
	m_files[rdoModelObjects::DPT].m_extention = _T(".dpt");
	m_files[rdoModelObjects::PRC].m_extention = _T(".prc");
	m_files[rdoModelObjects::FRM].m_extention = _T(".frm");
	m_files[rdoModelObjects::FUN].m_extention = _T(".fun");
	m_files[rdoModelObjects::SMR].m_extention = _T(".smr");
	m_files[rdoModelObjects::PMD].m_extention = _T(".pmd");
	m_files[rdoModelObjects::PMV].m_extention = _T(".pmv");
	m_files[rdoModelObjects::TRC].m_extention = _T(".trc");

//	m_files[rdoModelObjects::OPR].deleteifempty = true;
//	m_files[rdoModelObjects::DPT].deleteifempty = true;

	resetModelNames();

	after_constructor();
}

RDOThreadRepository::~RDOThreadRepository()
{
	m_traceFile.close();
}

void RDOThreadRepository::proc(REF(RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RT_STUDIO_MODEL_NEW:
		{
			msg.lock();
			PTR(NewModel) data = static_cast<PTR(NewModel)>(msg.param);
			newModel(data);
			msg.unlock();
			break;
		}
		case RT_STUDIO_MODEL_OPEN:
		{
			msg.lock();
			PTR(OpenFile) data = static_cast<PTR(OpenFile)>(msg.param);
			data->m_result = openModel(data->m_name);
			msg.unlock();
			break;
		}
		case RT_STUDIO_MODEL_CLOSE:
		{
			closeModel();
			break;
		}
		case RT_STUDIO_MODEL_SAVE:
		{
			rbool res = saveModel();
			msg.lock();
			if (msg.param) *static_cast<PTR(rbool)>(msg.param) = res;
			msg.unlock();
			break;
		}
		case RT_STUDIO_MODEL_SAVE_AS:
		{
			saveAsModel();
			break;
		}
		case RT_RUNTIME_MODEL_START_BEFORE:
		{
			beforeModelStart();
			break;
		}
		case RT_SIMULATOR_MODEL_STOP_OK           :
		case RT_SIMULATOR_MODEL_STOP_BY_USER      :
		case RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
		{
			stopModel();
			break;
		}
		case RT_RUNTIME_TRACE_STRING:
		{
			msg.lock();
			trace(*static_cast<PTR(tstring)>(msg.param));
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_MODEL_GET_FILEINFO:
		{
			msg.lock();
			PTR(FileInfo) data = static_cast<PTR(FileInfo)>(msg.param);
			data->m_name      = getFileName    (data->m_type);
			data->m_full_name = getFullFileName(data->m_type);
			data->m_extention = getExtention   (data->m_type);
			data->m_described = isDescribed    (data->m_type);
			data->m_readOnly  = isReadOnly     (data->m_type);
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_LOAD:
		{
			msg.lock();
			PTR(FileData) fdata = static_cast<PTR(FileData)>(msg.param);
			load(fdata->m_type, fdata->m_stream);
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_SAVE:
		{
			msg.lock();
			PTR(FileData) fdata = static_cast<PTR(FileData)>(msg.param);
			save(fdata->m_type, fdata->m_stream);
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_LOAD_BINARY:
		{
			msg.lock();
			PTR(BinaryFile) data = static_cast<PTR(BinaryFile)>(msg.param);
			loadBMP(data->m_name, data->m_stream);
			msg.unlock();
			break;
		}
		default: break;
	}
}

void RDOThreadRepository::resetModelNames()
{
	FileList::iterator it = m_files.begin();
	while (it != m_files.end())
	{
		it->resetname();
		it++;
	}
}

RDOThreadRepository::FindModel RDOThreadRepository::updateModelNames()
{
	rdo::textstream smrStream;
	loadFile(getFullFileName(rdoModelObjects::SMR), smrStream, true, true, m_files[rdoModelObjects::SMR].m_readOnly);
	rdoModelObjects::RDOSMRFileInfo fileInfo;
	kernel->simulator()->parseSMRFileInfo(smrStream, fileInfo);
	if (!fileInfo.m_error)
	{
 		m_files[rdoModelObjects::RTP].m_fileName = fileInfo.m_modelName.empty()     ? m_files[rdoModelObjects::SMR].m_fileName : fileInfo.m_modelName;
 		m_files[rdoModelObjects::RSS].m_fileName = fileInfo.m_resourceFile.empty()  ? m_files[rdoModelObjects::SMR].m_fileName : fileInfo.m_resourceFile;
		m_files[rdoModelObjects::EVN].m_fileName = fileInfo.m_modelName.empty()     ? m_files[rdoModelObjects::SMR].m_fileName : fileInfo.m_modelName;
		m_files[rdoModelObjects::PAT].m_fileName = fileInfo.m_modelName.empty()     ? m_files[rdoModelObjects::SMR].m_fileName : fileInfo.m_modelName;
 		m_files[rdoModelObjects::DPT].m_fileName = fileInfo.m_modelName.empty()     ? m_files[rdoModelObjects::SMR].m_fileName : fileInfo.m_modelName;
 		m_files[rdoModelObjects::PRC].m_fileName = fileInfo.m_modelName.empty()     ? m_files[rdoModelObjects::SMR].m_fileName : fileInfo.m_modelName;
 		m_files[rdoModelObjects::FRM].m_fileName = fileInfo.m_frameFile.empty()     ? m_files[rdoModelObjects::SMR].m_fileName : fileInfo.m_frameFile;
 		m_files[rdoModelObjects::FUN].m_fileName = fileInfo.m_modelName.empty()     ? m_files[rdoModelObjects::SMR].m_fileName : fileInfo.m_modelName;
 		m_files[rdoModelObjects::PMD].m_fileName = fileInfo.m_statisticFile.empty() ? m_files[rdoModelObjects::SMR].m_fileName : fileInfo.m_statisticFile;
 		m_files[rdoModelObjects::PMV].m_fileName = fileInfo.m_resultsFile.empty()   ? m_files[rdoModelObjects::SMR].m_fileName : fileInfo.m_resultsFile;
 		m_files[rdoModelObjects::TRC].m_fileName = fileInfo.m_traceFile.empty()      ? m_files[rdoModelObjects::SMR].m_fileName : fileInfo.m_traceFile;

		m_files[rdoModelObjects::RTP].m_described = !fileInfo.m_modelName.empty();
		m_files[rdoModelObjects::RSS].m_described = !fileInfo.m_resourceFile.empty();
		m_files[rdoModelObjects::EVN].m_described = !fileInfo.m_modelName.empty();
		m_files[rdoModelObjects::PAT].m_described = !fileInfo.m_modelName.empty();
		m_files[rdoModelObjects::DPT].m_described = !fileInfo.m_modelName.empty(); // !m_files[rdoModelObjects::OPR].m_described;
		m_files[rdoModelObjects::PRC].m_described = !fileInfo.m_modelName.empty(); // !m_files[rdoModelObjects::OPR].m_described;
		m_files[rdoModelObjects::FRM].m_described = !fileInfo.m_frameFile.empty();
		m_files[rdoModelObjects::FUN].m_described = !fileInfo.m_modelName.empty();
		m_files[rdoModelObjects::SMR].m_described = true;
		m_files[rdoModelObjects::PMD].m_described = !fileInfo.m_statisticFile.empty();
		m_files[rdoModelObjects::PMV].m_described = !fileInfo.m_resultsFile.empty();
		m_files[rdoModelObjects::TRC].m_described = !fileInfo.m_traceFile.empty();

		m_files[rdoModelObjects::RTP].m_mustExist = false;
		m_files[rdoModelObjects::RSS].m_mustExist = false;
		m_files[rdoModelObjects::EVN].m_mustExist = false;
		m_files[rdoModelObjects::PAT].m_mustExist = false;
		m_files[rdoModelObjects::DPT].m_mustExist = false; // m_files[rdoModelObjects::DPT].m_described;
		m_files[rdoModelObjects::PRC].m_mustExist = false;
		m_files[rdoModelObjects::FRM].m_mustExist = !fileInfo.m_frameFile.empty();
		m_files[rdoModelObjects::FUN].m_mustExist = false;
		m_files[rdoModelObjects::SMR].m_mustExist = true;
		m_files[rdoModelObjects::PMD].m_mustExist = !fileInfo.m_statisticFile.empty();
		m_files[rdoModelObjects::PMV].m_mustExist = false;
		m_files[rdoModelObjects::TRC].m_mustExist = false;
		return fm_ok;
	}
	else
	{
		if (fileInfo.m_modelName.empty()) return fm_smr_empty;
		std::vector< RDOThreadRepository::fileInfo >::iterator it = m_files.begin();
		while (it != m_files.end())
		{
			it->m_fileName  = m_files[rdoModelObjects::SMR].m_fileName;
			it->m_described = true;
			it->m_mustExist = true;
			it++;
		}
		return fm_smr_error;
	}
}

void RDOThreadRepository::newModel(CPTRC(NewModel) data)
{
	if (canCloseModel())
	{
		realCloseModel();
		if (data)
		{
			tstring path = data->m_path;
			tstring::size_type pos = path.find_last_of(_T('\\'));
			if (pos == tstring::npos)
			{
				pos = path.find_last_of(_T('/'));
			}
			if (pos != path.length() - 1)
			{
				path += _T('\\');
			}
			extractName(path + data->m_name + _T(".smr"));
			if (!rdo::File::exist(path))
			{
				::CreateDirectory(path.c_str(), NULL);
			}
		}
		else
		{
			m_modelName = _T("noname");
			m_modelPath = _T("");
		}
		FileList::iterator it = m_files.begin();
		while (it != m_files.end())
		{
			it->m_fileName = m_modelName;
			++it;
		}
		m_hasModel = true;
		broadcastMessage(RT_REPOSITORY_MODEL_NEW);
	}
	else
	{
		broadcastMessage(RT_REPOSITORY_MODEL_CLOSE_ERROR);
	}
}

rbool RDOThreadRepository::openModel(CREF(tstring) modelFileName)
{
	if (canCloseModel())
	{
		realCloseModel();

		rbool can_open = true;
		m_realOnlyInDlg = false;
		if (modelFileName.empty())
		{
			OpenFile data;
			broadcastMessage(RT_REPOSITORY_MODEL_OPEN_GET_NAME, &data, true);
			if (data.m_result)
			{
				m_realOnlyInDlg = data.m_readOnly;
				extractName(data.m_name);
			}
			else
			{
				can_open = false;
			}
		}
		else
		{
			extractName(modelFileName);
			can_open = !m_modelName.empty();
		}

		if (can_open)
		{
			FileList::iterator it = m_files.begin();
			while (it != m_files.end())
			{
				it->m_readOnly = m_realOnlyInDlg;
				it++;
			}
			if (rdo::File::exist(m_modelPath + m_modelName + m_files[rdoModelObjects::SMR].m_extention))
			{

				m_files[rdoModelObjects::SMR].m_fileName = m_modelName;
				m_hasModel = true;
				switch (updateModelNames())
				{
					case fm_ok       : broadcastMessage(RT_REPOSITORY_MODEL_OPEN); return true;
					case fm_smr_error: broadcastMessage(RT_REPOSITORY_MODEL_OPEN); return false;
					case fm_smr_empty: return false;
				}

			}
			else
			{
				broadcastMessage(RT_REPOSITORY_MODEL_OPEN_ERROR, &tstring(m_modelPath + m_modelName + m_files[rdoModelObjects::SMR].m_extention));
				setName(_T(""));
			}
		}

	}
	else
	{
		broadcastMessage(RT_REPOSITORY_MODEL_CLOSE_ERROR);
	}

	return false;
}

rbool RDOThreadRepository::saveModel()
{
	rbool flag = true;
	if (m_modelPath.empty())
	{
		flag = saveAsDlg();
	}
	if (flag)
	{
		broadcastMessage(RT_REPOSITORY_MODEL_SAVE);
	}
	return flag;
}

void RDOThreadRepository::saveAsModel()
{
	if (saveAsDlg())
	{
		broadcastMessage(RT_REPOSITORY_MODEL_SAVE);
	}
}

rbool RDOThreadRepository::saveAsDlg()
{
	OpenFile data;
	broadcastMessage(RT_REPOSITORY_MODEL_SAVE_GET_NAME, &data, true);
	if (data.m_result)
	{
		extractName(data.m_name);
		if (m_modelName.empty())
			return false;
		FileList::iterator it = m_files.begin();
		while (it != m_files.end())
		{
			it->m_fileName = m_modelName;
			++it;
		}
		return true;
	}
	else
	{
		return false;
	}
}

rbool RDOThreadRepository::canCloseModel()
{
	if (m_hasModel)
	{
		rbool res = true;
		broadcastMessage(RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE, &res, true);
		return res;
	}
	else
	{
		return true;
	}
}

void RDOThreadRepository::realCloseModel()
{
	if (m_hasModel)
	{
//	if (!m_modelName.empty()) {
		m_hasModel = false;
		broadcastMessage(RT_REPOSITORY_MODEL_CLOSE);
		m_modelName   = _T("");
		m_modelPath   = _T("");
		resetModelNames();
	}
}

void RDOThreadRepository::closeModel()
{
	if (canCloseModel())
	{
		realCloseModel();
	}
	else
	{
		broadcastMessage(RT_REPOSITORY_MODEL_CLOSE_ERROR);
	}
}

void RDOThreadRepository::extractName(CREF(tstring) fullName)
{
	m_modelPath = rdo::extractFilePath(fullName);

	tstring name = fullName;
	tstring::size_type pos = name.find_last_of(_T('.'));
	if (pos != tstring::npos)
	{
		tstring s;
		s.assign(&name[0], pos);
		name = s;
	}
	static char szDelims[] = _T(" \t\n\r");
	name.erase(0, name.find_first_not_of(szDelims));
	name.erase(name.find_last_not_of(szDelims) + 1, tstring::npos);
	pos = name.find_last_of(_T('\\'));
	if (pos == tstring::npos)
	{
		pos = name.find_last_of(_T('/'));
	}
	if (pos != tstring::npos)
	{
		tstring s(name, pos + 1, name.length() - pos);
		setName(s);
	}
	else
	{
		setName(_T(""));
	}
}

void RDOThreadRepository::setName(CREF(tstring) str)
{
	m_modelName = str;
	static char szDelims[] = _T(" \t\n\r");
	m_modelName.erase(0, m_modelName.find_first_not_of(szDelims));
	m_modelName.erase(m_modelName.find_last_not_of(szDelims) + 1, tstring::npos);
	if (m_modelName.empty())
	{
		m_modelPath = _T("");
		resetModelNames();
	}
}

void RDOThreadRepository::loadFile(CREF(tstring) fileName, REF(rdo::stream) stream, rbool described, rbool mustExist, REF(rbool) reanOnly) const
{
	if (described)
	{
		if (rdo::File::exist(fileName))
		{
			if (!m_realOnlyInDlg)
			{
				reanOnly = rdo::File::read_only(fileName);
			}
			else
			{
				reanOnly = true;
			}
			if (stream.isBinary())
			{
				std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary);
				stream << file.rdbuf();
				file.close();
			}
			else
			{
				std::ifstream file(fileName.c_str());
				stream << file.rdbuf();
				file.close();
			}
/*
			if (stream.getOpenMode() & std::ios::binary*)
			{
				std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary);
				file.seekg(0, std::ios::end);
				int len = file.tellg();
				file.seekg(0, std::ios::beg);
				stream.resize(len);
				file.read(stream.data(), len);
				file.close();
			}
			else
			{
				std::ifstream file(fileName.c_str());
				stream << file.rdbuf();
				file.close();
			}
*/
		}
		else
		{
			stream.setstate(std::ios_base::badbit);
			if (mustExist) stream.setstate(stream.rdstate() | std::ios_base::failbit);
		}
	}
	else
	{
		stream.setstate(std::ios_base::badbit);
	}
}

void RDOThreadRepository::saveFile(CREF(tstring) fileName, REF(rdo::stream) stream, rbool deleteIfEmpty) const
{
	if (!fileName.empty())
	{
		rbool file_exist = rdo::File::exist(fileName);
		if (!stream.str().empty() || (file_exist && !deleteIfEmpty))
		{
			if (stream.isBinary())
			{
				std::ofstream file(fileName.c_str(), std::ios::out | std::ios::binary);
				file << stream.rdbuf();
				file.close();
			}
			else
			{
				std::ofstream file(fileName.c_str());
				file << stream.rdbuf();
				file.close();
			}
		}
		else
		{
			if (file_exist && deleteIfEmpty)
			{
				rdo::File::unlink(fileName);
			}
		}
	}
}

void RDOThreadRepository::load(rdoModelObjects::RDOFileType type, REF(rdo::stream) stream)
{
	loadFile(getFullFileName(type), stream, m_files[type].m_described, m_files[type].m_mustExist, m_files[type].m_readOnly);
}

void RDOThreadRepository::save(rdoModelObjects::RDOFileType type, REF(rdo::stream) stream) const
{
	saveFile(getFullFileName(type), stream, m_files[type].m_deleteIfEmpty);
	if (type == rdoModelObjects::SMR)
	{
		const_cast<PTR(RDOThreadRepository)>(this)->updateModelNames();
	}
}

void RDOThreadRepository::loadBMP(CREF(tstring) name, REF(rdo::stream) stream) const
{
	tstring file_name = m_modelPath + name + _T(".bmp");
	if (rdo::File::exist(file_name))
	{
		std::ifstream file(file_name.c_str(), std::ios::in | std::ios::binary);
		stream << file.rdbuf();
		file.close();
	}
	else
	{
		stream.setstate(std::ios_base::badbit);
	}
}

void RDOThreadRepository::writeModelFilesInfo(REF(std::ofstream) stream) const
{
	stream << _T("Results_file   = ") << getFileExtName(rdoModelObjects::PMV) << _T("    ") << rdo::Time::local().asString() << std::endl;
	stream << _T("Run_file       = ") << getFileExtName(rdoModelObjects::SMR) << std::endl;
	stream << _T("Model_name     = ") << m_files[rdoModelObjects::SMR].m_fileName << std::endl;
	stream << _T("Resource_file  = ") << m_files[rdoModelObjects::RSS].m_fileName << m_files[rdoModelObjects::RSS].m_extention << std::endl;
}

void RDOThreadRepository::beforeModelStart()
{
	if (m_traceFile.is_open())
	{
		m_traceFile.close();
	}
	if (m_files[rdoModelObjects::TRC].m_described)
	{
		m_traceFile.open(getFullFileName(rdoModelObjects::TRC).c_str(), std::ios::out | std::ios::binary);
		if (m_traceFile.is_open())
		{
			writeModelFilesInfo(m_traceFile);
			rdo::textstream model_structure;
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_STRUCTURE, &model_structure);
			m_traceFile << std::endl << model_structure.str() << std::endl;
			m_traceFile << _T("$Tracing") << std::endl;
		}
	}
}

void RDOThreadRepository::stopModel()
{
	if (m_traceFile.is_open())
	{
		m_traceFile.close();
	}
	if (m_files[rdoModelObjects::PMV].m_described)
	{
		std::ofstream results_file;
		results_file.open(getFullFileName(rdoModelObjects::PMV).c_str(), std::ios::out | std::ios::binary);
		if (results_file.is_open())
		{
			writeModelFilesInfo(results_file);
			rdo::textstream stream;
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS_INFO, &stream);
			results_file << std::endl << stream.str() << std::endl;
			stream.str(_T(""));
			stream.clear();
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS, &stream);
			results_file << std::endl << stream.str() << std::endl;
		}
	}
}

void RDOThreadRepository::trace(CREF(tstring) str)
{
	if (m_traceFile.is_open())
	{
		m_traceFile.write(str.c_str(), str.length());
		m_traceFile.write(_T("\n"), 1);
		m_traceFile.flush();
	}
}

CLOSE_RDO_REPOSITORY_NAMESPACE
