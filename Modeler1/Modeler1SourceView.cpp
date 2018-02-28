// Modeler1SourceView.cpp : implementation file
//

#include "stdafx.h"
#include "Modeler1.h"
#include "Modeler1SourceView.h"


// CModeler1SourceView

IMPLEMENT_DYNCREATE(CModeler1SourceView, CEditView)

CModeler1SourceView::CModeler1SourceView()
{

}

CModeler1SourceView::~CModeler1SourceView()
{
}

BEGIN_MESSAGE_MAP(CModeler1SourceView, CEditView)
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
