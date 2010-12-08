///////////////////////////////////////////////////////////////////////////////
// File name:   ICAConnection_nix.hpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-11-24 17:49:44"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef ICACONNECTION_NIX_HPP
#define ICACONNECTION_NIX_HPP


#include <wx/wx.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#ifdef __WXGTK__
#include "wx/gtk/win_gtk.h"
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gspawn.h>
#endif

#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include "wx/treectrl.h"
#include "wx/taskbar.h"
#include <wx/tbarbase.h>
#include <wx/dir.h>

#include <X11/Intrinsic.h>
#include <X11/keysym.h>

#define SW_EVENTS ( \
	PropertyChangeMask | \
	StructureNotifyMask | \
	ResizeRedirectMask | \
	SubstructureNotifyMask | \
	0)

#include <wx/wupdlock.h>
#include "BasicConnection.hpp"


class ICAConnection;
class wxContICA;


class ICAFullScreen : public wxTopLevelWindow
{
   friend class ICAConnection;
public:
   //ICAFullScreen(wxContICA * cnt,RDPConn rdpconn);
   ICAFullScreen(wxContICA * cnt,Options_HashMap options);

   ~ICAFullScreen();
private:
//	wxRDP * rdp;
   ICAConnection * ica;
   //RDPConn rdpc;
   Options_HashMap local_options;
   wxContICA * cnt_old;

   wxScrolledWindow * win;
   ToolbarCnt * win2;
   FullScreenToolBar * tb;
   void on_toolbar(wxCommandEvent& event);
   void Leave();
   DECLARE_EVENT_TABLE()
};

class wxContICA : public wxWindow
{
public:
   wxContICA(wxWindow * parent,wxWindowID id,const wxPoint& point,const wxSize& size);
   void	on_set_focus(wxFocusEvent& event);
   void	on_kill_focus(wxFocusEvent& event);
   void	on_mouse_events(wxMouseEvent& event);
DECLARE_EVENT_TABLE()
};


class ICAConnection : public BasicConnection
{
public:
   // ICAConnection(MainFrame * m_frame, RDPConn rdpsettings ,wxWindow *parent, wxWindowID id = -1,
   // 		 const wxPoint& pos = wxDefaultPosition,
   // 		 const wxSize& size = wxDefaultSize,
   // 		 long style = 0,
   // 		 const wxString& name = wxPanelNameStr);

   ICAConnection(MainFrame * m_frame, Options_HashMap options,wxWindow *parent, wxWindowID id = -1,
		 const wxPoint& pos = wxDefaultPosition,
		 const wxSize& size = wxDefaultSize,
		 long style = 0,
		 const wxString& name = wxPanelNameStr);


   ~ICAConnection();

   virtual bool DoConnection();
   virtual bool Connect();
   virtual void Disconnect();
   virtual bool DisconnectClose();
//	bool DoRdp();
   void FullScreen(BOOL_L bRestore);
   void photo();

   void close_rdesktop_prg(wxCommandEvent& event);
   void rdesktop_not_found(wxCommandEvent& event);
   void event_dispatcher(wxCommandEvent& event);
   void OnIdle(wxIdleEvent& event);
//	void OnTimer(wxTimerEvent& event);
   bool request_close(BOOL_L bDetach = FALSE);
   bool get_state();
   void set_state(bool state_to_set);
   bool get_proc_state();
   void set_proc_state(bool state_to_set);
   wxString get_error_string();
   wxString get_message_string();
   void drop_error_string();
   int get_state_wxrdp();

   void on_set_focus(wxFocusEvent& event);
   void on_leave_focus(wxFocusEvent& event);
   void on_any_mouse_event(wxMouseEvent& event);
   void EnableFocus(BOOL_L state);
   void CheckFocus();


   void CentreRDP();
   void CheckWindow();


   virtual void SendKey(BOOL_L cad = FALSE);
   void SendXKeyEvent(XKeyEvent * event);
   virtual void GrabAll(BOOL_L state);


   virtual void CentreConnection();


   ICAFullScreen * fullscreen_frame;

   BOOL_L bClosing;

//	RDPConn rdpconn;
   Options_HashMap local_options;
   wxString Info;

   int info_uniq_name;
//	wxSplitterRDP * splitter_rdp;
   wxWindowUpdateLocker * lock;
   wxWindowUpdateLocker * lock_cnt;


   static void  wait_exit(GPid pid, gint status, gpointer data);


   GtkWidget *  sock;
   int sock_id;

   int output_fd;
   int error_fd;


private:


   ConnSplitter *m_parent;

   wxInputStream* m_in;
   wxOutputStream* m_out;
   wxString m_filename;
   //RDPConn rdp_settings;

   XID xid;

   Window ica_window;
public:
   Window GetICAWindowId();
private:
   bool InjectWindow();

   wxSize GetRealGeometry(Window id);
   Window GetWindowByTitle(Display * dpy, Window top, char * name);
   wxString Error_mgs;
   wxString Messgage;

   int wxrdp_status;

   bool MakeIniFile();
   wxString ICALaunch;
   wxString appsrv_ini;
   DECLARE_EVENT_TABLE()
};



#endif
