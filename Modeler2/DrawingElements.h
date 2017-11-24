#pragma once
#include "Element.h"
#include "DrawingContext.h"
// CDrawingElement command target

class CLineElement : public CElement
{
//public:
//	CLineElement();
//	virtual ~CLineElement();
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CRectangleElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CTriangleElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CEllipseElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CArrowRightElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CArrowDownElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CStarElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CParentheseLeftElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CParentheseRightElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CLineBrokenElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CDevelopmentElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CImportElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);

public:
	std::shared_ptr<CTypeData> m_TypeData;
};

class CImageElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CInfrastructureElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CTextElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CSimpleTextElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CNotImplementedElement : public CElement
{
public:
	virtual void Draw(CDrawingContext & ctxt);
};

class CConnectableElement : public CElement
{
public:
};
