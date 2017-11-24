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

// Modeler1View.cpp : implementation of the CModeler1View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Modeler1.h"
#endif

#include "MainFrm.h"
#include "Modeler1Doc.h"
#include "Modeler1View.h"
#include "Element.h"
#include "ElementManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModeler1View

IMPLEMENT_DYNCREATE(CModeler1View, CScrollView)

BEGIN_MESSAGE_MAP(CModeler1View, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CModeler1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FONT_FONT, &CModeler1View::OnFont)
	ON_UPDATE_COMMAND_UI(ID_FONT_FONT, &CModeler1View::OnUpdateFont)
	ON_COMMAND(ID_FONT_FONTSIZE, &CModeler1View::OnFontSize)
	ON_UPDATE_COMMAND_UI(ID_FONT_FONTSIZE, &CModeler1View::OnUpdateFontSize)
	ON_COMMAND(ID_EDIT_CUT, &CModeler1View::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CModeler1View::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CModeler1View::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CModeler1View::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CModeler1View::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CModeler1View::OnUpdateEditPaste)
	ON_COMMAND(ID_DESIGN_SELECT, &CModeler1View::OnModelingSelect)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_SELECT, &CModeler1View::OnUpdateModelingSelect)
	ON_COMMAND(ID_DESIGN_RECTANGLE, &CModeler1View::OnModelingRectangle)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_RECTANGLE, &CModeler1View::OnUpdateModelingRectangle)
	ON_COMMAND(ID_DESIGN_LINE, &CModeler1View::OnModelingLine)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_LINE, &CModeler1View::OnUpdateModelingLine)
	ON_COMMAND(ID_DESIGN_ELLIPSE, &CModeler1View::OnModelingEllipse)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_ELLIPSE, &CModeler1View::OnUpdateModelingEllipse)
	ON_COMMAND(ID_DESIGN_SHAPES, &CModeler1View::OnModelingShapes)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_SHAPES, &CModeler1View::OnUpdateModelingShapes)
	ON_COMMAND(ID_DEBUG_DUMP_OBJECTS, &CModeler1View::OnDebugDumpObjects)
	ON_COMMAND(ID_DESIGN_IMAGE, &CModeler1View::OnModelingImage)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_IMAGE, &CModeler1View::OnUpdateModelingImage)
	ON_COMMAND(ID_ACTION_REMOVE, OnActionRemove)
	ON_UPDATE_COMMAND_UI(ID_ACTION_REMOVE, OnUpdateActionRemove)
	ON_COMMAND(ID_ACTION_LOAD_MODULE, OnActionLoadModule)
	ON_UPDATE_COMMAND_UI(ID_ACTION_LOAD_MODULE, OnUpdateActionLoadModule)
	ON_COMMAND(ID_POSITION_MOVETOFRONT, OnPositionMoveToFront)
	ON_UPDATE_COMMAND_UI(ID_POSITION_MOVETOFRONT, OnUpdateMoveToFront)
	ON_COMMAND(ID_POSITION_MOVEFORWARD, OnPositionMoveForward)
	ON_UPDATE_COMMAND_UI(ID_POSITION_MOVEFORWARD, OnUpdatePositionMoveForward)
	ON_COMMAND(ID_POSITION_MOVEBACKWARD, OnPositionMoveBackward)
	ON_UPDATE_COMMAND_UI(ID_POSITION_MOVEBACKWARD, OnUpdatePositionMoveBackward)
	ON_COMMAND(ID_POSITION_MOVETOBACK, OnPositionMoveToBack)
	ON_UPDATE_COMMAND_UI(ID_POSITION_MOVETOBACK, OnUpdatePositionMoveToBack)
	ON_COMMAND(ID_FORMAT_FILLCOLOR, OnFormatFillColor)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_FILLCOLOR, OnUpdateFormatFillColor)
	ON_COMMAND(ID_FORMAT_NOFILL, OnFormatNoFillColor)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_NOFILL, OnUpdateFormatNoFillColor)
	ON_COMMAND(ID_FORMAT_LINECOLOR, OnFormatLineColor)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_LINECOLOR, OnUpdateFormatLineColor)
	ON_COMMAND_RANGE(ID_FORMAT_LINEWIDTH, ID_FORMAT_LINEWIDTH_MORE, OnFormatLineWidth)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FORMAT_LINEWIDTH, ID_FORMAT_LINEWIDTH_MORE, OnUpdateFormatLineWidth)
	ON_COMMAND(ID_FORMAT_PAGECOLOR, OnFormatPageColor)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_PAGECOLOR, OnUpdateFormatPageColor)
	ON_COMMAND(ID_DESIGN_SHAPESINFRA, &CModeler1View::OnModelingInfrastructure)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_SHAPESINFRA, &CModeler1View::OnUpdateModelingInfrastructure)
	ON_COMMAND(ID_DESIGN_TEXTBOX, &CModeler1View::OnModelingTextBox)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_TEXTBOX, &CModeler1View::OnUpdateModelingTextBox)
	ON_COMMAND_RANGE(ID_TEXT_LEFT, ID_TEXT_JUSTIFY, OnModelingTextBoxes)
	ON_COMMAND(ID_DESIGN_SHAPESDEV, &CModeler1View::OnModelingDevelopment)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_SHAPESDEV, &CModeler1View::OnUpdateModelingDevelopment)
	ON_COMMAND(ID_FORMAT_ZOOM, OnFormatZoom)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ZOOM, OnUpdateFormatZoom)
	ON_COMMAND(ID_FORMAT_ZOOM_IN, OnFormatZoomIn)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ZOOM_IN, OnUpdateFormatZoomIn)
	ON_COMMAND(ID_FORMAT_ZOOM_OUT, OnFormatZoomOut)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ZOOM_OUT, OnUpdateFormatZoomOut)
END_MESSAGE_MAP()

// CModeler1View construction/destruction

CModeler1View::CModeler1View()
{
	// TODO: add construction code here
	m_bActive = FALSE;
}

CModeler1View::~CModeler1View()
{
}

BOOL CModeler1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

BOOL CModeler1View::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	// do the scroll
	if (!CScrollView::OnScrollBy(sizeScroll, bDoScroll))
		return FALSE;

	// update the position of any in-place active item
	if (bDoScroll)
	{
		UpdateWindow();
	}
	
	Invalidate(FALSE);
	return TRUE;
}

// CModeler1View printing


void CModeler1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

void CModeler1View::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);
	GetManager()->PrepareDC(this, pDC, pInfo);
}

// CModeler1View drawing
//#define DRAW_STANDARD

void CModeler1View::OnDraw(CDC* pDC)
{
#ifdef DRAW_STANDARD
	CModeler1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	GetManager()->Draw(this, pDC);
#else
	CModeler1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	GetManager()->DrawEx(this, pDC);
#endif
}

BOOL CModeler1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CModeler1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CModeler1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CModeler1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CModeler1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CModeler1View diagnostics

#ifdef _DEBUG
void CModeler1View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CModeler1View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CModeler1Doc* CModeler1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModeler1Doc)));
	return (CModeler1Doc*)m_pDocument;
}
#endif //_DEBUG

void CModeler1View::LogDebug(CString strMessage)
{
	((CMainFrame *)AfxGetMainWnd())->LogDebug(strMessage);
}

void CModeler1View::LogDebugInView(CString strMessage)
{
#ifdef DEBUG_VIEW
	CClientDC dc(this);
	CString strEmpty(' ', 100);
	dc.DrawText(strMessage + strEmpty, CRect(10, 10, 1000, 400), DT_NOCLIP);
#endif
}

// CModeler1View message handlers

void CModeler1View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CSize size = GetManager()->GetSize();
	CClientDC dc(NULL);
	SetScrollSizes(MM_TEXT, size);

	CString str;
	str.Format(_T("Windows CSize x=%d y=%d"), size.cx, size.cy);
	LogDebug(str);

	GetManager()->UpdateClassView();
	GetManager()->UpdateFileView();
}

void CModeler1View::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	GetManager()->Update(this, lHint, pHint);
}

void CModeler1View::OnActivateView(BOOL bActivate, CView* pActiveView, CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActiveView, pDeactiveView);
	GetManager()->ActivateView(this, bActivate == TRUE ? true : false, pActiveView, pDeactiveView);
}

void CModeler1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	str.Format(_T("OnLButtonDown x=%d, y=%d"), point.x, point.y);
	//LogDebug(str);

	GetManager()->OnLButtonDown(this, nFlags, point);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CModeler1View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	str.Format(_T("OnMouseMove x=%d, y=%d"), point.x, point.y);
	//LogDebug(str);

	GetManager()->OnMouseMove(this, nFlags, point);

	CScrollView::OnMouseMove(nFlags, point);
}

void CModeler1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	str.Format(_T("OnLButtonUp x=%d, y=%d"), point.x, point.y);
	//LogDebug(str);

	GetManager()->OnLButtonUp(this, nFlags, point);

	CScrollView::OnLButtonUp(nFlags, point);
}

void CModeler1View::OnFont()
{
	// TODO: Add your command handler code here
	GetManager()->OnFont(this);
}

void CModeler1View::OnUpdateFont(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if( GetManager()->HasSelection() )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CModeler1View::OnFontSize()
{
	// TODO: Add your command handler code here
	GetManager()->OnFontSize(this);
}

void CModeler1View::OnUpdateFontSize(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if( GetManager()->HasSelection() )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CModeler1View::OnEditCut()
{
	// TODO: Add your command handler code here
	GetManager()->OnEditCut(this);
}

void CModeler1View::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if( GetManager()->HasSelection() )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CModeler1View::OnEditCopy()
{
	// TODO: Add your command handler code here
	GetManager()->OnEditCopy(this);
}

void CModeler1View::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if( GetManager()->HasSelection() )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CModeler1View::OnEditPaste()
{
	// TODO: Add your command handler code here
	GetManager()->OnEditPaste(this);
}

void CModeler1View::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if( GetManager()->m_clipboard.GetCount() > 0 )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}

}


void CModeler1View::OnModelingSelect()
{
	// TODO: Add your command handler code here
	GetManager()->m_type = ElementType::type_select;
}


void CModeler1View::OnUpdateModelingSelect(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(GetManager()->m_type == ElementType::type_select);
}

void CModeler1View::OnModelingRectangle()
{
	// TODO: Add your command handler code here
	GetManager()->m_type = ElementType::type_shapes_simple;
	GetManager()->m_shapeType = ShapeType::rectangle;
}

void CModeler1View::OnUpdateModelingRectangle(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio((GetManager()->m_type == ElementType::type_shapes_simple) && (GetManager()->m_shapeType == ShapeType::rectangle));
}

void CModeler1View::OnModelingLine()
{
	// TODO: Add your command handler code here
	GetManager()->m_type = ElementType::type_shapes_simple;
	GetManager()->m_shapeType = ShapeType::line;
}

void CModeler1View::OnUpdateModelingLine(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio((GetManager()->m_type == ElementType::type_shapes_simple) && (GetManager()->m_shapeType == ShapeType::line));
}

void CModeler1View::OnModelingEllipse()
{
	// TODO: Add your command handler code here
	GetManager()->m_type = ElementType::type_shapes_simple;
	GetManager()->m_shapeType = ShapeType::ellipse;
}

void CModeler1View::OnUpdateModelingEllipse(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio((GetManager()->m_type == ElementType::type_shapes_simple) && (GetManager()->m_shapeType == ShapeType::ellipse));
}

void CModeler1View::OnModelingImage()
{
	// TODO: Add your command handler code here
	GetManager()->m_type = ElementType::type_image;
	GetManager()->m_shapeType = ShapeType::image_fixed;
}

void CModeler1View::OnUpdateModelingImage(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(GetManager()->m_type == ElementType::type_image);
}

void CModeler1View::OnModelingShapes()
{
	// TODO: Add your command handler code here
	GetManager()->m_type = ElementType::type_shapes_simple;
	int shapeId = CMFCRibbonGallery::GetLastSelectedItem(ID_DESIGN_SHAPES);
	GetManager()->m_shapeType = CShapeType::ToShapeType(OffsetShapes_Simple + shapeId);
}

void CModeler1View::OnUpdateModelingShapes(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(GetManager()->m_type == ElementType::type_shapes_simple);
}

void CModeler1View::OnModelingInfrastructure()
{
	// TODO: Add your command handler code here
	GetManager()->m_type = ElementType::type_shapes_infrastructure;
	int shapeId = CMFCRibbonGallery::GetLastSelectedItem(ID_DESIGN_SHAPESINFRA);
	GetManager()->m_shapeType = CShapeType::ToShapeType(OffsetShapes_Infrastructure + shapeId);
	
	//CString str;
	//str.Format("shapeId=%d m_shapeType=%d", shapeId, GetManager()->m_shapeType);
	//AfxMessageBox(str);
}

void CModeler1View::OnUpdateModelingInfrastructure(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(GetManager()->m_type == ElementType::type_shapes_infrastructure);
}

void CModeler1View::OnModelingDevelopment()
{
	// TODO: Add your command handler code here
	GetManager()->m_type = ElementType::type_shapes_development;
	int shapeId = CMFCRibbonGallery::GetLastSelectedItem(ID_DESIGN_SHAPESDEV);
	GetManager()->m_shapeType = CShapeType::ToShapeType(OffsetShapes_Development + shapeId);
	
	//CString str;
	//str.Format("shapeId=%d m_shapeType=%d", shapeId, GetManager()->m_shapeType);
	//AfxMessageBox(str);
}

void CModeler1View::OnUpdateModelingDevelopment(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(GetManager()->m_type == ElementType::type_shapes_development);
}

void CModeler1View::OnDebugDumpObjects()
{
	// TODO: Add your command handler code here
	GetManager()->DebugDumpObjects(this);
}

void CModeler1View::OnActionRemove()
{
	GetManager()->RemoveSelectedObjects(this);
}

void CModeler1View::OnUpdateActionRemove(CCmdUI* pCmdUI)
{
	BOOL bSelection = GetManager()->HasSelection();
	pCmdUI->Enable(bSelection == TRUE);
}

void CModeler1View::OnActionLoadModule()
{
	GetManager()->LoadModule(this);
}

void CModeler1View::OnUpdateActionLoadModule(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CModeler1View::OnPositionMoveToFront()
{
	GetManager()->MoveToFront(this);
}

void CModeler1View::OnUpdateMoveToFront(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection());
}

void CModeler1View::OnPositionMoveForward()
{
	GetManager()->MoveForward(this);
}

void CModeler1View::OnUpdatePositionMoveForward(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection());
}

void CModeler1View::OnPositionMoveBackward()
{
	GetManager()->MoveBackward(this);
}

void CModeler1View::OnUpdatePositionMoveBackward(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection());
}

void CModeler1View::OnPositionMoveToBack()
{
	GetManager()->MoveToBack(this);
}

void CModeler1View::OnUpdatePositionMoveToBack(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection());
}

void CModeler1View::OnFormatFillColor()
{
	GetManager()->FillColor(this);
}

void CModeler1View::OnUpdateFormatFillColor(CCmdUI* pCmdUI)
{
	if( GetManager()->HasSelection() )
	{
		pCmdUI->Enable(GetManager()->m_selection.GetHead()->CanChangeFillColor());
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CModeler1View::OnFormatNoFillColor()
{
	GetManager()->NoFillColor(this);
}

void CModeler1View::OnUpdateFormatNoFillColor(CCmdUI* pCmdUI)
{
	if( GetManager()->HasSelection() )
	{
		pCmdUI->Enable(GetManager()->m_selection.GetHead()->CanChangeFillColor());
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CModeler1View::OnFormatLineColor()
{
	GetManager()->LineColor(this);
}

void CModeler1View::OnUpdateFormatLineColor(CCmdUI* pCmdUI)
{
	if( GetManager()->HasSelection() )
	{
		pCmdUI->Enable(GetManager()->m_selection.GetHead()->CanChangeLineColor());
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CModeler1View::OnFormatLineWidth(UINT nID)
{
	GetManager()->LineWidth(this, nID);
}

void CModeler1View::OnUpdateFormatLineWidth(CCmdUI* pCmdUI)
{
	if( GetManager()->HasSelection() )
	{
		pCmdUI->Enable(GetManager()->m_selection.GetHead()->CanChangeLineWidth());
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CModeler1View::OnFormatPageColor()
{
	GetManager()->PageColor(this);
}

void CModeler1View::OnUpdateFormatPageColor(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CModeler1View::OnFormatZoom()
{
	GetManager()->Zoom(this);
}

void CModeler1View::OnUpdateFormatZoom(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CModeler1View::OnFormatZoomIn()
{
	GetManager()->ZoomIn(this);
}

void CModeler1View::OnUpdateFormatZoomIn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CModeler1View::OnFormatZoomOut()
{
	GetManager()->ZoomOut(this);
}

void CModeler1View::OnUpdateFormatZoomOut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CModeler1View::OnModelingTextBox()
{
	// TODO: Add your command handler code here
	GetManager()->m_type = ElementType::type_text;
	GetManager()->m_shapeType = ShapeType::text;
	AfxMessageBox(_T("OnModelingTextBox"));
}

void CModeler1View::OnUpdateModelingTextBox(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(GetManager()->m_type == ElementType::type_text);
}

void CModeler1View::OnModelingTextBoxes(UINT nID)
{
	GetManager()->m_type = ElementType::type_text;
	
	switch(nID)
	{
		case ID_TEXT_LEFT:
			GetManager()->m_shapeType = ShapeType::text_left;
			break;
		case ID_TEXT_CENTER:
			GetManager()->m_shapeType = ShapeType::text_center;
			break;
		case ID_TEXT_RIGHT:
			GetManager()->m_shapeType = ShapeType::text_right;
			break;
		case ID_TEXT_JUSTIFY:
			GetManager()->m_shapeType = ShapeType::text_justify;
			break;
	}
}
