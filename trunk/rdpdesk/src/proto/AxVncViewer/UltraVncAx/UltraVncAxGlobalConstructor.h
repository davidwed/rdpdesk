//===============================
// UltraVncAxGlobalConstructor.h
//===============================

#pragma once

/////////////////////////////////////////////////////////////////////////////

#include "CCritSec.h"
#include "Helpers.h"

/////////////////////////////////////////////////////////////////////////////

class CUltraVncAxGlobalConstructor
{
public:

	// destruction.
	virtual ~ CUltraVncAxGlobalConstructor ();

public:

	// methods.
	VOID Construct ( HINSTANCE hInst );
	BOOL NewConnection ( CONST CHAR* pszHost, CONST CHAR* pszPassword, BOOL scale, BOOL fullscreen, BOOL shared, CONST CHAR* proxyHost, ULONG proxyPort, BOOL viewOnly );
	VOID ProcessKeyEvent( HWND hwnd, WPARAM wParam, LPARAM lParam );
	VOID ProcessCTRL_ALT_DEL(HWND hwnd);
	charstring ExecuteCommand( HWND hwnd, charstring& csCmdText, BOOL& bIsErr );

protected:

	// data.
	CCriticalSection				m_csConstructAccess;
};

/////////////////////////////////////////////////////////////////////////////
