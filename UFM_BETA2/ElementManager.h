// Copyright (C) SAS NET Azure Rangers
// All rights reserved.

#pragma once
#include "ElementContainer.h"
#include "Element.h"
#include "ElementFactory.h"
#include "CTextControlDialog.h"

class CCodeFile;

enum View
{
	Modeling,
	Source
};

class CElementManager : public CObject
{
private:

public:
	DECLARE_SERIAL(CElementManager);
	CElementManager();
	CElementManager(const CElementManager& elementManager);
	virtual ~CElementManager(void);

// Attributes
public:
	// Gabari Drawing objects
	CElementContainer m_objectsGabari;
	// Drawing objects
	CElementContainer m_objects;
	// Selection objects
	CElementContainer m_selection;
	// Clipboard objects
	CElementContainer m_clipboard;
	// Grouped Objects
	vector<shared_ptr<CElementGroup>> m_groups;
	std::wstring m_elementGroup;

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
	bool m_bSavingCode;
	SelectType m_selectType;
	ConnectorType m_connectorInUse;
	bool m_bDrawRectForConnectionPoint;
	CRect m_DrawRectForConnectionPoint;
	CRect m_DractRectHandleTrackerForConnectionPoint;
	CTextControlDialog* m_pDialog;
	bool m_bTextDialogOpen;
	bool m_ShowBackground;
	
public:
	// Selection 1st point
	CPoint m_selectPoint;
	// Selection Rect
	CRect m_selectionRect;
	CPoint m_clickPoint;
	bool m_bSelectionHasStarted;
	shared_ptr<CElement> pSelectionElement;
	bool m_bSizingALine;

// Methods for Attributes
public:
	//SelectMode GetSelectMode()	 { return m_selectMode; }
	const vector<std::shared_ptr<CElement>>& GetObjects() { return m_objects.m_objects; }
	bool IsTextDialogOpen() { return m_bTextDialogOpen; }

// Debugging Operations
public:
	void DebugDumpObjects(CModeler1View * pView);

// Operations
public:
	void FromJSON(const web::json::object& object);
	web::json::value AsJSON() const;
	void Serialize_LoadAsXML(CModeler1View * pView);
	void Serialize_SaveAsXML(CModeler1View * pView);
	void ConnectToMainFrame(CModeler1View * pView);
	void ConnectToPropertyGrid();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	void RemoveSelectedObjects(CModeler1View * pView);
	void ViewToManager(CModeler1View * pView, CPoint & point, CElement* pElement = nullptr);
	void ViewToManager(CModeler1View * pView, CRect & rect, CElement* pElement = nullptr);
	void ManagerToView(CModeler1View * pView, CPoint & point, CElement* pElement = nullptr);
	void ManagerToView(CModeler1View * pView, CRect & rect, CElement* pElement = nullptr);
	COLORREF GetPaperColor() const { return m_paperColor; }
	void Invalidate(CModeler1View * pView, std::shared_ptr<CElement> pElement);
	void Invalidate(CModeler1View * pView, BOOL erase = TRUE);
	void InvalObj(CModeler1View * pView, std::shared_ptr<CElement> pElement);
	CSize GetSize() const { return m_size; }
	void UpdateFromPropertyGrid(std::wstring objectId, std::wstring name, std::wstring value);
	void UpdateFromPropertyGrid(std::wstring objectId, std::wstring name, COLORREF color);
	void UpdateFromPropertyGrid(std::wstring objectId, std::wstring name, long value);
	void ActivateView(CModeler1View * pView, bool bActivate, CView* pActiveView, CView* pDeactiveView);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

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
	void AlignLeft(CModeler1View* pView);
	void AlignRight(CModeler1View* pView);
	void AlignTop(CModeler1View* pView);
	void AlignBottom(CModeler1View* pView);
	void AlignTextLeft(CModeler1View* pView);
	void AlignTextCenter(CModeler1View* pView);
	void AlignTextRight(CModeler1View* pView);
	void ExpandHigh(CModeler1View* pView);
	void ExpandLarge(CModeler1View* pView);

// Managing Background drawing
public:
	void DrawBackground(CModeler1View * pView, CDC * pDC);
	void DrawPaperLines(CModeler1View* pView, CDC* pDC);

// Managing UI object connections
public:
	void FindAConnectionFor(std::shared_ptr<CElement> pLineElement, CPoint point, CModeler1View* pView, ConnectorType connector);
	void SetConnector(std::shared_ptr<CElement> pLineElement, std::shared_ptr<CElement> pElementFound, ConnectorType connector);

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
	void OnFileOpenGabarit(CModeler1View* pView);
	CString SearchDrive(const CString& strFile, const CString& strFilePath, const bool& bRecursive, const bool& bStopWhenFound);
	std::vector<std::shared_ptr<CCodeFile>> _files;
	void LoadFolders(CModeler1View* pView);
	void OpenFolder(CModeler1View* pView);
	void OpenFile(CModeler1View* pView);
	void OpenFileContent(CModeler1View* pView);

// Managing Object Selection
public:
	bool HasSelection();
	bool IsSelected(std::shared_ptr<CElement> pElement);
	bool Select(std::shared_ptr<CElement> pElement);
	bool Deselect(std::shared_ptr<CElement> pElement);
	void SelectNone();
	void DrawSelectionRect(CModeler1View *pView);
	void OnSelectAll(CModeler1View* pView);

// Managing Grouping
public:
	void OnEditGroup(CModeler1View* pView);
	void OnEditUngroup(CModeler1View* pView);
	std::vector<std::wstring> Split(const std::wstring& s, wchar_t delim);
	void BuildGroups();
	void ExpandGroupAttributes();

// Managing Connecting
public:
	void OnDesignDeconnect(CModeler1View* pView);

// Overridables
public:
	virtual void PrepareDC(CModeler1View * pView, CDC* pDC, CPrintInfo* pInfo);
	virtual void Draw(CModeler1View * pView, CDC * pDC);
	virtual void DrawEx(CModeler1View * pView, CDC * pDC);
	virtual void Update(CModeler1View * pView, LPARAM lHint, CObject* pHint);
	void DrawConnector(Graphics& graphics, std::shared_ptr<CElement> pLineElement, ConnectorType connector);

// UI Handlers
public:
	virtual void OnLButtonDown(CModeler1View* pView, UINT nFlags, const CPoint& cpoint);
	virtual void OnLButtonDblClk(CModeler1View* pView, UINT nFlags, const CPoint& cpoint);
	virtual void OnLButtonUp(CModeler1View* pView, UINT nFlags, const CPoint& cpoint);
	virtual void OnMouseMove(CModeler1View* pView, UINT nFlags, const CPoint& cpoint);

// File 
public:
	void OnFileExportPNG(CModeler1View* pView);
	void OnFileImportJSON(CModeler1View* pView);
	void OnFileExportJSON(CModeler1View* pView);
// Font
public:
	void OnFontBold(CModeler1View* pView);
	void OnFontItalic(CModeler1View* pView);
	void OnFontUnderline(CModeler1View* pView);
	void OnFontStrikeThrough(CModeler1View* pView);
	void OnFontGrowFont(CModeler1View* pView);
	void OnFontShrink(CModeler1View* pView);
	void OnFontClearFormat(CModeler1View* pView);
	void OnFontColor(CModeler1View* pView);
	void OnFontTextHighlight(CModeler1View* pView);
	void OnFontChangeCase(CModeler1View* pView);

// Elements
public:
	void OnActionElements(CModeler1View* pView);
	void BuildElementsCombo(CModeler1View* pView);
	void OnElementsScalePlus(CModeler1View* pView);
	void OnElementsScaleMoins(CModeler1View* pView);
	void OnShapesLeftTop(CModeler1View* pView, ShapeType shapeType);
	void OnShapesCenter(CModeler1View* pView, ShapeType shapeType);

// Rotation
public:
	void OnFomatRotateRight90(CModeler1View* pView);
	void OnFomatRotateLeft90(CModeler1View* pView);

// Licensing
public:
	bool IsMyLocalDev();

// ActiveViews
public:
	void SetActiveView(CModeler1View* pView, View view);

// Windows Edit Controls
public:
	void HideAllEditControls();

};

enum FileType : int
{
	file,
	folder
};

class CCodeFile
{
public:
	CCodeFile() {}
	virtual ~CCodeFile() {}

public:
	std::wstring _name;
	std::wstring _path;
	FileType _type;
};
