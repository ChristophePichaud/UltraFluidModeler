#pragma once

#include "Constants.h"

enum SelectMode : int
{
	none,
	netselect,
	move,
	size
};

enum ElementType : int
{
	type_unknown, 
	type_select, 
	type_shapes_simple, 
	type_image, 
	type_shapes_infrastructure, 
	type_text, 
	type_shapes_development,
	type_shapes_import
};

#define OffsetShapes_Simple			0
#define OffsetShapes_Image			50
#define OffsetShapes_Infrastructure 100
#define OffsetShapes_Text			200
#define OffsetShapes_Development	300
#define OffsetShapes_Import			400

enum ShapeType : int 
{
	// Simple
	line = OffsetShapes_Simple,
	line_right,
	rectangle,
	ellipse,
	rectangle_round,
	triangle,
	line_broken,
	line_broken_right,
	notimp_igloo,
	notimpl_multi_line,
	arrow_right,
	arrow_down,
	notimpl_courbe1,
	notimpl_courbe2,
	parenthese_left,
	parenthese_right,
	star,
	line2 = 18,
	line_right2,
	line_left_right,
	// Image
	image_fixed = OffsetShapes_Image,
	image_scaled,
	// Infrastructure
	infrastructure_server_ad = OffsetShapes_Infrastructure,
	infrastructure_server,
	infrastructure_server_web,
	infrastructure_server_database,
	infrastructure_workstation,
	infrastructure_laptop,
	infrastructure_firewall,
	infrastructure_network,
	infrastructure_virtual_server,
	infrastructure_virtual_server_web,
	infrastructure_virtual_server_database,
	infrastructure_virtualization_server,
	infrastructure_server_ad_grey,
	infrastructure_server_grey,
	infrastructure_server_database_grey,
	infrastructure_server_farm,
	infrastructure_workstation_grey,
	infrastructure_laptop_grey,
	// Text
	text = OffsetShapes_Text,
	text_left,
	text_center,
	text_right,
	text_justify,
	// Development
	development_class = OffsetShapes_Development,
	development_interface,
	development_enumeration,
	development_package,
	development_comment,
	development_component,
	development_association,
	development_aggregation,
	development_composition,
	development_dependency,
	development_inheritance,
	development_package_import,
	development_connector,
	import_class = OffsetShapes_Import,
	unknown = 1000
};

enum TrackerState 
{ 
	normal, 
	selected, 
	active 
};

// Hints for UpdateAllViews/OnUpdate
#define HINT_UPDATE_WINDOW      0
#define HINT_UPDATE_DRAWOBJ     1
#define HINT_UPDATE_SELECTION   2
#define HINT_DELETE_SELECTION   3
#define HINT_UPDATE_OLE_ITEMS   4

class CShapeType
{
	public:
		static ShapeType ToShapeType(int value);
};

class CElement;
class CModeler1View;
class CElementManager;
class CDrawingContext;

class CElement : public CObject
{
public:
	DECLARE_SERIAL(CElement);
	CElement();
	virtual ~CElement(void);

// Internal ID is a GUID
	static int m_counter;
	void SetGuid();
	void SetName();

// Copy an object
	std::shared_ptr<CElement> MakeCopy();

// Virtual Operations
public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void Draw(CModeler1View * pView, CDC* pDC);
	virtual void Draw(CDrawingContext & ctxt);

// Operations
public:
	CString ToString();
	CString ToString(ElementType type);
	CString ToString(ShapeType type);
	//static bool IsDrawable(ElementType type);
	bool Intersects(const CRect& rect);
	void InvalidateObj(void);
	void CheckForKeepingAMinimumSize();

// Managing/Viewing Object Selection & Tracker helpers
public:
	int GetHandleCount();
	int HitTest(CPoint point, CModeler1View* pView, bool bSelected);
	CRect GetHandleRect(int nHandleID, CModeler1View* pView);
	CPoint GetHandle(int nHandle);
	void DrawTracker(CDrawingContext & ctxt, TrackerState state);
	HCURSOR GetHandleCursor(int nHandle);
	void MoveHandleTo(int nHandle, CPoint point, CModeler1View* pView);	

// Managing Object Format
public:
	virtual bool CanChangeFillColor() const;
	virtual bool CanChangeLineColor() const;
	virtual bool CanChangeLineWidth() const;
	
// Attributes
public:
	std::wstring m_name;
	ElementType m_type;
	ShapeType m_shapeType;
	std::wstring m_objectId;
	std::wstring m_caption;
	std::wstring m_text;
	std::wstring m_fontName;
	int m_fontSize;
	CRect m_rect;
	CPoint m_point;
	CPoint m_last;
	COLORREF m_colorFill;
	bool m_bColorFill;
	COLORREF m_colorLine;
	bool m_bSolidColorFill;
	bool m_bColorLine;
	int m_lineWidth;
	bool m_bLineWidth;
	std::wstring m_image;
	// Handling Moving stuff
	bool m_bMoving;
	// Connectivity
	CElementManager * m_pManager;
	CModeler1View * m_pView;

// Methods for Attributes
public:
	bool IsLine();
	std::wstring GetImageFilePath();
	bool HasColorFill()	const { return m_bColorFill; }
	bool IsSolidColorFill()	const { return m_bSolidColorFill; }
	bool HasColorLine()	const { return m_bColorLine; }

// Extra Pointer plumbing
public:
	CElementManager * GetManager() const { return m_pManager; }
	CModeler1View * GetView() const	{ return m_pView; }
};

void CallDraw(CDrawingContext & ctxt, std::shared_ptr<CElement> pElement);

class CMethodData
{
public:
	bool m_IsPublic;
	std::wstring m_Name;
};

class CFieldData
{
public:
	bool m_IsPublic;
	std::wstring m_Type;
	std::wstring m_Name;
};

class CPropertyData
{
public:
	std::wstring m_Type;
	std::wstring m_Name;
};

class CTypeData
{
public:
	std::wstring m_Name;
	std::wstring m_FullName;
	std::vector<std::shared_ptr<CMethodData>> m_Methods;
	std::vector<std::shared_ptr<CFieldData>> m_Fields;
	std::vector<std::shared_ptr<CPropertyData>> m_Properties;
};



