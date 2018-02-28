#include "StdAfx.h"
#include "AppInit.h"


CAppInit::CAppInit(void)
{
}


CAppInit::~CAppInit(void)
{
}


void CAppInit::Init()
{
	// TODO: Add extra initialization here
	ULONG_PTR uptr;
	GdiplusStartupInput input;
	input.GdiplusVersion = 1;
	input.DebugEventCallback = NULL;
	input.SuppressBackgroundThread = FALSE;
	input.SuppressExternalCodecs = FALSE;
	GdiplusStartupOutput output;
	uptr = GdiplusStartup(&uptr, &input, &output);
}

void CAppInit::Uninit()
{
}


