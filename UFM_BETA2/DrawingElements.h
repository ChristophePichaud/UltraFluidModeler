// Copyright (C) SAS NET Azure Rangers
// All rights reserved.

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

class CSelectionElement : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CPlanningElement : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CDiagramElement : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CCourbe1Element : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CCourbe2Element : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CCurveElement : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CPatateElement : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CCurveFuzzyElement : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CBasicRectangleElement : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CBasicRectangle2Element : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CBasicRectangle3Element : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CBasicLosangeElement : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CBasicRoundRectangleElement : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};

class CBasicDatabaseElement : public CElement
{
public:
	virtual void Draw(CDrawingContext& ctxt);
};
