#include "stdafx.h"
#include "ElementManager.h"
#include "Modeler1View.h"
#include "MainFrm.h"
#include "DrawingContext.h"
#include "DrawingElements.h"
#include "GroupContainer.h"

//
// CGroupContainer Class
//

IMPLEMENT_SERIAL(CGroupContainer, CObject, VERSIONABLE_SCHEMA | 9)

CGroupContainer::CGroupContainer()
{
}

CGroupContainer::~CGroupContainer(void)
{
}

void CGroupContainer::Serialize(CElementManager* pElementManager, CArchive& ar)
{
	USES_CONVERSION;

	AfxMessageBox(_T("CGroupContainer::Serialize"));

	if (ar.IsStoring() == TRUE)
	{
		//
		// Set version of file format
		//
		//ar.SetObjectSchema(9);

		CString name = W2T((LPTSTR)m_name.c_str());
		ar << name;
		CString elements = W2T((LPTSTR)m_elements.c_str());
		ar << elements;
	}
	else
	{
		int version = ar.GetObjectSchema();

		//if (version >= 9)
		{
			CString name;
			ar >> name;
			this->m_name = T2W((LPTSTR)(LPCTSTR)name);
			CString elements;
			ar >> elements;
			this->m_elements = T2W((LPTSTR)(LPCTSTR)elements);
		}
	}
}

std::vector<std::wstring> CGroupContainer::Split(const std::wstring& s, wchar_t delim)
{
	std::wstringstream ss(s);
	std::wstring item;
	std::vector<std::wstring> elems;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
		// elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
	}
	return elems;
}


//
// CGroupConnection Class
//

IMPLEMENT_SERIAL(CGroupConnection, CObject, VERSIONABLE_SCHEMA | 9)

CGroupConnection::CGroupConnection()
{
}

CGroupConnection::~CGroupConnection(void)
{
}

void CGroupConnection::Serialize(CElementManager* pElementManager, CArchive& ar)
{
	USES_CONVERSION;

	if (ar.IsStoring() == TRUE)
	{
		//
		// Set version of file format
		//
		//ar.SetObjectSchema(9);

		CTypedPtrList<CObList, CGroupContainer*> objects;
		for (vector<std::shared_ptr<CGroupContainer>>::iterator i = m_objects.begin(); i != m_objects.end(); i++)
		{
			std::shared_ptr<CGroupContainer> pGroupContainer = *i;

			objects.AddTail(pGroupContainer.get());
		}

		objects.Serialize(ar);
	}
	else
	{
		int version = ar.GetObjectSchema();

		//if (version >= 9)
		{
			CTypedPtrList<CObList, CGroupContainer*> objects;
			objects.Serialize(ar);

			POSITION pos = objects.GetHeadPosition();
			while (pos != NULL)
			{
				CGroupContainer* pGroupContainer = objects.GetNext(pos);

				std::shared_ptr<CGroupContainer> pNewGroupContainer = make_shared<CGroupContainer>();
				pNewGroupContainer->m_name = pGroupContainer->m_name;
				pNewGroupContainer->m_elements = pGroupContainer->m_elements;

				vector<wstring> vElements = CGroupContainer::Split(pNewGroupContainer->m_elements, _T('|'));

				for (wstring str : vElements)
				{
					shared_ptr<CElement> pElement = pElementManager->m_objects.FindElementByName(str);
					if (pElement != nullptr)
					{
						pElement->m_pElementGroupContainer = pNewGroupContainer.get();
						pElement->m_bGrouping = true;
						pNewGroupContainer->m_objects.push_back(pElement);
						m_objects.push_back(pNewGroupContainer);
					}
				}

			}
		}
	}
}

