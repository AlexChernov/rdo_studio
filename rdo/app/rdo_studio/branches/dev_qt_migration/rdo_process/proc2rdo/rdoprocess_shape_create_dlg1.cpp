#include "app/rdo_studio_mfc/rdo_process/proc2rdo/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create_dlg1.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape_create_dlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RPProcessShapeCreateDlg1::RPProcessShapeCreateDlg1(CWnd* pParent /*=NULL*/,RPProcessShapeCreate* ppParent)
	: CDialog(RPProcessShapeCreateDlg1::IDD, pParent)
{
	m_name = _T("");
	m_dlgfirst = 0.0;
	m_dlgamount = 0;
	m_dlgexp = 0.0;
	m_dlgdisp = 0.0;
	m_dlgmax = 0.0;
	m_dlgbase_gen = 0;
	m_pParent = ppParent;
}

void RPProcessShapeCreateDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT4, m_expCtrl);
	DDX_Control(pDX, IDC_EDIT5, m_dispCtrl);
	DDX_Control(pDX, IDC_EDIT6, m_maxCtrl);
	DDX_Control(pDX, IDC_STATIC7, m_expText);
	DDX_Control(pDX, IDC_STATIC8, m_dispText);
	DDX_Control(pDX, IDC_STATIC9, m_maxText);
	DDX_Control(pDX, IDC_COMBO1, m_comboCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, m_dlgfirst);
	DDX_Text(pDX, IDC_EDIT3, m_dlgamount);
	DDX_Text(pDX, IDC_EDIT4, m_dlgexp);
	DDX_Text(pDX, IDC_EDIT5, m_dlgdisp);
	DDX_Text(pDX, IDC_EDIT6, m_dlgmax);
	DDX_Text(pDX, IDC_EDIT8, m_dlgbase_gen);
	DDV_MinMaxInt(pDX, m_dlgbase_gen, -2147483647, 2147483647);
}

BOOL RPProcessShapeCreateDlg1::OnInitDialog()
{
	CDialog::OnInitDialog();

	//����������� ����� ����� � ����
	CString str( m_pParent->getName().c_str() );
	m_name = str;
	UpdateData(FALSE);

	// ������������� �� ���������� �������
	m_comboCtrl.SetCurSel(m_pParent->gtype); // ����� ��������

	m_dlgfirst=m_pParent->gfirst; // ����� �������
	if (m_pParent->gamount)
	{
		m_dlgamount = m_pParent->gamount.get(); // ���-�� �����������
	}
	m_dlgbase_gen=m_pParent->base_gen;
	//�������� �������
	m_dlgexp=m_pParent->gexp;
	m_dlgdisp=m_pParent->gdisp;
	m_dlgmax=m_pParent->gmax;
	//������ ����
	m_pParent->inf; // ������������
	m_pParent->gID; // ID ������
	m_pParent->gpar1;
	m_pParent->gpar2;
	m_pParent->gpar3;

	UpdateData(FALSE);

	RPProcessShapeCreateDlg1::OnCbnSelchange1();
	return TRUE;
}

BEGIN_MESSAGE_MAP(RPProcessShapeCreateDlg1, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchange1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
END_MESSAGE_MAP()

void RPProcessShapeCreateDlg1::OnCbnSelchange1() 
{
	int cur = m_comboCtrl.GetCurSel();
	UpdateData(TRUE);
	switch(cur) // ���������� �������� ���� ������ �� ������
	{
	case 0: // ���������
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
	UpdateData(FALSE);
}

void RPProcessShapeCreateDlg1::OnButton1() 
{
	RPProcessShapeCreateDlg2 dlg2( this, m_pParent );
	dlg2.DoModal();
}

void RPProcessShapeCreateDlg1::OnOK() 
{
	UpdateData(TRUE);
	m_pParent->setName( tstring(m_name) );
//	m_pParent->update_modify();	
	CDialog::OnOK();

	// ������ ���������� � ������
	// ������������� �� ���������� �������
	UpdateData(TRUE);
	m_pParent->gname    = m_name;
	m_pParent->gfirst   = m_dlgfirst; // ����� �������
	m_pParent->gamount  = m_dlgamount > 0
		? m_dlgamount
		: boost::optional<ruint>(); // ���-�� �����������
	m_pParent->gtype    = m_comboCtrl.GetCurSel(); // ����� ��������
	m_pParent->base_gen = m_dlgbase_gen;
	//�������� �������
	m_pParent->gexp  = m_dlgexp;
	m_pParent->gdisp = m_dlgdisp;
	m_pParent->gmax  = m_dlgmax;
}
