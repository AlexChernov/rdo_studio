/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/model/model.cpp
  \author    ������ ������ (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/bind.hpp>
#include <limits>
#include <QtGui/qmessagebox.h>
#include <QtGui/qfiledialog.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdostream.h"
#include "utils/rdoanimation.h"
#include "kernel/rdokernel.h"
#include "kernel/rdothread.h"
#include "repository/rdorepository.h"
#include "simulator/runtime/rdo_exception.h"
#include "app/rdo_studio_mfc/src/model/model.h"
#include "app/rdo_studio_mfc/src/model/view.h"
#include "app/rdo_studio_mfc/src/thread.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
#include "app/rdo_studio_mfc/src/frame/view.h"
#include "app/rdo_studio_mfc/src/dialog/new_model_dialog.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditortabctrl.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdobuildedit.h"
#include "app/rdo_studio_mfc/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio_mfc/rdo_edit/rdoeditorresults.h"
#include "app/rdo_studio_mfc/rdo_tracer/rdotracer.h"
#include "app/rdo_studio_mfc/resource.h"
#include "thirdparty/pugixml/src/pugixml.hpp"
// --------------------------------------------------------------------------------

using namespace rdoEditor;
using namespace rdo::service::simulation;
using namespace rdo::simulation::report;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModel::ModelTemplateItem
// --------------------------------------------------------------------------------
RDOStudioModel::ModelTemplateItem::ModelTemplateItem()
{}

RDOStudioModel::ModelTemplateItem::ModelTemplateItem(CREF(ModelTemplateItem) copy)
	: resName (copy.resName )
	, position(copy.position)
{}

RDOStudioModel::ModelTemplateItem::ModelTemplateItem(CREF(QString) resName)
	: resName(resName)
{}

RDOStudioModel::ModelTemplateItem::ModelTemplateItem(CREF(QString) resName, ruint position)
	: resName (resName )
	, position(position)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModel
// --------------------------------------------------------------------------------
PTR(RDOStudioModel) model = NULL;

RDOStudioModel::RDOStudioModel()
	: RDOThreadGUI(_T("RDOThreadModelGUI"), static_cast<PTR(RDOKernelGUI)>(studioApp.m_pStudioGUI))
	, m_frameManager   (boost::bind(&RDOStudioModel::onChangeFrame, this, _1))
	, m_GUI_HAS_MODEL  (false                     )
	, m_GUI_CAN_RUN    (true                      )
	, m_GUI_IS_RUNNING (false                     )
	, m_openError      (false                     )
	, m_smrEmptyError  (false                     )
	, m_modelClosed    (true                      )
	, m_frmDescribed   (false                     )
	, m_timeNow        (0                         )
	, m_speed          (1                         )
	, m_showRate       (60                        )
	, m_runtimeMode    (rdo::runtime::RTM_MaxSpeed)
	, m_exitCode       (rdo::simulation::report::EC_ModelNotFound)
	, m_modify         (false                     )
	, m_buildState     (BS_UNDEFINED              )
	, m_pModelView     (NULL                      )
	, m_pModelProcView (NULL                      )
	, m_name           ("")
{
	model = this;

	Ui::MainWindow* pMainWindow = studioApp.getMainWndUI();
	ASSERT(pMainWindow);

	connect(pMainWindow->actFileNew,     SIGNAL(triggered(bool)), this, SLOT(onFileNew    ()));
	connect(pMainWindow->actFileOpen,    SIGNAL(triggered(bool)), this, SLOT(onFileOpen   ()));
	connect(pMainWindow->actFileClose,   SIGNAL(triggered(bool)), this, SLOT(onFileClose  ()));
	connect(pMainWindow->actFileSaveAll, SIGNAL(triggered(bool)), this, SLOT(onFileSaveAll()));

	connect(pMainWindow->actModelBuild, SIGNAL(triggered(bool)), this, SLOT(onModelBuild()));
	connect(pMainWindow->actModelRun,   SIGNAL(triggered(bool)), this, SLOT(onModelRun  ()));
	connect(pMainWindow->actModelStop,  SIGNAL(triggered(bool)), this, SLOT(onModelStop ()));

	connect(pMainWindow->actModelRuntimeMaxSpeed, SIGNAL(triggered(bool)), this, SLOT(onModelRuntimeMaxSpeed()));
	connect(pMainWindow->actModelRuntimeJump,     SIGNAL(triggered(bool)), this, SLOT(onModelRuntimeJump()));
	connect(pMainWindow->actModelRuntimeSync,     SIGNAL(triggered(bool)), this, SLOT(onModelRuntimeSync()));
	connect(pMainWindow->actModelRuntimePause,    SIGNAL(triggered(bool)), this, SLOT(onModelRuntimePause()));

	connect(pMainWindow->actModelShowRateInc,     SIGNAL(triggered(bool)), this, SLOT(onModelShowRateInc()));
	connect(pMainWindow->actModelShowRateIncFour, SIGNAL(triggered(bool)), this, SLOT(onModelShowRateIncFour()));
	connect(pMainWindow->actModelShowRateDecFour, SIGNAL(triggered(bool)), this, SLOT(onModelShowRateDecFour()));
	connect(pMainWindow->actModelShowRateDec,     SIGNAL(triggered(bool)), this, SLOT(onModelShowRateDec()));

	connect(studioApp.getMainWndUI()->m_pModelSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(onModelSpeedValueChanged(int)));

	QActionGroup* runtimeGroup = new QActionGroup(this);
	runtimeGroup->addAction(pMainWindow->actModelRuntimeMaxSpeed);
	runtimeGroup->addAction(pMainWindow->actModelRuntimeJump);
	runtimeGroup->addAction(pMainWindow->actModelRuntimeSync);
	runtimeGroup->addAction(pMainWindow->actModelRuntimePause);

	connect(pMainWindow->actModelFrameNext, SIGNAL(triggered(bool)), this, SLOT(onShowNextFrame()));
	connect(pMainWindow->actModelFramePrev, SIGNAL(triggered(bool)), this, SLOT(onShowPrevFrame()));

	setHasModel (m_GUI_HAS_MODEL );
	setCanRun   (m_GUI_CAN_RUN   );
	setIsRunning(m_GUI_IS_RUNNING);

	ModelTemplate modelTemplate;
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_0/template_0.smr");
	m_modelTemplates[0] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_1/template_1.smr");
	m_modelTemplates[1] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::RTP] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.rtp", 15);
	modelTemplate[rdoModelObjects::RSS] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.rss", 12);
	modelTemplate[rdoModelObjects::EVN] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.evn", 9);
	modelTemplate[rdoModelObjects::PAT] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.pat", 9);
	modelTemplate[rdoModelObjects::DPT] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.dpt", 16);
	modelTemplate[rdoModelObjects::PRC] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.prc", 10);
	modelTemplate[rdoModelObjects::FRM] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.frm", 7);
	modelTemplate[rdoModelObjects::FUN] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.fun", 10);
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.smr");
	modelTemplate[rdoModelObjects::PMD] = ModelTemplateItem(":/new_model_template/new_model_template/template_2/template_2.pmd", 10);
	m_modelTemplates[2] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::RTP] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.rtp", 98);
	modelTemplate[rdoModelObjects::RSS] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.rss", 58);
	modelTemplate[rdoModelObjects::EVN] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.evn", 67);
	modelTemplate[rdoModelObjects::PAT] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.pat", 89);
	modelTemplate[rdoModelObjects::DPT] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.dpt", 132);
	modelTemplate[rdoModelObjects::PRC] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.prc", 75);
	modelTemplate[rdoModelObjects::FRM] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.frm", 84);
	modelTemplate[rdoModelObjects::FUN] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.fun", 44);
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.smr");
	modelTemplate[rdoModelObjects::PMD] = ModelTemplateItem(":/new_model_template/new_model_template/template_3/template_3.pmd", 43);
	m_modelTemplates[3] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::RTP] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.rtp");
	modelTemplate[rdoModelObjects::RSS] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.rss");
	modelTemplate[rdoModelObjects::EVN] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.evn");
	modelTemplate[rdoModelObjects::PAT] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.pat");
	modelTemplate[rdoModelObjects::DPT] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.dpt");
	modelTemplate[rdoModelObjects::FUN] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.fun");
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.smr");
	modelTemplate[rdoModelObjects::PMD] = ModelTemplateItem(":/new_model_template/new_model_template/template_4/template_4.pmd");
	m_modelTemplates[4] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::RTP] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.rtp");
	modelTemplate[rdoModelObjects::RSS] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.rss");
	modelTemplate[rdoModelObjects::EVN] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.evn");
	modelTemplate[rdoModelObjects::PAT] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.pat");
	modelTemplate[rdoModelObjects::DPT] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.dpt");
	modelTemplate[rdoModelObjects::FUN] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.fun");
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.smr");
	modelTemplate[rdoModelObjects::PMD] = ModelTemplateItem(":/new_model_template/new_model_template/template_5/template_5.pmd");
	m_modelTemplates[5] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::RTP] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.rtp");
	modelTemplate[rdoModelObjects::RSS] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.rss");
	modelTemplate[rdoModelObjects::EVN] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.evn");
	modelTemplate[rdoModelObjects::PAT] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.pat");
	modelTemplate[rdoModelObjects::DPT] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.dpt");
	modelTemplate[rdoModelObjects::FUN] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.fun");
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.smr");
	modelTemplate[rdoModelObjects::PMD] = ModelTemplateItem(":/new_model_template/new_model_template/template_6/template_6.pmd");
	m_modelTemplates[6] = modelTemplate;

	modelTemplate.clear();
	modelTemplate[rdoModelObjects::RTP] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.rtp");
	modelTemplate[rdoModelObjects::RSS] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.rss");
	modelTemplate[rdoModelObjects::EVN] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.evn");
	modelTemplate[rdoModelObjects::PAT] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.pat");
	modelTemplate[rdoModelObjects::DPT] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.dpt");
	modelTemplate[rdoModelObjects::FUN] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.fun");
	modelTemplate[rdoModelObjects::SMR] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.smr");
	modelTemplate[rdoModelObjects::PMD] = ModelTemplateItem(":/new_model_template/new_model_template/template_7/template_7.pmd");
	m_modelTemplates[7] = modelTemplate;

	notifies.push_back(RT_STUDIO_MODEL_GET_TEXT             );
	notifies.push_back(RT_REPOSITORY_MODEL_NEW              );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN             );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN_GET_NAME    );
	notifies.push_back(RT_REPOSITORY_MODEL_OPEN_ERROR       );
	notifies.push_back(RT_REPOSITORY_MODEL_CLOSE            );
	notifies.push_back(RT_REPOSITORY_MODEL_SAVE             );
	notifies.push_back(RT_SIMULATOR_PARSE_STRING            );
	notifies.push_back(RT_SIMULATOR_PARSE_OK                );
	notifies.push_back(RT_SIMULATOR_PARSE_ERROR             );
	notifies.push_back(RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY   );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_OK           );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_BY_USER      );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
	notifies.push_back(RT_RUNTIME_MODEL_START_BEFORE        );
	notifies.push_back(RT_RUNTIME_MODEL_START_AFTER         );
	notifies.push_back(RT_RUNTIME_MODEL_STOP_BEFORE         );
	notifies.push_back(RT_DEBUG_STRING                      );
	notifies.push_back(RT_RESULT_STRING                     );

	after_constructor();
}

RDOStudioModel::~RDOStudioModel()
{
	model = NULL;
//	closeModel();
}

rbool RDOStudioModel::init()
{
	PTR(IInit) pFrameManagerInit = dynamic_cast<PTR(IInit)>(&m_frameManager);
	ASSERT(pFrameManagerInit);
	pFrameManagerInit->init();

	return true;
}

void RDOStudioModel::proc(REF(RDOThread::RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RDOThread::RT_STUDIO_MODEL_GET_TEXT:
		{
			msg.lock();
			PTR(rdo::repository::RDOThreadRepository::FileData) fdata = static_cast<PTR(rdo::repository::RDOThreadRepository::FileData)>(msg.param);
			PTR(RDOEditorEdit) edit = m_pModelView->getTab().getItemEdit(fdata->m_type);
			if (edit)
			{
				edit->save(fdata->m_stream);
			}
			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_NEW:
		{
			newModelFromRepository();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_OPEN:
		{
			openModelFromRepository();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_OPEN_ERROR:
		{
			QMessageBox::critical(
				studioApp.getMainWnd(),
				"������ �������� ������",
				QString("���������� ������� ������ '%1'.").arg(static_cast<PTR(tstring)>(msg.param)->c_str())
			);
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_SAVE:
		{
			saveModelToRepository();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_OPEN_GET_NAME:
		{
			msg.lock();
			PTR(rdo::repository::RDOThreadRepository::OpenFile) data = static_cast<PTR(rdo::repository::RDOThreadRepository::OpenFile)>(msg.param);

			QString modelName = QFileDialog::getOpenFileName(
				NULL,
				"������� ������",
				QString(),
				"���-������ (*.rdox);;���-��������� (*.smr);;��� ����� (*.*)"
			);
			data->m_result   = !modelName.isEmpty();
			data->m_name     = modelName.toStdString();
			data->m_readOnly = false;

			msg.unlock();
			break;
		}
		case RDOThread::RT_REPOSITORY_MODEL_CLOSE:
		{
			closeModelFromRepository();
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_BEFORE:
		{
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_START_AFTER:
		{
			setIsRunning(true);
			sendMessage(kernel->runtime(), RT_RUNTIME_GET_MODE, &m_runtimeMode);
			setRuntimeMode(m_runtimeMode);
			sendMessage(kernel->runtime(), RT_RUNTIME_GET_SHOWRATE, &m_showRate);
			setSpeed(m_speed);
			afterModelStart();
			studioApp.getIMainWnd()->getDockDebug().raise();
			studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_STARTED));
			studioApp.getIMainWnd()->getDockDebug().getContext().update();
			int index = m_frameManager.getLastShowedFrame();
			if (index != -1)
			{
				PTR(FrameAnimationWnd) pView = m_frameManager.getFrameView(index);
				if (pView) pView->setFocus();
			}
			studioApp.getIMainWnd()->update_start();
			break;
		}
		case RDOThread::RT_RUNTIME_MODEL_STOP_BEFORE:
		{
			studioApp.getIMainWnd()->update_stop();
			sendMessage(kernel->runtime(), RT_RUNTIME_GET_TIMENOW, &m_timeNow);
			m_frameManager.clear();
			SYSTEMTIME time_stop;
			::GetSystemTime(&time_stop);
			ruint delay = ruint(~0);
			if (m_timeStart.wYear == time_stop.wYear && m_timeStart.wMonth == time_stop.wMonth)
			{
				delay = (time_stop.wDay - m_timeStart.wDay) * 24 * 60 * 60 * 1000 + (time_stop.wHour - m_timeStart.wHour) * 60 * 60 * 1000 + (time_stop.wMinute - m_timeStart.wMinute) * 60 * 1000 + (time_stop.wSecond - m_timeStart.wSecond) * 1000 + (time_stop.wMilliseconds - m_timeStart.wMilliseconds);
			}
			else if (time_stop.wYear - m_timeStart.wYear == 1 && m_timeStart.wMonth == 12 && time_stop.wMonth == 1)
			{
				delay = (time_stop.wDay + 31 - m_timeStart.wDay) * 24 * 60 * 60 * 1000 + (time_stop.wHour - m_timeStart.wHour) * 60 * 60 * 1000 + (time_stop.wMinute - m_timeStart.wMinute) * 60 * 1000 + (time_stop.wSecond - m_timeStart.wSecond) * 1000 + (time_stop.wMilliseconds - m_timeStart.wMilliseconds);
			}
			if (delay != -1)
			{
				studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_DURATION, delay));
			}
			setCanRun   (true );
			setIsRunning(false);
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_OK:
		{
			studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_FINISHED));
			studioApp.getIMainWnd()->getDockDebug().getContext().update();

			show_result();

			studioApp.autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_BY_USER:
		{
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_STOPED));
			studioApp.getIMainWnd()->getDockDebug().getContext().update();

			show_result();

			break;
		}
		case RDOThread::RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
		{
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			show_result();
			studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_RUNTIMEERROR_STOPED));
			studioApp.getIMainWnd()->getDockBuild().clear();
			studioApp.getIMainWnd()->getDockBuild().raise();
			studioApp.getIMainWnd()->getDockBuild().appendString(rdo::format(IDS_MODEL_RUNTIMEERROR));
			std::vector< FileMessage > errors;
			studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			STL_FOR_ALL_CONST(errors, it)
			{
				studioApp.getIMainWnd()->getDockBuild().appendString(*it);
				if (it->getType() == FileMessage::MT_WARNING)
				{
					warnings_cnt++;
				}
				else
				{
					errors_cnt++;
				}
			}
			if (errors_cnt || warnings_cnt)
			{
//				const_cast<PTR(rdoEditCtrl::RDOBuildEdit)>(output->getBuild())->showFirstError();
			}

			studioApp.autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_OK:
		{
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			std::vector<FileMessage> errors;
			studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			STL_FOR_ALL_CONST(errors, it)
			{
				studioApp.getIMainWnd()->getDockBuild().appendString(*it);
				if (it->getType() == FileMessage::MT_WARNING)
				{
					warnings_cnt++;
				}
				else
				{
					errors_cnt++;
				}
			}
			studioApp.getIMainWnd()->getDockBuild().appendString(rdo::format(IDS_MODEL_BUILDING_RESULTS, errors_cnt, warnings_cnt));
			if (errors_cnt || warnings_cnt)
			{
//				studioApp.getIMainWnd()->getDockBuild().getContext().showFirstError();
			}
			m_buildState = BS_COMPLETE;
			PTR(RPMethodProc2RDO) pMethod = getProc2rdo();
			if (pMethod && pMethod->checkModelStructure())
			{
				pMethod->generate();
			}
			::GetSystemTime(&m_timeStart);
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_ERROR:
		{
			setIsRunning(false);
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_EXITCODE, &m_exitCode);
			std::vector<FileMessage> errors;
			studioApp.m_pStudioGUI->sendMessage(kernel->simulator(), RDOThread::RT_SIMULATOR_GET_ERRORS, &errors);
			int errors_cnt   = 0;
			int warnings_cnt = 0;
			STL_FOR_ALL_CONST(errors, it)
			{
				studioApp.getIMainWnd()->getDockBuild().appendString(*it);
				if (it->getType() == FileMessage::MT_WARNING)
				{
					warnings_cnt++;
				}
				else
				{
					errors_cnt++;
				}
			}
			studioApp.getIMainWnd()->getDockBuild().appendString(rdo::format(IDS_MODEL_BUILDING_RESULTS, errors_cnt, warnings_cnt));
			if (errors_cnt || warnings_cnt)
			{
				studioApp.getIMainWnd()->getDockBuild().getContext().showFirstError();
			}

			setCanRun(true);
			m_buildState  = BS_ERROR;
			studioApp.autoCloseByModel();
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_ERROR_SMR_EMPTY:
		{
			m_smrEmptyError = true;
			m_buildState    = BS_ERROR;
			break;
		}
		case RDOThread::RT_SIMULATOR_PARSE_STRING:
		{
			msg.lock();
			studioApp.getIMainWnd()->getDockBuild().appendString(*static_cast<PTR(tstring)>(msg.param));
			msg.unlock();
			break;
		}
		case RDOThread::RT_DEBUG_STRING:
		{
			msg.lock();
			studioApp.getIMainWnd()->getDockDebug().appendString(*static_cast<PTR(tstring)>(msg.param));
			msg.unlock();
			break;
		}
		case RDOThread::RT_RESULT_STRING:
		{
			msg.lock();
			studioApp.getIMainWnd()->getDockResults().appendString(*static_cast<PTR(tstring)>(msg.param));
			msg.unlock();
			break;
		}
	}
}

void RDOStudioModel::show_result()
{
	rdo::textstream model_results;
	sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS, &model_results);
	tstring str = model_results.str();
	if (!str.empty())
	{
		rdo::repository::RDOThreadRepository::FileInfo data(rdoModelObjects::PMV);
		studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
		if (!data.m_described)
		{
			studioApp.getIMainWnd()->getDockDebug().appendString(_T("���������� �� ����� �������� � ����, �.�. � SMR �� ��������� Results_file\n"));
		}
		studioApp.getIMainWnd()->getDockResults().getContext().clearAll();
		studioApp.getIMainWnd()->getDockResults().raise();
		studioApp.getIMainWnd()->getDockResults().appendString(str);
	}
}

rbool RDOStudioModel::newModel(CREF(tstring) modelName, CREF(tstring) modelPath, ruint templateIndex)
{
	m_templateIndex = templateIndex;
	studioApp.getIMainWnd()->getDockBuild  ().clear();
	studioApp.getIMainWnd()->getDockDebug  ().clear();
	studioApp.getIMainWnd()->getDockResults().clear();
	studioApp.getIMainWnd()->getDockFind   ().clear();
	rdo::repository::RDOThreadRepository::NewModel data;
	data.m_name = modelName;
	data.m_path = modelPath;
	studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_NEW, &data);
	return true;
}

rbool RDOStudioModel::openModel(CREF(tstring) modelName)
{
	if (isRunning())
	{
		AfxGetMainWnd()->MessageBox(rdo::format(ID_MSG_MODEL_NEED_STOPED_FOR_OPEN).c_str(), NULL, MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	if (!closeModel())
	{
		return false;
	}
	studioApp.getIMainWnd()->getDockBuild  ().clear();
	studioApp.getIMainWnd()->getDockDebug  ().clear();
	studioApp.getIMainWnd()->getDockResults().clear();
	studioApp.getIMainWnd()->getDockFind   ().clear();
	studioApp.getIMainWnd()->getDockDebug().raise();
	studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_LOADING_BEGIN));
	studioApp.getIMainWnd()->getDockDebug().getContext().update();
	m_openError     = false;
	m_smrEmptyError = false;
	m_modelClosed   = false;
	rdo::repository::RDOThreadRepository::OpenFile data(modelName);
	studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_OPEN, &data);
	if (data.m_result && !m_openError && !m_smrEmptyError)
	{
		rdo::binarystream stream;
		rdo::repository::RDOThreadRepository::FileData fileData(rdoModelObjects::PMV, stream);
		studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &fileData);
		studioApp.getIMainWnd()->getDockResults().appendString(stream.str());
		studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_LOADING_OK));
		studioApp.setLastProjectName(getFullName());
	}
	else
	{
		m_modelClosed = true;
		if (m_smrEmptyError)
		{
			QMessageBox::critical(studioApp.getMainWnd(), _T("������ �������� ������"), _T("� smr-����� �� ������� ��� ������"));
			closeModel();
		}
		else
		{
			studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_LOADING_FAILD));
		}
	}
	return data.m_result;
}

rbool RDOStudioModel::saveModel() const
{
	rbool res = true;
	studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_SAVE, &res);
	return res;
}

rbool RDOStudioModel::closeModel()
{
	if (isRunning())
	{
		QMessageBox::warning(studioApp.getMainWnd(), "RAO-Studio", rdo::format(ID_MSG_MODEL_NEED_STOPED_FOR_CLOSE).c_str());
		return false;
	}

	if (!canCloseModel())
	{
		return false;
	}

	setHasModel(false);
	resetView();
	stopModel();
	studioApp.getIMainWnd()->getDockBuild  ().clear();
	studioApp.getIMainWnd()->getDockDebug  ().clear();
	studioApp.getIMainWnd()->getDockResults().clear();
	studioApp.getIMainWnd()->getDockFind   ().clear();
	studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_CLOSE);
	return true;
}

rbool RDOStudioModel::buildModel()
{
	if (hasModel() && !isRunning() && saveModel())
	{
		studioApp.getIMainWnd()->getDockBuild().clear();
		studioApp.getIMainWnd()->getDockDebug().clear();
		studioApp.getIMainWnd()->getDockResults().clear();
		studioApp.getIMainWnd()->getDockBuild().raise();
		studioApp.getIMainWnd()->getDockBuild().appendString(rdo::format(IDS_MODEL_BUILDING_BEGIN));
		studioApp.getIMainWnd()->getDockBuild().getContext().update();
		m_buildState = BS_UNDEFINED;
		studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_BUILD);
		return m_buildState == BS_COMPLETE;
	}
	return false;
}

rbool RDOStudioModel::runModel()
{
	if (buildModel())
	{
		setCanRun(false);
		studioApp.getIMainWnd()->getDockBuild().clear();
		studioApp.getIMainWnd()->getDockDebug().clear();
		studioApp.getIMainWnd()->getDockResults().clear();
		studioApp.getIMainWnd()->getDockBuild().raise();
		studioApp.getIMainWnd()->getDockBuild().getContext().update();
		studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_RUN);
		return true;
	}
	return false;
}

rbool RDOStudioModel::stopModel() const
{
	if (hasModel() && isRunning())
	{
		studioApp.broadcastMessage(RDOThread::RT_STUDIO_MODEL_STOP);
		return true;
	}
	return false;
}

void RDOStudioModel::createView()
{
	ASSERT(m_pModelView == NULL);
	m_pModelView = new RDOStudioModelView(NULL);
	m_pModelView->setModel(this);
	studioApp.getIMainWnd()->addSubWindow(m_pModelView);
	m_pModelView->parentWidget()->setWindowIcon(QIcon(QString::fromUtf8(":/images/images/mdi_model.png")));
}

void RDOStudioModel::resetView()
{
	if (m_pModelView)
	{
		m_pModelView->setModel(NULL);
	}
}

void RDOStudioModel::createProcView()
{
	ASSERT(m_pModelProcView == NULL);
	m_pModelProcView = new RPViewQt();
	studioApp.getIMainWnd()->addSubWindow(m_pModelProcView);
	m_pModelProcView->parentWidget()->setWindowIcon(QIcon(QString::fromUtf8(":/images/images/mdi_flowchart.png")));
}

void RDOStudioModel::newModelFromRepository()
{
	setHasModel(true);

	PTR(RPMethodProc2RDO) pMethod = getProc2rdo();
	if (pMethod)
	{
		createProcView();
		pMethod->makeFlowChart(rpMethod::project);
	}

	createView();
	rdo::repository::RDOThreadRepository::FileInfo data_smr(rdoModelObjects::RDOX);
	studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data_smr);
	setName(data_smr.m_name);

	for (int i = 0; i < m_pModelView->getTab().count(); i++)
	{
		PTR(RDOEditorEdit) edit = m_pModelView->getTab().getItemEdit(i);
		edit->setReadOnly(false);
		edit->clearAll();
		if (m_templateIndex.is_initialized() && m_modelTemplates.find(*m_templateIndex) != m_modelTemplates.end())
		{
			ModelTemplate::const_iterator it = m_modelTemplates[*m_templateIndex].find(m_pModelView->getTab().indexToType(i));
			if (it != m_modelTemplates[*m_templateIndex].end())
			{
				ASSERT(!it->second.resName.isEmpty())
				QString resourceData;
				QFile   file(it->second.resName);
				if (file.open(QIODevice::ReadOnly) && file.isOpen())
				{
					resourceData = file.readAll();
				}
				if (!resourceData.isEmpty())
				{
					edit->replaceCurrent(resourceData.toStdString());
					edit->scrollToLine(0);
					if (it->second.position.is_initialized())
					{
						edit->setCurrentPos(*it->second.position);
					}
				}
			}
//			edit->setModifyFalse();
//			edit->clearUndoBuffer();
		}
	}

	studioApp.setLastProjectName(getFullName());
	if (m_templateIndex.is_initialized())
	{
		saveModel();
	}
}

void RDOStudioModel::openModelFromRepository()
{
	setHasModel(true);

	PTR(RPMethodProc2RDO) pMethod = getProc2rdo();
	if (pMethod)
	{
		createProcView();
		loadFromXML();
	}

	PTR(CWnd) active = CWnd::GetActiveWindow();
	createView();
	rdo::repository::RDOThreadRepository::FileInfo data_smr(rdoModelObjects::RDOX);
	studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data_smr);
	setName(data_smr.m_name);

	int cnt = m_pModelView->getTab().count();
	studioApp.getMainWndUI()->statusBar()->beginProgress(0, cnt * 2 + 1);
	studioApp.getMainWndUI()->statusBar()->stepProgress();
	for (int i = 0; i < cnt; i++)
	{
		PTR(RDOEditorEdit) edit = m_pModelView->getTab().getItemEdit(i);
		edit->setReadOnly(false);
		edit->clearAll();
		rdo::binarystream stream;
		rbool canLoad = true;
		rdoModelObjects::RDOFileType type = m_pModelView->getTab().indexToType(i);
		if (m_pModelView->getTab().typeSupported(type))
		{
			rdo::repository::RDOThreadRepository::FileData fileData(type, stream);
			studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_LOAD, &fileData);
		}
		else
		{
			canLoad = false;
		}
		studioApp.getMainWndUI()->statusBar()->stepProgress();
		if (canLoad)
		{
			rdo::repository::RDOThreadRepository::FileInfo data(type);
			studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
			rbool stream_error = stream.rdstate() & std::ios_base::failbit ? true : false;
			if (!stream_error)
			{
				edit->load(stream);
				edit->setReadOnly(data.m_readOnly);
				if (data.m_readOnly)
				{
					studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_FILE_READONLY, tstring(data.m_name + data.m_extention).c_str()));
				}
			}
			else
			{
				int obj = 0;
				switch (type)
				{
				case rdoModelObjects::RTP: obj = IDS_MODEL_RESOURCETYPES; break;
				case rdoModelObjects::RSS: obj = IDS_MODEL_RESOURCES;     break;
				case rdoModelObjects::EVN: obj = IDS_MODEL_EVENTS;        break;
				case rdoModelObjects::PAT: obj = IDS_MODEL_PATTERNS;      break;
				case rdoModelObjects::DPT: obj = IDS_MODEL_DPTS;          break;
				case rdoModelObjects::FRM: obj = IDS_MODEL_FRAMES;        break;
				case rdoModelObjects::FUN: obj = IDS_MODEL_FUNCTIONS;     break;
				case rdoModelObjects::PMD: obj = IDS_MODEL_PMDS;          break;
				}
				if (obj)
				{
					studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_CANNOT_LOAD, rdo::format(obj).c_str(), data.m_fullName.c_str()));
					studioApp.getIMainWnd()->getDockDebug().getContext().update();
				}
				m_openError = true;
			}
		}
		edit->setCurrentPos(0);
		edit->setModifyFalse();
		edit->clearUndoBuffer();
		studioApp.getMainWndUI()->statusBar()->stepProgress();
	}
	studioApp.getMainWndUI()->statusBar()->endProgress();

	updateFrmDescribed();

	if (active) active->SetFocus();
}

void RDOStudioModel::saveModelToRepository()
{
	rbool smr_modified = false;
	rbool wasSaved     = false;
	PTR(RDOEditorEdit) smr_edit = m_pModelView->getTab().getItemEdit(rdoModelObjects::SMR);
	if (smr_edit->isModify())
	{
		rdo::binarystream stream;
		smr_edit->save(stream);
		m_smrEmptyError = false;
		rdo::repository::RDOThreadRepository::FileData fileData(rdoModelObjects::SMR, stream);
		studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_SAVE, &fileData);
		if (m_smrEmptyError)
		{
			QMessageBox::critical(studioApp.getMainWnd(), _T("������ ������ ������"), _T("� smr-����� �� ������� ��� ������, ������ �� ����� ��������"));
			return;
		}
		smr_modified = true;
	}

	int cnt = m_pModelView->getTab().count();
	int progress_cnt = 0;
	for (int i = 0; i < cnt; i++)
	{
		if (smr_modified || m_pModelView->getTab().getItemEdit(i)->isModify())
		{
			progress_cnt++;
		}
	}
	if (progress_cnt)
	{
		studioApp.getMainWndUI()->statusBar()->beginProgress(0, progress_cnt * 2 + 1);
		studioApp.getMainWndUI()->statusBar()->stepProgress();
		for (int i = 0; i < cnt; i++)
		{
			PTR(RDOEditorEdit) edit = m_pModelView->getTab().getItemEdit(i);
			if (smr_modified || edit->isModify())
			{
				rdo::binarystream stream;
				edit->save(stream);
				studioApp.getMainWndUI()->statusBar()->stepProgress();
				rdoModelObjects::RDOFileType type = m_pModelView->getTab().indexToType(i);
				switch (type)
				{
				case rdoModelObjects::RTP:
				case rdoModelObjects::RSS:
				case rdoModelObjects::EVN:
				case rdoModelObjects::PAT:
				case rdoModelObjects::DPT:
				case rdoModelObjects::PRC:
				case rdoModelObjects::FRM:
				case rdoModelObjects::FUN:
				case rdoModelObjects::PMD:
					{
						rdo::repository::RDOThreadRepository::FileData fileData(type, stream);
						studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_SAVE, &fileData);
						break;
					}
				default: break;
				}
				edit->setModifyFalse();
			}
			studioApp.getMainWndUI()->statusBar()->stepProgress();
		}
		studioApp.getMainWndUI()->statusBar()->endProgress();
		wasSaved = true;
	}

	rdo::repository::RDOThreadRepository::FileInfo data(rdoModelObjects::RDOX);
	studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
	setName(data.m_name);

	if (getProc2rdo())
	{
		saveToXML();
	}

	studioApp.getMainWndUI()->insertMenuFileReopenItem(getFullName());

	if (smr_modified)
	{
		updateFrmDescribed();
	}
}

void RDOStudioModel::saveToXML()
{
	// ������� ��������:
	pugi::xml_document doc;
	// ����� ������ ���� ���������:
	pugi::xml_node node = doc.append_child(_T("Model"));
	// ��������� �� ����������� ������� saveToXML(parentNode), ������� �������� ������� ���������� � ����:
	rpMethod::project->saveToXML(node);

	// ������� ���� '.prcx' � ������� �����������:
	rdo::repository::RDOThreadRepository::FileInfo fileInfo(rdoModelObjects::PRCX);
	sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &fileInfo);

	// ������������� ��������� ���� ��� �������� ������:
	std::ofstream outFile(fileInfo.m_fullName.c_str());
	// ��������� �������� ���� ����� �� ������� ������ �����-������:
	if (outFile.good())
	{
		doc.save(outFile);
		outFile.close();
	}
}

void RDOStudioModel::loadFromXML()
{
	// ������� ��������:
	pugi::xml_document doc;

	rdo::repository::RDOThreadRepository::FileInfo fileInfo(rdoModelObjects::PRCX);
	studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &fileInfo);

	// ��������� ����������� xml-���� � ��������� ����� �� ������ �����-������:
	std::ifstream inFile(fileInfo.m_fullName.c_str());
	if (inFile.good())
	{
		// ��������� �������� � ��������� �� ������� ������ �������� � ������� ����:
		if (doc.load(inFile))
		{
			pugi::xml_node node = doc.child(_T("Model"));
			if (!node.empty())
			{
				// ��������� �� ����������� ������� loadFromXML(node), ������� �������� �������� ������� �� �����:
				rpMethod::project->loadFromXML(node.first_child());
			}
		}
		inFile.close();
	}
	else
	{
		PTR(RPMethodProc2RDO) pMethod = getProc2rdo();
		pMethod->makeFlowChart(rpMethod::project);
	}
}

tstring RDOStudioModel::getFullName() const
{
	rdo::repository::RDOThreadRepository::FileInfo data(rdoModelObjects::RDOX);
	studioApp.m_pStudioGUI->sendMessage(kernel->repository(), RDOThread::RT_REPOSITORY_MODEL_GET_FILEINFO, &data);
	return data.m_fullName;
}

void RDOStudioModel::updateFrmDescribed()
{
//	frmDescribed = kernel->repository()->isDescribed(rdoModelObjects::FRM);
	m_frmDescribed = true;
}

rbool RDOStudioModel::canCloseModel()
{
	rbool result = true;
	if (isModify())
	{
		switch (QMessageBox::question(studioApp.getMainWnd(), "RAO-Studio", rdo::format(ID_MSG_MODELSAVE_QUERY).c_str(), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
		{
			case QMessageBox::Yes   : result = saveModel(); break;
			case QMessageBox::No    : result = true; break;
			case QMessageBox::Cancel: result = false; break;
		}
	}
	return result;
}

void RDOStudioModel::closeModelFromRepository()
{
	if (m_pModelProcView)
	{
		m_pModelProcView->parentWidget()->close();
		m_pModelProcView = NULL;
	}
	setHasModel(false);
	m_pModelView->parentWidget()->close();
	m_pModelView  = NULL;
	m_modelClosed = true;
	setName("");
}

CREF(tstring) RDOStudioModel::getName() const
{
	return m_name;
}

void RDOStudioModel::setName(CREF(tstring) str)
{
	tstring newName(str);
	rdo::trim(newName);

	if (m_name != newName)
	{
		m_name = newName;

		if (m_pModelView)
		{
			if (studioApp.getShowCaptionFullName())
			{
				m_pModelView->parentWidget()->setWindowTitle(QString::fromStdString(rdo::format(IDS_MODEL_NAME, getFullName().c_str())));
			}
			else
			{
				m_pModelView->parentWidget()->setWindowTitle(QString::fromStdString(rdo::format(IDS_MODEL_NAME, m_name.c_str())));
			}
		}
	}
}

void RDOStudioModel::afterModelStart()
{
	m_frameManager.clear();

	if (isFrmDescribed())
	{
		studioApp.getIMainWnd()->getDockDebug().raise();
		studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_RESOURCE_LOADING_BEGIN));
		studioApp.getIMainWnd()->getDockDebug().getContext().update();

		std::list< tstring > frames;
		std::list< tstring > bitmaps;
		rdo::service::simulation::RDOThreadSimulator::GetList getListFrames (rdo::service::simulation::RDOThreadSimulator::GetList::frames,  &frames );
		rdo::service::simulation::RDOThreadSimulator::GetList getListBitmaps(rdo::service::simulation::RDOThreadSimulator::GetList::bitmaps, &bitmaps);
		sendMessage(kernel->simulator(), RT_SIMULATOR_GET_LIST, &getListFrames );
		sendMessage(kernel->simulator(), RT_SIMULATOR_GET_LIST, &getListBitmaps);
		STL_FOR_ALL_CONST(bitmaps, bmp_it)
		{
			m_frameManager.insertBitmap(*bmp_it);
		}
		STL_FOR_ALL_CONST(frames, frame_it)
		{
			m_frameManager.insertFrame(*frame_it);
		}
		m_timeNow = 0;
		ruint initFrameNumber = kernel->simulator()->getInitialFrameNumber();
		if (initFrameNumber != ruint(~0))
		{
			--initFrameNumber;
		}
		m_frameManager.setLastShowedFrame(initFrameNumber);
		if (getRuntimeMode() != rdo::runtime::RTM_MaxSpeed && initFrameNumber < m_frameManager.count())
		{
			PTR(FrameAnimationWnd) pView = m_frameManager.createView(initFrameNumber);
			if (pView)
			{
				m_frameManager.getFrameView(initFrameNumber)->setFocus();
			}
		}
		studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(IDS_MODEL_RESOURCE_LOADING_OK));
		studioApp.getIMainWnd()->getDockDebug().getContext().update();
	}
	else
	{
		m_timeNow = 0;
		m_frameManager.setLastShowedFrame(ruint(~0));
	}
}

PTR(RPMethodProc2RDO) RDOStudioModel::getProc2rdo() const
{
	RPMethodManager::MethodList::const_iterator it = studioApp.getMethodManager().getList().begin();
	while (it != studioApp.getMethodManager().getList().end())
	{
		PTR(rpMethod::RPMethod) pMethod = *it;
		ASSERT(pMethod);
		if (pMethod->getClassName() == _T("RPMethodProc2RDO"))
		{
			PTR(RPMethodProc2RDO) pProc2RDO = dynamic_cast<PTR(RPMethodProc2RDO)>(pMethod);
			ASSERT(pProc2RDO);
			return pProc2RDO;
		}
		it++;
	}
	return NULL;
}

void RDOStudioModel::updateStyleOfAllModel() const
{
	if (m_pModelView)
	{
		for (int i = 0; i < m_pModelView->getTab().count(); i++)
		{
			m_pModelView->getTab().getItemEdit(i)->setEditorStyle(&studioApp.getStyle()->style_editor);
		}
	}

	m_frameManager.updateStyles();
}

rbool RDOStudioModel::isPrevModelClosed() const
{
	return m_modelClosed;
}

void RDOStudioModel::setRuntimeMode(const rdo::runtime::RunTimeMode value)
{
	if (isRunning())
	{
		m_runtimeMode = value;
		sendMessage(kernel->runtime(), RT_RUNTIME_SET_MODE, &m_runtimeMode);
		g_pTracer->setRuntimeMode(getRuntimeMode());
		switch (getRuntimeMode())
		{
			case rdo::runtime::RTM_MaxSpeed: closeAllFrame(); break;
			default:
			{
				PTR(FrameAnimationWnd) pView = m_frameManager.getFrameViewFirst();
				if (!pView)
				{
					m_frameManager.createView(m_frameManager.getLastShowedFrame());
				}
				break;
			}
		}
	}
	setUpActions();
}

tstring RDOStudioModel::getLastBreakPointName()
{
	tstring str;
	sendMessage(kernel->runtime(), RT_RUNTIME_GET_LAST_BREAKPOINT, &str);
	return str;
}

double RDOStudioModel::getSpeed() const
{
	return m_speed;
}

void RDOStudioModel::setSpeed(double persent)
{
	if (persent >= 0 && persent <= 1)
	{
		m_speed = persent;
		if (isRunning())
		{
			sendMessage(kernel->runtime(), RT_RUNTIME_SET_SPEED, &m_speed);
		}
		setUpActions();
	}
}

double RDOStudioModel::getShowRate() const
{
	return m_showRate;
}

void RDOStudioModel::setShowRate(double value)
{
	if (!isRunning())
		return;

	if (value >= DBL_MIN && value <= DBL_MAX)
	{
		m_showRate = value;
		sendMessage(kernel->runtime(), RT_RUNTIME_SET_SHOWRATE, &m_showRate);
		setUpActions();
	}
}

void RDOStudioModel::onShowNextFrame()
{
	m_frameManager.showNextFrame();
}

void RDOStudioModel::onShowPrevFrame()
{
	m_frameManager.showPrevFrame();
}

int RDOStudioModel::getFrameCount() const
{
	return m_frameManager.count();
}

CPTR(char) RDOStudioModel::getFrameName(int index) const
{
	return m_frameManager.getFrameName(index).c_str();
}

void RDOStudioModel::showFrame(int index)
{
	m_frameManager.showFrame(index);
}

void RDOStudioModel::closeAllFrame()
{
	m_frameManager.closeAll();
}

rbool RDOStudioModel::hasModel() const
{
	return m_GUI_HAS_MODEL;
}

void RDOStudioModel::setHasModel(rbool value)
{
	m_GUI_HAS_MODEL = value;
	setUpActions();
}

void RDOStudioModel::setCanRun(rbool value)
{
	m_GUI_CAN_RUN = value;
	setUpActions();
}

void RDOStudioModel::setUpActions()
{
	Ui::MainWindow* pMainWindow = studioApp.getMainWndUI();
	if (!pMainWindow)
		return;

	pMainWindow->actFileNew->setEnabled(canNew());
	pMainWindow->actFileOpen->setEnabled(canOpen());
	pMainWindow->actFileSaveAll->setEnabled(canSave());
	pMainWindow->actFileClose->setEnabled(canClose());

	pMainWindow->actModelBuild->setEnabled(canBuild());
	pMainWindow->actModelRun->setEnabled(canRun());
	pMainWindow->actModelStop->setEnabled(isRunning());

	pMainWindow->actModelRuntimeMaxSpeed->setEnabled(isRunning());
	pMainWindow->actModelRuntimeJump->setEnabled    (isRunning());
	pMainWindow->actModelRuntimeSync->setEnabled    (isRunning());
	pMainWindow->actModelRuntimePause->setEnabled   (isRunning());
	pMainWindow->actModelRuntimeMaxSpeed->setChecked(getRuntimeMode() == rdo::runtime::RTM_MaxSpeed);
	pMainWindow->actModelRuntimeJump->setChecked    (getRuntimeMode() == rdo::runtime::RTM_Jump);
	pMainWindow->actModelRuntimeSync->setChecked    (getRuntimeMode() == rdo::runtime::RTM_Sync);
	pMainWindow->actModelRuntimePause->setChecked   (getRuntimeMode() == rdo::runtime::RTM_Pause || getRuntimeMode() == rdo::runtime::RTM_BreakPoint);

	rbool canShowRate = isRunning() && getRuntimeMode() == rdo::runtime::RTM_Sync;
	pMainWindow->actModelShowRateInc->setEnabled    (canShowRate && getShowRate() * 1.5 <= DBL_MAX);
	pMainWindow->actModelShowRateIncFour->setEnabled(canShowRate && getShowRate() * 4.0 <= DBL_MAX);
	pMainWindow->actModelShowRateDecFour->setEnabled(canShowRate && getShowRate() / 4.0 >= DBL_MIN);
	pMainWindow->actModelShowRateDec->setEnabled    (canShowRate && getShowRate() / 1.5 >= DBL_MIN);

	pMainWindow->actModelFrameNext->setEnabled(m_frameManager.canShowNextFrame());
	pMainWindow->actModelFramePrev->setEnabled(m_frameManager.canShowPrevFrame());

	tstring runTimeMode;
	if (isRunning())
	{
		switch (getRuntimeMode())
		{
		case rdo::runtime::RTM_MaxSpeed  : runTimeMode = rdo::format(ID_STATUSBAR_MODEL_RUNTIME_MAXSPEED); break;
		case rdo::runtime::RTM_Jump      : runTimeMode = rdo::format(ID_STATUSBAR_MODEL_RUNTIME_JUMP    ); break;
		case rdo::runtime::RTM_Sync      : runTimeMode = rdo::format(ID_STATUSBAR_MODEL_RUNTIME_SYNC    ); break;
		case rdo::runtime::RTM_Pause     : runTimeMode = rdo::format(ID_STATUSBAR_MODEL_RUNTIME_PAUSE   ); break;
		case rdo::runtime::RTM_BreakPoint: runTimeMode = rdo::format(ID_STATUSBAR_MODEL_RUNTIME_BREAKPOINT, getLastBreakPointName().c_str()); break;
		}
	}
	studioApp.getMainWndUI()->statusBar()->update<StatusBar::SB_MODEL_RUNTYPE>(QString::fromStdString(runTimeMode));

	studioApp.getMainWndUI()->statusBar()->update<StatusBar::SB_MODEL_SPEED>(
		getRuntimeMode() != rdo::runtime::RTM_MaxSpeed || !isRunning()
			? QString("��������: %1%").arg(rsint(getSpeed() * 100))
			: ""
	);

	QString showRateStr;
	if (isRunning())
	{
		showRateStr = "�������: ";
		switch (getRuntimeMode())
		{
		case rdo::runtime::RTM_MaxSpeed:
		case rdo::runtime::RTM_Jump    :
			showRateStr += "�������������";
			break;

		case rdo::runtime::RTM_Pause     :
		case rdo::runtime::RTM_BreakPoint:
			showRateStr += "0.0";
			break;

		case rdo::runtime::RTM_Sync:
			{
				double showRate = model->getShowRate();
				if (showRate < 1e-10 || showRate > 1e10)
				{
					showRateStr += QString::fromStdString(rdo::format("%e", showRate));
				}
				else if (showRate >= 1)
				{
					showRateStr += QString::fromStdString(rdo::format("%1.1f", showRate));
				}
				else
				{
					showRateStr += QString::fromStdString(rdo::format("%1.10f", showRate));
				}
			}
			break;
		}
	}
	studioApp.getMainWndUI()->statusBar()->update<StatusBar::SB_MODEL_SHOWRATE>(showRateStr);
}

void RDOStudioModel::update()
{
	sendMessage(kernel->runtime(), RT_RUNTIME_GET_TIMENOW, &m_timeNow);

	studioApp.getMainWndUI()->statusBar()->update<StatusBar::SB_MODEL_TIME>(QString("�����: %1").arg(m_timeNow));

	rdo::runtime::RunTimeMode rm;
	sendMessage(kernel->runtime(), RT_RUNTIME_GET_MODE, &rm);
	if (rm != getRuntimeMode())
	{
		if (rm == rdo::runtime::RTM_BreakPoint)
		{
			studioApp.getIMainWnd()->getDockDebug().appendString(rdo::format(_T("����� � %f ��-�� ����� '%s'\n"), getTimeNow(), getLastBreakPointName().c_str()));
		}
		setRuntimeMode(rm);
	}
	if (getRuntimeMode() == rdo::runtime::RTM_MaxSpeed)
	{
		return;
	}
	int frames = getFrameCount();
	for (int i = 0; i < frames; i++)
	{
		if (m_frameManager.isChanged())
		{
			break;
		}
		PTR(FrameAnimationWnd) pView = m_frameManager.getFrameView(i);
		if (pView)
		{
			//! @todo qt: ���������� ������ ���������.
			//! � pView::paintEvent ����������� RT_RUNTIME_GET_FRAME, ���� ��� ��� �� �������.
			//! ����� ������������ ������� �����.
			if (pView->isVisible())
			{
				try
				{
					rdo::animation::Frame frame;
					rdo::runtime::RDOThreadRunTime::GetFrame getFrame(&frame, i);
					sendMessage(kernel->runtime(), RT_RUNTIME_GET_FRAME, &getFrame);
					m_frameManager.showFrame(&frame, i);
				}
				catch (REF(rdo::runtime::RDORuntimeException))
				{
					sendMessage(kernel->runtime(), RT_SIMULATOR_MODEL_STOP_RUNTIME_DELAY);
					return;
				}
			}
		}
	}
}

rbool RDOStudioModel::isModify() const
{
	if (!m_pModelView)
		return false;

	rbool result = false;

	for (int i = 0; i < getTab()->count(); i++)
	{
		if (getTab()->getItemEdit(i)->isModify())
		{
			result = true;
			break;
		}
	}

	if (m_modify != result)
	{
		m_modify = result;
	}

	return result;
}

rbool RDOStudioModel::canNew() const
{
	return (canRun() || !hasModel());
}

rbool RDOStudioModel::canOpen() const
{
	return (canRun() || !hasModel());
}

rbool RDOStudioModel::canSave() const
{
	return hasModel() && isModify();
}

rbool RDOStudioModel::canClose() const
{
	return hasModel() && !isRunning();
}

rbool RDOStudioModel::canBuild() const
{
	return canRun();
}

rbool RDOStudioModel::canRun() const
{
	return hasModel() && m_GUI_CAN_RUN;
}

rbool RDOStudioModel::isRunning() const
{
	return m_GUI_IS_RUNNING;
}

void RDOStudioModel::setIsRunning(rbool value)
{
	m_GUI_IS_RUNNING = value;
	setUpActions();
}

rbool RDOStudioModel::isFrmDescribed() const
{
	return m_frmDescribed;
}

double RDOStudioModel::getTimeNow() const
{
	return m_timeNow;
}

rdo::simulation::report::RDOExitCode RDOStudioModel::getExitCode() const
{
	return m_exitCode;
}

rdo::runtime::RunTimeMode RDOStudioModel::getRuntimeMode() const
{
	return m_runtimeMode;
}

REF(RDOStudioFrameManager) RDOStudioModel::getFrameManager()
{
	return m_frameManager;
}

void RDOStudioModel::onChangeFrame(ruint)
{
	setUpActions();
}

PTR(RPViewQt) RDOStudioModel::getProcView()
{
	return m_pModelProcView;
}

PTR(rdoEditor::RDOEditorTabCtrl) RDOStudioModel::getTab()
{
	if (!m_pModelView)
		return NULL;

	return &m_pModelView->getTab();
}

CPTR(rdoEditor::RDOEditorTabCtrl) RDOStudioModel::getTab() const
{
	if (!m_pModelView)
		return NULL;

	return &m_pModelView->getTab();
}

void RDOStudioModel::onFileNew()
{
	NewModelDialog dlg(studioApp.getMainWndUI());
	if (dlg.exec() == QDialog::Accepted)
	{
		newModel(dlg.getModelName(), dlg.getModelPath() + dlg.getModelName(), dlg.getTemplateIndex());
	}
}

void RDOStudioModel::onFileOpen()
{
	openModel();
}

void RDOStudioModel::onFileClose()
{
	closeModel();
}

void RDOStudioModel::onFileSaveAll()
{
	saveModel();
}

void RDOStudioModel::onModelBuild()
{
	buildModel();
}

void RDOStudioModel::onModelRun()
{
	runModel();
}

void RDOStudioModel::onModelStop()
{
	stopModel();
}

void RDOStudioModel::onModelRuntimeMaxSpeed()
{
	setRuntimeMode(rdo::runtime::RTM_MaxSpeed);
}

void RDOStudioModel::onModelRuntimeJump()
{
	setRuntimeMode(rdo::runtime::RTM_Jump);
}

void RDOStudioModel::onModelRuntimeSync()
{
	setRuntimeMode(rdo::runtime::RTM_Sync);
}

void RDOStudioModel::onModelRuntimePause()
{
	setRuntimeMode(rdo::runtime::RTM_Pause);
}

void RDOStudioModel::onModelShowRateInc()
{
	setShowRate(getShowRate() * 1.5);
}

void RDOStudioModel::onModelShowRateIncFour()
{
	setShowRate(getShowRate() * 4);
}

void RDOStudioModel::onModelShowRateDecFour()
{
	setShowRate(getShowRate() / 4);
}

void RDOStudioModel::onModelShowRateDec()
{
	setShowRate(getShowRate() / 1.5);
}

void RDOStudioModel::onModelSpeedValueChanged(int value)
{
	setSpeed(log( double(value + 1) ) / log(101.0));
}
