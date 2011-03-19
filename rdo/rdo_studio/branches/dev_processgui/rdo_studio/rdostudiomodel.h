/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdostudiomodel.h
 * author   : ������ ������
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOSTUDIOMODEL_H_
#define _RDOSTUDIOMODEL_H_

#if _MSC_VER > 1000
#pragma once
#endif

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_studio/rdostudioframemanager.h"
#include "rdo_studio/rdostudiomodeldoc.h"
#include "rdo_studio/rdostudiomodelview.h"
#include "rdo_studio/rdo_process/rdoprocess_childfrm.h"
#include "rdo_studio/rdo_process/rdoprocess_docview.h"
#include "rdo_studio/rdostudioplugins.h"
#include "rdo_lib/rdo_simulator/rdosimwin.h"
#include "rdo_kernel/rdothread.h"
// ===============================================================================

// ----------------------------------------------------------------------------
// ---------- RDOStudioModel
// ----------------------------------------------------------------------------
namespace rdoEditor {
	class RDOEditorTabCtrl;
}

class RDOStudioModel: public RDOThreadGUI
{
friend class RDOStudioModelDoc;
friend class RDOStudioFrameView;
friend class RDOStudioFrameTreeCtrl;
friend class RDOStudioApp;
friend class RDOThreadStudioGUI;
friend class RDOStudioPlugins;

private:
	PTR(CMultiDocTemplate)    m_pModelDocTemplate;
	RDOStudioFrameManager     m_frameManager;

	int                       m_useTemplate;
	rbool                     m_autoDeleteDoc;
	rbool                     m_showCanNotCloseModelMessage;

	rbool                     m_GUI_HAS_MODEL;
	rbool                     m_GUI_CAN_RUN;
	rbool                     m_GUI_IS_RUNING;
	rbool                     m_GUI_ACTION_NEW;
	rbool                     m_GUI_ACTION_OPEN;
	rbool                     m_GUI_ACTION_SAVE;
	rbool                     m_GUI_ACTION_CLOSE;
	rbool                     m_GUI_ACTION_BUILD;
	rbool                     m_GUI_ACTION_RUN;

	SYSTEMTIME                m_timeStart;

	mutable rbool             m_openError;
	mutable rbool             m_smrEmptyError;
	mutable rbool             m_modelClosed;

	rbool                     m_frmDescribed;
	double                    m_timeNow;
	double                    m_speed;
	double                    m_showRate;
	rbool                     m_tempPause;
	rdoRuntime::RunTimeMode   m_runtimeMode;
	rdoRuntime::RunTimeMode   m_runtimeModePrev;
	rdoSimulator::RDOExitCode m_exitCode;
	mutable rbool             m_prevModify;

	void  updateFrmDescribed      ();
	void  newModelFromRepository  ();
	void  openModelFromRepository ();
	void  saveModelToRepository   ();
	void  closeModelFromRepository();
	rbool canCloseModel           ();
	void  afterModelStart         ();

	PTR(RDOStudioModelDoc) getModelDoc() const
	{
		POSITION pos = m_pModelDocTemplate->GetFirstDocPosition();
		return pos ? static_cast<PTR(RDOStudioModelDoc)>(m_pModelDocTemplate->GetNextDoc(pos)) : NULL;
	}

	struct ModelTemplateItem
	{
		ruint m_resID;
		int   m_position;

		ModelTemplateItem()
			: m_resID   (~0)
			, m_position(~0)
		{}
		ModelTemplateItem(CREF(ModelTemplateItem) copy)
			: m_resID   (copy.m_resID   )
			, m_position(copy.m_position)
		{}
		ModelTemplateItem(ruint resID, int position)
			: m_resID   (resID   )
			, m_position(position)
		{}
	};
	typedef  std::map<rdoModelObjects::RDOFileType, ModelTemplateItem>  ModelTemplate;
	typedef  std::map<int, ModelTemplate>                               ModelTemplateList;
	ModelTemplateList m_modelTemplates;

	void show_result();

protected:
	virtual void proc(REF(RDOThread::RDOMessageInfo) msg);

public:
	CMultiDocTemplate* flowchartDocTemplate;
	RDOStudioModel();
	virtual ~RDOStudioModel();

	rbool newModel      (tstring _model_name = _T(""), tstring _model_path = _T(""), const int _useTemplate = -1);
	rbool openModel     (CREF(tstring) modelName = _T("")) const;
	rbool saveModel     () const;
	void  saveAsModel   () const;
	rbool closeModel    () const;
	rbool buildModel    () const;
	rbool runModel      ();
	rbool stopModel     () const;
	void  update        ();
	void  setGUIPause   ();
	void  setGUIContinue();

	tstring getName() const
	{
		PTR(RDOStudioModelDoc) pDoc = getModelDoc();
		return pDoc ? pDoc->getName() : _T("");
	}
	void    setName    (CREF(tstring) str);
	tstring getFullName() const;

	rbool   isModify   () const
	{
		PTR(RDOStudioModelDoc) pDoc = getModelDoc();
		rbool result = pDoc ? pDoc->isModify() : false;
		if (m_prevModify != result)
		{
			m_prevModify = result;
			if (plugins)
			{
				plugins->pluginProc(rdoPlugin::PM_MODEL_MODIFY);
			}
		}
		return result;
	}
	rbool  canNew        () const { return ((hasModel() && m_GUI_CAN_RUN) || !hasModel()) && m_GUI_ACTION_NEW;   }
	rbool  canOpen       () const { return ((hasModel() && m_GUI_CAN_RUN) || !hasModel()) && m_GUI_ACTION_OPEN;  }
	rbool  canSave       () const { return isModify()                                     && m_GUI_ACTION_SAVE;  }
	rbool  canClose      () const { return hasModel() && !isRunning()                     && m_GUI_ACTION_CLOSE; }
	rbool  canBuild      () const { return hasModel() && m_GUI_CAN_RUN                    && m_GUI_ACTION_BUILD; }
	rbool  canRun        () const { return hasModel() && m_GUI_CAN_RUN                    && m_GUI_ACTION_RUN;   }
	rbool  isRunning     () const { return m_GUI_IS_RUNING;                                                      }
	rbool  isFrmDescribed() const { return m_frmDescribed;                                                       }
	double getTimeNow    () const { return m_timeNow;                                                            }

	rdoSimulator::RDOExitCode getExitCode   () const { return m_exitCode;    }
	rdoRuntime::RunTimeMode   getRuntimeMode() const { return m_runtimeMode; }
	void    setRuntimeMode       (const rdoRuntime::RunTimeMode value);
	tstring getLastBreakPointName();
	double  getSpeed             () const            { return m_speed;       }
	void    setSpeed             (double persent);
	double  getShowRate          ()                  { return m_showRate;    }
	void    setShowRate          (double value);

	void       showNextFrame   ()                { m_frameManager.showNextFrame();                    }
	void       showPrevFrame   ()                { m_frameManager.showPrevFrame();                    }
	rbool      canShowNextFrame() const          { return m_frameManager.canShowNextFrame();          }
	rbool      canShowPrevFrame() const          { return m_frameManager.canShowPrevFrame();          }
	int        getFrameCount   () const          { return m_frameManager.count();                     }
	CPTR(char) getFrameName    (int index) const { return m_frameManager.getFrameName(index).c_str(); }
	void       showFrame       (int index)       { m_frameManager.showFrame(index);                   }
	void       closeAllFrame   ()                { m_frameManager.closeAll();                         }
	rbool      hasModel        () const          { return m_GUI_HAS_MODEL;                            }

	PTR(rdoEditor::RDOEditorTabCtrl) getTab() const
	{
		PTR(RDOStudioModelDoc) pDoc = getModelDoc();
		if (pDoc)
		{
			PTR(RDOStudioModelView) pView = pDoc->getView();
			if (pView)
			{
				return pView->tab;
			}
		}
		return NULL;
	}

	void  updateStyleOfAllModel() const;
	rbool isPrevModelClosed    () const { return m_modelClosed; }
};

// ----------------------------------------------------------------------------
extern PTR(RDOStudioModel) model;

#endif //! _RDOSTUDIOMODEL_H_
