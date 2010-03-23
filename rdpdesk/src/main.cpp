///////////////////////////////////////////////////////////////////////////////
// File name:   main.cpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-23 13:04:36" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#include "main.hpp"

#include "main_window.hpp"
#include "rdp_dialogs.hpp"
#include "fastconn_dialog.hpp"
#include "settings_dialog.hpp"
#include "tree_group.hpp"
#include "splitter.hpp"
#include "locale_helper.hpp"
#include "small_shoot.hpp"

#ifdef __WXGTK__
#include "RDPConnection_nix.hpp"
#include "ICAConnection_nix.hpp"
#endif
#ifdef __WXMSW__
#include "RDPConnection_win.hpp"
#include "ICAConnection_win.hpp"
#endif

IMPLEMENT_APP(RDP_Core)

ExceptionMessage::ExceptionMessage(bool fatal):
//msg(wxT("Fatal exception. Application will be closed.")),
wxDialog(NULL,-1,wxT(""),wxDefaultPosition,wxDefaultSize,wxCAPTION)
{
	if (fatal)
	{
		msg = wxT("Fatal exception. Application will be closed.");
	}
	else
	{
		msg = wxT("Internal Error. Application will be closed.");
	}

	m_static_msg = NULL;
	m_bitmap_err = NULL;
	m_line_separator = NULL;
	m_button_ok = NULL;	

	sizer_main = new wxBoxSizer(wxVERTICAL);
	sizer_err = new wxBoxSizer(wxHORIZONTAL);
	wxString title = wxString::Format(wxT("%s error"),TS_NAME);
	//m_static_msg = new wxStaticText(this,-1,wxT("Fatal exception. Application will be closed."),wxDefaultPosition,wxDefaultSize);
	m_static_msg = new wxStaticText(this,-1,msg,wxDefaultPosition,wxDefaultSize);
	m_bitmap_err = new wxStaticBitmap(this,-1,wxNullBitmap);
	m_line_separator = new wxStaticLine(this,wxID_ANY,wxDefaultPosition, wxSize(-1,2) ,wxLI_HORIZONTAL);
	//m_button_ok = new wxButton(this,wxID_OK,wxT("OK"),wxDefaultPosition,wxSize(100,-1));

	wxBitmap bmp = wxArtProvider::GetBitmap(wxART_ERROR);
	m_bitmap_err->SetBitmap(bmp);
	SetTitle(title);

	sizer_err->Add(m_bitmap_err,0,wxALL | wxALIGN_CENTER, 5);
	sizer_err->Add(m_static_msg,0,wxALL | wxALIGN_CENTER, 5);

	sizer_main->Add(sizer_err,0,wxALL | wxALIGN_CENTER, 5);
	sizer_main->Add(m_line_separator,0,wxALL | wxALIGN_CENTER | wxEXPAND, 5);
	sizer_main->Add(CreateStdDialogButtonSizer(wxOK),0,wxALL | wxALIGN_CENTER , 5);
	//sizer_main->Add(m_button_ok,0,wxALL | wxALIGN_CENTER , 5);
	
	SetSizer(sizer_main);
	sizer_main->SetSizeHints(this);

	Center();
	ShowModal();
}

ExceptionMessage::~ExceptionMessage()
{
	wxDELETE(m_static_msg);
	wxDELETE(m_bitmap_err);
	wxDELETE(m_line_separator);
	wxDELETE(m_button_ok);	

}

bool RDP_Core::OnInit()
{
	wxHandleFatalExceptions(true); // TEST

	wxLocale locale;

    int frame_width;
	int frame_heigth;
	wxDisplaySize(&frame_width,&frame_heigth);
		
	frame_width *= 0.85;
	frame_heigth *= 0.88;
		
	programsettings ps = load_main_settings();
	LocaleHelper lh;
	lh.SetLang(ps.lang ,&locale);

    main_frame = new Main_Frame(TS_NAME ,wxPoint(50,50), wxSize(frame_width,frame_heigth), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN );
    main_frame->Show(true); 
    return true;  
}

void RDP_Core::OnFatalException()
{
	wxBell();
	ExceptionMessage em;
/*	try
	{
		wxDELETE(main_frame->m_hideframe);
	}
	catch(...)
	{
	}
	*/
	wxApp::Exit(); 
}

void RDP_Core::OnUnhandledException()
{
	wxBell();
	ExceptionMessage em(false);
	wxApp::Exit();
}

bool RDP_Core::OnExceptionInMainLoop()
{
		wxBell();
	ExceptionMessage em(false);
	wxApp::Exit();

	return false;
}





BEGIN_EVENT_TABLE(FastConnDialog, wxDialog)
EVT_BUTTON    (ID_FASTCONN_CONNECT,        FastConnDialog::button_connect_func)
EVT_BUTTON    (ID_FASTCONN_CANCEL,        FastConnDialog::button_cancel_func)
EVT_BUTTON    (ID_FASTCONN_DETAILS,        FastConnDialog::button_details_func)
EVT_COMBOBOX  (ID_COMBOBOX_FAST,    FastConnDialog::combobox_change_func)
EVT_TEXT(ID_COMBOBOX_FAST, FastConnDialog::combobox_text_func)
EVT_CHAR_HOOK(FastConnDialog::dialog_hotkeys)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(RDPTree, wxTreeCtrl)
//EVT_LEFT_DCLICK(RDPTree::on_dleft_click)
EVT_TREE_KEY_DOWN(ID_RDPTREE, RDPTree::on_key_down) 
EVT_TREE_ITEM_MENU(ID_RDPTREE,RDPTree::createmenu)
EVT_TREE_DELETE_ITEM(ID_RDPTREE, RDPTree::on_deleting_item) 
EVT_MENU(ID_TREE_CONNECT,RDPTree::on_tree_connect)
EVT_MENU(ID_TREE_DELETE,RDPTree::on_tree_delete)
EVT_MENU(ID_TREE_CHANGE,RDPTree::on_tree_change)
EVT_MENU(ID_TREE_CLONE,RDPTree::on_tree_clone)
EVT_MENU(ID_TREE_ADDTONEWGROUP,RDPTree::on_add_group)
EVT_MENU(ID_TREE_SETCREDS,RDPTree::on_tree_set_credentials)
EVT_MENU(ID_TREE_NULLCOUNT,RDPTree::on_tree_delete_from_favorites)
EVT_MENU(-1,RDPTree::on_add_group)
EVT_MENU(ID_EVENT_FROM_WXRDP,RDPTree::on_event_from_wxrdp)
EVT_TREE_BEGIN_DRAG(ID_RDPTREE, RDPTree::on_begin_drag)
EVT_TREE_END_DRAG(ID_RDPTREE, RDPTree::on_end_drag)
EVT_TREE_ITEM_ACTIVATED(ID_RDPTREE,RDPTree::on_activated)
EVT_KILL_FOCUS(RDPTree::on_kill_focus)
END_EVENT_TABLE()



BEGIN_EVENT_TABLE(TreePanel, wxPanel)
EVT_SIZE(TreePanel::on_size_change)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(HideFrame, wxTaskBarIcon)
EVT_TASKBAR_LEFT_DCLICK(HideFrame::on_double_left_click)  
EVT_MENU(ID_HIDEFRAME_RESTORE, HideFrame::on_menu_restore) 
EVT_MENU(ID_HIDEFRAME_EXIT, HideFrame::on_menu_exit) 
EVT_MENU(ID_HIDEFRAME_NEW, HideFrame::on_menu_new) 
EVT_MENU(ID_HIDEFRAME_SETTINGS, HideFrame::on_menu_settings) 
EVT_MENU(-1,HideFrame::on_menu_connect)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
EVT_BUTTON    (ID_BUTTON_SD_OK,        SettingsDialog::button_ok_func)
EVT_BUTTON    (ID_BUTTON_SD_CANCEL,    SettingsDialog::button_cancel_func)
EVT_BUTTON    (ID_BUTTON_SD_APPLY,     SettingsDialog::button_apply_func)
EVT_BUTTON    (ID_BUTTON_RDPBASE,     SettingsDialog::button_rdpbase_func)
EVT_BUTTON    (ID_BUTTON_RDPKEY,     SettingsDialog::button_rdpkey_func)
EVT_BUTTON    (ID_BUTTON_RDESKTOP,     SettingsDialog::button_rdesktop_func)
EVT_BUTTON    (ID_BUTTON_KEYMAP,     SettingsDialog::button_keymap_func)
EVT_CHECKBOX  (ID_CHECKBOX_ICON,    SettingsDialog::checkbox_icon_func)
EVT_CHECKBOX  (ID_CHECKBOX_CRYPT,    SettingsDialog::checkbox_crypt_func)
EVT_CHAR_HOOK(SettingsDialog::dialog_hotkeys)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(PanelBitmapBtn, wxPanel)
EVT_BUTTON(ID_CONNECT_FROM_SCREENSHOT, PanelBitmapBtn::bmpbtn_func) 
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(ConnSplitter, wxSplitterWindow)
EVT_TIMER(ID_TIMER_SCREENSHOT,ConnSplitter::on_timer_screenshot) 
EVT_SET_FOCUS(ConnSplitter::on_enter_focus)
EVT_KILL_FOCUS(ConnSplitter::on_kill_focus) 
EVT_ACTIVATE(ConnSplitter::on_activate)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(CredsDialog, wxDialog)
EVT_BUTTON(ID_CREDSDIALOG_OK, CredsDialog::button_ok_func) 
EVT_BUTTON(ID_CREDSDIALOG_CANCEL, CredsDialog::button_cancel_func)
EVT_CHAR_HOOK(CredsDialog::dialog_hotkeys)
END_EVENT_TABLE()

#ifdef __WXMSW__
BEGIN_EVENT_TABLE(RDPConnection,wxWindow)
EVT_ACTIVEX(wxID_ANY,RDPConnection::DispatcherActiveX)	
EVT_SET_FOCUS(RDPConnection::on_enter_focus)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ICAConnection,wxWindow)
EVT_ACTIVEX(wxID_ANY,ICAConnection::DispatcherActiveX)	
EVT_SET_FOCUS(ICAConnection::on_enter_focus)
END_EVENT_TABLE()
#endif

#ifdef __WXGTK__


BEGIN_EVENT_TABLE(ICAConnection, wxWindow)
//EVT_IDLE(wxRDP::OnIdle)
//EVT_TIMER(wxID_ANY, ICAConnection::OnTimer)
EVT_SET_FOCUS(ICAConnection::on_set_focus)
EVT_MOUSE_EVENTS(ICAConnection::on_any_mouse_event)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(wxContICA,wxWindow)
EVT_SET_FOCUS(wxContICA::on_set_focus)
EVT_KILL_FOCUS(wxContICA::on_kill_focus)
EVT_MOUSE_EVENTS(wxContICA::on_mouse_events)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(RDPConnection, wxWindow)
EVT_IDLE(RDPConnection::OnIdle)
EVT_TIMER(wxID_ANY, RDPConnection::OnTimer)
EVT_SET_FOCUS(RDPConnection::on_set_focus)
EVT_MOUSE_EVENTS(RDPConnection::on_any_mouse_event)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxContRDP,wxWindow)
EVT_SET_FOCUS(wxContRDP::on_set_focus)
EVT_KILL_FOCUS(wxContRDP::on_kill_focus)
EVT_MOUSE_EVENTS(wxContRDP::on_mouse_events)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ToolbarCnt,wxWindow)
EVT_ENTER_WINDOW(ToolbarCnt::on_enter)
EVT_LEAVE_WINDOW(ToolbarCnt::on_leave_func)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(RDPFullScreen,wxTopLevelWindow)
//EVT_MOTION(RDPFullScreen::on_mouse_motion)
EVT_TOOL(-1,RDPFullScreen::on_toolbar)
EVT_CHAR_HOOK(RDPFullScreen::dialog_hotkeys)
EVT_KEY_DOWN(RDPFullScreen::dialog_hotkeys)
END_EVENT_TABLE()



BEGIN_EVENT_TABLE(ICAFullScreen,wxTopLevelWindow)
//EVT_MOTION(RDPFullScreen::on_mouse_motion)
EVT_TOOL(-1,ICAFullScreen::on_toolbar)
END_EVENT_TABLE()

#endif


BEGIN_EVENT_TABLE(Main_Frame, wxFrame)
EVT_MENU(ID_NEWCONNECTION, Main_Frame::on_config)
EVT_MENU(ID_USERDPFILE, Main_Frame::on_userdpfile)
EVT_MENU(ID_EXPORTRDPFILE, Main_Frame::on_exportrdpfile)
EVT_MENU(ID_EXITPROGRAM, Main_Frame::on_quit)
EVT_MENU(ID_CONNECT, Main_Frame::on_connect)
EVT_MENU(ID_DISCONNECT, Main_Frame::on_disconnect)
EVT_MENU(ID_FULLSCREEN, Main_Frame::on_fullscreen)
EVT_MENU(ID_CLOSEPAGE, Main_Frame::on_closepage)
EVT_MENU(ID_ABOUTDIALOG, Main_Frame::on_about)
EVT_MENU(ID_FASTCONN, Main_Frame::on_fastconn)
EVT_MENU(ID_SHOWTREE, Main_Frame::on_showtree)
EVT_MENU(ID_HIDEFRAME, Main_Frame::on_hideframe)
EVT_MENU(ID_SETTINGSDIALOG, Main_Frame::on_settings_dialog) 
EVT_MENU(ID_SAVECONNECTED, Main_Frame::on_saveconnection)
EVT_MENU(ID_GRABINPUT, Main_Frame::on_grabinput)
EVT_MENU(ID_SENDCAD, Main_Frame::on_sendcad)
EVT_MENU(ID_SCREENPHOTO, Main_Frame::on_screenphoto)
EVT_AUINOTEBOOK_PAGE_CHANGED(ID_NOTEBOOK,Main_Frame::notebook_change_func)
EVT_AUINOTEBOOK_PAGE_CHANGING(ID_NOTEBOOK,Main_Frame::notebook_changing_func)
EVT_AUINOTEBOOK_PAGE_CLOSE(ID_NOTEBOOK,Main_Frame::notebook_closepage_func)
EVT_UPDATE_UI(ID_ERRORHOSTNAME,Main_Frame::closepage_num_func) 
EVT_UPDATE_UI(ID_FOCUSPAGE,Main_Frame::FocusCurrentPageEvt) 
EVT_ICONIZE(Main_Frame::on_iconize)
EVT_SIZE(Main_Frame::on_sizing) 
EVT_UPDATE_UI(ID_REDRAW_ALL,Main_Frame::RedrawAll) 
EVT_UPDATE_UI(ID_REDRAW_TABS,Main_Frame::RedrawTabs) 
EVT_UPDATE_UI(ID_ENABLE_TABS,Main_Frame::EnableTabsEvt) 
EVT_UPDATE_UI(ID_DISABLE_TABS,Main_Frame::DisableTabsEvt) 
//EVT_AUITOOLBAR_TOOL_DROPDOWN(ID_AUITOOLBAR_SHOWTREE,Main_Frame::on_auitoolbar_showtree )
//EVT_AUITOOLBAR_TOOL_DROPDOWN(ID_AUITOOLBAR_HIDEFRAME,Main_Frame::on_auitoolbar_hideframe )
//EVT_AUITOOLBAR_TOOL_DROPDOWN(-1,Main_Frame::on_auitoolbar )
EVT_TOOL(ID_AUITOOLBAR_SHOWTREE,Main_Frame::on_auitoolbar_showtree )
EVT_TOOL(ID_AUITOOLBAR_HIDEFRAME,Main_Frame::on_auitoolbar_hideframe )
EVT_TOOL(ID_AUITOOLBAR_GRABINPUT,Main_Frame::on_auitoolbar_grabinput )
EVT_TOOL(-1,Main_Frame::on_auitoolbar )
EVT_MAXIMIZE(Main_Frame::on_maximize)
EVT_CLOSE(Main_Frame::on_closing) 
#ifdef __WXGTK__
EVT_UPDATE_UI(ID_ERRORHOSTNAME,Main_Frame::closepage_num_func)
EVT_COMMAND(wxID_ANY, ID_CLOSE_RDESKTOP_PRG, Main_Frame::notebook_closepage_event)
EVT_COMMAND(wxID_ANY, ID_CONNECTION_STATUS_UPDATE, Main_Frame::connection_status_update)
#endif

END_EVENT_TABLE()

#ifdef __WXMSW__

wchar_t * AnsiToUnicode(const char * buffer)
{
	int size = MultiByteToWideChar(CP_ACP,0,buffer,-1,0,0);
	wchar_t * wch = new wchar_t [size];
	MultiByteToWideChar(CP_ACP,0,buffer,-1,wch,size);
	return wch;
}
#endif

programsettings load_main_settings()
{
	programsettings ps;
	Benc bc;
	bc.LoadProgramSettings(&ps);
		
	return ps;
}

