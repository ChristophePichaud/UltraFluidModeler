// Copyright (C) SAS NET Azure Rangers
// All rights reserved.

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
		/*
		//colorPen.SetEndCap(LineCapArrowAnchor);
		//graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
		//Create a Path, and add two lines to it.
		Point points[3] = {Point(-5, -5), Point(0, 0), Point(5, -5)};
		GraphicsPath capPath;
		capPath.AddLines(points, 3);

		// Create a CustomLineCap object.
		CustomLineCap custCap(NULL, &capPath); 
   
		// Set the start and end caps for custCap.
		custCap.SetStrokeCap(LineCap::LineCapRoundAnchor); // (LineCapTriangle);

		// Create a Pen object, assign startStrokeCap and endStrokeCap as the 
		// start and end caps, and draw a line.
		colorPen.SetCustomEndCap(&custCap);
		*/

		AdjustableArrowCap aac(10, 4);
		colorPen.SetCustomEndCap(&aac);
		// Set a triangular dash cap for the pen.
		colorPen.SetDashStyle(DashStyle::DashStyleSolid);
		graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
	}
	else if( m_shapeType == ShapeType::line_left_right )
	{
		//colorPen.SetStartCap(LineCapArrowAnchor);
		//colorPen.SetEndCap(LineCapArrowAnchor);
		AdjustableArrowCap aac(10, 4);
		colorPen.SetCustomStartCap(&aac);
		colorPen.SetCustomEndCap(&aac);
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
// CCourbe1Element class
//
void CCourbe1Element::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush& lgBrush = ctxt.GetGradientBrushColor();

	// * 1 * * * * * * * *
	// * * * * * * * 2 * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * 3
	// * * * * * * * * * *
	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;
	Point points[7] = {/*1*/ Point(rect.left + step1_10_x * 2, rect.top),
		/*2*/ Point(rect.left + step1_10_x * 8, rect.top + step1_10_y * 1),
		/*3*/ Point(rect.left + step1_10_x * 9, rect.top + step1_10_y * 9)};
	int npoints = 3;
	graphics->DrawCurve(&colorPen, points, npoints);
}

//
// CCourbe2Element class
//
void CCourbe2Element::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush& lgBrush = ctxt.GetGradientBrushColor();

	// * * * * * 4 * * * *
	// * * * * 3 * 5 * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * 2 * * * * * * 6
	// 1 * * * * * * * * *
	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;
	Point points[7] = {/*1*/ Point(rect.left + step1_10_x * 1, rect.top + step1_10_y * 9),
		/*2*/ Point(rect.left + step1_10_x * 3, rect.top + step1_10_y * 8),
		/*3*/ Point(rect.left + step1_10_x * 5, rect.top + step1_10_y * 1),
		/*4*/ Point(rect.left + step1_10_x * 6, rect.top + step1_10_y * 0),
		/*5*/ Point(rect.left + step1_10_x * 7, rect.top + step1_10_y * 1),
		/*6*/ Point(rect.left + step1_10_x * 9, rect.top + step1_10_y * 8) };
	int npoints = 6;
	graphics->DrawCurve(&colorPen, points, npoints);
}

//
//  CPatateElement class
//
void CPatateElement::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush& lgBrush = ctxt.GetGradientBrushColor();

	// * * * * * * * 1 * *
	// * 2 * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * 6 *
	// * * * * * * * * * 5
	// * 3 * * * * * * * *
	// * * * * * * * * 4 *
	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;
	Point points[7] = {/*1*/ Point(rect.left + step1_10_x * 8, rect.top + step1_10_y * 0),
		/*2*/ Point(rect.left + step1_10_x * 2, rect.top + step1_10_y * 2),
		/*3*/ Point(rect.left + step1_10_x * 2, rect.top + step1_10_y * 8),
		/*4*/ Point(rect.left + step1_10_x * 8, rect.top + step1_10_y * 9),
		/*5*/ Point(rect.left + step1_10_x * 9, rect.top + step1_10_y * 7),
		/*6*/ Point(rect.left + step1_10_x * 8, rect.top + step1_10_y * 6),
		/*7*/ Point(rect.left + step1_10_x * 8, rect.top + step1_10_y * 0) };
	int npoints = 7;
	graphics->DrawCurve(&colorPen, points, npoints);
}

//
//  CCurveElement class
//
void CCurveElement::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush& lgBrush = ctxt.GetGradientBrushColor();

	// * * * * * * * * * *
	// * 1 * * * * * * * *
	// * * * * * * * 2 * *
	// * * * * * * * * 3 *
	// * * * * * * * 4 * *
	// * * 5 * * * * * * *
	// * 6 * * * * * * * *
	// * * 7 * * * * * * *
	// * * * * * * * * 8 *
	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;
	Point points[8] = {/*1*/ Point(rect.left + step1_10_x * 2, rect.top + step1_10_y * 1),
		/*2*/ Point(rect.left + step1_10_x * 7, rect.top + step1_10_y * 2),
		/*3*/ Point(rect.left + step1_10_x * 8, rect.top + step1_10_y * 3),
		/*4*/ Point(rect.left + step1_10_x * 7, rect.top + step1_10_y * 4),
		/*5*/ Point(rect.left + step1_10_x * 3, rect.top + step1_10_y * 5),
		/*6*/ Point(rect.left + step1_10_x * 2, rect.top + step1_10_y * 6),
		/*7*/ Point(rect.left + step1_10_x * 3, rect.top + step1_10_y * 7),
		/*8*/ Point(rect.left + step1_10_x * 8, rect.top + step1_10_y * 8) };
	int npoints = 8;

	if (m_shapeType == curve_right)
	{
		AdjustableArrowCap aac(10, 4);
		//colorPen.SetCustomStartCap(&aac);
		colorPen.SetCustomEndCap(&aac);
	}

	if (m_shapeType == curve_left_right)
	{
		AdjustableArrowCap aac(10, 4);
		colorPen.SetCustomStartCap(&aac);
		colorPen.SetCustomEndCap(&aac);
	}

	graphics->DrawCurve(&colorPen, points, npoints);
}

//
//  CCurveFuzzyElement class
//
void CCurveFuzzyElement::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush& lgBrush = ctxt.GetGradientBrushColor();

	// * * * * 1 * * * * *
	// * * * * * * * * * *
	// * * * * * * 3 * * *
	// * * 2 * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * 4 * *
	// * * * * * * * * * *
	// * * * * * 5 * * * *
	// * * * * * * * 6 * *
	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;
	Point points[6] = {/*1*/ Point(rect.left + step1_10_x * 4, rect.top + step1_10_y * 0),
		/*2*/ Point(rect.left + step1_10_x * 2, rect.top + step1_10_y * 3),
		/*3*/ Point(rect.left + step1_10_x * 6, rect.top + step1_10_y * 2),
		/*4*/ Point(rect.left + step1_10_x * 7, rect.top + step1_10_y * 5),
		/*5*/ Point(rect.left + step1_10_x * 5, rect.top + step1_10_y * 8),
		/*6*/ Point(rect.left + step1_10_x * 7, rect.top + step1_10_y * 9) };
	int npoints = 6;

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

	if (m_shapeType == ShapeType::line_broken || m_shapeType == line_broken_right ||
		m_shapeType == ShapeType::line_broken2 || m_shapeType == ShapeType::line_broken_right2 || m_shapeType == ShapeType::line_broken_left_right)
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

		if( m_shapeType == line_broken_right || m_shapeType == line_broken_right2 )
		{
			AdjustableArrowCap aac(10, 4);
			//colorPen.SetCustomStartCap(&aac);
			colorPen.SetCustomEndCap(&aac);
		}
			
		if (m_shapeType == line_broken_left_right )
		{
			AdjustableArrowCap aac(10, 4);
			colorPen.SetCustomStartCap(&aac);
			colorPen.SetCustomEndCap(&aac);
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
	CPoint& p1 = ctxt.GetTopLeft();
	CPoint& p2 = ctxt.GetBottomRight();

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
	if (m_shapeType == development_association)
	{
		Point points[5] = {Point(-5, 5), Point(-5, -5), Point(5, -5), Point(5, 5), Point(-5, 5) };
		GraphicsPath capPath;
		capPath.AddLines(points, 5);

		// Create a CustomLineCap object.
		CustomLineCap custCap(NULL, &capPath);

		// Create a Pen object, assign startStrokeCap and endStrokeCap as the
		// start and end caps, and draw a line.
		colorPen.SetCustomStartCap(&custCap);
		colorPen.SetCustomEndCap(&custCap);

		//AdjustableArrowCap aac(10, 4);
		//colorPen.SetCustomEndCap(&aac);
		// Set a triangular dash cap for the pen.
		colorPen.SetDashStyle(DashStyle::DashStyleSolid);
		graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
	}
	if (m_shapeType == development_aggregation)
	{
		Point points[5] = { Point(-5, 0), Point(0, -5), Point(5, 0), Point(0, 5), Point(-5, 0) };
		GraphicsPath capPath;
		capPath.AddLines(points, 5);

		// Create a CustomLineCap object.
		CustomLineCap custCap(NULL, &capPath);

		// Create a Pen object, assign startStrokeCap and endStrokeCap as the
		// start and end caps, and draw a line.
		colorPen.SetCustomStartCap(&custCap);
		//colorPen.SetCustomEndCap(&custCap);

		AdjustableArrowCap aac(10, 4);
		colorPen.SetCustomEndCap(&aac);
		// Set a triangular dash cap for the pen.
		colorPen.SetDashStyle(DashStyle::DashStyleSolid);
		graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
	}

	if (m_shapeType == development_composition)
	{
		Point points2[5] = { Point(rect.left -5, rect.top +0), Point(rect.left +0, rect.top -5), Point(rect.left +5, rect.top +0), 
								Point(rect.left +0, rect.top +5), Point(rect.left -5, rect.top +0) };
		SolidBrush solidBrush2(ctxt.GetColorBlack());
		graphics->FillPolygon(&solidBrush2, points2, 5);

		Point points[5] = { Point(-5, 0), Point(0, -5), Point(5, 0), Point(0, 5), Point(-5, 0) };
		GraphicsPath capPath;
		capPath.AddLines(points, 5);

		// Create a CustomLineCap object.
		CustomLineCap custCap(NULL, &capPath);

		// Create a Pen object, assign startStrokeCap and endStrokeCap as the
		// start and end caps, and draw a line.
		colorPen.SetCustomStartCap(&custCap);
		//colorPen.SetCustomEndCap(&custCap);

		AdjustableArrowCap aac(10, 4);
		colorPen.SetCustomEndCap(&aac);
		// Set a triangular dash cap for the pen.
		colorPen.SetDashStyle(DashStyle::DashStyleSolid);
		graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
	}

	if (m_shapeType == development_dependency)
	{
		Point points[3] = { Point(-5, -5), Point(0, 0), Point(5, -5) };
		GraphicsPath capPath;
		capPath.AddLines(points, 3);

		// Create a CustomLineCap object.
		CustomLineCap custCap(NULL, &capPath);

		// Create a Pen object, assign startStrokeCap and endStrokeCap as the
		// start and end caps, and draw a line.
		//colorPen.SetCustomStartCap(&custCap);
		colorPen.SetCustomEndCap(&custCap);

		// Set a triangular dash cap for the pen.
		colorPen.SetDashStyle(DashStyle::DashStyleDot);
		graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
	}

	if (m_shapeType == development_inheritance)
	{
		Point points[4] = { Point(-5, -5), Point(0, 0), Point(5, -5), Point(-5, -5) };
		GraphicsPath capPath;
		capPath.AddLines(points, 4);

		// Create a CustomLineCap object.
		CustomLineCap custCap(NULL, &capPath);

		// Create a Pen object, assign startStrokeCap and endStrokeCap as the
		// start and end caps, and draw a line.
		//colorPen.SetCustomStartCap(&custCap);
		colorPen.SetCustomEndCap(&custCap);

		// Set a triangular dash cap for the pen.
		colorPen.SetDashStyle(DashStyle::DashStyleSolid);
		graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
	}

	if (m_shapeType == development_package_import)
	{
		Point points[3] = { Point(-5, -5), Point(0, 0), Point(5, -5) };
		GraphicsPath capPath;
		capPath.AddLines(points, 3);

		// Create a CustomLineCap object.
		CustomLineCap custCap(NULL, &capPath);

		// Create a Pen object, assign startStrokeCap and endStrokeCap as the
		// start and end caps, and draw a line.
		//colorPen.SetCustomStartCap(&custCap);
		colorPen.SetCustomEndCap(&custCap);

		// Set a triangular dash cap for the pen.
		colorPen.SetDashStyle(DashStyle::DashStyleDot);
		graphics->DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
	}

}

//
// CImportElement class
//
void CImportElement::Draw(CDrawingContext & ctxt)
{
	CRect rect = m_rect;
	Graphics * graphics = ctxt.GetGraphics();
	Pen & colorPen = ctxt.GetPenColor();
	SolidBrush & solidBrush = ctxt.GetBrushColor();
	SolidBrush & solidBrushText = ctxt.GetBrushBlack();

	if (m_shapeType == ShapeType::import_class)
	{
		graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());

		CPoint & p1 = ctxt.GetTopLeft();
		CPoint & p2 = ctxt.GetBottomRight();
		CPoint pointText(rect.left + 10, rect.top + 10);

		// Font object
		FontFamily fontFamily(L"Calibri");
		Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
		graphics->SetTextRenderingHint(TextRenderingHintAntiAlias);

		//m_text = this->m_TypeData->m_Name;
		graphics->DrawString(CStringW(this->m_text.c_str()), -1, &font, PointF(pointText.x, pointText.y), &solidBrushText);

		if (this->m_TypeData == nullptr)
		{
			return;
		}

		int count = 2; // class name is entry 1 and was done see previous...
		for (auto it = this->m_TypeData->m_Properties.begin() ; it != this->m_TypeData->m_Properties.end() ; it++)
		{
			std::shared_ptr<CPropertyData> ppd = *it;
			std::wstring propertyName = ppd->m_Name;
			std::wstring propertyType = ppd->m_Type;
			std::wstringstream str1;
			str1 << propertyType << L" " << propertyName;

			CPoint pointText2(rect.left + 10, rect.top + count*10);

			//graphics->DrawString(CStringW(propertyName.c_str()), -1, &font, PointF(pointText2.x, pointText2.y), &solidBrushText);
			graphics->DrawString(CStringW(str1.str().c_str()), -1, &font, PointF(pointText2.x, pointText2.y), &solidBrushText);

			count++;
		}

		for (auto it = this->m_TypeData->m_Methods.begin(); it != this->m_TypeData->m_Methods.end(); it++)
		{
			std::shared_ptr<CMethodData> pmd = *it;
			std::wstring methodName = pmd->m_Name;

			CPoint pointText2(rect.left + 10, rect.top + count * 10);

			graphics->DrawString(CStringW(methodName.c_str()), -1, &font, PointF(pointText2.x, pointText2.y), &solidBrushText);

			count++;
		}

		for (auto it = this->m_TypeData->m_Fields.begin(); it != this->m_TypeData->m_Fields.end(); it++)
		{
			std::shared_ptr<CFieldData> pfd = *it;
			std::wstring fieldName = pfd->m_Name;
			std::wstring fieldType = pfd->m_Type;
			std::wstringstream str1;
			str1 << fieldType << L" " << fieldName;

			CPoint pointText2(rect.left + 10, rect.top + count * 10);

			//graphics->DrawString(CStringW(fieldName.c_str()), -1, &font, PointF(pointText2.x, pointText2.y), &solidBrushText);
			graphics->DrawString(CStringW(str1.str().c_str()), -1, &font, PointF(pointText2.x, pointText2.y), &solidBrushText);

			count++;
		}
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

		if (IsFixed())
		{
			// Adjust fixed image size
			CPoint p1(rect.left, rect.top);
			CPoint p2(p1.x + image.GetWidth(), p1.y + image.GetHeight());
			m_rect.SetRect(p1, p2);

			graphics->DrawImage(&image, rect.left, rect.top, image.GetWidth(), image.GetHeight());
		}
		else
		{
			Rect destRect(rect.left, rect.top, rect.Width(), rect.Height());
			graphics->DrawImage(&image, destRect, 0, 0, image.GetWidth(), image.GetHeight(), UnitPixel, NULL, NULL, NULL);
		}

#ifdef OLD

		if( m_type == ElementType::type_shapes_infrastructure )
		{
			//Rect destRect(rect.left, rect.top, rect.Width(), rect.Height());
			//graphics->DrawImage(&image, destRect, 0, 0, image.GetWidth(), image.GetHeight(), UnitPixel, NULL, NULL, NULL);
			////graphics->DrawImage(&image, rect.left, rect.top, image.GetWidth(), image.GetHeight());

			// Adjust fixed image size
			CPoint p1(rect.left, rect.top);
			CPoint p2(p1.x + image.GetWidth(), p1.y + image.GetHeight());
			m_rect.SetRect(p1, p2);

			graphics->DrawImage(&image, rect.left, rect.top, image.GetWidth(), image.GetHeight());
		}

		if( m_type == ElementType::type_image )
		{
			Rect destRect(rect.left, rect.top, rect.Width(), rect.Height());
			graphics->DrawImage(&image, destRect, 0, 0, image.GetWidth(), image.GetHeight(), UnitPixel, NULL, NULL, NULL);
			//graphics->DrawImage(&image, rect.left, rect.top, image.GetWidth(), image.GetHeight());
		}
#endif

#ifdef FIXED_SIZE
		// Infrastructure shapes are fixed size
		if( m_type == type_shapes_infrastructure )
		{
			// Adjust fixed image size
			CPoint p1(rect.left, rect.top);
			CPoint p2(p1.x + image.GetWidth(), p1.y + image.GetHeight());
			m_rect.SetRect(p1, p2);		
		}
#endif

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
	//PointF pointF(p1.x, p1.y);
	//SizeF sizeF(rect.Width(), rect.Height());
	//RectF rectF(pointF, sizeF);
	//PointF pointText(rect.left + 10, rect.top + 10);
	//SizeF sizeF(rect.Width() - 10, rect.Height() - 10);
	PointF pointText(rect.left + m_leftMargin, rect.top + m_topMargin);
	SizeF sizeF(rect.Width() - 10, rect.Height() - 10);
	RectF rectF(pointText, sizeF);

	//if( m_shapeType == ShapeType::text )
	{
		// Useless or not do fill a rectangle around the texte ???
		/*if( HasColorFill() )
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
		}*/

		// Font object
		//FontFamily fontFamily(L"Calibri");
		//Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
		CStringW fontName(m_fontName.c_str());
		FontFamily fontFamily(fontName);
		FontStyle style = FontStyle::FontStyleRegular;
		if (m_bBold == true)
		{
			style = (FontStyle)(style | FontStyle::FontStyleBold);
		}
		if (m_bItalic == true)
		{
			style = (FontStyle)(style | FontStyle::FontStyleItalic);
		}
		if (m_bUnderline == true)
		{
			style = (FontStyle)(style | FontStyle::FontStyleUnderline);
		}
		if (m_bStrikeThrough == true)
		{
			style = (FontStyle)(style | FontStyle::FontStyleStrikeout);
		}

		//Gdiplus::Font font(&fontFamily, this->m_fontSize, FontStyleRegular, UnitPixel);
		Gdiplus::Font font(&fontFamily, this->m_fontSize, style, UnitPixel);

		// StringFormat object
		StringFormat stringFormat;
		if (m_textAlign == _T("Left"))
		{
			stringFormat.SetAlignment(StringAlignmentNear);
		}
		else if (m_textAlign == _T("Center"))
		{
			stringFormat.SetAlignment(StringAlignmentCenter);
		}
		else if (m_textAlign == _T("Right"))
		{
			stringFormat.SetAlignment(StringAlignmentFar);
		}
		//stringFormat.SetLineAlignment(StringAlignmentCenter);
		// Brush object
		//SolidBrush solidBrush(Color(255, 0, 0, 0));

		if (HasColorFill())
		{
			if (IsSolidColorFill())
			{
				graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
			}
		}

		Color color;
		color.SetFromCOLORREF(m_colorText);
		SolidBrush solidBrush(color);
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
	//PointF pointText(rect.left + 10, rect.top + 10);
	//SizeF sizeF(rect.Width() - 10, rect.Height() - 10);
	PointF pointText(rect.left + m_leftMargin, rect.top + m_topMargin);
	SizeF sizeF(rect.Width() - 10, rect.Height() - 10);
	RectF rectF(pointText, sizeF);

	if( m_text.empty() == false )
	{
		StringFormat stringFormat;
		if (m_textAlign == _T("Left"))
		{
			stringFormat.SetAlignment(StringAlignmentNear);
		}
		if (m_textAlign == _T("Center"))
		{
			stringFormat.SetAlignment(StringAlignmentCenter);
		}
		if (m_textAlign == _T("Right"))
		{
			stringFormat.SetAlignment(StringAlignmentFar);
		}
		// Font object
		//FontFamily fontFamily(L"Calibri");
		CStringW fontName(m_fontName.c_str());
		FontFamily fontFamily(fontName);
		//Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);	
		Gdiplus::Font font(&fontFamily, this->m_fontSize, FontStyleRegular, UnitPixel);
		graphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
		graphics->DrawString(CStringW(m_text.c_str()), -1, &font, rectF, &stringFormat, &solidBrush);
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

//
// CSelectionElement class
//
void CSelectionElement::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush& lgBrush = ctxt.GetGradientBrushColor();

	if (HasColorFill())
	{
		if (IsSolidColorFill())
			graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		else
			graphics->FillRectangle(&lgBrush, rect.left, rect.top, rect.Width(), rect.Height());
	}
	if (HasColorLine())
	{
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	}
}

//
// CPlanningElement class
//
void CPlanningElement::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();

	if (m_shapeType == ShapeType::planning_month)
	{
		graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	}
	if (m_shapeType == ShapeType::planning_task)
	{
		graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
		// Draw single line
		int width = rect.Width() - 20; //*0.75;
		int height = 20; //Height()-20; //*0.25;
		graphics->DrawLine(&colorPen, rect.left + width, rect.top, rect.right, rect.top + height);
	}
}

//
// CDiagramElement class
//
void CDiagramElement::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();

	if (m_shapeType == ShapeType::diagram)
	{
		graphics->FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		graphics->DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
		int width = rect.Width() - 20; //*0.75;
		int height = 20; //Height()-20; //*0.25;
		graphics->DrawLine(&colorPen, rect.left + width, rect.top, rect.right, rect.top + height);
	}
}

//
// CBasicRectangleElement class
//
void CBasicRectangleElement::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush& lgBrush = ctxt.GetGradientBrushColor();

	// 1 * * * * * * * * 2
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// 4 * * * * * * * * 3

	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;
	Point points[5] = {/*1*/ Point(rect.left, rect.top),
		/*2*/ Point(rect.right, rect.top),
		/*3*/ Point(rect.right, rect.bottom),
		/*4*/ Point(rect.left, rect.bottom),
		/*5*/ Point(rect.left, rect.top) };
	int npoints = 5;

	if (HasColorFill())
	{
		if (IsSolidColorFill())
			graphics->FillPolygon(&solidBrush, points, npoints);
		else
			graphics->FillPolygon(&lgBrush, points, npoints);
	}
	if (HasColorLine())
	{
		graphics->DrawPolygon(&colorPen, points, npoints);
	}
}

//
// CBasicRectangle2Element class
//
void CBasicRectangle2Element::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush& lgBrush = ctxt.GetGradientBrushColor();

	// * * 1 * * * * * * 2
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// 4 * * * * * * 3 * *

	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;

	Point points[5] = {/*1*/ Point(rect.left + step1_10_x * 2, rect.top),
		/*2*/ Point(rect.right, rect.top),
		/*3*/ Point(rect.left + step1_10_x * 8, rect.bottom),
		/*4*/ Point(rect.left, rect.bottom),
		/*5*/ Point(rect.left + step1_10_x * 2, rect.top) };
	int npoints = 5;

	if (HasColorFill())
	{
		if (IsSolidColorFill())
			graphics->FillPolygon(&solidBrush, points, npoints);
		else
			graphics->FillPolygon(&lgBrush, points, npoints);
	}
	if (HasColorLine())
	{
		graphics->DrawPolygon(&colorPen, points, npoints);
	}
}

//
// CBasicRectangle3Element class
//
void CBasicRectangle3Element::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush& lgBrush = ctxt.GetGradientBrushColor();

	// 1 * * * * * * * * 2
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * 4 * * * * 3 * *

	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;

	Point points[5] = {/*1*/ Point(rect.left + step1_10_x * 0, rect.top),
		/*2*/ Point(rect.right, rect.top),
		/*3*/ Point(rect.left + step1_10_x * 8, rect.bottom),
		/*4*/ Point(rect.left + step1_10_x * 2, rect.bottom),
		/*5*/ Point(rect.left + step1_10_x * 0, rect.top) };
	int npoints = 5;

	if (HasColorFill())
	{
		if (IsSolidColorFill())
			graphics->FillPolygon(&solidBrush, points, npoints);
		else
			graphics->FillPolygon(&lgBrush, points, npoints);
	}
	if (HasColorLine())
	{
		graphics->DrawPolygon(&colorPen, points, npoints);
	}
}

//
// CBasicLosangeElement class
//
void CBasicLosangeElement::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush& lgBrush = ctxt.GetGradientBrushColor();

	// * * * * 1 * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// 4 * * * * * * * * 2
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * 3 * * * * *

	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;

	Point points[5] = {/*1*/ Point(rect.left + rect.Width() / 2, rect.top),
		/*2*/ Point(rect.right, rect.top + rect.Height() / 2),
		/*3*/ Point(rect.left + rect.Width() / 2, rect.bottom),
		/*4*/ Point(rect.left, rect.top + rect.Height() / 2),
		/*5*/ Point(rect.left + rect.Width() / 2, rect.top) };
	int npoints = 5;

	if (HasColorFill())
	{
		if (IsSolidColorFill())
			graphics->FillPolygon(&solidBrush, points, npoints);
		else
			graphics->FillPolygon(&lgBrush, points, npoints);
	}
	if (HasColorLine())
	{
		graphics->DrawPolygon(&colorPen, points, npoints);
	}
}

//
// CBasicRoundRectangleElement class
//
void CBasicRoundRectangleElement::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush& lgBrush = ctxt.GetGradientBrushColor();

	// * 2 * * * * * * 3 *
	// 1 * * * * * * * * 4
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// 8 * * * * * * * * 5
	// * 7 * * * * * * 6 *

	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;

	Point points[9] = {/*1*/ Point(rect.left + step1_10_x * 0, rect.top + step1_10_y * 1),
		/*2*/ Point(rect.left + step1_10_x * 1, rect.top),
		/*3*/ Point(rect.left + step1_10_x * 9, rect.top),
		/*4*/ Point(rect.left + step1_10_x * 10, rect.top + step1_10_y * 1),
		/*5*/ Point(rect.left + step1_10_x * 10, rect.top + step1_10_y * 9),
		/*6*/ Point(rect.left + step1_10_x * 9, rect.bottom),
		/*7*/ Point(rect.left + step1_10_x * 1, rect.bottom),
		/*8*/ Point(rect.left + step1_10_x * 0, rect.top + step1_10_y * 9),
		/*9*/ Point(rect.left + step1_10_x * 0, rect.top + step1_10_y * 1)};
	int npoints = 9;

	GraphicsPath gp;
	gp.AddCurve(points, npoints);

	if (HasColorFill())
	{
		if (IsSolidColorFill())
			graphics->FillPath(&solidBrush, &gp);
		else
			graphics->FillPath(&lgBrush, &gp);
	}

	//	if (HasColorLine())
	//{
	//}

	graphics->DrawPath(&colorPen, &gp);
}

//
// CBasicDatabaseElement class
//
void CBasicDatabaseElement::Draw(CDrawingContext& ctxt)
{
	CRect rect = m_rect;
	Graphics* graphics = ctxt.GetGraphics();
	Pen& colorPen = ctxt.GetPenColor();
	SolidBrush& solidBrush = ctxt.GetBrushColor();
	LinearGradientBrush& lgBrush = ctxt.GetGradientBrushColor();

	// * * * * * * * * * *
	// 4 * * * * * * * * 1
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// * * * * * * * * * *
	// 3 * * * * * * * * 2

	int step1_10_x = rect.Width() / 10;
	int step1_10_y = rect.Height() / 10;

	Point points[4] = {
		/*1*/ Point(rect.right, rect.top + step1_10_y * 1 ),
		/*2*/ Point(rect.right, rect.bottom),
		/*3*/ Point(rect.left + step1_10_x * 0, rect.bottom),
		/*4*/ Point(rect.left + step1_10_x * 0, rect.top + step1_10_y * 1) };
	int npoints = 4;

	GraphicsPath gp;
	gp.SetFillMode(FillMode::FillModeWinding);
	gp.AddLines(points, npoints);
	//gp.AddRectangle(Rect(rect.left, rect.top + step1_10_y * 1, rect.Width(), rect.Height()));
	gp.AddEllipse(Rect(rect.left, rect.top, rect.Width(), step1_10_y * 2));

	if (HasColorFill())
	{
		if (IsSolidColorFill())
			graphics->FillPath(&solidBrush, &gp);
		else
			graphics->FillPath(&lgBrush, &gp);
	}

	//	if (HasColorLine())
	//{
	//}

	graphics->DrawPath(&colorPen, &gp);
}
