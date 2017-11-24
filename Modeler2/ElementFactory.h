#pragma once
#include "Element.h"

class CElementFactory
{
public:
	CElementFactory(void);
	virtual ~CElementFactory(void);
};

class CFactory
{
public:
	static std::shared_ptr<CElement> CreateElementOfType(ElementType type, ShapeType shapeType);
	// counter of objects
	static int g_counter;
};


class CGuid
{
public:
	CGuid();
	virtual ~CGuid(void);

public:
	CString ToString();

private:
	UUID m_uuid;

};

