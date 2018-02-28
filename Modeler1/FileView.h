#pragma once

#include "ClassView.h"

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileViewBar : public CDockablePane
{
// Construction
public:
	CFileViewBar();

	void AdjustLayout();
	void OnChangeVisualStyle();

// Attributes
protected:
	CClassTreeWnd m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;
	// Root element
	HTREEITEM m_hRoot;
	HTREEITEM m_hSrc;
	HTREEITEM m_hInc;
	HTREEITEM m_hRes;
	HTREEITEM m_hOthers;

// Operations
public:
	void InitFileView();
	void UpdateFromObject(std::shared_ptr<CElement> pElement);

// Implementation
public:
	virtual ~CFileViewBar();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnSolutionOpen();
	afx_msg void OnSolutionOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};

