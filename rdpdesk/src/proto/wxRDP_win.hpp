#ifndef WXRDP_HPP
#define WXRDP_HPP

#pragma warning( disable : 4267)

#include <wx/wx.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/notebook.h>

#include <wx/aui/aui.h> 
#include <wx/aui/auibook.h> 
#include "wx/treectrl.h"
#include "wx/taskbar.h"

#include <wx/socket.h>

#include <wx/splitter.h>


#include "splitter.hpp"
///////////////////////

#include "wx/msw/ole/activex.h"
#include <oleidl.h>
#include <exdisp.h>
#include <docobj.h>

#include <objbase.h>
#include <atlbase.h>
#include <atlctl.h>
#include <iostream>
#import "mstscax.dll"
///////////////////////

wchar_t * AnsiToUnicode(const char * buffer);

#define RDP_EVENT_CONNECTED 2
#define RDP_EVENT_DISCONNECTED 4
#define RDP_EVENT_ENTERFULLSCREEN 5
#define RDP_EVENT_LEAVEFULLSCREEN 6


class wxRDP : public wxWindow
{
public:
	 wxRDP(Main_Frame * main,RDPConn rdpsettings ,wxWindow * parent, REFCLSID clsid, wxWindowID id = -1,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxPanelNameStr);

	~wxRDP();

	wxActiveXContainer * cnt;
	const MSTSCLib::IMsRdpClientPtr pRDP; 

	RDPConn rdpconn; 
	Main_Frame * main_frame;

	BOOL DoRdp();
	void Connect();
	void Disconnect();
	void FullScreen(BOOL bRestore);
	void SendKey(BOOL cad = TRUE);
	void photo();

	BOOL bConnected;
	BOOL bFullScreen;
	BOOL bWaitFlag;
	BOOL bNeedReconnect;

	void on_enter_focus(wxFocusEvent& event);

	wxString Info;
	int info_uniq_name;
	
	static DWORD WINAPI TestHost(wxRDP * rdp);
	HANDLE hTestHostThread;

	wxSplitterRDP * splitter_rdp;
	wxBitmap m_screenshot;
	void screenshot(wxRDP * rdp);

	void DispatcherActiveX(wxActiveXEvent& event);

	DECLARE_EVENT_TABLE()

};

#endif
