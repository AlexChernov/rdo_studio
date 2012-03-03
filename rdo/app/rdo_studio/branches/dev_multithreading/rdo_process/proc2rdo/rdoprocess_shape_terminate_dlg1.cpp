// rdoprocess_shape_terminate_dlg1_MJ.cpp : implementation file
//

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "rdoprocess_shape_terminate_dlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(RPShapeTerminateDlg1_MJ, CDialog)
	//{{AFX_MSG_MAP(RPShapeTerminateDlg1_MJ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RPShapeTerminateDlg1_MJ::RPShapeTerminateDlg1_MJ(PTR(CWnd) pWndParent, PTR(RPShapeTerminateMJ) pShape)
	: CDialog(RPShapeTerminateDlg1_MJ::IDD, pWndParent)
{
	//{{AFX_DATA_INIT(RPShapeTerminateDlg1_MJ)
	m_name = _T("");
	//}}AFX_DATA_INIT
	m_pShape = pShape;
	ASSERT(m_pShape);
}

void RPShapeTerminateDlg1_MJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RPShapeTerminateDlg1_MJ)
	DDX_Text(pDX, IDC_EDIT1, m_name            );
	DDX_Text(pDX, IDC_EDIT3, m_terminateCounter);
	//}}AFX_DATA_MAP
}

BOOL RPShapeTerminateDlg1_MJ::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_name             = CString(m_pShape->getName().c_str());
	m_terminateCounter = m_pShape->m_term_inc;
	UpdateData(FALSE);

	return TRUE;
}

void RPShapeTerminateDlg1_MJ::OnOK()
{
	UpdateData(true);

	if (m_pShape->setName(rp::string(m_name)))
	{
		m_pShape->m_term_inc = m_terminateCounter;
		m_pShape->m_name     = m_name;
		CDialog::OnOK();
	}
}
