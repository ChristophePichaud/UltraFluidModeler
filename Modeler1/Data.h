#pragma once

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
	void save(Archive & ar, const unsigned int version) const
	{
		ar & BOOST_SERIALIZATION_NVP(m_shapes);
	}

	template<class Archive> 
	void load(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(m_shapes);
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
	void save(Archive & ar, const unsigned int version) const
	{
		// ar & name;
		// ar & id;
		ar & BOOST_SERIALIZATION_NVP(m_name);
		ar & BOOST_SERIALIZATION_NVP(m_id);
		ar & BOOST_SERIALIZATION_NVP(m_rect);
		ar & BOOST_SERIALIZATION_NVP(m_type);
		ar & BOOST_SERIALIZATION_NVP(m_shapeType);
		ar & BOOST_SERIALIZATION_NVP(m_caption);
		ar & BOOST_SERIALIZATION_NVP(m_text);
		ar & BOOST_SERIALIZATION_NVP(m_x1);
		ar & BOOST_SERIALIZATION_NVP(m_y1);
		ar & BOOST_SERIALIZATION_NVP(m_x2);
		ar & BOOST_SERIALIZATION_NVP(m_y2);
		ar & BOOST_SERIALIZATION_NVP(m_colorFillR);
		ar & BOOST_SERIALIZATION_NVP(m_colorFillG);
		ar & BOOST_SERIALIZATION_NVP(m_colorFillB);
		ar & BOOST_SERIALIZATION_NVP(m_colorLineR);
		ar & BOOST_SERIALIZATION_NVP(m_colorLineG);
		ar & BOOST_SERIALIZATION_NVP(m_colorLineB);
		ar & BOOST_SERIALIZATION_NVP(m_bSolidColorFill);
		ar & BOOST_SERIALIZATION_NVP(m_bColorLine);
		ar & BOOST_SERIALIZATION_NVP(m_bColorFill);
	}

	template<class Archive> 
	void load(Archive & ar, const unsigned int version)
	{
		// ar & name;
		// ar & id;
		ar & BOOST_SERIALIZATION_NVP(m_name);
		ar & BOOST_SERIALIZATION_NVP(m_id);
		ar & BOOST_SERIALIZATION_NVP(m_rect);
		ar & BOOST_SERIALIZATION_NVP(m_type);
		ar & BOOST_SERIALIZATION_NVP(m_shapeType);
		ar & BOOST_SERIALIZATION_NVP(m_caption);
		ar & BOOST_SERIALIZATION_NVP(m_text);
		ar & BOOST_SERIALIZATION_NVP(m_x1);
		ar & BOOST_SERIALIZATION_NVP(m_y1);
		ar & BOOST_SERIALIZATION_NVP(m_x2);
		ar & BOOST_SERIALIZATION_NVP(m_y2);
		ar & BOOST_SERIALIZATION_NVP(m_colorFillR);
		ar & BOOST_SERIALIZATION_NVP(m_colorFillG);
		ar & BOOST_SERIALIZATION_NVP(m_colorFillB);
		ar & BOOST_SERIALIZATION_NVP(m_colorLineR);
		ar & BOOST_SERIALIZATION_NVP(m_colorLineG);
		ar & BOOST_SERIALIZATION_NVP(m_colorLineB);
		ar & BOOST_SERIALIZATION_NVP(m_bSolidColorFill);
		ar & BOOST_SERIALIZATION_NVP(m_bColorLine);
		ar & BOOST_SERIALIZATION_NVP(m_bColorFill);
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
};

BOOST_CLASS_VERSION(CSimpleShape, 1)

