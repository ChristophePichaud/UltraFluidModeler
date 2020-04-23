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
