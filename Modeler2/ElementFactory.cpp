#include "StdAfx.h"
#include "ElementFactory.h"
#include "Element.h"
#include "DrawingElements.h"

CElementFactory::CElementFactory(void)
{
}


CElementFactory::~CElementFactory(void)
{
}

//
// CFactory Class
//
int CFactory::g_counter = 0;

std::shared_ptr<CElement> CFactory::CreateElementOfType(ElementType type, ShapeType shapeType)
{
	//std::shared_ptr<CElement> pNewElement = new CElement();
	std::shared_ptr<CElement> pNewElement;
	//pNewElement = make_shared<CElement>();
	/*
	#ifdef USE_BOOST
		std::shared_ptr<CElement> pNewElement(new CElement());
	#else
		std::shared_ptr<CElement> pNewElement = make_shared<CElement>();
	#endif
	*/	
	if( type == ElementType::type_shapes_simple )
	{
		switch( shapeType )
		{
			case line:
			case line2:
			case line_right:
			case line_right2:
			case line_left_right:
			{
				std::shared_ptr<CElement> apNewElement(new CLineElement());
				pNewElement = apNewElement;
				break;
			}
			case rectangle:
			case rectangle_round:
			{
				std::shared_ptr<CElement> apNewElement(new CRectangleElement());
				pNewElement = apNewElement;
				break;
			}
			case triangle:
			{
				std::shared_ptr<CElement> apNewElement(new CTriangleElement());
				pNewElement = apNewElement;
				break;
			}
			case arrow_right:
			{
				std::shared_ptr<CElement> apNewElement(new CArrowRightElement());
				pNewElement = apNewElement;
				break;
			}
			case arrow_down:
			{
				std::shared_ptr<CElement> apNewElement(new CArrowDownElement());
				pNewElement = apNewElement;
				break;
			}
			case ellipse:
			{
				std::shared_ptr<CElement> apNewElement(new CEllipseElement());
				pNewElement = apNewElement;
				break;
			}
			case parenthese_left:
			{
				std::shared_ptr<CElement> apNewElement(new CParentheseLeftElement());
				pNewElement = apNewElement;
				break;
			}
			case parenthese_right:
			{
				std::shared_ptr<CElement> apNewElement(new CParentheseRightElement());
				pNewElement = apNewElement;
				break;
			}
			case star:
			{
				std::shared_ptr<CElement> apNewElement(new CStarElement());
				pNewElement = apNewElement;
				break;
			}
			case line_broken:
			case line_broken_right:
			{
				std::shared_ptr<CElement> apNewElement(new CLineBrokenElement());
				pNewElement = apNewElement;
				break;
			}
			case notimp_igloo:
			case notimpl_multi_line:
			default:
			{
				std::shared_ptr<CElement> apNewElement(new CNotImplementedElement());
				// Not implemented yet !
				pNewElement->m_type = ElementType::type_unknown;
				pNewElement = apNewElement;
				break;
			}
		}
	}
	
	if( type == ElementType::type_image )
	{
		switch( shapeType )
		{
			case image_fixed:
			case image_scaled:
			{
				std::shared_ptr<CElement> apNewElement(new CImageElement());
				pNewElement = apNewElement;
				break;
			}
			default:
			{
				std::shared_ptr<CElement> apNewElement(new CNotImplementedElement());
				pNewElement = apNewElement;
				// Not implemented yet !
				pNewElement->m_type = ElementType::type_unknown;
				break;
			}
		}
	}

	if( type == ElementType::type_shapes_infrastructure )
	{
		std::shared_ptr<CElement> apNewElement(new CImageElement());
		pNewElement = apNewElement;
		// No line around the object
		pNewElement->m_bColorLine = FALSE;
		
		switch( shapeType )
		{
			case ShapeType::infrastructure_server_ad:
				pNewElement->m_image = L"ServerAD.png";
				break;
			case ShapeType::infrastructure_server:
				pNewElement->m_image = L"Server.png";
				break;
			case ShapeType::infrastructure_server_web:
				pNewElement->m_image = L"ServerWeb.png";
				break;
			case ShapeType::infrastructure_server_database:
				pNewElement->m_image = L"ServerDatabase.png";
				break;
			case ShapeType::infrastructure_workstation:
				pNewElement->m_image = L"Workstation.png";
				break;
			case ShapeType::infrastructure_laptop:
				pNewElement->m_image = L"Laptop.png";
				break;
			case ShapeType::infrastructure_firewall:
				pNewElement->m_image = L"Firewall.png";
				break;
			case ShapeType::infrastructure_network:
				pNewElement->m_image = L"network.png";
				break;
			case infrastructure_virtual_server:
				pNewElement->m_image = L"Server_VM.png";
				break;
			case infrastructure_virtual_server_web:
				pNewElement->m_image = L"ServerWeb_VM.png";
				break;
			case infrastructure_virtual_server_database:
				pNewElement->m_image = L"ServerDatabase_VM.png";
				break;
			case infrastructure_virtualization_server:
				pNewElement->m_image = L"Virtualization_Server.png";
				break;
			case infrastructure_server_ad_grey:
				pNewElement->m_image = L"ServerAD_grey.png";
				break;
			case infrastructure_server_grey:
				pNewElement->m_image = L"Server_grey.png";
				break;
			case infrastructure_server_database_grey:
				pNewElement->m_image = L"ServerDatabase_grey.png";
				break;
			case infrastructure_server_farm:
				pNewElement->m_image = L"Server_Farm.png";
				break;
			case infrastructure_workstation_grey:
				pNewElement->m_image = L"Workstation_grey.png";
				break;
			case infrastructure_laptop_grey:
				pNewElement->m_image = L"Laptop_grey.png";
				break;
			default:
				// Not implemented yet !
				pNewElement->m_type = ElementType::type_unknown;
				break;
		}
	}

	if( type == ElementType::type_shapes_development )
	{
		std::shared_ptr<CElement> apNewElement(new CDevelopmentElement());
		pNewElement = apNewElement;
		pNewElement->m_bColorLine = true;
		pNewElement->m_bColorFill = true;
		pNewElement->m_bSolidColorFill = true;

		// Set colors for development shapes
		Color colorLineClass(255, 52, 101, 164);
		Color colorFillClass(255, 200, 210, 233);
		Color colorLineInterface(255, 176, 167, 129);
		Color colorFillInterface(255, 217, 230, 203);
		Color colorLineEnumeration(255, 112, 88, 41);
		Color colorFillEnumeration(255, 175, 165, 120);
		Color colorLinePackage(255, 191, 161, 132);
		Color colorFillPackage(255, 255, 251, 240);
		Color colorLineComment(255, 202, 178, 45);
		Color colorFillComment(255, 251, 247, 200);
		Color colorLineComponent(255, 83, 134, 191);
		Color colorFillComponent(255, 181, 210, 237);

		switch( shapeType )
		{
			case development_class:
				pNewElement->m_text = L"Class";
				pNewElement->m_colorFill = colorFillClass.ToCOLORREF();
				pNewElement->m_colorLine = colorLineClass.ToCOLORREF();
				break;
			case development_interface:
				pNewElement->m_text = L"Interface";
				pNewElement->m_colorFill = colorFillInterface.ToCOLORREF();
				pNewElement->m_colorLine = colorLineInterface.ToCOLORREF();
				break;
			case development_enumeration:
				pNewElement->m_text = L"Enumeration";
				pNewElement->m_colorFill = colorFillEnumeration.ToCOLORREF();
				pNewElement->m_colorLine = colorLineEnumeration.ToCOLORREF();
				break;
			case development_package:
				pNewElement->m_text = L"Package";
				pNewElement->m_colorFill = colorFillPackage.ToCOLORREF();
				pNewElement->m_colorLine = colorLinePackage.ToCOLORREF();
				break;
			case development_comment:
				pNewElement->m_text = L"Comment";
				pNewElement->m_colorFill = colorFillComment.ToCOLORREF();
				pNewElement->m_colorLine = colorLineComment.ToCOLORREF();
				break;
			case development_component:
				pNewElement->m_text = L"Component";
				pNewElement->m_colorFill = colorFillComponent.ToCOLORREF();
				pNewElement->m_colorLine = colorLineComponent.ToCOLORREF();
				break;
			case development_association:
			case development_aggregation:
			case development_composition:
			case development_dependency:
			case development_inheritance:
			case development_package_import:
			case development_connector:
				break;
		}
	}
	
	if (type == ElementType::type_shapes_import)
	{
		std::shared_ptr<CElement> apNewElement = make_shared<CImportElement>();
		pNewElement = apNewElement;
		pNewElement->m_bColorLine = true;
		pNewElement->m_bColorFill = true;
		pNewElement->m_bSolidColorFill = true;

		// Set same colors like development shapes
		Color colorLineClass(255, 52, 101, 164);
		Color colorFillClass(255, 200, 210, 233);

		switch (shapeType)
		{
		case import_class :
			pNewElement->m_text = L"";
			pNewElement->m_colorFill = colorFillClass.ToCOLORREF();
			pNewElement->m_colorLine = colorLineClass.ToCOLORREF();
			break;
		}
	}

	if( type == ElementType::type_text )
	{
		std::shared_ptr<CElement> apNewElement(new CTextElement());
		pNewElement = apNewElement;

		pNewElement->m_bColorLine = true;
		pNewElement->m_bColorFill = false;
		pNewElement->m_bSolidColorFill = true;
		pNewElement->m_text = L"<type text>";
	}

	if( type == ElementType::type_image )
	{
		std::shared_ptr<CElement> apNewElement(new CImageElement());
		pNewElement = apNewElement;
		// No line around the object
		pNewElement->m_bColorLine = FALSE;
	}
	
	//
	// Initialize default members for the element
	//
	pNewElement->m_type = type;
	pNewElement->m_shapeType = shapeType;
	return pNewElement;
}

//
// CGuid Class
//

CGuid::CGuid()
{
#ifdef GUID_TRUE
	::UuidCreateSequential(&m_uuid);
#endif
}

CGuid::~CGuid(void)
{
}

CString CGuid::ToString()
{
#ifdef GUID_TRUE
	unsigned char * sz;
	::UuidToStringA(&m_uuid, &sz);
	CString str((const unsigned char *)sz);
	RpcStringFree(&sz);
	return str;
#else
	return _T("");
#endif
}


