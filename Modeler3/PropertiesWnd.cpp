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

#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "Modeler1.h"
#include "Element.h"
#include "ElementManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

void CPropertiesWnd::SetManager(CElementManager * pManager)
{
	m_pManager = pManager;
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	m_wndObjectCombo.AddString(_T("Application"));
	m_wndObjectCombo.AddString(_T("Properties Window"));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	
	CMFCPropertyGridProperty* pProp = NULL;

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Appearance"));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Visible"), (_variant_t) true, _T("Specifies the object's visibility")));
	/*
	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Border"), _T("Dialog Frame"), _T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(pProp);
	*/
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Caption"), (_variant_t) _T(""), _T("Specifies the text that will be displayed in the window's title bar")));
	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Object Position/Size"), 0, TRUE);
	pProp = new CMFCPropertyGridProperty(_T("Left"), (_variant_t) (long)0, _T("Specifies the object's left position"));
	pProp->EnableSpinControl(TRUE, 0, 2000);
	pSize->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Top"), (_variant_t) (long)0, _T("Specifies the object's top position"));
	pProp->EnableSpinControl(TRUE, 0, 2000);
	pSize->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Right"), (_variant_t) (long)0, _T("Specifies the object's right"));
	pProp->EnableSpinControl(TRUE, 0, 2000);
	pSize->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty( _T("Bottom"), (_variant_t) (long)0, _T("Specifies the object's bottom"));
	pProp->EnableSpinControl(TRUE, 0, 2000);
	pSize->AddSubItem(pProp);
	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Font"));
	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);
	lstrcpy(lf.lfFaceName, _T("Arial"));
	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window")));
	//pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"), (_variant_t) true, _T("Specifies that the window uses MS Shell Dlg font")));
	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Misc"));
	pProp = new CMFCPropertyGridProperty(_T("Name"), _T("Application"));
	//pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("ID"), (_variant_t) _T(""), _T("Specifies the object's ID"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);
	pProp = new CMFCPropertyGridProperty(_T("Type"), (_variant_t) _T(""), _T("Specifies the object's type"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);
	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Text"), (_variant_t) _T(""), _T("Specifies the object's text")));
	pProp = new CMFCPropertyGridProperty(_T("Text Align"), _T(""), _T("Specifies the alignment of the text"));
	pProp->AddOption(_T("Left"));
	pProp->AddOption(_T("Center"));
	pProp->AddOption(_T("Right"));
	pProp->AllowEdit(FALSE);
	pGroup3->AddSubItem(pProp);
	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Font Name"), (_variant_t) _T("Calibri"), _T("Specifies the font of the text")));

	pProp = new CMFCPropertyGridProperty(_T("Font Size"), (_variant_t) (long)12, _T("Specifies the font's height of the text"));
	pProp->EnableSpinControl(TRUE, 0, 100);
	pGroup3->AddSubItem(pProp);
	CString strFilter = _T("Image Files(*.bmp;*.gif;*.jpg;*.png)|*.bmp;*.gif;*.jpg;*.png|All Files(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Image"), TRUE, _T(""), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
								strFilter, _T("Specifies the object's image"), 0));
	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Fixed"), (_variant_t)false, _T("Specifies the object's image size")));
	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Has Fill Color"), (_variant_t) false, _T("Specifies the object's fill color style")));
	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Is Fill Solid Color"), (_variant_t) false, _T("Specifies the object's fill solid color style")));
	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Has Line Color"), (_variant_t) true, _T("Specifies the object's line color style")));

	CMFCPropertyGridColorProperty* pFillColorProp = new CMFCPropertyGridColorProperty(_T("Fill Color"), RGB(255, 255, 255), NULL, _T("Specifies the object fill color"));
	pFillColorProp->EnableOtherButton(_T("Other..."));
	pFillColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_WINDOW)); // Window Bakcground Color
	pGroup3->AddSubItem(pFillColorProp);

	CMFCPropertyGridColorProperty* pLineColorProp = new CMFCPropertyGridColorProperty(_T("Line Color"), RGB(0, 0, 0), NULL, _T("Specifies the object line color"));
	pLineColorProp->EnableOtherButton(_T("Other..."));
	pLineColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_WINDOW)); // Window Bakcground Color
	pGroup3->AddSubItem(pLineColorProp);

	CString strFilter2 = _T("Diagram|*.sch;*.dia;*.dgm|All Files(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Document"), TRUE, _T(""), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		strFilter2, _T("Specifies the file's document"), 0));

	pProp = new CMFCPropertyGridProperty(_T("Document Type"), _T(""), _T("Specifies the type of document"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("File"));
	pProp->AddOption(_T("Folder"));
	pProp->AddOption(_T("Diagram"));
	pProp->AllowEdit(FALSE);
	pGroup3->AddSubItem(pProp);

	m_wndPropList.AddProperty(pGroup3);

	/*
	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Hierarchy"));
	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("First sub-level"));
	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("Second sub-level"));
	pGroup4->AddSubItem(pGroup41);
	pGroup41->AddSubItem(pGroup411);
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 1"), (_variant_t) _T("Value 1"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 2"), (_variant_t) _T("Value 2"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 3"), (_variant_t) _T("Value 3"), _T("This is a description")));
	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
	*/
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}

void CPropertiesWnd::UpdateProperties(std::shared_ptr<CElement> pObj)
{
	USES_CONVERSION;

	CString data;

	CTime dt = CTime::GetCurrentTime();

	UpdateProperty(prop_Name, pObj->m_name.c_str());
	UpdateProperty(prop_ID, pObj->m_objectId.c_str());
	UpdateProperty(prop_Caption, pObj->m_caption.c_str());
	UpdateProperty(prop_Image, pObj->m_image.c_str());
	UpdateProperty(prop_Type, pObj->ToString(pObj->m_type));
	UpdateProperty(prop_Left, pObj->m_rect.left);
	UpdateProperty(prop_Top, pObj->m_rect.top);
	UpdateProperty(prop_Right, pObj->m_rect.right);
	UpdateProperty(prop_Bottom, pObj->m_rect.bottom);

	UpdateProperty(prop_Text, pObj->m_text.c_str());
	UpdateProperty(prop_Text_Align, pObj->m_textAlign.c_str());

	// Font Name and Size
	CString str;
	//str.Format("%s(%d)", pObj->m_fontName.c_str(), pObj->m_fontSize);
	str.Format(_T("%s"), pObj->m_fontName.c_str());
	UpdateProperty(prop_Font, str);
	UpdateProperty(prop_Font_Name, pObj->m_fontName.c_str());
	UpdateProperty(prop_Font_Size, (LONG)(pObj->m_fontSize));

	// COLORREF as LONG
	UpdateProperty(prop_Fill_Color, (LONG)(pObj->m_colorFill));
	UpdateProperty(prop_Line_Color, (LONG)(pObj->m_colorLine));
	// BOOL as SHORT
	COleVariant vColorFill((SHORT)(pObj->HasColorFill()), VT_BOOL);
	UpdateProperty(prop_Has_Fill_Color, vColorFill);
	COleVariant vSolidColorFill((SHORT)(pObj->IsSolidColorFill()), VT_BOOL);
	UpdateProperty(prop_Is_Fill_Solid_Color, vSolidColorFill);
	COleVariant vColorLine((SHORT)(pObj->HasColorLine()), VT_BOOL);
	UpdateProperty(prop_Has_Line_Color, vColorLine);
	COleVariant vFixed((SHORT)(pObj->IsFixed()), VT_BOOL);
	UpdateProperty(prop_Fixed, vFixed);

	UpdateProperty(prop_Document, pObj->m_document.c_str());
	UpdateProperty(prop_Document_Type, pObj->m_documentTypeText.c_str());
}

void CPropertiesWnd::UpdateProperty(std::wstring propertyName, COleVariant vNewValue)
{
	USES_CONVERSION;

	CString str;
	int count = m_wndPropList.GetPropertyCount();
	for(int i=0 ; i<count ; i++ )
	{
		CMFCPropertyGridProperty * prop = m_wndPropList.GetProperty(i);
		std::wstring strName = prop->GetName();
		str.Format(_T("Name[%d]=%s"), i, strName.c_str());

		int itemCount = prop->GetSubItemsCount();
		for(int si=0 ; si<itemCount  ; si++ )
		{
			CMFCPropertyGridProperty * subProp = prop->GetSubItem(si);
			std::wstring propName = subProp->GetName();
			if( propName == propertyName )
			{
				if( propName == prop_Fill_Color || propName == prop_Line_Color )
				{
					CMFCPropertyGridColorProperty * pColorProp = (CMFCPropertyGridColorProperty *)subProp;
					pColorProp->SetColor(vNewValue.lVal);
					subProp->OnEndEdit();
					prop->Redraw();
				}
				else if( propName == prop_Font )
				{
					/*
					CMFCPropertyGridFontProperty * pFontProp = (CMFCPropertyGridFontProperty*)subProp;
					pFontProp->SetValue(vNewValue);
					subProp->OnEndEdit();
					prop->Redraw();
					*/
					subProp->SetValue(vNewValue);
					subProp->OnEndEdit();
					prop->Redraw();
				}
				else if(propName == prop_Fixed || propName == prop_Has_Fill_Color || propName == prop_Has_Line_Color || propName == prop_Is_Fill_Solid_Color )
				{
					subProp->SetValue(vNewValue);
					subProp->OnEndEdit();
					prop->Redraw();
				}
				else if (propName == prop_Text_Align)
				{
					subProp->SetValue(vNewValue);
					subProp->OnEndEdit();
					prop->Redraw();
				}
				else
				{
					subProp->SetValue(vNewValue);
					subProp->OnEndEdit();
					prop->Redraw();
				}
				return;
			}
		}
	}
}

BOOL CPropertiesWnd::GetProperty(std::wstring propertyName, std::wstring & propertyValue)
{
	USES_CONVERSION;

	CString str;
	int count = m_wndPropList.GetPropertyCount();
	for(int i=0 ; i<count ; i++ )
	{
		CMFCPropertyGridProperty * prop = m_wndPropList.GetProperty(i);
		std::wstring strName = prop->GetName();
		str.Format(_T("Name[%d]=%s"), i, strName.c_str());

		int itemCount = prop->GetSubItemsCount();
		for(int si=0 ; si<itemCount  ; si++ )
		{
			CMFCPropertyGridProperty * subProp = prop->GetSubItem(si);
			std::wstring name = subProp->GetName();
			std::wstring value = (LPCTSTR) subProp->FormatProperty();
			if( name == propertyName )
			{
				propertyValue = value;
				return TRUE;
			}
		}
	}
	return FALSE;
}

LRESULT CPropertiesWnd::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	USES_CONVERSION;

	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;
	std::wstring propName = pProp->GetName();
	COleVariant propValue = pProp->GetValue();
	std::wstring propValueText = (LPCTSTR) pProp->FormatProperty();
	std::wstring strObjectId = L"";
	if( GetProperty(prop_ID, strObjectId) == FALSE )
		return 0;
		
	if( propName == prop_Fill_Color  || propName == prop_Line_Color )
	{
		CMFCPropertyGridColorProperty * pColorProp = (CMFCPropertyGridColorProperty *)pProp;
		COLORREF color = pColorProp->GetColor();
		GetManager()->UpdateFromPropertyGrid(strObjectId, propName, color);
	}
	else if (propName == prop_Text_Align)
	{
		if (propValueText == _T("Left") || propValueText == _T("Center") || propValueText == _T("Right"))
		{
			GetManager()->UpdateFromPropertyGrid(strObjectId, propName, propValueText);
		}
	}
	else if( propName == prop_Font )
	{
		/*
		CMFCPropertyGridFontProperty * pFontProp = (CMFCPropertyGridFontProperty *)pProp;
		LPLOGFONT lpFont = pFontProp->GetLogFont();
		std::wstring fontName = T2W(lpFont->lfFaceName);
		long fontSize = lpFont->lfHeight;
		fontSize = MulDiv(fontSize, -1, 1);
		GetManager()->UpdateFromPropertyGrid(strObjectId, prop_Font_Name, fontName);
		GetManager()->UpdateFromPropertyGrid(strObjectId, prop_Font_Size, (long)fontSize);
		*/
		GetManager()->UpdateFromPropertyGrid(strObjectId, propName, propValueText);
	}
	else if( propName == prop_Left || propName == prop_Top || propName == prop_Right || propName == prop_Bottom )
	{
		GetManager()->UpdateFromPropertyGrid(strObjectId, propName, propValue.lVal);
	}
	else if( propName == prop_Font_Size )
	{
		GetManager()->UpdateFromPropertyGrid(strObjectId, propName, propValue.lVal);
	}
	else if(propName == prop_Fixed || propName == prop_Has_Fill_Color || propName == prop_Has_Line_Color || propName == prop_Is_Fill_Solid_Color )
	{
		GetManager()->UpdateFromPropertyGrid(strObjectId, propName, (long) propValue.bVal);
	}
	else if (propName == prop_Document_Type)
	{
		if (propValueText == _T("None") || propValueText == _T("File") || propValueText == _T("Folder") || propValueText == _T("Diagram"))
		{
			GetManager()->UpdateFromPropertyGrid(strObjectId, propName, propValueText);
		}
	}
	else
	{
		GetManager()->UpdateFromPropertyGrid(strObjectId, propName, propValueText);
	}
	
	return 0;   
}
