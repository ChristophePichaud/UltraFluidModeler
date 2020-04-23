// Modeler1SourceView.cpp : implementation file
//

#include "stdafx.h"
#include "Modeler1.h"
#include "Modeler1SourceView.h"
#include "MainFrm.h"


// CModeler1SourceView

IMPLEMENT_DYNCREATE(CModeler1SourceView, CEditView)

CModeler1SourceView::CModeler1SourceView()
{
}

CModeler1SourceView::~CModeler1SourceView()
{
}

BEGIN_MESSAGE_MAP(CModeler1SourceView, CEditView)
	ON_WM_CHAR()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CModeler1SourceView diagnostics

#ifdef _DEBUG
void CModeler1SourceView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CModeler1SourceView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CModeler1SourceView message handlers
void CModeler1SourceView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CEditView::OnChar(nChar, nRepCnt, nFlags);
	CEdit& ctrlEdit = this->GetEditCtrl();

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CElementManager* pManager = pFrame->GetManager();
	if (pManager->HasSelection())
	{
		shared_ptr<CElement> pElement = nullptr;
		pElement = pManager->m_selection.GetHead();

		CString text;
		ctrlEdit.GetWindowText(text);
		pElement->m_code = T2W((LPTSTR)(LPCTSTR)text);

	}
}


int CModeler1SourceView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CEdit& ctrlEdit = this->GetEditCtrl();
	CFont* pFont = ctrlEdit.GetFont();
	LOGFONT LogFont;
	pFont->GetLogFont(&LogFont);
	CFont* NewFont = new CFont();
	lstrcpy(LogFont.lfFaceName, _T("Calibri"));
	LogFont.lfHeight = 16;
	LogFont.lfWeight = FW_NORMAL;
	NewFont->CreateFontIndirect(&LogFont);
	ctrlEdit.SetFont(NewFont);

	return 0;
}
