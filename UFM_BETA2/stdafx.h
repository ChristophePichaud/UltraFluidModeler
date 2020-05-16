// Copyright (C) SAS NET Azure Rangers
// All rights reserved.

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define _CRT_SECURE_NO_WARNINGS TRUE

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#include <afxdlgs.h>			// MFC support for common dialogs

#include <GdiPlus.h>
using namespace Gdiplus;

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


//
// Boost section
//

#define BOOST_SERIALIZATION_DYN_LINK TRUE
#define BOOST_ALL_DYN_LINK TRUE

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/archive/tmpdir.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/assume_abstract.hpp>
//using namespace boost;

#ifdef _WIN64
    #ifdef _DEBUG
        #pragma comment (lib, "boost_container-vc140-mt-gd.lib")
        #pragma comment (lib, "boost_serialization-vc140-mt-gd.lib")
        #pragma comment (lib, "boost_wserialization-vc140-mt-gd.lib")
    #else    
        #pragma comment (lib, "boost_container-vc140-mt.lib")
        #pragma comment (lib, "boost_serialization-vc140-mt.lib")
        #pragma comment (lib, "boost_wserialization-vc140-mt.lib")
    #endif
#endif

/*
#define USE_SCINTILLA
#define SCI_NAMESPACE
#ifdef USE_SCINTILLA
#include <platform.h>
#include <scintilla.h>
#include <SciLexer.h>
#endif
*/

#include <afx.h>

#include "cpprest/json.h"
    
#ifdef _WIN64
    #ifdef _DEBUG
        #pragma comment (lib, "Lib\\cpprest_2_10d.lib")
    #else    
        #pragma comment (lib, "Lib\\cpprest_2_10.lib")
    #endif
#endif

#define VERSION_COMMUNITY
#define MAX_SHAPES          50

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

