///////////////////////////////////////////////////////////////////////////////
// File name:   main.cpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-12-02 15:30:16"
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
#include "RFBConnection_nix.hpp"
#include "FRDPConnection_nix.hpp"
#endif
#ifdef __WXMSW__
#include "RDPConnection_win.hpp"
#include "ICAConnection_win.hpp"
#include "RFBConnection_win.hpp"
#endif

#include "res/splash_screen_400x300.cpp"
#include <wx/stdpaths.h>
#include <wx/intl.h>

IMPLEMENT_APP(RDP_Core)

ExceptionMessage::ExceptionMessage(bool fatal):
//msg(wxT("Fatal exception. Application will be closed.")),
wxDialog(NULL,-1,wxT(""),wxDefaultPosition,wxDefaultSize,wxCAPTION)
{
	if (fatal)
	{
		msg = _("Fatal exception. Application will be closed.");
	}
	else
	{
		msg = _("Internal Error. Application will be closed.");
	}

	m_static_msg = NULL;
	m_bitmap_err = NULL;
	m_line_separator = NULL;
	m_button_ok = NULL;

	sizer_main = new wxBoxSizer(wxVERTICAL);
	sizer_err = new wxBoxSizer(wxHORIZONTAL);
	wxString title = wxString::Format(_("%s error"),TS_NAME);
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
	wxHandleFatalExceptions(true);
	wxInitAllImageHandlers();
	programsettings ps = load_main_settings();
	LocaleHelper lh;
        lh.SetLang(ps.lang ,&locale);

// 	language =  wxLANGUAGE_DEFAULT;

// 	language = locale->GetSystemLanguage();

// 	if(wxLocale::IsAvailable(language))
// 	{
// 		locale = new wxLocale( language, wxLOCALE_CONV_ENCODING );

// #ifdef __WXGTK__
// 		locale->AddCatalogLookupPathPrefix(wxT("/usr"));
// 		locale->AddCatalogLookupPathPrefix(wxT("/usr/local"));
// 		wxStandardPaths* paths = (wxStandardPaths*) &wxStandardPaths::Get();
// 		wxString prefix = paths->GetInstallPrefix();
// 		locale->AddCatalogLookupPathPrefix( prefix );
// #endif

// 		locale->AddCatalog(wxT("rdpdesk"));

// 		if(! locale->IsOk() )
// 		{
// 			delete locale;
// 			locale = new wxLocale( wxLANGUAGE_DEFAULT );
// 			language = wxLANGUAGE_DEFAULT;
// 		}

// 	}
// 	else
// 	{
// 		locale = new wxLocale( wxLANGUAGE_DEFAULT );
// 		language = wxLANGUAGE_DEFAULT;


// 	}

	int frame_width;
	int frame_heigth;
	wxDisplaySize(&frame_width,&frame_heigth);

	frame_width *= 0.85;
	frame_heigth *= 0.88;


	wxSplashScreen* splash = new wxSplashScreen(wxGetBitmapFromMemory(splash_screen_400x300_png),
												wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
												6000, NULL, -1, wxDefaultPosition, wxDefaultSize,
												wxSIMPLE_BORDER|wxSTAY_ON_TOP);
	splash->GetTimeout();
	wxYield();

	main_frame = new MainFrame(TS_NAME ,wxPoint(50,50), wxSize(frame_width,frame_heigth),
								wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN );
	main_frame->Show(true);
	Autoupdate *auto_update_thread = new Autoupdate(main_frame);
	wxThreadError err = auto_update_thread->Create();
	err = auto_update_thread->Run();
	return true;
}

void RDP_Core::OnFatalException()
{
	wxBell();
	ExceptionMessage em;
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
EVT_UPDATE_UI(ID_RDPWIN_NEED_RECONNECT_EVENT, RDPConnection::OnNeedReconnect)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(RFBConnection, wxWindow)
EVT_ACTIVEX(wxID_ANY, RFBConnection::DispatcherActiveX)
EVT_SET_FOCUS(RFBConnection::on_enter_focus)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(RFBFullScreen, wxTopLevelWindow)
EVT_SET_FOCUS(RFBFullScreen::on_enter_focus)
EVT_TOOL(-1,RFBFullScreen::on_toolbar)
END_EVENT_TABLE()

#endif

#ifdef __WXGTK__

BEGIN_EVENT_TABLE(RFBConnection, wxWindow)
//EVT_IDLE(wxRDP::OnIdle)
//EVT_TIMER(wxID_ANY, ICAConnection::OnTimer)
EVT_SET_FOCUS(RFBConnection::on_set_focus)
// EVT_MOUSE_EVENTS(RFBConnection::on_any_mouse_event)
EVT_COMMAND(wxID_ANY, WXRFB_WINDOW_RFV, RFBConnection::WindowRFB)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxContRFB,wxWindow)
EVT_SET_FOCUS(wxContRFB::on_set_focus)
EVT_KILL_FOCUS(wxContRFB::on_kill_focus)
EVT_MOUSE_EVENTS(wxContRFB::on_mouse_events)
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

BEGIN_EVENT_TABLE(RFBFullScreen,wxTopLevelWindow)
//EVT_MOTION(RDPFullScreen::on_mouse_motion)
EVT_TOOL(-1,RFBFullScreen::on_toolbar)
END_EVENT_TABLE()

#endif

BEGIN_EVENT_TABLE(FullScreenToolBar, wxAuiToolBar)
EVT_TOOL(wxID_ANY, FullScreenToolBar::ProcessEvent)
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

