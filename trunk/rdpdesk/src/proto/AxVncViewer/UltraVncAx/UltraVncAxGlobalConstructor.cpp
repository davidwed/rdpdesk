//===============================================================
// UltraVncAxGlobalConstructor.cpp
//
// -- Implementation file of class CUltraVncAxGlobalConstructor.
//===============================================================

/////////////////////////////////////////////////////////////////////////////

// -- App obj definition file:
#include "..\VNCviewerApp32.h"

// -- Resource symbols:
#include "..\res\resource.h"

// -- Win32 VncViewer app entry point function:
extern int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow);

// -- Global application object pointer (mantained by the VNCviewerApp class):
extern VNCviewerApp *pApp;

// -- other externs:
bool ParseDisplay(LPTSTR display, LPTSTR phost, int hostlen, int *port);

/////////////////////////////////////////////////////////////////////////////

#include "UltraVncAxGlobalConstructor.h"

//
// Implementation:
//

CUltraVncAxGlobalConstructor:: ~ CUltraVncAxGlobalConstructor ()
{
	// free.
	if ( pApp )
		delete pApp;
}

VOID CUltraVncAxGlobalConstructor::Construct ( HINSTANCE hInst )
{
	CCriticalSection::scope			__access__( m_csConstructAccess );

	// construct app object.
	if ( pApp == NULL )
	{
		CHAR*			pszCmdLine = "/nostatus /notoolbar";

		// create the app obj...
		WinMain( hInst, NULL, pszCmdLine, 0 );
		pApp = new VNCviewerApp32( hInst, pszCmdLine );
	}
}

BOOL CUltraVncAxGlobalConstructor::NewConnection ( CONST CHAR* pszHost,CONST CHAR* pszPassword , BOOL scale, BOOL fullscreen, BOOL shared, CONST CHAR* proxyHost, ULONG proxyPort, BOOL viewOnly)
{
	INT				iPort = 0;

	// parse host name.
	CHAR			host[ 256 ] = "";
	if ( ! ::ParseDisplay( const_cast<CHAR*>( pszHost ), host, 255, & iPort ) )
		return FALSE;
	//password
	strcpy(pApp->m_options.m_clearPassword, pszPassword);
	
	//scale
	if(scale)
	{
		pApp->m_options.m_scaling = true;
	}
	else
	{
		pApp->m_options.m_scaling = false;
	}
	if(fullscreen)
	{
		pApp->m_options.m_FullScreen = true;
	}
	else
	{
		pApp->m_options.m_FullScreen = false;
	}
	if(shared)
	{
		pApp->m_options.m_Shared = true;
	}
	else
	{
		pApp->m_options.m_Shared = false;
	}

	if(proxyHost)
	{
		if(strlen(proxyHost) > 0)
		{
			strcpy(pApp->m_options.m_proxyhost, proxyHost);
		}
		if(proxyPort)
		{
			pApp->m_options.m_proxyport = proxyPort;
		}
	}

	if(viewOnly)
	{
		pApp->m_options.m_ViewOnly = true;
	}
	else
	{
		pApp->m_options.m_ViewOnly = false;
	}


	// start the new connection.
	pApp->NewConnection ( host, iPort );
	// return.
	return TRUE;
}

VOID CUltraVncAxGlobalConstructor::ProcessKeyEvent( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	// call the implementation.
	ClientConnection*		pConn = (ClientConnection*) ::GetWindowLong( hwnd, GWL_USERDATA );
	if ( pConn->m_running )
		pConn->ProcessKeyEvent( (int) wParam, (DWORD) lParam );
}

VOID CUltraVncAxGlobalConstructor::ProcessCTRL_ALT_DEL(HWND hwnd)
{
	ClientConnection*		pConn = (ClientConnection*) ::GetWindowLong( hwnd, GWL_USERDATA );
	if ( pConn->m_running )
	{
		pConn->SendKeyEvent_ex(XK_Control_L, true);
		pConn->SendKeyEvent_ex(XK_Alt_L,     true);
		pConn->SendKeyEvent_ex(XK_Delete,    true);
		pConn->SendKeyEvent_ex(XK_Delete,    false);
		pConn->SendKeyEvent_ex(XK_Alt_L,     false);
		pConn->SendKeyEvent_ex(XK_Control_L, false);
	}
}

charstring CUltraVncAxGlobalConstructor::ExecuteCommand( HWND hwnd, charstring& csCmdText, BOOL& bIsErr )
{
	bIsErr = FALSE;
	charstring			csRetVal = "Command ok.";

	::TrimAtBothSides( csCmdText );

	// which command ?

	ClientConnection*		pConn = (ClientConnection*) ::GetWindowLong( hwnd, GWL_USERDATA );
	if ( pConn->m_running )
	{
		if ( ::stricmp( csCmdText.c_str (), "filetransfer" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, ID_FILETRANSFER, 0 );
		else if ( ::stricmp( csCmdText.c_str (), "disableremoteim" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, ID_DINPUT, 0 );
		else if ( ::stricmp( csCmdText.c_str (), "enableremoteim" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, ID_INPUT, 0 );
		else if ( ::stricmp( csCmdText.c_str (), "requestrefresh" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, ID_REQUEST_REFRESH, 0 );
		else if ( ::stricmp( csCmdText.c_str (), "fullcolors" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, ID_MAXCOLORS, 0 );
		else if ( ::stricmp( csCmdText.c_str (), "256colors" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, ID_256COLORS, 0 );
		else if ( ::stricmp( csCmdText.c_str (), "sendctrlaltdel" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, ID_CONN_CTLALTDEL, 0 );
		else if ( ::stricmp( csCmdText.c_str (), "sendstart" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, ID_CONN_CTLESC, 0 );
		else if ( ::stricmp( csCmdText.c_str (), "sendctrldown" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, ID_CONN_CTLDOWN, 0 );
		else if ( ::stricmp( csCmdText.c_str (), "sendctrlup" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, ID_CONN_CTLUP, 0 );
		else if ( ::stricmp( csCmdText.c_str (), "sendaltdown" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, ID_CONN_ALTDOWN, 0 );
		else if ( ::stricmp( csCmdText.c_str (), "sendaltup" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, ID_CONN_ALTUP, 0 );
		else if ( ::stricmp( csCmdText.c_str (), "about" ) == 0 )
			::SendMessage( hwnd, WM_SYSCOMMAND, IDD_APP_ABOUT, 0 );
		else
		{
			// return an error.
			csRetVal = "Invalid command.";
			bIsErr = TRUE;
		}
	}
	else
	{
		// return an error.
		csRetVal = "Client not connected.";
		bIsErr = TRUE;
	}

	// return.
	return csRetVal;
}

/////////////////////////////////////////////////////////////////////////////
