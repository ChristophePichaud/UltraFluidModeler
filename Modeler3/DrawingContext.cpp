#include "StdAfx.h"
#include "DrawingContext.h"

CDrawingContext::CDrawingContext(std::shared_ptr<CElement> pElement) 
	: m_gdiPenBlack(Color(255, 0, 0, 0)), 
		m_gdiPenColor(Color(255, 0, 0, 0)), 
		m_gdiBrushColor(m_gdiColorBlack),
		m_gdiBrushBlack(m_gdiColorBlack),
		m_gdiGradientBrush(
		Point(pElement->m_rect.left, pElement->m_rect.top), 
		Point(pElement->m_rect.right, pElement->m_rect.bottom), 
		m_gdiColorBlack, 
		m_gdiColorBlack) 
{
	m_gdiColorWhite.SetFromCOLORREF(Color::White);
	m_gdiColorBlack.SetFromCOLORREF(Color::Black);
	m_gdiColorLine.SetValue(Color::MakeARGB(255, GetRValue(pElement->m_colorLine), GetGValue(pElement->m_colorLine), GetBValue(pElement->m_colorLine)));
	m_gdiColorFill.SetValue(Color::MakeARGB(255, GetRValue(pElement->m_colorFill), GetGValue(pElement->m_colorFill), GetBValue(pElement->m_colorFill)));
	m_gdiPenBlack.SetColor(m_gdiColorBlack);
	m_gdiPenColor.SetColor(m_gdiColorLine);
	m_gdiPenColor.SetWidth(pElement->m_lineWidth);
	m_pointTopLeft = pElement->m_rect.TopLeft();
	m_pointBottomRight = pElement->m_rect.BottomRight();
	m_gdiBrushColor.SetColor(m_gdiColorFill);
	
	Color color1(255, 241, 247, 255);
	m_gdiGradientBrush.SetLinearColors(color1, m_gdiColorFill);


}


CDrawingContext::~CDrawingContext(void)
{
}
