#include "StdAfx.h"
#include "Modeler1.h"
#include "MainFrm.h"
#include "FileView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileViewBar

CFileViewBar::CFileViewBar()
{
}

CFileViewBar::~CFileViewBar()
{
}

BEGIN_MESSAGE_MAP(CFileViewBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_SOLUTION_OPEN, OnSolutionOpen)
	ON_COMMAND(ID_SOLUTION_OPEN_WITH, OnSolutionOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_SHAPES_LEFT_TOP, OnShapesLeftTop)
	ON_COMMAND(ID_SHAPES_CENTER, OnShapesCenter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CFileViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create solution explorer\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SOLUTION_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_SOLUTION_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill view context(dummy code, don't seek here something magic :-)):
	InitFileView();
	AdjustLayout();

	return 0;
}

void CFileViewBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileViewBar::InitFileView()
{
	m_wndFileView.DeleteAllItems();

	m_hRoot = m_wndFileView.InsertItem(_T("Shapes Collection"), 0, 0);
	m_wndFileView.SetItemState(m_hRoot, TVIS_BOLD, TVIS_BOLD);

	//m_hSrc = m_wndFileView.InsertItem(_T("Basic Shapes"), 0, 0, m_hRoot);


	HTREEITEM hParent = nullptr;
	HTREEITEM hBasicShape = nullptr;
	HTREEITEM hTextShape = nullptr;
	HTREEITEM hImageShape = nullptr;
	HTREEITEM hInfrastructureShape = nullptr;
	HTREEITEM hDevelopmentShape = nullptr;
	HTREEITEM hPlanningShape = nullptr;

	for (int ishape = 0; ishape < 600; ++ishape)
	{

		if (ishape == OffsetShapes_Simple)
		{
			hParent = m_wndFileView.InsertItem(_T("Basic Shapes"), 0, 0, m_hRoot);
			hBasicShape = hParent;
		}
		if (ishape == OffsetShapes_Text)
		{
			hParent = m_wndFileView.InsertItem(_T("Text Shapes"), 0, 0, m_hRoot);
			hTextShape = hParent;
		}
		if (ishape == OffsetShapes_Image)
		{
			hParent = m_wndFileView.InsertItem(_T("Images Shapes"), 0, 0, m_hRoot);
			hImageShape = hParent;
		}
		if (ishape == OffsetShapes_Infrastructure)
		{
			hParent = m_wndFileView.InsertItem(_T("Infrastructure Shapes"), 0, 0, m_hRoot);
			hInfrastructureShape = hParent;
		}
		if (ishape == OffsetShapes_Development)
		{
			hParent = m_wndFileView.InsertItem(_T("Development Shapes"), 0, 0, m_hRoot);
			hDevelopmentShape = hParent;
		}
		if (ishape == OffsetShapes_Planning)
		{
			hParent = m_wndFileView.InsertItem(_T("Planning Shapes"), 0, 0, m_hRoot);
			hPlanningShape = hParent;
		}


		CString str = CElement::static_ToString((ShapeType)ishape);

		if (str == _T(""))
		{
			continue;
		}

		HTREEITEM hItem = m_wndFileView.InsertItem(str, 1, 1, hParent);
		m_wndFileView.SetItemData(hItem, ishape);
	}

	m_wndFileView.Expand(m_hRoot, TVE_EXPAND);
	//m_wndFileView.Expand(m_hSrc, TVE_EXPAND);
	/*
	m_wndFileView.Expand(hBasicShape, TVE_EXPAND);
	m_wndFileView.Expand(hTextShape, TVE_EXPAND);
	m_wndFileView.Expand(hImageShape, TVE_EXPAND);
	m_wndFileView.Expand(hInfrastructureShape, TVE_EXPAND);
	m_wndFileView.Expand(hDevelopmentShape, TVE_EXPAND);
	m_wndFileView.Expand(hPlanningShape, TVE_EXPAND);
	*/
}

void CFileViewBar::UpdateFromObject(std::shared_ptr<CElement> pElement)
{
	/*
	CString str;
	HTREEITEM hParent;
	if( pElement->m_type == ElementType::type_shapes_infrastructure )
		hParent = m_hSrc;
	else if( pElement->m_type == ElementType::type_shapes_development )
		hParent = m_hSrc;
	else
		hParent = m_hOthers;

	str.Format(_T("%s_%s.cpp"), pElement->m_text.c_str(), pElement->m_name.c_str());
	m_wndFileView.InsertItem(str, 1, 1, hParent);
	str.Format(_T("%s_%s.h"), pElement->m_text.c_str(), pElement->m_name.c_str());
	m_wndFileView.InsertItem(str, 1, 1, m_hInc);
	*/
}

void CFileViewBar::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}

		CString text = m_wndFileView.GetItemText(hTreeItem);
		DWORD data = m_wndFileView.GetItemData(hTreeItem);
		if ((data == 0) && (text != _T("line")))
		{
			return;
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SHAPES);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CFileViewBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CFileViewBar::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CFileViewBar::OnProperties()
{
	AfxMessageBox(_T("Properties..."));

}

void CFileViewBar::OnSolutionOpen()
{
	// TODO: Add your command handler code here
}

void CFileViewBar::OnSolutionOpenWith()
{
	// TODO: Add your command handler code here
}

void CFileViewBar::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CFileViewBar::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CFileViewBar::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CFileViewBar::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CFileViewBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileViewBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileViewBar::OnChangeVisualStyle()
{
}

void CFileViewBar::OnShapesLeftTop()
{
	HTREEITEM hTreeItem = m_wndFileView.GetSelectedItem();
	if (hTreeItem == NULL)
	{
		return;
	}

	CString text = m_wndFileView.GetItemText(hTreeItem);
	DWORD type = m_wndFileView.GetItemData(hTreeItem);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CModeler1View* pView = pMainFrame->GetActiveView();
	if (pView != nullptr)
	{
		pMainFrame->GetManager()->OnShapesLeftTop(pView, (ShapeType)type);
	}
}

void CFileViewBar::OnShapesCenter()
{
	HTREEITEM hTreeItem = m_wndFileView.GetSelectedItem();
	if (hTreeItem == NULL)
	{
		return;
	}

	CString text = m_wndFileView.GetItemText(hTreeItem);
	DWORD type = m_wndFileView.GetItemData(hTreeItem);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CModeler1View* pView = pMainFrame->GetActiveView();
	if (pView != nullptr)
	{
		pMainFrame->GetManager()->OnShapesCenter(pView, (ShapeType)type);
	}
}
