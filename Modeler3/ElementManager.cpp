#include "StdAfx.h"
#include "Modeler1.h"
#include "ElementManager.h"
#include "Modeler1View.h"
#include "MainFrm.h"
#include "DrawingContext.h"
#include "DrawingElements.h"
//
// CElementManager
//

IMPLEMENT_SERIAL(CElementManager, CObject, VERSIONABLE_SCHEMA | 4)

CElementManager::CElementManager()
{
	m_objectId = L"";
	m_lastPoint = CPoint(0, 0);
	m_paperColor = RGB(255, 255, 255); //RGB(242, 242, 200); //RGB(255, 255, 255); //RGB(188, 251, 255);
	m_size = CSize(1500, 1500);

	// Initialize Current UI interaction members
	m_bDrawing = FALSE;
	// Current selected drawing tool = SELECT
	m_type = ElementType::type_select;
	// Current selected shape type from Ribbon = type_unknown
	m_shapeType = ShapeType::unknown;
	m_nDragHandle = 1;
	m_fZoomFactor = 1.0f;
	
	m_bSavingCode = false;
	
	// Initiate the connection with the Property Window
	ConnectToPropertyGrid();
}

void CElementManager::ConnectToPropertyGrid()
{
	CWnd * p = AfxGetMainWnd();
	CMainFrame * pmf = (CMainFrame *)p;
	pmf->SetManager(this);
}

CElementManager::~CElementManager(void)
{
}

void CElementManager::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_size;
		ar << m_paperColor;
		ar << m_lastPoint;
	}
	else
	{
		ar >> m_size;
		ar >> m_paperColor;
		ar >> m_lastPoint;
	}

	m_objects.Serialize(this, ar);
}

void CElementManager::RemoveSelectedObjects(CModeler1View * pView)
{
	m_objects.Remove(m_selection);
	SelectNone();

	// Update ClassView & FileView
	UpdateClassView();//pNewElement);
	UpdateFileView();//pNewElement);

	Invalidate(pView);
}

void CElementManager::OnFont(CModeler1View * pView)
{
	USES_CONVERSION;

	CMFCRibbonBar* pRibbon = ((CMainFrame*) pView->GetTopLevelFrame())->GetRibbonBar();

	CMFCRibbonFontComboBox* pFontCombo = DYNAMIC_DOWNCAST(CMFCRibbonFontComboBox, pRibbon->FindByID(ID_FONT_FONT));
	if (pFontCombo == NULL)
	{
		return;
	}

	CString fontName = pFontCombo->GetEditText();
	if (pFontCombo->FindItem(fontName) == -1)
	{
		// Restore current name:
		pFontCombo->SelectItem(10);
		CString strWarning;
		strWarning.Format(_T("The font %s does not exits on your system"), fontName);
		AfxMessageBox(strWarning, MB_OK | MB_ICONWARNING);
		return;
	}

	const CMFCFontInfo* pDesc = pFontCombo->GetFontDesc();

	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	pElement->m_fontName = T2W((LPTSTR)(LPCTSTR)fontName);

	// Redraw the element
	InvalObj(pView, pElement);
}

void CElementManager::OnFontSize(CModeler1View * pView)
{
	CMFCRibbonBar* pRibbon = ((CMainFrame*) pView->GetTopLevelFrame())->GetRibbonBar();

	CMFCRibbonComboBox* pFontCombo = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pRibbon->FindByID(ID_FONT_FONTSIZE));
	if (pFontCombo == NULL)
	{
		return;
	}

	CString fontSize = pFontCombo->GetEditText();
	int iFontSize = _ttoi(fontSize);

	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	pElement->m_fontSize = iFontSize;

	// Redraw the element
	InvalObj(pView, pElement);
}

void CElementManager::OnEditCut(CModeler1View * pView)
{
	// the clipboard is cleared
	m_clipboard.RemoveAll();
	// the current selection is cleared
	RemoveSelectedObjects(pView);
}

void CElementManager::OnEditCopy(CModeler1View * pView)
{
	m_clipboard.RemoveAll();
	//m_selection.ChangeInnerAttributes();
	m_clipboard.Clone(m_selection);
	//m_clipboard.ChangeInnerAttributes();
}

void CElementManager::OnEditPaste(CModeler1View * pView)
{
	m_objects.Clone(m_clipboard);
	for( vector<std::shared_ptr<CElement>>::const_iterator itSel = m_clipboard.m_objects.begin() ; itSel!=m_clipboard.m_objects.end() ; itSel++ )
	{
		std::shared_ptr<CElement> pElement = *itSel;
		Select(pElement);
	}
	m_selectMode = SelectMode::move;

	m_clipboard.RemoveAll();

	Invalidate(pView);
}

void CElementManager::MoveToFront(CModeler1View * pView)
{
	m_objects.MoveToFront(m_selection);
	Invalidate(pView);
}

void CElementManager::MoveForward(CModeler1View * pView)
{
	m_objects.MoveForward(m_selection);
	Invalidate(pView);
}

void CElementManager::MoveBackward(CModeler1View * pView)
{
	m_objects.MoveBackward(m_selection);
	Invalidate(pView);
}

void CElementManager::MoveToBack(CModeler1View * pView)
{
	m_objects.MoveToBack(m_selection);
	Invalidate(pView);
}

bool CElementManager::HasSelection()
{
	if( m_selection.GetCount() > 0 )
		return true;
	else
		return false;
}

bool CElementManager::IsSelected(std::shared_ptr<CElement> pElement)
{
	std::shared_ptr<CElement> ptr = m_selection.FindElement(pElement->m_objectId);
	if (ptr != nullptr)
		return true;
	else
		return false;
}

void CElementManager::SelectNone()
{
	m_selection.RemoveAll();
}

bool CElementManager::Select(std::shared_ptr<CElement> pElement)
{
	m_selection.AddTail(pElement);
	return true;
}

bool CElementManager::Deselect(std::shared_ptr<CElement> pElement)
{
	m_selection.Remove(pElement);
	return true;
}

void CElementManager::ViewToManager(CModeler1View * pView, CPoint & point)
{
	// CScrollView changes the viewport origin and mapping mode.
	// It's necessary to convert the point from device coordinates
	// to logical coordinates, such as are stored in the document.
	//CClientDC dc(pView);
	//pView->OnPrepareDC(&dc, NULL);
	//dc.DPtoLP(&point);

	CString str;
	CPoint scrollPoint = pView->GetScrollPosition();
	str.Format(_T("scroll point {%d,%d}"), scrollPoint.x, scrollPoint.y);
	//pView->LogDebug(str);

	point.operator+=(scrollPoint);

	CClientDC dc(pView);
	Graphics graphics(dc.m_hDC);
	graphics.ScaleTransform(m_fZoomFactor, m_fZoomFactor);
	Point points[1] = {Point(point.x, point.y)};
	// Transform the points in the array from world to page coordinates.
	graphics.TransformPoints(
		CoordinateSpaceWorld, 
		CoordinateSpaceDevice, 
		points, 
		1);
	point.x = points[0].X;
	point.y = points[0].Y;
}

void CElementManager::ViewToManager(CModeler1View * pView, CRect & rect)
{
	// CScrollView changes the viewport origin and mapping mode.
	// It's necessary to convert the point from device coordinates
	// to logical coordinates, such as are stored in the document.
	//CClientDC dc(pView);
	//pView->OnPrepareDC(&dc, NULL);
	//dc.DPtoLP(&rect);

	CClientDC dc(pView);
	Graphics graphics(dc.m_hDC);
	graphics.ScaleTransform(m_fZoomFactor, m_fZoomFactor);
	CPoint point1 = rect.TopLeft();
	CPoint point2 = rect.BottomRight();
	Point points[2] = { Point((int) point1.x, (int) point1.y), 
						Point((int) point2.x, (int) point2.y) };
	// Transform the points in the array from world to page coordinates.
	graphics.TransformPoints(
		CoordinateSpaceWorld, 
		CoordinateSpaceDevice, 
		points, 
		2);

	point1.x = points[0].X;
	point1.y = points[0].Y;
	point2.x = points[1].X;
	point2.y = points[1].Y;
	rect.SetRect(point1, point2);
}

void CElementManager::ManagerToView(CModeler1View * pView, CPoint & point)
{
	//CClientDC dc(pView);
	//pView->OnPrepareDC(&dc, NULL);
	//dc.LPtoDP(&point);

	CClientDC dc(pView);
	Graphics graphics(dc.m_hDC);
	graphics.ScaleTransform(m_fZoomFactor, m_fZoomFactor);
	Point points[1] = {Point(point.x, point.y)};
	// Transform the points in the array from world to page coordinates.
	graphics.TransformPoints(
		CoordinateSpaceDevice, 
		CoordinateSpaceWorld, 
		points, 
		1);
	point.x = points[0].X;
	point.y = points[0].Y;
}

void CElementManager::ManagerToView(CModeler1View * pView, CRect & rect)
{
	//CClientDC dc(pView);
	//pView->OnPrepareDC(&dc, NULL);
	//dc.LPtoDP(&rect);

	CClientDC dc(pView);
	Graphics graphics(dc.m_hDC);
	graphics.ScaleTransform(m_fZoomFactor, m_fZoomFactor);
	CPoint point1 = rect.TopLeft();
	CPoint point2 = rect.BottomRight();
	Point points[2] = { Point((int) point1.x, (int) point1.y), 
						Point((int) point2.x, (int) point2.y) };
	// Transform the points in the array from world to page coordinates.
	graphics.TransformPoints(
		CoordinateSpaceDevice, 
		CoordinateSpaceWorld, 
		points, 
		2);

	point1.x = points[0].X;
	point1.y = points[0].Y;
	point2.x = points[1].X;
	point2.y = points[1].Y;
	rect.SetRect(point1, point2);
}

void CElementManager::PrepareDC(CModeler1View * pView, CDC* pDC, CPrintInfo* pInfo)
{
	return;

	// mapping mode is MM_ANISOTROPIC
	// these extents setup a mode similar to MM_LOENGLISH
	// MM_LOENGLISH is in .01 physical inches
	// these extents provide .01 logical inches

	CSize size = GetSize();

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(size.cx, size.cy);
	pDC->SetWindowExt(size.cx, size.cy);

	// set the origin of the coordinate system to the center of the page
	CPoint ptOrg;
	ptOrg.x = 0; //GetDocument()->GetSize().cx / 2;
	ptOrg.y = 0; //GetDocument()->GetSize().cy / 2;

	// ptOrg is in logical coordinates
	pDC->OffsetWindowOrg(0, 0); //-ptOrg.x,ptOrg.y);
}

void CElementManager::DrawBackground(CModeler1View * pView, CDC * pDC)
{
	Graphics graphics(pDC->m_hDC);
	CSize size = GetSize();
	//SolidBrush brush(Color(255, 255, 255, 255));
	Color colorLine(255, GetRValue(GetPaperColor()), GetGValue(GetPaperColor()), GetBValue(GetPaperColor()));
	SolidBrush brush(colorLine);
	Rect fillRect(0, 0 , size.cx, size.cy);
	graphics.FillRectangle(&brush, fillRect);
}

void CElementManager::Draw(CModeler1View * pView, CDC * pDC)
{
	// Initialize GDI+ graphics context
	Graphics graphics(pDC->m_hDC);
	// just like that
	//graphics.ScaleTransform(0.75f, 0.75f);
	graphics.ScaleTransform(m_fZoomFactor, m_fZoomFactor);
	
	// TODO: add draw code for native data here
	for( vector<std::shared_ptr<CElement>>::const_iterator i = GetObjects().begin() ; i!=GetObjects().end() ; i++ )
	{
		std::shared_ptr<CElement> pElement = *i;		
		// FIXME: Update the view for Property Window
		pElement->m_pView = pView;
		
		// Construct the graphic context for each element
		CDrawingContext ctxt(pElement);
		ctxt.m_pGraphics = &graphics;

		//pElement->Draw(pView, pDC);
		pElement->Draw(ctxt);

		// HACK 14072012 : no more SimpleTextElement / Just other text elements except for caption old element
		// caption property is deprecated. be carefull. DO NOT USE m_caption any more !!!
		// informations: 
		//    for m_type that are simple shapes  ; it means it is not about text shapes,
		//    the m_text property can be optional and if it exists,
		//    it should be appended to the rendering area by creating a dedicated object. 
		//    We call it CSimpleTextElement.
		if( pElement->m_text.empty() == false && 
			(pElement->m_type != ElementType::type_text)
			)
		{
			//std::shared_ptr<CElement> pTextElement = make_shared<CSimpleTextElement>();
			std::shared_ptr<CElement> pTextElement(new CSimpleTextElement());
			pTextElement->m_rect = pElement->m_rect;
			pTextElement->m_text = pElement->m_text;
			pTextElement->m_textAlign = pElement->m_textAlign;
			pTextElement->Draw(ctxt);
		}

		//if( !pDC->IsPrinting() && IsSelected(pObj) )
		//	DrawTracker(pObj, pDC, TrackerState::selected);
		if (pView != NULL && pView->m_bActive && !pDC->IsPrinting() && IsSelected(pElement))
			pElement->DrawTracker(ctxt, TrackerState::selected);
	}
}

void CElementManager::DrawEx(CModeler1View * pView, CDC * pDC)
{
	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap;

	// only paint the rect that needs repainting
	CRect client;
	CSize size = GetSize();
	CRect fillRect(0, 0 , size.cx, size.cy);
	
	// version n°1
	// Previous : it was working since Ribbon UI...
	//pDC->GetClipBox(client);
	//ManagerToView(pView, rect);
	
	// version n°2
	// HACK?
	//pView->GetClientRect(client);
	
	// version n°3
	client = fillRect;
	
	CRect rect = client;	
	//rect.NormalizeRect();

	//CString str;
	//str.Format(_T("client=%d,%d,%d,%d"), client.left, client.top, client.right, client.bottom);
	//pView->LogDebug(str);

	if (!pDC->IsPrinting())
	{
		// draw to offscreen bitmap for fast looking repaints
		if (dc.CreateCompatibleDC(pDC))
		{
			if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				pView->OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;

				// offset origin more because bitmap is just piece of the whole drawing
				dc.OffsetViewportOrg(rect.left, rect.top);
				pOldBitmap = dc.SelectObject(&bitmap);
				//dc.SetBrushOrg(rect.left % 8, rect.top % 8);

				// might as well clip to the same rectangle
				dc.IntersectClipRect(client);
			}
		}
	}

#ifdef DRAW_PAPER_BACKGROUND
	// paint background
	CBrush brush;
	if (!brush.CreateSolidBrush(GetPaperColor()))
		return;
	brush.UnrealizeObject();
	
	//CSize size = GetSize();
	//CRect fillRect(0, 0 , size.cx, size.cy);
	pDrawDC->FillRect(fillRect, &brush);
	//pDrawDC->FillRect(client, &brush);

	//if (!pDC->IsPrinting() && m_bGrid)
	//	DrawGrid(pDrawDC);
#endif
	
	// Background drawing routine call
	DrawBackground(pView, pDrawDC);

	// Main drawing routine call
	Draw(pView, pDrawDC);

	if (pDrawDC != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0,0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0,0);
		dc.SetMapMode(MM_TEXT);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBitmap);
	}

	// Caution, it flicks !
	//pView->LogDebug(_T("CElementManager::DrawEx"));
}

void CElementManager::OnLButtonDown(CModeler1View* pView, UINT nFlags, const CPoint& cpoint)
{
	// Caution, it flicks !
	//pView->LogDebug(_T("CElementManager::OnLButtonDown"));

	if( m_type == ElementType::type_unknown )
	{
		//FIXME : do we need to handle not implemented objects ?
		return;
	}
	
	//m_bDrawing = true;
	
	CPoint point = cpoint;
	//m_clickPoint = point;
	ViewToManager(pView, point);
	m_clickPoint = point;
	//ManagerToView(pView, m_clickPoint);
	m_lastPoint = point;
	m_selectPoint = point;

	// Debugging
	CString str;
	str.Format(_T("m_clickPoint {%d,%d} / point {%d,%d} / cpoint {%d,%d}"), m_clickPoint.x, m_clickPoint.y, point.x, point.y, cpoint.x, cpoint.y);
	pView->LogDebug(str);

	if( m_type == ElementType::type_select )
	{

		m_selectMode = SelectMode::none;

		// Check for resizing (only allowed on single selections)
		if( HasSelection() &&  m_selection.GetCount() == 1)
		{
			std::shared_ptr<CElement> pElement = m_selection.GetHead();
			// Change cursor look because mouse click is over an object for sizing 
			m_nDragHandle = pElement->HitTest(point, pView, TRUE);
			if (m_nDragHandle != 0)
			{
				m_selectMode = SelectMode::size;
				pView->LogDebug(_T("selectMode == sized"));
			}
		}

		if( m_selectMode == SelectMode::none )
		{
			// See if the click was on an object
			std::shared_ptr<CElement> pElement = m_objects.ObjectAt(point);
			if( pElement != NULL )
			{
				//if( HasSelection() )
				//{
				//	pView->LogDebug("selection cleared");
				//	SelectNone();
				//}

				pView->LogDebug(_T("object found ->") + pElement->ToString());
				if( IsSelected(pElement) == false )
				{
					if( (nFlags & MK_SHIFT) || (nFlags & MK_CONTROL))
					{
					}
					else
						SelectNone();

					if (pElement->m_bGrouping == false)
					{
						pView->LogDebug(_T("object selected ->") + pElement->ToString());
						m_objectId = pElement->m_objectId;
						Select(pElement);
					}
					else
					{
						for (vector<std::shared_ptr<CElement>>::const_iterator itSel = pElement->m_pElementGroup->m_Groups.begin(); itSel != pElement->m_pElementGroup->m_Groups.end(); itSel++)
						{
							std::shared_ptr<CElement> pObj = *itSel;
							Select(pObj);
						}
					}

					pElement->m_bMoving = true;
				}

				m_selectMode = SelectMode::move;
				pView->LogDebug(_T("selectMode == move"));

				// Update UI
				UpdateUI(pView, pElement);
				// Redraw
				Invalidate(pView, pElement);
			}
			else
			{
				// See if the click was on an object
				// TRUE -> select and start move if so
				// FALSE -> Click on background, start a net-selection
				// m_selectMode = netSelect;

				if( HasSelection() )
				{
					pView->LogDebug(_T("selection cleared"));
					SelectNone();
					Invalidate(pView, pElement);
				}
				
				//m_selectPoint = point;
				m_selectMode = SelectMode::netselect;
				pView->LogDebug(_T("selectMode == netselect"));
			}
		}
	}
	// We are not in a select operation
	// -> this is a drawing operation
	// We have to create...
	// Create a Drawable Object...
	else
	{
		pView->LogDebug(_T("selection cleared"));
		SelectNone();

		std::shared_ptr<CElement> pNewElement = CFactory::CreateElementOfType(m_type, m_shapeType);
		if( m_type == ElementType::type_unknown )
		{
			pView->LogDebug(_T("object not implemented yet ! ->") + pNewElement->ToString());
			return;
		}

		if (m_shapeType == ShapeType::selection)
		{
			m_bSelectionHasStarted = true;
			pSelectionElement = pNewElement;
		}
		
		pNewElement->m_point = point;
		// For plumbing purpose...
		pNewElement->m_pManager = this;
		pNewElement->m_pView = pView;

		// Add an object
		m_objects.AddTail(pNewElement);	
		pView->LogDebug(_T("object created ->") + pNewElement->ToString());
		
		// Store last created object
		m_objectId = pNewElement->m_objectId;

		// Select the new element
		Select(pNewElement);
		pView->LogDebug(_T("object selected ->") + pNewElement->ToString());

		m_selectMode = SelectMode::size;
		pView->LogDebug(_T("selectMode == size"));

		pView->GetDocument()->SetModifiedFlag();

		// Update ClassView & FileView
		UpdateClassView();//pNewElement);
		UpdateFileView();//pNewElement);

		// Update UI
		UpdateUI(pView, pNewElement);
	}

}

void CElementManager::OnLButtonDblClk(CModeler1View* pView, UINT nFlags, const CPoint& cpoint)
{
	// Caution, it flicks !
	//pView->LogDebug(_T("CElementManager::OnLButtonDblClk"));

}

void CElementManager::DrawSelectionRect(CModeler1View *pView)
{
	CClientDC dc(pView);
	Graphics graphics(dc.m_hDC);
	graphics.ScaleTransform(m_fZoomFactor, m_fZoomFactor);

	Color colorBlack(255, 0, 0, 0);
	Pen penBlack(colorBlack);
	CRect rect = m_selectionRect;
	rect.NormalizeRect();

	Invalidate(pView);
	graphics.DrawRectangle(&penBlack, rect.left, rect.top, rect.Width(), rect.Height());
}

void CElementManager::OnMouseMove(CModeler1View* pView, UINT nFlags, const CPoint& cpoint)
{
	// Caution, it flicks !
	//pView->LogDebug(_T("CElementManager::OnMouseMove"));

	// a SELECT operation is started
	if( m_selectMode == SelectMode::none )
	{
	}
	
	//if( m_bDrawing == FALSE )
	//	return;

	CPoint point = cpoint;
	CPoint m_movePoint = point;
	ViewToManager(pView, point);
	CPoint lastPoint = point;

	// Debugging
	CString str;
	str.Format(_T("point {%d,%d} / {%d,%d}"), cpoint.x, cpoint.y, point.x, point.y);
	//pView->LogDebug(str);

	// Unused code !
	if (m_selectMode == SelectMode::netselect) //|| m_shapeType == ShapeType::selection)
	//if (m_bSelectionHasStarted == true)
	{
		//pView->LogDebug("selection is under drawing");
		
		CRect selectionRect(m_selectPoint.x, m_selectPoint.y, point.x, point.y);
		CRect rect = selectionRect;
		ViewToManager(pView, rect);

		//std::shared_ptr<CElement> pObj = m_objects.ObjectAt(point);
		vector<std::shared_ptr<CElement>> v = m_objects.ObjectsInRect(rect);
		if (v.size() != 0)
		{
			for (std::shared_ptr<CElement> pElement : v)
			{
				if (IsSelected(pElement) == false)
				{
					if (pElement->m_bGrouping == false)
					{
						Select(pElement);
					}
				}
			}
		}

		//CRect rect2(m_clickPoint.x, m_clickPoint.y, m_movePoint.x, m_movePoint.y);
		//ManagerToView(pView, rect2);
		//m_selectionRect = rect2;
		//DrawSelectionRect(pView);

		return;
	}
	
	std::shared_ptr<CElement> pElement = m_selection.GetHead(); //m_objects.FindElement(m_objectId);
	if( pElement == NULL )
	{
		//pView->LogDebug(_T("FindElement return NULL; return;"));
		return;
	}

	if( m_type == ElementType::type_select )
	{
		if( HasSelection() )
		{
			// Change cursor look temporary just because mouse could be over a shape
			int nHandle = pElement->HitTest(point, pView, true);
			if (nHandle != 0)
			{
				SetCursor(pElement->GetHandleCursor(nHandle));
			}

			if( m_selectMode == SelectMode::move )
			{
				//pView->LogDebug("object selection moved ->" + pElement->ToString());
			
				for( vector<std::shared_ptr<CElement>>::const_iterator itSel = m_selection.m_objects.begin() ; itSel!=m_selection.m_objects.end() ; itSel++ )
				{
					std::shared_ptr<CElement> pObj = *itSel;

					CPoint delta = (CPoint)(point - m_lastPoint);		
					//std::shared_ptr<CElement> pObj = m_selection.GetHead();
					InvalObj(pView, pObj);
					pObj->m_rect += delta;
					pObj->m_point = pObj->m_rect.TopLeft();
					InvalObj(pView, pObj);
				}
				
				pView->GetDocument()->SetModifiedFlag();
			}

			if( m_selectMode == SelectMode::size )
			{
				if( m_nDragHandle != 0)
				{
					//pView->LogDebug("object selection sized ->" + pElement->ToString());

					std::shared_ptr<CElement> pObj = m_selection.GetHead();
					pObj->MoveHandleTo(m_nDragHandle, point, pView);
					// Find a connection ?
					FindAConnectionFor(pObj, point, pView);
					InvalObj(pView, pObj);

					pView->GetDocument()->SetModifiedFlag();
				}
			}

		}
	}
	else
	{
		if( m_selectMode == SelectMode::size )
		{
			//pView->LogDebug("obect is under drawing ->" + pElement->ToString());
		
			pElement->m_last = point;
			pElement->InvalidateObj();
			//MoveToBack(pView);
			// Find a connection ?
			FindAConnectionFor(pElement, point, pView);
			InvalObj(pView, pElement);

			pView->GetDocument()->SetModifiedFlag();
		}
	}	

	m_lastPoint = point;

	// Check for mouse cursor -> sizing/moving
 	if( m_selectMode == SelectMode::size )
	{
		//if (m_nDragHandle != 0)
 		{
			SetCursor(m_selection.GetHead()->GetHandleCursor(m_nDragHandle));
		}
	}

	if( m_type == ElementType::type_select && m_selection.GetCount() == 0)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
}


void CElementManager::OnLButtonUp(CModeler1View* pView, UINT nFlags, const CPoint& cpoint)
{
	CPoint point = cpoint;
	ViewToManager(pView, point);

	/*
	if (m_selectMode == SelectMode::netselect)
	{
		//m_selectionRect
		POINT p;
		p.x = point.x;
		p.y = point.y;

		for (vector<std::shared_ptr<CElement>>::const_iterator i = GetObjects().begin(); i != GetObjects().end(); i++)
		{
			std::shared_ptr<CElement> pElement = *i;

			for (int y = 0; y < p.y; ++y)
			{
				for (int x = 0; x < p.x; ++x)
				{
					POINT pToCheck;
					pToCheck.x = x;
					pToCheck.y = y;

					CRect rect = pElement->m_rect;
					if (rect.PtInRect(pToCheck) == TRUE)
					{
						if (IsSelected(pElement) == false)
						{
							Select(pElement);
						}
						goto label1;
					}
				}
			}

		label1:
			bool b = true;
		}
	}
	*/

	// Caution, it flicks !
	//pView->LogDebug(_T("CElementManager::OnLButtonUp"));

	//if( m_selectMode == SelectMode::move || m_selectMode == SelectMode::size )
	{
		// Selection Moving or Sizing is finished. 
		// Nothing to do.
		m_selectMode = SelectMode::none;
	}

	//m_bDrawing = FALSE;

	std::shared_ptr<CElement> pElement = m_objects.FindElement(m_objectId);
	if (pElement == NULL)
		return;

	if (m_type == ElementType::type_select)
	{
		if (HasSelection() && m_selection.GetCount() == 1)
		{
			// Nothing to do...
			pView->LogDebug(_T("object selection finished ->") + pElement->ToString());
		}
	}
	else
	{
		// Finish a drawing operation
		pElement->m_last = point;
		pElement->InvalidateObj();
		pElement->CheckForKeepingAMinimumSize();
		// Switch the view in Select Mode
		m_type = ElementType::type_select;

		pView->LogDebug(_T("object drawing finished ->") + pElement->ToString());
	}


	if (m_bSelectionHasStarted == true)
	{
		CRect rect = pSelectionElement->m_rect;

		vector<std::shared_ptr<CElement>>::iterator pos;
		pos = find(m_objects.m_objects.begin(), m_objects.m_objects.end(), pSelectionElement);
		if (pos != m_objects.m_objects.end())
		{
			m_objects.m_objects.erase(pos);
		}

		//ViewToManager(pView, rect);

		vector<std::shared_ptr<CElement>> v = m_objects.ObjectsInRect(rect);
		if (v.size() != 0)
		{
			for (std::shared_ptr<CElement> pElement : v)
			{
				if (IsSelected(pElement) == false)
				{
					if (pElement->m_bGrouping == false)
					{
						Select(pElement);
					}
				}
			}
		}

		pSelectionElement = nullptr;
		m_bSelectionHasStarted = false;
	}


	pElement->m_bMoving = FALSE;
	// Update UI
	UpdateUI(pView, pElement);
	// Redraw
	InvalObj(pView, pElement);	

	m_selectMode = SelectMode::none;
	pView->GetDocument()->SetModifiedFlag();
}

void CElementManager::InvalObj(CModeler1View * pView, std::shared_ptr<CElement> pElement)
{
	CRect rect = pElement->m_rect;
	ManagerToView(pView, rect);
	if( pView->m_bActive && IsSelected(pElement) )
	{
		rect.left -= 4;
		rect.top -= 5;
		rect.right += 5;
		rect.bottom += 4;
	}
	rect.InflateRect(1, 1); // handles CDrawOleObj objects

	pView->InvalidateRect(rect, FALSE);
	//FIXME:
	Invalidate(pView, pElement);
}

void CElementManager::Invalidate(CModeler1View * pView)
{
	pView->Invalidate(FALSE);
}

void CElementManager::Invalidate(CModeler1View * pView, std::shared_ptr<CElement> pElement)
{
	pView->Invalidate(FALSE);
}

void CElementManager::Update(CModeler1View * pView, LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	case HINT_UPDATE_WINDOW:    // redraw entire window
		pView->LogDebug(_T("CElementManager::Update HINT_UPDATE_WINDOW"));
		pView->Invalidate(FALSE);
		break;

	case HINT_UPDATE_DRAWOBJ:   // a single object has changed
		{
			pView->LogDebug(_T("CElementManager::Update HINT_UPDATE_DRAWOBJ"));
			CElement * p = (CElement *)pHint;
			InvalObj(pView, std::shared_ptr<CElement>(p));
		}
		break;

	case HINT_UPDATE_SELECTION: // an entire selection has changed
		{
			pView->LogDebug(_T("CElementManager::Update HINT_UPDATE_SELECTION"));
			pView->Invalidate(FALSE);
			//CElementContainer * pList = pHint != NULL ? (CElementContainer*)pHint : &m_selection;
			//POSITION pos = pList->m_objects.GetHeadPosition();
			//while (pos != NULL)
			//{
			//	CElement * pElement = pList->m_objects.GetNext(pos);
			//	InvalObj(pView, pElement);

			//	// Update UI
			//	UpdateUI(pView, pElement);
			//}			
		}
		break;

	case HINT_DELETE_SELECTION: // an entire selection has been removed
		/*
		if (pHint != &m_selection)
		{
			CDrawObjList* pList = (CDrawObjList*)pHint;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
			{
				CDrawObj* pObj = pList->GetNext(pos);
				InvalObj(pObj);
				Remove(pObj);   // remove it from this view's selection
			}
		}
		*/
		break;

	case HINT_UPDATE_OLE_ITEMS:
		/*
		{
			CDrawDoc* pDoc = GetDocument();
			POSITION pos = pDoc->GetObjects()->GetHeadPosition();
			while (pos != NULL)
			{
				CDrawObj* pObj = pDoc->GetObjects()->GetNext(pos);
				if (pObj->IsKindOf(RUNTIME_CLASS(CDrawOleObj)))
					InvalObj(pObj);
			}
		}
		*/
		break;

	default:
		ASSERT(FALSE);
		break;
	}
}

void CElementManager::UpdateClassView()
{
	std::shared_ptr<CElement> pNullElement;
	UpdateClassView(pNullElement);
	for( vector<std::shared_ptr<CElement>>::const_iterator i = GetObjects().begin() ; i!=GetObjects().end() ; i++ )
	{
		std::shared_ptr<CElement> pElement = *i;
		UpdateClassView(pElement);
	}
}

void CElementManager::UpdateClassView(std::shared_ptr<CElement> pElement)
{
	CWnd * p = AfxGetMainWnd();
	CMainFrame * pmf = (CMainFrame *)p;
	if( pElement == NULL )
		pmf->InitClassView();
	else
		pmf->UpdateClassViewFromObject(pElement);
}

void CElementManager::UpdateFileView()
{
	std::shared_ptr<CElement> pNullElement;
	UpdateFileView(pNullElement);
	for( vector<std::shared_ptr<CElement>>::const_iterator i = GetObjects().begin() ; i!=GetObjects().end() ; i++ )
	{
		std::shared_ptr<CElement> pElement = *i;
		UpdateFileView(pElement);
	}
}

void CElementManager::UpdateFileView(std::shared_ptr<CElement> pElement)
{
	CWnd * p = AfxGetMainWnd();
	CMainFrame * pmf = (CMainFrame *)p;
	if( pElement == NULL )
		pmf->InitFileView();
	else
		pmf->UpdateFileViewFromObject(pElement);
}

void CElementManager::UpdateUI(CModeler1View * pView, std::shared_ptr<CElement> pElement)
{	
	// Update Property Grid
	UpdatePropertyGrid(pView, pElement);
	// Update Ribbon UI
	UpdateRibbonUI(pView, pElement);
}

void CElementManager::UpdatePropertyGrid(CModeler1View * pView, std::shared_ptr<CElement> pElement)
{
	CWnd * p = AfxGetMainWnd();
	CMainFrame * pmf = (CMainFrame *)p;
	pmf->UpdatePropertiesFromObject(pElement);
}

void CElementManager::UpdateRibbonUI(CModeler1View * pView, std::shared_ptr<CElement> pElement)
{	
	CWnd * p = AfxGetMainWnd();
	CMainFrame * pmf = (CMainFrame *)p;
	pmf->UpdateRibbonUI(pView);
}

void CElementManager::UpdateFromPropertyGrid(std::wstring objectId, std::wstring name, std::wstring value)
{
	bool bUpdateUI = false;
	std::shared_ptr<CElement> pElement = m_objects.FindElement(objectId);
	if( pElement == NULL )
	{
		return;
	}

	if( name == prop_Name )
	{
		pElement->m_name = value;
		bUpdateUI = true;
	}

	if( name == prop_Caption )
	{
		pElement->m_caption = value;
	}

	if( name == prop_Text )
	{
		pElement->m_text = value;
	}

	if (name == prop_Text_Align)
	{
		if (value == _T("Left") || value == _T("Center") || value == _T("Right"))
		{
			pElement->m_textAlign = value;
		}
	}

	if (name == prop_Image)
	{
		pElement->m_image = value;
	}

	if (name == prop_Font_Name)
	{
		pElement->m_fontName = value;
	}

	// Some properties could change the UI in class view or file view
	if( bUpdateUI == true )
	{
		this->UpdateClassView();
		this->UpdateFileView();
	}
	InvalObj(pElement->GetView(), pElement);
}

void CElementManager::UpdateFromPropertyGrid(std::wstring objectId, std::wstring name, COLORREF color)
{
	std::shared_ptr<CElement> pElement = m_objects.FindElement(objectId);
	if( pElement == NULL )
	{
		return;
	}

	if (name == prop_Fill_Color)
	{
		pElement->m_colorFill = color;
	}
	
	if (name == prop_Line_Color)
	{
		pElement->m_colorLine = color;
	}

	UpdateRibbonUI(pElement->GetView(), pElement);
	InvalObj(pElement->GetView(), pElement);
}

void CElementManager::UpdateFromPropertyGrid(std::wstring objectId, std::wstring name, long value)
{
	std::shared_ptr<CElement> pElement = m_objects.FindElement(objectId);
	if( pElement == NULL )
	{
		return;
	}

	if (name == prop_Left)
	{
		pElement->m_rect.left = value;
	}
	
	if (name == prop_Right)
	{
		pElement->m_rect.right = value;
	}
	
	if (name == prop_Top)
	{
		pElement->m_rect.top = value;
	}

	if (name == prop_Bottom)
	{
		pElement->m_rect.bottom = value;
	}

	if (name == prop_Has_Fill_Color)
	{
		pElement->m_bColorFill = value == 0 ? false: true;
	}

	if (name == prop_Is_Fill_Solid_Color)
	{
		pElement->m_bSolidColorFill = value == 0 ? false: true;
	}

	if (name == prop_Has_Line_Color)
	{
		pElement->m_bColorLine = value == 0 ? false: true;
	}

	if (name == prop_Font_Size)
	{
		pElement->m_fontSize = value;
	}

	if (name == prop_Fixed)
	{
		pElement->m_bFixed = value == 0 ? false : true;
	}

	InvalObj(pElement->GetView(), pElement);
}

void CElementManager::ActivateView(CModeler1View * pView, bool bActivate, CView* pActiveView, CView* pDeactiveView)
{
	// invalidate selections when active status changes
	if (pView->m_bActive != bActivate)
	{
		if( bActivate )  // if becoming active update as if active
			pView->m_bActive = bActivate;
		if( HasSelection() )
			Update(pView, HINT_UPDATE_SELECTION, NULL);
		pView->m_bActive = bActivate;

		// Initiate the connection with the Property Window
		ConnectToPropertyGrid();
		ConnectToMainFrame(pView);
		// Update ClassView & FileView
		UpdateClassView();
		UpdateFileView();
	}

	//((CMainFrame*)AfxGetMainWnd())->UpdateUI(this);
	//((CMainFrame*)AfxGetMainWnd())->UpdateContextTab(this);
}

void CElementManager::ConnectToMainFrame(CModeler1View * pView)
{
	CWnd * p = AfxGetMainWnd();
	CMainFrame * pmf = (CMainFrame *)p;
	pmf->SetView(pView);
}

void CElementManager::DebugDumpObjects(CModeler1View * pView)
{
	pView->LogDebug(_T("Dumping m_objects..."));
	m_objects.DebugDumpObjects(pView);
	pView->LogDebug(_T("Dumping m_selection..."));
	m_selection.DebugDumpObjects(pView);
}

void CElementManager::NoFillColor(CModeler1View * pView)
{
	//POSITION pos = m_selection.m_objects.GetHeadPosition();
	//while (pos != NULL)
	//{
	//	CElement * pElement = m_selection.m_objects.GetNext(pos);
	//	if (pElement->CanChangeFillColor())
	//	{
	//		pElement->m_bColorFill = FALSE;
	//	}
	//}

	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	if (pElement->CanChangeFillColor())
	{
		pElement->m_bColorFill = FALSE;
	}

	Invalidate(pView);
}

void CElementManager::FillColor(CModeler1View * pView)
{
	COLORREF color = ((CMainFrame*)AfxGetMainWnd())->GetColorFromColorButton(ID_FORMAT_FILLCOLOR);

	if (color == (COLORREF) -1)
	{
		return;
	}

	//POSITION pos = m_selection.m_objects.GetHeadPosition();
	//while (pos != NULL)
	//{
	//	CElement * pElement = m_selection.m_objects.GetNext(pos);
	//	if (pElement->CanChangeFillColor())
	//	{
	//		pElement->m_colorFill = color;
	//		pElement->m_bColorFill = true;
	//	}
	//}

	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	if (pElement->CanChangeFillColor())
	{
		pElement->m_colorFill = color;
		pElement->m_bColorFill = true;
	}

	Invalidate(pView);
}

void CElementManager::LineColor(CModeler1View * pView)
{
	COLORREF color = ((CMainFrame*)AfxGetMainWnd())->GetColorFromColorButton(ID_FORMAT_LINECOLOR);

	if (color == (COLORREF) -1)
	{
		return;
	}

	//POSITION pos = m_selection.m_objects.GetHeadPosition();
	//while (pos != NULL)
	//{
	//	CElement * pElement = m_selection.m_objects.GetNext(pos);
	//	if (pElement->CanChangeLineColor())
	//	{
	//		pElement->m_colorLine = color;
	//		pElement->m_bColorLine = true;
	//	}
	//}

	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	if (pElement->CanChangeLineColor())
	{
		pElement->m_colorLine = color;
		pElement->m_bColorLine = true;
	}

	Invalidate(pView);
}

void CElementManager::LineWidth(CModeler1View * pView, UINT nID)
{
	int weight = -1;
	if (nID == ID_FORMAT_LINEWIDTH)
	{
		weight = ((CMainFrame*)AfxGetMainWnd())->GetWidthFromLineWidth(ID_FORMAT_LINEWIDTH);
	}
	else if (nID == ID_FORMAT_LINEWIDTH_MORE)
	{
		/*
		CLineWeightDlg dlg(AfxGetMainWnd());

		if (m_selection.GetCount() == 1)
		{
			dlg.m_penSize = m_selection.GetHead()->IsEnableLine() ? m_selection.GetHead()->GetLineWeight() : 0;
		}

		if (dlg.DoModal() == IDOK)
		{
			weight = dlg.m_penSize;
		}
		*/
	}

	if (weight == -1)
	{
		return;
	}

	//POSITION pos = m_selection.m_objects.GetHeadPosition();
	//while (pos != NULL)
	//{
	//	CElement * pElement = m_selection.m_objects.GetNext(pos);
	//	if (pElement->CanChangeLineWidth())
	//	{
	//		pElement->m_bLineWidth = weight > 0;
	//		pElement->m_lineWidth = weight;
	//	}
	//}

	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	if (pElement->CanChangeLineWidth())
	{
		pElement->m_bLineWidth = weight > 0;
		pElement->m_lineWidth = weight;
	}

	Invalidate(pView);
}

void CElementManager::PageColor(CModeler1View * pView)
{
	COLORREF color = ((CMainFrame*)AfxGetMainWnd())->GetColorFromColorButton(ID_FORMAT_PAGECOLOR);

	if (color == (COLORREF) -1)
	{
		return;
	}

	m_paperColor = color;
	Invalidate(pView);
}

void CElementManager::Zoom(CModeler1View * pView)
{
	//m_fZoomFactor += 0.10f;
	//Invalidate(pView);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMFCRibbonBar* pRibbon = pFrame->GetRibbonBar();
	CMFCRibbonComboBox* pFindCombobox = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pRibbon->FindByID(ID_FORMAT_ZOOM));
		// this returns the last value before the combo box edit field got the focus:
	CString value = pFindCombobox->GetEditText();

	//AfxMessageBox(value);

	if (value == _T("25%"))
	{
		m_fZoomFactor = 0.25f;
	}
	if (value == _T("50%"))
	{
		m_fZoomFactor = 0.50f;
	}
	if (value == _T("75%"))
	{
		m_fZoomFactor = 0.75f;
	}
	if (value == _T("100%"))
	{
		m_fZoomFactor = 1.0f;
	}
	if (value == _T("150%"))
	{
		m_fZoomFactor = 1.5f;
	}
	if (value == _T("200%"))
	{
		m_fZoomFactor = 2.0f;
	}
	if (value == _T("400%"))
	{
		m_fZoomFactor = 4.0f;
	}

	Invalidate(pView);
}

void CElementManager::ZoomIn(CModeler1View * pView)
{
	m_fZoomFactor += 0.10f;
	Invalidate(pView);
}

void CElementManager::ZoomOut(CModeler1View * pView)
{
	m_fZoomFactor -= 0.10f;
	Invalidate(pView);
}

void CalcAutoPointRect2(int count, std::shared_ptr<CElement> pNewElement)
{
	int x = count % 20;
	int y = count % 20;

	pNewElement->m_point.x = 50 * x;
	pNewElement->m_point.y = 50 * y;

	pNewElement->m_rect.top = pNewElement->m_point.y;
	pNewElement->m_rect.left = pNewElement->m_point.x;
	pNewElement->m_rect.bottom = pNewElement->m_point.y + 100;
	pNewElement->m_rect.right = pNewElement->m_point.x + 100;
}

void CalcAutoPointRect(int count, std::shared_ptr<CElement> pNewElement)
{
	int c = 0;
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			if (count%400 == c)
			{
				pNewElement->m_point.x = 200 * x;
				pNewElement->m_point.y = 50 * y;

				pNewElement->m_rect.left = pNewElement->m_point.x;
				pNewElement->m_rect.top = pNewElement->m_point.y;
				pNewElement->m_rect.right = pNewElement->m_point.x + 100 + 50;
				pNewElement->m_rect.bottom = pNewElement->m_point.y + 30;
				return;
			}

			c++;
		}
	}
}

void CElementManager::LoadModule(CModeler1View * pView)
{
}

void CElementManager::FindAConnectionFor(std::shared_ptr<CElement> pElement, CPoint point, CModeler1View* pView)
{
}

void CElementManager::OnFileOpenGabarit(CModeler1View* pView)
{
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() != IDOK)
		return;

	CStringW fileName = dlg.GetPathName();

	CDocument* pDocument = ((CModeler1App*)AfxGetApp())->m_pDocTemplate->CreateNewDocument();
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	pDocument->OnNewDocument();
	// open an existing document
	CWaitCursor wait;
	pDocument->OnOpenDocument((LPTSTR)(LPCTSTR)fileName);
	CModeler1Doc* pDoc = (CModeler1Doc*)pDocument;

	vector<shared_ptr<CElement>> newVector;
	for (shared_ptr<CElement> pElement : pDoc->GetManager()->GetObjects())
	{
		std::shared_ptr<CElement> pNewElement = CFactory::CreateElementOfType(pElement->m_type, pElement->m_shapeType);
		pNewElement->m_name = pElement->m_name;
		pNewElement->m_text = pElement->m_text;
		pNewElement->m_objectId = pElement->m_objectId;
		pNewElement->m_rect = pElement->m_rect;
		pNewElement->m_bColorFill = pElement->m_bColorFill;
		pNewElement->m_bColorLine = pElement->m_bColorLine;
		pNewElement->m_bColorFill = pElement->m_bColorFill;
		pNewElement->m_bLineWidth = pElement->m_bLineWidth;
		pNewElement->m_bSolidColorFill = pElement->m_bSolidColorFill;
		//pNewElement->m_caption = pElement->m_caption;
		pNewElement->m_colorFill = pElement->m_colorFill;
		pNewElement->m_colorLine = pElement->m_colorLine;
		pNewElement->m_image = pElement->m_image;
		pNewElement->m_last = pElement->m_last;
		pNewElement->m_lineWidth = pElement->m_lineWidth;
		pNewElement->m_pManager = this;
		pNewElement->m_point = pElement->m_point;
		pNewElement->m_pView = pView;

		newVector.push_back(pNewElement);
	}

	//pView->GetDocument()->GetManager()->m_objects.m_objects.insert(pView->GetDocument()->GetManager()->m_objects.m_objects.end(), newVector.begin(), newVector.end());
	// eq. MoveBack
	for (shared_ptr<CElement> pElement : newVector)
	{
		pView->GetDocument()->GetManager()->m_objects.m_objects.insert(pView->GetDocument()->GetManager()->m_objects.m_objects.begin(), pElement);
	}

	Invalidate(pView);
}

void CElementManager::AlignLeft(CModeler1View* pView)
{
	if (HasSelection())
	{
		shared_ptr<CElement> pElementBase = m_selection.m_objects[0];

		for (vector<std::shared_ptr<CElement>>::const_iterator itSel = m_selection.m_objects.begin(); itSel != m_selection.m_objects.end(); itSel++)
		{
			std::shared_ptr<CElement> pObj = *itSel;
	
			int width = pObj->m_rect.Width();
			pObj->m_rect.left = pElementBase->m_rect.left;
			pObj->m_rect.right = pObj->m_rect.left + width;
			pObj->m_point = pObj->m_rect.TopLeft();
			InvalObj(pView, pObj);
		}

		pView->GetDocument()->SetModifiedFlag();
	}
}

void CElementManager::AlignRight(CModeler1View* pView)
{
	if (HasSelection())
	{
		shared_ptr<CElement> pElementBase = m_selection.m_objects[0];

		for (vector<std::shared_ptr<CElement>>::const_iterator itSel = m_selection.m_objects.begin(); itSel != m_selection.m_objects.end(); itSel++)
		{
			std::shared_ptr<CElement> pObj = *itSel;

			int width = pObj->m_rect.Width();
			pObj->m_rect.right = pElementBase->m_rect.right;
			pObj->m_rect.left = pObj->m_rect.right - width;
			pObj->m_point = pObj->m_rect.TopLeft();
			InvalObj(pView, pObj);
		}

		pView->GetDocument()->SetModifiedFlag();
	}
}

void CElementManager::AlignTop(CModeler1View* pView)
{
	if (HasSelection())
	{
		shared_ptr<CElement> pElementBase = m_selection.m_objects[0];

		for (vector<std::shared_ptr<CElement>>::const_iterator itSel = m_selection.m_objects.begin(); itSel != m_selection.m_objects.end(); itSel++)
		{
			std::shared_ptr<CElement> pObj = *itSel;

			int height = pObj->m_rect.Height();
			pObj->m_rect.top = pElementBase->m_rect.top;
			pObj->m_rect.bottom = pObj->m_rect.top + height;
			pObj->m_point = pObj->m_rect.TopLeft();
			InvalObj(pView, pObj);
		}

		pView->GetDocument()->SetModifiedFlag();
	}
}

void CElementManager::AlignBottom(CModeler1View* pView)
{
	if (HasSelection())
	{
		shared_ptr<CElement> pElementBase = m_selection.m_objects[0];

		for (vector<std::shared_ptr<CElement>>::const_iterator itSel = m_selection.m_objects.begin(); itSel != m_selection.m_objects.end(); itSel++)
		{
			std::shared_ptr<CElement> pObj = *itSel;

			int height = pObj->m_rect.Height();
			pObj->m_rect.bottom = pElementBase->m_rect.bottom;
			pObj->m_rect.top = pObj->m_rect.bottom - height;
			pObj->m_point = pObj->m_rect.TopLeft();
			InvalObj(pView, pObj);
		}

		pView->GetDocument()->SetModifiedFlag();
	}
}

void CElementManager::AlignTextLeft(CModeler1View* pView)
{
	if (HasSelection())
	{
		shared_ptr<CElement> pElementBase = m_selection.m_objects[0];

		for (vector<std::shared_ptr<CElement>>::const_iterator itSel = m_selection.m_objects.begin(); itSel != m_selection.m_objects.end(); itSel++)
		{
			std::shared_ptr<CElement> pObj = *itSel;

			pObj->m_textAlign = _T("Left");
			InvalObj(pView, pObj);
		}

		pView->GetDocument()->SetModifiedFlag();
	}
}

void CElementManager::AlignTextCenter(CModeler1View* pView)
{
	if (HasSelection())
	{
		shared_ptr<CElement> pElementBase = m_selection.m_objects[0];

		for (vector<std::shared_ptr<CElement>>::const_iterator itSel = m_selection.m_objects.begin(); itSel != m_selection.m_objects.end(); itSel++)
		{
			std::shared_ptr<CElement> pObj = *itSel;

			pObj->m_textAlign = _T("Center");
			InvalObj(pView, pObj);
		}

		pView->GetDocument()->SetModifiedFlag();
	}
}

void CElementManager::AlignTextRight(CModeler1View* pView)
{
	if (HasSelection())
	{
		shared_ptr<CElement> pElementBase = m_selection.m_objects[0];

		for (vector<std::shared_ptr<CElement>>::const_iterator itSel = m_selection.m_objects.begin(); itSel != m_selection.m_objects.end(); itSel++)
		{
			std::shared_ptr<CElement> pObj = *itSel;

			pObj->m_textAlign = _T("Right");
			InvalObj(pView, pObj);
		}

		pView->GetDocument()->SetModifiedFlag();
	}
}

void CElementManager::OnEditGroup(CModeler1View* pView)
{
	//AfxMessageBox(L"Grouping");

	shared_ptr<CElementGroup> speg = make_shared<CElementGroup>();

	for (vector<std::shared_ptr<CElement>>::const_iterator itSel = m_selection.m_objects.begin(); itSel != m_selection.m_objects.end(); itSel++)
	{
		std::shared_ptr<CElement> pObj = *itSel;
		pObj->m_pElementGroup = speg;
		pObj->m_bGrouping = true;
		speg->m_Groups.push_back(pObj);
	}

	this->m_groups.push_back(speg);
}

void CElementManager::OnEditUngroup(CModeler1View* pView)
{
	//AfxMessageBox(L"Ungrouping");
	shared_ptr<CElementGroup> speg = nullptr;

	for (vector<std::shared_ptr<CElement>>::const_iterator itSel = m_selection.m_objects.begin(); itSel != m_selection.m_objects.end(); itSel++)
	{
		std::shared_ptr<CElement> pElement = *itSel;
		speg = pElement->m_pElementGroup;
		break;
	}

	for (vector<std::shared_ptr<CElementGroup>>::iterator it = m_groups.begin(); it != m_groups.end(); it++)
	{
		shared_ptr<CElementGroup> pElementGroup = *it;

		if (pElementGroup == speg)
		{
			for (vector<std::shared_ptr<CElement>>::const_iterator itSel = pElementGroup->m_Groups.begin(); itSel != pElementGroup->m_Groups.end(); itSel++)
			{
				std::shared_ptr<CElement> pObj = *itSel;
				pObj->m_pElementGroup = nullptr;
				pObj->m_bGrouping = false;

			}
			m_groups.erase(it);
			break;
		}
	}
}
