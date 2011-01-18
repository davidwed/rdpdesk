///////////////////////////////////////////////////////////////////////////////
// File name:   BasicConnection.hpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef BASICCONNECTION_HPP
#define BASICCONNECTION_HPP

#ifdef __WXMSW__
#pragma warning( disable : 4267)
#endif

#include <memory>
#include <wx/wx.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/notebook.h>

#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include "wx/treectrl.h"
#include "wx/taskbar.h"



#include <wx/splitter.h>


#include "splitter.hpp"
///////////////////////
#ifdef __WXMSW__
#include "wx/msw/ole/activex.h"
#include <oleidl.h>
#include <exdisp.h>
#include <docobj.h>
#include <wx/socket.h>

#include <objbase.h>
#include <atlbase.h>
#include <atlctl.h>
#include <iostream>
#endif

#ifdef __WXMSW__
#define WindowConnectionClass wxWindow
#endif

#ifdef __WXGTK__
#define WindowConnectionClass wxScrolledWindow
#endif

#ifdef BACKAND
#include "res/close-session-icon-16x16.cpp"
#include "res/keyboard-input-icon-16x16.cpp"
#include "res/control-alt-del-icon-16x16.cpp"
#include "res/window-nofullscreen-icon-16x16.cpp"
#include "res/make-bmp-screenshot-icon-16x16.cpp"
#include "res/eject-16x16.cpp"
#endif //BACKAND

enum {
	ID_FULLSCREEN_LEAVE       = 500,
	ID_FULLSCREEN_CLOSE       = 501,
	ID_FULLSCREEN_GRABINPUT   = 502,
	ID_FULLSCREEN_SENDCAD     = 503,
	ID_FULLSCREEN_PHOTO       = 504,
	ID_FULLSCREEN_AUTOHIDE    = 505,
	ID_FULLSCREEN_TABSMENU    = 506,
	ID_FULLSCREEN_CONNTREE    = 507,
	ID_FULLSCREEN_ADD_TO_MENU = 520,
	ID_FULLSCREEN_DEL_TO_MENU = 521,
};



class FullScreenToolBar : public wxAuiToolBar {
public :
	FullScreenToolBar(const wxString &caption, wxWindow *parent, wxWindowID id,
	                  const wxPoint& point, const wxSize& size);
	FullScreenToolBar(MainFrame * main, const wxString &caption,
	                  wxWindow * parent, wxWindowID id,
	                  const wxPoint& point, const wxSize& size);
	~FullScreenToolBar();
	void ProcessEvent (wxCommandEvent& event);
	void init(const wxString &caption, wxWindow * parent, wxWindowID id,
	          const wxPoint& point, const wxSize& size);

	wxStaticText * m_caption;

	wxWindow * fullscreen;
	wxWindow * win;
	wxMenu* EstablishedConnectionsMenu;
	wxMenu* ConnectionsMenu;
	MainFrame * main_frame;

	DECLARE_EVENT_TABLE();
};

class ToolbarCnt : public wxWindow {
public:
	ToolbarCnt(wxWindow * parent);
	~ToolbarCnt() {}

	void on_enter(wxMouseEvent& event);
	void on_leave_func(wxMouseEvent& event);
	void on_leave();
	FullScreenToolBar * fs;
	wxSize original_size;
#ifdef __WXGTK__
	DECLARE_EVENT_TABLE();
#endif

};


class BasicConnection : public WindowConnectionClass {
public:
	BasicConnection(MainFrame * main, const Options_HashMap &all_options,
	                wxWindow * parent,
	                wxWindowID id = wxID_ANY,
	                const wxPoint& pos = wxDefaultPosition,
	                const wxSize& size = wxDefaultSize,
	                long style = 0,
	                const wxString& name = wxPanelNameStr);

	virtual ~BasicConnection();
   wxWindow * cnt;

#ifdef __WXMSW__
   IUnknown * pUnknown;
#endif

   //RDPConn rdpconn;
   MainFrame * main_frame;
////
   virtual bool DoConnection() = 0;
   virtual bool Connect() = 0;
   virtual void Disconnect() = 0;
   virtual void FullScreen(BOOL_L bRestore) = 0;
   virtual void SendKey(BOOL_L cad) = 0;
   virtual void GrabAll(BOOL_L state) = 0;

   virtual void CentreConnection() = 0;
   virtual bool DisconnectClose() = 0;

   Options_HashMap Get_Options();
   void SendConnectEvent();
   void SendDisconnectEvent();
   void SendEnterFullscreenEvent();
   void SendLeaveFullscreenEvent();

   void CheckOptions();
   void photo();

   BOOL_L bConnected;
   BOOL_L bFullScreen;
   BOOL_L bWaitFlag;
   BOOL_L bNeedReconnect;
   BOOL_L bGrab;

   BOOL_L bObjectOk;

   wxString Info;
   int info_uniq_name;

   ConnSplitter * conn_splitter;
   wxBitmap m_screenshot;
   void screenshot(wxWindow * win);

   int uniq_conn;
   void SetUniqConn();
   Options_HashMap options;

};



/*
class ConnFullScreen : public wxTopLevelWindow
{
	friend class BasicConnection;

public:
	RDPFullScreen(wxWindow * cnt,RDPConn rdpconn);
	~RDPFullScreen();
private:

//	wxRDP * rdp;
	BasicConnection * conn;

	RDPConn rdpc;
	wxContRDP * cnt_old;

	wxScrolledWindow * win;
	ToolbarCnt * win2;
	FullScreenToolBar * tb;

	void on_toolbar(wxCommandEvent& event);
	void Leave();

	DECLARE_EVENT_TABLE()

};

*/

#endif // BASICCONNECTION_HPP
