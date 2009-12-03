///////////////////////////////////////////////////////////////////////////////
// File name: main_window.cpp
// Date create: Tue Jul 21 22:04:48 2009
// Version: 0.0
// Time-stamp: "2009-11-20 12:51:10"
// E-mail: 
// Content-Type: text/plain; charset=utf8
// $Id$ 
// Description: 
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "main_window.hpp"



#ifdef __WXMSW__
MSTSCLib::IMsRdpClient* pInterfaceRDP;
MSTSCLib::IMsTscAxEvents * pEvtRdp;
#include <wincrypt.h>
#pragma comment (lib,"crypt32.lib")
#endif


BOOL bTestScreen = TRUE;

MainSplitter::MainSplitter(Main_Frame * parent, wxWindowID id, const wxPoint& pos,const wxSize& size,long style, const wxString& name)
:wxSplitterWindow(parent,-1,pos,size,wxSP_3D)
{
	
}



#include "res/main.xpm"

void Main_Frame::on_about(wxCommandEvent& event)
{
	wxAboutDialogInfo about;
	about.SetVersion(TS_VERSION);
	about.SetName(TS_NAME);
	about.SetDescription(TS_DESCRIPTION);
	about.SetCopyright(TS_COPYRIGHT);
	wxIcon icon;
	icon.CopyFromBitmap(LOAD_XPM(utilities_terminal32));
	about.SetIcon(icon);
	wxAboutBox(about);
}

void Main_Frame::on_quit(wxCommandEvent& event)
{
	wxWindowUpdateLocker frame_lock(this);
	
    int iCount = (int)this->nb->GetPageCount();
	wxCommandEvent evt;
	evt.SetId(0);
/*	for (int i = 0; i < iCount; i ++)
	{
		this->nb->SetSelection(i);
		this->on_closepage(evt);
	}
*/
	for (int i = 0; i < iCount; i++)
	{
		wxSplitterRDP * splitter_rdp;
		splitter_rdp = (wxSplitterRDP *)nb->GetPage(i);
		if (!splitter_rdp) continue;

		wxRDP * rdp = splitter_rdp->rdp;
		if (!rdp) continue;
#ifdef __WXMSW__	
		if (rdp->bConnected)
		{
			rdp->Disconnect(); 
		}

#endif
#ifdef __WXGTK__
		rdp->request_close(FALSE);
#endif
	}
	Close();
}

void Main_Frame::on_closing(wxCloseEvent& event)
{
	Show(false);
	Destroy();
}

void Main_Frame::AddRDP(RDPConn rdpconn, int info_uniq_name)
{
#ifdef __WXGTK__
	// Check for rdesktop
	if (!wxFileExists(wxT("/usr/bin/rdesktop")))
	{
		wxMessageBox(wxT("rdesktop (RDP client for Linux) not found.\nPlease install rdesktop (www.rdesktop.org)"),wxT("Error"),wxICON_ERROR);
		return;
	}

#endif

#ifdef __WXMSW__
	wxSplitterRDP * splitter_rdp = new wxSplitterRDP(this,rdpconn,nb,nb->GetId() ,wxPoint(220,10),wxSize(770,640),WS_VISIBLE ,wxString("Test",4));
#endif

#ifdef __WXGTK__
	wxSplitterRDP * splitter_rdp = new wxSplitterRDP(this,rdpconn,nb,nb->GetId(),wxPoint(220,10),wxSize(770,640),0,wxT("Test"));
	splitter_rdp->init();
	
#endif

	if (splitter_rdp == NULL) 
	{
		return;
	}

	info_uniq_name = rdpconn.uniq_name;
	

	this->nb->AddPage(splitter_rdp,wxT("Please wait"));
#ifdef __WXMSW__

	splitter_rdp->init();
#endif
	wxRDP * rdp = splitter_rdp->rdp; 
	if (info_uniq_name > 0)
	{
		rdp->info_uniq_name = info_uniq_name;
		rdp->main_frame->m_panel_tree->rdptree->from_wxrdp(rdp->info_uniq_name,TREEDATA_INC_OBJCOUNT);
	}
		
	if (this->bFocusNewPage) 
	{
		this->nb->SetSelection(this->nb->GetPageCount() - 1 );
	}
	if (!rdp->DoRdp())
	{
		if (bFocusNewPage)
		{
			this->nb->SetSelection(0);
		}
		int pc = nb->GetPageCount();
		if (pc > 0) nb->DeletePage(pc - 1);
		
		
		if (this->nb->GetPageCount() > 0) 
		{
			EnableConnectionMenu();
			CheckCurrentConnectionMenu();
		}
		else 
		{
			DisableConnectionMenu();
			this->SetStatusText(wxT("No connections"),1);
		}
	}
	else
	{
#ifdef __WXGTK__
		int pc = nb->GetPageCount();
		if (pc > 0)
		{
			if (rdpconn.connection_name != wxT(""))
				nb->SetPageText(pc - 1,rdpconn.connection_name);
			else
				nb->SetPageText(pc - 1,rdpconn.hostname);
		}
		
		EnableConnectionMenu();
		CheckCurrentConnectionMenu();

#endif

	}

	FocusCurrentPage();
}

void Main_Frame::on_config(wxCommandEvent& event)
{
	RDPConn rdpconn;
	clear_rdpconn(&rdpconn);
	
	RDPDialog dialog(this,wxCAPTION,&rdpconn,this->BaseFile,&base,TRUE);
	int iRes;
	iRes = dialog.ShowModal();
			
	if (iRes == 0) return;
	if (iRes == 2)
	{
		m_panel_tree->rdptree->ReloadSettings();
		return;
	}
	
	AddRDP(rdpconn);
}


void HideFrame::on_double_left_click(wxTaskBarIconEvent& event)
{
	if (!main_frame->bIconized)
	{
		wxCommandEvent evt;
		evt.SetId(1);
		main_frame->settingsMenu->Check(ID_HIDEFRAME,true); 
		main_frame->on_hideframe(evt);
		return;
	}
	else
	{
		wxCommandEvent evt;
		evt.SetId(1);
		on_menu_restore(evt);
	}
	return;
}

#include "res/document-save.xpm"
#ifdef __WXMSW__
#include "res/document-open.xpm"
#include "res/edit-select-all.xpm"
#endif
#include "res/view-fullscreen.xpm"
#include "res/folder.xpm"
//#include "res/help-browser.xpm"
#include "res/list-add.xpm"
#include "res/network-offline.xpm"
#include "res/network-transmit-receive.xpm"
#include "res/preferences-desktop.xpm"
#include "res/preferences-system.xpm"
#include "res/process-stop.xpm"
#include "res/system-log-out.xpm"
#include "res/system-search.xpm"
#include "res/utilities-terminal.xpm"
#include "res/camera-photo.xpm"
#ifdef __WXGTK__
#include "res/input-keyboard.xpm"	
#endif
#include "res/go-down.xpm"	


Main_Frame::Main_Frame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame((wxFrame *)NULL, -1, title, pos, size, style),
	tree(NULL),
	FrameOK(FALSE)
{
	bMaximizeEvent = FALSE;
	bNeedHideFrame = FALSE;

	GETBASEPATH();
	wxString temp = BASEPATH;

	Benc bc;
	if(!bc.Load(&base))
	{
		wxMessageBox(wxT("Error read from base"),wxT("Closing application..."),wxICON_ERROR);
		Close();
	}

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
	icon_main.CopyFromBitmap(LOAD_XPM(utilities_terminal32));
	SetIcon(icon_main);
	
	file_menu = new wxMenu;
	file_menu->Append(ID_NEWCONNECTION,wxT("&New connection...\tCtrl-N"),_("Create new connection"));
	file_menu->Append(ID_FASTCONN,wxT("&Fast connection...\tCtrl-F"),_("Fast connection or open saved connection")); 
#ifdef __WXMSW__
	file_menu->Append(ID_USERDPFILE,wxT("&Load RDP File...\tCtrl-O"),_("Use saved connection settings"));
	file_menu->Append(ID_EXPORTRDPFILE,wxT("&Export RDP File...\tCtrl-E"),_("Export WinRDP connection settings"));
#endif
	file_menu->AppendSeparator();
	file_menu->Append(ID_EXITPROGRAM, wxT("E&xit\tAlt-X"),wxT("Quit this program"));

	settingsMenu = new wxMenu;
	settingsMenu->AppendCheckItem(ID_SHOWTREE, wxT("&Connections tree...\tCtrl-T"), _("Enable / disable connections tree")); 
	settingsMenu->AppendCheckItem(ID_HIDEFRAME, wxT("&Hide frame...\tCtrl-H"), _("Hide frame")); 
	settingsMenu->Append(ID_SETTINGSDIALOG,wxT("&Options...\tAlt-O"),_("Program settings"));

	currentConnMenu = new wxMenu;
	currentConnMenu->Append(ID_CONNECT,wxT("&Connect...\tAlt-C"),_("Connection with current settings"));
	currentConnMenu->Append(ID_DISCONNECT,wxT("&Disconnect...\tAlt-D"),_("Disconnect  from server or reconnect"));

	currentConnMenu->Append(ID_FULLSCREEN,wxT("&Full screen...\tAlt-F"),_("Create full screen RDP window"));

	currentConnMenu->Append(ID_SAVECONNECTED,wxT("&Save this...\tAlt-S"),_("Save this connection settings"));
	currentConnMenu->Append(ID_SCREENPHOTO,wxT("&Make screenshot...\tAlt-H"),_("Create screenshot and save this"));

#ifdef __WXGTK__
	currentConnMenu->AppendCheckItem(ID_GRABINPUT,wxT("&Grab input...\tAlt-G"),_("Grab keyboard input"));
#endif	
	currentConnMenu->Append(ID_SENDCAD,wxT("&Send Ctrl+Alt+Del...\tAlt-Z"),_("Send Ctrl+Alt+Del to remote host"));


	
	currentConnMenu->Append(ID_CLOSEPAGE,wxT("&Disconnect and close...\tCtrl-W"),_("Close current page"));

	helpMenu = new wxMenu;
	helpMenu->Append(ID_ABOUTDIALOG, wxT("&About...\tF1"),_("Show about dialog"));
	
    menu_bar = new wxMenuBar();
    menu_bar->Append(file_menu, wxT("&File"));
	
	menu_bar->Append(currentConnMenu, wxT("&Connection"));
	menu_bar->Append(settingsMenu, wxT("&Settings"));
    menu_bar->Append(helpMenu, wxT("&Help"));
		
    SetMenuBar(menu_bar);
	menu_bar->EnableTop(1,false); 

    CreateStatusBar(2);
	SetStatusText(wxT("No connections"),1);
		

	int xsize = this->GetClientSize().x;
	int ysize = this->GetClientSize().y;

	m_splitter = new MainSplitter(this,-1,wxPoint(0,40),wxSize(xsize, ysize - 40),wxSP_3D);

	m_panel_nb = new wxPanel(m_splitter,-1,wxPoint(0,0),m_splitter->GetClientSize(),wxRESIZE_BORDER);
	nb = new wxAuiNotebook(m_panel_nb,ID_NOTEBOOK, wxPoint(0, 0),m_panel_nb->GetClientSize() , wxAUI_NB_DEFAULT_STYLE);
	nb->SetArtProvider(new wxAuiSimpleTabArt);

	m_panel_tree = new TreePanel(this,m_splitter,-1,wxPoint(0,0),wxSize(0.17 * xsize,ysize - 40 ),wxSTAY_ON_TOP  );	
	m_panel_tree->rdptree->LoadSettings();

	IsTree = FALSE;

	m_splitter->Initialize( m_panel_nb);
	m_splitter->SetSashGravity(0.0);   //0.3
	m_splitter->SetSashSize(SPLITTER_SASH_SIZE);


	m_panel_maintoolbar = new wxPanel(this,-1,wxPoint(0,0),wxSize(this->GetClientSize().x, 40));
	wxSize toolbar_size = m_panel_maintoolbar->GetClientSize();

	main_auitoolbar_general = new wxAuiToolBar(m_panel_maintoolbar,-1,/*wxDefaultPosition*/wxPoint(0,0), wxSize(toolbar_size.x* 0.18,toolbar_size.y) ,
                                         wxAUI_TB_DEFAULT_STYLE  | wxAUI_TB_NO_AUTORESIZE);
    main_auitoolbar_general->SetToolBitmapSize(wxSize(24,24));
	main_auitoolbar_general->AddTool(ID_AUITOOLBAR_EXIT,wxT("Exit"),LOAD_XPM(system_log_out),wxT("Close program"));
	main_auitoolbar_general->AddSeparator();
	main_auitoolbar_general->AddTool(ID_AUITOOLBAR_NEWCONN,wxT("New connection"),LOAD_XPM(list_add),wxT("New connection"));
	main_auitoolbar_general->AddTool(ID_AUITOOLBAR_FASTCONN,wxT("Fast connection"),LOAD_XPM(folder),wxT("Fast connection"));

#ifdef __WXMSW__	
	main_auitoolbar_general->AddTool(ID_AUITOOLBAR_LOADRDPFILE,wxT("Load RDP file"),LOAD_XPM(document_open)/*wxArtProvider::GetBitmap(wxART_QUIT, wxART_OTHER, wxSize(16,16))*/,wxT("Load RDP file"));
	main_auitoolbar_general->AddTool(ID_AUITOOLBAR_EXPORTRDPFILE,wxT("Export RDP file"),LOAD_XPM(edit_select_all)/*wxArtProvider::GetBitmap(wxART_QUIT, wxART_OTHER, wxSize(16,16))*/,wxT("Export RDP file"));

#endif	
	main_auitoolbar_general->AddSeparator();
	main_auitoolbar_general->Realize();

		
	wxPoint pos_toolbar(main_auitoolbar_general->GetSize().GetWidth(),0); 

	main_auitoolbar_settings = new wxAuiToolBar(m_panel_maintoolbar,-1,pos_toolbar, wxSize(toolbar_size.x* 0.11 ,toolbar_size.y) ,
                                         wxAUI_TB_NO_TOOLTIPS | wxAUI_TB_NO_AUTORESIZE);

	main_auitoolbar_settings->SetToolBitmapSize(wxSize(24,24));
	main_auitoolbar_settings->AddTool(ID_AUITOOLBAR_SHOWTREE, LOAD_XPM(system_search),NULL,true,NULL,wxT("Show / Hide connections tree"));
/*	
	main_auitoolbar_settings->AddTool(ID_AUITOOLBAR_HIDEFRAME,
	LOAD_XPM(preferences_desktop),NULL,true,NULL,wxT("Hide frame"));
*/
	main_auitoolbar_settings->AddTool(ID_AUITOOLBAR_OPTIONS,wxT("Options"),LOAD_XPM(preferences_system),wxT("Options"));
	main_auitoolbar_settings->AddSeparator();
	//main_auitoolbar_settings->AddTool(ID_AUITOOLBAR_ABOUT,wxT("About"),wxBitmap(help_browser,wxBITMAP_TYPE_XPM ),wxT("About"));
	//main_auitoolbar_settings->AddSeparator();
	main_auitoolbar_settings->Realize();

	
	wxPoint p(pos_toolbar.x + main_auitoolbar_settings->GetSize().GetWidth(),0);

	main_auitoolbar_connection = new wxAuiToolBar(m_panel_maintoolbar,-1,p, toolbar_size ,
                                         wxAUI_TB_DEFAULT_STYLE  | wxAUI_TB_NO_AUTORESIZE);
	
	main_auitoolbar_connection->SetToolBitmapSize(wxSize(24,24));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_CONNECT,wxT("Connect"),LOAD_XPM(network_transmit_receive ),wxT("Connect"));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_DISCONNECT,wxT("Disconnect"),LOAD_XPM(network_offline ),wxT("Disconnect"));

	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_FULLSCREEN,wxT("Full Screen"),LOAD_XPM(view_fullscreen ),wxT("Full Screen"));

	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_SAVECONNECTED,wxT("Save connection"),LOAD_XPM(document_save ),wxT("Save connection"));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_SCREENPHOTO,wxT("Make screenshot"),LOAD_XPM(camera_photo ),wxT("Make screenshot"));

#ifdef __WXGTK__
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_GRABINPUT,LOAD_XPM(input_keyboard ),NULL,true,NULL,wxT("Grab Input"));
#endif

	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_SENDCAD,wxT("Send Ctrl+Alt+Del"),LOAD_XPM(go_down ),wxT("Send Ctrl+Alt+Del"));
	

	
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_CLOSEPAGE,wxT("Disconnect and close"),LOAD_XPM(process_stop ),wxT("Disconnect and close"));
	DisableConnectionMenu();
	main_auitoolbar_connection->Realize();
		
	if (bUseHideFrame)
	{	m_hideframe = new HideFrame(this);
		wxIcon icon;
		icon.CopyFromBitmap(LOAD_XPM(utilities_terminal));
		m_hideframe->SetIcon(icon,TS_NAME);
		
	}
	else
	{
		main_auitoolbar_settings->EnableTool(ID_AUITOOLBAR_HIDEFRAME,false);
		settingsMenu->Enable(ID_HIDEFRAME,false);
	}
	
	FrameOK = TRUE;

}

void Main_Frame::EnableConnectionMenu()
{
	main_auitoolbar_connection->Clear();
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_CONNECT,wxT("Connect"),LOAD_XPM(network_transmit_receive ),wxT("Connect"));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_DISCONNECT,wxT("Disconnect"),LOAD_XPM(network_offline ),wxT("Disconnect"));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_FULLSCREEN,wxT("Full Screen"),LOAD_XPM(view_fullscreen ),wxT("Full Screen"));

	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_SAVECONNECTED,wxT("Save connection"),LOAD_XPM(document_save ),wxT("Save connection"));
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_SCREENPHOTO,wxT("Make screenshot"),LOAD_XPM(camera_photo ),wxT("Make screenshot"));

#ifdef __WXGTK__
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_GRABINPUT,LOAD_XPM(input_keyboard ),NULL,true,NULL,wxT("Grab Input"));
#endif	
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_SENDCAD,wxT("Send Ctrl+Alt+Del"),LOAD_XPM(go_down ),wxT("Send Ctrl+Alt+Del"));

	
	main_auitoolbar_connection->AddTool(ID_AUITOOLBAR_CLOSEPAGE,wxT("Disconnect and close"),LOAD_XPM(process_stop ),wxT("Disconnect and close"));

	main_auitoolbar_connection->Enable(true);
	main_auitoolbar_connection->Refresh();
	main_auitoolbar_connection->Realize();
	menu_bar->EnableTop(1,true);
}

void Main_Frame::DisableConnectionMenu()
{
	main_auitoolbar_connection->Clear();
	main_auitoolbar_connection->Enable(false);
	main_auitoolbar_connection->Refresh();
	main_auitoolbar_connection->Realize();
	menu_bar->EnableTop(1,false);
}

void Main_Frame::CheckCurrentConnectionMenu()
{
	wxString strStatus;
	if (this->nb->GetPageCount() > 0) 
	{
		EnableConnectionMenu();
	}
	else 
	{
		DisableConnectionMenu();
		this->SetStatusText(wxT("No connections"),1);
		return;
	}

	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)nb->GetPage(nb->GetSelection());
	if (splitter_rdp == NULL) return;
	wxRDP * rdp = splitter_rdp->rdp;
	if (rdp == NULL) return;
	
	if (rdp->bConnected)
	{
		strStatus = wxString::Format(_T("%s connected"),rdp->rdpconn.hostname.data()); 
		SetStatusText(strStatus,1); 

		currentConnMenu->Enable(ID_CONNECT,false);
		currentConnMenu->Enable(ID_DISCONNECT,true);
		main_auitoolbar_connection->EnableTool(ID_AUITOOLBAR_CONNECT,false);
		main_auitoolbar_connection->EnableTool(ID_AUITOOLBAR_DISCONNECT,true);


		if (rdp->bFullScreen)
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
		if (rdp->bGrab)
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
		strStatus = wxString::Format(_T("%s disconnected"),rdp->rdpconn.hostname.data()); 
		SetStatusText(strStatus,1); 
			
		currentConnMenu->Enable(ID_CONNECT,true);
		currentConnMenu->Enable(ID_DISCONNECT,false);

		main_auitoolbar_connection->EnableTool(ID_AUITOOLBAR_CONNECT,true);
		main_auitoolbar_connection->EnableTool(ID_AUITOOLBAR_DISCONNECT,false);

		main_auitoolbar_connection->Refresh();
	}


}



void Main_Frame::enable_auitoolbar_conn(bool state)
{
	this->main_auitoolbar_connection->Enable(state);
}


void Main_Frame::on_auitoolbar_grabinput(wxCommandEvent& event)
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




void Main_Frame::on_auitoolbar_showtree(wxCommandEvent& event)
{

	wxCommandEvent evt;
	evt.SetId(ID_SHOWTREE);

	if (this->settingsMenu->IsChecked(ID_SHOWTREE))
	{
		this->settingsMenu->Check(ID_SHOWTREE,false);
	}
	else
	{
		this->settingsMenu->Check(ID_SHOWTREE,true);
	}
	this->on_showtree(evt);
	this->main_auitoolbar_settings->Refresh();
}

void Main_Frame::on_auitoolbar_hideframe(wxCommandEvent& event)
{
	wxCommandEvent evt;
	evt.SetId(ID_HIDEFRAME);
	this->settingsMenu->Check(ID_HIDEFRAME,true);
	this->on_hideframe(evt);
}


void Main_Frame::on_auitoolbar(wxCommandEvent& event)
{
	wxCommandEvent evt;
	switch (event.GetId())
	{
	case ID_AUITOOLBAR_EXIT:
		
		
		evt.SetId(ID_EXITPROGRAM);
		this->on_quit(evt); 
		break;

	case ID_AUITOOLBAR_NEWCONN:
		evt.SetId(ID_NEWCONNECTION);
		this->on_config(evt); 
		break;

	case ID_AUITOOLBAR_FASTCONN:
		evt.SetId(ID_FASTCONN);
		this->on_fastconn(evt); 
		break;

	case ID_AUITOOLBAR_LOADRDPFILE:
		evt.SetId(ID_USERDPFILE);
		this->on_userdpfile(evt); 
		break;

	case ID_AUITOOLBAR_EXPORTRDPFILE:
		evt.SetId(ID_EXPORTRDPFILE);
		this->on_exportrdpfile(evt); 
		break;

	case ID_AUITOOLBAR_CONNECT:
		evt.SetId(ID_CONNECT);
		this->on_connect(evt); 
		break;
	case ID_AUITOOLBAR_DISCONNECT:
		evt.SetId(ID_DISCONNECT);
		this->on_disconnect(evt); 
		break;
	case ID_AUITOOLBAR_FULLSCREEN:
		evt.SetId(ID_FULLSCREEN);
		this->on_fullscreen(evt); 
		break;

	case ID_AUITOOLBAR_CLOSEPAGE:
		evt.SetId(ID_CLOSEPAGE);
		this->on_closepage(evt); 
		break;

	case ID_AUITOOLBAR_OPTIONS:
		evt.SetId(ID_SETTINGSDIALOG);
		this->on_settings_dialog(evt); 
		break;

	case ID_AUITOOLBAR_ABOUT:
		evt.SetId(ID_ABOUTDIALOG);
		this->on_about(evt); 
		break;

	case ID_AUITOOLBAR_SAVECONNECTED:
		evt.SetId(ID_SAVECONNECTED);
		this->on_saveconnection(evt); 
		break;
		
	case ID_AUITOOLBAR_SENDCAD:
		evt.SetId(ID_SENDCAD);
		this->on_sendcad(evt); 
		break;	

	case ID_AUITOOLBAR_SCREENPHOTO:
		evt.SetId(ID_SCREENPHOTO);
		this->on_screenphoto(evt); 
		break;	

	default:

		break;

	}
}



wxMenu *HideFrame::CreatePopupMenu()
{
	wxMenu *menu = new wxMenu;
	
	menu->Append(ID_HIDEFRAME_RESTORE, _T("&Restore"));
	if (!main_frame->bIconized)
	{
		menu->Enable(ID_HIDEFRAME_RESTORE,false);
	}
	menu->AppendSeparator();
	
	RDPTree * rdptree = main_frame->m_panel_tree->rdptree; 

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
	menu->Append(ID_HIDEFRAME_NEW, _T("&New"));
	menu->Append(ID_HIDEFRAME_SETTINGS, _T("&Settings"));
	menu->AppendSeparator();
	menu->Append(ID_HIDEFRAME_EXIT, _T("&Exit"));
	return menu;

}

void HideFrame::on_menu_connect(wxCommandEvent &event) 
{
	if (event.GetId() >= 7000)
	{
		RDPTree * rdptree = main_frame->m_panel_tree->rdptree;
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

void HideFrame::on_menu_exit(wxCommandEvent &event)
{
	wxCommandEvent evt;
	evt.SetId(1); 
	main_frame->on_quit(evt);
}

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
	main_frame->main_auitoolbar_settings->ToggleTool(ID_AUITOOLBAR_HIDEFRAME,false);
	
	wxUpdateUIEvent evt;
	evt.Check(true);
	evt.SetId(ID_FOCUSPAGE);
	wxPostEvent(main_frame,evt);
	return;

}

void Main_Frame::FocusCurrentPageEvt(wxUpdateUIEvent& event)
{
	int iPageCount = (DWORD)nb->GetPageCount();
	if (iPageCount <= 0) 
	{
		return;
	}
	
	nb->SetFocus();
	
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp;

	splitter_rdp->SetFocus(); 
	if (rdp)
	{
		rdp->SetFocus();
	}

}

void Main_Frame::FocusCurrentPage()
{
	wxUpdateUIEvent evt;
	evt.Check(true);
	evt.SetId(ID_FOCUSPAGE);
	wxPostEvent(this,evt);
}


void Main_Frame::notebook_change_func(wxAuiNotebookEvent& event)
{
	notebook_change_func_ne();
}

void Main_Frame::on_grabinput(wxCommandEvent& event)
{
#ifdef __WXGTK__

	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp;

	rdp->GrabAll(!rdp->bGrab);
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

void Main_Frame::on_sendcad(wxCommandEvent& event)
{
	FocusCurrentPage();
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp;

	rdp->SendKey(TRUE);
}

void Main_Frame::on_screenphoto(wxCommandEvent& event)
{	
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp;

	rdp->photo();
}


void Main_Frame::switch_current(BOOL bDisconnect)
{
	int lc = nb->GetPageCount();
	if (lc <= 0) return;

	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp;

	if (bDisconnect)
	{
		if (rdp->bConnected)
		{
			wxCommandEvent evt;
			evt.SetId(1);
			on_disconnect(evt);
			bNeedConnect = TRUE;
			
		}
		else
		{
			bNeedConnect = FALSE;
			return;
		}
		
	}
	else
	{
		if (rdp->bConnected)
		{
			return;
		}
		else
		{
			wxCommandEvent evt;
			evt.SetId(1);
			on_connect(evt);
			bNeedConnect = FALSE;
			
		}
	
	}

	
}



void Main_Frame::notebook_change_func_ne()
{

	this->nb->SetFocus();
	
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp; 
	if (rdp && splitter_rdp->screenshot)
	{
		if (!splitter_rdp->m_timer_screenshot.IsRunning())
		{
			splitter_rdp->m_timer_screenshot.Start(20000);  
		}

	splitter_rdp->SetFocus(); 
	rdp->SetFocus();
	}

	if (this->nb->GetPageCount() > 0) 
	{
		EnableConnectionMenu();
		CheckCurrentConnectionMenu();
	}
	else 
	{
		DisableConnectionMenu();
		this->SetStatusText(wxT("No connections"),1);
	}
	
}

void Main_Frame::on_saveconnection(wxCommandEvent& event)
{
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp;
	BOOL flag;
	RDPConn rdpc = rdp->rdpconn;
	if (rdpc.uniq_name != 0)
	{
		flag = FALSE;
	}
	else
	{
		flag = TRUE;
	}
	RDPDialog * rdpdialog = new RDPDialog(this,wxCAPTION,&rdpc,NULL,&base,flag);
	rdpdialog->LoadRDPConn();
	
	int iRes = rdpdialog->ShowModal();
	if (iRes == 2)
	{
		this->m_panel_tree->rdptree->ReloadSettings();
	}
	delete rdpdialog;
}

void Main_Frame::on_connect(wxCommandEvent &event)
{
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp; 
#ifdef __WXMSW__	
	rdp->Connect();
	
#endif
#ifdef __WXGTK__
	if (rdp != NULL)
	{
		if (!rdp->DoRdp())
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

}

void Main_Frame::on_disconnect(wxCommandEvent &event)
{
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp; 

	splitter_rdp->create_screenshot(); 
	
#ifdef __WXMSW__
	rdp->Disconnect();
	
#endif
#ifdef __WXGTK__
	if (rdp != NULL)
	{
		rdp->request_close();
	}
#endif

}

void Main_Frame::on_fullscreen(wxCommandEvent &event) 
{
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp; 

	if (!rdp->bConnected) return;
	else
	{
		rdp->FullScreen(!rdp->bFullScreen); 
	}
}

void Main_Frame::closepage_num_func(wxUpdateUIEvent &event)
{
	int num = this->current_page_for_delete;
		
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(num);
	wxRDP * rdp = splitter_rdp->rdp; 

	if (rdp->info_uniq_name != 0)
	{
		rdp->main_frame->m_panel_tree->rdptree->from_wxrdp(rdp->info_uniq_name,TREEDATA_DEC_OBJCOUNT);
		rdp->info_uniq_name = 0;
	}	
#ifdef __WXMSW__
	if (rdp->bConnected)
	{
		rdp->Disconnect();
	}

#endif
	this->nb->DeletePage(num);
	if (num != 0) 	this->nb->SetSelection(num - 1);

	if (this->nb->GetPageCount() > 0)
	{
		EnableConnectionMenu();
		CheckCurrentConnectionMenu();
	}
	else 
	{
		DisableConnectionMenu();
		this->SetStatusText(wxT("No connections"),1);
	}

	FocusCurrentPage();
}

void Main_Frame::on_closepage(wxCommandEvent &event)
{
	on_closepage_ne();
}

void Main_Frame::on_closepage_ne()
{
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp;
	int iPos = this->nb->GetSelection();

	if (rdp->info_uniq_name != 0)
	{
		rdp->main_frame->m_panel_tree->rdptree->from_wxrdp(rdp->info_uniq_name,TREEDATA_DEC_OBJCOUNT);
		rdp->info_uniq_name = 0;
	}
	
#ifdef __WXMSW__	
	if (rdp->bConnected)
	{
		rdp->Disconnect(); 
	}

#endif
#ifdef __WXGTK__
	rdp->request_close(TRUE);
#endif


#ifdef __WXGTK__
	if (!rdp->bConnected)
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
		this->SetStatusText(wxT("No connections"),1);
	}

#ifdef __WXGTK__
	}
#endif


}

#ifdef __WXMSW__
RDPConn ParseString(WCHAR wstr[], RDPConn rdpconn)
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
		if (mode == 2) rdpconn.bFullScreen = TRUE;
		else rdpconn.bFullScreen = FALSE;
		return rdpconn;
	}

	// Check for desktop width
	if ((begin = wcswcs(buff,L"desktopwidth:i:")) != NULL)
	{
		begin += wcslen(L"desktopwidth:i:");
		int mode = _wtoi(begin);	
		rdpconn.width = (USHORT)mode;
		return rdpconn;
	}
	// Check for desktop height
	if ((begin = wcswcs(buff,L"desktopheight:i:")) != NULL)
	{
		begin += wcslen(L"desktopheight:i:");
		int mode = _wtoi(begin);	
		rdpconn.heigth = (USHORT)mode;
		return rdpconn;
	}
	// Check for color depth
	if ((begin = wcswcs(buff,L"session bpp:i:")) != NULL)
	{
		begin += wcslen(L"session bpp:i:");
		int mode = _wtoi(begin);	
		rdpconn.color_depth = (BYTE)mode;
		return rdpconn;
	}
	// Check for full address
	if ((begin = wcswcs(buff,L"full address:s:")) != NULL)
	{
		begin += wcslen(L"full address:s:");
		wcscpy(temp,begin);
		wxString str = wxString(temp);
		rdpconn.hostname = str;
		return rdpconn;
	}	
	// Check for keyboard mode
	if ((begin = wcswcs(buff,L"keyboardhook:i:")) != NULL)
	{
		begin += wcslen(L"keyboardhook:i:");
		mode = _wtoi(begin);	
		rdpconn.keyboard = (BYTE)mode;
		return rdpconn;
	}
	// Check for audio mode
	if ((begin = wcswcs(buff,L"audiomode:i:")) != NULL)
	{
		begin += wcslen(L"audiomode:i:");
		mode = _wtoi(begin);	
		rdpconn.SoundType = (BYTE)mode;
		return rdpconn;
	}
	// Check for redirect drives
	if ((begin = wcswcs(buff,L"redirectdrives:i:")) != NULL)
	{
		begin += wcslen(L"redirectdrives:i:");
		mode = _wtoi(begin);
		if (mode == 1) rdpconn.bShareDrives = TRUE;
		else	rdpconn.bShareDrives = FALSE;
		return rdpconn;
	}
	// Check for redirect printers
	if ((begin = wcswcs(buff,L"redirectprinters:i:")) != NULL)
	{
		begin += wcslen(L"redirectprinters:i:");
		mode = _wtoi(begin);	
		if (mode == 1) rdpconn.bSharePrinters = TRUE;
		else	rdpconn.bSharePrinters = FALSE;
		return rdpconn;
	}
	// Check for redirect COM ports
	if ((begin = wcswcs(buff,L"redirectcomports:i:")) != NULL)
	{
		begin += wcslen(L"redirectcomports:i:");
		mode = _wtoi(begin);	
		if (mode == 1) rdpconn.bShareComPorts = TRUE;
		else	rdpconn.bShareComPorts = FALSE;
		return rdpconn;
	}
	// Check for redirect smart cards
	if ((begin = wcswcs(buff,L"redirectsmartcards:i:")) != NULL)
	{
		begin += wcslen(L"redirectsmartcards:i:");
		mode = _wtoi(begin);	
		if (mode == 1) rdpconn.bShareSmartCards = TRUE;
		else	rdpconn.bShareSmartCards = FALSE;
		return rdpconn;
	}
	// Check for username
	if ((begin = wcswcs(buff,L"username:s:")) != NULL)
	{
		begin += wcslen(L"username:s:");
		memset(temp,0,sizeof(temp));
		wcscpy(temp,begin);
		wxString *strUsername = new wxString(temp);
		rdpconn.username = *strUsername;
		return rdpconn;
	}
	// Check for domain
	if ((begin = wcswcs(buff,L"domain:s:")) != NULL)
	{
		begin += wcslen(L"domain:s:");
		memset(temp,0,sizeof(temp));
		wcscpy(temp,begin);
		wxString *str = new wxString(temp);
		rdpconn.domain = *str;
		return rdpconn;
	}


	// Check for program path
	if ((begin = wcswcs(buff,L"alternate shell:s:")) != NULL)
	{
		begin += wcslen(L"alternate shell:s:");
		memset(temp,0,sizeof(temp));
		wcscpy(temp,begin);
		
		if (wcslen(temp) == 0) rdpconn.bUseProgram = FALSE;
		else
		{
			rdpconn.bUseProgram = TRUE;
			rdpconn.bProgramMaximized = FALSE;
			wxString str = wxString(temp);
			rdpconn.shell = str;
		}
		return rdpconn;
	}
	
	// Check for program directory
	if ((begin = wcswcs(buff,L"shell working directory:s:")) != NULL)
	{
		begin += wcslen(L"shell working directory:s:");
		memset(temp,0,sizeof(temp));
		wcscpy(temp,begin);
		wxString str = wxString(temp);
		rdpconn.directory = str;
		return rdpconn;
	}

	// Check for wallpaper
	if ((begin = wcswcs(buff,L"disable wallpaper:i:")) != NULL)
	{
		begin += wcslen(L"disable wallpaper:i:");
		mode = _wtoi(begin);	
		if (mode == 1) rdpconn.bEnableWallpaper = FALSE;
		else	rdpconn.bEnableWallpaper = TRUE;
		return rdpconn;
	}

	// Check for full window drag
	if ((begin = wcswcs(buff,L"disable full window drag:i:")) != NULL)
	{
		begin += wcslen(L"disable full window drag:i:");
		mode = _wtoi(begin);	
		if (mode == 1) rdpconn.bEnableFullWindowDrag = FALSE;
		else	rdpconn.bEnableFullWindowDrag = TRUE;
		return rdpconn;
	}

	// Check for animation
	if ((begin = wcswcs(buff,L"disable menu anims:i:")) != NULL)
	{
		begin += wcslen(L"disable menu anims:i:");
		mode = _wtoi(begin);	
		if (mode == 1) rdpconn.bEnableAnimation = FALSE;
		else	rdpconn.bEnableAnimation = TRUE;
		return rdpconn;
	}

	// Check for themes
	if ((begin = wcswcs(buff,L"disable themes:i:")) != NULL)
	{
		begin += wcslen(L"disable themes:i:");
		mode = _wtoi(begin);	
		if (mode == 1) rdpconn.bEnableThemes = FALSE;
		else	rdpconn.bEnableThemes = TRUE;
		return rdpconn;
	}

	// Check for bitmap caching
	if ((begin = wcswcs(buff,L"bitmapcachepersistenable:i:")) != NULL)
	{
		begin += wcslen(L"bitmapcachepersistenable:i:");
		mode = _wtoi(begin);	
		if (mode == 1) rdpconn.bEnableBitmapCaching = TRUE;
		else	rdpconn.bEnableBitmapCaching = FALSE;
		return rdpconn;
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
		if (!CryptUnprotectData(&dbIn,NULL,0,0,0,CRYPTPROTECT_UI_FORBIDDEN,&dbOut)) return rdpconn;
		memset(temp,0,sizeof(temp));
		memcpy(temp,dbOut.pbData,dbOut.cbData);
		wxString str = wxString(temp);
		rdpconn.password = str;
		return rdpconn;
	}
	
	return rdpconn;
}
#endif

void Main_Frame::on_userdpfile(wxCommandEvent& event)
{
#ifdef __WXMSW__
	wxString str;
	RDPConn rdpconn;

	wxFileDialog * openfiledialog = new wxFileDialog(this,wxT("Choose a RDP file"),wxT(""),wxT(""),wxT("*.rdp"),wxFD_OPEN);
	int iRes;
	iRes = openfiledialog->ShowModal();
	if (iRes == wxID_CANCEL){ delete openfiledialog; return;}
	
	CHAR Path[MAX_PATH];
	strcpy(Path,openfiledialog->GetPath().c_str());

	delete openfiledialog;

	HANDLE hFile;
	hFile = CreateFileA(Path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,3,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)  goto error;

	WCHAR wBuff[4096];
	memset(wBuff,0,sizeof(wBuff));
	DWORD dwFileSize = GetFileSize(hFile,NULL);
	SetFilePointer(hFile,0,0,0);
	DWORD dwTemp;
	if (!ReadFile(hFile,wBuff,dwFileSize,&dwTemp,NULL)) goto error;
	
	CloseHandle(hFile);

	wchar_t separators[1] = {L'\r'};
	wchar_t * token;
	WCHAR wstr[2048];
	memset(wstr,0,2048);
	token = wcstok(wBuff,separators);
	while (token != NULL)
	{
		wcscpy(wstr,token);
		rdpconn = ParseString(wstr,rdpconn);
		token = wcstok(NULL,separators);
	}
	
	rdpconn.port = wxString(_("3389"));
	
	rdpconn.bControlSize = FALSE; 

	RDPDialog * dialog = new RDPDialog(this,wxCAPTION,&rdpconn,this->BaseFile,&base,TRUE);
	dialog->LoadRDPConn();
	dialog->FillRDPConn(); 

	AddRDP(rdpconn);
	delete dialog;
	
	return;
error:
	wxMessageBox(wxT("Error rdp file"),wxT("Error"),wxICON_ERROR);
	return;
#endif
}

void Main_Frame::on_exportrdpfile(wxCommandEvent& event)
{
	
#ifdef __WXMSW__
	wxString str;
	RDPConn rdpconn;
	
	this->clear_rdpconn(&rdpconn);

	wxFileDialog * openfiledialog = new wxFileDialog(this,wxT("Choose a RDP file"),wxT(""),wxT(""),wxT("*.rdp"),wxFD_OPEN);
	int iRes;
	iRes = openfiledialog->ShowModal();
	if (iRes == wxID_CANCEL){ delete openfiledialog; return;}
	
	CHAR Path[MAX_PATH];
	strcpy(Path,openfiledialog->GetPath().c_str());
	delete openfiledialog;
	HANDLE hFile;
	hFile = CreateFileA(Path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,3,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)  
	{
		wxMessageBox(wxT("Error rdp file"),wxT("Error"),wxICON_ERROR);
		return;
	}

	WCHAR wBuff[4096];
	memset(wBuff,0,sizeof(wBuff));
	DWORD dwFileSize = GetFileSize(hFile,NULL);
	SetFilePointer(hFile,0,0,0);
	DWORD dwTemp;
	if (!ReadFile(hFile,wBuff,dwFileSize,&dwTemp,NULL))
	{
		wxMessageBox(wxT("Error rdp file"),wxT("Error"),wxICON_ERROR);
		return;
	}
	
	CloseHandle(hFile);

	wchar_t separators[1] = {L'\r'};
	wchar_t * token;
	WCHAR wstr[2048];
	memset(wstr,0,2048);
	token = wcstok(wBuff,separators);
	while (token != NULL)
	{
		wcscpy(wstr,token);
		rdpconn = ParseString(wstr,rdpconn);
		token = wcstok(NULL,separators);
	}
	
	rdpconn.port = wxString(_("3389"));
	rdpconn.bControlSize = FALSE; 
	
	RDPDialog dialog = RDPDialog(this,wxCAPTION,&rdpconn,this->BaseFile,&base,TRUE);
	dialog.LoadRDPConn();
	iRes = dialog.ShowModal();
		
	m_panel_tree->rdptree->ReloadSettings();

	if (iRes == 1)
	{
		AddRDP(rdpconn);
	}
#endif
	return;
}

void Main_Frame::on_fastconn(wxCommandEvent &event)
{
	FastConnDialog fcDialog(this,wxCAPTION,NULL,this->BaseFile,this->base);
	
	int iRes = fcDialog.ShowModal(); 
	if (iRes == 0) return;

	if(iRes == 1) 
	{
		if (fcDialog.GetRDPConn().uniq_name > 0)
		{	
			this->m_panel_tree->rdptree->curr_uniq_name = fcDialog.GetRDPConn().uniq_name;
			wxCommandEvent evt;
			evt.SetId(1);
			this->m_panel_tree->rdptree->on_tree_connect(evt);
		}
		else
		{
			AddRDP(fcDialog.GetRDPConn());
		}
	}
}

void Main_Frame::clear_rdpconn(RDPConn *rdp_conn)
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

void Main_Frame::notebook_changing_func(wxAuiNotebookEvent& event)
{
	if (nb->GetSelection() >= 0)
	{
		wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
		if (splitter_rdp)
		{
			if (splitter_rdp->state)
			{
				splitter_rdp->create_screenshot();
			}
			if (splitter_rdp->m_timer_screenshot.IsRunning())
			{
				splitter_rdp->m_timer_screenshot.Stop();  
			}
		}
	}
}

void Main_Frame::notebook_closepage_func(wxAuiNotebookEvent& event)
{
	notebook_closepage_func_ne();
}

void Main_Frame::notebook_closepage_func_ne()
{
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp; 

	if (rdp->info_uniq_name != 0)
	{
		rdp->main_frame->m_panel_tree->rdptree->from_wxrdp(rdp->info_uniq_name,TREEDATA_DEC_OBJCOUNT);
		rdp->info_uniq_name = 0;
	}

	if (rdp->bConnected)
	{
#ifdef __WXMSW__
		rdp->Disconnect();
#endif
#ifdef __WXGTK__
		rdp->request_close(TRUE);
#endif
	}
	if (this->nb->GetPageCount() > 1)
	{
		EnableConnectionMenu();
		CheckCurrentConnectionMenu();
	}
	else 
	{
		DisableConnectionMenu();
		this->SetStatusText(wxT("No connections"),1);
	}
	
}

void Main_Frame::ShowTree(BOOL bShow)
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
		wxSplitterRDP * temp;
		int Count = (DWORD)this->nb->GetPageCount();
		for (int i = 0; i < Count; i ++)
		{
			temp = (wxSplitterRDP *)this->nb->GetPage(i);
			wxRDP * rdp = temp->rdp;
				
			rdp->CenterOnParent();
			rdp->cnt->SetPosition(wxPoint(0,0));
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
		wxSplitterRDP * temp;
		int Count = (DWORD)this->nb->GetPageCount();
		for (int i = 0; i < Count; i ++)
		{
			temp = (wxSplitterRDP *)this->nb->GetPage(i);
			wxRDP * rdp = temp->rdp;
			rdp->CenterOnParent();
			rdp->cnt->CenterOnParent();
				
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



void Main_Frame::on_showtree(wxCommandEvent &event) 
{
	if (this->settingsMenu->IsChecked(ID_SHOWTREE))
	{
		this->settingsMenu->Check(ID_SHOWTREE,true);
		this->main_auitoolbar_settings->ToggleTool(ID_AUITOOLBAR_SHOWTREE,true);
		this->main_auitoolbar_settings->Refresh();
		
		EnableTabs(FALSE);
		this->IsTree = TRUE;
		ShowTree(IsTree);
		return;
	}
	else
	{
		this->settingsMenu->Check(ID_SHOWTREE,false);
		this->main_auitoolbar_settings->ToggleTool(ID_AUITOOLBAR_SHOWTREE,false);
		this->main_auitoolbar_settings->Refresh();
		EnableTabs(FALSE);
		this->IsTree = FALSE;
		ShowTree(IsTree);
	}

}

Main_Frame::~Main_Frame()
{
	Benc bc;
	bc.Save(&base);

	delete m_hideframe;
	delete nb;
}

void Main_Frame::notebook_closepage_event(wxCommandEvent& event)
{
	notebook_closepage_func_ne();
	on_closepage_ne();
}


void Main_Frame::on_iconize(wxIconizeEvent& event)
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

void Main_Frame::on_hideframe(wxCommandEvent& event)
{
	if (this->settingsMenu->IsChecked(ID_HIDEFRAME))
	{
		this->settingsMenu->Check(ID_HIDEFRAME,true);
		this->main_auitoolbar_settings->ToggleTool(ID_AUITOOLBAR_HIDEFRAME,false);
		
		this->Iconize(true);
	
	}
	else
	{
		
	}
}

void Main_Frame::on_maximize(wxMaximizeEvent& event)
{
	event.Skip();
	return;
}

void Main_Frame::EnableTabsEvt(wxUpdateUIEvent& event)
{
	EnableTabs(TRUE);
}

void Main_Frame::DisableTabsEvt(wxUpdateUIEvent& event)
{
	EnableTabs(FALSE);
}

void Main_Frame::EnableTabs(BOOL bEnable)
{
#ifdef __WXMSW__
	if (!FrameOK) return;
	size_t iCount = nb->GetPageCount();
	for (size_t i = 0; i < iCount; i++)
	{
		wxSplitterRDP * sp_rdp = (wxSplitterRDP *)nb->GetPage(i);
		if (!bEnable)
			sp_rdp->rdp->cnt->Disable();	
		else
			sp_rdp->rdp->cnt->Enable();
	}	
 
	if (!IsShown())
	{
		Show(true);
	}
#endif
}

void Main_Frame::RedrawAll(wxUpdateUIEvent& event)
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
//	m_splitter->Refresh();
//	m_splitter->Update();

	// Resize main splitter and show or hide tree

	wxSize split_size = m_splitter->GetClientSize();
	int split_x = split_size.x;
	int split_y = split_size.y;
	if (IsTree)
	{
		//m_splitter->Unsplit(m_panel_tree); 
		m_panel_nb->SetPosition(wxPoint(0,0)); 
		m_panel_nb->SetSize(this->m_splitter->GetClientSize()); 
#ifdef __WXGTK__
		nb->FitInside();
#endif		
		//nb->SetSize(m_panel_nb->GetClientSize()); 
		
//		m_panel_nb->Refresh(); 
//		m_panel_nb->Update();
		this->m_panel_tree->Show(true);
		//if (m_splitter->IsSplit())  m_splitter->Unsplit(); 
		//m_splitter->SplitVertically(m_panel_tree, m_panel_nb,0.17 * split_x); 
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
	
	}
	else
	{
		//m_splitter->Unsplit(m_panel_tree); 
		this->m_panel_tree->Show(false); 
		
		m_panel_nb->SetPosition(wxPoint(0,0)); 
		m_panel_nb->SetSize(this->m_splitter->GetClientSize()); 
		nb->SetSize(m_panel_nb->GetClientSize());
		
	}
	
//	m_panel_tree->Update();
//	m_panel_nb->Refresh(); 
//	m_panel_nb->Update();

	// Resize all splitters
	wxUpdateUIEvent evt;
	evt.Check(true);
	evt.SetId(ID_REDRAW_TABS);
	RedrawTabs(evt);
	//wxPostEvent(this,evt);


//	m_panel_tree->Refresh();
//	m_panel_tree->Update();
//	m_splitter->Refresh();
//	m_splitter->Update();

	wxUpdateUIEvent evt_enable;
	evt_enable.Check(true);
	evt_enable.SetId(ID_ENABLE_TABS);
	wxPostEvent(this,evt_enable);
}

void Main_Frame::RedrawTabs(wxUpdateUIEvent& event)
{
#ifdef __WXGTK__
	wxWindowUpdateLocker nb_lock(nb);
#endif
	
	wxSplitterRDP * temp;
	
	int Count = (DWORD)this->nb->GetPageCount();
	for (int i = 0; i < Count; i ++)
	{
		temp = (wxSplitterRDP *)this->nb->GetPage(i);
							
		temp->rdp->SetPosition(wxPoint(0,0));
		temp->rdp->SetSize(temp->GetClientSize());

#ifdef __WXGTK__
		if (temp->rdp->cnt)
		{
			temp->rdp->CentreRDP();
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

void Main_Frame::on_sizing(wxSizeEvent& event)
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

void Main_Frame::on_settings_dialog(wxCommandEvent &event)
{
	SettingsDialog sd(this,this,wxCAPTION);
	sd.ShowModal(); 
}

#ifdef __WXGTK__
void Main_Frame::connection_status_update(wxCommandEvent& event)
{
	if (nb->GetPageCount() <= 0) return;
		
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	if (splitter_rdp != NULL)
	{
		wxRDP * rdp = splitter_rdp->rdp;
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
	}
}


void Main_Frame::switch_splitter()
{
	wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)this->nb->GetPage(nb->GetSelection());
	wxRDP * rdp = splitter_rdp->rdp;
	if (rdp->bConnected == true)
	{
		splitter_rdp->switch_to_off();
	} 
	else 
	{
		splitter_rdp->switch_to_on();
	}
}

void Main_Frame::error_message(wxString error_mgs)
{
	wxMessageBox(error_mgs,wxT("Error"),wxICON_ERROR);
}

#endif
