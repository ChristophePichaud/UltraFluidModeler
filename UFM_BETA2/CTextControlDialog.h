#pragma once

// CTextControlDialog dialog

class CElement;

class CTextControlDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTextControlDialog)

public:
	CTextControlDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTextControlDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TEXT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_Text;
	shared_ptr<CElement> m_pElement;
    CEdit m_Text2;
};
