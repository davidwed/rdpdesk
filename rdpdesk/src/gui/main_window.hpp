///////////////////////////////////////////////////////////////////////////////
// File name:   main_window.hpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-11-30 21:18:15"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////


#ifndef __MAIN_WINDOW_HPP__
#define __MAIN_WINDOW_HPP__

/*
  #include "settings_dialog.hpp"

  #include <wx/process.h>
  #include <wx/txtstrm.h>
  #include <wx/wupdlock.h>

  #ifdef __WXMSW__
  #include "wxRDP_win.hpp"
  #include "ICAConnection.hpp"
  #endif

  #ifdef __WXGTK__
  #include "wxRDP_nix.hpp"
  #include "splitter.hpp"
  #endif
  #include "defineds.hpp"
*/

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/wupdlock.h>

#include <wx/aui/aui.h>
#include <wx/aui/auibook.h>
#include <wx/treectrl.h>
#include <wx/taskbar.h>
#include <wx/tbarbase.h>
#include <wx/dir.h>
#include <wx/aboutdlg.h>
#include <wx/html/htmlwin.h>

#include "main.hpp"
#include "bencoded.hpp"
#include "RDPOptionsDialog.hpp"
#include "ICAOptionsDialog.hpp"
#include "RFBOptionsDialog.hpp"
#include "FRDPOptionsDialog.hpp"

#ifdef __WXGTK__
class TreePanel;
#endif

enum {
   ID_HIDEFRAME_RESTORE  = 240,
   ID_HIDEFRAME_EXIT     = 241,
   ID_HIDEFRAME_SETTINGS = 242,
   ID_HIDEFRAME_NEW      = 243,
};

class MainFrame;

class HideFrame : public wxTaskBarIcon {
public:
	HideFrame(MainFrame * main) : main_frame(main) {}

	virtual wxMenu *CreatePopupMenu();

	void on_double_left_click(wxTaskBarIconEvent& event);

	void on_menu_restore(wxCommandEvent& event);
	void on_menu_exit(wxCommandEvent& event);
	void on_menu_connect(wxCommandEvent& event);
	void on_menu_new(wxCommandEvent& event);
	void on_menu_settings(wxCommandEvent& event);

	MainFrame * const main_frame;
	DECLARE_EVENT_TABLE()
};

enum {
	ID_AUITOOLBAR_EXIT          = 350,
	ID_AUITOOLBAR_NEWCONN       = 351,
	ID_AUITOOLBAR_FASTCONN      = 352,
	ID_AUITOOLBAR_LOADRDPFILE   = 353,
	ID_AUITOOLBAR_EXPORTRDPFILE = 354,
	ID_AUITOOLBAR_CONNECT       = 355,
	ID_AUITOOLBAR_DISCONNECT    = 356,
	ID_AUITOOLBAR_FULLSCREEN    = 357,
	ID_AUITOOLBAR_CLOSEPAGE     = 358,
	ID_AUITOOLBAR_SHOWTREE      = 359,
	ID_AUITOOLBAR_HIDEFRAME     = 360,
	ID_AUITOOLBAR_OPTIONS       = 361,
	ID_AUITOOLBAR_ABOUT         = 362,
	ID_AUITOOLBAR_SAVECONNECTED = 363,
	ID_AUITOOLBAR_GRABINPUT     = 364,
	ID_AUITOOLBAR_SENDCAD       = 365,
	ID_AUITOOLBAR_SCREENPHOTO   = 366,
};

class HtmlMessageViewer : public wxHtmlWindow {
public:
	HtmlMessageViewer(wxWindow *parent,
	                  wxWindowID id = wxID_ANY,
	                  const wxPoint& pos = wxDefaultPosition,
	                  const wxSize& size = wxDefaultSize,
	                  long style = wxHW_SCROLLBAR_AUTO,
	                  const wxString& name = _("Message"));
	void OnLinkClicked(const wxHtmlLinkInfo& link);
};

class MainSplitter : public wxSplitterWindow {
public:
	MainSplitter(MainFrame * parent,
	             wxWindowID id = wxID_ANY,
	             const wxPoint& pos = wxDefaultPosition,
	             const wxSize& size = wxDefaultSize,
	             long style = 0,
	             const wxString& name = wxPanelNameStr);

	virtual void OnDoubleClickSash(int x, int y){};
};

class MainFrame : public wxFrame {
public:
   MainFrame(const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style = wxDEFAULT_FRAME_STYLE);
   ~MainFrame();

	void on_quit(wxCommandEvent& event);
	void on_about(wxCommandEvent& event);
	void on_config(wxCommandEvent& event);
	void OnBuildinDoc(wxCommandEvent& event);

	void on_connect(wxCommandEvent& event);
	void on_disconnect(wxCommandEvent& event);
	void on_fullscreen(wxCommandEvent& event);
	void on_closepage(wxCommandEvent& event);
	void on_grabinput(wxCommandEvent& event);
	void on_sendcad(wxCommandEvent& event);
	void on_screenphoto(wxCommandEvent& event);

	void on_closepage_ne();

	void closepage_num_func(wxUpdateUIEvent &event);

	void notebook_change_func(wxAuiNotebookEvent& event);
	void notebook_change_func_ne();

	void notebook_changing_func(wxAuiNotebookEvent& event);
	void notebook_closepage_func(wxAuiNotebookEvent& event);

	void notebook_closepage_func_ne();
	void notebookClosePageEvent(wxCommandEvent& event);
	void notebook_middle_press_on_tab_func(wxAuiNotebookEvent& event);

	void on_userdpfile(wxCommandEvent& event);
	void on_exportrdpfile(wxCommandEvent& event);

	void OnFastConn(wxCommandEvent& event);
	void clear_rdpconn(RDPConn *rdp_conn);

	//void AddRDP(RDPConn rdpconn, int info_uniq_name = 0);
	void AddRDP(Options_HashMap local_options, int info_uniq_name = 0);
	void FocusCurrentPage();
	void FocusCurrentPageEvt(wxUpdateUIEvent& event);

	void on_showtree(wxCommandEvent& event);
	void on_sizing(wxSizeEvent& event);

	void on_hideframe(wxCommandEvent& event);
	void on_iconize(wxIconizeEvent& event);

	void on_settings_dialog(wxCommandEvent& event);

	void on_saveconnection(wxCommandEvent& event);

	void on_auitoolbar(wxCommandEvent& event);
	void enable_auitoolbar_conn(bool state);
	void on_auitoolbar_showtree(wxCommandEvent& event);
	void on_auitoolbar_hideframe(wxCommandEvent& event);
	void on_auitoolbar_grabinput(wxCommandEvent& event);

//	wxLocale * locale;

	void on_maximize(wxMaximizeEvent& event);

	void switch_current(BOOL_L bDisconnect);
	void RedrawAll(wxUpdateUIEvent& event);
	void RedrawTabs(wxUpdateUIEvent& event);

	void EnableTabs(BOOL_L bEnable);
	void EnableTabsEvt(wxUpdateUIEvent& event);
	void DisableTabsEvt(wxUpdateUIEvent& event);

	void ShowTree(BOOL_L bShow);

	void on_closing(wxCloseEvent& event);
	void ParseUpdateFile(wxCommandEvent& event);

	base_conn base;

#ifdef __WXGTK__
	void connectionStatusUpdate(wxCommandEvent& event);
	void switch_splitter();
	void error_message(wxString error_mgs);
#endif

	int current_page_for_delete;
	BOOL_L IsTree;
	BOOL_L bNeedConnect;

	wxAuiNotebook * nb;
	wxAuiManager aui;

	//RDPTree * tree;
	wxTreeCtrl * tree;


	wxPanel * m_panel_nb;
	//TreePanel * m_panel_tree;
	wxPanel * m_panel_tree;

	wxScrolledWindow * m_window_tree;
	wxScrolledWindow * m_window_nb;
	MainSplitter * m_splitter;

	wxPanel * m_panel_maintoolbar;
	wxAuiToolBar * main_auitoolbar_general;
	wxAuiToolBar * main_auitoolbar_connection;
	wxAuiToolBar * mainAUIToolbarSettings;

	void EnableConnectionMenu();
	void DisableConnectionMenu();
	void CheckCurrentConnectionMenu();

	wxDialog * m_dialog_tree;

	wxMenu *file_menu;
	wxMenu *currentConnMenu;
	wxMenu *helpMenu;
	wxMenu *settingsMenu;
	wxMenuBar *menu_bar;
	wxTextFile  *BaseFile;

	HideFrame * m_hideframe;
	wxMutex m_mutex_status;

	BOOL_L bMaximizeEvent;

	BOOL_L bShowFrame;
	BOOL_L bFocusNewPage;
	BOOL_L bUseHideFrame;

	BOOL_L bIconized;
	BOOL_L bMaximized;
	BOOL_L bUseCrypt;

	BOOL_L bNeedHideFrame;

   void AddConnectionRecord(Options_HashMap *local_options);
   void Del_Connections_Record(int uniq_name);

	Connections_List all_connection_records; // TODO: hide in private section
private:
   void OpenWebPage(wxCommandEvent &event);
   BOOL_L FrameOK;
   DECLARE_EVENT_TABLE()
};

enum {
	ID_SENDCAD                  = 143,
	ID_GRABINPUT                = 144,
	ID_SAVECONNECTED            = 145,
	ID_SETTINGSDIALOG           = 146,
	ID_HIDEFRAME                = 147,
	ID_SHOWTREE                 = 148,
	ID_ERRORHOSTNAME            = 149,
	ID_NEWCONNECTION            = 150,
	ID_USERDPFILE               = 151,
	ID_EXITPROGRAM              = 152,
	ID_CONNECT                  = 153,
	ID_DISCONNECT               = 154,
	ID_FULLSCREEN               = 155,
	ID_CLOSEPAGE                = 156,
	ID_ABOUTDIALOG              = 157,
	ID_NOTEBOOK                 = 158,
	ID_EXPORTRDPFILE            = 159,
	ID_FASTCONN                 = 160,
	ID_SCREENPHOTO              = 161,
	ID_FOCUSPAGE                = 162,
	ID_DOCUMENTATION            = 163,
	ID_FORUM                    = 164,
	ID_SUPPORT                  = 165,
	ID_BUILDIN_DOC              = 166,


	ID_REDRAW_ALL               = 7800,
	ID_REDRAW_TABS              = 7801,
	ID_ENABLE_TABS              = 7802,
	ID_DISABLE_TABS             = 7803,

	ID_CONNECTION_STATUS_UPDATE = 9000,
	ID_CLOSE_RDESKTOP_PRG       = 9001,
	ID_RDESKTOP_NOT_FOUND       = 9002,
};


#endif
