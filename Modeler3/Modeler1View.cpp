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
	ON_COMMAND(ID_FILE_OPEN_GABARIT, OnFileOpenGabarit)
	ON_COMMAND(ID_DESIGN_TASK, &CModeler1View::OnModelingTask)
	ON_COMMAND(ID_DESIGN_MONTH, &CModeler1View::OnModelingMonth)
	ON_COMMAND(ID_FORMAT_ALIGN_LEFT, &CModeler1View::OnFormatAlignLeft)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_LEFT, OnUpdateFormatAlignLeft)
	ON_COMMAND(ID_FORMAT_ALIGN_RIGHT, &CModeler1View::OnFormatAlignRight)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_RIGHT, OnUpdateFormatAlignRight)
	ON_COMMAND(ID_FORMAT_ALIGN_TOP, &CModeler1View::OnFormatAlignTop)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_TOP, OnUpdateFormatAlignTop)
	ON_COMMAND(ID_FORMAT_ALIGN_BOTTOM, &CModeler1View::OnFormatAlignBottom)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_BOTTOM, OnUpdateFormatAlignBottom)
	ON_COMMAND(ID_FORMAT_TEXT_ALIGN_LEFT, &CModeler1View::OnFormatTextAlignLeft)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_TEXT_ALIGN_LEFT, OnUpdateFormatTextAlignLeft)
	ON_COMMAND(ID_FORMAT_TEXT_ALIGN_CENTER, &CModeler1View::OnFormatTextAlignCenter)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_TEXT_ALIGN_CENTER, OnUpdateFormatTextAlignCenter)
	ON_COMMAND(ID_FORMAT_TEXT_ALIGN_RIGHT, &CModeler1View::OnFormatTextAlignRight)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_TEXT_ALIGN_RIGHT, OnUpdateFormatTextAlignRight)
	ON_COMMAND(ID_EDIT_GROUP, &CModeler1View::OnEditGroup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GROUP, &CModeler1View::OnUpdateEditGroup)
	ON_COMMAND(ID_EDIT_UNGROUP, &CModeler1View::OnEditUngroup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNGROUP, &CModeler1View::OnUpdateEditUngroup)
	ON_COMMAND(ID_DESIGN_SELECTION, &CModeler1View::OnDesignSelection)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_SELECTION, &CModeler1View::OnUpdateDesignSelection)
	ON_COMMAND(ID_DESIGN_CLASS, &CModeler1View::OnDesignClass)
	ON_COMMAND(ID_DESIGN_INTERFACE, &CModeler1View::OnDesignInterface)
	ON_COMMAND(ID_DESIGN_ENUM, &CModeler1View::OnDesignEnum)
	ON_COMMAND(ID_DESIGN_PACKAGE, &CModeler1View::OnDesignPackage)
	ON_COMMAND(ID_DESIGN_COMMENT, &CModeler1View::OnDesignComment)
	ON_COMMAND(ID_DESIGN_COMPONENT, &CModeler1View::OnDesignComponent)
	ON_COMMAND(ID_FILE_EXPORT_PNG, &CModeler1View::OnFileExportPNG)
	ON_COMMAND(ID_DESIGN_SELECT_ALL, &CModeler1View::OnDesignSelectAll)
	ON_COMMAND(ID_CLIPBOARD_CUT, &CModeler1View::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_CLIPBOARD_CUT, &CModeler1View::OnUpdateEditCut)
	ON_COMMAND(ID_CLIPBOARD_COPY, &CModeler1View::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_CLIPBOARD_COPY, &CModeler1View::OnUpdateEditCopy)
	ON_COMMAND(ID_CLIPBOARD_PASTE, &CModeler1View::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_CLIPBOARD_PASTE, &CModeler1View::OnUpdateEditPaste)
	ON_COMMAND(ID_FONT_GROWFONT, &CModeler1View::OnFontGrowFont)
	ON_UPDATE_COMMAND_UI(ID_FONT_GROWFONT, &CModeler1View::OnUpdateFontGrowFont)
	ON_COMMAND(ID_FONT_SHRINK, &CModeler1View::OnFontShrink)
	ON_UPDATE_COMMAND_UI(ID_FONT_SHRINK, &CModeler1View::OnUpdateFontShrink)
	ON_COMMAND(ID_FONT_CLEARFORMAT, &CModeler1View::OnFontClearFormat)
	ON_UPDATE_COMMAND_UI(ID_FONT_CLEARFORMAT, &CModeler1View::OnUpdateFontClearFormat)
	ON_COMMAND(ID_FONT_BOLD, &CModeler1View::OnFontBold)
	ON_UPDATE_COMMAND_UI(ID_FONT_BOLD, &CModeler1View::OnUpdateFontBold)
	ON_COMMAND(ID_FONT_ITALIC, &CModeler1View::OnFontItalic)
	ON_UPDATE_COMMAND_UI(ID_FONT_ITALIC, &CModeler1View::OnUpdateFontItalic)
	ON_COMMAND(ID_FONT_UNDERLINE, &CModeler1View::OnFontUnderline)
	ON_UPDATE_COMMAND_UI(ID_FONT_UNDERLINE, &CModeler1View::OnUpdateFontUnderline)
	ON_COMMAND(ID_FONT_STRIKETHROUGH, &CModeler1View::OnFontStrikeThrough)
	ON_UPDATE_COMMAND_UI(ID_FONT_STRIKETHROUGH, &CModeler1View::OnUpdateFontStrikeThrough)
	ON_COMMAND(ID_FONT_SUBSCRIPT, &CModeler1View::OnFontSubscript)
	ON_UPDATE_COMMAND_UI(ID_FONT_SUBSCRIPT, &CModeler1View::OnUpdateFontSubscript)
	ON_COMMAND(ID_FONT_SUPERSCRIPT, &CModeler1View::OnFontSuperscript)
	ON_UPDATE_COMMAND_UI(ID_FONT_SUPERSCRIPT, &CModeler1View::OnUpdateFontSuperscript)
	ON_COMMAND(ID_FONT_CHANGECASE, &CModeler1View::OnFontChangeCase)
	ON_UPDATE_COMMAND_UI(ID_FONT_CHANGECASE, &CModeler1View::OnUpdateFontChangeCase)
	ON_COMMAND(ID_FONT_TEXTHIGHLIGHT, &CModeler1View::OnFontTextHighlight)
	ON_UPDATE_COMMAND_UI(ID_FONT_TEXTHIGHLIGHT, &CModeler1View::OnUpdateFontTextHighlight)
	ON_COMMAND(ID_FONT_COLOR, &CModeler1View::OnFontColor)
	ON_UPDATE_COMMAND_UI(ID_FONT_COLOR, &CModeler1View::OnUpdateFontColor)
	ON_COMMAND(ID_DESIGN_TEXT, &CModeler1View::OnDesignText)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_TEXT, &CModeler1View::OnUpdateDesignText)
	ON_COMMAND(ID_DESIGN_CONNECT, &CModeler1View::OnDesignConnect)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_CONNECT, &CModeler1View::OnUpdateDesignConnect)
	ON_COMMAND(ID_SELECT_ALL, &CModeler1View::OnSelectAll)
	ON_UPDATE_COMMAND_UI(ID_SELECT_ALL, &CModeler1View::OnUpdateSelectAll)
	ON_COMMAND(ID_SELECT_ONLY_LINES, &CModeler1View::OnSelectOnlyLines)
	ON_UPDATE_COMMAND_UI(ID_SELECT_ONLY_LINES, &CModeler1View::OnUpdateSelectOnlyLines)
	ON_COMMAND(ID_SELECT_ONLY_ITEMS, &CModeler1View::OnSelectOnlyItems)
	ON_UPDATE_COMMAND_UI(ID_SELECT_ONLY_ITEMS, &CModeler1View::OnUpdateSelectOnlyItems)
	ON_COMMAND(ID_SELECT_INTUITIVE, &CModeler1View::OnSelectIntuitive)
	ON_UPDATE_COMMAND_UI(ID_SELECT_INTUITIVE, &CModeler1View::OnUpdateSelectIntuitive)
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
	//AfxMessageBox(_T("OnModelingTextBox"));
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

void CModeler1View::OnFileOpenGabarit()
{
	// TODO: Add your command handler code here
	GetManager()->OnFileOpenGabarit(this);
}

void CModeler1View::OnModelingTask()
{
	GetManager()->m_type = ElementType::type_shapes_planning;
	GetManager()->m_shapeType = CShapeType::ToShapeType(planning_task);
}

void CModeler1View::OnModelingMonth()
{
	GetManager()->m_type = ElementType::type_shapes_planning;
	GetManager()->m_shapeType = CShapeType::ToShapeType(planning_month);
}

void CModeler1View::OnFormatAlignLeft()
{
	GetManager()->AlignLeft(this);
}

void CModeler1View::OnUpdateFormatAlignLeft(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection());
}

void CModeler1View::OnFormatAlignRight()
{
	GetManager()->AlignRight(this);
}

void CModeler1View::OnUpdateFormatAlignRight(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection());
}

void CModeler1View::OnFormatAlignTop()
{
	GetManager()->AlignTop(this);
}

void CModeler1View::OnUpdateFormatAlignTop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection());
}

void CModeler1View::OnFormatAlignBottom()
{
	GetManager()->AlignBottom(this);
}

void CModeler1View::OnUpdateFormatAlignBottom(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection());
}

void CModeler1View::OnFormatTextAlignLeft()
{
	GetManager()->AlignTextLeft(this);
}

void CModeler1View::OnUpdateFormatTextAlignLeft(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection());
}

void CModeler1View::OnFormatTextAlignCenter()
{
	GetManager()->AlignTextCenter(this);
}

void CModeler1View::OnUpdateFormatTextAlignCenter(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection());
}

void CModeler1View::OnFormatTextAlignRight()
{
	GetManager()->AlignTextRight(this);
}

void CModeler1View::OnUpdateFormatTextAlignRight(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection());
}

void CModeler1View::OnEditGroup()
{
	// TODO: Add your command handler code here
	GetManager()->OnEditGroup(this);
}

void CModeler1View::OnUpdateEditGroup(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (GetManager()->m_selection.GetCount() >= 2)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CModeler1View::OnEditUngroup()
{
	// TODO: Add your command handler code here
	GetManager()->OnEditUngroup(this);
}

void CModeler1View::OnUpdateEditUngroup(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (GetManager()->HasSelection())
	{
		pCmdUI->Enable(GetManager()->m_selection.GetHead()->m_bGrouping == true);
	}
}

void CModeler1View::OnDesignSelection()
{
	// TODO: Add your command handler code here
	GetManager()->m_type = ElementType::type_selection;
	GetManager()->m_shapeType = ShapeType::selection;
}

void CModeler1View::OnUpdateDesignSelection(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(GetManager()->m_type == ElementType::type_selection);
}

void CModeler1View::OnDesignClass()
{
	GetManager()->m_type = ElementType::type_shapes_development;
	GetManager()->m_shapeType = CShapeType::ToShapeType(development_class);
}

void CModeler1View::OnDesignInterface()
{
	GetManager()->m_type = ElementType::type_shapes_development;
	GetManager()->m_shapeType = CShapeType::ToShapeType(development_interface);
}

void CModeler1View::OnDesignEnum()
{
	GetManager()->m_type = ElementType::type_shapes_development;
	GetManager()->m_shapeType = CShapeType::ToShapeType(development_enumeration);
}

void CModeler1View::OnDesignPackage()
{
	GetManager()->m_type = ElementType::type_shapes_development;
	GetManager()->m_shapeType = CShapeType::ToShapeType(development_package);
}

void CModeler1View::OnDesignComment()
{
	GetManager()->m_type = ElementType::type_shapes_development;
	GetManager()->m_shapeType = CShapeType::ToShapeType(development_comment);
}

void CModeler1View::OnDesignComponent()
{
	GetManager()->m_type = ElementType::type_shapes_development;
	GetManager()->m_shapeType = CShapeType::ToShapeType(development_component);
}

void CModeler1View::OnFileExportPNG()
{
	GetManager()->OnFileExportPNG(this);
}

void CModeler1View::OnDesignSelectAll()
{
	GetManager()->OnSelectAll(this);
}

void CModeler1View::OnFontGrowFont()
{
	GetManager()->OnFontGrowFont(this);
}

void CModeler1View::OnFontShrink()
{
	GetManager()->OnFontShrink(this);
}

void CModeler1View::OnFontClearFormat()
{
	GetManager()->OnFontClearFormat(this);
}

void CModeler1View::OnFontBold()
{
	GetManager()->OnFontBold(this);
}

void CModeler1View::OnFontItalic()
{
	GetManager()->OnFontItalic(this);
}

void CModeler1View::OnFontUnderline()
{
	GetManager()->OnFontUnderline(this);
}

void CModeler1View::OnFontStrikeThrough()
{
	GetManager()->OnFontStrikeThrough(this);
}

void CModeler1View::OnFontSubscript()
{

}

void CModeler1View::OnFontSuperscript()
{

}

void CModeler1View::OnFontChangeCase()
{
	GetManager()->OnFontChangeCase(this);
}

void CModeler1View::OnFontTextHighlight()
{
	GetManager()->OnFontTextHighlight(this);
}

void CModeler1View::OnFontColor()
{
	GetManager()->OnFontColor(this);
}

void CModeler1View::OnUpdateFontGrowFont(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection() == true);
}

void CModeler1View::OnUpdateFontShrink(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection() == true);
}

void CModeler1View::OnUpdateFontClearFormat(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection() == true);
}

void CModeler1View::OnUpdateFontBold(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection() == true);
}

void CModeler1View::OnUpdateFontItalic(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection() == true);
}

void CModeler1View::OnUpdateFontUnderline(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection() == true);
}

void CModeler1View::OnUpdateFontStrikeThrough(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection() == true);
}

void CModeler1View::OnUpdateFontSubscript(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(false);
}

void CModeler1View::OnUpdateFontSuperscript(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(false);
}

void CModeler1View::OnUpdateFontChangeCase(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection() == true);
}

void CModeler1View::OnUpdateFontTextHighlight(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection() == true);
}

void CModeler1View::OnUpdateFontColor(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetManager()->HasSelection() == true);
}

void CModeler1View::OnDesignText()
{
	GetManager()->m_type = ElementType::type_text;
	GetManager()->m_shapeType = ShapeType::text;
}

void CModeler1View::OnUpdateDesignText(CCmdUI* pCmdUI)
{
}

void CModeler1View::OnDesignConnect()
{
	GetManager()->m_type = ElementType::type_shapes_simple;
	GetManager()->m_shapeType = ShapeType::line_right;
}

void CModeler1View::OnUpdateDesignConnect(CCmdUI* pCmdUI)
{
}

void CModeler1View::OnSelectAll()
{
	GetManager()->m_selectType = SelectType::all;
	GetManager()->m_type = ElementType::type_selection;
	GetManager()->m_shapeType = ShapeType::selection;
}

void CModeler1View::OnUpdateSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(GetManager()->m_selectType == SelectType::all);
}

void CModeler1View::OnSelectOnlyLines()
{
	GetManager()->m_selectType = SelectType::only_lines;
	GetManager()->m_type = ElementType::type_selection;
	GetManager()->m_shapeType = ShapeType::selection;
}

void CModeler1View::OnUpdateSelectOnlyLines(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(GetManager()->m_selectType == SelectType::only_lines);
}

void CModeler1View::OnSelectOnlyItems()
{
	GetManager()->m_selectType = SelectType::only_items;
	GetManager()->m_type = ElementType::type_selection;
	GetManager()->m_shapeType = ShapeType::selection;
}

void CModeler1View::OnUpdateSelectOnlyItems(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(GetManager()->m_selectType == SelectType::only_items);
}

void CModeler1View::OnSelectIntuitive()
{
	GetManager()->m_selectType = SelectType::intuitive;
	GetManager()->m_type = ElementType::type_selection;
	GetManager()->m_shapeType = ShapeType::selection;
}

void CModeler1View::OnUpdateSelectIntuitive(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(GetManager()->m_selectType == SelectType::intuitive);
}

