#if !defined(AFX_RDOPROCESS_SHAPE_CREATE_DLG2_MJ_H__0B67F21A_9090_4CE0_B47B_AD518FC2589D__INCLUDED_)
#define AFX_RDOPROCESS_SHAPE_CREATE_DLG2_MJ_H__0B67F21A_9090_4CE0_B47B_AD518FC2589D__INCLUDED_
#include "app/rdo_studio_mfc/resource.h"
// rdoprocess_shape_create_dlg2_MJ.h : header file
//
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create.h"
/////////////////////////////////////////////////////////////////////////////
// RPShapeCreateDlg2_MJ dialog

class RPShapeCreateDlg2_MJ : public CDialog
{
// Construction
public:
	RPShapeCreateDlg2_MJ(CWnd* pParent = NULL,RPShapeCreateMJ* ppParent=NULL);    // standard constructor
	
	RPShapeCreateMJ* pParentMJ; // ������ �� ���������� ������

virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_CREATE_2_MJ };
	int		m_dlginf;
	int		m_dlgid;
	double	m_dlgpar1;
	double	m_dlgpar2;
	double	m_dlgpar3;

private:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RDOPROCESS_SHAPE_CREATE_DLG2_MJ_H__0B67F21A_9090_4CE0_B47B_AD518FC2589D__INCLUDED_)
