// Copyright (C) SAS NET Azure Rangers
// All rights reserved.

#pragma once
class XMLData
{
};

class CSimpleShape;
class CMainData;
class CShapeCollection;

//
// CShapeCollection
//
class CShapeCollection
{
public:
	CShapeCollection() {}
	~CShapeCollection() {}

private:
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		ar& BOOST_SERIALIZATION_NVP(m_shapes);
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		ar& BOOST_SERIALIZATION_NVP(m_shapes);
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
	vector<boost::shared_ptr<CSimpleShape> > m_shapes;
};

BOOST_CLASS_VERSION(CShapeCollection, 1)

//
// CSimpleShape
//
class CSimpleShape
{
public:
	CSimpleShape() {}
	~CSimpleShape() {}

private:
	friend class boost::serialization::access;
	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		// ar & name;
		// ar & id;
		ar& BOOST_SERIALIZATION_NVP(m_name);
		ar& BOOST_SERIALIZATION_NVP(m_id);
		ar& BOOST_SERIALIZATION_NVP(m_rect);
		ar& BOOST_SERIALIZATION_NVP(m_type);
		ar& BOOST_SERIALIZATION_NVP(m_shapeType);
		ar& BOOST_SERIALIZATION_NVP(m_caption);
		ar& BOOST_SERIALIZATION_NVP(m_text);
		ar& BOOST_SERIALIZATION_NVP(m_x1);
		ar& BOOST_SERIALIZATION_NVP(m_y1);
		ar& BOOST_SERIALIZATION_NVP(m_x2);
		ar& BOOST_SERIALIZATION_NVP(m_y2);
		ar& BOOST_SERIALIZATION_NVP(m_colorFillR);
		ar& BOOST_SERIALIZATION_NVP(m_colorFillG);
		ar& BOOST_SERIALIZATION_NVP(m_colorFillB);
		ar& BOOST_SERIALIZATION_NVP(m_colorLineR);
		ar& BOOST_SERIALIZATION_NVP(m_colorLineG);
		ar& BOOST_SERIALIZATION_NVP(m_colorLineB);
		ar& BOOST_SERIALIZATION_NVP(m_bSolidColorFill);
		ar& BOOST_SERIALIZATION_NVP(m_bColorLine);
		ar& BOOST_SERIALIZATION_NVP(m_bColorFill);
		ar& BOOST_SERIALIZATION_NVP(m_lineWidth);
		ar& BOOST_SERIALIZATION_NVP(m_image);
		ar& BOOST_SERIALIZATION_NVP(m_textAlign);
		ar& BOOST_SERIALIZATION_NVP(m_bFixed);
		ar& BOOST_SERIALIZATION_NVP(m_fontSize);
		ar& BOOST_SERIALIZATION_NVP(m_fontName);
		ar& BOOST_SERIALIZATION_NVP(m_code);
		ar& BOOST_SERIALIZATION_NVP(m_bBold);
		ar& BOOST_SERIALIZATION_NVP(m_bItalic);
		ar& BOOST_SERIALIZATION_NVP(m_bUnderline);
		ar& BOOST_SERIALIZATION_NVP(m_bStrikeThrough);
		ar& BOOST_SERIALIZATION_NVP(m_colorTextR);
		ar& BOOST_SERIALIZATION_NVP(m_colorTextG);
		ar& BOOST_SERIALIZATION_NVP(m_colorTextB);
		ar& BOOST_SERIALIZATION_NVP(m_connectorName1);
		ar& BOOST_SERIALIZATION_NVP(m_connectorName2);
		ar& BOOST_SERIALIZATION_NVP(m_connectorDragHandle1);
		ar& BOOST_SERIALIZATION_NVP(m_connectorDragHandle2);
		ar& BOOST_SERIALIZATION_NVP(m_document);
		ar& BOOST_SERIALIZATION_NVP(m_elementGroupNames);
		ar& BOOST_SERIALIZATION_NVP(m_elementGroupElements);
		ar& BOOST_SERIALIZATION_NVP(m_documentType);
		ar& BOOST_SERIALIZATION_NVP(m_documentTypeText);
		ar& BOOST_SERIALIZATION_NVP(m_version);
		ar& BOOST_SERIALIZATION_NVP(m_product);
		ar& BOOST_SERIALIZATION_NVP(m_leftMargin);
		ar& BOOST_SERIALIZATION_NVP(m_topMargin);
		ar& BOOST_SERIALIZATION_NVP(m_rotateAngle);
		ar& BOOST_SERIALIZATION_NVP(m_team);
		ar& BOOST_SERIALIZATION_NVP(m_authors);
		ar& BOOST_SERIALIZATION_NVP(m_bShowElementName);
		ar& BOOST_SERIALIZATION_NVP(m_standardShapesTextColorR);
		ar& BOOST_SERIALIZATION_NVP(m_standardShapesTextColorG);
		ar& BOOST_SERIALIZATION_NVP(m_standardShapesTextColorB);
		ar& BOOST_SERIALIZATION_NVP(m_connectorShapesTextColorR);
		ar& BOOST_SERIALIZATION_NVP(m_connectorShapesTextColorG);
		ar& BOOST_SERIALIZATION_NVP(m_connectorShapesTextColorB);
		ar& BOOST_SERIALIZATION_NVP(m_bShowConnectors);
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		// ar & name;
		// ar & id;
		ar& BOOST_SERIALIZATION_NVP(m_name);
		ar& BOOST_SERIALIZATION_NVP(m_id);
		ar& BOOST_SERIALIZATION_NVP(m_rect);
		ar& BOOST_SERIALIZATION_NVP(m_type);
		ar& BOOST_SERIALIZATION_NVP(m_shapeType);
		ar& BOOST_SERIALIZATION_NVP(m_caption);
		ar& BOOST_SERIALIZATION_NVP(m_text);
		ar& BOOST_SERIALIZATION_NVP(m_x1);
		ar& BOOST_SERIALIZATION_NVP(m_y1);
		ar& BOOST_SERIALIZATION_NVP(m_x2);
		ar& BOOST_SERIALIZATION_NVP(m_y2);
		ar& BOOST_SERIALIZATION_NVP(m_colorFillR);
		ar& BOOST_SERIALIZATION_NVP(m_colorFillG);
		ar& BOOST_SERIALIZATION_NVP(m_colorFillB);
		ar& BOOST_SERIALIZATION_NVP(m_colorLineR);
		ar& BOOST_SERIALIZATION_NVP(m_colorLineG);
		ar& BOOST_SERIALIZATION_NVP(m_colorLineB);
		ar& BOOST_SERIALIZATION_NVP(m_bSolidColorFill);
		ar& BOOST_SERIALIZATION_NVP(m_bColorLine);
		ar& BOOST_SERIALIZATION_NVP(m_bColorFill);
		ar& BOOST_SERIALIZATION_NVP(m_lineWidth);
		ar& BOOST_SERIALIZATION_NVP(m_image);
		ar& BOOST_SERIALIZATION_NVP(m_textAlign);
		ar& BOOST_SERIALIZATION_NVP(m_bFixed);
		ar& BOOST_SERIALIZATION_NVP(m_fontSize);
		ar& BOOST_SERIALIZATION_NVP(m_fontName);
		ar& BOOST_SERIALIZATION_NVP(m_code);
		ar& BOOST_SERIALIZATION_NVP(m_bBold);
		ar& BOOST_SERIALIZATION_NVP(m_bItalic);
		ar& BOOST_SERIALIZATION_NVP(m_bUnderline);
		ar& BOOST_SERIALIZATION_NVP(m_bStrikeThrough);
		ar& BOOST_SERIALIZATION_NVP(m_colorTextR);
		ar& BOOST_SERIALIZATION_NVP(m_colorTextG);
		ar& BOOST_SERIALIZATION_NVP(m_colorTextB);
		ar& BOOST_SERIALIZATION_NVP(m_connectorName1);
		ar& BOOST_SERIALIZATION_NVP(m_connectorName2);
		ar& BOOST_SERIALIZATION_NVP(m_connectorDragHandle1);
		ar& BOOST_SERIALIZATION_NVP(m_connectorDragHandle2);
		ar& BOOST_SERIALIZATION_NVP(m_document);
		ar& BOOST_SERIALIZATION_NVP(m_elementGroupNames);
		ar& BOOST_SERIALIZATION_NVP(m_elementGroupElements);
		ar& BOOST_SERIALIZATION_NVP(m_documentType);
		ar& BOOST_SERIALIZATION_NVP(m_documentTypeText);
		ar& BOOST_SERIALIZATION_NVP(m_version);
		ar& BOOST_SERIALIZATION_NVP(m_product);
		ar& BOOST_SERIALIZATION_NVP(m_leftMargin);
		ar& BOOST_SERIALIZATION_NVP(m_topMargin);
		ar& BOOST_SERIALIZATION_NVP(m_rotateAngle);
		ar& BOOST_SERIALIZATION_NVP(m_team);
		ar& BOOST_SERIALIZATION_NVP(m_authors);
		ar& BOOST_SERIALIZATION_NVP(m_bShowElementName);
		ar& BOOST_SERIALIZATION_NVP(m_standardShapesTextColorR);
		ar& BOOST_SERIALIZATION_NVP(m_standardShapesTextColorG);
		ar& BOOST_SERIALIZATION_NVP(m_standardShapesTextColorB);
		ar& BOOST_SERIALIZATION_NVP(m_connectorShapesTextColorR);
		ar& BOOST_SERIALIZATION_NVP(m_connectorShapesTextColorG);
		ar& BOOST_SERIALIZATION_NVP(m_connectorShapesTextColorB);
		ar& BOOST_SERIALIZATION_NVP(m_bShowConnectors);
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
	wstring m_name;
	wstring m_id;
	string m_rect;
	long m_type;
	long m_shapeType;
	wstring m_caption;
	wstring m_text;
	long m_x1;
	long m_y1;
	long m_x2;
	long m_y2;
	int m_colorFillR;
	int m_colorFillG;
	int m_colorFillB;
	int m_colorLineR;
	int m_colorLineG;
	int m_colorLineB;
	bool m_bSolidColorFill;
	bool m_bColorLine;
	bool m_bColorFill;
	int m_lineWidth;
	wstring m_image;
	wstring m_textAlign;
	bool m_bFixed;
	int m_fontSize;
	wstring m_fontName;
	wstring m_code;
	bool m_bBold;
	bool m_bItalic;
	bool m_bUnderline;
	bool m_bStrikeThrough;
	int m_colorTextR;
	int m_colorTextG;
	int m_colorTextB;
	wstring m_connectorName1;
	wstring m_connectorName2;
	int m_connectorDragHandle1;
	int m_connectorDragHandle2;
	std::wstring m_document;
	std::wstring m_elementGroupNames;
	std::wstring m_elementGroupElements;
	long m_documentType;
	std::wstring m_documentTypeText;
	std::wstring m_version;
	std::wstring m_product;
	int m_leftMargin;
	int m_topMargin;
	int m_rotateAngle;
	std::wstring m_team;
	std::wstring m_authors;
	bool m_bShowElementName;
	int m_standardShapesTextColorR;
	int m_standardShapesTextColorG;
	int m_standardShapesTextColorB;
	int m_connectorShapesTextColorR;
	int m_connectorShapesTextColorG;
	int m_connectorShapesTextColorB;
	bool m_bShowConnectors;
};

BOOST_CLASS_VERSION(CSimpleShape, 1)

