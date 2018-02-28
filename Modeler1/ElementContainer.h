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
	CElementContainer();
	virtual ~CElementContainer(void);

// Operations
public:
	virtual void Serialize(CElementManager * pElementManager, CArchive& ar);   // overridden for document i/o

// Operations
public:
	std::shared_ptr<CElement> FindElement(ElementType type);
	std::shared_ptr<CElement> FindElement(std::wstring objectId);
	bool FindElement(std::wstring objectId, vector<std::shared_ptr<CElement>>::iterator & it);
	std::shared_ptr<CElement> ObjectAt(const CPoint & point);	
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

