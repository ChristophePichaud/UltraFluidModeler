// Copyright (C) SAS NET Azure Rangers
// All rights reserved.

#pragma once
#include "Element.h"
#include "ElementContainer.h"

// special 'list' class for this application(requires afxtempl.h)
typedef CTypedPtrList<CObList, CElement *> CElementList;

class CElementContainer : public CObject
{
private:
	
public:
	DECLARE_SERIAL(CElementContainer);
	CElementContainer(const CElementContainer& elementContainer);
	CElementContainer();
	virtual ~CElementContainer(void);

// Operations
public:
	virtual void Serialize(CElementManager * pElementManager, CArchive& ar);   // overridden for document i/o

// Operations
public:
	std::shared_ptr<CElement> FindElement(ElementType type);
	std::shared_ptr<CElement> FindElement(std::wstring objectId);
	std::shared_ptr<CElement> FindElementByName(std::wstring name);
	bool FindElement(std::wstring objectId, vector<std::shared_ptr<CElement>>::iterator & it);
	std::shared_ptr<CElement> ObjectAt(const CRect& rect, SelectType selectType = SelectType::intuitive);
	std::shared_ptr<CElement> ObjectAt(const CPoint & point, SelectType selectType = SelectType::intuitive);
	std::shared_ptr<CElement> ObjectAt(const CPoint& point, std::shared_ptr<CElement> pObj);
	std::shared_ptr<CElement> ObjectExceptLinesAt(const CPoint& point, std::shared_ptr<CElement> pObj);
	vector<std::shared_ptr<CElement>> ObjectsInRect(const CRect& rect);
	vector<std::shared_ptr<CElement>> ObjectsInRectEx(const CRect& rect, SelectType selectType = SelectType::intuitive);
	void Remove(const CElementContainer& selection);
	void Copy(const CElementContainer& selection);
	void Clone(const CElementContainer& selection);
	void ChangeInnerAttributes();
	void CopyWithAttributesChange(const CElementContainer& selection);

// Managing Object Positions
public:
	void MoveToFront(const CElementContainer& selection);
	void MoveForward(const CElementContainer& selection);
	void MoveBackward(const CElementContainer& selection);
	void MoveToBack(const CElementContainer& selection);
	void MoveToBack(std::shared_ptr<CElement> pElement);

// Debugging Operations
public:
	void DebugDumpObjects(CModeler1View * pView);

// Operations MFC like
public:
	std::shared_ptr<CElement> GetHead();
	int GetCount();
	void RemoveAll();
	void Remove(std::shared_ptr<CElement> pElement);
	void AddTail(std::shared_ptr<CElement> pElement);

// Attributes
public:
	vector<std::shared_ptr<CElement>> m_objects;
	//CElementList m_objects;	
};

