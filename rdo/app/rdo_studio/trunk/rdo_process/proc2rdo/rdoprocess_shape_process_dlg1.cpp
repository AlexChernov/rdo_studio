#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process_dlg1.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_process_dlg2.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include<list>

BEGIN_MESSAGE_MAP(RPListBox, CListBox)
END_MESSAGE_MAP()

RPProcessShapeProcessDlg1::RPProcessShapeProcessDlg1(CWnd* pParent /*=NULL*/,RPProcessShapeProcess* ppParent)
	: CDialog(RPProcessShapeProcessDlg1::IDD, pParent),
	brush1( RGB(0xFF, 0x00, 0x00) ),
	brush2( RGB(0x00, 0xFF, 0x00) )
{
	m_name = _T("");
	m_list_text = _T("");
	m_gprior = 0;
	m_gexp = 0.0;
	m_gdisp = 0.0;
	m_gmin = 0.0;
	m_gmax = 0.0;
	m_gbase_gen = 0;

    pParentMJ = ppParent;
}


void RPProcessShapeProcessDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_gtype);
	DDX_Control(pDX, IDC_COMBO3, m_gqueque);
	DDX_Control(pDX, IDC_LIST1, m_ResList);
	DDX_Control(pDX, IDC_BUTTON2, m_DelRes);
	DDX_Control(pDX, IDC_BUTTON1, m_AddRes);
	DDX_Control(pDX, IDC_EDIT4, m_expCtrl);
	DDX_Control(pDX, IDC_EDIT5, m_dispCtrl);
	DDX_Control(pDX, IDC_EDIT7, m_maxCtrl);
	DDX_Control(pDX, IDC_STATIC4, m_expText);
	DDX_Control(pDX, IDC_STATIC5, m_dispText);
	DDX_Control(pDX, IDC_STATIC7, m_maxText);
	DDX_Control(pDX, IDC_COMBO4, m_parameter);
	DDX_Control(pDX, IDC_COMBO2, m_action);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_LBString(pDX, IDC_LIST1, m_list_text);
	DDX_Text(pDX, IDC_EDIT2, m_gprior);
	DDX_Text(pDX, IDC_EDIT4, m_gexp);
	DDX_Text(pDX, IDC_EDIT5, m_gdisp);
	DDX_Text(pDX, IDC_EDIT7, m_gmax);
	DDX_Text(pDX, IDC_EDIT8, m_gbase_gen);
	DDV_MinMaxInt(pDX, m_gbase_gen, -2147483647, 2147483647);
}

BOOL RPProcessShapeProcessDlg1::OnInitDialog()
{
	CDialog::OnInitDialog();
		 //����������� ����� ����� � ����
	CString str( pParentMJ->getName().c_str() );
    m_name = str;
	UpdateData(FALSE);
	
// ������������� �� ���������� �������
	m_gtype.SetCurSel(pParentMJ->gtype); // ����� ��������
	m_gbase_gen=pParentMJ->base_gen;
	//�������� �������
	m_gexp=pParentMJ->gexp;
	m_gdisp=pParentMJ->gdisp;
	m_gmax=pParentMJ->gmax;
	m_gmin=pParentMJ->gmin;


m_action.SetCurSel(pParentMJ->action);
m_gprior = pParentMJ->prior;
m_gqueque.SetCurSel(pParentMJ->queue);
//m_parameter.SetCurSel(pParentMJ->parameter);


//������ �� ������ ��������m_resourceList
std::list<CString>::iterator it = pParentMJ->m_resourceList.begin();
	while( it != pParentMJ->m_resourceList.end() ) 
	{
	m_ResList.AddString(*it);
	it++;
	}	




UpdateData(FALSE);
OnCbnSelchange2(); 
OnCbnSelchange1();
	return TRUE;
}



BEGIN_MESSAGE_MAP(RPProcessShapeProcessDlg1, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchange1)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnCbnSelchange3)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchange2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
END_MESSAGE_MAP()

void RPProcessShapeProcessDlg1::OnCbnSelchange1() 
{
int cur = m_gtype.GetCurSel();

switch(cur) // ���������� �������� ���� ������ �� ������
{
case 0: // ���������
	m_expCtrl.EnableWindow(TRUE);
	m_expText.SetWindowText(_T("���������"));
	m_dispText.ShowWindow(SW_HIDE);
	m_dispCtrl.ShowWindow(SW_HIDE);
	m_maxText.ShowWindow(SW_HIDE);
	m_maxCtrl.ShowWindow(SW_HIDE);
					break;	
case 1: // ����������
	m_expText.SetWindowText(_T("���. ��������"));
	m_dispText.ShowWindow(SW_SHOW);
	m_dispText.SetWindowText(_T("���������"));
	m_dispCtrl.ShowWindow(SW_SHOW);
	m_maxText.ShowWindow(SW_HIDE);
	m_maxCtrl.ShowWindow(SW_HIDE);
					break;
case 2: // ����������� �����
	m_expText.SetWindowText(_T("����� �������"));
	m_dispText.ShowWindow(SW_SHOW);
	m_dispText.SetWindowText(_T("������ �������"));
	m_dispCtrl.ShowWindow(SW_SHOW);
	m_maxText.ShowWindow(SW_HIDE);
	m_maxCtrl.ShowWindow(SW_HIDE);
					break;
case 3: // �����������
	m_expText.SetWindowText(_T("����� �������"));
	m_dispText.ShowWindow(SW_SHOW);
	m_dispText.SetWindowText(_T("������"));
	m_dispCtrl.ShowWindow(SW_SHOW);
	m_maxText.ShowWindow(SW_SHOW);
	m_maxText.SetWindowText(_T("������ �������"));
	m_maxCtrl.ShowWindow(SW_SHOW);
					break;
case 4: //����������������
	m_expText.SetWindowText(_T("���. ��������"));
	m_dispText.ShowWindow(SW_HIDE);
	m_dispCtrl.ShowWindow(SW_HIDE);
	m_maxText.ShowWindow(SW_HIDE);
	m_maxCtrl.ShowWindow(SW_HIDE);
					break;
}	
}

void RPProcessShapeProcessDlg1::OnCbnSelchange3() 
{
int cur = m_gqueque.GetCurSel();

switch(cur) // ���������� �������� ���� ������ �� ������
{
case 0: // fifo
	m_parameter.EnableWindow(FALSE);
					break;	
case 1: // lifo
	m_parameter.EnableWindow(FALSE);
				break;
case 2: // �����������
	m_parameter.EnableWindow(TRUE);
					break;
case 3: // ������������
	m_parameter.EnableWindow(TRUE);
					break;
}	
}

void RPProcessShapeProcessDlg1::OnButton1() 
{
	RPProcessShapeProcessDlg2 dlg( this, pParentMJ, this );
	dlg.DoModal();
}

void RPProcessShapeProcessDlg1::OnOK() 
{
	// ����� � ���� ����� �����
	UpdateData(TRUE);
	pParentMJ->setName(tstring(m_name));

	
// ������������� �� ���������� �������
	pParentMJ->gtype= m_gtype.GetCurSel(); // ����� ��������
	pParentMJ->base_gen=m_gbase_gen;
	//�������� �������
	pParentMJ->gexp=m_gexp;
	pParentMJ->gdisp=m_gdisp;
	pParentMJ->gmax=m_gmax;
	pParentMJ->gmin=m_gmin;
	pParentMJ->gname=m_name;


pParentMJ->action=m_action.GetCurSel();
pParentMJ->prior = m_gprior ;
pParentMJ->queue=m_gqueque.GetCurSel();




	CDialog::OnOK();	
}

void RPProcessShapeProcessDlg1::OnCbnSelchange2() 
{
	int cur = m_action.GetCurSel();

	switch(cur) // ���������� �������� ���� ������ �� ������
	{
	case 0: // ���������
			m_gqueque.EnableWindow(FALSE); // ��������� �������
			m_parameter.EnableWindow(FALSE);// ��������� �������
			m_AddRes.EnableWindow(FALSE); 
			m_DelRes.EnableWindow(FALSE); 
			m_ResList.EnableWindow(FALSE); 
			
						break;	
	case 1: //������ ��������� ����������
			m_gqueque.EnableWindow(TRUE);// ��������� �������
				 if(m_gqueque.GetCurSel() == 0 || m_gqueque.GetCurSel() == 1)
					   m_parameter.EnableWindow(FALSE);
				 else
					   m_parameter.EnableWindow(TRUE);

			m_AddRes.EnableWindow(TRUE); 
			m_DelRes.EnableWindow(TRUE); 
			m_ResList.EnableWindow(TRUE); 
		
					break;

	case 2: // ������ ���������
		m_gqueque.EnableWindow(TRUE);// ��������� �������
				if(m_gqueque.GetCurSel() == 0 || m_gqueque.GetCurSel() == 1)
					m_parameter.EnableWindow(FALSE);
				else
					m_parameter.EnableWindow(TRUE);

			m_AddRes.EnableWindow(TRUE); 
			m_DelRes.EnableWindow(TRUE); 
			m_ResList.EnableWindow(TRUE); 
						break;



	case 3: // ��������� ����������
		m_gqueque.EnableWindow(TRUE);// ��������� �������
				 if(m_gqueque.GetCurSel() == 0 || m_gqueque.GetCurSel() == 1)
					   m_parameter.EnableWindow(FALSE);
				 else
					   m_parameter.EnableWindow(TRUE);

			m_AddRes.EnableWindow(TRUE); 
			m_DelRes.EnableWindow(TRUE); 
			m_ResList.EnableWindow(TRUE); 
						break;
	}	
}

HBRUSH RPProcessShapeProcessDlg1::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	return CDialog::OnCtlColor( pDC, pWnd, nCtlColor );
	//if ( nCtlColor == CTLCOLOR_LISTBOX && pWnd == &m_ResList ) {
	//	return m_ResList.IsWindowEnabled() ? brush1 : brush2;
	//} else {
	//	return CDialog::OnCtlColor( pDC, pWnd, nCtlColor );
	//}
}

void RPProcessShapeProcessDlg1::OnButton2() 
{


}
