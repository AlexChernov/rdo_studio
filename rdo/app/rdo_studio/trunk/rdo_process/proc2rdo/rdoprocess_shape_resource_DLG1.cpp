// rdoprocess_shape_resource_DLG1_MJ.cpp : implementation file
//

#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_resource_DLG1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RPShapeResourceDLG1_MJ dialog


RPShapeResourceDLG1_MJ::RPShapeResourceDLG1_MJ (CWnd* pParent, RPShapeResource_MJ* ppParent)
	: CDialog(RPShapeResourceDLG1_MJ::IDD, pParent)
{
	m_name = _T("");
	m_dlgamount = 0;
	pParentMJ = ppParent;
}


void RPShapeResourceDLG1_MJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, m_dlgamount);
}

BOOL RPShapeResourceDLG1_MJ::OnInitDialog()
{
// ����������� ����� �����
	CString str( pParentMJ->getName().c_str() );
    m_name = str;

	m_dlgamount = pParentMJ->gamount;
	UpdateData(FALSE);

return TRUE;
}




BEGIN_MESSAGE_MAP(RPShapeResourceDLG1_MJ, CDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RPShapeResourceDLG1_MJ message handlers

void RPShapeResourceDLG1_MJ::OnOK() 
{

	UpdateData(TRUE);
	pParentMJ->setName(std::string(m_name));

	pParentMJ->gamount = m_dlgamount;	
	CDialog::OnOK();
}
