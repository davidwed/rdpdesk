// UltraVncAxObj.cpp : Implementation of CUltraVncAxObj

#include "stdafx.h"
#include "UltraVncAx.h"
#include "UltraVncAxObj.h"

#include <shlobj.h>

/////////////////////////////////////////////////////////////////////////////

// == We can exchange data with the VNC APP through global variables because the "Connect" method doesn't return until the main app window is created. ==

// -- hwnd of the currently initializing ActiveX window:
HWND g_hwndAx = NULL;

// -- hwnd of the VNC APP main frame:
HWND g_hwndAppFrame = NULL;

/////////////////////////////////////////////////////////////////////////////

// -- wrapper class instance around the original app:
static CUltraVncAxGlobalConstructor g_cUltraVncAxGlobalConstructor;

/////////////////////////////////////////////////////////////////////////////
// CUltraVncAxObj

CUltraVncAxObj::CUltraVncAxObj() :
	m_bConnectedEvFired( FALSE )
{
	m_bWindowOnly = TRUE;

	m_scale = FALSE;
	m_fullscreen = FALSE;

	// initialize the VNCViewer object.
	g_cUltraVncAxGlobalConstructor.Construct ( _Module.m_hInst );

	// attach the hook fn.
	m_hHook = ::SetWindowsHookEx( WH_KEYBOARD, & fnHookKeyboard, NULL, ::GetCurrentThreadId() );
}

CUltraVncAxObj::~ CUltraVncAxObj()
{
	// free the hook.
	if ( m_hHook )
		::UnhookWindowsHookEx( m_hHook );
}

HRESULT CUltraVncAxObj::OnDraw(ATL_DRAWINFO& di)
{
	// draw a box and return.

	RECT& rc = *(RECT*)di.prcBounds;

	::SelectObject( di.hdcDraw, ::GetStockObject( BLACK_BRUSH ) );
	::SelectObject( di.hdcDraw, ::GetStockObject( BLACK_PEN ) );

	::Rectangle( di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom );

	return S_OK;
}

STDMETHODIMP CUltraVncAxObj::Connect()
{
	//
	// can enter ???
	//

	if ( CanEnter () == FALSE )
		return E_FAIL;

	// validate.
	if ( m_csServer.size () == 0 )
		return AtlReportError( GetObjectCLSID (), "Server name not specified.", GUID_NULL, 0 );

	// connect.

	g_hwndAx = m_hWnd;
	g_hwndAppFrame = NULL;
	{
		if ( g_cUltraVncAxGlobalConstructor.NewConnection( m_csServer.c_str (), this->m_csClearPassword.c_str(), m_scale, m_fullscreen, m_shared, m_proxyhost.c_str(), m_proxyport, m_viewonly ) == FALSE )
			return AtlReportError( GetObjectCLSID (), "Invalid server name specified.", GUID_NULL, 0 );
	}
	g_hwndAx = NULL;

	// connect op succeeded ?

	HWND hwndAppFrame = g_hwndAppFrame;
	g_hwndAppFrame = NULL;

	if ( hwndAppFrame == NULL )
		return AtlReportError( GetObjectCLSID (), "Connect operation timed out or authentication error.", GUID_NULL, 0 );
	else if ( hwndAppFrame == GetVncWnd() )
	{
		// fire the connected ev...
		m_bConnectedEvFired = TRUE;
		Fire_OnConnected ();
	}
	else
	{
		::SendMessage( hwndAppFrame, WM_CLOSE, 0, 0 );
		return AtlReportError( GetObjectCLSID (), "Unspecified connect error.", GUID_NULL, 0 );
	}

	// return.
	return S_OK;
}

STDMETHODIMP CUltraVncAxObj::Disconnect()
{
	//
	// can enter ???
	//

	if ( CanEnter () == FALSE )
		return E_FAIL;

	// disconnect.
	HWND		h = GetVncWnd();
	if ( h == NULL )
		return AtlReportError( GetObjectCLSID (), "You are not connected to any server.", GUID_NULL, 0 );
	else
		::SendMessage( h, WM_CLOSE, 0, 0 );

	// return.
	return S_OK;
}

STDMETHODIMP CUltraVncAxObj::get_Connected(BOOL *pVal)
{
	//
	// can enter ???
	//

	if ( CanEnter () == FALSE )
		return E_FAIL;

	// return whether we are connected or not...
	* pVal = GetVncWnd () != NULL && m_bConnectedEvFired;
	return S_OK;
}

STDMETHODIMP CUltraVncAxObj::get_IsInitialized(BOOL *pVal)
{
	//
	// can enter ???
	//

	if ( CanEnter () == FALSE )
		return E_FAIL;

	// return whether we are initialized...
	* pVal = m_hWnd != NULL;
	return S_OK;
}

STDMETHODIMP CUltraVncAxObj::get_Server(BSTR *pVal)
{
	//
	// can enter ???
	//

	if ( CanEnter () == FALSE )
		return E_FAIL;

	// return the information.
	* pVal = ::SysAllocString( ::CharStringToWideString( m_csServer ).c_str() );
	return S_OK;
}

STDMETHODIMP CUltraVncAxObj::put_Server(BSTR newVal)
{
	//
	// can enter ???
	//

	if ( CanEnter () == FALSE )
		return E_FAIL;

	// store the server name...

	charstring		cs = ::BstrToCharstring( newVal );
	::TrimAtBothSides( cs );
	m_csServer = cs;

	return S_OK;
}


STDMETHODIMP CUltraVncAxObj::ExecuteCommand( /*[in]*/ BSTR cmdText, /*[out, retval]*/ BSTR *pRetVal )
{
	//
	// can enter ???
	//

	if ( CanEnter () == FALSE )
		return E_FAIL;

	// parse the input params.

	charstring			csCmdText = ::BstrToCharstring( cmdText );
	charstring			csRetVal = "Undefined.";

	// process the command.

	HWND		h = GetVncWnd();
	if ( h == NULL )
		return AtlReportError( GetObjectCLSID (), "You are not connected to any server.", GUID_NULL, 0 );
	else
	{
		// call the implementation.

		BOOL bIsErr = FALSE;

		csRetVal = g_cUltraVncAxGlobalConstructor.ExecuteCommand( h, csCmdText, bIsErr );
		if ( bIsErr )
			return AtlReportError( GetObjectCLSID (), csRetVal.c_str (), GUID_NULL, 0 );
	}

	// return.
	* pRetVal = ::SysAllocString( ::CharStringToWideString( csRetVal ).c_str() );
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK fnEnum ( HWND hwnd, LPARAM lParam )
{
	// return the first child...
	* (HWND*) lParam = hwnd;
	return FALSE;
}

HWND CUltraVncAxObj::GetChildWnd( HWND parent )
{
	// enum the child windows.
	HWND hwnd = NULL;
	::EnumChildWindows( parent, & fnEnum, (LPARAM) & hwnd );
	return hwnd;
}

HWND CUltraVncAxObj::GetVncWnd ()
{
	// call the implementation.
	return GetChildWnd( m_hWnd );
}

LRESULT CUltraVncAxObj::OnDestroy (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// we have to call the WM_CLOSE handler of the VNC app window ?

	HWND hwndVnc = GetVncWnd ();

	if ( hwndVnc )
		::SendMessage( hwndVnc, WM_CLOSE, 0, 0 );

	// return.
	return 0;
}

LRESULT CUltraVncAxObj::OnRelayMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// we have to relay the msg ?

	HWND hwndVnc = GetVncWnd ();

	if ( hwndVnc )
	{
		::SendMessage( hwndVnc, uMsg, wParam, lParam );
		bHandled = TRUE;
	}

	// return.
	return 0;
}

LRESULT CUltraVncAxObj::OnParentNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// which event ?
	if ( LOWORD( wParam ) == WM_DESTROY && lParam != 0 )
	{
		// we have to fire the corresponding ev ?
		if ( m_bConnectedEvFired )
		{
			m_bConnectedEvFired = FALSE;
			Fire_OnDisconnected( CComBSTR( "Connection ended." ) );
		}
	}

	// return.
	return 0;
}

BOOL CUltraVncAxObj::IsVncWnd( HWND hwnd )
{
	if ( hwnd == NULL )
		return FALSE;

	// get and check the class name...
	CHAR		szClassName[ MAX_PATH ] = "";
	::GetClassName( hwnd, szClassName, sizeof( szClassName ) );
	if ( ::strcmp( szClassName, "VNCMDI_Window" ) == 0 ||
		::strcmp( szClassName, "VNCviewerwindow" ) == 0 )
	{
		// return a non-zero value.
		return TRUE;
	}

	// return failure.
	return FALSE;
}

static WPARAM s_wpVirtualKeysToIntercept[] =
{
	VK_CANCEL,
	VK_BACK, VK_TAB,
	VK_CLEAR, VK_RETURN,
	VK_SHIFT, VK_CONTROL, VK_MENU, VK_PAUSE, VK_CAPITAL,
	VK_ESCAPE,
	VK_SPACE, VK_PRIOR, VK_NEXT, VK_END, VK_HOME, VK_LEFT, VK_UP, VK_RIGHT, VK_DOWN, VK_SELECT, VK_PRINT, VK_EXECUTE, VK_SNAPSHOT, VK_INSERT, VK_DELETE, VK_HELP,
	VK_LWIN, VK_RWIN, VK_APPS,
	VK_NUMLOCK, VK_SCROLL
};

static BOOL VkHasToIntercept( WPARAM wp )
{
	// check in the vector...
	for( int i=0; i<sizeof(s_wpVirtualKeysToIntercept)/sizeof(WPARAM); i ++ )
		if ( s_wpVirtualKeysToIntercept[ i ] == wp )
			return TRUE;

	// return failure...
	return FALSE;
}

LRESULT CALLBACK CUltraVncAxObj::fnHookKeyboard( int code, WPARAM wParam, LPARAM lParam )
{
	// process.
	if ( code < 0 )
		return ::CallNextHookEx( NULL, code, wParam, lParam );
	else if ( code == HC_ACTION )
	{
		// which key ?
		if ( ( lParam & (1<<29) ) || ::VkHasToIntercept( wParam ) )
		{
			// which window ?
			HWND		hwnd = ::GetFocus ();
			if ( hwnd )
			{
				HWND		child = GetChildWnd( hwnd );

				BOOL		resParent = IsVncWnd( hwnd );
				BOOL		resChild = IsVncWnd( child );
				if ( resParent || resChild )
				{
					// process the message...
					g_cUltraVncAxGlobalConstructor.ProcessKeyEvent( resParent ? hwnd : child, wParam, lParam );

					// return a nonzero value.
					return 1;
				}
			}
		}
	}

	// return.
	return ::CallNextHookEx( NULL, code, wParam, lParam );
}

/////////////////////////////////////////////////////////////////////////////



/////////////////
BOOL CUltraVncAxObj::CanEnter ()
{
/*	// check out the URL of the calling page...

	HRESULT				hr;

	CComPtr<IServiceProvider>			pISP;
	hr = m_spClientSite->QueryInterface( IID_IServiceProvider, (void**) & pISP );

	if ( FAILED( hr ) || pISP == NULL )
		return FALSE;

	CComPtr<IWebBrowserApp>				pIWebBrowserApp;
	hr = pISP->QueryService( SID_SWebBrowserApp, IID_IWebBrowserApp, (void**) & pIWebBrowserApp );

	if ( FAILED( hr ) || pIWebBrowserApp == NULL )
		return FALSE;

	BSTR			bstrUrl;
	hr = pIWebBrowserApp->get_LocationURL( & bstrUrl );

	if ( FAILED( hr ) )
		return FALSE;

	charstring		csUrl = ::BstrToCharstring( bstrUrl );
	::SysFreeString( bstrUrl );

#ifndef _DEBUG

	CHAR*				psz = "https://www.gototerminal.com/";

	if ( csUrl.size () < ::strlen( psz ) ||
		::memicmp( csUrl.c_str (), psz, ::strlen( psz ) ) )
	{
		::MessageBox( NULL, "This Active-X object can be used only from https://www.GoToTerminal.com.\n\nPlease navigate to this address and try again...", "GoToTerminal", MB_OK );
		return FALSE;
	}

#endif

	// return success.
*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


STDMETHODIMP CUltraVncAxObj::put_Password(BSTR newVal)
{
	// TODO: Add your implementation code here
	m_csClearPassword = BstrToCharstring(newVal);
	return S_OK;
}

STDMETHODIMP CUltraVncAxObj::put_Scale(VARIANT_BOOL newVal)
{
	// TODO: Add your implementation code here
	if(newVal)
	{
		m_scale = TRUE;
	}
	else
	{
		m_scale = FALSE;
	}
	return S_OK;
}

STDMETHODIMP CUltraVncAxObj::put_FullScreen(VARIANT_BOOL newVal)
{
	// TODO: Add your implementation code here
	m_fullscreen = newVal;
	return S_OK;
}

STDMETHODIMP CUltraVncAxObj::put_Shared(VARIANT_BOOL newVal)
{
	m_shared = newVal;
	return S_OK;
}

STDMETHODIMP CUltraVncAxObj::put_ProxyHost(BSTR newVal)
{
	m_proxyhost = BstrToCharstring(newVal);
	return S_OK;
}

STDMETHODIMP CUltraVncAxObj::put_ProxyPort(ULONG newVal)
{
	m_proxyport = newVal;
	return S_OK;
}

STDMETHODIMP CUltraVncAxObj::put_ViewOnly(VARIANT_BOOL newVal)
{
	m_viewonly = newVal;
	return S_OK;
}
STDMETHODIMP CUltraVncAxObj::sendCAD(void)
{
	HWND		hwnd = ::GetFocus ();
			if ( hwnd )
			{
				HWND		child = GetChildWnd( hwnd );

				BOOL		resParent = IsVncWnd( hwnd );
				BOOL		resChild = IsVncWnd( child );
				if ( resParent || resChild )
				{
					// process the message...
					g_cUltraVncAxGlobalConstructor.ProcessCTRL_ALT_DEL( resParent ? hwnd : child );
					// return a nonzero value.
				}
			}
	
	return S_OK;
}
