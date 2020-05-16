// Copyright (C) SAS NET Azure Rangers
// All rights reserved.

#include "StdAfx.h"
#include "Modeler1.h"
#include "MainFrm.h"
#include "ClassView.h"

/////////////////////////////////////////////////////////////////////////////
// CClassTreeWnd

CClassTreeWnd::CClassTreeWnd()
{
}

CClassTreeWnd::~CClassTreeWnd()
{
}

BEGIN_MESSAGE_MAP(CClassTreeWnd, CTreeCtrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassTreeWnd message handlers

BOOL CClassTreeWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CClassViewBar;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClassViewBar::CClassViewBar()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CClassViewBar::~CClassViewBar()
{
}

BEGIN_MESSAGE_MAP(CClassViewBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMEBER_FUNCTION, OnClassAddMemeberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMEBER_VARIABLE, OnClassAddMemeberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassViewBar message handlers

int CClassViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	/*
	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}
	*/

	// Fill view context(dummy code, don't seek here something magic :-)):
	InitClassView();

	return 0;
}

void CClassViewBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CClassViewBar::InitClassView()
{
	m_wndClassView.DeleteAllItems();
	
	m_hRoot = m_wndClassView.InsertItem(_T("Modeler Objects"), 0, 0);

	HTREEITEM hRoot = m_hRoot;
	m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	m_hInfra = m_wndClassView.InsertItem(_T("Infrastructure"), 2, 2, hRoot);
	m_hDev = m_wndClassView.InsertItem(_T("Development"), 2, 2, hRoot);
	m_hGlobal = m_wndClassView.InsertItem(_T("Globals"), 2, 2, hRoot);

	HTREEITEM hClass = m_wndClassView.InsertItem(_T("CMainFrame"), 1, 1, m_hDev);
	m_wndClassView.InsertItem(_T("AssertValid()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("CMainFrame()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("~CMainFrame()"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("Dump(CDumpContext& dc)"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("OnCreate(LPCREATESTRUCT lpCreateStruct)"), 4, 4, hClass);
	m_wndClassView.InsertItem(_T("PreCreateWindow(CREATESTRUCT& cs)"), 3, 3, hClass);
	m_wndClassView.InsertItem(_T("m_wndMenuBar"), 6, 6, hClass);
	m_wndClassView.InsertItem(_T("m_wndToolBar"), 6, 6, hClass);
	m_wndClassView.InsertItem(_T("m_wndStatusBar"), 6, 6, hClass);
	//m_wndClassView.Expand(hClass, TVE_EXPAND);

	m_wndClassView.InsertItem(_T("theApp"), 5, 5, m_hGlobal);

	m_wndClassView.Expand(m_hGlobal, TVE_EXPAND);
	m_wndClassView.Expand(m_hDev, TVE_EXPAND);
	m_wndClassView.Expand(m_hInfra, TVE_EXPAND);
	m_wndClassView.Expand(m_hRoot, TVE_EXPAND);
}

void CClassViewBar::UpdateFromObject(std::shared_ptr<CElement> pElement)
{
	CString str;
	HTREEITEM hParent;
	if( pElement->m_type == ElementType::type_shapes_infrastructure )
		hParent = m_hInfra;
	else if( pElement->m_type == ElementType::type_shapes_development )
		hParent = m_hDev;
	else
		hParent = m_hGlobal;

	str.Format(_T("%s (%s)"), pElement->m_name.c_str(), pElement->ToString(pElement->m_shapeType));
	HTREEITEM hClass = m_wndClassView.InsertItem(str, 1, 1, hParent);
	//m_wndClassView.InsertItem(_T("CAboutDlg()"), 3, 3, hClass);
	//m_wndClassView.InsertItem(_T("DoDataExchange(CDataExchange* pDX)"), 4, 4, hClass);
	HTREEITEM hProperties = m_wndClassView.InsertItem(_T("Properties"), 2, 2, hClass);
	str.Format(_T("m_objectId : %s"), pElement->m_objectId.c_str());
	m_wndClassView.InsertItem(str, 5, 5, hProperties);
	str.Format(_T("m_type : %s"), pElement->ToString(pElement->m_type));
	m_wndClassView.InsertItem(str, 5, 5, hProperties);
	str.Format(_T("m_text : %s"), pElement->m_text.c_str());
	m_wndClassView.InsertItem(str, 5, 5, hProperties);

	m_wndClassView.Expand(m_hGlobal, TVE_EXPAND);
	m_wndClassView.Expand(m_hDev, TVE_EXPAND);
	m_wndClassView.Expand(m_hInfra, TVE_EXPAND);
	m_wndClassView.Expand(m_hRoot, TVE_EXPAND);
}

void CClassViewBar::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
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
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

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

void CClassViewBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndClassView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CClassViewBar::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CClassViewBar::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CClassViewBar::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CClassViewBar::OnClassAddMemeberFunction()
{
	AfxMessageBox(_T("Add member function..."));
}

void CClassViewBar::OnClassAddMemeberVariable()
{
	// TODO: Add your command handler code here
}

void CClassViewBar::OnClassDefinition()
{
	// TODO: Add your command handler code here
}

void CClassViewBar::OnClassProperties()
{
	// TODO: Add your command handler code here
}

void CClassViewBar::OnNewFolder()
{
	AfxMessageBox(_T("New Folder..."));
}

void CClassViewBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CClassViewBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndClassView.SetFocus();
}

void CClassViewBar::OnChangeVisualStyle()
{
	/*
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT24 : IDR_SORT, 0, 0, TRUE);
	*/
}



