#include "stdafx.h"
#include "rdostudiomodelnew.h"
#include <shlobj.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioModelNew
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioModelNew, CDialog)
	//{{AFX_MSG_MAP(RDOStudioModelNew)
	ON_BN_CLICKED(IDC_MODEL_PATH_BUTTON, OnModelPathButton)
	ON_EN_CHANGE(IDC_MODEL_NAME, OnChangeModelName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioModelNew::RDOStudioModelNew():
	CDialog( IDD )
{
	//{{AFX_DATA_INIT(RDOStudioModelNew)
	m_modelName = _T("mymodel");
	m_modelPath = getMyDocFolder();
	m_info = _T("");
	m_comment = TRUE;
	m_model_template = 0;
	//}}AFX_DATA_INIT
}

RDOStudioModelNew::~RDOStudioModelNew()
{
}

void RDOStudioModelNew::DoDataExchange(CDataExchange* pDX) 
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RDOStudioModelNew)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Text(pDX, IDC_MODEL_NAME, m_modelName);
	DDX_Text(pDX, IDC_MODEL_PATH, m_modelPath);
	DDX_Text(pDX, IDC_INFO, m_info);
	DDX_Check(pDX, IDC_COMMENT, m_comment);
	DDX_Radio(pDX, IDC_MODEL_EMPTY, m_model_template);
	//}}AFX_DATA_MAP
}

void RDOStudioModelNew::OnOK() 
{
	CDialog::OnOK();
}

BOOL RDOStudioModelNew::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	return CDialog::OnNotify(wParam, lParam, pResult);
}

CString RDOStudioModelNew::getMyDocFolder() const
{
	LPITEMIDLIST pidl = NULL;
	if ( ::SHGetSpecialFolderLocation( m_hWnd, CSIDL_PERSONAL, &pidl ) != NOERROR ) return "";
	TCHAR szFolderDroppedIn[MAX_PATH];
	return ::SHGetPathFromIDList( pidl, szFolderDroppedIn ) ? szFolderDroppedIn : "";
}

void RDOStudioModelNew::updateInfo()
{
	if ( !m_modelPath.IsEmpty() ) {
		if ( m_modelPath[m_modelPath.GetLength()-1] != '\\' && m_modelPath[m_modelPath.GetLength()-1] != '/' ) {
			m_modelPath += '\\';
		}
	}
	if ( m_modelName.IsEmpty() ) {
		m_info = "���������� ������� ��� ������";
		m_ok.EnableWindow( false );
	} else {
		CString fullpath = m_modelPath + m_modelName;
		CFileFind finder;
		if ( finder.FindFile( fullpath ) ) {
			finder.FindNextFile();
			if ( finder.IsDirectory() ) {
				m_info = rdo::format("����� ���������� ��� ����������: '%s\\'", (LPCTSTR)fullpath).c_str();

			} else if ( finder.IsReadOnly() || finder.IsCompressed() || finder.IsSystem() || finder.IsHidden() || finder.IsTemporary() || finder.IsNormal() || finder.IsArchived() ) {
				m_info = rdo::format("����� ���� ��� ����������: '%s'", (LPCTSTR)fullpath).c_str();
			}
			m_ok.EnableWindow( false );
		} else {
			m_info = rdo::format("����� ������� ����������: '%s\\'", (LPCTSTR)fullpath).c_str();
			m_ok.EnableWindow( true );
		}
		finder.Close();
	}
	UpdateData( false );
}

static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    TCHAR szPath[_MAX_PATH];
    switch (uMsg) {
    case BFFM_INITIALIZED:
        if (lpData)
            SendMessage(hWnd,BFFM_SETSELECTION,TRUE,lpData);
        break;
    case BFFM_SELCHANGED:
        SHGetPathFromIDList(LPITEMIDLIST(lParam),szPath);
        SendMessage(hWnd, BFFM_SETSTATUSTEXT, NULL, LPARAM(szPath));
        break;
    }
    return 0;
}

BOOL GetFolder(LPCTSTR szTitle, LPTSTR szPath, LPCTSTR szRoot, HWND hWndOwner)
{
    if (szPath == NULL)
        return false;

    bool result = false;

    LPMALLOC pMalloc;
    if (::SHGetMalloc(&pMalloc) == NOERROR) {
        BROWSEINFO bi;
        ::ZeroMemory(&bi,sizeof bi);
        bi.ulFlags   = BIF_RETURNONLYFSDIRS;

        // ���������� ����-��������� �������
        bi.hwndOwner = hWndOwner;

        // ���������� ��������� � �������
        bi.lpszTitle = szTitle;

        // ����������� �������� ��������
        bi.lpfn      = BrowseCallbackProc;
        bi.ulFlags  |= BIF_STATUSTEXT;

        // ��������� �������� �� ���������
        bi.lParam    = LPARAM(szPath);

        // ��������� ��������� ��������
        if (szRoot != NULL) {
            IShellFolder *pDF;
            if (SHGetDesktopFolder(&pDF) == NOERROR) {
                LPITEMIDLIST pIdl = NULL;
                ULONG        chEaten;
                ULONG        dwAttributes;

                USES_CONVERSION;
                LPOLESTR oleStr = T2OLE(szRoot);

                pDF->ParseDisplayName(NULL,NULL,oleStr,&chEaten,&pIdl,&dwAttributes);
                pDF->Release();

                bi.pidlRoot = pIdl;
            }
        }

        LPITEMIDLIST pidl = ::SHBrowseForFolder(&bi);
        if (pidl != NULL) {
            if (::SHGetPathFromIDList(pidl,szPath))
                result = true;
            pMalloc->Free(pidl);
        }
        if (bi.pidlRoot != NULL)
            pMalloc->Free((void*)bi.pidlRoot);
        pMalloc->Release();
    }
    return result;
}

void RDOStudioModelNew::OnModelPathButton()
{
	TCHAR szTitle[] = "����� ���������� ��� ������";
	TCHAR szPath[MAX_PATH];
	if ( GetFolder( szTitle, szPath, NULL, m_hWnd ) ) {
		m_modelPath = szPath;
		updateInfo();
	}
}

void RDOStudioModelNew::OnChangeModelName()
{
	static_cast<CEdit*>(GetDlgItem(IDC_MODEL_NAME))->GetWindowText( m_modelName );
	updateInfo();
}

BOOL RDOStudioModelNew::OnInitDialog()
{
	BOOL res = CDialog::OnInitDialog();
	updateInfo();
	return res;
}
