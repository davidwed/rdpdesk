///////////////////////////////////////////////////////////////////////////////
// File name:   RDPConnection_nix.hpp
// Version:     0.0
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef WXRDP_NIX_HPP
#define WXRDP_NIX_HPP

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

//#include "rdpbasedefs.hpp"
//#include "defineds.hpp"
//#include "splitter.hpp"

#include <wx/wupdlock.h>

#include <X11/Intrinsic.h>
#include <X11/keysym.h>

const int WXRDP_OK = 7000;
const int RDESKTOP_NOT_FOUND = 7001;
const int WXRDP_HOST_EMPTY = 7002;

/*
const int ID_FULLSCREEN_LEAVE = 500;
const int ID_FULLSCREEN_CLOSE = 501;
const int ID_FULLSCREEN_GRABINPUT = 502;
const int ID_FULLSCREEN_SENDCAD = 503;
const int ID_FULLSCREEN_PHOTO = 504;
const int ID_FULLSCREEN_AUTOHIDE = 505;
*/

//class wxRDP;
//class wxSplitterRDP;
class RDPConnection;



class wxContRDP : public wxWindow
{
public:
	wxContRDP(wxWindow * parent,wxWindowID id,const wxPoint& point,const wxSize& size);

void	on_set_focus(wxFocusEvent& event);
void	on_kill_focus(wxFocusEvent& event);
void	on_mouse_events(wxMouseEvent& event);

DECLARE_EVENT_TABLE()
};


class RDPFullScreen : public wxTopLevelWindow
{
   friend class RDPConnection;

public:
	RDPFullScreen(MainFrame *main, wxContRDP * cnt, Options_HashMap options);
   ~RDPFullScreen();
private:

//	wxRDP * rdp;
	RDPConnection * rdp;

	//RDPConn rdpc;
	Options_HashMap local_options;

	wxContRDP * cnt_old;

	wxScrolledWindow * win;
	ToolbarCnt * win2;
	FullScreenToolBar * tb;

	void on_toolbar(wxCommandEvent& event);
	void Leave();
	void dialog_hotkeys(wxKeyEvent &event);
	MainFrame *main_frame;
   DECLARE_EVENT_TABLE()
};

class RDPConnection : public BasicConnection
{
public:
   RDPConnection(MainFrame * m_frame, Options_HashMap all_options,
		 wxWindow *parent, wxWindowID id = -1,
		 const wxPoint& pos = wxDefaultPosition,
		 const wxSize& size = wxDefaultSize,
		 long style = 0,
		 const wxString& name = wxPanelNameStr);




   ~RDPConnection();


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


   void CentreRDP();
   void CheckWindow();


   virtual void SendKey(BOOL_L cad = FALSE);
   void SendXKeyEvent(XKeyEvent * event);
   virtual void GrabAll(BOOL_L state);
//	BOOL_L bGrab;


//	wxContRDP * cnt;

   BOOL_L bClosing;

//	RDPConn rdpconn;
   wxString Info;

   int info_uniq_name;

//	Main_Frame * main_frame;
//	bool bConnected;
//	BOOL_L bFullScreen;

   RDPFullScreen * fullscreen_frame;

   ConnSplitter * conn_splitter;
   wxWindowUpdateLocker * lock;
   wxWindowUpdateLocker * lock_cnt;


   static void  wait_exit(GPid pid, gint status, gpointer data);


   GtkWidget *  sock;
   int sock_id;

   int output_fd;
   int error_fd;

private:

//	wxSplitterRDP *m_parent;
//	ConnSplitter * m_parent;


   wxInputStream* m_in;
   wxOutputStream* m_out;
   wxString m_filename;
   //RDPConn rdp_settings;
   Options_HashMap local_options;

   XID xid;
   wxString Error_mgs;
   wxString Messgage;

   int wxrdp_status;

   DECLARE_EVENT_TABLE()
};


#endif
