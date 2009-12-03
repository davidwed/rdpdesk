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
#include "rdpbasedefs.hpp"
#include "defineds.hpp"
#include "splitter.hpp"

#include <wx/wupdlock.h>

#include <X11/Intrinsic.h>
#include <X11/keysym.h>



#include "res/emblem-unreadable.xpm"
//#include "res/user-desktop.xpm"
#include "res/input-keyboard16.xpm"
#include "res/go-down16.xpm"
#include "res/preferences-system-windows.xpm"
#include "res/camera-photo16.xpm"
#include "res/media-eject.xpm"


const int WXRDP_OK = 7000;
const int RDESKTOP_NOT_FOUND = 7001;
const int WXRDP_HOST_EMPTY = 7002;

const int ID_FULLSCREEN_LEAVE = 500;
const int ID_FULLSCREEN_CLOSE = 501;
const int ID_FULLSCREEN_GRABINPUT = 502;
const int ID_FULLSCREEN_SENDCAD = 503;
const int ID_FULLSCREEN_PHOTO = 504;
const int ID_FULLSCREEN_AUTOHIDE = 505;

class wxRDP;
class wxSplitterRDP;


class wxContRDP : public wxWindow
{
public:
	wxContRDP(wxWindow * parent,wxWindowID id,const wxPoint& point,const wxSize& size);
	
void	on_set_focus(wxFocusEvent& event);
void	on_kill_focus(wxFocusEvent& event);
void	on_mouse_events(wxMouseEvent& event);

DECLARE_EVENT_TABLE()
};

class FullScreenToolBar : public wxAuiToolBar
{
public :
	FullScreenToolBar(wxString caption, wxWindow * parent, wxWindowID id, const wxPoint& point, const wxSize& size);
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
	void on_leave(wxMouseEvent& event);
	FullScreenToolBar * fs;
	wxSize original_size;
	
	DECLARE_EVENT_TABLE()
	

};

class RDPFullScreen : public wxTopLevelWindow
{
	friend class wxRDP;
	
public:
	RDPFullScreen(wxContRDP * cnt,RDPConn rdpconn);
	~RDPFullScreen();
private:
	
	wxRDP * rdp;
	RDPConn rdpc;
	wxContRDP * cnt_old;

	wxScrolledWindow * win;
	ToolbarCnt * win2;
	FullScreenToolBar * tb;
		
	void on_toolbar(wxCommandEvent& event);
	void Leave();
	
	DECLARE_EVENT_TABLE()
	
};

class wxRDP : public wxScrolledWindow
{
public:
	wxRDP(Main_Frame * m_frame, RDPConn rdpsettings ,wxSplitterRDP *parent, wxWindowID id = -1,
		  const wxPoint& pos = wxDefaultPosition,
		  const wxSize& size = wxDefaultSize,
		  long style = 0,
		  const wxString& name = wxPanelNameStr);


	~wxRDP();
	bool DoRdp();
	void FullScreen(BOOL bRestore);
	void photo();

	void close_rdesktop_prg(wxCommandEvent& event);
	void rdesktop_not_found(wxCommandEvent& event);
	void event_dispatcher(wxCommandEvent& event);
	void OnIdle(wxIdleEvent& event);
	void OnTimer(wxTimerEvent& event);
	bool request_close(BOOL bDetach = FALSE);
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
	void EnableFocus(BOOL state);
	void CheckFocus();
	

	void CentreRDP();
	void CheckWindow();
	

	void SendKey(BOOL cad = FALSE);
	void SendXKeyEvent(XKeyEvent * event);
	void GrabAll(BOOL state);
	BOOL bGrab;
	

	wxContRDP * cnt;

	BOOL bClosing;
	
	RDPConn rdpconn;
	wxString Info;
	
	int info_uniq_name;

	Main_Frame * main_frame;
	bool bConnected;
	BOOL bFullScreen;

	RDPFullScreen * fullscreen_frame;
		
	wxSplitterRDP * splitter_rdp;
	wxWindowUpdateLocker * lock;
	wxWindowUpdateLocker * lock_cnt;
	

	static void  wait_exit(GPid pid, gint status, gpointer data);
	

    GtkWidget *  sock;
	int sock_id;

	int output_fd;
	int error_fd;
	
private:

	wxSplitterRDP *m_parent;

	wxInputStream* m_in;
	wxOutputStream* m_out;
	wxString m_filename;
	RDPConn rdp_settings;

	XID xid;
	
	wxString Error_mgs;
	wxString Messgage;

	int wxrdp_status;

	DECLARE_EVENT_TABLE()
};


#endif
