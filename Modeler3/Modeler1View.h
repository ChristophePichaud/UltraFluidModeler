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

// Modeler1View.h : interface of the CModeler1View class
//

#pragma once
#include "Modeler1Doc.h"

class CModeler1View : public CScrollView
{
protected: // create from serialization only
	CModeler1View();
	DECLARE_DYNCREATE(CModeler1View)

// Attributes
public:
	CElementManager * GetManager() const
	{
		return GetDocument()->GetManager();
	}

	CModeler1Doc* GetDocument() const;

	bool m_bActive; // is the view active?

// Operations
public:
	void LogDebug(CString message);
	void LogDebugInView(CString strMessage);

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll);
	virtual void OnActivateView(BOOL bActivate, CView* pActiveView, CView* pDeactiveView);
	
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	
// Implementation
public:
	virtual ~CModeler1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:	
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFont();
	afx_msg void OnUpdateFont(CCmdUI *pCmdUI);
	afx_msg void OnFontSize();
	afx_msg void OnUpdateFontSize(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnModelingSelect();
	afx_msg void OnUpdateModelingSelect(CCmdUI *pCmdUI);
	afx_msg void OnModelingRectangle();
	afx_msg void OnUpdateModelingRectangle(CCmdUI *pCmdUI);
	afx_msg void OnModelingLine();
	afx_msg void OnUpdateModelingLine(CCmdUI *pCmdUI);
	afx_msg void OnModelingEllipse();
	afx_msg void OnUpdateModelingEllipse(CCmdUI *pCmdUI);
	afx_msg void OnModelingImage();
	afx_msg void OnUpdateModelingImage(CCmdUI *pCmdUI);
	afx_msg void OnModelingShapes();
	afx_msg void OnUpdateModelingShapes(CCmdUI *pCmdUI);
	afx_msg void OnDebugDumpObjects();
	afx_msg void OnActionRemove();
	afx_msg void OnUpdateActionRemove(CCmdUI* pCmdUI);
	afx_msg void OnActionLoadModule();
	afx_msg void OnUpdateActionLoadModule(CCmdUI* pCmdUI);
	afx_msg void OnPositionMoveToFront();
	afx_msg void OnUpdateMoveToFront(CCmdUI* pCmdUI);
	afx_msg void OnPositionMoveForward();
	afx_msg void OnUpdatePositionMoveForward(CCmdUI* pCmdUI);
	afx_msg void OnPositionMoveBackward();
	afx_msg void OnUpdatePositionMoveBackward(CCmdUI* pCmdUI);
	afx_msg void OnPositionMoveToBack();
	afx_msg void OnUpdatePositionMoveToBack(CCmdUI* pCmdUI);
	afx_msg void OnFormatFillColor();
	afx_msg void OnUpdateFormatFillColor(CCmdUI* pCmdUI);
	afx_msg void OnFormatNoFillColor();
	afx_msg void OnUpdateFormatNoFillColor(CCmdUI* pCmdUI);
	afx_msg void OnFormatLineColor();
	afx_msg void OnUpdateFormatLineColor(CCmdUI* pCmdUI);
	afx_msg void OnFormatLineWidth(UINT nID);
	afx_msg void OnUpdateFormatLineWidth(CCmdUI* pCmdUI);
	afx_msg void OnFormatPageColor();
	afx_msg void OnUpdateFormatPageColor(CCmdUI* pCmdUI);
	afx_msg void OnModelingInfrastructure();
	afx_msg void OnUpdateModelingInfrastructure(CCmdUI *pCmdUI);
	afx_msg void OnModelingTextBox();
	afx_msg void OnUpdateModelingTextBox(CCmdUI *pCmdUI);
	afx_msg void OnModelingTextBoxes(UINT nID);
	afx_msg void OnModelingDevelopment();
	afx_msg void OnUpdateModelingDevelopment(CCmdUI *pCmdUI);
	afx_msg void OnFormatZoom();
	afx_msg void OnUpdateFormatZoom(CCmdUI* pCmdUI);
	afx_msg void OnFormatZoomIn();
	afx_msg void OnUpdateFormatZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnFormatZoomOut();
	afx_msg void OnUpdateFormatZoomOut(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // debug version in Modeler1View.cpp
inline CModeler1Doc* CModeler1View::GetDocument() const
   { return reinterpret_cast<CModeler1Doc*>(m_pDocument); }
#endif

