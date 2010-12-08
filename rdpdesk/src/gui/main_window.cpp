///////////////////////////////////////////////////////////////////////////////
// File name:   main_window.cpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-12-06 18:36:58"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include "main_window.hpp"

#include "fastconn_dialog.hpp"
#include "rdp_dialogs.hpp"
#include "settings_dialog.hpp"
#include "splitter.hpp"
#include "BasicConnection.hpp"
#include "tree_group.hpp"
#include "small_shoot.hpp"
#include "helpbrowser.hpp"

#include "res/save-settings-icon-24x24.cpp"
#ifdef __WXMSW__
#include "res/load-file-icon-24x24.cpp"
#include "res/file-export-icon-24x24.cpp"
#endif
#include "res/window-fullscreen-icon-24x24.cpp"
#include "res/instant-connection-icon-24x24.cpp"
#include "res/new-connection-icon-24x24.cpp"
#include "res/terminate-connection-24x24.cpp"
#include "res/establish-connection-24x24.cpp"
#include "res/application-settings-24x24.cpp"
#include "res/close-session-icon-24x24.cpp"
#include "res/quit-icon-24x24.cpp"
#include "res/open-connection-icon-24x24.cpp"
#include "res/make-bmp-screenshot-icon-24x24.cpp"
#ifdef __WXGTK__
#include "res/keyboard-input-icon-24x24.cpp"
#endif
#include "res/control-alt-del-icon-24x24.cpp"
#include "res/rdcm.cpp"



#ifdef __WXMSW__
#include <wincrypt.h>
#pragma comment (lib,"crypt32.lib")
#endif

#include <wx/xml/xml.h>
#include <wx/stdpaths.h>

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(ID_NEWCONNECTION,  MainFrame::on_config)
EVT_MENU(ID_USERDPFILE,     MainFrame::on_userdpfile)
EVT_MENU(ID_EXPORTRDPFILE,  MainFrame::on_exportrdpfile)
EVT_MENU(ID_EXITPROGRAM,    MainFrame::on_quit)
EVT_MENU(ID_CONNECT,        MainFrame::on_connect)
EVT_MENU(ID_DISCONNECT,     MainFrame::on_disconnect)
EVT_MENU(ID_FULLSCREEN,     MainFrame::on_fullscreen)
EVT_MENU(ID_CLOSEPAGE,      MainFrame::on_closepage)
EVT_MENU(ID_ABOUTDIALOG,    MainFrame::on_about)
EVT_MENU(ID_FASTCONN,       MainFrame::OnFastConn)
EVT_MENU(ID_SHOWTREE,       MainFrame::on_showtree)
EVT_MENU(ID_HIDEFRAME,      MainFrame::on_hideframe)
EVT_MENU(ID_SETTINGSDIALOG, MainFrame::on_settings_dialog)
EVT_MENU(ID_SAVECONNECTED,  MainFrame::on_saveconnection)
EVT_MENU(ID_GRABINPUT,      MainFrame::on_grabinput)
EVT_MENU(ID_SENDCAD,        MainFrame::on_sendcad)
EVT_MENU(ID_SCREENPHOTO,    MainFrame::on_screenphoto)
EVT_MENU(ID_DOCUMENTATION,  MainFrame::OpenWebPage)
EVT_MENU(ID_FORUM,          MainFrame::OpenWebPage)
EVT_MENU(ID_SUPPORT,        MainFrame::OpenWebPage)
EVT_MENU(ID_BUILDIN_DOC,    MainFrame::OnBuildinDoc)
EVT_AUINOTEBOOK_PAGE_CHANGED(ID_NOTEBOOK,  MainFrame::notebook_change_func)
EVT_AUINOTEBOOK_PAGE_CHANGING(ID_NOTEBOOK, MainFrame::notebook_changing_func)
EVT_AUINOTEBOOK_PAGE_CLOSE(ID_NOTEBOOK,    MainFrame::notebook_closepage_func)
EVT_AUINOTEBOOK_TAB_MIDDLE_UP(ID_NOTEBOOK,MainFrame::notebook_middle_press_on_tab_func)
EVT_UPDATE_UI(ID_ERRORHOSTNAME, MainFrame::closepage_num_func)
EVT_UPDATE_UI(ID_FOCUSPAGE,     MainFrame::FocusCurrentPageEvt)
EVT_UPDATE_UI(ID_REDRAW_ALL,    MainFrame::RedrawAll)
EVT_UPDATE_UI(ID_REDRAW_TABS,   MainFrame::RedrawTabs)
EVT_UPDATE_UI(ID_ENABLE_TABS,   MainFrame::EnableTabsEvt)
EVT_UPDATE_UI(ID_DISABLE_TABS,  MainFrame::DisableTabsEvt)
EVT_TOOL(ID_AUITOOLBAR_SHOWTREE,  MainFrame::on_auitoolbar_showtree )
EVT_TOOL(ID_AUITOOLBAR_HIDEFRAME, MainFrame::on_auitoolbar_hideframe )
EVT_TOOL(ID_AUITOOLBAR_GRABINPUT, MainFrame::on_auitoolbar_grabinput )
EVT_TOOL(wxID_ANY, MainFrame::on_auitoolbar )
EVT_MAXIMIZE(MainFrame::on_maximize)
EVT_ICONIZE(MainFrame::on_iconize)
EVT_SIZE(MainFrame::on_sizing)
EVT_CLOSE(MainFrame::on_closing)
#ifdef __WXGTK__
EVT_UPDATE_UI(ID_ERRORHOSTNAME, MainFrame::closepage_num_func)
EVT_COMMAND(wxID_ANY, ID_CLOSE_RDESKTOP_PRG,       MainFrame::notebookClosePageEvent)
EVT_COMMAND(wxID_ANY, ID_CONNECTION_STATUS_UPDATE, MainFrame::connectionStatusUpdate)
#endif
EVT_COMMAND(wxID_ANY, ID_UPFATE_FILE, MainFrame::ParseUpdateFile)
END_EVENT_TABLE()

BOOL_L bTestScreen = TRUE; // TODO: remove

MainSplitter::MainSplitter(MainFrame * parent, wxWindowID id, const wxPoint& pos,const wxSize& size,long style, const wxString& name)
   :wxSplitterWindow(parent,-1,pos,size,wxSP_3D)
{
}


///////////////////////////////////////////////////////////////////////////////
//! \brief About dialog
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_about(wxCommandEvent& event)
{
   wxAboutDialogInfo about;
   about.SetVersion(wxString::Format(wxT("%i.%i"), TS_VERSION_MAJOR, TS_VERSION_MINOR));
   about.SetName(TS_NAME);
   about.SetDescription(TS_DESCRIPTION);
   about.SetCopyright(TS_COPYRIGHT);
   wxIcon icon;
   //icon.CopyFromBitmap(LOAD_XPM(utilities_terminal32));
   icon.CopyFromBitmap(wxGetBitmapFromMemory(rdcm_png));
   about.SetWebSite(wxT("www.rdpdesk.com"), wxT("www.rdpdesk.com"));
   about.SetIcon(icon);
   wxAboutBox(about);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief processe Quit event
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_quit(wxCommandEvent& event)
{
   wxWindowUpdateLocker frame_lock(this);

   int iCount = (int)this->nb->GetPageCount();
   wxCommandEvent evt;
   evt.SetId(0);
   for (int i = 0; i < iCount; i++)
   {
      ConnSplitter * conn_splitter;
      conn_splitter = (ConnSplitter *)nb->GetPage(i);
      if (!conn_splitter) continue;

      BasicConnection * conn = conn_splitter->conn;
      if (!conn) continue;

      if (conn->bConnected)
      {
	 conn->DisconnectClose();
      }
   }
   Close();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process Copy event
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_closing(wxCloseEvent& event)
{
   Show(false);
   Destroy();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Add connections to form
//! \param Options_HashMap local_options - options array
//! \param int info_uniq_name - uniq number
///////////////////////////////////////////////////////////////////////////////
void MainFrame::AddRDP(Options_HashMap local_options, int info_uniq_name)
{
#ifdef __WXMSW__
   ConnSplitter * conn_splitter = new ConnSplitter(this,local_options,nb,nb->GetId() ,
						   wxPoint(220,10),wxSize(770,640),WS_VISIBLE ,
						   wxString("Test",4));
#endif

#ifdef __WXGTK__
   ConnSplitter * conn_splitter = new ConnSplitter(this,local_options,nb,nb->GetId(),
						   wxPoint(220,10), wxSize(770,640), 0, wxT("Test"));
   conn_splitter->init();
   if (conn_splitter->check_conn() == false)
   {
      conn_splitter->Destroy();
      return;
   }
#endif

   if (conn_splitter == NULL)
      return;

   info_uniq_name = wxAtoi(local_options[wxT("uniq_name")]);
   nb->AddPage(conn_splitter,_("Please wait"));
#ifdef __WXMSW__
   conn_splitter->init();
#endif

   if (!conn_splitter->check_conn()) {
      on_closepage_ne();
      return;
   }

   BasicConnection * conn = conn_splitter->conn;
   if (info_uniq_name > 0) {
      conn->info_uniq_name = info_uniq_name;
      TREEPANEL(conn->main_frame->m_panel_tree)->rdptree->from_wxrdp(conn->info_uniq_name,
								     TREEDATA_INC_OBJCOUNT);
   }

   if (bFocusNewPage)
      nb->SetSelection(nb->GetPageCount() - 1 );

   if (!conn->DoConnection()) {
      if (bFocusNewPage)
	 nb->SetSelection(0);

      const int pc = nb->GetPageCount();
      if (pc > 0)
	 nb->DeletePage(pc - 1);
      if (nb->GetPageCount() > 0) {
	 EnableConnectionMenu();
	 CheckCurrentConnectionMenu();
      } else {
	 DisableConnectionMenu();
	 SetStatusText(_("No connections"),1);
      }
   } else {
#ifdef __WXGTK__
      const int pc = nb->GetPageCount();
      if (pc > 0)
      {
         const wxString connection_name = local_options[wxT("connection_name")];
         const wxString hostname        = local_options[wxT("hostname")];
         if (connection_name != wxT(""))
            nb->SetPageText(pc - 1, connection_name);
         else
            nb->SetPageText(pc - 1, hostname);
      }
      EnableConnectionMenu();
      CheckCurrentConnectionMenu();
#endif
#ifdef __WXMSW__
      if(local_options[wxT("proto")] == wxT("rfb"))
      {
         const int pc = nb->GetPageCount();
         if (pc > 0) {
            const wxString connection_name = local_options[wxT("connection_name")];
            const wxString hostname        = local_options[wxT("hostname")];
            if (connection_name != wxT(""))
               nb->SetPageText(pc - 1, connection_name);
            else
               nb->SetPageText(pc - 1, hostname);
         }
      }
#endif
   }

   FocusCurrentPage();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process configure event
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_config(wxCommandEvent& event)
{
   Options_HashMap local_options;

   RDPDialog dialog(this);
   const int iRes = dialog.ShowModal();

   if (RDPDLG_RET_CANCEL == iRes) return;

   local_options = dialog.GetOptions();

   if (RDPDLG_RET_SAVE == iRes) {
      TREEPANEL(m_panel_tree)->rdptree->ReloadSettings();
      return;
   }

   AddRDP(local_options);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process double click on element
///////////////////////////////////////////////////////////////////////////////
void HideFrame::on_double_left_click(wxTaskBarIconEvent& event)
{
   if (!main_frame->bIconized)
   {
      wxCommandEvent evt;
      evt.SetId(1); // WTF?? magic number
      main_frame->settingsMenu->Check(ID_HIDEFRAME,true);
      main_frame->on_hideframe(evt);
      return;
   }
   else
   {
      wxCommandEvent evt;
      evt.SetId(1); // WTF?? magic number
      on_menu_restore(evt);
   }
   return;
}




MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style):
   wxFrame((wxFrame *)NULL, -1, title, pos, size, style),
   tree(NULL),
   FrameOK(FALSE)
{
   bMaximizeEvent = FALSE;
   bNeedHideFrame = FALSE;
   all_connection_records.Clear();

   GETBASEPATH();
   wxString temp = BASEPATH;

   Benc bc;
   if(!bc.Load(&base))
   {
      wxMessageBox(_("Error read from base"),_("Closing application..."),wxICON_ERROR);
      Close();
   }
   all_connection_records = bc.Load();

   programsettings ps = load_main_settings();
#ifdef TS_NORMAL_VERSION
   bShowFrame = ps.bShowFrame;
#else
   bShowFrame = TRUE;
#endif
   bFocusNewPage = ps.bFocusPage;
   bUseHideFrame = ps.bIcon;
   if (!bUseHideFrame)
   {
      bShowFrame = TRUE;
   }

   bUseCrypt = ps.bUseCrypt;

   m_hideframe = NULL;

   wxIcon icon_main;
   //icon_main.CopyFromBitmap(LOAD_XPM(utilities_terminal32));
   icon_main.CopyFromBitmap(wxGetBitmapFromMemory(rdcm_png));
   //SetIcon(icon_main);

   file_menu = new wxMenu;
   file_menu->Append(ID_NEWCONNECTION,_("&New connection...\tCtrl-N"),
		     _("Create new connection"));
   file_menu->Append(ID_FASTCONN,
		     _("&Fast connection...\tCtrl-F"),
		     _("Fast connection or open saved connection"));
#ifdef __WXMSW__
   file_menu->Append(ID_USERDPFILE,_("&Load RDP File...\tCtrl-O"),
		     _("Use saved connection settings"));
   file_menu->Append(ID_EXPORTRDPFILE,_("&Export RDP File...\tCtrl-E"),
		     _("Export WinRDP connection settings"));
#endif
   file_menu->AppendSeparator();
   file_menu->Append(ID_EXITPROGRAM,
		     _("E&xit\tAlt-X"), _("Quit this program"));

   settingsMenu = new wxMenu;
   settingsMenu->AppendCheckItem(ID_SHOWTREE, _("&Connections tree...\tCtrl-T"),
                                              _("Enable / disable connections tree"));
   settingsMenu->AppendCheckItem(ID_HIDEFRAME, _("&Hide frame...\tCtrl-H"),
                                               _("Hide frame"));
   settingsMenu->Append(ID_SETTINGSDIALOG, _("&Options...\tAlt-O"),
                                           _("Program settings"));

   currentConnMenu = new wxMenu;
   currentConnMenu->Append(ID_CONNECT,_("&Connect...\tAlt-C"),_("Connection with current settings"));
   currentConnMenu->Append(ID_DISCONNECT,_("&Disconnect...\tAlt-D"),_("Disconnect  from server or reconnect"));

   currentConnMenu->Append(ID_FULLSCREEN,_("&Full screen...\tAlt-F"),_("Create full screen RDP window"));

   currentConnMenu->Append(ID_SAVECONNECTED,_("&Save this...\tAlt-S"),_("Save this connection settings"));
   currentConnMenu->Append(ID_SCREENPHOTO,_("&Make screenshot...\tAlt-H"),_("Create screenshot and save this"));

#ifdef __WXGTK__
   currentConnMenu->AppendCheckItem(ID_GRABINPUT,_("&Grab input...\tAlt-G"),_("Grab keyboard input"));
#endif
   currentConnMenu->Append(ID_SENDCAD,_("&Send Ctrl+Alt+Del...\tAlt-Z"),_("Send Ctrl+Alt+Del to remote host"));

   currentConnMenu->Append(ID_CLOSEPAGE,_("&Disconnect and close...\tCtrl-W"),_("Close current page"));

   helpMenu = new wxMenu;
   helpMenu->Append(ID_ABOUTDIALOG,   _("&About...\tF1"),
                                      _("Show about dialog"));
   helpMenu->Append(ID_DOCUMENTATION, _("&Online documentation"),
                                      _("Open documentation from web site"));
   helpMenu->Append(ID_BUILDIN_DOC,   _("&Build-in documentation"),
                                      _("Open documentation"));
   helpMenu->Append(ID_FORUM,         _("&Forum"),
                                      _("Open forum web page"));
   helpMenu->Append(ID_SUPPORT,       _("&Support"),
                                      _("Open support web page"));

   menu_bar = new wxMenuBar();
   menu_bar->Append(file_menu, _("&File"));

   menu_bar->Append(currentConnMenu, _("&Connection"));
   menu_bar->Append(settingsMenu,    _("&Settings")  );
   menu_bar->Append(helpMenu,        _("&Help")      );

   SetMenuBar(menu_bar);
   menu_bar->EnableTop(1,false);

   CreateStatusBar(2); // TODO: WTF?? magic number
   SetStatusText(_("No connections"), 1);

   const int xsize = GetClientSize().x;
   const int ysize = GetClientSize().y;

   m_splitter = new MainSplitter(this, wxID_ANY, wxPoint(0,40),
                                 wxSize(xsize, ysize - 40),wxSP_3D);

   m_panel_nb = new wxPanel(m_splitter, wxID_ANY, wxPoint(0,0),
                            m_splitter->GetClientSize(), wxRESIZE_BORDER);
   nb = new wxAuiNotebook(m_panel_nb,
                          ID_NOTEBOOK,
                          wxPoint(0, 0),
                          m_panel_nb->GetClientSize(),
                          wxAUI_NB_DEFAULT_STYLE);
   nb->SetArtProvider(new wxAuiSimpleTabArt);

   m_panel_tree = new TreePanel(this, m_splitter, wxID_ANY,
                                wxPoint(0, 0),
                                wxSize(0.17 * xsize, ysize - 40),
                                wxSTAY_ON_TOP);
   TREEPANEL(m_panel_tree)->rdptree->LoadSettings();

   IsTree = FALSE;

   m_splitter->Initialize( m_panel_nb);
   m_splitter->SetSashGravity(0.0);   //0.3
   m_splitter->SetSashSize(SPLITTER_SASH_SIZE);


   m_panel_maintoolbar = new wxPanel(this, wxID_ANY, wxPoint(0, 0),
                                     wxSize(GetClientSize().x, 40));
   wxSize toolbar_size = m_panel_maintoolbar->GetClientSize();

   main_auitoolbar_general =
         new wxAuiToolBar(m_panel_maintoolbar,
                          wxID_ANY,
                          wxPoint(0,0),
                          wxSize(toolbar_size.x * 0.18, toolbar_size.y),
                          wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_NO_AUTORESIZE);
   main_auitoolbar_general->SetToolBitmapSize(wxSize(24,24));
   main_auitoolbar_general->AddTool(ID_AUITOOLBAR_EXIT,
                                    _("Exit"),
                                    wxGetBitmapFromMemory(quit_icon_24x24_png),
                                    _("Close program"));
   main_auitoolbar_general->AddSeparator();
   main_auitoolbar_general->AddTool(ID_AUITOOLBAR_NEWCONN,
                                    _("New connection"),
                          wxGetBitmapFromMemory(new_connection_icon_24x24_png),
                                    _("New connection"));
   main_auitoolbar_general->AddTool(ID_AUITOOLBAR_FASTCONN,
                                    _("Fast connection"),
                      wxGetBitmapFromMemory(instant_connection_icon_24x24_png),
                                    _("Fast connection"));

#ifdef __WXMSW__
	main_auitoolbar_general->AddTool(ID_AUITOOLBAR_LOADRDPFILE,
	                                 _("Load RDP file"),
	                             wxGetBitmapFromMemory(load_file_icon_24x24_png),
	                                 _("Load RDP file"));
	main_auitoolbar_general->AddTool(ID_AUITOOLBAR_EXPORTRDPFILE,
	                                 _("Export RDP file"),
	                           wxGetBitmapFromMemory(file_export_icon_24x24_png),
	                                 _("Export RDP file"));

#endif
	main_auitoolbar_general->AddSeparator();
	main_auitoolbar_general->Realize();


	wxPoint pos_toolbar(main_auitoolbar_general->GetSize().GetWidth(),0);

	mainAUIToolbarSettings =
	      new wxAuiToolBar(m_panel_maintoolbar,
	                       wxID_ANY,
	                       pos_toolbar,
	                       wxSize(toolbar_size.x * 0.11, toolbar_size.y),
	                       wxAUI_TB_NO_TOOLTIPS | wxAUI_TB_NO_AUTORESIZE);

	mainAUIToolbarSettings->SetToolBitmapSize(wxSize(24,24));
	mainAUIToolbarSettings->AddTool(ID_AUITOOLBAR_SHOWTREE,
	                      wxGetBitmapFromMemory(open_connection_icon_24x24_png),
	                                  NULL,
	                                  true,
	                                  NULL,
	                                  _("Show / Hide connections tree"));
   mainAUIToolbarSettings->AddTool(ID_AUITOOLBAR_OPTIONS,
                                     _("Options"),
                         wxGetBitmapFromMemory(application_settings_24x24_png),
                                     _("Options"));

	mainAUIToolbarSettings->AddSeparator();
	mainAUIToolbarSettings->Realize();


	wxPoint p(pos_toolbar.x + mainAUIToolbarSettings->GetSize().GetWidth(),0);

	main_auitoolbar_connection = new wxAuiToolBar(m_panel_maintoolbar,-1,p, toolbar_size ,
												  wxAUI_TB_DEFAULT_STYLE  | wxAUI_TB_NO_AUTORESIZE);

	main_auitoolbar_connection->SetToolBitmapSize(wxSize(24,24));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_CONNECT,
										_("Connect"),
										wxGetBitmapFromMemory(establish_connection_24x24_png),
										_("Connect"));

	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_DISCONNECT,
										_("Disconnect"),
										wxGetBitmapFromMemory(terminate_connection_24x24_png),
										_("Disconnect"));

	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_FULLSCREEN,
										_("Full Screen"),
										wxGetBitmapFromMemory(window_fullscreen_icon_24x24_png),
										_("Full Screen"));

	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_SAVECONNECTED,
										_("Save connection"),
										wxGetBitmapFromMemory(save_settings_icon_24x24_png),
										_("Save connection"));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_SCREENPHOTO,
										_("Make screenshot"),
										wxGetBitmapFromMemory(make_bmp_screenshot_icon_24x24_png),
										_("Make screenshot"));
#ifdef __WXGTK__
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_GRABINPUT,
										wxGetBitmapFromMemory(keyboard_input_icon_24x24_png),
										NULL,
										true,
										NULL,
										_("Grab Input"));
#endif
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_SENDCAD,_("Send Ctrl+Alt+Del"),
										wxGetBitmapFromMemory(control_alt_del_icon_24x24_png),
										_("Send Ctrl+Alt+Del"));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_CLOSEPAGE,
										_("Disconnect and close"),
										wxGetBitmapFromMemory(close_session_icon_24x24_png),
										_("Disconnect and close"));
	DisableConnectionMenu();
	main_auitoolbar_connection->Realize();

	if (bUseHideFrame) {
		m_hideframe = new HideFrame(this);
		wxIcon icon;
		//icon.CopyFromBitmap(LOAD_XPM(utilities_terminal));
		icon.CopyFromBitmap(wxGetBitmapFromMemory(rdcm_png));
		m_hideframe->SetIcon(icon, TS_NAME);
	} else {
        mainAUIToolbarSettings->EnableTool(ID_AUITOOLBAR_HIDEFRAME, false);
		settingsMenu->Enable(ID_HIDEFRAME, false);
	}

	FrameOK = TRUE;
}
///////////////////////////////////////////////////////////////////////////////
//! \brief Create connection menu
///////////////////////////////////////////////////////////////////////////////
void MainFrame::EnableConnectionMenu()
{
	main_auitoolbar_connection->Clear();
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_CONNECT,
										_("Connect"),
										wxGetBitmapFromMemory(establish_connection_24x24_png),
										_("Connect"));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_DISCONNECT,
										_("Disconnect"),
										wxGetBitmapFromMemory(terminate_connection_24x24_png),
										_("Disconnect"));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_FULLSCREEN,
										_("Full Screen"),
										wxGetBitmapFromMemory(window_fullscreen_icon_24x24_png),
										_("Full Screen"));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_SAVECONNECTED,
										_("Save connection"),
										wxGetBitmapFromMemory(save_settings_icon_24x24_png),
										_("Save connection"));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_SCREENPHOTO,
										_("Make screenshot"),
										wxGetBitmapFromMemory(make_bmp_screenshot_icon_24x24_png),
										_("Make screenshot"));
#ifdef __WXGTK__
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_GRABINPUT,
										wxGetBitmapFromMemory(keyboard_input_icon_24x24_png),
										NULL,
										true,
										NULL,
										_("Grab Input"));
#endif
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_SENDCAD,wxT("Send Ctrl+Alt+Del"),
										wxGetBitmapFromMemory(control_alt_del_icon_24x24_png),
										_("Send Ctrl+Alt+Del"));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_CLOSEPAGE,
										_("Disconnect and close"),
										wxGetBitmapFromMemory(close_session_icon_24x24_png),
										_("Disconnect and close"));

	main_auitoolbar_connection->Enable(true);
	main_auitoolbar_connection->Refresh();
	main_auitoolbar_connection->Realize();
	menu_bar->EnableTop(1,true);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Remove connetction menu
///////////////////////////////////////////////////////////////////////////////
void MainFrame::DisableConnectionMenu()
{
	main_auitoolbar_connection->Clear();
	main_auitoolbar_connection->Enable(false);
	main_auitoolbar_connection->Refresh();
	main_auitoolbar_connection->Realize();
	menu_bar->EnableTop(1,false);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process connection menu
///////////////////////////////////////////////////////////////////////////////
void MainFrame::CheckCurrentConnectionMenu()
{
	wxString strStatus;
	if (this->nb->GetPageCount() > 0)
	{
		EnableConnectionMenu();
	}
	else
	{
		DisableConnectionMenu();
		this->SetStatusText(_("No connections"),1);
		return;
	}

	ConnSplitter * conn_splitter = (ConnSplitter *)nb->GetPage(nb->GetSelection());
	if (conn_splitter == NULL) return;
	/*wxRDP * */ BasicConnection * conn = conn_splitter->conn;
	if (conn == NULL) return;

	if (conn->bConnected)
	{
		//strStatus = wxString::Format(wxT("%s connected"),conn->rdpconn.hostname.data());
		SetStatusText(strStatus,1);

		currentConnMenu->Enable(ID_CONNECT,false);
		currentConnMenu->Enable(ID_DISCONNECT,true);
		main_auitoolbar_connection->EnableTool(ID_AUITOOLBAR_CONNECT,false);
		main_auitoolbar_connection->EnableTool(ID_AUITOOLBAR_DISCONNECT,true);


		if (conn->bFullScreen)
		{
			currentConnMenu->Enable(ID_FULLSCREEN,false);
			main_auitoolbar_connection->EnableTool(ID_AUITOOLBAR_FULLSCREEN,false);
		}
		else
		{
			currentConnMenu->Enable(ID_FULLSCREEN,true);
			main_auitoolbar_connection->EnableTool(ID_AUITOOLBAR_FULLSCREEN,true);
		}



#ifdef __WXGTK__
		if (conn->bGrab)
		{
			main_auitoolbar_connection->ToggleTool(ID_AUITOOLBAR_GRABINPUT,true);
			currentConnMenu->Check(ID_GRABINPUT,true);

		}
		else
		{
			main_auitoolbar_connection->ToggleTool(ID_AUITOOLBAR_GRABINPUT,false);
			currentConnMenu->Check(ID_GRABINPUT,false);
		}




#endif
		main_auitoolbar_connection->Refresh();

	}
	else
	{
		//strStatus = wxString::Format(wxT("%s disconnected"),conn->rdpconn.hostname.data());
		SetStatusText(strStatus,1);

		currentConnMenu->Enable(ID_CONNECT,true);
		currentConnMenu->Enable(ID_DISCONNECT,false);

		main_auitoolbar_connection->EnableTool(ID_AUITOOLBAR_CONNECT,true);
		main_auitoolbar_connection->EnableTool(ID_AUITOOLBAR_DISCONNECT,false);

		main_auitoolbar_connection->Refresh();
	}


}


///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void MainFrame::enable_auitoolbar_conn(bool state)
{
	this->main_auitoolbar_connection->Enable(state);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process button "Grub all input"
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_auitoolbar_grabinput(wxCommandEvent& event)
{
	wxCommandEvent evt;
	evt.SetId(ID_GRABINPUT);

	if (this->currentConnMenu->IsChecked(ID_GRABINPUT))
	{
		this->currentConnMenu->Check(ID_GRABINPUT,false);
	}
	else
	{
		this->currentConnMenu->Check(ID_GRABINPUT,true);
	}
	this->on_grabinput(evt);

}




void MainFrame::on_auitoolbar_showtree(wxCommandEvent& event) {
	wxCommandEvent evt;
	evt.SetId(ID_SHOWTREE);

	settingsMenu->Check(ID_SHOWTREE, !settingsMenu->IsChecked(ID_SHOWTREE));

	on_showtree(evt);
	mainAUIToolbarSettings->Refresh();
}

void MainFrame::on_auitoolbar_hideframe(wxCommandEvent& event)
{
	wxCommandEvent evt;
	evt.SetId(ID_HIDEFRAME);
	this->settingsMenu->Check(ID_HIDEFRAME,true);
	this->on_hideframe(evt);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process main toolbar
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_auitoolbar(wxCommandEvent& event) {
   // TODO: rewrite in OOP style x_X
   wxCommandEvent evt;
   switch (event.GetId()) {
   case ID_AUITOOLBAR_EXIT:
     evt.SetId(ID_EXITPROGRAM);
     on_quit(evt);
     break;
   case ID_AUITOOLBAR_NEWCONN:
     evt.SetId(ID_NEWCONNECTION);
     on_config(evt);
     break;
   case ID_AUITOOLBAR_FASTCONN:
     evt.SetId(ID_FASTCONN);
     OnFastConn(evt);
     break;
   case ID_AUITOOLBAR_LOADRDPFILE:
     evt.SetId(ID_USERDPFILE);
     on_userdpfile(evt);
     break;
   case ID_AUITOOLBAR_EXPORTRDPFILE:
     evt.SetId(ID_EXPORTRDPFILE);
     on_exportrdpfile(evt);
     break;
   case ID_AUITOOLBAR_CONNECT:
     evt.SetId(ID_CONNECT);
     on_connect(evt);
     break;
   case ID_AUITOOLBAR_DISCONNECT:
     evt.SetId(ID_DISCONNECT);
     on_disconnect(evt);
     break;
   case ID_AUITOOLBAR_FULLSCREEN:
     evt.SetId(ID_FULLSCREEN);
     on_fullscreen(evt);
     break;
   case ID_AUITOOLBAR_CLOSEPAGE:
     evt.SetId(ID_CLOSEPAGE);
     on_closepage(evt);
     break;
   case ID_AUITOOLBAR_OPTIONS:
     evt.SetId(ID_SETTINGSDIALOG);
     on_settings_dialog(evt);
     break;
   case ID_AUITOOLBAR_ABOUT:
     evt.SetId(ID_ABOUTDIALOG);
     on_about(evt);
     break;
   case ID_AUITOOLBAR_SAVECONNECTED:
     evt.SetId(ID_SAVECONNECTED);
     on_saveconnection(evt);
     break;
   case ID_AUITOOLBAR_SENDCAD:
     evt.SetId(ID_SENDCAD);
     on_sendcad(evt);
     break;
   case ID_AUITOOLBAR_SCREENPHOTO:
     evt.SetId(ID_SCREENPHOTO);
     on_screenphoto(evt);
     break;
	}
}


///////////////////////////////////////////////////////////////////////////////
//! \brief Create munu
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
wxMenu *HideFrame::CreatePopupMenu()
{
	wxMenu *menu = new wxMenu;

	menu->Append(ID_HIDEFRAME_RESTORE, _("&Restore"));
	if (!main_frame->bIconized)
	{
		menu->Enable(ID_HIDEFRAME_RESTORE,false);
	}
	menu->AppendSeparator();

	RDPTree * rdptree = TREEPANEL(main_frame->m_panel_tree)->rdptree;

	wxTreeItemIdValue cookie;
	wxTreeItemId curr = rdptree->GetFirstChild(rdptree->root,cookie);
	int count = 0;
	while (curr.IsOk())
	{
		wxMenu * groupmenu = new wxMenu();
		wxTreeItemIdValue cookie_child;
		wxTreeItemId curr_child = rdptree->GetFirstChild(curr,cookie_child);
		while (curr_child.IsOk())
		{
			groupmenu->Append(ID_TREE_ADDTOEXISTSGROUP + count, rdptree->GetItemText(curr_child));
			count ++;
			curr_child = rdptree->GetNextChild(curr,cookie_child);
		}
		menu->AppendSubMenu(groupmenu,rdptree->GetItemText(curr));
		curr = rdptree->GetNextChild(rdptree->root,cookie);
	}
	menu->AppendSeparator();
	menu->Append(ID_HIDEFRAME_NEW, _("&New"));
	menu->Append(ID_HIDEFRAME_SETTINGS, _("&Settings"));
	menu->AppendSeparator();
	menu->Append(ID_HIDEFRAME_EXIT, _("&Exit"));
	return menu;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process connect event from menu
///////////////////////////////////////////////////////////////////////////////
void HideFrame::on_menu_connect(wxCommandEvent &event)
{
	if (event.GetId() >= 7000)
	{
		RDPTree * rdptree = TREEPANEL(main_frame->m_panel_tree)->rdptree;
		wxTreeItemIdValue cookie;
		wxTreeItemId curr = rdptree->GetFirstChild(rdptree->root,cookie);
		int count = 0;
		while (curr.IsOk())
		{
			wxTreeItemIdValue cookie_child;
			wxTreeItemId curr_child = rdptree->GetFirstChild(curr,cookie_child);
			while (curr_child.IsOk())
			{
				if (event.GetId() == (ID_TREE_ADDTOEXISTSGROUP + count))
				{
					RDPTreeData * rdptreedata = (RDPTreeData *)rdptree->GetItemData(curr_child);
					if (!rdptreedata) return;
					rdptree->curr_uniq_name = rdptreedata->uniq_name;

					wxCommandEvent evt;
					evt.SetId(1);
					rdptree->on_tree_connect(evt);

					return;
				}
				count ++;
				curr_child = rdptree->GetNextChild(curr,cookie_child);
			}
			curr = rdptree->GetNextChild(rdptree->root,cookie);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process new element event from menu
///////////////////////////////////////////////////////////////////////////////
void HideFrame::on_menu_new(wxCommandEvent& event)
{
	if (main_frame->bIconized)
	{
		wxCommandEvent taskbar_evt;
		taskbar_evt.SetId(1);
		on_menu_restore(taskbar_evt);

	}
	wxCommandEvent evt;
	evt.SetId(1);
	main_frame->on_config(evt);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process settings event from menu
///////////////////////////////////////////////////////////////////////////////
void HideFrame::on_menu_settings(wxCommandEvent& event)
{
	if (main_frame->bIconized)
	{
		wxCommandEvent taskbar_evt;
		taskbar_evt.SetId(1);
		on_menu_restore(taskbar_evt);

	}
	wxCommandEvent evt;
	evt.SetId(1);
	main_frame->on_settings_dialog(evt);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process exit event from menu
///////////////////////////////////////////////////////////////////////////////
void HideFrame::on_menu_exit(wxCommandEvent &event)
{
	wxCommandEvent evt;
	evt.SetId(1);
	main_frame->on_quit(evt);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process restore event from menu
///////////////////////////////////////////////////////////////////////////////
void HideFrame::on_menu_restore(wxCommandEvent &event)
{
#ifdef __WXMSW__
	if (!main_frame->IsShown())
	{
		main_frame->Show(true);
	}
#endif
	main_frame->Restore();
	main_frame->bIconized = FALSE;
#ifdef __WXMSW__
	main_frame->SendSizeEvent();
#endif
#ifdef __WXGTK__
	main_frame->Iconize(false);
#endif

   main_frame->settingsMenu->Check(ID_HIDEFRAME,false);
   main_frame->mainAUIToolbarSettings->ToggleTool(ID_AUITOOLBAR_HIDEFRAME,false);

	wxUpdateUIEvent evt;
	evt.Check(true);
	evt.SetId(ID_FOCUSPAGE);
	wxPostEvent(main_frame,evt);
	return;

}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process Focus event from menu
///////////////////////////////////////////////////////////////////////////////
void MainFrame::FocusCurrentPageEvt(wxUpdateUIEvent& event)
{
	int iPageCount = (DWORD)nb->GetPageCount();
	if (iPageCount <= 0)
	{
		return;
	}

	nb->SetFocus();

	ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
	/*wxRDP * */ BasicConnection * conn = conn_splitter->conn;

	conn_splitter->SetFocus();
	if (conn)
	{
		conn->SetFocus();
	}

}

void MainFrame::FocusCurrentPage()
{
	wxUpdateUIEvent evt;
	evt.Check(true);
	evt.SetId(ID_FOCUSPAGE);
	wxPostEvent(this,evt);
}


///////////////////////////////////////////////////////////////////////////////
//! \brief Process change page in ain notebook element event from menu
///////////////////////////////////////////////////////////////////////////////
void MainFrame::notebook_change_func(wxAuiNotebookEvent& event)
{
	notebook_change_func_ne();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process grub event from menu
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_grabinput(wxCommandEvent& event)
{
#ifdef __WXGTK__

	ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
//	wxRDP * rdp = conn_splitter->rdp;
	BasicConnection * conn = conn_splitter->conn;

	conn->GrabAll(!conn->bGrab);
	if (currentConnMenu->IsChecked(ID_GRABINPUT))
	{
		currentConnMenu->Check(ID_GRABINPUT,true);
		this->main_auitoolbar_connection->ToggleTool(ID_AUITOOLBAR_GRABINPUT,true);
		this->main_auitoolbar_connection->Refresh();
	}
	else
	{
		currentConnMenu->Check(ID_GRABINPUT,false);
		this->main_auitoolbar_connection->ToggleTool(ID_AUITOOLBAR_GRABINPUT,false);
		this->main_auitoolbar_connection->Refresh();
	}

#endif

}


void MainFrame::on_sendcad(wxCommandEvent& event)
{
	FocusCurrentPage();
	ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
	/*wxRDP * */ BasicConnection * conn = conn_splitter->conn;

	conn->SendKey(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process screenshot event from menu
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_screenphoto(wxCommandEvent& event)
{
	ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
	/*wxRDP * */ BasicConnection * conn = conn_splitter->conn;

	conn->photo();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process amin switch element
///////////////////////////////////////////////////////////////////////////////
void MainFrame::switch_current(BOOL_L bDisconnect)
{
   int lc = nb->GetPageCount();
   if (lc <= 0) return;

   ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
   /*wxRDP * */ BasicConnection * conn = conn_splitter->conn;

   if (bDisconnect)
   {
      if (conn->bConnected)
      {
         wxCommandEvent evt;
         evt.SetId(1);
         on_disconnect(evt);
         bNeedConnect = TRUE;
      } else {
         bNeedConnect = FALSE;
         return;
      }
   } else {
      if (conn->bConnected) {
         return;
      } else {
         wxCommandEvent evt;
         evt.SetId(1);
         on_connect(evt);
         bNeedConnect = FALSE;
      }
   }
}

void MainFrame::notebook_change_func_ne()
{

	this->nb->SetFocus();

	ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
	BasicConnection * conn = conn_splitter->conn;
	if (conn && conn_splitter->screenshot)
	{
		if (!conn_splitter->m_timer_screenshot.IsRunning())
		{
			conn_splitter->m_timer_screenshot.Start(20000);
		}

	conn_splitter->SetFocus();
	conn->SetFocus();
	}

	if (this->nb->GetPageCount() > 0)
	{
		EnableConnectionMenu();
		CheckCurrentConnectionMenu();
	}
	else
	{
		DisableConnectionMenu();
		this->SetStatusText(_("No connections"),1);
	}
	//FUNC_AND_LINE;

//	std::cout <<
//	nb->GetPage(nb->GetSelection())->GetScreenPosition().x <<
//	std::endl;
//	std::cout << (wxAuiNotebookPage)(nb->GetPage(nb->GetSelection()))->rect.GetTopLeft().x << std::endl;
//	std::cout << (wxAuiNotebookPage)(nb->GetPage(nb->GetSelection()))->rect.GetTopLeft().y << std::endl;
}

void MainFrame::on_saveconnection(wxCommandEvent& event)
{
	ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
	/*wxRDP * */ BasicConnection * conn = conn_splitter->conn;
	BOOL_L flag;
	//RDPConn rdpc = conn->rdpconn;
	Options_HashMap local_options = conn->Get_Options();

	if (wxAtoi(local_options[wxT("uniq_name")]) != 0)
	{
		flag = FALSE;
	}
	else
	{
		flag = TRUE;
	}
	//RDPDialog * rdpdialog = new RDPDialog(this,wxCAPTION,,NULL,&base,flag);
	//rdpdialog->LoadRDPConn();
	std::auto_ptr<RDPDialog> rdpDialog(new RDPDialog(this));
	rdpDialog->SetOptions(&local_options);
	const int iRes = rdpDialog->ShowModal();
	if (RDPDLG_RET_SAVE == iRes)
		TREEPANEL(this->m_panel_tree)->rdptree->ReloadSettings();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process connect event
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_connect(wxCommandEvent &event)
{
   ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
   /*wxRDP * */ BasicConnection * conn = conn_splitter->conn;
#ifdef __WXMSW__
   conn->Connect();

#endif
#ifdef __WXGTK__
   if (conn != NULL)
   {
//		if (!conn->DoRdp())
      if (!conn->Connect())
      {
	 this->nb->DeletePage(this->nb->GetSelection());
	 this->nb->SetSelection(0);
	 if (this->nb->GetPageCount() > 0) menu_bar->EnableTop(1,true);
	 else menu_bar->EnableTop(1,false);
      }
      else
      {
	 this->notebook_change_func_ne();
	 this->switch_splitter();
      }
   }

#endif
   wxCommandEvent eventCustom(ID_FULLSCREEN_ADD_TO_MENU);
   wxPostEvent(this, eventCustom);
}


///////////////////////////////////////////////////////////////////////////////
//! \brief Process diaconnetc event
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_disconnect(wxCommandEvent &event)
{
	ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
	/*wxRDP * */ BasicConnection * conn = conn_splitter->conn;
wxCommandEvent eventCustom(ID_FULLSCREEN);
		wxPostEvent(this, eventCustom);
	conn_splitter->create_screenshot();
	conn->Disconnect();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process fullscreen event
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_fullscreen(wxCommandEvent &event)
{
   ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
   if (conn_splitter) {
      /*wxRDP * */ BasicConnection * conn = conn_splitter->conn;

      if (!conn->bConnected) return;
      else
      {
	 conn->FullScreen(!conn->bFullScreen);
      }
   }
}
///////////////////////////////////////////////////////////////////////////////
//! \brief Process clousing page event
///////////////////////////////////////////////////////////////////////////////
void MainFrame::closepage_num_func(wxUpdateUIEvent &event)
{
	int num = current_page_for_delete;

	ConnSplitter * conn_splitter = (ConnSplitter *)nb->GetPage(num);
	if (conn_splitter != NULL)
	{
		BasicConnection * conn = conn_splitter->conn;

		if (conn->info_uniq_name != 0)
		{
			TREEPANEL(conn->main_frame->m_panel_tree)->rdptree->from_wxrdp(conn->info_uniq_name,TREEDATA_DEC_OBJCOUNT);
			conn->info_uniq_name = 0;
		}
#ifdef __WXMSW__
		if (conn->bConnected)
		{
			conn->Disconnect();
		}

#endif
		this->nb->DeletePage(num);
		if (num != 0) 	nb->SetSelection(num - 1);

		if (this->nb->GetPageCount() > 0)
		{
			EnableConnectionMenu();
			CheckCurrentConnectionMenu();
		}
		else
		{
			DisableConnectionMenu();
			this->SetStatusText(_("No connections"),1);
		}
	}
	FocusCurrentPage();
}

///////////////////////////////////////////////////////////////////////////////
//! \sa on_closepage_ne
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_closepage(wxCommandEvent &event)
{
	on_closepage_ne();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Close page from notebook
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_closepage_ne()
{
	ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
	BasicConnection * conn = conn_splitter->conn;
	int iPos = this->nb->GetSelection();

	if (conn->info_uniq_name != 0)
	{

		TREEPANEL(conn->main_frame->m_panel_tree)->rdptree->from_wxrdp(conn->info_uniq_name,
																	   TREEDATA_DEC_OBJCOUNT);

		conn->info_uniq_name = 0;
	}

	if (conn->bConnected)
	{

		conn->DisconnectClose();

	}


#ifdef __WXGTK__
	if (!conn->bConnected)
	{
#endif

		this->nb->DeletePage(iPos);

		if (this->nb->GetPageCount() > 0)
		{

			EnableConnectionMenu();

			CheckCurrentConnectionMenu();

		}
		else
		{

			DisableConnectionMenu();

			this->SetStatusText(_("No connections"),1);
		}

#ifdef __WXGTK__
	}
#endif
}

#ifdef __WXMSW__
///////////////////////////////////////////////////////////////////////////////
//! \brief Process RDP files
///////////////////////////////////////////////////////////////////////////////
Options_HashMap ParseString(WCHAR wstr[], Options_HashMap local_options)
{
	WCHAR *begin;
	WCHAR buff[2048];
	memset(buff,0,sizeof(buff));
	wcscpy(buff,wstr);
	WCHAR temp[2048];
	memset(temp,0,sizeof(temp));
	int mode;

	// Check for full screen
	if ((begin = wcswcs(buff,L"screen mode id:i:")) != NULL)
	{
		begin += wcslen(L"screen mode id:i:");
		int mode = _wtoi(begin);
		if (mode == 2) local_options[wxT("full_screen")] = wxT("1");
		else local_options[wxT("full_screen")] = wxT("0");
		return local_options;
	}

	// Check for desktop width
	if ((begin = wcswcs(buff,L"desktopwidth:i:")) != NULL)
	{
		begin += wcslen(L"desktopwidth:i:");
		int mode = _wtoi(begin);
		local_options[wxT("width")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}
	// Check for desktop height
	if ((begin = wcswcs(buff,L"desktopheight:i:")) != NULL)
	{
		begin += wcslen(L"desktopheight:i:");
		int mode = _wtoi(begin);
		local_options[wxT("heigth")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}
	// Check for color depth
	if ((begin = wcswcs(buff,L"session bpp:i:")) != NULL)
	{
		begin += wcslen(L"session bpp:i:");
		int mode = _wtoi(begin);
		local_options[wxT("color_depth")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}
	// Check for full address
	if ((begin = wcswcs(buff,L"full address:s:")) != NULL)
	{
		begin += wcslen(L"full address:s:");
		wcscpy(temp,begin);
		wxString str = wxString(temp);
		local_options[wxT("hostname")] = str;
		return local_options;
	}
	// Check for keyboard mode
	if ((begin = wcswcs(buff,L"keyboardhook:i:")) != NULL)
	{
		begin += wcslen(L"keyboardhook:i:");
		mode = _wtoi(begin);
		local_options[wxT("keyboard")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}
	// Check for audio mode
	if ((begin = wcswcs(buff,L"audiomode:i:")) != NULL)
	{
		begin += wcslen(L"audiomode:i:");
		mode = _wtoi(begin);
		local_options[wxT("sound_type")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}
	// Check for redirect drives
	if ((begin = wcswcs(buff,L"redirectdrives:i:")) != NULL)
	{
		begin += wcslen(L"redirectdrives:i:");
		mode = _wtoi(begin);
		local_options[wxT("share_drives")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}
	// Check for redirect printers
	if ((begin = wcswcs(buff,L"redirectprinters:i:")) != NULL)
	{
		begin += wcslen(L"redirectprinters:i:");
		mode = _wtoi(begin);
		local_options[wxT("share_printers")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}
	// Check for redirect COM ports
	if ((begin = wcswcs(buff,L"redirectcomports:i:")) != NULL)
	{
		begin += wcslen(L"redirectcomports:i:");
		mode = _wtoi(begin);
		local_options[wxT("share_com_ports")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}
	// Check for redirect smart cards
	if ((begin = wcswcs(buff,L"redirectsmartcards:i:")) != NULL)
	{
		begin += wcslen(L"redirectsmartcards:i:");
		mode = _wtoi(begin);
		local_options[wxT("share_smart_cards")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}
	// Check for username
	if ((begin = wcswcs(buff,L"username:s:")) != NULL)
	{
		begin += wcslen(L"username:s:");
		memset(temp,0,sizeof(temp));
		wcscpy(temp,begin);
		wxString *strUsername = new wxString(temp);
		local_options[wxT("username")] = *strUsername;
		return local_options;
	}
	// Check for domain
	if ((begin = wcswcs(buff,L"domain:s:")) != NULL)
	{
		begin += wcslen(L"domain:s:");
		memset(temp,0,sizeof(temp));
		wcscpy(temp,begin);
		wxString *str = new wxString(temp);
		local_options[wxT("domain")] = *str;
		return local_options;
	}


	// Check for program path
	if ((begin = wcswcs(buff,L"alternate shell:s:")) != NULL)
	{
		begin += wcslen(L"alternate shell:s:");
		memset(temp,0,sizeof(temp));
		wcscpy(temp,begin);

		if (wcslen(temp) == 0) local_options[wxT("use_program")] = wxT("0");
		else
		{
			local_options[wxT("use_program")] = wxT("1");
			local_options[wxT("program_maximized")] = wxT("0");
			wxString str = wxString(temp);
			local_options[wxT("shell")] = str;
		}
		return local_options;
	}

	// Check for program directory
	if ((begin = wcswcs(buff,L"shell working directory:s:")) != NULL)
	{
		begin += wcslen(L"shell working directory:s:");
		memset(temp,0,sizeof(temp));
		wcscpy(temp,begin);
		wxString str = wxString(temp);
		local_options[wxT("directory")] = str;
		return local_options;
	}

	// Check for wallpaper
	if ((begin = wcswcs(buff,L"disable wallpaper:i:")) != NULL)
	{
		begin += wcslen(L"disable wallpaper:i:");
		mode = _wtoi(begin);
		local_options[wxT("enable_wallpaper")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}

	// Check for full window drag
	if ((begin = wcswcs(buff,L"disable full window drag:i:")) != NULL)
	{
		begin += wcslen(L"disable full window drag:i:");
		mode = _wtoi(begin);
		local_options[wxT("enable_full_window_drag")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}

	// Check for animation
	if ((begin = wcswcs(buff,L"disable menu anims:i:")) != NULL)
	{
		begin += wcslen(L"disable menu anims:i:");
		mode = _wtoi(begin);
		local_options[wxT("enable_animation")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}

	// Check for themes
	if ((begin = wcswcs(buff,L"disable themes:i:")) != NULL)
	{
		begin += wcslen(L"disable themes:i:");
		mode = _wtoi(begin);
		local_options[wxT("enable_themes")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}

	// Check for bitmap caching
	if ((begin = wcswcs(buff,L"bitmapcachepersistenable:i:")) != NULL)
	{
		begin += wcslen(L"bitmapcachepersistenable:i:");
		mode = _wtoi(begin);
		local_options[wxT("enable_bitmap_caching")] = wxString::Format(wxT("%i"), mode);
		return local_options;
	}

	// Check for password
	if ((begin = wcswcs(buff,L"password 51:b:")) != NULL)
	{
		begin += wcslen(L"password 51:b:");
		memset(temp,0,sizeof(temp));
		wcscpy(temp,begin);

		BYTE RDPPassw[4096];
		memset(RDPPassw,0,4096);
		CHAR Digits[]="0123456789ABCDEF";
		int count = 0;
		for (int i = 0; i < 1328; i+=2)
		{
			UINT c1,c2;
			for (int j = 0; j < 16; j++)
			{
				if (Digits[j] == temp[i])
					c1 = j;
				if (Digits[j] == temp[i+1])
					c2 = j;
			}
			RDPPassw[count ++] = 16*c1 + c2;
		}

		DATA_BLOB dbIn;
		DATA_BLOB dbOut;
		dbIn.cbData = 1328;
		dbIn.pbData = RDPPassw;
		dbOut.pbData = NULL;
		dbOut.cbData = 0;
		if (!CryptUnprotectData(&dbIn,NULL,0,0,0,CRYPTPROTECT_UI_FORBIDDEN,&dbOut)) return local_options;
		memset(temp,0,sizeof(temp));
		memcpy(temp,dbOut.pbData,dbOut.cbData);
		wxString str = wxString(temp);
		local_options[wxT("password")] = str;
		return local_options;
	}

	return local_options;
}
#endif

///////////////////////////////////////////////////////////////////////////////
//! \brief Process userdpfile event
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_userdpfile(wxCommandEvent& event)
{
#ifdef __WXMSW__
	wxString str;
	Options_HashMap local_options_rdp_files;

	std::auto_ptr<wxFileDialog> openfiledialog(new wxFileDialog(this,
																_("Choose a RDP file"),
																wxT(""),
																wxT(""),
																wxT("*.rdp"),
																wxFD_OPEN));
	int iRes;
	iRes = openfiledialog->ShowModal();
	if (iRes == wxID_CANCEL) return;

	CHAR Path[MAX_PATH];
	strcpy(Path,openfiledialog->GetPath().c_str());

	HANDLE hFile;
	hFile = CreateFileA(Path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,3,
						FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)  {
		wxMessageBox(_("Error rdp file"),_("Error"),wxICON_ERROR);
		return;
	}

	WCHAR wBuff[4096];
	memset(wBuff,0,sizeof(wBuff));
	DWORD dwFileSize = GetFileSize(hFile,NULL);
	SetFilePointer(hFile,0,0,0);
	DWORD dwTemp;
	if (!ReadFile(hFile,wBuff,dwFileSize,&dwTemp,NULL)) {

		CloseHandle(hFile);

		wchar_t separators[1] = {L'\r'};
		wchar_t * token;
		WCHAR wstr[2048];
		memset(wstr,0,2048 * sizeof(WCHAR));
		token = wcstok(wBuff,separators);
		while (token != NULL)
		{
			wcscpy(wstr,token);
			local_options_rdp_files = ParseString(wstr,local_options_rdp_files);
			token = wcstok(NULL,separators);
		}
		local_options_rdp_files[wxT("port")] = wxT("3389");
		local_options_rdp_files[wxT("control_size")] = wxT("0");
		std::auto_ptr<RDPDialog> dialog(new RDPDialog(this));
		dialog->SetOptions(&local_options_rdp_files);
		AddRDP(local_options_rdp_files);
		return;
	}
	wxMessageBox(_("Error rdp file"),_("Error"),wxICON_ERROR);
	return;
#endif
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process export to rdp file event
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_exportrdpfile(wxCommandEvent& event)
{

#ifdef __WXMSW__
	wxString str;
	//RDPConn rdpconn;
	Options_HashMap local_options;

	//this->clear_rdpconn(&rdpconn);

	std::auto_ptr<wxFileDialog> openfiledialog(new wxFileDialog(this,
																_("Choose a RDP file"),
																wxT(""),
																wxT(""),
																wxT("*.rdp"),
																wxFD_OPEN));
	int iRes;
	iRes = openfiledialog->ShowModal();
	if (iRes == wxID_CANCEL) return;

	CHAR Path[MAX_PATH];
	strcpy(Path,openfiledialog->GetPath().c_str());
	HANDLE hFile;
	hFile = CreateFileA(Path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,3,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		wxMessageBox(_("Error rdp file"),_("Error"),wxICON_ERROR);
		return;
	}

	WCHAR wBuff[4096];
	memset(wBuff,0,sizeof(wBuff));
	DWORD dwFileSize = GetFileSize(hFile,NULL);
	SetFilePointer(hFile,0,0,0);
	DWORD dwTemp;
	if (!ReadFile(hFile,wBuff,dwFileSize,&dwTemp,NULL))
	{
		wxMessageBox(_("Error rdp file"),_("Error"),wxICON_ERROR);
		return;
	}

	CloseHandle(hFile);

	wchar_t separators[1] = {L'\r'};
	wchar_t * token;
	WCHAR wstr[2048];
	memset(wstr,0,2048 * sizeof(WCHAR));
	token = wcstok(wBuff,separators);
	while (token != NULL)
	{
		wcscpy(wstr,token);
		local_options = ParseString(wstr,local_options);
		token = wcstok(NULL,separators);
	}

	local_options[wxT("port")] = wxString(wxT("3389"));
	local_options[wxT("control_size")] = wxT("0");

	RDPDialog dialog(this);
	dialog.SetOptions(&local_options);
	iRes = dialog.ShowModal();

	TREEPANEL(m_panel_tree)->rdptree->ReloadSettings();

	if (iRes == 1)
	{
		AddRDP(local_options);
	}
#endif
	return;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process fast configuretion dialog event
///////////////////////////////////////////////////////////////////////////////
void MainFrame::OnFastConn(wxCommandEvent &event) {
	FastConnDialog fcDialog(this, wxCAPTION, NULL, BaseFile, base);

	if (FASTCONN_RET_CANCEL == fcDialog.ShowModal())
		return;

	const Options_HashMap localOptions = fcDialog.GetOptions();
	AddRDP(localOptions);
}

void MainFrame::clear_rdpconn(RDPConn *rdp_conn)
{
	if (rdp_conn != NULL)
	{
		rdp_conn->hostname.Empty();
		rdp_conn->username.Empty();
		rdp_conn->password.Empty();
		rdp_conn->domain.Empty();
		rdp_conn->port = wxT("3389");
		rdp_conn->attach_to_console = FALSE;
		rdp_conn->width = 600;
		rdp_conn->heigth = 800;
		rdp_conn->color_depth = 16;

		rdp_conn->bSmartSizing = FALSE;
		rdp_conn->bControlSize = TRUE;
		rdp_conn->bFullScreen = FALSE;
		rdp_conn->force_update_screen = FALSE;

		//Program settings
		rdp_conn->shell.Empty();
		rdp_conn->directory.Empty();
		rdp_conn->bUseProgram = FALSE;
		rdp_conn->bProgramMaximized = TRUE;

		// Share settings
		rdp_conn->keyboard_map.Empty();
		rdp_conn->keyboard = 0;
		rdp_conn->bShareDrives = FALSE;
		rdp_conn->bSharePrinters  = FALSE;
		rdp_conn->bShareComPorts = FALSE;
		rdp_conn->bShareSmartCards = FALSE;
		rdp_conn->SoundType = 0;

		// Performance settings

		rdp_conn->bEnableBitmapCaching = TRUE;
		rdp_conn->bEnableWallpaper = FALSE;
		rdp_conn->bEnableFullWindowDrag = FALSE;
		rdp_conn->bEnableAnimation = FALSE;
		rdp_conn->bEnableThemes = FALSE;
		rdp_conn->bandwidth = 0;


		rdp_conn->backing_store = FALSE;
		rdp_conn->encription_enable_french = FALSE;
		rdp_conn->encription_enable_new = TRUE;
		rdp_conn->use_rdp_version = 5;
		rdp_conn->send_mouse_event = TRUE;
		rdp_conn->private_color_map = FALSE;
		rdp_conn->single_mode = 0;
		rdp_conn->numlock_sync = FALSE;
		rdp_conn->enable_compres = FALSE;

		rdp_conn->connection_name.Empty();
		rdp_conn->group_name.Empty();
		rdp_conn->dwConnectionCount = 0;

		rdp_conn->redirect_devices_nix.Empty();
	}

}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process note book change event
///////////////////////////////////////////////////////////////////////////////
void MainFrame::notebook_changing_func(wxAuiNotebookEvent& event)
{
	if (nb->GetSelection() >= 0)
	{
		ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
		if (conn_splitter)
		{
			if (conn_splitter->state)
			{
				conn_splitter->create_screenshot();
			}
			if (conn_splitter->m_timer_screenshot.IsRunning())
			{
				conn_splitter->m_timer_screenshot.Stop();
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
//! \sa notebook_closepage_func_ne
///////////////////////////////////////////////////////////////////////////////
void MainFrame::notebook_closepage_func(wxAuiNotebookEvent& event)
{
	if (event.GetId() != wxEVT_COMMAND_AUINOTEBOOK_TAB_MIDDLE_UP)
		notebook_closepage_func_ne();
}

void MainFrame::notebook_middle_press_on_tab_func(wxAuiNotebookEvent& event)
{
	event.StopPropagation();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process close page event
///////////////////////////////////////////////////////////////////////////////
void MainFrame::notebook_closepage_func_ne()
{
	ConnSplitter * conn_splitter = (ConnSplitter *)nb->GetPage(nb->GetSelection());
	BasicConnection * conn = conn_splitter->conn;


	if (conn->info_uniq_name != 0)
	{
		TREEPANEL(conn->main_frame->m_panel_tree)->rdptree->from_wxrdp(conn->info_uniq_name,
																	   TREEDATA_DEC_OBJCOUNT);
		conn->info_uniq_name = 0;
	}
	if (conn->bConnected)
	{

		conn->DisconnectClose();

	}

	if (this->nb->GetPageCount() > 1)
	{
		EnableConnectionMenu();
		CheckCurrentConnectionMenu();
	}
	else
	{
		DisableConnectionMenu();
		this->SetStatusText(_("No connections"),1);
	}

}
///////////////////////////////////////////////////////////////////////////////
//! \brief Show or hide connections tree
///////////////////////////////////////////////////////////////////////////////
void MainFrame::ShowTree(BOOL_L bShow)
{
	wxSize sz = this->GetClientSize();
	int xsize = sz.x;
	int ysize = sz.y;

	if (bShow)
	{
		this->m_panel_tree->Show(true);

		if (m_splitter->IsSplit())  m_splitter->Unsplit();
		m_splitter->SplitVertically(m_panel_tree, m_panel_nb,0.17 * xsize);


		m_panel_nb->SetPosition(wxPoint(0.17 * xsize,40));
#ifdef __WXMSW__
		m_panel_nb->SetSize(wxSize(0.83 * xsize , ysize - 40) );
#endif
#ifdef __WXGTK__
		m_panel_nb->SetSize(wxSize(0.83 * xsize - SPLITTER_SASH_SIZE, ysize - 40) );
#endif

		nb->SetSize(m_panel_nb->GetClientSize());

#ifdef __WXGTK__

		nb->FitInside();
		ConnSplitter * temp;
		int Count = (DWORD)this->nb->GetPageCount();
		for (int i = 0; i < Count; i ++)
		{
			temp = (ConnSplitter *)this->nb->GetPage(i);
			//	wxRDP * rdp = temp->rdp;
			BasicConnection * conn = temp->conn;

			conn->CenterOnParent();
			conn->cnt->SetPosition(wxPoint(0,0));
			temp->CenterOnParent();
			temp->Update();
		}
#endif
	}
	else
	{
		m_splitter->Unsplit(m_panel_tree);
		this->m_panel_tree->Show(false);

		m_panel_nb->SetPosition(wxPoint(0,0));
		m_panel_nb->SetSize(this->m_splitter->GetClientSize());
		nb->SetSize(m_panel_nb->GetClientSize());

#ifdef __WXGTK__

		nb->FitInside();
		ConnSplitter * temp;
		int Count = (DWORD)this->nb->GetPageCount();
		for (int i = 0; i < Count; i ++)
		{
			temp = (ConnSplitter *)this->nb->GetPage(i);
//			wxRDP * rdp = temp->rdp;
			BasicConnection * conn = temp->conn;

			conn->CenterOnParent();
			conn->cnt->CenterOnParent();

			temp->CenterOnParent();
			temp->Update();
		}

#endif

	}

	m_panel_tree->Update();
	m_panel_nb->Refresh();
	m_panel_nb->Update();

	wxUpdateUIEvent evt_enable;
	evt_enable.Check(true);
	evt_enable.SetId(ID_ENABLE_TABS);
	wxPostEvent(this,evt_enable);

}


///////////////////////////////////////////////////////////////////////////////
//! \brief Process showing tree event
///////////////////////////////////////////////////////////////////////////////
void MainFrame::on_showtree(wxCommandEvent &event)
{
	if (this->settingsMenu->IsChecked(ID_SHOWTREE))
	{
		this->settingsMenu->Check(ID_SHOWTREE,true);
		this->mainAUIToolbarSettings->ToggleTool(ID_AUITOOLBAR_SHOWTREE,true);
		this->mainAUIToolbarSettings->Refresh();

		EnableTabs(FALSE);
		this->IsTree = TRUE;
		ShowTree(IsTree);
		return;
	}
	else
	{
		this->settingsMenu->Check(ID_SHOWTREE,false);
		this->mainAUIToolbarSettings->ToggleTool(ID_AUITOOLBAR_SHOWTREE,false);
		this->mainAUIToolbarSettings->Refresh();
		EnableTabs(FALSE);
		this->IsTree = FALSE;
		ShowTree(IsTree);
	}

}

MainFrame::~MainFrame()
{
	Benc bc;
	//bc.Save(&base);
	bc.Save(&all_connection_records);
	delete m_hideframe;
	delete nb;
}

void MainFrame::notebookClosePageEvent(wxCommandEvent& event)
{
	notebook_closepage_func_ne();
	on_closepage_ne();
}


void MainFrame::on_iconize(wxIconizeEvent& event)
{
	if (event.Iconized())
	{
		bIconized = TRUE;
	}

	if (this->bUseHideFrame)
	{
		if (!this->bShowFrame)
		{
			this->Show(false);
		}
	}


}

void MainFrame::on_hideframe(wxCommandEvent& event)
{
	if (this->settingsMenu->IsChecked(ID_HIDEFRAME))
	{
		this->settingsMenu->Check(ID_HIDEFRAME,true);
		this->mainAUIToolbarSettings->ToggleTool(ID_AUITOOLBAR_HIDEFRAME,false);

		this->Iconize(true);

	}
	else
	{

	}
}

void MainFrame::on_maximize(wxMaximizeEvent& event)
{
// TEST (ICA Connection)
#ifdef __WXGTK__
	size_t conn_num = nb->GetPageCount();
	for (size_t i = 0; i < conn_num; i++)
	{
		ConnSplitter * splitter = (ConnSplitter *)nb->GetPage(i);
		if (!splitter) continue;
		BasicConnection * basic_conn = (BasicConnection *)splitter->conn;
		if (!basic_conn) continue;
		basic_conn->CentreConnection();
	}
#endif



	event.Skip();
	return;
}

void MainFrame::EnableTabsEvt(wxUpdateUIEvent& event)
{
	EnableTabs(TRUE);
}

void MainFrame::DisableTabsEvt(wxUpdateUIEvent& event)
{
	EnableTabs(FALSE);
}

void MainFrame::EnableTabs(BOOL_L bEnable)
{
#ifdef __WXMSW__
	if (!FrameOK) return;
	size_t iCount = nb->GetPageCount();
	for (size_t i = 0; i < iCount; i++)
	{
		ConnSplitter * sp_rdp = (ConnSplitter *)nb->GetPage(i);
		if (!bEnable)
			sp_rdp->conn->cnt->Disable();
		else
			sp_rdp->conn->cnt->Enable();
	}

	if (!IsShown())
	{
		Show(true);
	}
#endif
}

void MainFrame::RedrawAll(wxUpdateUIEvent& event)
{
	if (!FrameOK) return;


	wxWindowUpdateLocker win(this);
	// Resize main objects
	int xsize = this->GetClientSize().x;
	int ysize = this->GetClientSize().y;

	m_panel_maintoolbar->SetSize(wxSize(xsize, 40));
	m_panel_maintoolbar->SetPosition(wxPoint(0,0));

	m_splitter->SetSize(wxSize(xsize, ysize - 40));
	m_splitter->SetPosition(wxPoint(0,40));

	wxSize split_size = m_splitter->GetClientSize();
	int split_x = split_size.x;
	int split_y = split_size.y;
	m_panel_nb->SetPosition(wxPoint(0,0));
	m_panel_nb->SetSize(this->m_splitter->GetClientSize());
#ifdef __WXGTK__
	nb->FitInside();
#endif
	m_splitter->SetSashPosition(0.17 * split_x);
	m_panel_nb->SetPosition(wxPoint(0.17 * split_x,0));
#ifdef __WXMSW__
	m_panel_nb->SetSize(wxSize(0.83 * split_x , split_y) );
#endif
#ifdef __WXGTK__
	m_panel_nb->SetSize(wxSize(0.83 * split_x - SPLITTER_SASH_SIZE , split_y) );
#endif
#ifdef __WXGTK__
	m_panel_nb->FitInside();
#endif
	nb->SetSize(m_panel_nb->GetClientSize());

	if (IsTree)
	{
		this->m_panel_tree->Show(true);
	}
	else
	{
		this->m_panel_tree->Show(false);

		m_panel_nb->SetPosition(wxPoint(0,0));
		m_panel_nb->SetSize(this->m_splitter->GetClientSize());
		nb->SetSize(m_panel_nb->GetClientSize());
	}

	// Resize all splitters
	wxUpdateUIEvent evt;
	evt.Check(true);
	evt.SetId(ID_REDRAW_TABS);
	RedrawTabs(evt);

	wxUpdateUIEvent evt_enable;
	evt_enable.Check(true);
	evt_enable.SetId(ID_ENABLE_TABS);
	wxPostEvent(this,evt_enable);
}

void MainFrame::RedrawTabs(wxUpdateUIEvent& event)
{
#ifdef __WXGTK__
	wxWindowUpdateLocker nb_lock(nb);
#endif

	ConnSplitter * temp;

	int Count = (DWORD)this->nb->GetPageCount();
	for (int i = 0; i < Count; i ++)
	{
		temp = (ConnSplitter *)this->nb->GetPage(i);

		temp->conn->SetPosition(wxPoint(0,0));
		//temp->conn->SetSize(temp->GetClientSize());
		//temp->conn->SetSize(this->nb->GetSize());
		//std::cout << "GETCLIENT SIZE w="<< this->nb->GetPage(i)->GetClientSize().GetWidth() << " h=" << temp->GetSize().GetHeight() << std::endl;
		//std::cout << "NBSIZE w="<< this->nb->GetSize().GetWidth() << " h=" << this->GetSize().GetHeight() << std::endl;

#ifdef __WXGTK__
		if (temp->conn->cnt)
		{
			temp->conn->CentreConnection();
		}
#endif

		temp->screenshot->SetPosition(wxPoint(0,0));
		temp->screenshot->SetSize(temp->GetClientSize());
		temp->screenshot->bmpbtn->CenterOnParent();

		temp->screenshot->bmpbtn->SetPosition(wxPoint(0,0));
		temp->screenshot->bmpbtn->SetSize(temp->screenshot->GetClientSize());

		temp->Refresh();
		temp->Update();

	}

	m_panel_nb->Refresh();
	m_panel_nb->Update();
}

void MainFrame::on_sizing(wxSizeEvent& event)
{
	EnableTabs(FALSE);
	Update();
	wxUpdateUIEvent evt;
	evt.Check(true);
	evt.SetId(ID_REDRAW_ALL);
	RedrawAll(evt);
	event.Skip();
	return;
}

void MainFrame::on_settings_dialog(wxCommandEvent &event)
{
	SettingsDialog sd(this,this,wxCAPTION);
	sd.ShowModal();
}

#ifdef __WXGTK__
void MainFrame::connectionStatusUpdate(wxCommandEvent& event)
{
	if (nb->GetPageCount() <= 0) return;

	ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
	if (conn_splitter != NULL)
	{
		notebook_change_func_ne();
#ifdef __WXGTK__
		switch_splitter();
#endif
		/*
		wxRDP * rdp = splitter_rdp->rdp;
		//BasicConnection * conn = splitter_rdp->rdp;

		if (rdp !=  NULL)
		{
			if (rdp->get_proc_state() == false)
			{

			}
			else
			{
				notebook_change_func_ne();
#ifdef __WXGTK__
				switch_splitter();
#endif
			}
		}
		*/
	}
}


void MainFrame::switch_splitter()
{
	ConnSplitter * conn_splitter = (ConnSplitter *)this->nb->GetPage(nb->GetSelection());
//	wxRDP * rdp = conn_splitter->rdp;
	if (conn_splitter->conn && conn_splitter->conn->bConnected == true)
		conn_splitter->switch_to_off();
	else
		conn_splitter->switch_to_on();
}

void MainFrame::error_message(wxString error_mgs)
{
	wxMessageBox(error_mgs,_("Error"),wxICON_ERROR);
}

#endif

///////////////////////////////////////////////////////////////////////////////
//! \brief Add connections record
///////////////////////////////////////////////////////////////////////////////
void MainFrame::AddConnectionRecord(Options_HashMap *local_options)
{
   Benc bc;
   Options_HashMap options;
   bool found_copy = false;
   //wxMessageBox((*local_options)[wxT("uniq_name")]);
   if ((*local_options)[wxT("hostname")] != wxT(""))
   {
      if ( (*local_options)[wxT("uniq_name")].IsEmpty() != true)
      {
         for (int i = 0; i < (int)all_connection_records.Count(); i++) {
            options = all_connection_records.Item(i);
            if (options[wxT("uniq_name")] == (*local_options)[wxT("uniq_name")]) {
					all_connection_records.RemoveAt(i);
					all_connection_records.Insert(*local_options,i);
					bc.Save(&all_connection_records);
					found_copy = true;
					break;
				}
	 }
	 if (found_copy == false)
	 {
				all_connection_records.Add(*local_options);
				bc.Save(&all_connection_records);
			}
		} else {
			(*local_options)[wxT("uniq_name")] = wxString::Format(wxT("%d"), bc.generate_uniq_name());
			all_connection_records.Add(*local_options);
			bc.Save(&all_connection_records);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
//! \brief Delete connections record by uniq name
///////////////////////////////////////////////////////////////////////////////
void MainFrame::Del_Connections_Record(int uniq_name)
{
   Benc bc;
   Options_HashMap options;
//   if ( (*local_options)[wxT("uniq_name")].IsEmpty() != true)
//    {
   for (int i = 0; i < (int)all_connection_records.Count(); i++)
       {
	  options = all_connection_records.Item(i);
	  if (uniq_name == wxAtoi(options[wxT("uniq_name")]))
	  {
  	       all_connection_records.RemoveAt(i);
	       bc.Save(&all_connection_records);
	  //all_connection_records.Insert(*local_options,i);
  	       break;
	  }
  	 }
//    }
    // else
    // {
    //    (*local_options)[wxT("uniq_name")] = wxString::Format(wxT("%d"),
    // 							     bc.generate_uniq_name());
    //    all_connection_records.Add(*local_options);
    //    bc.Save(&all_connection_records);
    // }
}

void MainFrame::ParseUpdateFile(wxCommandEvent& event)
{
//	FUNC_AND_LINE;
	wxXmlNode *rootNode;
	wxString caption, message;
	int major = 0, minor = 0;
	wxStandardPaths sp;
	std::auto_ptr<wxXmlDocument> xmlFile(new wxXmlDocument());
	const wxString updateXmlFile(wxString::Format(wxT("%s/rdcm_update.xml"), sp.GetTempDir().data()));
	if(!xmlFile->Load(updateXmlFile, wxT("UTF-8"))) //FIXME:
		return ;
	rootNode = xmlFile->GetRoot();
	if (rootNode->GetName() == wxT("rdcm"))
	{
		wxXmlNode *child = xmlFile->GetRoot()->GetChildren();
		while (child) {
			if (child->GetName() == wxT("version"))
			{
				wxXmlNode *child_vers = child->GetChildren();
				while (child_vers)
				{
					if (child_vers->GetName() == wxT("major"))
					{
						major = wxAtoi(child_vers->GetNodeContent());

					} else if (child_vers->GetName() == wxT("minor")) {
						minor = wxAtoi(child_vers->GetNodeContent());
					}
					child_vers = child_vers->GetNext();
				}
				delete (child_vers);
			} else if (child->GetName() == wxT("caption")) {
				caption = child->GetNodeContent();
			} else if (child->GetName() == wxT("message")) {
				message = child->GetNodeContent();
			}
			child = child->GetNext();
		}
		delete (child);
	}

	if (caption.Len() > 0 && message.Len() > 0)
		if (major > TS_VERSION_MAJOR ||
		   (major == TS_VERSION_MAJOR && minor > TS_VERSION_MINOR)) {
			wxDialog dlg(this, wxID_ANY, caption);
			wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
			HtmlMessageViewer *html = new HtmlMessageViewer(&dlg, -1, wxDefaultPosition,
		                                                  wxSize(380, 160), wxHW_SCROLLBAR_NEVER);
			html->SetPage(message);
			html->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWFRAME));
			sizer->Add(html, 1, wxALL, 10);
			sizer->Add(new wxButton(&dlg, wxID_OK, _("Ok")),
		                        0, wxALL | wxALIGN_RIGHT, 15);
			dlg.SetAutoLayout(true);
			dlg.SetSizer(sizer);
			sizer->Fit(&dlg);
			dlg.Centre();
			dlg.ShowModal();
		}
	wxRemoveFile(updateXmlFile);
}

void MainFrame::OpenWebPage(wxCommandEvent &event) {
   switch(event.GetId()) {
    case ID_DOCUMENTATION: wxLaunchDefaultBrowser(DOCUMENTATION_URL); break;
    case ID_FORUM:         wxLaunchDefaultBrowser(FORUM_URL);         break;
    case ID_SUPPORT:       wxLaunchDefaultBrowser(SUPPORT_URL);       break;
  }
}

void MainFrame::OnBuildinDoc(wxCommandEvent& event) {
   std::auto_ptr<HelpBrowser> helpDlg(new HelpBrowser(this));
   helpDlg->ShowModal();
}

HtmlMessageViewer::HtmlMessageViewer(wxWindow *parent,
                                     wxWindowID id,
                                     const wxPoint& pos,
                                     const wxSize& size,
                                     long style,
                                     const wxString& name)
: wxHtmlWindow(parent, id, pos, size, style, name)
{
}

void HtmlMessageViewer::OnLinkClicked(const wxHtmlLinkInfo& link)
{
	if (link.GetHref().StartsWith(wxT("http://")))
		wxLaunchDefaultBrowser(link.GetHref());
	else
		wxHtmlWindow::OnLinkClicked(link);
}
