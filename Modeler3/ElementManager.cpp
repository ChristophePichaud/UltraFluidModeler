#include "StdAfx.h"
#include "Modeler1.h"
#include "ElementManager.h"
#include "Modeler1View.h"
#include "MainFrm.h"
#include "DrawingContext.h"
#include "DrawingElements.h"
#include <sstream>
#include "TabbedView.h"
#include "Modeler1SourceView.h"

//
// CElementManager
//

IMPLEMENT_SERIAL(CElementManager, CObject, VERSIONABLE_SCHEMA | 11)

CElementManager::CElementManager()
{
	m_objectId = L"";
	m_lastPoint = CPoint(0, 0);
	m_paperColor = RGB(255, 255, 255); //RGB(242, 242, 200); //RGB(255, 255, 255); //RGB(188, 251, 255);
	m_size = CSize(3000, 3000);

	// Initialize Current UI interaction members
	m_bDrawing = FALSE;
	// Current selected drawing tool = SELECT
	m_type = ElementType::type_select;
	// Current selected shape type from Ribbon = type_unknown
	m_shapeType = ShapeType::unknown;
	m_nDragHandle = 1;
	m_fZoomFactor = 1.0f;
	
	m_bSavingCode = false;
	m_bSizingALine = false;

	m_selectType = SelectType::intuitive;
	m_elementGroup = _T("ElementGroup");

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
		//
		// Set version of file format
		//
		ar.SetObjectSchema(11);

		//CString elementGroup = W2T((LPTSTR)m_elementGroup.c_str());
		//ar << elementGroup;

		ar << m_size;
		ar << m_paperColor;
		ar << m_lastPoint;
	}
	else
	{
		int version = ar.GetObjectSchema();

		if (version >= 9)
		{
			//CString elementGroup;
			//ar >> elementGroup;
			//this->m_elementGroup = T2W((LPTSTR)(LPCTSTR)elementGroup);
		}

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

	UpdatePropertyGrid(pView, pElement);

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

	UpdatePropertyGrid(pView, pElement);

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

	//m_clipboard.RemoveAll();

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

void CElementManager::DrawConnector(Graphics& graphics, std::shared_ptr<CElement> pLineElement, ConnectorType connector)
{
	shared_ptr<CElement> pElement1; // = pLineElement->m_pConnector->m_pElement1;
	if (connector == ConnectorType::connector1)
	{
		pElement1 = pLineElement->m_pConnector->m_pElement1;
	}
	else
	{
		pElement1 = pLineElement->m_pConnector->m_pElement2;
	}

	CPoint point1;
	if (pElement1 == nullptr)
	{
		point1 = pLineElement->m_rect.TopLeft();
	}
	else
	{
		//point1 = pElement1->m_rect.CenterPoint();
		int handle;// = pLineElement->m_connectorDragHandle1;
		if (connector == ConnectorType::connector1)
		{
			handle = pLineElement->m_connectorDragHandle1;
		}
		else
		{
			handle = pLineElement->m_connectorDragHandle2;
		}


		if (handle == 0)
		{
			point1 = pElement1->m_rect.TopLeft();
		}
		else
		{
			point1 = pElement1->GetHandle(handle);
		}

		CPoint point2;
		point2.x = point1.x;
		point2.y = point1.y;
		CRect rect(point1, point2);
		rect.NormalizeRect();

		SolidBrush colorBrush(Color::DarkOrange);
		graphics.FillRectangle(&colorBrush, rect.left - 3, rect.top - 3, 7, 7);
		Pen  colorPen(Color::Black);
		graphics.DrawRectangle(&colorPen, rect.left - 3, rect.top - 3, 7, 7);

		std::wstring imagePath = L"Images\\Custom\\Connect.png";
		Image image(CStringW(imagePath.c_str()));
		CPoint p1(rect.left, rect.top);
		CPoint p2(p1.x + image.GetWidth(), p1.y + image.GetHeight());
		graphics.DrawImage(&image, rect.left - 3, rect.top - 3, image.GetWidth(), image.GetHeight());
	}
}

void CElementManager::Draw(CModeler1View * pView, CDC * pDC)
{
	// Initialize GDI+ graphics context
	Graphics graphics(pDC->m_hDC);
	// just like that
	//graphics.ScaleTransform(0.75f, 0.75f);
	graphics.ScaleTransform(m_fZoomFactor, m_fZoomFactor);
	
	// Iterate on Line elements
	// if connector1 exists, its draghandle 1 is connector1.centeroint else nothing (its inner value)
	// if connector2 exists, its draghandle 2 is connector2.centeroint else nothing (its inner value)
	// Then the m_rect value is m_rect = CRrect(point1, point2);
	if (m_bSizingALine == false)
	{
		for (vector<std::shared_ptr<CElement>>::const_iterator i = GetObjects().begin(); i != GetObjects().end(); i++)
		{
			std::shared_ptr<CElement> pElement = *i;

			if (pElement->IsLine() == false)
			{
				continue;
			}

			//pElement->m_rect.NormalizeRect();

			shared_ptr<CElement> pElement1 = pElement->m_pConnector->m_pElement1;
			CPoint point1;
			if (pElement1 == nullptr)
			{
				point1 = pElement->m_rect.TopLeft();
			}
			else
			{
				//point1 = pElement1->m_rect.CenterPoint();
				int handle = pElement->m_connectorDragHandle1;
				if (handle == 0)
				{
					point1 = pElement1->m_rect.TopLeft();
				}
				else
				{
					point1 = pElement1->GetHandle(handle);
				}
			}

			shared_ptr<CElement> pElement2 = pElement->m_pConnector->m_pElement2;
			CPoint point2;
			if (pElement2 == nullptr)
			{
				point2 = pElement->m_rect.BottomRight();
			}
			else
			{
				//point2 = pElement2->m_rect.CenterPoint();
				int handle = pElement->m_connectorDragHandle2;
				if (handle == 0)
				{
					point2 = pElement2->m_rect.TopLeft();
				}
				else
				{
					point2 = pElement2->GetHandle(handle);
				}
			}

			CRect rect(point1, point2);
			pElement->m_rect = rect;
		}
	}

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
			//std::shared_ptr<CElement> pTextElement(new CSimpleTextElement());
			//pTextElement->m_rect = pElement->m_rect;
			//pTextElement->m_text = pElement->m_text;
			//pTextElement->m_textAlign = pElement->m_textAlign;
			//pTextElement->Draw(ctxt);
			std::shared_ptr<CElement> pTextElement(new CTextElement());
			//pTextElement->m_name = pElement->m_name;
			//pTextElement->m_objectId = pElement->m_objectId;
			pTextElement->m_caption = pElement->m_caption;
			pTextElement->m_text = pElement->m_text;
			pTextElement->m_code = pElement->m_code;
			pTextElement->m_image = pElement->m_image;
			pTextElement->m_lineWidth = pElement->m_lineWidth;
			pTextElement->m_pManager = pElement->m_pManager;
			pTextElement->m_pView = pElement->m_pView;
			pTextElement->m_rect = pElement->m_rect;
			pTextElement->m_bColorFill = pElement->m_bColorFill;
			pTextElement->m_bColorLine = pElement->m_bColorLine;
			pTextElement->m_bLineWidth = pElement->m_bLineWidth;
			pTextElement->m_bSolidColorFill = pElement->m_bSolidColorFill;
			pTextElement->m_colorFill = pElement->m_colorFill;
			pTextElement->m_colorLine = pElement->m_colorLine;
			pTextElement->m_textAlign = pElement->m_textAlign;
			pTextElement->m_fontName = pElement->m_fontName;
			pTextElement->m_bFixed = pElement->m_bFixed;
			pTextElement->m_bBold = pElement->m_bBold;
			pTextElement->m_bItalic = pElement->m_bItalic;
			pTextElement->m_bUnderline = pElement->m_bUnderline;
			pTextElement->m_bStrikeThrough = pElement->m_bStrikeThrough;
			//pTextElement->m_code = pElement->m_code;
			pTextElement->m_fontSize = pElement->m_fontSize;
			pTextElement->m_colorText = pElement->m_colorText;
			pTextElement->Draw(ctxt);
		}

		//if( !pDC->IsPrinting() && IsSelected(pObj) )
		//	DrawTracker(pObj, pDC, TrackerState::selected);
		if (pView != NULL && pView->m_bActive && !pDC->IsPrinting() && IsSelected(pElement))
			pElement->DrawTracker(ctxt, TrackerState::selected);
	}

	// Last....
	// Add connector shape to the handles
	for (vector<std::shared_ptr<CElement>>::const_iterator i = GetObjects().begin(); i != GetObjects().end(); i++)
	{
		std::shared_ptr<CElement> pElement = *i;

		if (pElement->IsLine() == false)
		{
			continue;
		}

		DrawConnector(graphics, pElement, ConnectorType::connector1);
		DrawConnector(graphics, pElement, ConnectorType::connector2);

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
				
				CString str;
				str.Format(_T("m_nDragHandle=%d - selectMode == sized"), m_nDragHandle);
				pView->LogDebug(str);
				//pView->LogDebug(_T("selectMode == sized"));
			}
		}

		if( m_selectMode == SelectMode::none )
		{
			// See if the click was on an object
			std::shared_ptr<CElement> pElement = m_objects.ObjectAt(point, m_selectType);
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

#ifdef VERSION_COMMUNITY
		if (CFactory::g_counter > MAX_SHAPES)
		{
			AfxMessageBox(_T("Maximum number or shapes reached !\nFor more, please buy the Architect Edition."));
			return;
		}
#endif

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

		m_nDragHandle == 1;
		FindAConnectionFor(pNewElement, point, pView, ConnectorType::connector1);

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
					FindAConnectionFor(pElement, point, pView, ConnectorType::connector2);
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
			FindAConnectionFor(pElement, point, pView, ConnectorType::connector2);
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

		// remove the selection element from the objects list
		vector<std::shared_ptr<CElement>>::iterator pos;
		pos = find(m_objects.m_objects.begin(), m_objects.m_objects.end(), pSelectionElement);
		if (pos != m_objects.m_objects.end())
		{
			m_objects.m_objects.erase(pos);
		}

		// remove the selection element from the selection list
		vector<std::shared_ptr<CElement>>::iterator pos2;
		pos2 = find(m_selection.m_objects.begin(), m_selection.m_objects.end(), pSelectionElement);
		if (pos2 != m_selection.m_objects.end())
		{
			m_selection.m_objects.erase(pos2);
		}

		//ViewToManager(pView, rect);
		SelectNone();

		vector<std::shared_ptr<CElement>> v = m_objects.ObjectsInRectEx(rect, m_selectType); // version Ex : do not select lines with full connector
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

	m_bSizingALine = false;

	// Set selectType to default
	m_selectType = SelectType::intuitive;

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

	if (name == prop_Connector1Handle)
	{
		if (value == _T("") || value == _T("TopLeft") || value == _T("Center") || value == _T("TopCenter")
			|| value == _T("BottomCenter") || value == _T("LeftCenter") || value == _T("RightCenter"))
		{
			pElement->m_connectorDragHandle1 = pElement->DragHandleFromString(value);
		}
	}

	if (name == prop_Connector2Handle)
	{
		if (value == _T("") || value == _T("TopLeft") || value == _T("Center") || value == _T("TopCenter")
			|| value == _T("BottomCenter") || value == _T("LeftCenter") || value == _T("RightCenter"))
		{
			pElement->m_connectorDragHandle2 = pElement->DragHandleFromString(value);
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

	if (name == prop_Document)
	{
		pElement->m_document = value;
	}

	if (name == prop_Document_Type)
	{
		if (value == _T("None") || value == _T("File") || value == _T("Folder") || value == _T("Diagram"))
		{
			pElement->m_documentType = pElement->FromString(value);
		}
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
	for (int y = 0; y < 30; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (count%400 == c)
			{
				pNewElement->m_point.x = 175 * x;
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

CString CElementManager::SearchDrive(const CString& strFile, const CString& strFilePath, const bool& bRecursive, const bool& bStopWhenFound)
{
	USES_CONVERSION;

	CWnd* pWnd = AfxGetMainWnd();
	CMainFrame* pMainFrame = (CMainFrame*)pWnd;

	CString strFoundFilePath;
	WIN32_FIND_DATA file;

	CString strPathToSearch = strFilePath;
	strPathToSearch += _T("\\");

	HANDLE hFile = FindFirstFile((strPathToSearch + "*"), &file);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			CString strTheNameOfTheFile = file.cFileName;

			// It could be a directory we are looking at
			// if so look into that dir
			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//if ((strTheNameOfTheFile != ".") && (strTheNameOfTheFile != ".."))
				{
					// ADD TO COLLECTION TYPE
					std::shared_ptr<CCodeFile> cf = std::make_shared<CCodeFile>();
					cf->_type = FileType::folder;
					cf->_name = T2W((LPTSTR)(LPCTSTR)(strTheNameOfTheFile));
					cf->_path = T2W((LPTSTR)(LPCTSTR)(strPathToSearch + strTheNameOfTheFile));
					_files.push_back(cf);
				}

				if ((strTheNameOfTheFile != ".") && (strTheNameOfTheFile != "..") && (bRecursive))
				{
					strFoundFilePath = SearchDrive(strFile, strPathToSearch + strTheNameOfTheFile, bRecursive, bStopWhenFound);

					if (!strFoundFilePath.IsEmpty() && bStopWhenFound)
						break;
				}		
			}
			else
			{
				//if (strTheNameOfTheFile == strFile)
				{
					strFoundFilePath = strPathToSearch + strTheNameOfTheFile; //strFile;

					// ADD TO COLLECTION TYPE
					std::shared_ptr<CCodeFile> cf = std::make_shared<CCodeFile>();
					cf->_type = FileType::file;
					cf->_name = T2W((LPTSTR)(LPCTSTR)strTheNameOfTheFile); //strFile;
					cf->_path = T2W((LPTSTR)(LPCTSTR)strFoundFilePath);
					_files.push_back(cf);

					if (bStopWhenFound)
						break;
				}
			}
		} while (FindNextFile(hFile, &file));

		FindClose(hFile);
	}

	return strFoundFilePath;
}

wstring GetFileContent(wstring filename)
{
	ifstream file(filename);
	string str;
	string file_contents;
	while (std::getline(file, str))
	{
		file_contents += str + string("\r\n");
	}
	wstring ws(file_contents.begin(), file_contents.end());
	return ws;
}

wstring GetFileContent(shared_ptr<CCodeFile> codeFile)
{
	return GetFileContent(codeFile->_path);
}


void CElementManager::LoadModule(CModeler1View * pView)
{
	CFolderPickerDialog dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;

	CString strPath = dlg.GetFolderPath();
	_files.clear();
	SearchDrive(_T("*.*"), strPath, false, false);

	int count = 0;
	for (shared_ptr<CCodeFile> file : _files)
	{
		std::shared_ptr<CElement> pNewElement = CFactory::CreateElementOfType(ElementType::type_shapes_development, ShapeType::development_class);
		CalcAutoPointRect(count, pNewElement);
		pNewElement->m_pManager = this;
		pNewElement->m_pView = pView;
		pNewElement->m_text = file->_name;
		// Read file content
		pNewElement->m_code = GetFileContent(file);
		pNewElement->m_documentType = DocumentType::document_file;
		pNewElement->m_documentTypeText = _T("File");

		// Add an object
		m_objects.AddTail(pNewElement);
		pView->LogDebug(_T("object created ->") + pNewElement->ToString());

		++count;
	}

	Invalidate(pView);
}

void CElementManager::LoadFolders(CModeler1View* pView)
{
	CFolderPickerDialog dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;

	CString strPath = dlg.GetFolderPath();
	_files.clear();
	SearchDrive(_T("*.*"), strPath, false, false);

	int count = 0;
	for (shared_ptr<CCodeFile> file : _files)
	{
		std::shared_ptr<CElement> pNewElement = nullptr;
		if (file->_type == FileType::file)
		{
			pNewElement = CFactory::CreateElementOfType(ElementType::type_shapes_development, ShapeType::development_class);
			pNewElement->m_documentType = DocumentType::document_file;
			pNewElement->m_documentTypeText = _T("File");
		}
		else
		{
			pNewElement = CFactory::CreateElementOfType(ElementType::type_shapes_development, ShapeType::development_interface);
			pNewElement->m_documentType = DocumentType::document_folder;
			pNewElement->m_documentTypeText = _T("Folder");
		}
		
		CalcAutoPointRect(count, pNewElement);
		pNewElement->m_pManager = this;
		pNewElement->m_pView = pView;
		pNewElement->m_text = file->_name;
		// Read file content
		//pNewElement->m_code = GetFileContent(file);
		pNewElement->m_document = file->_path;

		// Add an object
		m_objects.AddTail(pNewElement);
		pView->LogDebug(_T("object created ->") + pNewElement->ToString());

		++count;
	}

	Invalidate(pView);
}

void CElementManager::OpenFolder(CModeler1View* pView)
{
	shared_ptr<CElement> pElement = m_selection.GetHead();

	CString strPath = pElement->m_document.c_str();;
	_files.clear();
	SearchDrive(_T("*.*"), strPath, false, false);

	m_objects.RemoveAll();
	Invalidate(pView);

	int count = 0;
	for (shared_ptr<CCodeFile> file : _files)
	{
		std::shared_ptr<CElement> pNewElement = nullptr;
		if (file->_type == FileType::file)
		{
			pNewElement = CFactory::CreateElementOfType(ElementType::type_shapes_development, ShapeType::development_class);
			pNewElement->m_documentType = DocumentType::document_file;
			pNewElement->m_documentTypeText = _T("File");
		}
		else
		{
			pNewElement = CFactory::CreateElementOfType(ElementType::type_shapes_development, ShapeType::development_interface);
			pNewElement->m_documentType = DocumentType::document_folder;
			pNewElement->m_documentTypeText = _T("Folder");
		}

		CalcAutoPointRect(count, pNewElement);
		pNewElement->m_pManager = this;
		pNewElement->m_pView = pView;
		pNewElement->m_text = file->_name;
		// Read file content
		//pNewElement->m_code = GetFileContent(file);
		pNewElement->m_document = file->_path;

		// Add an object
		m_objects.AddTail(pNewElement);
		pView->LogDebug(_T("object created ->") + pNewElement->ToString());

		++count;
	}

	Invalidate(pView);
}

void CElementManager::OpenFile(CModeler1View* pView)
{

	shared_ptr<CElement> pElement = m_selection.GetHead();
	::ShellExecuteW(NULL, NULL, pElement->m_document.c_str(), NULL/*lpszArgs*/, NULL, SW_SHOW);
}

void CElementManager::OpenFileContent(CModeler1View* pView)
{

	shared_ptr<CElement> pElement = m_selection.GetHead();
	pElement->m_code = GetFileContent(pElement->m_document);
	
	CRuntimeClass* prt = RUNTIME_CLASS(CTabbedView); // CModeler1SourceView);
	CView* pview = NULL;
	// Continue search in inactive View by T(o)m
	CModeler1Doc* pDoc = pView->GetDocument();
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		pview = pDoc->GetNextView(pos);
		CRuntimeClass* pRT = pview->GetRuntimeClass();
		
		if( prt = pRT)
		{
			CTabbedView* pTView = (CTabbedView*)pview;
			pTView->SetActiveView(1);
			break;
		}
		pView = NULL;       // not valid vie
	}
}

void CElementManager::SetConnector(std::shared_ptr<CElement> pLineElement, std::shared_ptr<CElement> pElementFound, ConnectorType connector)
{
	/*
	pLineElement->m_connectorDragHandle2 = 2;

	shared_ptr<CElement> pElement = nullptr;
	pElement = pElementFound;

	// Connect to the right connector
	CPoint pointLine1 = pLineElement->m_rect.TopLeft();
	CPoint pointLine2 = pLineElement->m_rect.BottomRight();

	CPoint pointElement1 = pElement->m_rect.TopLeft();
	CPoint pointElement2 = pElement->m_rect.CenterPoint();
	CPoint pointElementCenter = pElement->m_rect.TopLeft();
	CRect re = pElement->m_rect;
	int handle = 2;
	*/

	/*
	if (pointLine2.x < re.TopLeft().x && pointLine2.y < re.TopLeft().y)
	{
		// haut centre
		handle = 2;
	}
	else if (pointLine2.x < re.CenterPoint().x && pointLine2.y < re.TopLeft().y)
	{
		// milieu gauche
		handle = 8;
	}
	else if (pointLine2.x < re.CenterPoint().x && pointLine2.y < re.BottomRight().y)
	{
		// bas centre
		handle = 6;
	}

	if (connector == ConnectorType::connector1)
	{
		pLineElement->m_connectorDragHandle1 = handle;
	}
	else
	{
		pLineElement->m_connectorDragHandle2 = handle;
	}
	*/

	/*
	CPoint point1 = pLineElement->m_rect.TopLeft();
	CPoint point2;
	if (connector == ConnectorType::connector1)
	{
		point2 = pLineElement->m_pConnector->m_pElement1->GetHandle(handle);
	}
	else
	{
		point2 = pLineElement->m_pConnector->m_pElement2->GetHandle(handle);
	}
	CRect rect(point1, point2);
	rect.NormalizeRect();
	pLineElement->m_rect = rect;
	*/

	/*
	shared_ptr<CElement> pElement = pLineElement;
	shared_ptr<CElement> pElement1 = pLineElement->m_pConnector->m_pElement1;
	shared_ptr<CElement> pElement2 = pLineElement->m_pConnector->m_pElement2;
	CPoint point1;

	if (pElement1 == nullptr)
	{
		point1 = pElement->m_rect.TopLeft();
	}
	else
	{
		//point1 = pElement1->m_rect.CenterPoint();
		int handle = pElement->m_connectorDragHandle1;
		if (handle == 0)
		{
			point1 = pElement1->m_rect.TopLeft();
		}
		else
		{
			point1 = pElement1->GetHandle(handle);
		}
	}
	*/
}

void CElementManager::FindAConnectionFor(std::shared_ptr<CElement> pLineElement, CPoint point, CModeler1View* pView, ConnectorType connector)
{
	// Find a connection ?
	if (pLineElement->IsLine() == true)
	{
		m_bSizingALine = true;

		SelectNone();
		Select(pLineElement);
		std::shared_ptr<CElement> pElement = m_objects.ObjectExceptLinesAt(point, pLineElement);
		if (pElement != NULL)
		{
			CClientDC dc(pView);
			Graphics graphics(dc.m_hDC);
			graphics.ScaleTransform(m_fZoomFactor, m_fZoomFactor);
			SolidBrush solidBrush(Color::Yellow);
			CRect rect = pElement->m_rect;
			graphics.FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());

			// Register the connector
			// if start, we take only the first connector in handle
			if (connector == ConnectorType::connector1 || m_nDragHandle == 1)
			{
				pLineElement->m_pConnector->m_pElement1 = pElement;
				pLineElement->m_connectorDragHandle1 = 2;

				// Connect to the right connector
				SetConnector(pLineElement, pElement, ConnectorType::connector1);
			}
			else if (connector == ConnectorType::connector2 || m_nDragHandle == 2)
			{
				pLineElement->m_pConnector->m_pElement2 = pElement;
				pLineElement->m_connectorDragHandle2 = 2;

				// Connect to the right connector
				SetConnector(pLineElement, pElement, ConnectorType::connector2);
			}
		}
		else
		{
			// Register no connector
			if (connector == ConnectorType::connector1 || m_nDragHandle == 1 )
			{
				pLineElement->m_pConnector->m_pElement1 = nullptr;
			}
			else if (connector == ConnectorType::connector2 || m_nDragHandle == 2)
			{
				pLineElement->m_pConnector->m_pElement2 = nullptr;
			}
		}
	}
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
		pNewElement->m_code = pElement->m_code;
		pNewElement->m_image = pElement->m_image;
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
		pNewElement->m_textAlign = pElement->m_textAlign;
		pNewElement->m_fontName = pElement->m_fontName;
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

		UpdatePropertyGrid(pView, pElementBase);

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

		UpdatePropertyGrid(pView, pElementBase);

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

		UpdatePropertyGrid(pView, pElementBase);

		pView->GetDocument()->SetModifiedFlag();
	}
}

void CElementManager::OnEditGroup(CModeler1View* pView)
{
	static int count = 0;
	//AfxMessageBox(L"Grouping");

	++count;

	wstringstream ss;
	ss << _T("Group_") << count;

	shared_ptr<CElementGroup> speg = make_shared<CElementGroup>();
	for (vector<std::shared_ptr<CElement>>::const_iterator itSel = m_selection.m_objects.begin(); itSel != m_selection.m_objects.end(); itSel++)
	{
		std::shared_ptr<CElement> pElement = *itSel;
		pElement->m_pElementGroup = speg;
		pElement->m_bGrouping = true;
		speg->m_name = ss.str();
		speg->m_Groups.push_back(pElement);
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

std::vector<std::wstring> CElementManager::Split(const std::wstring& s, wchar_t delim)
{
	std::wstringstream ss(s);
	std::wstring item;
	std::vector<std::wstring> elems;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
		// elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
	}
	return elems;
}

void CElementManager::BuildGroups()
{
	wstring n = CElement::m_elementGroupNames;
	wstring elts = CElement::m_elementGroupElements;
	vector<wstring> vnames = CElementManager::Split(n, _T('|'));
	vector<wstring> vlistes = CElementManager::Split(elts, _T('|'));

	//for (int i = 0; i < vnames.size(); ++i)
	//{
	//	wstring aName = vnames[i];
	//	if (aName.size() == 0)
	//	{
	//		continue;
	//	}
	//}

	for (int j = 0; j < vlistes.size(); ++j)
	{

		wstring aName = vnames[j];
		wstring aList = vlistes[j];
		if (aList.size() <= 2)
		{
			continue;
		}

		shared_ptr<CElementGroup> speg = make_shared<CElementGroup>();

		vector<wstring> velements = CElementManager::Split(aList, _T(';'));
		for (int x = 0; x < velements.size(); ++x)
		{
			wstring element = velements[x];
			if (element.size() <= 2)
			{
				continue;
			}

			std::shared_ptr<CElement> pElement = this->m_objects.FindElementByName(element);
			if (pElement != nullptr)
			{
				pElement->m_pElementGroup = speg;
				pElement->m_bGrouping = true;
				speg->m_Groups.push_back(pElement);
			}
		}

		this->m_groups.push_back(speg);
	}

}

int CElementManager::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

void CElementManager::OnFileExportPNG(CModeler1View* pView)
{
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() != IDOK)
		return;
	CStringW fileName = dlg.GetPathName();
	wstring doc = (LPTSTR)(LPCTSTR)fileName;

	Bitmap myBitmap(m_size.cx, m_size.cy, PixelFormat32bppARGB);
	Graphics graphics(&myBitmap);

	// just like that
	//graphics.ScaleTransform(0.75f, 0.75f);
	graphics.ScaleTransform(m_fZoomFactor, m_fZoomFactor);

	// TODO: add draw code for native data here
	for (vector<std::shared_ptr<CElement>>::const_iterator i = GetObjects().begin(); i != GetObjects().end(); i++)
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
		if (pElement->m_text.empty() == false &&
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

	}

	// Save bitmap (as a png)
	CLSID pngClsid;
	int result = GetEncoderClsid(L"image/png", &pngClsid);
	myBitmap.Save(doc.c_str(), &pngClsid, NULL);
}

void CElementManager::OnSelectAll(CModeler1View* pView)
{
	for (vector<std::shared_ptr<CElement>>::const_iterator itSel = m_objects.m_objects.begin(); itSel != m_objects.m_objects.end(); itSel++)
	{
		std::shared_ptr<CElement> pElement = *itSel;
		Select(pElement);
	}

	Invalidate(pView);
}

void CElementManager::OnFontBold(CModeler1View* pView)
{
	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	if (pElement->m_bBold == true)
	{
		pElement->m_bBold = false;
	}
	else
	{
		pElement->m_bBold = true;
	}

	UpdatePropertyGrid(pView, pElement);

	// Redraw the element
	InvalObj(pView, pElement);
}

void CElementManager::OnFontItalic(CModeler1View* pView)
{
	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	if (pElement->m_bItalic == true)
	{
		pElement->m_bItalic = false;
	}
	else
	{
		pElement->m_bItalic = true;
	}

	UpdatePropertyGrid(pView, pElement);

	// Redraw the element
	InvalObj(pView, pElement);
}

void CElementManager::OnFontUnderline(CModeler1View* pView)
{
	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	if (pElement->m_bUnderline == true)
	{
		pElement->m_bUnderline = false;
	}
	else
	{
		pElement->m_bUnderline = true;
	}

	UpdatePropertyGrid(pView, pElement);

	// Redraw the element
	InvalObj(pView, pElement);
}

void CElementManager::OnFontStrikeThrough(CModeler1View* pView)
{
	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	if (pElement->m_bStrikeThrough == true)
	{
		pElement->m_bStrikeThrough = false;
	}
	else
	{
		pElement->m_bStrikeThrough = true;
	}

	UpdatePropertyGrid(pView, pElement);

	// Redraw the element
	InvalObj(pView, pElement);
}

void CElementManager::OnFontGrowFont(CModeler1View* pView)
{
	CMFCRibbonBar* pRibbon = ((CMainFrame*)pView->GetTopLevelFrame())->GetRibbonBar();

	CMFCRibbonComboBox* pFontCombo = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pRibbon->FindByID(ID_FONT_FONTSIZE));
	if (pFontCombo == NULL)
	{
		return;
	}

	CString fontSize = pFontCombo->GetEditText();
	int iFontSize = _ttoi(fontSize);

	iFontSize += 2;

	if (iFontSize > 60)
	{
		return;
	}

	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	pElement->m_fontSize = iFontSize;

	TCHAR sz[255];
	_stprintf_s(sz, _T("%d"), pElement->m_fontSize);
	pFontCombo->SelectItem(sz);

	UpdatePropertyGrid(pView, pElement);

	// Redraw the element
	InvalObj(pView, pElement);
}

void CElementManager::OnFontShrink(CModeler1View* pView)
{
	CMFCRibbonBar* pRibbon = ((CMainFrame*)pView->GetTopLevelFrame())->GetRibbonBar();

	CMFCRibbonComboBox* pFontCombo = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pRibbon->FindByID(ID_FONT_FONTSIZE));
	if (pFontCombo == NULL)
	{
		return;
	}

	CString fontSize = pFontCombo->GetEditText();
	int iFontSize = _ttoi(fontSize);

	iFontSize -= 2;

	if (iFontSize < 8)
	{
		return;
	}

	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	pElement->m_fontSize = iFontSize;

	TCHAR sz[255];
	_stprintf_s(sz, _T("%d"), pElement->m_fontSize);
	pFontCombo->SelectItem(sz);

	UpdatePropertyGrid(pView, pElement);

	// Redraw the element
	InvalObj(pView, pElement);
}

void CElementManager::OnFontClearFormat(CModeler1View* pView)
{
	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	pElement->m_fontName = _T("Calibri");
	pElement->m_fontSize = 12;
	pElement->m_bBold = false;
	pElement->m_bItalic = false;
	pElement->m_bUnderline = false;
	pElement->m_bStrikeThrough = false;
	pElement->m_colorText = RGB(0, 0, 0);
	pElement->m_colorFill = RGB(255, 255, 255);
	pElement->m_bColorFill = true;
	pElement->m_bColorLine = false;
	pElement->m_bSolidColorFill = true;

	UpdatePropertyGrid(pView, pElement);
	UpdateRibbonUI(pView, pElement);

	// Redraw the element
	InvalObj(pView, pElement);
}

void CElementManager::OnFontColor(CModeler1View* pView)
{
	CMFCRibbonBar* pRibbon = ((CMainFrame*)pView->GetTopLevelFrame())->GetRibbonBar();

	COLORREF color = ((CMainFrame*)AfxGetMainWnd())->GetColorFromColorButton(ID_FONT_COLOR);

	if (color == (COLORREF)-1)
	{
		return;
	}

	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	pElement->m_colorText = color;

	// Redraw the element
	InvalObj(pView, pElement);
}

void CElementManager::OnFontTextHighlight(CModeler1View* pView)
{
	CMFCRibbonBar* pRibbon = ((CMainFrame*)pView->GetTopLevelFrame())->GetRibbonBar();

	COLORREF color = ((CMainFrame*)AfxGetMainWnd())->GetColorFromColorButton(ID_FONT_TEXTHIGHLIGHT);

	if (color == (COLORREF)-1)
	{
		return;
	}

	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	pElement->m_colorFill = color;
	pElement->m_bColorFill = true;
	pElement->m_bColorLine = false;
	pElement->m_bSolidColorFill = true;

	UpdatePropertyGrid(pView, pElement);

	// Redraw the element
	InvalObj(pView, pElement);
}

void CElementManager::OnFontChangeCase(CModeler1View* pView)
{
	static int count = 0;

	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	wstring text = pElement->m_text;

	++count;

	if (count % 2 == 0)
	{
		transform(text.begin(), text.end(), text.begin(), tolower);
	}
	else
	{
		transform(text.begin(), text.end(), text.begin(), toupper);
	}

	pElement->m_text = text;

	UpdatePropertyGrid(pView, pElement);

	// Redraw the element
	InvalObj(pView, pElement);
}

void CElementManager::OnActionElements(CModeler1View* pView)
{
	CWnd* p = AfxGetMainWnd();
	CMainFrame* pmf = (CMainFrame*)p;
	pmf->OnActionElements(pView);
}

void CElementManager::BuildElementsCombo(CModeler1View* pView)
{
	CWnd* p = AfxGetMainWnd();
	CMainFrame* pmf = (CMainFrame*)p;
	pmf->BuildElementsCombo(pView);
}

void CElementManager::OnDesignDeconnect(CModeler1View* pView)
{
	std::shared_ptr<CElement> pElement = m_selection.GetHead();
	pElement->m_pConnector->m_pElement1 = nullptr;
	pElement->m_pConnector->m_pElement2 = nullptr;
}

