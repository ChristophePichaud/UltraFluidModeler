// Copyright (C) SAS NET Azure Rangers
// All rights reserved.

// Modeler1.h : main header file for the Modeler1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CModeler1App:
// See Modeler1.cpp for the implementation of this class
//

class CModeler1App : public CWinAppEx
{
public:
	CModeler1App();

private:
	HINSTANCE m_hSciDLL;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	CMultiDocTemplate* m_pDocTemplate;
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CModeler1App theApp;
