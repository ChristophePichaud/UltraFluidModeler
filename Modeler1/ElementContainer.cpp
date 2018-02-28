#include "StdAfx.h"
#include "ElementContainer.h"
#include "Modeler1View.h"

//
// CElementContainer Class
//

IMPLEMENT_SERIAL(CElementContainer, CObject, 0)

CElementContainer::CElementContainer()
{
	//m_objects.reserve(1000);
}

CElementContainer::~CElementContainer(void)
{
}

void CElementContainer::Serialize(CElementManager * pElementManager, CArchive& ar)
{
	if( ar.IsStoring() == TRUE )
	{
		CTypedPtrList<CObList, CElement *> objects;
		for( vector<std::shared_ptr<CElement>>::iterator i = m_objects.begin() ; i!=m_objects.end() ; i++ )
		{
			std::shared_ptr<CElement> pElement = *i;
			objects.AddTail(pElement.get());
		}
	
		objects.Serialize(ar);
	}
	else
	{
		CTypedPtrList<CObList, CElement *> objects;
		objects.Serialize(ar);

		POSITION pos = objects.GetHeadPosition();
		while( pos != NULL )
		{
			CElement * pElement = objects.GetNext(pos);
			//std::shared_ptr<CElement> pNewElement(pElement);

			//FIXED 14July2012 05:30PM
			std::shared_ptr<CElement> pNewElement = CFactory::CreateElementOfType(pElement->m_type, pElement->m_shapeType);
			pNewElement->m_name = pElement->m_name;
			pNewElement->m_text = pElement->m_text;
			pNewElement->m_objectId = pElement->m_objectId;
			pNewElement->m_rect = pElement->m_rect;
			pNewElement->m_bColorFill = pElement->m_bColorFill;
			pNewElement->m_bColorLine = pElement->m_bColorLine;
			pNewElement->m_bColorFill = pElement->m_bColorFill;
			pNewElement->m_bLineWidth = pElement->m_bLineWidth;
			pNewElement->m_bSolidColorFill = pElement->m_bSolidColorFill;
			//pNewElement->m_caption = pElement->m_caption;
			pNewElement->m_colorFill = pElement->m_colorFill;
			pNewElement->m_colorLine = pElement->m_colorLine;
			pNewElement->m_image = pElement->m_image;
			pNewElement->m_last = pElement->m_last;
			pNewElement->m_lineWidth = pElement->m_lineWidth;
			pNewElement->m_pManager = pElementManager; // TODO
			pNewElement->m_point = pElement->m_point;
			POSITION pos = /*pNewElement->m_pView =*/ ar.m_pDocument->GetFirstViewPosition(); //nullptr; // TODO
			pNewElement->m_pView = (CModeler1View *) (ar.m_pDocument->GetNextView(pos)); //()->GetRoutingView();
			m_objects.push_back(pNewElement);
		}
	}

}

std::shared_ptr<CElement> CElementContainer::FindElement(ElementType type)
{
	// find last element of a given type (m_type)
	for( vector<std::shared_ptr<CElement>>::iterator i = m_objects.begin() ; i!=m_objects.end() ; i++ )
	{
		std::shared_ptr<CElement> pElement = *i;
		if( pElement->m_type == type )
		{
			return pElement;
		}
	}
	std::shared_ptr<CElement> pNull;
	return pNull;
}

std::shared_ptr<CElement> CElementContainer::FindElement(std::wstring objectId)
{
	// find last element of a given type
	for( vector<std::shared_ptr<CElement>>::iterator i = m_objects.begin() ; i!=m_objects.end() ; i++ )
	{
		std::shared_ptr<CElement> pElement = *i;
		if( pElement->m_objectId == objectId )
		{
			return pElement;
		}
	}
	std::shared_ptr<CElement> pNull;
	return pNull;
}

bool CElementContainer::FindElement(std::wstring objectId, vector<std::shared_ptr<CElement>>::iterator & it)
{
	// find last element of a given type
	for( vector<std::shared_ptr<CElement>>::iterator i = m_objects.begin() ; i!=m_objects.end() ; i++ )
	{
		std::shared_ptr<CElement> pElement = *i;
		if( pElement->m_objectId == objectId )
		{
			it = i;
			return true;
		}
	}
	return false;
}

std::shared_ptr<CElement> CElementContainer::ObjectAt(const CPoint & point)
{
	CRect rect(point, CSize(1, 1));
	for( vector<std::shared_ptr<CElement>>::reverse_iterator i = m_objects.rbegin() ; i!=m_objects.rend() ; i++ )
	{
		std::shared_ptr<CElement> pElement = *i;
		if (pElement->Intersects(rect))
		{
			return pElement;
		}
	}
	std::shared_ptr<CElement> pNull;
	return pNull;
}

std::shared_ptr<CElement> CElementContainer::GetHead()
{
	vector<std::shared_ptr<CElement>>::iterator i = m_objects.begin();
	if( i!=m_objects.end() )
	{
		std::shared_ptr<CElement> pElement = *i;
		return pElement;
	}

	std::shared_ptr<CElement> pNull;
	return pNull;
}

int CElementContainer::GetCount()
{
	int count = m_objects.size();
	return count;
}


void CElementContainer::MoveToFront(const CElementContainer& selection)
{
	/*
	POSITION pos = selection.m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CElement * pElement = selection.m_objects.GetNext(pos);
		POSITION posObjects = m_objects.Find(pElement);
		if( posObjects != NULL )
		{
			m_objects.RemoveAt(posObjects);
			m_objects.AddTail(pElement);
		}
	}
	*/
	// MSVC STL no problem
	//for( auto itSel = selection.m_objects.begin() ; itSel!=selection.m_objects.end() ; itSel++ )
	// Boost stl need a const_iterator...
	//for( auto itSel = selection.m_objects.begin() ; itSel!=selection.m_objects.end() ; itSel++ )
	for( vector<std::shared_ptr<CElement>>::const_iterator itSel = selection.m_objects.begin() ; itSel!=selection.m_objects.end() ; itSel++ )
	{
		std::shared_ptr<CElement> pElement = *itSel;
		vector<std::shared_ptr<CElement>>::iterator pos = find(m_objects.begin(), m_objects.end(), pElement);
		if( pos != m_objects.end() )
		{
			m_objects.erase(pos);
			m_objects.push_back(pElement);
		}
	}
}

void CElementContainer::MoveToBack(const CElementContainer& selection)
{
	/*
	POSITION pos = selection.m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CElement * pElement = selection.m_objects.GetNext(pos);
		POSITION posObjects = m_objects.Find(pElement);
		if( posObjects != NULL )
		{
			m_objects.RemoveAt(posObjects);
			m_objects.AddHead(pElement);
		}
	}
	*/
	for( vector<std::shared_ptr<CElement>>::const_iterator itSel = selection.m_objects.begin() ; itSel!=selection.m_objects.end() ; itSel++ )
	{
		std::shared_ptr<CElement> pElement = *itSel;
		vector<std::shared_ptr<CElement>>::iterator pos = find(m_objects.begin(), m_objects.end(), pElement);
		if( pos != m_objects.end() )
		{
			m_objects.erase(pos);
			m_objects.insert(m_objects.begin(), pElement);
		}
	}
}

void CElementContainer::MoveForward(const CElementContainer& selection)
{
	/*
	POSITION pos = selection.m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CElement * pElement = selection.m_objects.GetNext(pos);
		POSITION posObjects = m_objects.Find(pElement);
		if( posObjects != NULL )
		{
			if (posObjects != m_objects.GetTailPosition())
			{
				POSITION posNext = posObjects;
				m_objects.GetNext(posNext);
				m_objects.RemoveAt(posObjects);
				m_objects.InsertAfter(posNext, pElement);
			}
		}
	}
	*/
	for( vector<std::shared_ptr<CElement>>::const_iterator itSel = selection.m_objects.begin() ; itSel!=selection.m_objects.end() ; itSel++ )
	{
		std::shared_ptr<CElement> pElement = *itSel;
		vector<std::shared_ptr<CElement>>::iterator pos = find(m_objects.begin(), m_objects.end(), pElement);
		if( pos != m_objects.end() )
		{
			std::shared_ptr<CElement> & pLastElement = m_objects.back();
			if( pElement != pLastElement )
			{
				m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), pElement), m_objects.end());
				m_objects.insert(pos+1, pElement);
			}
		}
	}
}

void CElementContainer::MoveBackward(const CElementContainer& selection)
{
	/*
	POSITION pos = selection.m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CElement * pElement = selection.m_objects.GetNext(pos);
		POSITION posObjects = m_objects.Find(pElement);
		if( posObjects != NULL )
		{
			if (posObjects != m_objects.GetHeadPosition())
			{
				POSITION posNext = posObjects;
				m_objects.GetPrev(posNext);
				m_objects.RemoveAt(posObjects);
				m_objects.InsertBefore(posNext, pElement);
			}
		}
	}
	*/
	for( vector<std::shared_ptr<CElement>>::const_iterator itSel = selection.m_objects.begin() ; itSel!=selection.m_objects.end() ; itSel++ )
	{
		std::shared_ptr<CElement> pElement = *itSel;
		vector<std::shared_ptr<CElement>>::iterator pos = find(m_objects.begin(), m_objects.end(), pElement);
		if( pos != m_objects.end() )
		{
			std::shared_ptr<CElement> & pFirstElement = m_objects.front();
			if( pElement != pFirstElement )
			{
				pos--;
				m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), pElement), m_objects.end());
				m_objects.insert(pos, pElement);
			}
		}
	}
}

void CElementContainer::Remove(const CElementContainer& selection)
{
	//POSITION pos = selection.m_objects.GetHeadPosition();
	//while (pos != NULL)
	//{
	//	CElement * pElement = selection.m_objects.GetNext(pos);
	//	POSITION posObjects = m_objects.Find(pElement);
	//	if( posObjects != NULL )
	//	{
	//		m_objects.RemoveAt(posObjects);
	//	}
	//}

	for( vector<std::shared_ptr<CElement>>::const_iterator itSel = selection.m_objects.begin() ; itSel!=selection.m_objects.end() ; itSel++ )
	{
		std::shared_ptr<CElement> pElement = *itSel;
		
		vector<std::shared_ptr<CElement>>::iterator pos;
		pos = find(m_objects.begin(), m_objects.end(), pElement);
		if( pos != m_objects.end() )
		{
			m_objects.erase(pos);
		}
	}
}

void CElementContainer::Copy(const CElementContainer& selection)
{
	for( vector<std::shared_ptr<CElement>>::const_iterator itSel = selection.m_objects.begin() ; itSel!=selection.m_objects.end() ; itSel++ )
	{
		std::shared_ptr<CElement> pElement = *itSel;
		m_objects.push_back(pElement);
	}
}

void CElementContainer::Clone(const CElementContainer& selection)
{
	for( vector<std::shared_ptr<CElement>>::const_iterator itSel = selection.m_objects.begin() ; itSel!=selection.m_objects.end() ; itSel++ )
	{
		std::shared_ptr<CElement> pElement = *itSel;
		std::shared_ptr<CElement> pNewElement = pElement->MakeCopy();
		pNewElement->SetGuid();
		pNewElement->SetName();
		m_objects.push_back(pNewElement);
	}
}

void CElementContainer::CopyWithAttributesChange(const CElementContainer& selection)
{
	for( vector<std::shared_ptr<CElement>>::const_iterator itSel = selection.m_objects.begin() ; itSel!=selection.m_objects.end() ; itSel++ )
	{
		std::shared_ptr<CElement> pElement = *itSel;
		pElement->SetName();
		pElement->SetGuid();
		m_objects.push_back(pElement);
	}
		
}

void CElementContainer::ChangeInnerAttributes()
{
	for( vector<std::shared_ptr<CElement>>::const_iterator itSel = m_objects.begin() ; itSel!=m_objects.end() ; itSel++ )
	{
		std::shared_ptr<CElement> pElement = *itSel;
		pElement->SetName();
		pElement->SetGuid();
	}
}

void CElementContainer::RemoveAll()
{
	m_objects.clear();
}

void CElementContainer::Remove(std::shared_ptr<CElement> pElement)
{
	for( vector<std::shared_ptr<CElement>>::iterator i = m_objects.begin() ; i!=m_objects.end() ; i++ )
	{
		std::shared_ptr<CElement> pObj = *i;
		if( pObj->m_objectId == pElement->m_objectId )
		{
			m_objects.erase(i);
			break;
			//delete pObj;
		}
	}
}

void CElementContainer::AddTail(std::shared_ptr<CElement> pElement)
{
	m_objects.push_back(pElement);
}

void CElementContainer::DebugDumpObjects(CModeler1View * pView)
{
	CString str;
	for( vector<std::shared_ptr<CElement>>::iterator i = m_objects.begin() ; i!=m_objects.end() ; i++ )
	{
		std::shared_ptr<CElement> pElement = *i;
		str.Format(_T("dump %s"), pElement->ToString());
		pView->LogDebug(str);
	}
}
