#pragma once


// CModeler1SourceView view

class CModeler1SourceView : public CEditView
{
	DECLARE_DYNCREATE(CModeler1SourceView)

protected:
	CModeler1SourceView();           // protected constructor used by dynamic creation
	virtual ~CModeler1SourceView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


