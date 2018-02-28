// DrawingElement.cpp : implementation file
//

#include "stdafx.h"
#include "Modeler1.h"
#include "DrawingElements.h"
#include "Modeler1View.h"


// CLineElement class

//CLineElement::CLineElement()
//{
//}
//
//CLineElement::~CLineElement()
//{
//}

void CLineElement::Draw(CDrawingContext & ctxt)
{
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	CPoint & p1 = ctxt.GetTopLeft();
	CPoint & p2 = ctxt.GetBottomRight();

	if( m_shapeType == ShapeType::line ||  m_shapeType == ShapeType::line2 )
	{
		graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
	}
	else if( m_shapeType == ShapeType::line_right || m_shapeType == ShapeType::line_right2 )
	{
		//colorPen.SetEndCap(LineCapArrowAnchor);
		//graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);

		//Create a Path, and add two lines to it.
		Point points[3] = {Point(-5, -5), Point(0, 0), Point(5, -5)};
		GraphicsPath capPath;
		capPath.AddLines(points, 3);

		// Create a CustomLineCap object.
		CustomLineCap custCap(NULL, &capPath); 
   
		// Set the start and end caps for custCap.
		custCap.SetStrokeCap(LineCapTriangle);

		// Create a Pen object, assign startStrokeCap and endStrokeCap as the 
		// start and end caps, and draw a line.
		colorPen.SetCustomEndCap(&custCap);
		graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
	}
	else if( m_shapeType == ShapeType::line_left_right )
	{
		colorPen.SetStartCap(LineCapArrowAnchor);
		colorPen.SetEndCap(LineCapArrowAnchor);
		graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
	}
}

//
// CRectangleElement class
//
void CRectangleElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush & lgBrush = ctxt.GetGradientBrushColor();

	if( HasColorFill() )
	{
		if( IsSolidColorFill() )
			graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		else
			graphics->FillRectangle(&lgBrush, rect.left, rect.top, rect.Width(), rect.Height());
	}
	if( HasColorLine() )
	{
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	}
}

//
// CTriangleElement class
//
void CTriangleElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush & lgBrush = ctxt.GetGradientBrushColor();

	Point points[3] = {Point(rect.left + rect.Width()/2, rect.top), 
					Point(rect.right, rect.bottom), 
					Point(rect.left, rect.bottom)};
	if( HasColorFill() )
	{
		if( IsSolidColorFill() )
			graphics->FillPolygon(&solidBrush, points, 3);
		else
			graphics->FillPolygon(&lgBrush, points, 3);
	}
	if( HasColorLine() )
	{
		graphics->DrawPolygon(&colorPen, points, 3);
	}
}

//
// CEllipseElement class
//
void CEllipseElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush & lgBrush = ctxt.GetGradientBrushColor();

	if( HasColorFill() )
	{
		if( IsSolidColorFill() )
			graphics->FillEllipse(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		else
			graphics->FillEllipse(&lgBrush, rect.left, rect.top, rect.Width(), rect.Height());
	}
	if( HasColorLine() )
	{
		graphics->DrawEllipse(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	}
}

//
// CArrowRightElement class
//
void CArrowRightElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush & lgBrush = ctxt.GetGradientBrushColor();

	// * * * * 3 * * * * *
	// 1 - - - 2 * * * * *
	// - * * * * * * * * 4
	// 7 - - - 6 * * * * *
	// * * * * 5 * * * * *

	int step1_5_x = rect.Width() / 5;
	int step1_5_y = rect.Height() / 5;
	Point points[7] = {	Point(rect.left, rect.top + step1_5_y), 
						Point(rect.left + step1_5_x*3, rect.top + step1_5_y), 
						Point(rect.left + step1_5_x*3, rect.top), 
						Point(rect.right, rect.top + rect.Height()/2),
						Point(rect.left + step1_5_x*3, rect.bottom), 
						Point(rect.left + step1_5_x*3, rect.bottom - step1_5_y),
						Point(rect.left, rect.bottom - step1_5_y)};
	int npoints = 7;
	if( HasColorFill() )
	{
		if( IsSolidColorFill() )
			graphics->FillPolygon(&solidBrush, points, npoints);
		else
			graphics->FillPolygon(&lgBrush, points, npoints);
	}
	if( HasColorLine() )
	{
		graphics->DrawPolygon(&colorPen, points, npoints);
	}
}

//
// CArrowDownElement class
//
void CArrowDownElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush & lgBrush = ctxt.GetGradientBrushColor();

	// * * 1 * * * 2 * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// 6 * 7 * * * 3 * * 4
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * 5 * * * * *

	int step1_5_x = rect.Width() / 5;
	int step1_5_y = rect.Height() / 5;
	Point points[7] = {	/*1*/ Point(rect.left + step1_5_x*1, rect.top), 
						/*2*/ Point(rect.left + step1_5_x*4, rect.top), 
						/*3*/ Point(rect.left + step1_5_x*4, rect.top + step1_5_y*3), 
						/*4*/ Point(rect.right, rect.top + step1_5_y*3),
						/*5*/ Point(rect.left + rect.Width()/2, rect.bottom), 
						/*6*/ Point(rect.left, rect.top + step1_5_y*3),
						/*7*/ Point(rect.left + step1_5_x*1, rect.top + step1_5_y*3)};

	int npoints = 7;
	if( HasColorFill() )
	{
		if( IsSolidColorFill() )
			graphics->FillPolygon(&solidBrush, points, npoints);
		else
			graphics->FillPolygon(&lgBrush, points, npoints);
	}
	if( HasColorLine() )
	{
		graphics->DrawPolygon(&colorPen, points, npoints);
	}
}

//
// CStarElement class
//
void CStarElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush & lgBrush = ctxt.GetGradientBrushColor();

	// * * * * 1 * * * * *
	// * * * * * * * * * *
	// 3 * 2 * * * *10 * 9
	// * * * * * * * * * *
	// * * 4 * * * * 8 * *
	// * * * * * * * * * *
	// * * * * 6 * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// 5 * * * * * * * * 7

	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;
	Point points[10] = {/*1*/ Point(rect.left + rect.Width()/2, rect.top), 
						/*2*/ Point(rect.left + step1_10_x*3, rect.top + step1_10_y*3), 
						/*3*/ Point(rect.left, rect.top + step1_10_y*3), 
						/*4*/ Point(rect.left + step1_10_x*3, rect.top + step1_10_y*5), 
						/*5*/ Point(rect.left, rect.bottom), 							
						/*6*/ Point(rect.left + rect.Width()/2, rect.top + step1_10_y*7), 
						/*7*/ Point(rect.right, rect.bottom),
						/*8*/ Point(rect.left + step1_10_x*7, rect.top + step1_10_y*5), 
						/*9*/ Point(rect.right, rect.top + step1_10_y*3), 
						/*10*/Point(rect.left + step1_10_x*7, rect.top + step1_10_y*3)};
	int npoints = 10;
	if( HasColorFill() )
	{
		if( IsSolidColorFill() )
			graphics->FillPolygon(&solidBrush, points, npoints);
		else
			graphics->FillPolygon(&lgBrush, points, npoints);
	}
	if( HasColorLine() )
	{
		graphics->DrawPolygon(&colorPen, points, npoints);
	}
}

//
// CParentheseLeftElement class
//
void CParentheseLeftElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush & lgBrush = ctxt.GetGradientBrushColor();

	// * * * * * 1 * * * *
	// * 2 * * * * * * * *
	// * * * * * * * * * *
	// * 3 * * * * * * * *
	// 4 * * * * * * * * *
	// * 5 * * * * * * * *
	// * * * * * * * * * *
	// * 6 * * * * * * * *
	// * * * * * 7 * * *
	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;
	Point points[7] = {/*1*/ Point(rect.left + step1_10_x*5, rect.top), 
						/*2*/ Point(rect.left + step1_10_x*1, rect.top + step1_10_y*1), 
						/*3*/ Point(rect.left + step1_10_x*1, rect.top + step1_10_y*4), 
						/*4*/ Point(rect.left + step1_10_x*0, rect.top + step1_10_y*5), 
						/*5*/ Point(rect.left + step1_10_x*1, rect.top + step1_10_y*6), 							
						/*6*/ Point(rect.left + step1_10_x*1, rect.top + step1_10_y*9), 
						/*7*/ Point(rect.left + step1_10_x*5, rect.bottom /*top + step1_10_y*9*/)};
	int npoints = 7;
	graphics->DrawCurve(&colorPen, points, npoints);
}

//
// CParentheseRightElement class
//
void CParentheseRightElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush & lgBrush = ctxt.GetGradientBrushColor();

	// * * * * 1 * * * * *
	// * * * * * * * * 2 *
	// * * * * * * * * * *
	// * * * * * * * * 3 *
	// * * * * * * * * * 4
	// * * * * * * * * 5 *
	// * * * * * * * * * *
	// * * * * * * * * 6 *
	// * * * * 7 * * * * *
	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;
	Point points[7] = {/*1*/ Point(rect.left + step1_10_x*5, rect.top), 
						/*2*/ Point(rect.left + step1_10_x*9, rect.top + step1_10_y*1), 
						/*3*/ Point(rect.left + step1_10_x*9, rect.top + step1_10_y*4), 
						/*4*/ Point(rect.right /*.left + step1_10_x*10*/, rect.top + step1_10_y*5), 
						/*5*/ Point(rect.left + step1_10_x*9, rect.top + step1_10_y*6), 							
						/*6*/ Point(rect.left + step1_10_x*9, rect.top + step1_10_y*9), 
						/*7*/ Point(rect.left + step1_10_x*5, rect.bottom /*top + step1_10_y*9*/)};
	int npoints = 7;
	graphics->DrawCurve(&colorPen, points, npoints);
}

//
// CLineBrokenElement class
//
void CLineBrokenElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush & lgBrush = ctxt.GetGradientBrushColor();

	if( m_shapeType == ShapeType::line_broken ||  m_shapeType == line_broken_right )
	{
		// 1 * * * 2 * * * * *
		// * * * * * * * * * *
		// * * * * * * * * * *
		// * * * * * * * * * *
		// * * * * 3 * * * * 4
		int step1_5_x = rect.Width() / 5;
		int step1_5_y = rect.Height() / 5;
		Point points[4] = {	Point(rect.left, rect.top), 
							Point(rect.left + rect.Width()/2, rect.top), 
							Point(rect.left + rect.Width()/2, rect.bottom), 
							Point(rect.right, rect.bottom)};
		int npoints = 4;

		if( m_shapeType == line_broken_right )
		{
			//colorPen.SetEndCap(LineCapArrowAnchor);
			//Create a Path, and add two lines to it.
			Point points[3] = {Point(-5, -5), Point(0, 0), Point(5, -5)};
			GraphicsPath capPath;
			capPath.AddLines(points, 3);
			// Create a CustomLineCap object.
			CustomLineCap custCap(NULL, &capPath); 
			// Set the start and end caps for custCap.
			custCap.SetStrokeCap(LineCapTriangle);
			// Create a Pen object, assign startStrokeCap and endStrokeCap as the 
			// start and end caps, and draw a line.
			colorPen.SetCustomEndCap(&custCap);
		}
			
		graphics->DrawLines(&colorPen, points, npoints);
	}
}

//
// CDevelopmentElement class
//
void CDevelopmentElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();

	if( m_shapeType == ShapeType::development_class )
	{
		graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	}
	if( m_shapeType == ShapeType::development_interface )
	{
		graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	}
	if( m_shapeType == ShapeType::development_enumeration )
	{
		graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	}
	if( m_shapeType == ShapeType::development_package )
	{
		int step1_4_x = rect.Width() / 4;

		// Draw little rectangle			
		graphics->FillRectangle(&solidBrush, rect.left, rect.top, step1_4_x*3, 30);
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, step1_4_x*3, 30);
		// Draw large rectangle
		graphics->FillRectangle(&solidBrush, rect.left, rect.top + 30, rect.Width(), rect.Height() - 30);
		graphics->DrawRectangle(&colorPen, rect.left, rect.top + 30, rect.Width(), rect.Height() - 30);
	}
	if( m_shapeType == ShapeType::development_comment )
	{
		graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
		// Draw single line
		int width = rect.Width()-20; //*0.75;
		int height = 20; //Height()-20; //*0.25;
		graphics->DrawLine(&colorPen, rect.left + width, rect.top, rect.right, rect.top + height);
	}
	if( m_shapeType == ShapeType::development_component )
	{
		graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height()/2);
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
		// middle fill
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height() /2);
	}
}

//
// CImageElement class
//
void CImageElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	CPoint & p1 = ctxt.GetTopLeft();
	CPoint & p2 = ctxt.GetBottomRight();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();

	if( m_image.length() == 0 )
	{
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
		graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
		graphics->DrawLine(&colorPen, p1.x, p2.y, p2.x, p1.y);
	}
	else
	{			
		// Load the image		
		std::wstring imageFilePath;
			
		// FIXME: Infrastructure shapes hard coded...
		if( m_type == ElementType::type_shapes_infrastructure )
			imageFilePath = GetImageFilePath();		
		if( m_type == ElementType::type_image )
			imageFilePath = GetImageFilePath();
		Image image(CStringW(imageFilePath.c_str()));
		//graphics->DrawImage(&image, rect.left, rect.top, rect.Width(), rect.Height());
		//graphics->DrawImage(&image, rect.left, rect.top, image.GetWidth(), image.GetHeight());

		if( m_type == ElementType::type_shapes_infrastructure )
		{
			graphics->DrawImage(&image, rect.left, rect.top, image.GetWidth(), image.GetHeight());
		}

		if( m_type == ElementType::type_image )
		{
			//Rect destRect(rect.left, rect.top, rect.Width(), rect.Height());
			//graphics->DrawImage(&image, destRect, 0, 0, image.GetWidth(), image.GetHeight(), UnitPixel, NULL, NULL, NULL);
			graphics->DrawImage(&image, rect.left, rect.top, image.GetWidth(), image.GetHeight());
		}
			
		// Infrastructure shapes are fixed size
		//if( m_type == type_shapes_infrastructure )
		{
			// Adjust fixed image size
			CPoint p1(rect.left, rect.top);
			CPoint p2(p1.x + image.GetWidth(), p1.y + image.GetHeight());
			m_rect.SetRect(p1, p2);		
		}

		// Useless or not to draw a rectangle around the image ???
		if( HasColorLine() )
		{
			rect = m_rect;
			graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
		}

	}
}

//
// CInfrastructureElement class
//
void CInfrastructureElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();

}

//
// CTextElement class
//
void CTextElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	CPoint & p1 = ctxt.GetTopLeft();
	CPoint & p2 = ctxt.GetBottomRight();
	SolidBrush & solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush & lgBrush = ctxt.GetGradientBrushColor();
	// RectF object for Text
	PointF pointF(p1.x, p1.y);
	SizeF sizeF(rect.Width(), rect.Height());
	RectF rectF(pointF, sizeF);

	//if( m_shapeType == ShapeType::text )
	{
		// Useless or not do fill a rectangle around the texte ???
		if( HasColorFill() )
		{
			if( IsSolidColorFill() )
				graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
			else
				graphics->FillRectangle(&lgBrush, rect.left, rect.top, rect.Width(), rect.Height());
		}
		// Useless or not to draw a rectangle around the text ???
		if( HasColorLine() )
		{
			graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
		}

		// Font object
		//FontFamily fontFamily(L"Calibri");
		//Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
		CStringW fontName(m_fontName.c_str());
		FontFamily fontFamily(fontName);
		Gdiplus::Font font(&fontFamily, this->m_fontSize, FontStyleRegular, UnitPixel);
		// StringFormat object
		StringFormat stringFormat;
		if( m_shapeType == ShapeType::text_left )
		{
			stringFormat.SetAlignment(StringAlignmentNear);
		}
		if( m_shapeType == ShapeType::text_center )
		{
			stringFormat.SetAlignment(StringAlignmentCenter);
		}
		if( m_shapeType == ShapeType::text_right )
		{
			stringFormat.SetAlignment(StringAlignmentFar);
		}
		//stringFormat.SetLineAlignment(StringAlignmentCenter);
		// Brush object
		SolidBrush solidBrush(Color(255, 0, 0, 0));
		graphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
		graphics->DrawString(CStringW(m_text.c_str()), -1, &font, rectF, &stringFormat, &solidBrush);
	}
}

void CSimpleTextElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	CPoint & p1 = ctxt.GetTopLeft();
	CPoint & p2 = ctxt.GetBottomRight();
	SolidBrush & solidBrush = ctxt.GetBrushBlack();
	LinearGradientBrush & lgBrush = ctxt.GetGradientBrushColor();
	CPoint pointText(rect.left + 10, rect.top + 10);

	if( m_text.empty() == false )
	{
		// Font object
		FontFamily fontFamily(L"Calibri");
		Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
		graphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
		graphics->DrawString(CStringW(m_text.c_str()), -1, &font, PointF(pointText.x, pointText.y), &solidBrush);
	}

}

//
// CNotImplementedElement class
//
void CNotImplementedElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	CPoint & p1 = ctxt.GetTopLeft();
	CPoint & p2 = ctxt.GetBottomRight();
	SolidBrush & solidBrush = ctxt.GetBrushBlack();
	LinearGradientBrush & lgBrush = ctxt.GetGradientBrushColor();
	CPoint pointText(rect.left + 10, rect.top + 10);

	if( IsSolidColorFill() )
		graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
	else
		graphics->FillRectangle(&lgBrush, rect.left, rect.top, rect.Width(), rect.Height());

	CString str;
	str.Format(_T("%s Shape type=%d"), m_text.c_str(), m_shapeType);
	// Font object
	FontFamily fontFamily(L"Calibri");
	Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
	graphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
	graphics->DrawString(CStringW(str), -1, &font, PointF(pointText.x, pointText.y), &solidBrush);
}
