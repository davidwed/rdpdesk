///////////////////////////////////////////////////////////////////////////////
// File name:   RFBConnection_nix.hpp
// Version:     0.0
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RFBDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef WXRFB_NIX_HPP
#define WXRFB_NIX_HPP

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

#include "BasicConnection.hpp"

#include <wx/wupdlock.h>

#include <X11/Intrinsic.h>
#include <X11/keysym.h>

const int WXRFB_OK = 9000;
const int RDPDESK_VNCVIEWER_NOT_FOUND = 9001;
const int WXRFB_HOST_EMPTY = 9002;
const int WXRFB_WINDOW_RFV = 9003;


/*
const int ID_FULLSCREEN_LEAVE = 500;
const int ID_FULLSCREEN_CLOSE = 501;
const int ID_FULLSCREEN_GRABINPUT = 502;
const int ID_FULLSCREEN_SENDCAD = 503;
const int ID_FULLSCREEN_PHOTO = 504;
const int ID_FULLSCREEN_AUTOHIDE = 505;
*/

//class wxRFB;
//class wxSplitterRFB;
class RFBConnection;


class RFBFindWindow : public wxThread
{
public:
   RFBFindWindow(int parent_window_id, int *child_window_id, RFBConnection *parent);
   //void SetParametrs();
   virtual void OnExit();
private:
   virtual void *Entry();
   int display_tree_info_1(Window window, int recurse, int level);
   int local_parent_window_id;
   int *local_child_window_id;
   RFBConnection *local_parent;
};

class wxContRFB : public wxWindow
{
public:
   wxContRFB(wxWindow * parent,wxWindowID id,const wxPoint& point,const wxSize& size);

   void	on_set_focus(wxFocusEvent& event);
   void	on_kill_focus(wxFocusEvent& event);
   void	on_mouse_events(wxMouseEvent& event);

   DECLARE_EVENT_TABLE()
};


class RFBFullScreen : public wxTopLevelWindow
{
   friend class RFBConnection;

public:
   RFBFullScreen(wxContRFB * cnt, Options_HashMap options);
   ~RFBFullScreen();
private:

//	wxRFB * rdp;
   RFBConnection * rdp;

   //RFBConn rdpc;
   Options_HashMap local_options;

   wxContRFB * cnt_old;

   wxScrolledWindow * win;
   ToolbarCnt * win2;
   FullScreenToolBar * tb;

   void on_toolbar(wxCommandEvent& event);
   void Leave();
   void dialog_hotkeys(wxKeyEvent &event);
   DECLARE_EVENT_TABLE()
};




class RFBConnection : public BasicConnection
{
public:
   RFBConnection(MainFrame * m_frame, Options_HashMap all_options,
		 wxWindow *parent, wxWindowID id = -1,
		 const wxPoint& pos = wxDefaultPosition,
		 const wxSize& size = wxDefaultSize,
		 long style = 0,
		 const wxString& name = wxPanelNameStr);

   ~RFBConnection();


   virtual bool DoConnection();
   virtual bool Connect();
   virtual void Disconnect();
   virtual bool DisconnectClose();
   virtual void CentreConnection();

   bool DoRdp();
   void FullScreen(BOOL_L bRestore);
   void photo();

   void close_rdesktop_prg(wxCommandEvent& event);
   void rdesktop_not_found(wxCommandEvent& event);
   void event_dispatcher(wxCommandEvent& event);
   void OnIdle(wxIdleEvent& event);
   void OnTimer(wxTimerEvent& event);
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

   void CentreRFB();
   void CheckWindow();

   virtual void SendKey(BOOL_L cad = FALSE);
   void SendXKeyEvent(XKeyEvent * event);
   virtual void GrabAll(BOOL_L state);
   void WindowRFB (wxCommandEvent& event);

//	BOOL_L bGrab;


//	wxContRFB * cnt;

   BOOL_L bClosing;

//	RFBConn rdpconn;
   wxString Info;

   int info_uniq_name;

   RFBFullScreen * fullscreen_frame;

   ConnSplitter * conn_splitter;
   wxWindowUpdateLocker * lock;
   wxWindowUpdateLocker * lock_cnt;

   static void  wait_exit(GPid pid, gint status, gpointer data);

   GtkWidget *  sock;
   int sock_id;

   int output_fd;
   int error_fd;

private:

//	wxSplitterRFB *m_parent;
//	ConnSplitter * m_parent;
   // Window GetRFBWindowId();
   // Window GetWindowByTitle(Display * dpy, Window top, char * name);

   wxInputStream* m_in;
   wxOutputStream* m_out;
   wxString m_filename;
   //RFBConn rdp_settings;
   Options_HashMap local_options;
   RFBFindWindow *finder;

   XID xid;
   wxString Error_mgs;
   wxString Messgage;

   int wxrdp_status;
   Window rfb_window;

   DECLARE_EVENT_TABLE()
};


#endif
