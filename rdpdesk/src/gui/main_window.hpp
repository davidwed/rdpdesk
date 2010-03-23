///////////////////////////////////////////////////////////////////////////////
// File name:   main_window.hpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-21 21:14:43" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////


#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

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


#include "main.hpp"
#include "bencoded.hpp"
#include "RDPOptionsDialog.hpp"
#include "ICAOptionsDialog.hpp"

#ifdef __WXGTK__
class TreePanel;
#endif

const int ID_HIDEFRAME_RESTORE = 240;
const int ID_HIDEFRAME_EXIT = 241;
const int ID_HIDEFRAME_SETTINGS = 242;
const int ID_HIDEFRAME_NEW = 243;


class HideFrame : public wxTaskBarIcon
{
public:

	HideFrame(Main_Frame * main)
	{
		main_frame = main;
	}
	
	virtual wxMenu *CreatePopupMenu();

	void on_double_left_click(wxTaskBarIconEvent& event);

	void on_menu_restore(wxCommandEvent& event);
	void on_menu_exit(wxCommandEvent& event);
	void on_menu_connect(wxCommandEvent& event);
	void on_menu_new(wxCommandEvent& event);
	void on_menu_settings(wxCommandEvent& event);

	Main_Frame * main_frame;
	DECLARE_EVENT_TABLE()
};


const int ID_AUITOOLBAR_EXIT = 350;
const int ID_AUITOOLBAR_NEWCONN = 351;
const int ID_AUITOOLBAR_FASTCONN = 352;
const int ID_AUITOOLBAR_LOADRDPFILE = 353;
const int ID_AUITOOLBAR_EXPORTRDPFILE = 354;
const int ID_AUITOOLBAR_CONNECT = 355;
const int ID_AUITOOLBAR_DISCONNECT = 356;
const int ID_AUITOOLBAR_FULLSCREEN = 357;
const int ID_AUITOOLBAR_CLOSEPAGE = 358;
const int ID_AUITOOLBAR_SHOWTREE = 359;
const int ID_AUITOOLBAR_HIDEFRAME = 360;
const int ID_AUITOOLBAR_OPTIONS = 361;
const int ID_AUITOOLBAR_ABOUT = 362;
const int ID_AUITOOLBAR_SAVECONNECTED = 363;
const int ID_AUITOOLBAR_SCREENPHOTO = 366;

const int ID_AUITOOLBAR_GRABINPUT = 364;
const int ID_AUITOOLBAR_SENDCAD = 365;




class MainSplitter : public wxSplitterWindow
{
public:
	
	MainSplitter(Main_Frame * parent, wxWindowID id = -1,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxPanelNameStr);

		
	virtual void OnDoubleClickSash(int x, int y){};
};

class Main_Frame : public wxFrame
{
public:
	
   Main_Frame(const wxString& title, const wxPoint& pos, const wxSize& size, long style = wxDEFAULT_FRAME_STYLE );
   ~Main_Frame();

   void on_quit(wxCommandEvent& event);
   void on_about(wxCommandEvent& event);
   void on_config(wxCommandEvent& event);

   void on_connect(wxCommandEvent& event);
   void on_disconnect(wxCommandEvent& event);
   void on_fullscreen(wxCommandEvent& event);
   void on_closepage(wxCommandEvent& event);
   void on_grabinput(wxCommandEvent& event);
   void on_sendcad(wxCommandEvent& event);
   void on_screenphoto(wxCommandEvent& event);	

   void on_closepage_ne();

   void closepage_num_func(wxUpdateUIEvent &event);
   int current_page_for_delete;

   void notebook_change_func(wxAuiNotebookEvent& event);
   void notebook_change_func_ne();

   void notebook_changing_func(wxAuiNotebookEvent& event);
   void notebook_closepage_func(wxAuiNotebookEvent& event);

   void notebook_closepage_func_ne();
   void notebook_closepage_event(wxCommandEvent& event);

   void on_userdpfile(wxCommandEvent& event);
   void on_exportrdpfile(wxCommandEvent& event);

   void on_fastconn(wxCommandEvent& event);
   void clear_rdpconn(RDPConn *rdp_conn);

   //void AddRDP(RDPConn rdpconn, int info_uniq_name = 0);
   void AddRDP(Options_HashMap local_options, int info_uniq_name = 0);
   void FocusCurrentPage();
   void FocusCurrentPageEvt(wxUpdateUIEvent& event);

   void on_showtree(wxCommandEvent& event);
   BOOL IsTree;
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

   void switch_current(BOOL bDisconnect);
   BOOL bNeedConnect;

   void RedrawAll(wxUpdateUIEvent& event);
   void RedrawTabs(wxUpdateUIEvent& event);
	
   void EnableTabs(BOOL bEnable);
   void EnableTabsEvt(wxUpdateUIEvent& event);
   void DisableTabsEvt(wxUpdateUIEvent& event);

   void ShowTree(BOOL bShow);
	
   void on_closing(wxCloseEvent& event);

   base_conn base;
   Connections_List all_connection_records;
   

#ifdef __WXGTK__
   void connection_status_update(wxCommandEvent& event);
   void switch_splitter();
   void error_message(wxString error_mgs);
#endif


	
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
   wxAuiToolBar * main_auitoolbar_settings;

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

   BOOL bMaximizeEvent;

   BOOL bShowFrame;
   BOOL bFocusNewPage;
   BOOL bUseHideFrame;

   BOOL bIconized;
   BOOL bMaximized;
   BOOL bUseCrypt;

   BOOL bNeedHideFrame;

   void Add_Connections_Record(Options_HashMap *local_options);
   void Del_Connections_Record(int uniq_name);
   
   
private:
   BOOL FrameOK;
   DECLARE_EVENT_TABLE()
};

const int ID_NEWCONNECTION = 150 ;
const int ID_USERDPFILE = 151 ;
const int ID_EXITPROGRAM = 152 ;
const int ID_CONNECT = 153 ;
const int ID_DISCONNECT = 154 ;
const int ID_FULLSCREEN = 155 ;
const int ID_CLOSEPAGE = 156 ;
const int ID_ABOUTDIALOG = 157 ;
const int ID_NOTEBOOK = 158;
const int ID_EXPORTRDPFILE = 159 ;
const int ID_FASTCONN = 160;
const int ID_ERRORHOSTNAME = 149;

const int ID_SHOWTREE = 148;
const int ID_HIDEFRAME = 147;
const int ID_SETTINGSDIALOG = 146;
const int ID_SAVECONNECTED = 145;
const int ID_SCREENPHOTO = 161;

const int ID_FOCUSPAGE = 162;

const int ID_GRABINPUT = 144;
const int ID_SENDCAD = 143;


const int ID_CONNECTION_STATUS_UPDATE = 9000;
const int ID_CLOSE_RDESKTOP_PRG = 9001;
const int ID_RDESKTOP_NOT_FOUND = 9002;

const int ID_REDRAW_ALL = 7800;
const int ID_REDRAW_TABS = 7801;
const int ID_ENABLE_TABS = 7802;
const int ID_DISABLE_TABS = 7803;


#endif
