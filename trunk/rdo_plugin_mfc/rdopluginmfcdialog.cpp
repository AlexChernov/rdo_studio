#include "stdafx.h"
#include "rdopluginmfcdialog.h"
#include "rdopluginmfc.h"

#define RDOPLUGIN_EXPORTS
#include <rdoplugin.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPluginMFCDialog
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOPluginMFCDialog, CDialog)
	//{{AFX_MSG_MAP(RDOPluginMFCDialog)
	ON_COMMAND(ID_MODEL_RUN, OnModelRun)
	ON_UPDATE_COMMAND_UI(ID_MODEL_RUN, OnUpdateModelRun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOPluginMFCDialog::RDOPluginMFCDialog( CWnd* pParent )
	: CDialog( RDOPluginMFCDialog::IDD, pParent )
{
	//{{AFX_DATA_INIT(RDOPluginMFCDialog)
	//}}AFX_DATA_INIT
}

RDOPluginMFCDialog::~RDOPluginMFCDialog()
{
}

void RDOPluginMFCDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RDOPluginMFCDialog)
	//}}AFX_DATA_MAP
}

void RDOPluginMFCDialog::OnCancel() 
{
	CDialog::OnCancel();
	pluginMFCApp.PostThreadMessage( rdoPlugin::PLUGIN_MUSTEXIT_MESSAGE, reinterpret_cast<WPARAM>(AfxGetInstanceHandle()), 0 );
}

void RDOPluginMFCDialog::OnModelRun() 
{
	pluginMFCApp.studio.runModel();
}

void RDOPluginMFCDialog::OnUpdateModelRun(CCmdUI* pCmdUI) 
{
}
