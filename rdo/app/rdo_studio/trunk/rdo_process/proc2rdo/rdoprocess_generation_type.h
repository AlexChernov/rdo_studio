#if !defined(AFX_RDOPROCESS_GENERATION_TYPE_MJ_H__6E103D6A_16C3_4ADD_869A_70505FA8090B__INCLUDED_)
#define AFX_RDOPROCESS_GENERATION_TYPE_MJ_H__6E103D6A_16C3_4ADD_869A_70505FA8090B__INCLUDED_
#include "app/rdo_studio_mfc/resource.h"
// rdoprocess_generation_type_MJ.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RP_GENERATION_TYPE_MJ dialog

class RP_GENERATION_TYPE_MJ : public CDialog
{
// Construction
public:
	RP_GENERATION_TYPE_MJ(CWnd* pParent = NULL);   // standard constructor
	virtual BOOL OnInitDialog();
// Dialog Data

	enum { IDD = IDD_DIALOG_GEN_TYPE };
	CComboBox	m_combo;
	CEdit	m_time;
	double	m_time_value;

private:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnCloseupCombo1();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RDOPROCESS_GENERATION_TYPE_MJ_H__6E103D6A_16C3_4ADD_869A_70505FA8090B__INCLUDED_)
