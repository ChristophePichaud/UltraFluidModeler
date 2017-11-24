#include "stdafx.h"
#include "Modeler1.h"

#include "Modeler1Doc.h"
#include "TabbedView.h"

#include "Modeler1View.h"
#include "Modeler1SourceView.h"
//#include "SourceCodeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabbedView

IMPLEMENT_DYNCREATE(CTabbedView, CTabView)

BEGIN_MESSAGE_MAP(CTabbedView, CTabView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTabView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTabView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabbedView construction/destruction

CTabbedView::CTabbedView()
{
}

CTabbedView::~CTabbedView()
{
}

BOOL CTabbedView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CTabView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTabbedView drawing

void CTabbedView::OnDraw(CDC* /*pDC*/)
{
//	CTabbedViewDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTabbedView printing

void CTabbedView::OnFilePrintPreview() 
{
	AFXPrintPreview (this);
}

BOOL CTabbedView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTabbedView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTabbedView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTabbedView diagnostics

#ifdef _DEBUG
void CTabbedView::AssertValid() const
{
	CTabView::AssertValid();
}

void CTabbedView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}

CModeler1Doc* CTabbedView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModeler1Doc)));
	return (CModeler1Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTabbedView message handlers

int CTabbedView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	AddView (RUNTIME_CLASS (CModeler1View), _T("Modeling"), 100);
	AddView (RUNTIME_CLASS (CModeler1SourceView), _T("Comments"), 101);
	//AddView (RUNTIME_CLASS (CSourceCodeView), _T("Code"), 102);


	return 0;
}

BOOL CTabbedView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}
