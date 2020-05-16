// CTextControlDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "CTextControlDialog.h"
#include "Element.h"
#include "ElementManager.h"
#include "afxdialogex.h"
#include "resource.h"

// CTextControlDialog dialog

IMPLEMENT_DYNAMIC(CTextControlDialog, CDialogEx)

CTextControlDialog::CTextControlDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TEXT, pParent)
{

}

CTextControlDialog::~CTextControlDialog()
{
}

void CTextControlDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TEXT, m_Text);
    DDX_Control(pDX, IDC_TEXT2, m_Text2);
}


BEGIN_MESSAGE_MAP(CTextControlDialog, CDialogEx)
    ON_BN_CLICKED(IDOK, &CTextControlDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTextControlDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTextControlDialog message handlers


void CTextControlDialog::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString text1;
	m_Text.GetWindowText(text1);
	CString text2;
	m_Text2.GetWindowText(text2);

	if (m_pElement->IsLine() == true)
	{
		m_pElement->m_textConnector1 = (LPTSTR)(LPCTSTR)text1;
		m_pElement->m_textConnector2 = (LPTSTR)(LPCTSTR)text2;
		m_pElement->GetManager()->Invalidate(m_pElement->m_pView);
	}
	else
	{
		m_pElement->m_text = (LPTSTR)(LPCTSTR)text1;
		m_pElement->GetManager()->Invalidate(m_pElement->m_pView);
	}

	ShowWindow(SW_HIDE);

	//CDialogEx::OnOK();
}


void CTextControlDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
	//CDialogEx::OnCancel();
}
