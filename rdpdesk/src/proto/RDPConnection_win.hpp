///////////////////////////////////////////////////////////////////////////////
// File name:   RDPConnection_win.hpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-03 18:20:30" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#ifndef WXRDP_HPP
#define WXRDP_HPP

#pragma warning( disable : 4267)
/*
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
*/
#include "BasicConnection.hpp"
#import "mstscax.dll"
///////////////////////

wchar_t * AnsiToUnicode(const char * buffer);

#define RDP_EVENT_CONNECTED 2
#define RDP_EVENT_DISCONNECTED 4
#define RDP_EVENT_ENTERFULLSCREEN 5
#define RDP_EVENT_LEAVEFULLSCREEN 6
#define RDP_EVENT_CONFIRMCLOSE	0xf

class RDPConnection : public BasicConnection//wxWindow
{
public:
	 RDPConnection(MainFrame * main,Options_HashMap options ,wxWindow * parent , wxWindowID id = -1,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxPanelNameStr);

	~RDPConnection();

	//wxActiveXContainer * cnt;
	//const MSTSCLib::IMsRdpClientPtr pRDP; 

	//RDPConn rdpconn; 
	//Main_Frame * main_frame;

	virtual bool DoConnection();
	virtual bool Connect();
	virtual void Disconnect();
	virtual void FullScreen(BOOL bRestore);

	virtual void SendKey(BOOL cad);
	virtual void GrabAll(BOOL state);
	virtual void CentreConnection();
	virtual bool DisconnectClose();
	//void SendKey(BOOL cad = TRUE);
	//void photo();

	//BOOL bConnected;
	//BOOL bFullScreen;
	//BOOL bWaitFlag;
	//BOOL bNeedReconnect;

	void on_enter_focus(wxFocusEvent& event);

	//wxString Info;
	//int info_uniq_name;
	
	static DWORD WINAPI TestHost(RDPConnection * rdp);
	HANDLE hTestHostThread;

	//wxSplitterRDP * splitter_rdp;
	//wxBitmap m_screenshot;
	//void screenshot(wxRDP * rdp);

	void DispatcherActiveX(wxActiveXEvent& event);

	DECLARE_EVENT_TABLE()
private:
	Options_HashMap local_options;
	void OnNeedReconnect(wxUpdateUIEvent& event);

	void OnConfirmCloseSuccess();

};

const int ID_RDPWIN_NEED_RECONNECT_EVENT = 3200;

#endif
