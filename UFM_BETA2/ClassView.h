// Copyright (C) SAS NET Azure Rangers
// All rights reserved.

#pragma once

#include "Resource.h"
#include "Element.h"

/////////////////////////////////////////////////////////////////////////////
// CClassTreeWnd window

class CClassTreeWnd : public CTreeCtrl
{
// Construction
public:
	CClassTreeWnd();

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
public:
	virtual ~CClassTreeWnd();

protected:
	DECLARE_MESSAGE_MAP()
};

class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CClassViewBar : public CDockablePane
{
public:
	CClassViewBar();
	virtual ~CClassViewBar();

// Operations
public:
	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	CClassToolBar m_wndToolBar;
	CClassTreeWnd m_wndClassView;
	CImageList m_ClassViewImages;
	UINT m_nCurrSort;
	// Root element
	HTREEITEM m_hRoot;
	HTREEITEM m_hInfra;
	HTREEITEM m_hDev;
	HTREEITEM m_hGlobal;

// Operations
public:
	void InitClassView();
	void UpdateFromObject(std::shared_ptr<CElement> pElement);

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemeberFunction();
	afx_msg void OnClassAddMemeberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};
