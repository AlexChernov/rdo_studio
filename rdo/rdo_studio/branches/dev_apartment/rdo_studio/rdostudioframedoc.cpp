#include "rdo_studio/stdafx.h"
#include "rdo_studio/rdostudioframedoc.h"
#include "rdo_studio/rdostudiomodel.h"
#include "rdo_studio/rdostudioframemanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameDoc
// --------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioFrameDoc, CDocument)

BEGIN_MESSAGE_MAP(RDOStudioFrameDoc, CDocument)
	//{{AFX_MSG_MAP(RDOStudioFrameDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioFrameDoc::RDOStudioFrameDoc():
	CDocument(),
	frame( NULL )
{
}

RDOStudioFrameDoc::~RDOStudioFrameDoc()
{
}

BOOL RDOStudioFrameDoc::OnNewDocument()
{
	if ( !CDocument::OnNewDocument() ) return FALSE;
	return TRUE;
}

void RDOStudioFrameDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
	} else {
	}
}

#ifdef _DEBUG
void RDOStudioFrameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void RDOStudioFrameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
