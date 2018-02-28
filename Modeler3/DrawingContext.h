#pragma once
#include "Element.h"

class CDrawingContext
{
public:
	// Drawing Attributes are built in the ctor
	CDrawingContext(std::shared_ptr<CElement> pElement);
	virtual ~CDrawingContext(void);

public:
	Graphics * GetGraphics() { return m_pGraphics; }
// Methods for Drawing Attributes
public:
	Color & GetColorWhite() { return m_gdiColorWhite; }
	Color & GetColorBlack() { return m_gdiColorBlack; }
	Color & GetColorLine() { return m_gdiColorLine; }
	Color & GetColorFill() { 	return m_gdiColorFill; }
	Pen & GetPenBlack() { return m_gdiPenBlack; }
	Pen & GetPenColor() { return m_gdiPenColor; }
	SolidBrush & GetBrushColor() { return m_gdiBrushColor; }
	SolidBrush & GetBrushBlack() { return m_gdiBrushBlack; }
	LinearGradientBrush & GetGradientBrushColor() { return m_gdiGradientBrush; }
	CPoint GetTopLeft() { return m_pointTopLeft; }
	CPoint GetBottomRight() { return m_pointBottomRight; }

public:
	Graphics * m_pGraphics;

public:
	// GDI+ Drawing objects
	Color m_gdiColorWhite;
	Color m_gdiColorBlack;
	Color m_gdiColorLine;
	Color m_gdiColorFill;
	Pen m_gdiPenBlack;
	Pen m_gdiPenColor;
	SolidBrush m_gdiBrushColor;
	SolidBrush m_gdiBrushBlack;
	LinearGradientBrush m_gdiGradientBrush;

	// MFC Drawing objects
	CPoint m_pointTopLeft;
	CPoint m_pointBottomRight;
};

