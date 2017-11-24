#pragma once
#include "ElementContainer.h"
#include "Element.h"
#include "ElementFactory.h"

class CElementManager : public CObject
{
private:

public:
	DECLARE_SERIAL(CElementManager);
	CElementManager();
	virtual ~CElementManager(void);

// Attributes
public:
	// Drawing objects
	CElementContainer m_objects;
	// Selection objects
	CElementContainer m_selection;
	// Clipboard objects
	CElementContainer m_clipboard;

	COLORREF m_paperColor;
	// Page size in logical coordinates
	CSize m_size;
	CPoint m_lastPoint;
	// Current working object
	std::wstring m_objectId;
	// Current Select action
	SelectMode m_selectMode;
	// Cursor hanlde
	int m_nDragHandle;
	// Zoom float factor (default 1.0f)
	float m_fZoomFactor;
	
// Attributes Current UI interaction members
public:
	// Is in drawing...
	BOOL m_bDrawing;
	// Current selected drawing tool
	ElementType m_type;
	// Current selected shape type from Ribbon
	ShapeType m_shapeType;

public:
	// Selection 1st point
	CPoint m_selectPoint;
	// Selection Rect
	CRect m_selectionRect;

// Methods for Attributes
public:
	//SelectMode GetSelectMode()	 { return m_selectMode; }
	const vector<std::shared_ptr<CElement>>& GetObjects() { return m_objects.m_objects; }

// Debugging Operations
public:
	void DebugDumpObjects(CModeler1View * pView);

// Operations
public:
	void Serialize_LoadAsXML(CModeler1View * pView);
	void Serialize_SaveAsXML(CModeler1View * pView);
	void ConnectToMainFrame(CModeler1View * pView);
	void ConnectToPropertyGrid();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	void RemoveSelectedObjects(CModeler1View * pView);
	void ViewToManager(CModeler1View * pView, CPoint & point);
	void ViewToManager(CModeler1View * pView, CRect & rect);
	void ManagerToView(CModeler1View * pView, CPoint & point);
	void ManagerToView(CModeler1View * pView, CRect & rect);
	COLORREF GetPaperColor() const { return m_paperColor; }
	void Invalidate(CModeler1View * pView, std::shared_ptr<CElement> pElement);
	void Invalidate(CModeler1View * pView);
	void InvalObj(CModeler1View * pView, std::shared_ptr<CElement> pElement);
	CSize GetSize() const { return m_size; }
	void UpdateFromPropertyGrid(std::wstring objectId, std::wstring name, std::wstring value);
	void UpdateFromPropertyGrid(std::wstring objectId, std::wstring name, COLORREF color);
	void UpdateFromPropertyGrid(std::wstring objectId, std::wstring name, long value);
	void ActivateView(CModeler1View * pView, bool bActivate, CView* pActiveView, CView* pDeactiveView);

// Managing Font operations
public:
	void OnFont(CModeler1View * pView);
	void OnFontSize(CModeler1View * pView);

// Managing Clipboard operations
public:
	void OnEditCut(CModeler1View * pView);
	void OnEditCopy(CModeler1View * pView);
	void OnEditPaste(CModeler1View * pView);
	
// Managing Object Positions
public:
	void MoveToFront(CModeler1View * pView);
	void MoveForward(CModeler1View * pView);
	void MoveBackward(CModeler1View * pView);
	void MoveToBack(CModeler1View * pView);

// Managing Background drawing
public:
	void DrawBackground(CModeler1View * pView, CDC * pDC);

// Managing UI object connections
public:
	void FindAConnectionFor(std::shared_ptr<CElement> pElement, CPoint point, CModeler1View* pView);

// Managing UI dependencies (Ribbon UI, Property Grid, ClassView)
public:
	void UpdateUI(CModeler1View * pView, std::shared_ptr<CElement> pElement);
	void UpdateRibbonUI(CModeler1View * pView, std::shared_ptr<CElement> pElement);
	void UpdatePropertyGrid(CModeler1View * pView, std::shared_ptr<CElement> pElement);
	void UpdateClassView();
	void UpdateClassView(std::shared_ptr<CElement> pElement);
	void UpdateFileView();
	void UpdateFileView(std::shared_ptr<CElement> pElement);

// Managing Object Format
public:
	void FillColor(CModeler1View * pView);
	void NoFillColor(CModeler1View * pView);
	void LineColor(CModeler1View * pView);
	void LineWidth(CModeler1View * pView, UINT nID);
	void PageColor(CModeler1View * pView);

// Managing Zoom Operations
public:
	void Zoom(CModeler1View * pView);
	void ZoomIn(CModeler1View * pView);
	void ZoomOut(CModeler1View * pView);

// Load Module Operations
public:
	void LoadModule(CModeler1View * pView);

// Managing Object Selection
public:
	bool HasSelection();
	bool IsSelected(std::shared_ptr<CElement> pElement);
	bool Select(std::shared_ptr<CElement> pElement);
	bool Deselect(std::shared_ptr<CElement> pElement);
	void SelectNone();
	void DrawSelectionRect(CModeler1View *pView);

// Overridables
public:
	virtual void PrepareDC(CModeler1View * pView, CDC* pDC, CPrintInfo* pInfo);
	virtual void Draw(CModeler1View * pView, CDC * pDC);
	virtual void DrawEx(CModeler1View * pView, CDC * pDC);
	virtual void Update(CModeler1View * pView, LPARAM lHint, CObject* pHint);

// UI Handlers
public:
	virtual void OnLButtonDown(CModeler1View* pView, UINT nFlags, const CPoint& cpoint);
	virtual void OnLButtonDblClk(CModeler1View* pView, UINT nFlags, const CPoint& cpoint);
	virtual void OnLButtonUp(CModeler1View* pView, UINT nFlags, const CPoint& cpoint);
	virtual void OnMouseMove(CModeler1View* pView, UINT nFlags, const CPoint& cpoint);
};
