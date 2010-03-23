///////////////////////////////////////////////////////////////////////////////
// File name:   BasicConnection.hpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-19 22:19:19" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#ifndef BASICCONNECTION_HPP
#define BASICCONNECTION_HPP

#ifdef __WXMSW__
#pragma warning( disable : 4267)
#endif

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

#include "res/emblem-unreadable.xpm"
//#include "res/user-desktop.xpm"
#include "res/input-keyboard16.xpm"
#include "res/go-down16.xpm"
#include "res/preferences-system-windows.xpm"
#include "res/camera-photo16.xpm"
#include "res/media-eject.xpm"


const int ID_FULLSCREEN_LEAVE = 500;
const int ID_FULLSCREEN_CLOSE = 501;
const int ID_FULLSCREEN_GRABINPUT = 502;
const int ID_FULLSCREEN_SENDCAD = 503;
const int ID_FULLSCREEN_PHOTO = 504;
const int ID_FULLSCREEN_AUTOHIDE = 505;


class FullScreenToolBar : public wxAuiToolBar
{
public :
	FullScreenToolBar(wxString caption, wxWindow * parent, wxWindowID id,
			  const wxPoint& point, const wxSize& size);
	~FullScreenToolBar();

	wxStaticText * m_caption;
	

	wxWindow * fullscreen;
	wxWindow * win;

};

class ToolbarCnt : public wxWindow
{
public:
	ToolbarCnt(wxWindow * parent);
	~ToolbarCnt() 
	{
	};

	void on_enter(wxMouseEvent& event);
	void on_leave_func(wxMouseEvent& event);
   	void on_leave();
	FullScreenToolBar * fs;
	wxSize original_size;
#ifdef __WXGTK__
	DECLARE_EVENT_TABLE();
#endif

};




class BasicConnection : public WindowConnectionClass
{
public:
   BasicConnection(Main_Frame * main,Options_HashMap all_options, wxWindow * parent,
		   wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
		   const wxSize& size = wxDefaultSize, long style = 0, const
		   wxString& name = wxPanelNameStr);

   ~BasicConnection();
   wxWindow * cnt;

#ifdef __WXMSW__	 
   IUnknown * pUnknown;
#endif

   //RDPConn rdpconn;
   Main_Frame * main_frame;
////
   virtual bool DoConnection() = 0;
   virtual bool Connect() = 0;
   virtual void Disconnect() = 0;
   virtual void FullScreen(BOOL bRestore) = 0;
   virtual void SendKey(BOOL cad) = 0;
   virtual void GrabAll(BOOL state) = 0;

   virtual void CentreConnection() = 0;
   virtual bool DisconnectClose() = 0;

   Options_HashMap Get_Options ();
   void SendConnectEvent();
   void SendDisconnectEvent();
   void SendEnterFullscreenEvent();
   void SendLeaveFullscreenEvent();

   void CheckOptions();
   void photo();

   BOOL bConnected;
   BOOL bFullScreen;
   BOOL bWaitFlag;
   BOOL bNeedReconnect;
   BOOL bGrab;

   BOOL bObjectOk;

   wxString Info;
   int info_uniq_name;
	
   ConnSplitter * conn_splitter;
   wxBitmap m_screenshot;
   void screenshot(wxWindow * win);

   int uniq_conn;
   void SetUniqConn();
   Options_HashMap options;
private:

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
