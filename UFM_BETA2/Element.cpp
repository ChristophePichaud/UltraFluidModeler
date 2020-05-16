// Copyright (C) SAS NET Azure Rangers
// All rights reserved.

#include "StdAfx.h"
#include "Element.h"
#include "ElementFactory.h"
#include "Modeler1Doc.h"
#include "Modeler1View.h"
#include "MainFrm.h"
#include "DrawingContext.h"
#include "DrawingElements.h"

// 
// CShapeType
//
ShapeType CShapeType::ToShapeType(int value)
{
	ShapeType ret = ShapeType::unknown;
	switch( value)
	{
		case line:
		case line_right:
		case rectangle:
		case ellipse:
		case rectangle_round:
		case triangle:
		case line_broken:
		case line_broken_right:
		case notimp_igloo:
		case notimpl_multi_line:
		case arrow_right:
		case arrow_down:
		case parenthese_left:
		case parenthese_right:
		case courbe1:
		case courbe2:
		case line2:
		case line_right2:
		case line_left_right:
		case line_broken2:
		case line_broken_right2:
		case line_broken_left_right:
		case star:
		case curve:
		case curve_right:
		case curve_left_right:
		case patate:
		case courbe2_bis:
		case curve_fuzzy:
		case basic_rectangle:
		case basic_rectangle2:
		case basic_rectangle3:
		case basic_losange:
		case basic_round_rectangle:
		case basic_database:
			ret = (ShapeType)value;
			break;
		case image_fixed:
		case image_scaled:
			ret = (ShapeType)value;
			break;
		case infrastructure_server_ad:
		case infrastructure_server:
		case infrastructure_server_web:
		case infrastructure_server_database:
		case infrastructure_workstation:
		case infrastructure_laptop:
		case infrastructure_firewall:
		case infrastructure_network:
		case infrastructure_virtual_server:
		case infrastructure_virtual_server_web:
		case infrastructure_virtual_server_database:
		case infrastructure_virtualization_server:
		case infrastructure_server_ad_grey:
		case infrastructure_server_grey:
		case infrastructure_server_database_grey:
		case infrastructure_server_farm:
		case infrastructure_workstation_grey:
		case infrastructure_laptop_grey:
			ret = (ShapeType)value;
			break;
		case text:
		case text_left:
		case text_center:
		case text_right:
		case text_justify:
			ret = (ShapeType)value;
			break;	
		case development_class:
		case development_interface:
		case development_enumeration:
		case development_package:
		case development_comment:
		case development_association:
		case development_aggregation:
		case development_composition:
		case development_dependency:
		case development_inheritance:
		case development_package_import:
		case development_connector:
		case development_component:
			ret = (ShapeType)value;
			break;
		case planning_month:
		case planning_task:
			ret = (ShapeType)value;
		default: break;
	}
	return ret;
}

//
// CElement Class
//

IMPLEMENT_SERIAL(CElement, CObject, VERSIONABLE_SCHEMA | 16)

int CElement::m_counter = 0;
std::wstring CElement::m_elementGroupNames = _T("");
std::wstring CElement::m_elementGroupElements = _T("");
std::wstring CElement::m_team = _T("");
std::wstring CElement::m_authors = _T("");
bool CElement::m_bShowElementName = false;
COLORREF CElement::m_standardShapesTextColor = RGB(0, 128, 255);
COLORREF CElement::m_connectorShapesTextColor = RGB(0, 255, 54);
bool CElement::m_bShowConnectors = false;

CElement::CElement()
{
	m_pManager = NULL;
	m_pView = NULL;

	m_shapeType = ShapeType::unknown;
	m_rect.SetRectEmpty();
	m_point = CPoint(0, 0);
	m_last = m_point;
	m_text = _T("");
	m_textConnector1 = _T("");
	m_textConnector2 = _T("");
	m_code = _T("");
	m_textAlign = _T("None");
	m_bColorFill = true;
	m_colorFill = RGB(154, 200, 249);
	m_bSolidColorFill = false;
	m_bColorLine = true;
	m_colorLine = RGB(0, 0, 0);
	m_bLineWidth = true;
	m_lineWidth = 1;
	m_image = _T("");
	m_bFixed = true;
	m_fontName = _T("Calibri");
	m_fontSize = 12;
	m_bBold = false;
	m_bItalic = false;
	m_bUnderline = false;
	m_bStrikeThrough = false;
	m_colorText = RGB(0, 0, 0);
	m_leftMargin = 10;
	m_topMargin = 10;

	m_pConnector = make_shared<CConnector>();
	m_pConnector->m_pElement1 = nullptr;
	m_pConnector->m_pElement2 = nullptr;
	m_connectorName1 = _T("");
	m_connectorName2 = _T("");
	m_connectorDragHandle1 = 0;
	m_connectorDragHandle2 = 0;
		
	m_document = _T("");
	m_documentType = DocumentType::document_none;
	m_documentTypeText = _T("None");
	m_version = _T("");
	m_product = _T("");

	m_rotateAngle = 0;

	//m_bControlsCreated = false;

	m_bMoving = false;

	m_type = ElementType::type_unknown;
	m_shapeType = ShapeType::unknown;

	SetGuid();
	SetName();
}

CElement::~CElement(void)
{
}

void CElement::SetGuid()
{
	USES_CONVERSION;

	m_counter++;

	CGuid guid;
#ifdef GUID_TRUE
	m_objectId = guid.ToString();
#else
	SYSTEMTIME st;
	GetSystemTime(&st);
	CTime dt = CTime::GetCurrentTime();
	CString objectId;
	objectId.Format(_T("guid_%d_%s_%03d_%ld"), m_counter, dt.Format(_T("%H%M%S")), st.wMilliseconds, GetTickCount());
	this->m_objectId = T2W((LPTSTR)(LPCTSTR)objectId);
#endif
}

void CElement::SetName()
{
	USES_CONVERSION;

	// element name
	CTime dt = CTime::GetCurrentTime();
	CString name; 
	name.Format(_T("Element n_%d"), CFactory::g_counter); //_%s.%03d", dt.Format("%H%M%S"), tm.wMilliseconds);
	this->m_name = T2W((LPTSTR)(LPCTSTR)name);
}

std::shared_ptr<CElement> CElement::MakeCopy()
{
		std::shared_ptr<CElement> pNewElement = CFactory::CreateElementOfType((ElementType)this->m_type,
																			(ShapeType)this->m_shapeType);
		pNewElement->m_name = this->m_name;
		pNewElement->m_objectId = this->m_objectId;
		pNewElement->m_caption = this->m_caption;
		pNewElement->m_text = this->m_text;
		pNewElement->m_code = this->m_code;
		pNewElement->m_image = this->m_image;
		pNewElement->m_lineWidth = this->m_lineWidth;
		pNewElement->m_pManager = this->m_pManager;
		pNewElement->m_pView = this->m_pView;
		pNewElement->m_rect = this->m_rect;
		pNewElement->m_bColorFill = m_bColorFill;
		pNewElement->m_bColorLine = m_bColorLine;
		pNewElement->m_bLineWidth = m_bLineWidth;
		pNewElement->m_bSolidColorFill = m_bSolidColorFill;
		pNewElement->m_colorFill = m_colorFill;
		pNewElement->m_colorLine = m_colorLine;
		pNewElement->m_textAlign = m_textAlign;
		pNewElement->m_fontName = m_fontName;
		pNewElement->m_bFixed = m_bFixed;
		pNewElement->m_bBold = m_bBold;
		pNewElement->m_bItalic = m_bItalic;
		pNewElement->m_bUnderline = m_bUnderline;
		pNewElement->m_bStrikeThrough = m_bStrikeThrough;
		pNewElement->m_code = m_code;
		pNewElement->m_fontSize = m_fontSize;
		pNewElement->m_leftMargin = m_leftMargin;
		pNewElement->m_topMargin = m_topMargin;
		pNewElement->m_colorText = m_colorText;
		pNewElement->m_document = m_document;
		pNewElement->m_elementGroupNames = m_elementGroupNames;
		pNewElement->m_elementGroupElements = m_elementGroupElements;
		pNewElement->m_documentType = m_documentType;
		pNewElement->m_connectorDragHandle1 = m_connectorDragHandle1;
		pNewElement->m_connectorDragHandle2 = m_connectorDragHandle2;
		pNewElement->m_pConnector->m_pElement1 = m_pConnector->m_pElement1;
		pNewElement->m_pConnector->m_pElement2 = m_pConnector->m_pElement2;
		pNewElement->m_version = m_version;
		pNewElement->m_product= m_product;
		pNewElement->m_rotateAngle = m_rotateAngle;
		pNewElement->m_bShowElementName = m_bShowElementName;
		return pNewElement;
}

CElement::CElement(const CElement& element)
{
	this->m_name = element.m_name;
	this->m_objectId = element.m_objectId;
	this->m_caption = element.m_caption;
	this->m_text = element.m_text;
	this->m_code = element.m_code;
	this->m_image = element.m_image;
	this->m_lineWidth = element.m_lineWidth;
	this->m_pManager = element.m_pManager;
	this->m_pView = element.m_pView;
	this->m_rect = element.m_rect;
	this->m_bColorFill = element.m_bColorFill;
	this->m_bColorLine = element.m_bColorLine;
	this->m_bLineWidth = element.m_bLineWidth;
	this->m_bSolidColorFill = element.m_bSolidColorFill;
	this->m_colorFill = element.m_colorFill;
	this->m_colorLine = element.m_colorLine;
	this->m_textAlign = element.m_textAlign;
	this->m_fontName = element.m_fontName;
	this->m_bFixed = element.m_bFixed;
	this->m_bBold = element.m_bBold;
	this->m_bItalic = element.m_bItalic;
	this->m_bUnderline = element.m_bUnderline;
	this->m_bStrikeThrough = element.m_bStrikeThrough;
	this->m_code = element.m_code;
	this->m_fontSize = element.m_fontSize;
	this->m_colorText = element.m_colorText;
	this->m_leftMargin = element.m_leftMargin;
	this->m_topMargin = element.m_topMargin;
	this->m_document = element.m_document;
	this->m_elementGroupNames = element.m_elementGroupNames;
	this->m_elementGroupElements = element.m_elementGroupElements;
	this->m_documentType = element.m_documentType;
	this->m_connectorDragHandle1 = element.m_connectorDragHandle1;
	this->m_connectorDragHandle2 = element.m_connectorDragHandle2;
	this->m_version = element.m_version;
	this->m_product = element.m_product;
	this->m_rotateAngle = element.m_rotateAngle;
	this->m_bShowElementName = element.m_bShowElementName;
}

CString CElement::ToString(shared_ptr<CElement> pElement)
{
	CString str = pElement == nullptr ? _T("") : W2T((LPTSTR)pElement->m_name.c_str());
	return str;
}

shared_ptr<CElement> CElement::FromJSON(const web::json::object& object)
{
	ElementType type = (ElementType)(object.at(U("Type")).as_integer());
	ShapeType shapeType = (ShapeType)(object.at(U("ShapeType")).as_integer());

	//shared_ptr<CElement> pElement = make_shared<CElement>();
	std::shared_ptr<CElement> pElement = CFactory::CreateElementOfType(type, shapeType);

	pElement->m_objectId = object.at(U("ObjectId")).as_string();
	pElement->m_name = object.at(U("Name")).as_string();
	pElement->m_caption = object.at(U("Caption")).as_string();
	pElement->m_text = object.at(U("Text")).as_string();
	pElement->m_code = object.at(U("Code")).as_string();
	pElement->m_type = (ElementType)(object.at(U("Type")).as_integer());
	pElement->m_shapeType = (ShapeType)(object.at(U("ShapeType")).as_integer());
	int left = object.at(U("Rect.left")).as_integer();
	int right = object.at(U("Rect.right")).as_integer();
	int top = object.at(U("Rect.top")).as_integer();
	int bottom = object.at(U("Rect.bottom")).as_integer();
	CRect rect(left, top, right, bottom);
	pElement->m_rect = rect;
	int px = object.at(U("Point.x")).as_integer();
	int py = object.at(U("Point.y")).as_integer();
	CPoint point(px, py);
	pElement->m_last = point;
	pElement->m_colorFill = (COLORREF)(object.at(U("ColorFill")).as_integer());
	pElement->m_colorLine = (COLORREF)(object.at(U("ColorLine")).as_integer());
	pElement->m_lineWidth = (COLORREF)(object.at(U("LineWidth")).as_integer());
	pElement->m_image = object.at(U("Image")).as_string();;
	pElement->m_bColorFill = (object.at(U("bColorFill")).as_integer()) == 1 ? true: false;
	pElement->m_bColorLine = (object.at(U("bColorLine")).as_integer()) == 1 ? true : false;
	pElement->m_bLineWidth = (object.at(U("bLineWidth")).as_integer()) == 1 ? true : false;
	pElement->m_textAlign = object.at(U("TextAlign")).as_string();
	pElement->m_fontName = object.at(U("FontName")).as_string();;
	pElement->m_bFixed = object.at(U("bFixed")).as_integer();
	pElement->m_bBold = object.at(U("bBold")).as_integer();
	pElement->m_bItalic = object.at(U("bItalic")).as_integer();
	pElement->m_bUnderline = object.at(U("bUnderline")).as_integer();
	pElement->m_bStrikeThrough = object.at(U("bStrikeThrough")).as_integer();
	pElement->m_code = object.at(U("Code")).as_string();;
	pElement->m_fontSize = object.at(U("FontSize")).as_integer();
	pElement->m_colorText = (COLORREF)(object.at(U("ColorText")).as_integer());
	pElement->m_document = object.at(U("Document")).as_string();
	pElement->m_documentType = (DocumentType)(object.at(U("DocumentType")).as_integer());
	pElement->m_elementGroupNames = object.at(U("GroupNames")).as_string();
	pElement->m_elementGroupElements = object.at(U("GroupElements")).as_string();
	pElement->m_connectorDragHandle1 = object.at(U("ConnectorDragHandle1")).as_integer();
	pElement->m_connectorDragHandle2 = object.at(U("ConnectorDragHandle2")).as_integer();
	pElement->m_connectorName1 = object.at(U("ConnectorName1")).as_string();
	pElement->m_connectorName2 = object.at(U("ConnectorName2")).as_string();
	pElement->m_version = object.at(U("Version")).as_string();
	pElement->m_product = object.at(U("Product")).as_string();
	pElement->m_team = object.at(U("Team")).as_string();
	pElement->m_authors = object.at(U("Authors")).as_string();
	pElement->m_leftMargin = object.at(U("LeftMargin")).as_integer();
	pElement->m_topMargin = object.at(U("TopMargin")).as_integer();
	pElement->m_rotateAngle = object.at(U("RotationAngle")).as_integer();
	pElement->m_bShowElementName = object.at(U("bViewName")).as_integer();

	return pElement;
}

web::json::value CElement::AsJSON() const
{
	web::json::value res = web::json::value::object();
	res[U("ObjectId")] = web::json::value::string(m_objectId);
	res[U("Name")] = web::json::value::string(m_name);
	res[U("Caption")] = web::json::value::string(m_caption);
	res[U("Text")] = web::json::value::string(m_text);
	res[U("Code")] = web::json::value::string(m_code);
	res[U("Type")] = web::json::value::number(m_type);
	res[U("ShapeType")] = web::json::value::number(m_shapeType);
	res[U("Rect.left")] = web::json::value::number(m_rect.left);
	res[U("Rect.right")] = web::json::value::number(m_rect.right);
	res[U("Rect.top")] = web::json::value::number(m_rect.top);
	res[U("Rect.bottom")] = web::json::value::number(m_rect.bottom);
	res[U("Point.x")] = web::json::value::number(m_point.x);
	res[U("Point.y")] = web::json::value::number(m_point.y);
	res[U("Last.x")] = web::json::value::number(m_last.x);
	res[U("Last.y")] = web::json::value::number(m_last.y);
	res[U("ColorFill")] = web::json::value::number((int)m_colorFill);
	res[U("ColorLine")] = web::json::value::number((int)m_colorLine);
	res[U("LineWidth")] = web::json::value::number((int)m_lineWidth);
	res[U("Image")] = web::json::value::string(m_image);
	res[U("bColorFill")] = web::json::value::number((int)m_bColorFill);
	res[U("bColorLine")] = web::json::value::number((int)m_bColorLine);
	res[U("bLineWidth")] = web::json::value::number((int)m_bLineWidth);
	res[U("TextAlign")] = web::json::value::string(m_textAlign);
	res[U("FontName")] = web::json::value::string(m_fontName);
	res[U("bFixed")] = web::json::value::number((int)m_bFixed);
	res[U("bBold")] = web::json::value::number((int)m_bBold);
	res[U("bItalic")] = web::json::value::number((int)m_bItalic);
	res[U("bUnderline")] = web::json::value::number((int)m_bUnderline);
	res[U("bStrikeThrough")] = web::json::value::number((int)m_bStrikeThrough);
	res[U("Code")] = web::json::value::string(m_code);
	res[U("FontSize")] = web::json::value::number(m_fontSize);
	res[U("ColorText")] = web::json::value::number((int)m_colorText);
	res[U("Document")] = web::json::value::string(m_document);
	res[U("DocumentType")] = web::json::value::number(m_documentType);
	res[U("GroupNames")] = web::json::value::string(m_elementGroupNames);
	res[U("GroupElements")] = web::json::value::string(m_elementGroupElements);
	res[U("ConnectorDragHandle1")] = web::json::value::number(m_connectorDragHandle1);
	res[U("ConnectorDragHandle2")] = web::json::value::number(m_connectorDragHandle2);
	res[U("ConnectorName1")] = web::json::value::string(m_connectorName1);
	res[U("ConnectorName2")] = web::json::value::string(m_connectorName2);
	res[U("Version")] = web::json::value::string(m_version);
	res[U("Product")] = web::json::value::string(m_product);
	res[U("Team")] = web::json::value::string(m_team);
	res[U("Authors")] = web::json::value::string(m_authors);
	res[U("LeftMargin")] = web::json::value::number(m_leftMargin);
	res[U("TopMargin")] = web::json::value::number(m_topMargin);
	res[U("RotationAngle")] = web::json::value::number(m_rotateAngle);
	res[U("bViewName")] = web::json::value::number((int)m_bShowElementName);
	return res;

}

void CElement::Serialize(CArchive& ar)
{
	USES_CONVERSION;

	if (ar.IsStoring())
	{
		// Get reteurn -1 by default
		//int version = ar.GetObjectSchema();
		//CString str;
		//str.Format(_T("version=%d"), version);
		//AfxMessageBox(str);

		//
		// Set version of file format
		//
		ar.SetObjectSchema(16);

		CString text1 = W2T((LPTSTR)m_textConnector1.c_str());
		ar << text1;
		CString text2 = W2T((LPTSTR)m_textConnector2.c_str());
		ar << text2;

		// The schema v16 contains extra info: showElementName
		ar << m_bShowElementName;

		// The schema v15 contains extra info: team, authors
		CString team = W2T((LPTSTR)m_team.c_str());
		ar << team;
		CString authors = W2T((LPTSTR)m_authors.c_str());
		ar << authors;

		// The schema v14 contains extra info: rotateAngle
		ar << m_rotateAngle;

		// The schema v13 contains extra info: leftMargin, topMargin
		ar << m_leftMargin;
		ar << m_topMargin;

		// The schema v12 contains extra info: version, product
		CString version = W2T((LPTSTR)m_version.c_str());
		ar << version;
		CString product = W2T((LPTSTR)m_product.c_str());
		ar << product;

		// The schema v11 contains extra info: connectorDragHandle1 & 2
		ar << m_connectorDragHandle1;
		ar << m_connectorDragHandle2;

		// The schema v10 contains extra info: documenttypetext
		//CString doct = W2T((LPTSTR)m_documentTypeText.c_str());
		//ar << doct;
		int doct = m_documentType;
		ar << doct;

		// The schema v9 contains extra info: names, elements
		CString names = W2T((LPTSTR)CElement::m_elementGroupNames.c_str());
		ar << names;
		CString elements = W2T((LPTSTR)CElement::m_elementGroupElements.c_str());
		ar << elements;

		// The schema v8 contains extra info: document
		CString doc = W2T((LPTSTR)m_document.c_str());
		ar << doc;

		// The schema v7 contains extra info: connector1 & 2
		CString cn1 = CElement::ToString(m_pConnector->m_pElement1);
		ar << cn1;
		CString cn2 = CElement::ToString(m_pConnector->m_pElement2);
		ar << cn2;

		// The schema v6 contains extra info: colortext
		ar << m_colorText;

		// The schema v5 contains extra info: bold, italic, underline, StrikeThrough
		ar << m_bBold;
		ar << m_bItalic;
		ar << m_bUnderline;
		ar << m_bStrikeThrough;

		// The schema v4 contains extra info: code
		CString code = W2T((LPTSTR)m_code.c_str());
		ar << code;

		// The schema v3 contains extra info: fontName, fontSize and Fixed
		CString fontName = W2T((LPTSTR)m_fontName.c_str());
		ar << fontName;
		int fontSize = m_fontSize;
		ar << fontSize;
		ar << m_bFixed;

		// The schema v2 contains extra info: textAlign
		CString textAlign = W2T((LPTSTR)m_textAlign.c_str());
		ar << textAlign;

		CString name = W2T((LPTSTR)m_name.c_str());
		ar << name;
		int type = m_type;
		ar << type;
		int shapeType = m_shapeType;
		ar << shapeType;
		CString objectId = W2T((LPTSTR)m_objectId.c_str());
		ar << objectId;
		CString caption = W2T((LPTSTR)m_caption.c_str());
		ar << caption;
		CString text = W2T((LPTSTR)m_text.c_str());
		ar << text;
		ar << m_rect;
		ar << m_point;
		ar << m_last;
		ar << m_colorFill;
		ar << m_bColorFill;
		ar << m_colorLine;
		ar << m_bColorLine;
		ar << m_bColorLine;
		ar << m_lineWidth;
		CString image = W2T((LPTSTR)m_image.c_str());
		ar << image;
	}
	else
	{
		int version = ar.GetObjectSchema();
		//CString str;
		//str.Format(_T("version=%d"), version);
		//AfxMessageBox(str);


		// get the document back pointer from the archive
		CModeler1Doc * pDocument = (CModeler1Doc*)ar.m_pDocument;
		m_pManager = pDocument->GetManager();


		if (version >= 16)
		{
			CString text1;
			ar >> text1;
			m_textConnector1 = T2W((LPTSTR)(LPCTSTR)text1);
			CString text2;
			ar >> text2;
			m_textConnector2 = T2W((LPTSTR)(LPCTSTR)text2);

			ar >> m_bShowElementName;
		}

		if (version >= 15)
		{
			CString team;
			ar >> team;
			m_team = T2W((LPTSTR)(LPCTSTR)team);
			CString authors;
			ar >> authors;
			m_authors = T2W((LPTSTR)(LPCTSTR)authors);
		}

		if (version >= 14)
		{
			ar >> m_rotateAngle;
		}

		if (version >= 13)
		{
			ar >> m_leftMargin;
			ar >> m_topMargin;
		}

		if (version >= 12)
		{
			CString version;
			ar >> version;
			m_version = T2W((LPTSTR)(LPCTSTR)version);
			CString product;
			ar >> product;
			m_product = T2W((LPTSTR)(LPCTSTR)product);
		}

		if (version >= 11)
		{
			ar >> m_connectorDragHandle1;
			ar >> m_connectorDragHandle2;
		}
			
		if (version >= 10)
		{
			//CString doct;
			//ar >> doct;
			//this->m_documentTypeText = T2W((LPTSTR)(LPCTSTR)doct);
			int doct;
			ar >> doct;
			m_documentType = (DocumentType)doct;
		}

		if (version >= 9)
		{
			CString names;
			ar >> names;
			CElement::m_elementGroupNames = T2W((LPTSTR)(LPCTSTR)names);
			CString elements;
			ar >> elements;
			CElement::m_elementGroupElements = T2W((LPTSTR)(LPCTSTR)elements);
		}

		if (version >= 8)
		{
			CString doc;
			ar >> doc;
			this->m_document = T2W((LPTSTR)(LPCTSTR)doc);
		}

		if (version >= 7)
		{
			CString cn1;
			ar >> cn1;
			this->m_connectorName1 = T2W((LPTSTR)(LPCTSTR)cn1);
			CString cn2;
			ar >> cn2;
			this->m_connectorName2 = T2W((LPTSTR)(LPCTSTR)cn2);
		}

		if (version >= 6)
		{
			ar >> m_colorText;
		}

		if (version >= 5)
		{
			ar >> m_bBold;
			ar >> m_bItalic;
			ar >> m_bUnderline;
			ar >> m_bStrikeThrough;
		}
			
		if (version >= 4)
		{
			CString code;
			ar >> code;
			this->m_code = T2W((LPTSTR)(LPCTSTR)code);
		}
		
		if (version >= 3)
		{
			CString fontName;
			ar >> fontName;
			this->m_fontName = T2W((LPTSTR)(LPCTSTR)fontName);
			int fontSize;
			ar >> fontSize;
			m_fontSize = fontSize;
			ar >> m_bFixed;
		}

		if (version >= 2)
		{
			CString textAlign;
			ar >> textAlign;
			this->m_textAlign = T2W((LPTSTR)(LPCTSTR)textAlign);
		}

		CString name;
		ar >> name;
		this->m_name = T2W((LPTSTR)(LPCTSTR)name);
		int type;
		ar >> type;
		m_type = (ElementType)type;
		int shapeType;
		ar >> shapeType;
		m_shapeType = (ShapeType)shapeType;
		CString objectId;
		ar >> objectId;
		this->m_objectId = T2W((LPTSTR)(LPCTSTR)objectId);
		CString caption;
		ar >> caption;
		this->m_caption = T2W((LPTSTR)(LPCTSTR)caption);
		CString text;
		ar >> text;
		this->m_text = T2W((LPTSTR)(LPCTSTR)text);
		ar >> m_rect;
		ar >> m_point;
		ar >> m_last;
		ar >> m_colorFill;
		ar >> m_bColorFill;
		ar >> m_colorLine;
		ar >> m_bColorLine;
		ar >> m_bColorLine;
		ar >> m_lineWidth;
		CString image;
		ar >> image;
		this->m_image = T2W((LPTSTR)(LPCTSTR)image);
	}
}

CString CElement::ToString()
{
	CString str;
	str.Format(_T("Element name=<%s> id={%s} type=<%s> shape=<%s> rect={%d,%d,%d,%d} caption=<%s> text=<%s> connector=<%s> handle1=<%d> handle2=<%d> image=<%s> colorFill={%03d%03d%03d} colorLine={%03d%03d%03d}"), 
		m_name.c_str(), m_objectId.c_str(), ToString(m_type), ToString(m_shapeType),
		m_rect.left, m_rect.top, m_rect.right, m_rect.bottom,
		m_caption.c_str(), m_text.c_str(), 
		ToString(m_pConnector), 
		m_connectorDragHandle1, m_connectorDragHandle1,
		m_image.c_str(),
		GetRValue(m_colorFill), GetGValue(m_colorFill), GetBValue(m_colorFill),
		GetRValue(m_colorLine), GetGValue(m_colorLine), GetBValue(m_colorLine));
	return str;
}

CString CElement::ToString(shared_ptr<CConnector> pConnector)
{
	CString str = _T("");

	shared_ptr<CElement> pElement1 = pConnector->m_pElement1;
	shared_ptr<CElement> pElement2 = pConnector->m_pElement2;

	str.Format(_T("c1:%s c2:%s"), pElement1 == nullptr ? _T("NULL") : pElement1->m_name.c_str(), 
								  pElement2 == nullptr ? _T("NULL") : pElement2->m_name.c_str());
	return str;
}

CString CElement::ToString(ElementType type)
{
	CString str = _T("");
	switch(type)
	{
		case ElementType::type_unknown:
			str = _T("type_unknown->not implemented yet");
			break;

		case ElementType::type_shapes_simple:
			str = _T("type_shapes_simple");
			break;

		case ElementType::type_image:
			str = _T("type_image");
			break;

		case ElementType::type_shapes_infrastructure:
			str = _T("type_shapes_infrastructure");
			break;
		
		case ElementType::type_text:
			str = _T("type_text");
			break;

		case ElementType::type_shapes_development:
			str = _T("type_shapes_development");
			break;
		
		default:
			break;
	}
	return str;
}

CString CElement::ToString(ShapeType type)
{
	return CElement::static_ToString(type);
}

CString CElement::static_ToString(ShapeType type)
{
	CString str = _T("");
	switch(type)
	{
		case line:
			str = _T("line");
			break;
		case line_right:
			str = _T("line_right");
			break;
		case rectangle:
			str = _T("rectangle");
			break;
		case ellipse:
			str = _T("ellipse");
			break;
		case rectangle_round:
			str = _T("rectangle_round");
			break;
		case triangle:
			str = _T("triangle");
			break;
		case line_broken:
			str = _T("line_broken");
			break;
		case line_broken_right:
			str = _T("line_broken_right");
			break;
		case line2:
			str = _T("line2");
			break;
		case line_right2:
			str = _T("line_right2");
			break;
		case line_left_right:
			str = _T("line_left_right");
			break;
		case arrow_right:
			str = _T("arrow_right");
			break;
		case arrow_down:
			str = _T("arrow_down");
			break;
		case parenthese_left:
			str = _T("parenthese_left");
			break;
		case parenthese_right:
			str = _T("parenthese_right");
			break;
		case star:
			str = _T("star");
			break;

		case curve:
			str = _T("curve");
			break;
		case curve_right:
			str = _T("curve_right");
			break;
		case curve_left_right:
			str = _T("curve_left_right");
			break;
		case courbe2_bis:
			str = _T("courbe2_bis");
			break;
		case patate:
			str = _T("patate");
			break;
		case curve_fuzzy:
			str = _T("curve_fuzzy");
			break;
		case basic_rectangle:
			str = _T("basic_rectangle");
			break;
		case basic_rectangle2:
			str = _T("basic_rectangle2");
			break;
		case basic_rectangle3:
			str = _T("basic_rectangle3");
			break;
		case basic_losange:
			str = _T("basic_losange");
			break;
		case basic_round_rectangle:
			str = _T("basic_round_rectangle");
			break;
		case basic_database:
			str = _T("basic_database");
			break;

		case image_fixed:
			str = _T("image_fixed");
			break;
		case image_scaled:
			str = _T("image_scaled");
			break;

		case infrastructure_server_ad:
			str = _T("infrastructure_server_ad");
			break;
		case infrastructure_server:
			str = _T("infrastructure_server");
			break;
		case infrastructure_server_web:
			str = _T("infrastructure_server_web");
			break;
		case infrastructure_server_database:
			str = _T("infrastructure_server_web");
			break;
		case infrastructure_workstation:
			str = _T("infrastructure_workstation");
			break;
		case infrastructure_laptop:
			str = _T("infrastructure_laptop");
			break;
		case infrastructure_firewall:
			str = _T("infrastructure_firewall");
			break;
		case infrastructure_network:
			str = _T("infrastructure_network");
			break;
		case infrastructure_virtual_server:
			str = _T("infrastructure_virtual_server");
			break;
		case infrastructure_virtual_server_web:
			str = _T("infrastructure_virtual_server_web");
			break;
		case infrastructure_virtual_server_database:
			str = _T("infrastructure_virtual_server_database");
			break;
		case infrastructure_virtualization_server:
			str = _T("infrastructure_virtualization_server");
			break;
		case infrastructure_server_ad_grey:
			str = _T("infrastructure_server_ad");
			break;
		case infrastructure_server_grey:
			str = _T("infrastructure_server");
			break;
		case infrastructure_server_database_grey:
			str = _T("infrastructure_server_database");
			break;
		case infrastructure_server_farm:
			str = _T("infrastructure_server_farm");
			break;
		case infrastructure_workstation_grey:
			str = _T("infrastructure_workstation");
			break;
		case infrastructure_laptop_grey:
			str = _T("infrastructure_laptop");
			break;

		case text:
			str = _T("text");
			break;
		case text_left:
			str = _T("text_left");
			break;
		case text_center:
			str = _T("text_center");
			break;
		case text_right:
			str = _T("text_right");
			break;
		case text_justify:
			str = _T("text_justify");
			break;	

		case development_class:
			str = _T("development_class");
			break;
		case development_interface:
			str = _T("development_interface");
			break;
		case development_enumeration:
			str = _T("development_enumeration");
			break;
		case development_package:
			str = _T("development_package");
			break;
		case development_comment:
			str = _T("development_comment");
			break;
		case development_association:
			str = _T("development_association");
			break;
		case development_aggregation:
			str = _T("development_aggregation");
			break;
		case development_composition:
			str = _T("development_composition");
			break;
		case development_dependency:
			str = _T("development_dependency");
			break;
		case development_inheritance:
			str = _T("development_inheritance");
			break;
		case development_package_import:
			str = _T("development_package_import");
			break;
		case development_connector:
			str = _T("development_connector0");
			break;
		case development_component:
			str = _T("development_component");
			break;

		case planning_month:
			str = _T("planning_month");
			break;
		case planning_task:
			str = _T("planning_task");
			break;
	}
	return str;
}

CString CElement::ToString(DocumentType type)
{
	CString str = _T("");
	switch (type)
	{
	case DocumentType::document_none:
		str = _T("None");
		break;

	case DocumentType::document_file:
		str = _T("File");
		break;

	case DocumentType::document_folder:
		str = _T("Folder");
		break;

	case DocumentType::document_diagram:
		str = _T("Diagram");
		break;

	default:
		break;
	}
	return str;
}

DocumentType CElement::FromString(wstring type)
{
	DocumentType doctype = DocumentType::document_none;
	if (type == _T("None"))
	{
		doctype = DocumentType::document_none;
	}
	if (type == _T("File"))
	{
		doctype = DocumentType::document_file;
	}
	if (type == _T("Folder"))
	{
		doctype = DocumentType::document_folder;
	}
	if (type == _T("Diagram"))
	{
		doctype = DocumentType::document_diagram;
	}
	return doctype;
}

ElementType CElement::From(ShapeType type)
{
/*
	type_unknown,
	type_select,
	type_shapes_simple,
	type_image,
	type_shapes_infrastructure,
	type_text,
	type_shapes_development,
	type_shapes_import,
	type_shapes_planning,
	type_selection,
	type_connection,
	type_file
};

#define OffsetShapes_Simple			0
#define OffsetShapes_Image			50
#define OffsetShapes_Infrastructure 100
#define OffsetShapes_Text			200
#define OffsetShapes_Development	300
#define OffsetShapes_Import			400
#define OffsetShapes_Planning		500
*/

	if (type < OffsetShapes_Image)
	{
		return ElementType::type_shapes_simple;
	}
	if (type < OffsetShapes_Infrastructure)
	{
		return ElementType::type_image;
	}
	if (type < OffsetShapes_Text)
	{
		return ElementType::type_shapes_infrastructure;
	}
	if (type < OffsetShapes_Development)
	{
		return ElementType::type_text;
	}
	if (type < OffsetShapes_Import)
	{
		return ElementType::type_shapes_development;
	}
	if (type < OffsetShapes_Planning)
	{
		return ElementType::type_shapes_import;
	}

	return ElementType::type_shapes_planning;
}

/*
bool CElement::IsDrawable(ElementType type)
{
	bool ret = false;
	switch(type)
	{
		case ElementType::type_rectangle:
		case ElementType::type_class:
		case ElementType::type_line:
		case ElementType::type_ellipse:
		case ElementType::type_image:
		case ElementType::type_shapes:
			ret = true;
		break;
	}
	return ret;
}
*/

bool CElement::CanChangeFillColor() const
{
	return true;
}

bool CElement::CanChangeLineColor() const
{
	return true;
}

bool CElement::CanChangeLineWidth() const
{
	return true;
}

bool CElement::Intersects(const CRect& rect)
{
	CRect fixed = m_rect;
	CSize addSize(5,5);
	fixed.InflateRect(addSize);
	fixed.NormalizeRect();
	CRect rectT = rect;
	rectT.NormalizeRect();
	
// Debug
#ifdef DRAW_EXTERNAL_RECTANGLE_HITS
	//CClientDC dc(this->m_pView);
	//Graphics graphics(dc.m_hDC);
	//
	//Color colorBlack(255, 0, 0, 0);
	//Pen penBlack(colorBlack);

	//CRect rect1 = fixed;
	//CRect rect2 = rectT;
	//CRect rect3 = rect1 & rect2;

	//GetManager()->ManagerToView(this->m_pView, rect1);
	//GetManager()->ManagerToView(this->m_pView, rect2);
	//GetManager()->ManagerToView(this->m_pView, rect3);
	//graphics.DrawRectangle(&penBlack, rect1.left, rect1.top, rect1.Width(), rect1.Height());
	//graphics.DrawRectangle(&penBlack, rect2.left, rect2.top, rect2.Width(), rect2.Height());
	//graphics.DrawRectangle(&penBlack, rect3.left, rect3.top, rect3.Width(), rect3.Height());
#endif
	
	return !(rectT & fixed).IsRectEmpty();
}

bool CElement::IsLine()
{
	if( m_shapeType == ShapeType::line ||  m_shapeType == ShapeType::line2 ||
		m_shapeType == ShapeType::line_left_right ||
		m_shapeType == ShapeType::line_broken ||  m_shapeType == line_broken_right ||
		m_shapeType == ShapeType::line_right || m_shapeType == ShapeType::line_right2 || //)
		m_shapeType == ShapeType::line_left_right ||
		m_shapeType == ShapeType::development_association ||
		m_shapeType == ShapeType::development_aggregation ||
		m_shapeType == ShapeType::development_composition ||
		m_shapeType == ShapeType::development_dependency ||
		m_shapeType == ShapeType::development_inheritance ||
		m_shapeType == ShapeType::development_package_import ||
		m_shapeType == ShapeType::line_broken2 ||
		m_shapeType == ShapeType::line_broken_right2 ||
		m_shapeType == ShapeType::line_broken_left_right )

	{
		return true;
	}
	else
	{
		return false;
	}
}

void CElement::InvalidateObj(void)
{
	this->m_rect.SetRect(this->m_point, this->m_last);
	if( IsLine() )
	{
	}
	else
	{
		// rectangle
		this->m_rect.NormalizeRect();
	}
}

void CElement::CheckForKeepingAMinimumSize()
{
	const int defaultSize = 10;

	// For all types
	if( m_type == type_shapes_simple || m_type == type_image ) //m_type == type_shapes_infrastructure ) //||  )
	{
		if( m_rect.Width() < defaultSize && m_rect.Height() < defaultSize )
		{
			CPoint p1 = m_rect.TopLeft();
			CPoint p2;
			p2.x = p1.x + defaultSize;
			p2.y = p1.y + defaultSize;
			m_rect.SetRect(p1, p2);
		}
	}
}

// Selection & Tracker helpers

int CElement::GetHandleCount()
{
	if( IsLine() )
	{
		return 2; //8;
	}
	else
		return 8;
}

int CElement::HitTest(CPoint point, CModeler1View* pView, bool bSelected)
{
	CString str;
	if (bSelected)
	{
		int nHandleCount = GetHandleCount();
		for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		{
			// GetHandleRect returns in logical coords
			CRect rc = GetHandleRect(nHandle, pView);

			// TODO
			//Gdiplus::Region rc2;
			//rc2.Transform(matrix);
			

			// Debugging
			//str.Format(_T("HitTest try point{%d,%d} in rect{%d,%d,%d,%d}"), point.x, point.y, rc.left, rc.top, rc.right, rc.bottom);
			//pView->LogDebug(str);

			CRect rect(point, CSize(1, 1));
			CRect fixed = rc;
			CSize addSize(5,5);
			fixed.InflateRect(addSize);
			fixed.NormalizeRect();
			if( (rect & fixed).IsRectEmpty() == FALSE )
			{
				// Debugging
				//str.Format("HitTest returns %d", nHanlde);
				//pView->LogDebug(str);
				return nHandle;
			}
		
			//if (point.x >= rc.left && point.x < rc.right && point.y <= rc.top && point.y > rc.bottom)
			//	return nHandle;
		}
	}
	else
	{
		if (point.x >= m_rect.left && point.x < m_rect.right && point.y <= m_rect.top && point.y > m_rect.bottom)
			return 1;
	}

	return 0;
}

// return rectange of handle in logical coords
CRect CElement::GetHandleRect(int nHandleID, CModeler1View* pView)
{
	CRect rect;

	// get the center of the handle in logical coords
	CPoint point = GetHandle(nHandleID);
	// convert to client/device coords
	GetManager()->ManagerToView(pView, point, this);
	//pView->DocToClient(point);
	// return CRect of handle in device coords
	rect.SetRect(point.x-3, point.y-3, point.x+3, point.y+3);
	//pView->ClientToDoc(rect);
	GetManager()->ViewToManager(pView, rect, this);

	return rect;
}

CString CElement::DragHandleToString(int nHandle)
{
	CString str = _T("");

	switch (nHandle)
	{
	case 0:
		str = _T("");
		break;

	case 2:
		str = _T("TopCenter");
		break;

	case 4:
		str = _T("RightCenter");
		break;

	case 6:
		str = _T("BottomCenter");
		break;

	case 8:
		str = _T("LeftCenter");
		break;
	}

	return str;
}

int CElement::DragHandleFromString(wstring value)
{

	int dragHandle = 0;

	if (value == _T(""))
	{
		dragHandle = 0;
	}
		
	if (value == _T("Center"))
	{
		dragHandle = 9;
	}

	if (value == _T("TopCenter"))
	{
		dragHandle = 2;
	}

	if (value == _T("RightCenter"))
	{
		dragHandle = 4;
	}

	if (value == _T("BottomCenter"))
	{
		dragHandle = 6;
	}

	if (value == _T("LeftCenter"))
	{
		dragHandle = 8;
	}

	return dragHandle;
}

// returns logical coords of center of handle
CPoint CElement::GetHandle(int nHandle)
{
	// lines have only two dots tracker !
	if( IsLine() && nHandle == 2 )
		nHandle = 5;
	
	int x, y, xCenter, yCenter;
	CRect rect = m_rect;

	// this gets the center regardless of left/right and top/bottom ordering
	xCenter = rect.left + rect.Width() / 2;
	yCenter = rect.top + rect.Height() / 2;

	switch (nHandle)
	{
	case 1:
		x = rect.left;
		y = rect.top;
		break;

	case 2:
		x = xCenter;
		y = rect.top;
		break;

	case 3:
		x = rect.right;
		y = rect.top;
		break;

	case 4:
		x = rect.right;
		y = yCenter;
		break;

	case 5:
		x = rect.right;
		y = rect.bottom;
		break;

	case 6:
		x = xCenter;
		y = rect.bottom;
		break;

	case 7:
		x = rect.left;
		y = rect.bottom;
		break;

	case 8:
		x = rect.left;
		y = yCenter;
		break;
	}

	return CPoint(x, y);
}

HCURSOR CElement::GetHandleCursor(int nHandle)
{
	// lines have only two dots tracker !
	if( IsLine() && nHandle == 2 )
		nHandle = 5;
	
	LPCTSTR id;
	switch (nHandle)
	{
	case 1:
	case 5:
		id = IDC_SIZENWSE;
		break;

	case 2:
	case 6:
		id = IDC_SIZENS;
		break;

	case 3:
	case 7:
		id = IDC_SIZENESW;
		break;

	case 4:
	case 8:
		id = IDC_SIZEWE;
		break;

	default:
		id = IDC_ARROW;
		//ASSERT(FALSE);
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

// point must be in logical
void CElement::MoveHandleTo(int nHandle, CPoint point, CModeler1View* pView)
{
	// lines have only two dots tracker !
	if( IsLine() && nHandle == 2 )
		nHandle = 5;
	
	CRect position = m_rect;
	switch (nHandle)
	{
	case 1:
		position.left = point.x;
		position.top = point.y;
		break;

	case 2:
		position.top = point.y;
		break;

	case 3:
		position.right = point.x;
		position.top = point.y;
		break;

	case 4:
		position.right = point.x;
		break;

	case 5:
		position.right = point.x;
		position.bottom = point.y;
		break;

	case 6:
		position.bottom = point.y;
		break;

	case 7:
		position.left = point.x;
		position.bottom = point.y;
		break;

	case 8:
		position.left = point.x;
		break;

	default:
		ASSERT(FALSE);
	}

	m_rect = position;

}

void CElement::DrawTracker(CDrawingContext & ctxt, TrackerState state)
{
	Color colorBlack(255, 0, 0, 0);
	SolidBrush solidBrush(colorBlack);
	CRect rect = m_rect;

	switch (state)
	{
	case normal:
		break;

	case selected:
	case active:
		{
			int nHandleCount = GetHandleCount();
			for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
			{
				CPoint handle = GetHandle(nHandle);
				//pDC->PatBlt(handle.x - 3, handle.y - 3, 7, 7, DSTINVERT);
				ctxt.GetGraphics()->FillRectangle(&solidBrush, handle.x - 3, handle.y - 3, 7, 7);
			}
		}
		break;
	}
}

void CElement::DrawTracker(CPoint cnx, CDrawingContext& ctxt,  CModeler1View* pView)
{
	//Color colorBlack(255, 0, 0, 0);
	SolidBrush solidBrush(Color::Yellow);
	CRect rect = m_rect;
	ctxt.GetGraphics()->FillRectangle(&solidBrush, cnx.x - 5, cnx.y - 5, 10, 10);
}

void CElement::DrawTracker(CModeler1View* pView)
{
	CClientDC dc(pView);
	Graphics graphics(dc.m_hDC);
	CDrawingContext ctxt(this);
	ctxt.m_pGraphics = &graphics;

	Matrix matrix;
	CPoint pt = m_rect.CenterPoint();
	PointF point;
	point.X = pt.x;
	point.Y = pt.y;
	matrix.RotateAt(m_rotateAngle, point);
	graphics.SetTransform(&matrix);
	//graphics.RotateTransform(pElement->m_rotateAngle, MatrixOrder::MatrixOrderAppend);
	graphics.ScaleTransform(GetManager()->m_fZoomFactor, GetManager()->m_fZoomFactor);

	DrawTracker(ctxt, TrackerState::active);
}

CRect CElement::GetRectTextConnector(ConnectorType connector)
{
	CPoint p1;
	CPoint p2;
	
	if (connector == ConnectorType::connector1)
	{
		switch (m_connectorDragHandle1)
		{
			// LeftTop
		case 1:
			// TopCenter
		case 2:
			// RightTop
		case 3:
			p1 = CPoint(m_rect.TopLeft().x, m_rect.TopLeft().y - 30);
			p2 = CPoint(m_rect.TopLeft().x + 200, m_rect.TopLeft().y);
			break;

			//RightCenter
		case 4:
			//RightBottom
		case 5:
			//BottomCenter
		case 6:
			//LeftBottom
		case 7:
			p1 = CPoint(m_rect.TopLeft().x, m_rect.TopLeft().y);
			p2 = CPoint(m_rect.TopLeft().x + 200, m_rect.TopLeft().y + 30);
			break;

			//LeftCenter
		case 8:
		default:
			p1 = CPoint(m_rect.TopLeft().x - 50, m_rect.TopLeft().y);
			p2 = CPoint(m_rect.TopLeft().x + 150, m_rect.TopLeft().y + 30);
			break;
		}

		//p1 = CPoint(m_rect.TopLeft().x, m_rect.TopLeft().y);
		//p2 = CPoint(m_rect.TopLeft().x + 200, m_rect.TopLeft().y + 30);
	}
	
	if (connector == ConnectorType::connector2)
	{
		switch (m_connectorDragHandle2)
		{
			// LeftTop
		case 1:
			// TopCenter
		case 2:
			// RightTop
		case 3:
			p1 = CPoint(m_rect.BottomRight().x, m_rect.BottomRight().y - 30);
			p2 = CPoint(m_rect.BottomRight().x + 200, m_rect.BottomRight().y);
			break;

			//RightCenter
		case 4:
			//RightBottom
		case 5:
			//BottomCenter
		case 6:
			//LeftBottom
		case 7:
			p1 = CPoint(m_rect.BottomRight().x, m_rect.BottomRight().y);
			p2 = CPoint(m_rect.BottomRight().x + 200, m_rect.BottomRight().y + 30);
			break;

			//LeftCenter
		case 8:
		default:
			p1 = CPoint(m_rect.BottomRight().x - 50, m_rect.BottomRight().y);
			p2 = CPoint(m_rect.BottomRight().x + 150, m_rect.BottomRight().y + 30);
			break;
		}

		//	p1 = CPoint(m_rect.BottomRight().x, m_rect.BottomRight().y);
		//	p2 = CPoint(m_rect.BottomRight().x + 200, m_rect.BottomRight().y + 30);
	}

	CRect rect(p1, p2);
	return rect;
}

std::wstring CElement::GetImageFilePath()
{
	std::wstring ret = L"";
	
	// FIXME
	std::wstring strShapePath = L"Images\\Shapes\\";
	if( this->m_type == ElementType::type_shapes_infrastructure )
	{
		ret = strShapePath + m_image;
	}
	else
	{
		ret = m_image;
	}
	return ret;
}

void CElement::Draw(CDrawingContext & ctxt)
{
	// Nothing to implement
	Color colorBlack(255, 0, 0, 0);
	Pen penBlack(colorBlack);
	CRect rect = m_rect;
	ctxt.GetGraphics()->DrawRectangle(&penBlack, rect.left, rect.top, rect.Width(), rect.Height());

	// Debug Only
	CString str;
	str.Format(_T("CElement::Draw()"));
	m_pView->LogDebug(str);
	//AfxMessageBox(str);
}

void CallDraw(CDrawingContext & ctxt, std::shared_ptr<CElement> pElement)
{
	pElement->Draw(ctxt);
}

void CElement::Draw(CModeler1View * pView, CDC* pDC)
{
	Graphics graphics(pDC->m_hDC);
	//graphics.ScaleTransform(0.75f, 0.75f);
	graphics.ScaleTransform(GetManager()->m_fZoomFactor, GetManager()->m_fZoomFactor);

	//graphics.SetRenderingOrigin(0, 0);
	//graphics.RotateTransform(45.0);
	//graphics.RotateTransform(5.0f); //, MatrixOrderAppend);  // then rotate
	
#ifdef DRAW_DIRECLY_A_LINE
	{
		if( m_type == ElementType::type_shapes_simple )
		{	
			if( IsLine() )
			{
				CRect rect = m_rect;
				//rect.NormalizeRect();
				CPoint p1 = rect.TopLeft();
				CPoint p2 = rect.BottomRight();
				
				// Color for line
				Color colorLine(255, GetRValue(m_colorLine), GetGValue(m_colorLine), GetBValue(m_colorLine));
				Pen penLine(colorLine);
				graphics.DrawLine(&penLine, p1.x, p1.y, p2.x, p2.y);
				return;
			}
		}
	}
#endif

	CRect rect = m_rect;
	//rect.NormalizeRect();
	CPoint p1 = rect.TopLeft();
	CPoint p2 = rect.BottomRight();
	CPoint pointText(rect.left + 10, rect.top + 10);
	// RectF object for Text
	PointF pointF(p1.x, p1.y);
	SizeF sizeF(rect.Width(), rect.Height());
	RectF rectF(pointF, sizeF);

	Color colorWhite(255, 255, 255, 255);
	// Set gradient color
	Color color1(255, 241, 247, 255);
	Color color2(255, GetRValue(m_colorFill), GetGValue(m_colorFill), GetBValue(m_colorFill));
	// Set the color along the entire boundary of the path to aqua.
	LinearGradientBrush lgBrush( Point(rect.left, rect.top), 
								 Point(rect.right, rect.bottom),
								 color1, 
								 color2);
	//Pen pen(&lgBrush);
	Color colorBlack(255, 0, 0, 0);
	Pen penBlack(colorBlack);
	// Color for line
	Color colorLine(255, GetRValue(m_colorLine), GetGValue(m_colorLine), GetBValue(m_colorLine));
	Pen penLine(colorLine);
	// Color for and Width
	int m_width = m_lineWidth;// FIXME m_8;
	Pen colorPen(colorLine, m_width);
	// Color for fill
	Color colorFill(255, GetRValue(m_colorFill), GetGValue(m_colorFill), GetBValue(m_colorFill));
	SolidBrush solidBrush(colorFill);
	SolidBrush solidBrushWhite(colorWhite);

	//
	// Moving an object means drawing a single dash-dot rectangle 
	//
	//if( this->m_bMoving == true )
	//{
	//	penBlack.SetDashStyle(DashStyleDot);
	//	
	//	if( IsLine() )
	//	{
	//		graphics.DrawLine(&penBlack, p1.x, p1.y, p2.x, p2.y);
	//	}
	//	else
	//	{
	//		graphics.DrawRectangle(&penBlack, rect.left, rect.top, rect.Width(), rect.Height());
	//	}
	//	return;
	//}

	//if( m_type == ElementType::type_shapes_simple )
	{	
		//if( m_shapeType == ShapeType::line ||  m_shapeType == ShapeType::line2 )
		//{
		//	graphics.DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
		//}
		//else 
		//if( m_shapeType == ShapeType::line_broken ||  m_shapeType == line_broken_right )
		//{
		//	// 1 * * * 2 * * * * *
		//	// * * * * * * * * * *
		//	// * * * * * * * * * *
		//	// * * * * * * * * * *
		//	// * * * * 3 * * * * 4
		//	int step1_5_x = rect.Width() / 5;
		//	int step1_5_y = rect.Height() / 5;
		//	Point points[4] = {	Point(rect.left, rect.top), 
		//						Point(rect.left + rect.Width()/2, rect.top), 
		//						Point(rect.left + rect.Width()/2, rect.bottom), 
		//						Point(rect.right, rect.bottom)};
		//	int npoints = 4;

		//	if( m_shapeType == line_broken_right )
		//	{
		//		//colorPen.SetEndCap(LineCapArrowAnchor);
		//		//Create a Path, and add two lines to it.
		//		Point points[3] = {Point(-5, -5), Point(0, 0), Point(5, -5)};
		//		GraphicsPath capPath;
		//		capPath.AddLines(points, 3);
		//		// Create a CustomLineCap object.
		//		CustomLineCap custCap(NULL, &capPath); 
		//		// Set the start and end caps for custCap.
		//		custCap.SetStrokeCap(LineCapTriangle);
		//		// Create a Pen object, assign startStrokeCap and endStrokeCap as the 
		//		// start and end caps, and draw a line.
		//		colorPen.SetCustomEndCap(&custCap);
		//	}
		//	
		//	graphics.DrawLines(&colorPen, points, npoints);
		//}
		//else
		//if( m_shapeType == ShapeType::line_right || m_shapeType == ShapeType::line_right2 )
		//{
		//	//colorPen.SetEndCap(LineCapArrowAnchor);
		//	//graphics.DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);

		//	//Create a Path, and add two lines to it.
		//	Point points[3] = {Point(-5, -5), Point(0, 0), Point(5, -5)};
		//	GraphicsPath capPath;
		//	capPath.AddLines(points, 3);

		//	// Create a CustomLineCap object.
		//	CustomLineCap custCap(NULL, &capPath); 
  // 
		//	// Set the start and end caps for custCap.
		//	custCap.SetStrokeCap(LineCapTriangle);

		//	// Create a Pen object, assign startStrokeCap and endStrokeCap as the 
		//	// start and end caps, and draw a line.
		//	colorPen.SetCustomEndCap(&custCap);
		//	graphics.DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);

		//}
		//else if( m_shapeType == ShapeType::line_left_right )
		//{
		//	colorPen.SetStartCap(LineCapArrowAnchor);
		//	colorPen.SetEndCap(LineCapArrowAnchor);
		//	graphics.DrawLine(&colorPen, p1.x, p1.y, p2.x, p2.y);
		//}
		//else if( m_shapeType == ShapeType::rectangle || m_shapeType == ShapeType::rectangle_round )
		//{
		//	if( HasColorFill() )
		//	{
		//		if( IsSolidColorFill() )
		//			graphics.FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		//		else
		//			graphics.FillRectangle(&lgBrush, rect.left, rect.top, rect.Width(), rect.Height());
		//	}
		//	if( HasColorLine() )
		//	{
		//		graphics.DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
		//	}
		//}
		//else if( m_shapeType == ShapeType::triangle )
		//{
		//	Point points[3] = {Point(rect.left + rect.Width()/2, rect.top), 
		//					Point(rect.right, rect.bottom), 
		//					Point(rect.left, rect.bottom)};
		//	if( HasColorFill() )
		//	{
		//		if( IsSolidColorFill() )
		//			graphics.FillPolygon(&solidBrush, points, 3);
		//		else
		//			graphics.FillPolygon(&lgBrush, points, 3);
		//	}
		//	if( HasColorLine() )
		//	{
		//		graphics.DrawPolygon(&colorPen, points, 3);
		//	}
		//}
		//else if( m_shapeType == ShapeType::arrow_right )
		//{
		//	// * * * * 3 * * * * *
		//	// 1 - - - 2 * * * * *
		//	// - * * * * * * * * 4
		//	// 7 - - - 6 * * * * *
		//	// * * * * 5 * * * * *

		//	int step1_5_x = rect.Width() / 5;
		//	int step1_5_y = rect.Height() / 5;
		//	Point points[7] = {	Point(rect.left, rect.top + step1_5_y), 
		//						Point(rect.left + step1_5_x*3, rect.top + step1_5_y), 
		//						Point(rect.left + step1_5_x*3, rect.top), 
		//						Point(rect.right, rect.top + rect.Height()/2),
		//						Point(rect.left + step1_5_x*3, rect.bottom), 
		//						Point(rect.left + step1_5_x*3, rect.bottom - step1_5_y),
		//						Point(rect.left, rect.bottom - step1_5_y)};
		//	int npoints = 7;
		//	if( HasColorFill() )
		//	{
		//		if( IsSolidColorFill() )
		//			graphics.FillPolygon(&solidBrush, points, npoints);
		//		else
		//			graphics.FillPolygon(&lgBrush, points, npoints);
		//	}
		//	if( HasColorLine() )
		//	{
		//		graphics.DrawPolygon(&colorPen, points, npoints);
		//	}
		//}
		//else if( m_shapeType == ShapeType::arrow_down )
		//{
		//	// * * 1 * * * 2 * * *
		//	// * * * * * * * * * *
		//	// * * * * * * * * * *
		//	// * * * * * * * * * *
		//	// * * * * * * * * * *
		//	// 6 * 7 * * * 3 * * 4
		//	// * * * * * * * * * *
		//	// * * * * * * * * * *
		//	// * * * * * * * * * *
		//	// * * * * 5 * * * * *

		//	int step1_5_x = rect.Width() / 5;
		//	int step1_5_y = rect.Height() / 5;
		//	Point points[7] = {	/*1*/ Point(rect.left + step1_5_x*1, rect.top), 
		//						/*2*/ Point(rect.left + step1_5_x*4, rect.top), 
		//						/*3*/ Point(rect.left + step1_5_x*4, rect.top + step1_5_y*3), 
		//						/*4*/ Point(rect.right, rect.top + step1_5_y*3),
		//						/*5*/ Point(rect.left + rect.Width()/2, rect.bottom), 
		//						/*6*/ Point(rect.left, rect.top + step1_5_y*3),
		//						/*7*/ Point(rect.left + step1_5_x*1, rect.top + step1_5_y*3)};

		//	int npoints = 7;
		//	if( HasColorFill() )
		//	{
		//		if( IsSolidColorFill() )
		//			graphics.FillPolygon(&solidBrush, points, npoints);
		//		else
		//			graphics.FillPolygon(&lgBrush, points, npoints);
		//	}
		//	if( HasColorLine() )
		//	{
		//		graphics.DrawPolygon(&colorPen, points, npoints);
		//	}
		//}
		//else if( m_shapeType == ShapeType::star )
		//{
		//	// * * * * 1 * * * * *
		//	// * * * * * * * * * *
		//	// 3 * 2 * * * *10 * 9
		//	// * * * * * * * * * *
		//	// * * 4 * * * * 8 * *
		//	// * * * * * * * * * *
		//	// * * * * 6 * * * * *
		//	// * * * * * * * * * *
		//	// * * * * * * * * * *
		//	// 5 * * * * * * * * 7

		//	int step1_10_x = rect.Width() / 10;
		//	int step1_10_y = rect.Height() / 10;
		//	Point points[10] = {/*1*/ Point(rect.left + rect.Width()/2, rect.top), 
		//						/*2*/ Point(rect.left + step1_10_x*3, rect.top + step1_10_y*3), 
		//						/*3*/ Point(rect.left, rect.top + step1_10_y*3), 
		//						/*4*/ Point(rect.left + step1_10_x*3, rect.top + step1_10_y*5), 
		//						/*5*/ Point(rect.left, rect.bottom), 							
		//						/*6*/ Point(rect.left + rect.Width()/2, rect.top + step1_10_y*7), 
		//						/*7*/ Point(rect.right, rect.bottom),
		//						/*8*/ Point(rect.left + step1_10_x*7, rect.top + step1_10_y*5), 
		//						/*9*/ Point(rect.right, rect.top + step1_10_y*3), 
		//						/*10*/Point(rect.left + step1_10_x*7, rect.top + step1_10_y*3)};
		//	int npoints = 10;
		//	if( HasColorFill() )
		//	{
		//		if( IsSolidColorFill() )
		//			graphics.FillPolygon(&solidBrush, points, npoints);
		//		else
		//			graphics.FillPolygon(&lgBrush, points, npoints);
		//	}
		//	if( HasColorLine() )
		//	{
		//		graphics.DrawPolygon(&colorPen, points, npoints);
		//	}
		//}
		//else if( m_shapeType == ShapeType::parenthese_left )
		//{
		//	// * * * * * 1 * * * *
		//	// * 2 * * * * * * * *
		//	// * * * * * * * * * *
		//	// * 3 * * * * * * * *
		//	// 4 * * * * * * * * *
		//	// * 5 * * * * * * * *
		//	// * * * * * * * * * *
		//	// * 6 * * * * * * * *
		//	// * * * * * 7 * * *
		//	int step1_10_x = rect.Width() / 10;
		//	int step1_10_y = rect.Height() / 10;
		//	Point points[7] = {/*1*/ Point(rect.left + step1_10_x*5, rect.top), 
		//						/*2*/ Point(rect.left + step1_10_x*1, rect.top + step1_10_y*1), 
		//						/*3*/ Point(rect.left + step1_10_x*1, rect.top + step1_10_y*4), 
		//						/*4*/ Point(rect.left + step1_10_x*0, rect.top + step1_10_y*5), 
		//						/*5*/ Point(rect.left + step1_10_x*1, rect.top + step1_10_y*6), 							
		//						/*6*/ Point(rect.left + step1_10_x*1, rect.top + step1_10_y*9), 
		//						/*7*/ Point(rect.left + step1_10_x*5, rect.bottom /*top + step1_10_y*9*/)};
		//	int npoints = 7;
		//	graphics.DrawCurve(&colorPen, points, npoints);
		//}
		//else if( m_shapeType == ShapeType::parenthese_right )
		//{
		//	// * * * * 1 * * * * *
		//	// * * * * * * * * 2 *
		//	// * * * * * * * * * *
		//	// * * * * * * * * 3 *
		//	// * * * * * * * * * 4
		//	// * * * * * * * * 5 *
		//	// * * * * * * * * * *
		//	// * * * * * * * * 6 *
		//	// * * * * 7 * * * * *
		//	int step1_10_x = rect.Width() / 10;
		//	int step1_10_y = rect.Height() / 10;
		//	Point points[7] = {/*1*/ Point(rect.left + step1_10_x*5, rect.top), 
		//						/*2*/ Point(rect.left + step1_10_x*9, rect.top + step1_10_y*1), 
		//						/*3*/ Point(rect.left + step1_10_x*9, rect.top + step1_10_y*4), 
		//						/*4*/ Point(rect.right /*.left + step1_10_x*10*/, rect.top + step1_10_y*5), 
		//						/*5*/ Point(rect.left + step1_10_x*9, rect.top + step1_10_y*6), 							
		//						/*6*/ Point(rect.left + step1_10_x*9, rect.top + step1_10_y*9), 
		//						/*7*/ Point(rect.left + step1_10_x*5, rect.bottom /*top + step1_10_y*9*/)};
		//	int npoints = 7;
		//	graphics.DrawCurve(&colorPen, points, npoints);
		//}
		//else if( m_shapeType == ShapeType::ellipse )
		//{
		//	if( HasColorFill() )
		//	{
		//		if( IsSolidColorFill() )
		//			graphics.FillEllipse(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
		//		else
		//			graphics.FillEllipse(&lgBrush, rect.left, rect.top, rect.Width(), rect.Height());
		//	}
		//	if( HasColorLine() )
		//	{
		//		graphics.DrawEllipse(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
		//	}
		//}
		//else
		{
			//if( IsSolidColorFill() )
			//	graphics.FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
			//else
			//	graphics.FillRectangle(&lgBrush, rect.left, rect.top, rect.Width(), rect.Height());

			//CString str;
			//str.Format("%s Shape type=%d", m_text.c_str(), m_shapeType);
			//// Font object
			//FontFamily fontFamily(L"Calibri");
			//Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
			//SolidBrush solidBrush(colorBlack);
			//graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
			//graphics.DrawString(CStringW(str), -1, &font, PointF(pointText.x, pointText.y), &solidBrush);
		}
	}
	
	//if( m_type == ElementType::type_shapes_infrastructure ||
	//		m_type == ElementType::type_image )
	//{
	//	if( m_image.IsEmpty() )
	//	{
	//		graphics.DrawRectangle(&penLine, rect.left, rect.top, rect.Width(), rect.Height());
	//		graphics.DrawLine(&penLine, p1.x, p1.y, p2.x, p2.y);
	//		graphics.DrawLine(&penLine, p1.x, p2.y, p2.x, p1.y);
	//	}
	//	else
	//	{			
	//		// Load the image		
	//		CStringW imageFilePath;
	//		
	//		// FIXME: Infrastructure shapes hard coded...
	//		if( m_type == ElementType::type_shapes_infrastructure )
	//			imageFilePath = GetImageFilePath();		
	//		if( m_type == ElementType::type_image )
	//			imageFilePath = GetImageFilePath();
	//		Image image(imageFilePath);
	//		//graphics.DrawImage(&image, rect.left, rect.top, rect.Width(), rect.Height());
	//		//graphics.DrawImage(&image, rect.left, rect.top, image.GetWidth(), image.GetHeight());

	//		if( m_type == ElementType::type_shapes_infrastructure )
	//		{
	//			graphics.DrawImage(&image, rect.left, rect.top, image.GetWidth(), image.GetHeight());
	//		}

	//		if( m_type == ElementType::type_image )
	//		{
	//			//Rect destRect(rect.left, rect.top, rect.Width(), rect.Height());
	//			//graphics.DrawImage(&image, destRect, 0, 0, image.GetWidth(), image.GetHeight(), UnitPixel, NULL, NULL, NULL);
	//			graphics.DrawImage(&image, rect.left, rect.top, image.GetWidth(), image.GetHeight());
	//		}
	//		
	//		// Infrastructure shapes are fixed size
	//		//if( m_type == type_shapes_infrastructure )
	//		{
	//			// Adjust fixed image size
	//			CPoint p1(rect.left, rect.top);
	//			CPoint p2(p1.x + image.GetWidth(), p1.y + image.GetHeight());
	//			m_rect.SetRect(p1, p2);		
	//		}

	//		// Useless or not to draw a rectangle around the image ???
	//		if( HasColorLine() )
	//		{
	//			rect = m_rect;
	//			graphics.DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	//		}

	//	}
	//}
	
	//if( m_type == ElementType::type_shapes_development )
	//{	
	//	if( m_shapeType == ShapeType::development_class )
	//	{
	//		graphics.FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
	//		graphics.DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	//	}
	//	if( m_shapeType == ShapeType::development_interface )
	//	{
	//		graphics.FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
	//		graphics.DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	//	}
	//	if( m_shapeType == ShapeType::development_enumeration )
	//	{
	//		graphics.FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
	//		graphics.DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	//	}
	//	if( m_shapeType == ShapeType::development_package )
	//	{
	//		int step1_4_x = rect.Width() / 4;

	//		// Draw little rectangle			
	//		graphics.FillRectangle(&solidBrush, rect.left, rect.top, step1_4_x*3, 30);
	//		graphics.DrawRectangle(&colorPen, rect.left, rect.top, step1_4_x*3, 30);
	//		// Draw large rectangle
	//		graphics.FillRectangle(&solidBrush, rect.left, rect.top + 30, rect.Width(), rect.Height() - 30);
	//		graphics.DrawRectangle(&colorPen, rect.left, rect.top + 30, rect.Width(), rect.Height() - 30);
	//	}
	//	if( m_shapeType == ShapeType::development_comment )
	//	{
	//		graphics.FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
	//		graphics.DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	//		// Draw single line
	//		int width = rect.Width()-20; //*0.75;
	//		int height = 20; //Height()-20; //*0.25;
	//		graphics.DrawLine(&colorPen, rect.left + width, rect.top, rect.right, rect.top + height);
	//	}
	//	if( m_shapeType == ShapeType::development_component )
	//	{
	//		graphics.FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height()/2);
	//		graphics.DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	//		// middle fill
	//		graphics.DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height() /2);
	//	}
	//}


	//if( m_type == ElementType::type_text )
	//{
	//	//if( m_shapeType == ShapeType::text )
	//	{
	//		// Useless or not do fill a rectangle around the texte ???
	//		if( HasColorFill() )
	//		{
	//			if( IsSolidColorFill() )
	//				graphics.FillRectangle(&solidBrush, rect.left, rect.top, rect.Width(), rect.Height());
	//			else
	//				graphics.FillRectangle(&lgBrush, rect.left, rect.top, rect.Width(), rect.Height());
	//		}
	//		// Useless or not to draw a rectangle around the text ???
	//		if( HasColorLine() )
	//		{
	//			graphics.DrawRectangle(&colorPen, rect.left, rect.top, rect.Width(), rect.Height());
	//		}

	//		// Font object
	//		FontFamily fontFamily(L"Calibri");
	//		Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
	//		// StringFormat object
	//		StringFormat stringFormat;
	//		if( m_shapeType == ShapeType::text_left )
	//		{
	//			stringFormat.SetAlignment(StringAlignmentNear);
	//		}
	//		if( m_shapeType == ShapeType::text_center )
	//		{
	//			stringFormat.SetAlignment(StringAlignmentCenter);
	//		}
	//		if( m_shapeType == ShapeType::text_right )
	//		{
	//			stringFormat.SetAlignment(StringAlignmentFar);
	//		}
	//		//stringFormat.SetLineAlignment(StringAlignmentCenter);
	//		// Brush object
	//		SolidBrush solidBrush(Color(255, 0, 0, 0));
	//		graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	//		graphics.DrawString(CStringW(m_text), -1, &font, rectF, &stringFormat, &solidBrush);
	//	}
	//}
	//else
	{
		if( m_text.empty() == false )
		{
			//graphics.FillRectangle(&lgBrush, rect.left, rect.top, rect.Width(), rect.Height());

			// Font object
			FontFamily fontFamily(L"Calibri");
			Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
			SolidBrush solidBrush(Color(255, 0, 0, 0));
			graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
			graphics.DrawString(CStringW(m_text.c_str()), -1, &font, PointF(pointText.x, pointText.y), &solidBrush);
		}
	}
	
}



