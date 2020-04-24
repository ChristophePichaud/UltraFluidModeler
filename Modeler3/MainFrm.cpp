// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Modeler1.h"

#include "MainFrm.h"
#include "ElementContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_VIEW_PROPERTIES, OnViewProperties)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIES, OnUpdateViewProperties)
	ON_COMMAND(ID_VIEW_CLASS_VIEW, OnViewClassView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CLASS_VIEW, OnUpdateViewClassView)
	ON_COMMAND(ID_VIEW_FILE_VIEW, OnViewFileView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILE_VIEW, OnUpdateViewFileView)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_WINDOWS_7);
	m_pModelerView = NULL;
	CreateLists();
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::CreateLists()
{
	m_arInfraShapes.RemoveAll();

	m_arInfraShapes.Add(_T("AD Server"));
	m_arInfraShapes.Add(_T("Server"));
	m_arInfraShapes.Add(_T("Web Server "));
	m_arInfraShapes.Add(_T("Database Server"));
	m_arInfraShapes.Add(_T("Workstation"));
	m_arInfraShapes.Add(_T("Laptop"));
	m_arInfraShapes.Add(_T("Firewall"));
	m_arInfraShapes.Add(_T("Network"));
	m_arInfraShapes.Add(_T("Virtual Server"));
	m_arInfraShapes.Add(_T("Virtual Web Server"));
	m_arInfraShapes.Add(_T("Virtual Database Server"));
	m_arInfraShapes.Add(_T("Virtualization Server"));
	m_arInfraShapes.Add(_T("AD Server"));
	m_arInfraShapes.Add(_T("Server"));
	m_arInfraShapes.Add(_T("Database Server"));
	m_arInfraShapes.Add(_T("Server Farm"));
	m_arInfraShapes.Add(_T("Workstation"));
	m_arInfraShapes.Add(_T("laptop"));

	m_arInfraDev.RemoveAll();

	m_arInfraDev.Add(_T("Class"));
	m_arInfraDev.Add(_T("Interface"));
	m_arInfraDev.Add(_T("Enumeration"));
	m_arInfraDev.Add(_T("Package"));
	m_arInfraDev.Add(_T("Comment"));
	m_arInfraDev.Add(_T("Component"));
	m_arInfraDev.Add(_T("Association"));
	m_arInfraDev.Add(_T("Aggregation"));
	m_arInfraDev.Add(_T("Composition"));
	m_arInfraDev.Add(_T("Dependency"));
	m_arInfraDev.Add(_T("Inheritance"));
	m_arInfraDev.Add(_T("Package Import"));
	m_arInfraDev.Add(_T("Connector"));
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	//CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES);
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENUIMAGES24 : 0);

	//m_wndRibbonBar.Create(this);
	//m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
	if (!CreateRibbonBar())
	{
		TRACE0("Failed to create ribbon bar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Navigation pane will be created at left, so temporary disable docking at the left side:
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// Outlook bar is created and docking on the left side should be allowed.
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndClassView);
	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateRibbonBar()
{
	if (!m_wndRibbonBar.Create(this))
	{
		return FALSE;
	}

	// Init main button:
	InitMainButton();
	InitTabButtons();
	return TRUE;
}

void CMainFrame::CreateDocumentColors()
{
	typedef struct
	{
		COLORREF color;
		TCHAR* szName;
	}
	ColorTableEntry;

	int i = 0;
	int nNumColours = 0;

	static ColorTableEntry colorsMain [] =
	{
		{ RGB(255, 255, 255), _T("White, Background 1") },
		{ RGB(0, 0, 0), _T("Black, Text 1") },
		{ RGB(238, 236, 225), _T("Tan, Background 2") },
		{ RGB(31, 73, 125), _T("Dark Blue, Text 2") },
		{ RGB(79, 129, 189), _T("Blue, Accent 1") },
		{ RGB(192, 80, 77), _T("Red, Accent 2") },
		{ RGB(155, 187, 89), _T("Olive Green, Accent 3") },
		{ RGB(128, 100, 162), _T("Purple, Accent 4") },
		{ RGB(75, 172, 198), _T("Aqua, Accent 5") },
		{ RGB(245, 150, 70), _T("Orange, Accent 6") }
	};

	static ColorTableEntry colorsAdditional [] =
	{
		{ RGB(242, 242, 242), _T("White, Background 1, Darker 5%") },
		{ RGB(127, 127, 127), _T("Black, Text 1, Lighter 50%") },
		{ RGB(214, 212, 202), _T("Tan, Background 2, Darker 10%") },
		{ RGB(210, 218, 229), _T("Dark Blue, Text 2, Lighter 80%") },
		{ RGB(217, 228, 240), _T("Blue, Accent 1, Lighter 80%") },
		{ RGB(244, 219, 218), _T("Red, Accent 2, Lighter 80%") },
		{ RGB(234, 241, 221), _T("Olive Green, Accent 3, Lighter 80%")},
		{ RGB(229, 223, 235), _T("Purple, Accent 4, Lighter 80%") },
		{ RGB(216, 237, 242), _T("Aqua, Accent 5, Lighter 80%") },
		{ RGB(255, 234, 218), _T("Orange, Accent 6, Lighter 80%") },
		{ RGB(215, 215, 215), _T("White, Background 1, Darker 15%") },
		{ RGB(89, 89, 89), _T("Black, Text 1, Lighter 35%") },
		{ RGB(177, 176, 167), _T("Tan, Background 2, Darker 25%") },
		{ RGB(161, 180, 201), _T("Dark Blue, Text 2, Lighter 60%") },
		{ RGB(179, 202, 226), _T("Blue, Accent 1, Lighter 60%") },
		{ RGB(233, 184, 182), _T("Red, Accent 2, Lighter 60%") },
		{ RGB(213, 226, 188), _T("Olive Green, Accent 3, Lighter 60%")},
		{ RGB(203, 191, 215), _T("Purple, Accent 4, Lighter 60%") },
		{ RGB(176, 220, 231), _T("Aqua, Accent 5, Lighter 60%") },
		{ RGB(255, 212, 181), _T("Orange, Accent 6, Lighter 60%") },
		{ RGB(190, 190, 190), _T("White, Background 1, Darker 25%") },
		{ RGB(65, 65, 65), _T("Black, Text 1, Lighter 25%") },
		{ RGB(118, 117, 112), _T("Tan, Background 2, Darker 35%") },
		{ RGB(115, 143, 175), _T("Dark Blue, Text 2, Lighter 40%") },
		{ RGB(143, 177, 213), _T("Blue, Accent 1, Lighter 40%") },
		{ RGB(222, 149, 147), _T("Red, Accent 2, Lighter 40%") },
		{ RGB(192, 213, 155), _T("Olive Green, Accent 3, Lighter 40%")},
		{ RGB(177, 160, 197), _T("Purple, Accent 4, Lighter 40%") },
		{ RGB(137, 203, 218), _T("Aqua, Accent 5, Lighter 40%") },
		{ RGB(255, 191, 145), _T("Orange, Accent 6, Lighter 40%") },
		{ RGB(163, 163, 163), _T("White, Background 1, Darker 35%") },
		{ RGB(42, 42, 42), _T("Black, Text 1, Lighter 15%") },
		{ RGB(61, 61, 59), _T("Tan, Background 2, Darker 50%") },
		{ RGB(20, 57, 92), _T("Dark Blue, Text 2, Darker 25%") },
		{ RGB(54, 96, 139), _T("Blue, Accent 1, Darker 25%") },
		{ RGB(149, 63, 60), _T("Red, Accent 2, Darker 25%") },
		{ RGB(114, 139, 71), _T("Olive Green, Accent 3, Darker 25%") },
		{ RGB(97, 76, 119), _T("Purple, Accent 4, Darker 25%") },
		{ RGB(41, 128, 146), _T("Aqua, Accent 5, Darker 25%") },
		{ RGB(190, 112, 59), _T("Orange, Accent 6, Darker 25%") },
		{ RGB(126, 126, 126), _T("White, Background 1, Darker 50%") },
		{ RGB(20, 20, 20), _T("Black, Text 1, Lighter 5%") },
		{ RGB(29, 29, 28), _T("Tan, Background 2, Darker 90%") },
		{ RGB(17, 40, 64), _T("Dark Blue, Text 2, Darker 50%") },
		{ RGB(38, 66, 94), _T("Blue, Accent 1, Darker 50%") },
		{ RGB(100, 44, 43), _T("Red, Accent 2, Darker 50%") },
		{ RGB(77, 93, 49), _T("Olive Green, Accent 3, Darker 50%") },
		{ RGB(67, 53, 81), _T("Purple, Accent 4, Darker 50%") },
		{ RGB(31, 86, 99), _T("Aqua, Accent 5, Darker 50%") },
		{ RGB(126, 77, 42), _T("Orange, Accent 6, Darker 50%") },
	};

	static ColorTableEntry colorsStandard [] =
	{
		{ RGB(200, 15, 18), _T("Dark Red") },
		{ RGB(255, 20, 24), _T("Red") },
		{ RGB(255, 191, 40), _T("Orange") },
		{ RGB(255, 255, 49), _T("Yellow") },
		{ RGB(138, 207, 87), _T("Light Green") },
		{ RGB(0, 175, 84), _T("Green") },
		{ RGB(0, 174, 238), _T("Light Blue") },
		{ RGB(0, 111, 189), _T("Blue") },
		{ RGB(0, 36, 95), _T("Black") },
		{ RGB(114, 50, 157), _T("Purple") },
	};

	m_lstMainColors.RemoveAll();
	nNumColours = sizeof(colorsMain) / sizeof(ColorTableEntry);

	for (i = 0; i < nNumColours; i++)
	{
		m_lstMainColors.AddTail(colorsMain [i].color);
		CMFCRibbonColorButton::SetColorName(colorsMain [i].color, colorsMain [i].szName);
	};

	m_lstAdditionalColors.RemoveAll();
	nNumColours = sizeof(colorsAdditional) / sizeof(ColorTableEntry);

	for (i = 0; i < nNumColours; i++)
	{
		m_lstAdditionalColors.AddTail(colorsAdditional [i].color);
		CMFCRibbonColorButton::SetColorName(colorsAdditional [i].color, colorsAdditional [i].szName);
	};

	m_lstStandardColors.RemoveAll();
	nNumColours = sizeof(colorsStandard) / sizeof(ColorTableEntry);

	for (i = 0; i < nNumColours; i++)
	{
		m_lstStandardColors.AddTail(colorsStandard [i].color);
		CMFCRibbonColorButton::SetColorName(colorsStandard [i].color, colorsStandard [i].szName);
	};
}

void CMainFrame::InitMainButton()
{
	m_MainButton.SetImage(IDB_RIBBON_MAIN);
	m_MainButton.SetToolTipText(_T("File"));
	m_MainButton.SetText(_T("\nf"));

	CreateDocumentColors();

	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize(45, 45));

	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(_T("File"), IDB_RIBBON_FILESMALL, IDB_RIBBON_FILELARGE);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW, _T("&New"), 0, 0));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN, _T("&Open..."), 1, 1));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN_GABARIT, _T("&Open gabarit..."), 1, 1));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE, _T("&Save"), 2, 2));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE_AS, _T("Save &As..."), 3, 3));
	pMainPanel->AddSeparator();
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_EXPORT_PNG, _T("Export As &PNG..."), 0, 0));
	pMainPanel->AddSeparator();
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_CLOSE, _T("&Close"), 8, 8));
	pMainPanel->AddRecentFilesList(_T("Recent Documents"));
	pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, _T("E&xit"), 27));

	CMFCRibbonCategory* pCategory = m_wndRibbonBar.AddCategory(_T("&Home"), IDB_RIBBON_WRITESMALL, IDB_RIBBON_WRITELARGE);
	// Create "Design" panel
	CMFCRibbonPanel* pPanelClipboard = pCategory->AddPanel(_T("Clipboard\nc"), m_PanelImages.ExtractIcon(2));
	pPanelClipboard->Add(new CMFCRibbonButton(ID_CLIPBOARD_COPY, _T("Copy\nc"), 13));
	pPanelClipboard->Add(new CMFCRibbonButton(ID_CLIPBOARD_CUT, _T("Cut\nc"), 12));
	pPanelClipboard->Add(new CMFCRibbonButton(ID_CLIPBOARD_PASTE, _T("Paste\nc"), 11));

	CMFCRibbonPanel* pPanelDesign = pCategory->AddPanel(_T("Design\nzd"), m_PanelImages.ExtractIcon(2));
	pPanelDesign->Add(new CMFCRibbonButton(ID_DESIGN_SELECT, _T("Select\nc"), 2));
	pPanelDesign->Add(new CMFCRibbonButton(ID_DESIGN_SELECTION, _T("Selection\nc"), 2));
	pPanelDesign->Add(new CMFCRibbonButton(ID_DESIGN_SELECT_ALL, _T("Select All\nc"), 2));
	pPanelDesign->Add(new CMFCRibbonButton(ID_DESIGN_CONNECT, _T("Connect\nc"), 2));

	//pPanelDesign->Add(new CMFCRibbonButton(ID_DESIGN_RECTANGLE, _T("Rectangle\ng"), 2));
	//pPanelDesign->Add(new CMFCRibbonButton(ID_DESIGN_LINE, _T("Line\ng"), 2));
	//pPanelDesign->Add(new CMFCRibbonButton(ID_DESIGN_ELLIPSE, _T("Ellipse\ng"), 2));
	
	CMFCRibbonButton* pInsertPictureBtn = new CMFCRibbonButton(ID_DESIGN_IMAGE, _T("Picture"), 4, 4);
	pPanelDesign->Add(pInsertPictureBtn);

	CMFCRibbonGallery *pInsertShapesBtn = new CMFCRibbonGallery(ID_DESIGN_SHAPES, _T("Shapes"), 6, 6);
	pInsertShapesBtn->SetButtonMode();
	pInsertShapesBtn->SetIconsInRow(12);
	pInsertShapesBtn->AddGroup(_T("Recently Used Shapes"), IDB_SHAPE1, 20);
	pInsertShapesBtn->AddGroup(_T("Lines"), IDB_SHAPE2, 20);
	pInsertShapesBtn->AddGroup(_T("Basic Shapes"), IDB_SHAPE3, 20);
	pInsertShapesBtn->AddGroup(_T("Block Arrows"), IDB_SHAPE4, 20);
	pInsertShapesBtn->AddGroup(_T("Flowchart"), IDB_SHAPE5, 20);
	pInsertShapesBtn->AddGroup(_T("Callouts"), IDB_SHAPE6, 20);
	pInsertShapesBtn->AddGroup(_T("Stars and Banners"), IDB_SHAPE7, 20);
	pInsertShapesBtn->AddSubItem(new CMFCRibbonButton(ID_DESIGN_SHAPES_NEW, _T("&New Drawing Canvas"), 29, -1));
	pPanelDesign->Add(pInsertShapesBtn);

	CMFCRibbonButton* pBtnDrawText = new CMFCRibbonButton(ID_DESIGN_TEXTBOX, _T("Draw Text Box\nx"), 15, 15);
	pBtnDrawText->SetMenu(IDR_DRAW_TEXT_MENU);
	pPanelDesign->Add(pBtnDrawText);
	// Add hidden button
	pCategory->AddHidden(new CMFCRibbonButton(ID_TEXT_LEFT, _T("Left"), 28));
	pCategory->AddHidden(new CMFCRibbonButton(ID_TEXT_CENTER, _T("Center"), 29));
	pCategory->AddHidden(new CMFCRibbonButton(ID_TEXT_RIGHT, _T("Right"), 30));
	pCategory->AddHidden(new CMFCRibbonButton(ID_TEXT_JUSTIFY, _T("Justify"), 31));

	CRibbonListButton *pListBtnInfra = new CRibbonListButton(ID_DESIGN_SHAPESINFRA, _T("Infrastructure\nti"), 20, -1, FALSE);
	pListBtnInfra->AddGroup(_T("Built-In"), IDB_SHAPES_INFRA, 64, m_arInfraShapes);
	pListBtnInfra->SetIconsInRow(4);
	pListBtnInfra->EnableMenuResize();
	pPanelDesign->Add(pListBtnInfra);
	
	CRibbonListButton *pListBtnDev = new CRibbonListButton(ID_DESIGN_SHAPESDEV, _T("Development\ntd"), 35, -1, FALSE);
	pListBtnDev->AddGroup(_T("Built-In"), IDB_SHAPES_DEV, 64, m_arInfraDev);
	pListBtnDev->SetIconsInRow(4);
	pListBtnDev->EnableMenuResize();
	pPanelDesign->Add(pListBtnDev);

	// Create "Planning" panel
	CMFCRibbonPanel* pPanelPlanning = pCategory->AddPanel(_T("Planning\npa"), m_PanelImages.ExtractIcon(2));
	pPanelPlanning->Add(new CMFCRibbonButton(ID_DESIGN_TASK, _T("Task\ntt"), 36));
	pPanelPlanning->Add(new CMFCRibbonButton(ID_DESIGN_MONTH, _T("Month\ntm"), 37));
	pPanelPlanning->Add(new CMFCRibbonButton(ID_DESIGN_TEXT, _T("Text\ndt"), 46));

	// Create "Development" panel
	CMFCRibbonPanel* pPanelDev = pCategory->AddPanel(_T("Development\nwd"), m_PanelImages.ExtractIcon(2));
	pPanelDev->Add(new CMFCRibbonButton(ID_DESIGN_CLASS, _T("Class\ndc"), 37));
	pPanelDev->Add(new CMFCRibbonButton(ID_DESIGN_INTERFACE, _T("Interface\ndim"), 42));
	pPanelDev->Add(new CMFCRibbonButton(ID_DESIGN_ENUM, _T("Enum\nde"), 43));
	pPanelDev->Add(new CMFCRibbonButton(ID_DESIGN_PACKAGE, _T("Package\ndp"), 44));
	pPanelDev->Add(new CMFCRibbonButton(ID_DESIGN_COMMENT, _T("Comment\ndd"), 36));
	pPanelDev->Add(new CMFCRibbonButton(ID_DESIGN_COMPONENT, _T("Component\ndo"), 45));

	// Create "Debug" panel
	//CMFCRibbonPanel* pPanelDebug = pCategory->AddPanel(_T("Debug\nzd"), m_PanelImages.ExtractIcon(2));
	//pPanelDebug->Add(new CMFCRibbonButton(ID_DEBUG_DUMP_OBJECTS, _T("Dump Objects\nc"), 2));

	// Create "Font" panel
	CMFCRibbonPanel* pPanelFont = pCategory->AddPanel(_T("Font\nzd"), m_PanelImages.ExtractIcon(2));
	CMFCRibbonButtonsGroup * apFontGroup = new CMFCRibbonButtonsGroup();
	CMFCRibbonFontComboBox::m_bDrawUsingFont = TRUE;
	m_pFontCombo = new CMFCRibbonFontComboBox(ID_FONT_FONT, TRUETYPE_FONTTYPE );
	m_pFontCombo->SetWidth(55, TRUE); // Width in "floaty" mode
	m_pFontCombo->SelectItem(_T("Calibri"));
	apFontGroup->AddButton(m_pFontCombo);

	m_pFontSizeCombo = new CMFCRibbonComboBox(ID_FONT_FONTSIZE, FALSE, 39);
	m_pFontSizeCombo->AddItem(_T("8"));
	m_pFontSizeCombo->AddItem(_T("9"));
	m_pFontSizeCombo->AddItem(_T("10"));
	m_pFontSizeCombo->AddItem(_T("11"));
	m_pFontSizeCombo->AddItem(_T("12"));
	m_pFontSizeCombo->AddItem(_T("14"));
	m_pFontSizeCombo->AddItem(_T("16"));
	m_pFontSizeCombo->AddItem(_T("18"));
	m_pFontSizeCombo->AddItem(_T("20"));
	m_pFontSizeCombo->AddItem(_T("22"));
	m_pFontSizeCombo->AddItem(_T("24"));
	m_pFontSizeCombo->AddItem(_T("26"));
	m_pFontSizeCombo->AddItem(_T("28"));
	m_pFontSizeCombo->AddItem(_T("30"));
	m_pFontSizeCombo->AddItem(_T("32"));
	m_pFontSizeCombo->AddItem(_T("34"));
	m_pFontSizeCombo->AddItem(_T("36"));
	m_pFontSizeCombo->AddItem(_T("38"));
	m_pFontSizeCombo->AddItem(_T("40"));
	m_pFontSizeCombo->AddItem(_T("42"));
	m_pFontSizeCombo->AddItem(_T("44"));
	m_pFontSizeCombo->AddItem(_T("46"));
	m_pFontSizeCombo->AddItem(_T("48"));
	m_pFontSizeCombo->AddItem(_T("50"));
	m_pFontSizeCombo->AddItem(_T("52"));
	m_pFontSizeCombo->AddItem(_T("54"));
	m_pFontSizeCombo->AddItem(_T("56"));
	m_pFontSizeCombo->AddItem(_T("58"));
	m_pFontSizeCombo->AddItem(_T("60"));
	m_pFontSizeCombo->SetWidth(20, TRUE); // Width in "floaty" mode
	m_pFontSizeCombo->SelectItem(7);
	apFontGroup->AddButton(m_pFontSizeCombo);
	pPanelFont->Add(apFontGroup);

	// Add toolbar(all toolbar buttons will be automatically
	// converted to ribbon elements:
	pPanelFont->AddToolBar(IDR_FONT);

	// Replace ID_FONT_COLOR and ID_FONT_TEXTHIGHLIGHT elements
	// by color pickers:
	CMFCRibbonColorButton* pFontColorBtn = new CMFCRibbonColorButton();
	pFontColorBtn->EnableAutomaticButton(_T("&Automatic"), RGB(0, 0, 0));
	pFontColorBtn->EnableOtherButton(_T("&More Colors..."), _T("More Colors"));
	pFontColorBtn->SetColumns(10);
	pFontColorBtn->SetColor(RGB(255, 0, 0));
	pFontColorBtn->SetColorBoxSize(CSize(17, 17));
	pFontColorBtn->AddColorsGroup(_T("Theme Colors"), m_lstMainColors);
	pFontColorBtn->AddColorsGroup(_T(""), m_lstAdditionalColors, TRUE /* Contiguous Columns*/);
	pFontColorBtn->AddColorsGroup(_T("Standard Colors"), m_lstStandardColors);
	pPanelFont->ReplaceByID(ID_FONT_COLOR, pFontColorBtn);

	CMFCRibbonColorButton* pFontColorHighlightBtn = new CMFCRibbonColorButton();
	pFontColorHighlightBtn->SetColor(RGB(255, 255, 255));
	pFontColorHighlightBtn->EnableAutomaticButton(_T("&No Color"), RGB(240, 240, 240), TRUE, NULL, FALSE /* Bottom */, TRUE /* Draw border */);
	pPanelFont->ReplaceByID(ID_FONT_TEXTHIGHLIGHT, pFontColorHighlightBtn);

	pFontColorHighlightBtn->SetColorBoxSize(CSize(26, 26));
	pFontColorHighlightBtn->AddSubItem(new CMFCRibbonButton(ID_STOP_HIGHLIGHTING, _T("&Stop Highlighting")));

	// Create "Show/Hide" panel:
	CMFCRibbonPanel* pPanelShow = pCategory->AddPanel(_T("Show/Hide\nzs"), m_PanelImages.ExtractIcon(4));
	pPanelShow->Add(new CMFCRibbonCheckBox(ID_VIEW_FILE_VIEW, _T("Solution View\nc")));
	pPanelShow->Add(new CMFCRibbonCheckBox(ID_VIEW_CLASS_VIEW, _T("Class View\nc")));
	pPanelShow->Add(new CMFCRibbonCheckBox(ID_VIEW_PROPERTIES, _T("Properties View\np")));

	// Create "Action" panel
	CMFCRibbonPanel* pPanelAction = pCategory->AddPanel(_T("Action\nzd"), m_PanelImages.ExtractIcon(2));
	pPanelAction->Add(new CMFCRibbonButton(ID_ACTION_REMOVE, _T("Remove\nc"), 12));
	pPanelAction->Add(new CMFCRibbonButton(ID_ACTION_LOAD_MODULE, _T("Import .NET Module\nin"), 35));
	pPanelAction->Add(new CMFCRibbonButton(ID_DEBUG_DUMP_OBJECTS, _T("Dump Objects\ndc"), 2));

	// Create "Format" panel
	CMFCRibbonPanel* pPanelFormat = pCategory->AddPanel(_T("Format and Style\nzd"), m_PanelImages.ExtractIcon(2));
	CMFCRibbonColorButton * pBtnFillColor = new CMFCRibbonColorButton(ID_FORMAT_FILLCOLOR, _T("Fill Color\nsf"), FALSE, 1, -1);
	pBtnFillColor->SetDefaultCommand(FALSE);
	pBtnFillColor->EnableAutomaticButton(_T("&Automatic"), RGB(128, 128, 128));
	pBtnFillColor->EnableOtherButton(_T("&More Fill Colors..."), _T("More Fill Colors"));
	pBtnFillColor->SetColumns(10);
	pBtnFillColor->SetColorBoxSize(CSize(17, 17));
	pBtnFillColor->AddColorsGroup(_T("Theme Colors"), m_lstMainColors);
	pBtnFillColor->AddColorsGroup(_T(""), m_lstAdditionalColors, TRUE);
	pBtnFillColor->AddColorsGroup(_T("Standard Colors"), m_lstStandardColors);
	pBtnFillColor->AddSubItem(new CMFCRibbonButton(ID_FORMAT_NOFILL, _T("&No Fill\nn"), -1));
	pBtnFillColor->SetColor((COLORREF)-1);
	pPanelFormat->Add(pBtnFillColor);

	CMFCRibbonColorButton * pBtnLineColor = new CMFCRibbonColorButton(ID_FORMAT_LINECOLOR, _T("Line Color\nso"), FALSE, 3, -1);
	pBtnLineColor->SetDefaultCommand(FALSE);
	pBtnLineColor->EnableAutomaticButton(_T("&Automatic"), RGB(0, 0, 0));
	pBtnLineColor->EnableOtherButton(_T("&More Line Colors..."), _T("More Line Colors"));
	pBtnLineColor->SetColumns(10);
	pBtnLineColor->SetColorBoxSize(CSize(17, 17));
	pBtnLineColor->AddColorsGroup(_T("Theme Colors"), m_lstMainColors);
	pBtnLineColor->AddColorsGroup(_T(""), m_lstAdditionalColors, TRUE);
	pBtnLineColor->AddColorsGroup(_T("Standard Colors"), m_lstStandardColors);
	pBtnLineColor->AddSubItem(new CMFCRibbonButton(ID_FORMAT_NOLINE, _T("&No Line\nn"), 2));
	pBtnLineColor->SetColor((COLORREF)-1);
	pPanelFormat->Add(pBtnLineColor);

	CStringArray sa;
	sa.Add(_T("1 pt"));
	sa.Add(_T("2 pt"));
	sa.Add(_T("3 pt"));
	sa.Add(_T("4 pt"));
	sa.Add(_T("5 pt"));
	sa.Add(_T("6 pt"));
	sa.Add(_T("7 pt"));
	
	CRibbonListButton * pBtnLineWeight = new CRibbonListButton(ID_FORMAT_LINEWIDTH, _T("Line Width\nsw"), 4, -1, IDB_LINEWEIGHT, 96, sa);
	pBtnLineWeight->AddSubItem(new CMFCRibbonButton(ID_FORMAT_LINEWIDTH_MORE, _T("More &Lines..."), 5, -1));
	pBtnLineWeight->EnableMenuResize(TRUE, TRUE); // Vertical only
	pPanelFormat->Add(pBtnLineWeight);

	CMFCRibbonColorButton * pBtnPageColor = new CMFCRibbonColorButton(ID_FORMAT_PAGECOLOR, _T("Page Color\npo"), TRUE, -1, 26);
	pBtnPageColor->SetDefaultCommand(FALSE);
	pBtnPageColor->EnableAutomaticButton(_T("&Automatic"), RGB(0, 0, 0));
	pBtnPageColor->EnableOtherButton(_T("&More Line Colors..."), _T("More Line Colors"));
	pBtnPageColor->SetColumns(10);
	pBtnPageColor->SetColorBoxSize(CSize(17, 17));
	pBtnPageColor->AddColorsGroup(_T("Theme Colors"), m_lstMainColors);
	pBtnPageColor->AddColorsGroup(_T(""), m_lstAdditionalColors, TRUE);
	pBtnPageColor->AddColorsGroup(_T("Standard Colors"), m_lstStandardColors);
	pPanelFormat->Add(pBtnPageColor);
	pPanelFormat->AddSeparator();
	pPanelFormat->Add(new CMFCRibbonButton(ID_FORMAT_ZOOM_IN, _T("Zoom In\ni"), -1));
	pPanelFormat->Add(new CMFCRibbonButton(ID_FORMAT_ZOOM_OUT, _T("Zoom Out\no"), -1));
	CMFCRibbonComboBox *pBtnZoom = new CMFCRibbonComboBox(ID_FORMAT_ZOOM, FALSE, 50, _T("Zoom: "), -1);
	pBtnZoom->AddItem(_T("25%"));
	pBtnZoom->AddItem(_T("50%"));
	pBtnZoom->AddItem(_T("75%"));
	pBtnZoom->AddItem(_T("100%"));
	pBtnZoom->AddItem(_T("150%"));
	pBtnZoom->AddItem(_T("200%"));
	pBtnZoom->AddItem(_T("400%"));
	pBtnZoom->SelectItem(3);
	pPanelFormat->Add(pBtnZoom);
	pPanelFormat->AddSeparator();
	pPanelFormat->Add(new CMFCRibbonButton(ID_FORMAT_TEXT_ALIGN_LEFT, _T("Text Align Left\nal"), 28));
	pPanelFormat->Add(new CMFCRibbonButton(ID_FORMAT_TEXT_ALIGN_CENTER, _T("Text Align Center\nac"), 29));
	pPanelFormat->Add(new CMFCRibbonButton(ID_FORMAT_TEXT_ALIGN_RIGHT, _T("Text Align Right\nar"), 30));
	pPanelFormat->AddSeparator();
	pPanelFormat->Add(new CMFCRibbonButton(ID_FORMAT_ALIGN_LEFT, _T("Align Left\nal"), 38));
	pPanelFormat->Add(new CMFCRibbonButton(ID_FORMAT_ALIGN_RIGHT, _T("Align Right\nar"), 39));
	pPanelFormat->Add(new CMFCRibbonButton(ID_FORMAT_ALIGN_TOP, _T("Align Top\nat"), 40));
	pPanelFormat->Add(new CMFCRibbonButton(ID_FORMAT_ALIGN_BOTTOM, _T("Align Bottom\nab"), 41));

	// Create "Position" panel
	CMFCRibbonPanel* pPanelPosition = pCategory->AddPanel(_T("Position\nzd"), m_PanelImages.ExtractIcon(2));
	pPanelPosition->Add(new CMFCRibbonButton(ID_POSITION_MOVETOFRONT, _T("Move to Front\nc"), -1, 22));
	pPanelPosition->Add(new CMFCRibbonButton(ID_POSITION_MOVEFORWARD, _T("Move Forward\nc"), -1, 24));
	pPanelPosition->Add(new CMFCRibbonButton(ID_POSITION_MOVEBACKWARD, _T("Move Backward\nc"), -1, 25));
	pPanelPosition->Add(new CMFCRibbonButton(ID_POSITION_MOVETOBACK, _T("Move to Back\nc"), -1, 23));


}

void CMainFrame::InitTabButtons()
{
	// Add "Style" button to the right of tabs:
	CMFCRibbonButton* pStyleButton = new CMFCRibbonButton((UINT) -1, _T("Style\ns"));
	pStyleButton->SetMenu(IDR_THEME_MENU, TRUE /* Right align */);

	m_wndRibbonBar.AddToTabs(pStyleButton);

	// Add "About" button to the right of tabs:
	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon(6)));

}

BOOL CMainFrame::CreateDockingWindows()
{
	// Crate class view:
	if (!m_wndClassView.Create(_T("Class View"), this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASS, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Class View bar\n");
		return FALSE;      // fail to create
	}

	// Create file view:
	if (!m_wndFileView.Create(_T("Solution Explorer"), this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Solution Explorer bar\n");
		return FALSE;      // fail to create
	}

	BOOL bNameValid;
	// Create output window
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}

	// Create properties window
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG

void CMainFrame::LogDebug(CString message)
{
	m_wndOutput.AddStringToDebug(message);
}

// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		//m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		//m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		//m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		//m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		//m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		//m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		//m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		//m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}

void CMainFrame::UpdatePropertiesFromObject(std::shared_ptr<CElement> pElement)
{
	m_wndProperties.UpdateProperties(pElement);
}

void CMainFrame::InitClassView()
{
	m_wndClassView.InitClassView();
}

void CMainFrame::UpdateClassViewFromObject(std::shared_ptr<CElement> pElement)
{
	m_wndClassView.UpdateFromObject(pElement);
}

void CMainFrame::InitFileView()
{
	m_wndFileView.InitFileView();
}

void CMainFrame::UpdateFileViewFromObject(std::shared_ptr<CElement> pElement)
{
	m_wndFileView.UpdateFromObject(pElement);
}

void CMainFrame::SetManager(CElementManager * pManager)
{
	m_pManager = pManager;
	m_wndProperties.SetManager(pManager);
}

void CMainFrame::SetView(CModeler1View * pView)
{
	m_pModelerView = pView;
}

void CMainFrame::OnViewProperties()
{
	m_wndProperties.ShowPane(!m_wndProperties.IsVisible(), FALSE, TRUE);
}
void CMainFrame::OnUpdateViewProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndProperties.IsVisible());
}

void CMainFrame::OnViewClassView()
{
	m_wndClassView.ShowPane(!m_wndClassView.IsVisible(), FALSE, TRUE);
}

void CMainFrame::OnUpdateViewClassView(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndClassView.IsVisible());
}

void CMainFrame::OnViewFileView()
{
	m_wndFileView.ShowPane(!m_wndFileView.IsVisible(), FALSE, TRUE);
}

void CMainFrame::OnUpdateViewFileView(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndFileView.IsVisible());
}

COLORREF CMainFrame::GetColorFromColorButton(int nButtonID)
{
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;

	m_wndRibbonBar.GetElementsByID(nButtonID, arButtons);

	if (arButtons.GetSize() == 0)
	{
		return(COLORREF) -1;
	}

	CMFCRibbonColorButton* pColorButton = (CMFCRibbonColorButton*)arButtons.GetAt(0);

	COLORREF clr = pColorButton->GetColor();
	if (clr == (COLORREF)-1)
	{
		clr = pColorButton->GetAutomaticColor();
	}

	return clr;
}

int CMainFrame::GetWidthFromLineWidth(int nButtonID)
{
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;

	m_wndRibbonBar.GetElementsByID(nButtonID, arButtons);

	if (arButtons.GetSize() == 0)
	{
		return -1;
	}

	int width = -1;
	CMFCRibbonGallery* pBtn = DYNAMIC_DOWNCAST(CMFCRibbonGallery, arButtons.GetAt(0));
	if (pBtn != NULL && pBtn->GetSelectedItem() != -1)
	{
		width = pBtn->GetSelectedItem() + 1;
	}

	return width;
}

void CMainFrame::UpdateRibbonUI(CModeler1View * pView)
{
	USES_CONVERSION;

	CElementContainer & selection = pView->GetManager()->m_selection;
	int count = selection.GetCount();

	if( count == 0 )
	{
		return;
	}
	
	COLORREF clrFill = (COLORREF)-1;
	COLORREF clrLine = (COLORREF)-1;
	int nLineWidth  = -1;
	if (count == 1)
	{
		std::shared_ptr<CElement> pElement = selection.GetHead();
		if (pElement->CanChangeFillColor() )
		{
			clrFill = pElement->m_colorFill;
		}
		if (pElement->CanChangeLineColor() )
		{
			clrLine = pElement->m_colorLine;
		}
		if (pElement->CanChangeLineWidth() )
		{
			nLineWidth = pElement->m_lineWidth - 1;
		}
	}

	{
		CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
		m_wndRibbonBar.GetElementsByID(ID_FORMAT_FILLCOLOR, arButtons);

		if (arButtons.GetSize() > 0)
		{
			for (int i = 0; i < arButtons.GetSize(); i++)
			{
				CMFCRibbonColorButton* pButton = DYNAMIC_DOWNCAST
					(CMFCRibbonColorButton, arButtons[i]
				);
				pButton->SetColor(clrFill);
			}
		}
	}

	{
		CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
		m_wndRibbonBar.GetElementsByID(ID_FORMAT_LINECOLOR, arButtons);

		if (arButtons.GetSize() > 0)
		{
			for (int i = 0; i < arButtons.GetSize(); i++)
			{
				CMFCRibbonColorButton* pButton = DYNAMIC_DOWNCAST
					(CMFCRibbonColorButton, arButtons[i]
				);
				pButton->SetColor(clrLine);
			}
		}
	}

	{
		CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
		m_wndRibbonBar.GetElementsByID(ID_FORMAT_LINEWIDTH, arButtons);

		if (arButtons.GetSize() > 0)
		{
			for (int i = 0; i < arButtons.GetSize(); i++)
			{
				CRibbonListButton* pButton = DYNAMIC_DOWNCAST
					(CRibbonListButton, arButtons[i]
				);
				pButton->SelectItem(nLineWidth);
			}
		}
	}

	// Update font combo boxes name and size
	std::shared_ptr<CElement> pElement = selection.GetHead();
	m_pFontCombo->SelectItem(pElement->m_fontName.c_str());
	TCHAR sz[255];
	_stprintf_s(sz, _T("%d"), pElement->m_fontSize);
	m_pFontSizeCombo->SelectItem(sz);


	{
		Color color = pElement->m_colorText;
		CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
		m_wndRibbonBar.GetElementsByID(ID_FONT_COLOR, arButtons);
		if (arButtons.GetSize() > 0)
		{
			for (int i = 0; i < arButtons.GetSize(); i++)
			{
				CMFCRibbonColorButton* pButton = DYNAMIC_DOWNCAST(CMFCRibbonColorButton, arButtons[i]);
				pButton->SetColor(color.GetValue());
			}
		}
	}

	{
		Color color = pElement->m_colorFill;
		CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arButtons;
		m_wndRibbonBar.GetElementsByID(ID_FONT_TEXTHIGHLIGHT, arButtons);
		if (arButtons.GetSize() > 0)
		{
			for (int i = 0; i < arButtons.GetSize(); i++)
			{
				CMFCRibbonColorButton* pButton = DYNAMIC_DOWNCAST(CMFCRibbonColorButton, arButtons[i]);
				pButton->SetColor(color.GetValue());
			}
		}
	}
	m_wndRibbonBar.RedrawWindow();

}

/*
BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMainFrame::AdjustClientArea()
{
	if (m_pPrintPreviewFrame != NULL)
	{
		CFrameWndEx::AdjustClientArea();
		return;
	}

	CWnd* pChildWnd = GetDlgItem(AFX_IDW_PANE_FIRST);
	if (pChildWnd != NULL)
	{
		CRect rectClientAreaBounds = m_dockManager.GetClientAreaBounds();

		rectClientAreaBounds.left += m_rectBorder.left;
		rectClientAreaBounds.top  += m_rectBorder.top;
		rectClientAreaBounds.right -= m_rectBorder.right;
		rectClientAreaBounds.bottom -= m_rectBorder.bottom;

		if (rectClientAreaBounds.Width() > 100 && rectClientAreaBounds.Height() > 100)
		{
			m_rectFill = rectClientAreaBounds;
			rectClientAreaBounds.DeflateRect(40, 20, 40, 20);
		}
		else
		{
			m_rectFill.SetRectEmpty();
		}

		pChildWnd->CalcWindowRect(rectClientAreaBounds);

		if (!pChildWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
		{
			pChildWnd->ModifyStyle(0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

			if (!m_rectFill.IsRectEmpty())
			{
				pChildWnd->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
				pChildWnd->ModifyStyle(0, WS_BORDER);
			}
		}
		else
		{
			pChildWnd->ModifyStyle(0, WS_CLIPSIBLINGS);
		}

		pChildWnd->SetWindowPos(&wndBottom, rectClientAreaBounds.left, rectClientAreaBounds.top, rectClientAreaBounds.Width(), rectClientAreaBounds.Height(), SWP_NOACTIVATE);
	}

	if (!m_rectFill.IsRectEmpty())
	{
		RedrawWindow(m_rectFill);
	}
}

void CMainFrame::OnPaint()
{
	if (m_rectFill.IsRectEmpty())
	{
		Default();
		return;
	}

	CPaintDC dc(this); // device context for painting

	CRect rectChild;
	rectChild.SetRectEmpty();

	CWnd* pChildWnd = GetDlgItem(AFX_IDW_PANE_FIRST);
	if (pChildWnd != NULL)
	{
		pChildWnd->GetWindowRect(rectChild);
		ScreenToClient(rectChild);
	}

	CRect rectScreen;

	MONITORINFO mi;
	mi.cbSize = sizeof(MONITORINFO);
	if (GetMonitorInfo(MonitorFromPoint(CPoint(0, 0), MONITOR_DEFAULTTONEAREST), &mi))
	{
		rectScreen = mi.rcWork;
	}
	else
	{
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);
	}

	if (m_MemBitmap.GetSafeHandle() != NULL)
	{
		BITMAP bmp;
		m_MemBitmap.GetBitmap(&bmp);

		if (bmp.bmWidth != rectScreen.Width() || bmp.bmHeight != rectScreen.Height())
		{
			m_MemBitmap.DeleteObject();
		}
	}

	if (m_MemBitmap.GetSafeHandle() == NULL)
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);

		m_MemBitmap.CreateCompatibleBitmap(&dc, rectScreen.Width(), rectScreen.Height());

		CBitmap* pBmpOld = dcMem.SelectObject(&m_MemBitmap);

		CDrawingManager dm(dcMem);

		rectScreen.OffsetRect(-rectScreen.TopLeft());

		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_2003:
			::FillRect(dcMem.GetSafeHdc(), rectScreen, ::GetSysColorBrush(COLOR_APPWORKSPACE));
			break;

		case ID_VIEW_APPLOOK_WIN7:
			dm.FillGradient(rectScreen, RGB (197, 207, 223), RGB (219, 228, 241), TRUE);
			break;

		case ID_VIEW_APPLOOK_2007_3:
			dm.FillGradient(rectScreen, RGB(114, 125, 152), RGB(178, 185, 202), TRUE);
			break;

		default:
			{
				const BOOL bLuna     = theApp.m_nAppLook == ID_VIEW_APPLOOK_2007_1;
				const BOOL bObsidian = theApp.m_nAppLook == ID_VIEW_APPLOOK_2007_2;

				CMFCControlRenderer& pat = bLuna ? m_Pat[1] : bObsidian ? m_Pat[2] : m_Pat[3];

				COLORREF clrF = bLuna ? RGB(136, 170, 214) : bObsidian ? RGB( 71,  71,  71) : RGB(164, 170, 186);
				COLORREF clrM = bLuna ? RGB( 86, 125, 176) : bObsidian ? RGB( 56,  56,  56) : RGB(156, 160, 167);
				COLORREF clrL = bLuna ? RGB(101, 145, 205) : bObsidian ? RGB( 11,  11,  11) : RGB(204, 207, 216);

				const CSize szPat = pat.GetParams().m_rectImage.Size();

				CRect rectPat = rectScreen;

				rectPat.bottom = rectPat.top + szPat.cy;

				CRect rectGradient = rectScreen;
				rectGradient.top += szPat.cy;

				if (rectGradient.Height() < 50 ||
					(rectGradient.Height() - 120) < 50)
				{
					rectGradient.bottom = rectGradient.top + 50;
				}
				else
				{
					rectGradient.bottom -= 120;
				}

				dm.FillGradient(rectGradient, clrM, clrF, TRUE);

				if (rectGradient.bottom < rectScreen.bottom)
				{
					rectGradient.top    = rectGradient.bottom;
					rectGradient.bottom = rectScreen.bottom;

					dm.FillGradient(rectGradient, clrL, clrM, TRUE);
				}

				pat.Draw(&dcMem, rectPat, 0);
			}
		}

		dcMem.SelectObject(pBmpOld);
	}

	CRect rectFill = m_rectFill;

	if (!m_rectSizing.IsRectEmpty())
	{
		dc.FillRect(m_rectSizing, &afxGlobalData.brBarFace);
		rectFill.right += m_rectSizing.Width();
	}

	dc.DrawState(rectFill.TopLeft(), rectFill.Size(), &m_MemBitmap, DSS_NORMAL);
}
*/

