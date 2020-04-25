#pragma once
#include "Element.h"

typedef CTypedPtrList<CObList, CElement*> CElementList;

class CGroupContainer : public CObject
{
private:

public:
	DECLARE_SERIAL(CGroupContainer);
	CGroupContainer();
	virtual ~CGroupContainer(void);

	// Operations
public:
	virtual void Serialize(CElementManager* pElementManager, CArchive& ar);   // overridden for document i/o

public:
	static std::vector<std::wstring> CGroupContainer::Split(const std::wstring& s, wchar_t delim);

	// Attributes
public:
	vector<std::shared_ptr<CElement>> m_objects;
	wstring m_name;
	wstring m_elements;
};

class CGroupConnection : public CObject
{
private:

public:
	DECLARE_SERIAL(CGroupConnection);
	CGroupConnection();
	virtual ~CGroupConnection(void);

	// Operations
public:
	virtual void Serialize(CElementManager* pElementManager, CArchive& ar);   // overridden for document i/o

	// Attributes
public:
	vector<std::shared_ptr<CGroupContainer>> m_objects;
};

