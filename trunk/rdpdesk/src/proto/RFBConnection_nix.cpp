///////////////////////////////////////////////////////////////////////////////
// File name:   RFBConnection_nix.cpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-12-06 20:15:36"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RFBDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include <memory>

#include <wx/stattext.h>

#include "RFBConnection_nix.hpp"

//BEGIN_EVENT_TABLE(RFBFullScreen, wxTopLevelWindow)

//END_EVENT_TABLE()

RFBFullScreen::RFBFullScreen(wxContRFB * cnt, Options_HashMap options) :
	wxTopLevelWindow(NULL, -1, wxT(""), wxDefaultPosition, wxDefaultSize)
{
	rdp = (RFBConnection *)cnt->GetParent();
	cnt_old = cnt;
	//rdpc = rdpconn;
	local_options = options;
	win = NULL;
	win2 = NULL;
	tb = NULL;
	SetSize(wxGetDisplaySize());
	Show();
	win = new wxScrolledWindow(this, -1, wxPoint(0,0), GetClientSize());
	ShowFullScreen(true);
//      win->SetSize(GetClientSize());
	win->FitInside();
	win2 = new ToolbarCnt(this);

	const int heigth = wxAtoi(local_options[wxT("heigth")]);
	const int width = wxAtoi(local_options[wxT("width")]);
	const wxString connection_name, full_screen;
	// for (int i = 0 ; i < local_options.Count(); i++)
	// {
	//    if (local_options.Item(i).Item(0).name == wxT("connection_name"))
	//    {
	//        connection_name = local_options.Item(i).Item(0).value;
	//    }
	//    if (local_options.Item(i).Item(0).name == wxT("control_size"))
	//    {
	//        control_size = local_options.Item(i).Item(0).value;
	//    }
	//    if (local_options.Item(i).Item(0).name == wxT("full_screen"))
	//    {
	//        full_screen = local_options.Item(i).Item(0).value;
   //    }
   //    if (local_options.Item(i).Item(0).name == wxT("heigth"))
   //    {
   //    heigth = wxAtoi(local_options.Item(i).Item(0).value);
   //    }
   //    if (local_options.Item(i).Item(0).name == wxT("width"))
   //    {
   //    width = wxAtoi(local_options.Item(i).Item(0).value);
   //    }
   // }
	tb = new FullScreenToolBar(connection_name, win2, -1, wxPoint(100, 100), wxSize(200, 22));

	win2->fs = tb;

	GtkWidget * widget_fullscreen = win->m_wxwindow;
	GtkWidget * widget_cont = cnt->m_wxwindow;
	gtk_container_add(GTK_CONTAINER(widget_fullscreen),widget_cont);
	gtk_widget_reparent(widget_cont,widget_fullscreen);

	wxSize fs = wxGetDisplaySize();

	if (wxAtoi(local_options[wxT("control_size")])) {
		RFBConnection * rdp_temp = (RFBConnection *)cnt->GetParent();
		const wxSize sz = rdp_temp->GetVirtualSize();
		gtk_widget_set_size_request(widget_cont, sz.x, sz.y);
	}
	else if (wxAtoi(local_options[wxT("full_screen")])) {
		wxSize disp_size = wxGetDisplaySize();
		gtk_widget_set_size_request(widget_cont, disp_size.x, disp_size.y);
	} else
		gtk_widget_set_size_request(widget_cont, width, heigth);

	if (!wxAtoi(local_options[wxT("control_size")])) {
		if ((width > fs.x) || (heigth > fs.y)) {
			if (!wxAtoi(full_screen))
				win->SetScrollbars(1, 1, width, heigth);
		}
		else  ////////////////
		{
			int rdp_x = 0;
			int rdp_y = 0;
			gtk_widget_get_size_request(widget_cont,&rdp_x,&rdp_y);
			win->SetSize(wxSize(rdp_x,rdp_y));
		}     ////////
	} else {
		int rdp_x = 0;
		int rdp_y = 0;
		gtk_widget_get_size_request(widget_cont,&rdp_x,&rdp_y);
		win->SetSize(wxSize(rdp_x,rdp_y));
	}

	gtk_widget_show(widget_fullscreen);
	tb->fullscreen = this;
	tb->win = win;
	cnt->CenterOnParent();
	win->Center();
	win->SetVirtualSize(cnt->GetSize());
	win->FitInside();
	win->Refresh();
	win->Update();
	win2->Raise();
	win2->Fit();
	win2->SetPosition(wxPoint(0,0));
	win2->Center(wxHORIZONTAL);
	win2->on_leave();
	Update();
}

RFBFullScreen::~RFBFullScreen() {
	delete win;
	delete tb;
	delete win2;
}

void RFBFullScreen::dialog_hotkeys(wxKeyEvent &event)
{
//   std::cout << "THIS" << std::endl;
   wxCommandEvent evt;
   evt.SetId(1);
   switch(event.GetKeyCode())
   {
      case WXK_RETURN + WXK_ALT + WXK_CONTROL:
         //this->button_ok_func(evt);
//       wxMessageBox (wxT("THIS"));

         break;
      case WXK_ESCAPE:
         //this->button_cancel_func(evt);
         break;

         // case WXK_TAB:
         //      if (event.GetModifiers() == wxMOD_SHIFT)
         //      {
         //         if (notebook)
         //         {
         //            size_t iSel = (notebook->GetSelection() + 1) % (notebook->GetPageCount());
         //            notebook->SetSelection(iSel);
         //         }
         //      }
         //      break;
      default:

         break;

   }
   event.Skip();
}

void RFBFullScreen::Leave()
{
   ShowFullScreen(false);

   RFBConnection * rdp = (RFBConnection *)cnt_old->GetParent();
   GtkWidget * rdp_w = rdp->m_wxwindow;
   GtkWidget * rdp_c = cnt_old->m_wxwindow;
   int heigth = wxAtoi(local_options[wxT("heigth")]);
   int width = wxAtoi(local_options[wxT("width")]);
   wxString connection_name, control_size, full_screen;
   // for (int i = 0 ; i < local_options.Count(); i++)
   // {
   //    if (local_options.Item(i).Item(0).name == wxT("connection_name"))
   //    {
   //    connection_name = local_options.Item(i).Item(0).value;
   //    }
   //    if (local_options.Item(i).Item(0).name == wxT("control_size"))
   //    {
   //    control_size = local_options.Item(i).Item(0).value;
   //    }
   //    if (local_options.Item(i).Item(0).name == wxT("full_screen"))
   //    {
   //    full_screen = local_options.Item(i).Item(0).value;
   //    }
   //    if (local_options.Item(i).Item(0).name == wxT("heigth"))
   //    {
   //    heigth = wxAtoi(local_options.Item(i).Item(0).value);
   //    }
   //    if (local_options.Item(i).Item(0).name == wxT("width"))
   //    {
   //    width = wxAtoi(local_options.Item(i).Item(0).value);
   //    }
   // }
   gtk_widget_reparent(rdp_c,rdp_w);
   if (wxAtoi(local_options[wxT("control_size")]))
   {
      wxSize sz = rdp->GetVirtualSize();
      gtk_widget_set_size_request(rdp_c,sz.x,sz.y);
   }
   else if (wxAtoi(local_options[wxT("full_screen")]))
   {
      wxSize disp_size = wxGetDisplaySize();
      gtk_widget_set_size_request(rdp_c, disp_size.x, disp_size.y);
   }
   else
   {
      gtk_widget_set_size_request(rdp_c,width,heigth);
   }
   // for (int j = 0 ; j < local_options.Count(); j++)
   // {
   //    if (local_options.Item(j).Item(0).name == wxT("full_screen"))
   //    {
   //    local_options.Item(j).Item(0).value = wxT("0");
   //    }
   // }

   rdp->bFullScreen = FALSE;
   rdp->conn_splitter->m_timer_screenshot.Start();
   Close();
   Destroy();
}


void RFBFullScreen::on_toolbar(wxCommandEvent& event) {
	switch(event.GetId()) {
	case ID_FULLSCREEN_PHOTO:   rdp->photo();
	                            break;
	case ID_FULLSCREEN_SENDCAD: rdp->SendKey(TRUE);
	                            break;
	case ID_FULLSCREEN_GRABINPUT:
		if (tb->GetToolToggled(ID_FULLSCREEN_GRABINPUT))
			rdp->GrabAll(TRUE);
		else
			rdp->GrabAll(FALSE);
		break;

	case ID_FULLSCREEN_CLOSE: Leave();
	                          rdp->request_close();
	                          break;
	case ID_FULLSCREEN_LEAVE: Leave();
	                          break;
	}
}


RFBConnection::RFBConnection(MainFrame *m_frame,Options_HashMap all_options,
                             wxWindow *parent, wxWindowID id,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style,
                             const wxString& name)
   :BasicConnection(m_frame,all_options,parent,id,pos,size,0,name)
{

   // if (!wxFileExists(wxT("/usr/bin/rdpdesk.vncview"))) rdpdesk.vncviewer
   // {
   //       std::cout << __LINE__ << std::endl;

   //    return;
   // }
	local_options = all_options;
	sock             = NULL;
	bClosing = FALSE;

	fullscreen_frame = NULL;
	lock             = NULL;
	lock_cnt         = NULL;
	Info.Clear();
	info_uniq_name   = 0;
//      main_frame = m_frame;
//      bConnected = false;
//      bFullScreen = false;
   conn_splitter = (ConnSplitter *)parent;
   // for (int i = 0; i < options.Count();  i++)
   // {
   //    if (options.Item(i).Item(0).name == wxT("pid"))
   //    {
   //    options.Item(i).Item(0).value = wxT("0");
   //    found_pid = true;
   //    }

   // }
   // if (found_pid == false)
   // {
   //    temp_options.type = wxT("s");
   //    temp_options.name = wxT("pid");
   //    temp_options.value = wxT("0");
   //    temp_array.Add (temp_options);
   //    options.Add(temp_array);
   // }
   wxFile temp;
   GtkWidget *widget = m_wxwindow;
   cnt = new wxContRFB(this, -1, wxPoint(0, 0), wxDefaultSize);
   gtk_widget_realize(widget);
   GdkWindow *gdkWin = GTK_PIZZA (widget)->bin_window;
   Window wid = GDK_WINDOW_XWINDOW(gdkWin);
   xid =  wid;
   //rdp_settings = rdpsettings;
//      rdpconn = rdpsettings;

   GETRDESKTOPPATH();

   if ((RDESKTOPPATH != wxT("")) && (temp.Exists(RDESKTOPPATH)))
      wxrdp_status = WXRFB_OK;
   // else
   // {
   //    Error_mgs = wxT("Can't found \"rdeskto\" file.\nPlease press Alt+O, and edit patch to \"rdesktop\" executable file");
   //    wxrdp_status = RDESKTOP_NOT_FOUND;
   // }
   bObjectOk = TRUE;
   finder = 0;
   rfb_window = 0;
}

RFBConnection::~RFBConnection() {}

void RFBConnection::CentreRFB() {
	CheckWindow();

	const wxSize sz_cnt = cnt->GetSize();
	const wxSize sz_this = conn_splitter->GetClientSize();

	cnt->SetPosition(wxPoint(0,0));

	if ((sz_cnt.x < sz_this.x) || (sz_cnt.y < sz_this.y))
		cnt->CenterOnParent();
}

void RFBConnection::photo() {
	wxClientDC client_dc(this->cnt);
	wxBitmap bmp(client_dc.GetSize().GetWidth(), client_dc.GetSize().GetHeight());
	wxMemoryDC mdc(bmp);
	mdc.Blit(0, 0, bmp.GetWidth(), bmp.GetHeight(), &client_dc, 0, 0);
	mdc.SelectObject(wxNullBitmap);

	std::auto_ptr<wxFileDialog> savefiledialog(
		new wxFileDialog(this,
		                 wxT("Save screenshot"),
		                 wxT(""),
		                 wxT(""),
		                 wxT("*.bmp"),
		                 wxFD_SAVE | wxFD_OVERWRITE_PROMPT));

	const int iRes = savefiledialog->ShowModal();
	if (iRes == wxID_CANCEL)
		return;

	const wxString filepath(savefiledialog->GetPath());
	if (!filepath.IsEmpty())
	   bmp.SaveFile(filepath + wxT(".bmp"), wxBITMAP_TYPE_BMP);
}

void RFBConnection::FullScreen(BOOL_L bRestore) {
	if (bRestore) {
		//        RFBFullScreen * win = NULL;
		fullscreen_frame = new RFBFullScreen((wxContRFB *)cnt,local_options);
		if (fullscreen_frame) {
			conn_splitter->m_timer_screenshot.Stop();
			bFullScreen = TRUE;
		}
	}
}


void RFBConnection::SendKey(BOOL_L cad)
{
//   std::cout << GetParent()->FindFucus()->GetId() << std::endl;

//      GtkWidget *widget = main_frame->m_wxwindow;
   GtkWidget *widget = cnt->m_wxwindow;

   GdkWindow *gdkWin = GTK_PIZZA (widget)->bin_window;
   Window wid = GDK_WINDOW_XWINDOW(gdkWin);
   Display * disp = XOpenDisplay(NULL);

   Window cur_focus;
   XKeyEvent event;

   cur_focus = wid;

   event.display = disp;
   event.window = rfb_window;
   event.root = RootWindow(event.display, DefaultScreen(event.display));
   event.subwindow = None;
   event.time = CurrentTime;
   event.x = 1;
   event.y = 1;
   event.x_root = 1;
   event.y_root = 1;
   event.same_screen = TRUE;

   event.state = 0;

   if (cad)
   {
      event.type = KeyPress;

      event.keycode = XKeysymToKeycode(disp, XK_Control_L);
      SendXKeyEvent(&event);

      event.keycode = XKeysymToKeycode(disp, XK_Alt_L);
      SendXKeyEvent(&event);

      event.keycode = XKeysymToKeycode(disp, XK_Delete);
      SendXKeyEvent(&event);

      event.type = KeyRelease;

      event.keycode = XKeysymToKeycode(disp, XK_Control_L);
      SendXKeyEvent(&event);

      event.keycode = XKeysymToKeycode(disp, XK_Alt_L);
      SendXKeyEvent(&event);
   }
   else
   {
      event.type = KeyPress;
      event.keycode = XKeysymToKeycode(disp, XK_Control_L);
      SendXKeyEvent(&event);
      event.type = KeyRelease;
      SendXKeyEvent(&event);
      event.type = KeyPress;
      event.keycode = XKeysymToKeycode(disp, XK_Alt_L);
      SendXKeyEvent(&event);
      event.type = KeyRelease;
      SendXKeyEvent(&event);

      event.type = KeyPress;
      event.keycode = XKeysymToKeycode(disp, XK_Alt_L);
      SendXKeyEvent(&event);
      event.type = KeyRelease;
      SendXKeyEvent(&event);
   }

   XCloseDisplay(disp);
}

void RFBConnection::SendXKeyEvent(XKeyEvent * event)
{
   XSync(event->display, FALSE);
   XSendEvent(event->display, event->window, TRUE, KeyPressMask, (XEvent *)event);
   XSync(event->display, FALSE);
}

void RFBConnection::GrabAll(BOOL_L state)
{
   if (state)
   {
      EnableFocus(FALSE);
      gdk_keyboard_grab (gtk_widget_get_window (GTK_WIDGET (sock)), TRUE, GDK_CURRENT_TIME);
      EnableFocus(TRUE);
      bGrab = TRUE;
   }
   else
   {
      EnableFocus(FALSE);
      EnableFocus(TRUE);
      bGrab = FALSE;
   }
}

wxContRFB::wxContRFB(wxWindow * parent,wxWindowID id,const wxPoint& point,const wxSize& size):
   wxWindow(parent,id,point,size)
{
}


void wxContRFB::on_set_focus(wxFocusEvent& event) {
	RFBConnection * rdp = (RFBConnection *)GetParent();
	if (rdp->bConnected)
		rdp->CheckFocus();

	event.Skip();
}

void wxContRFB::on_kill_focus(wxFocusEvent& event) {
	RFBConnection * rdp = (RFBConnection *)GetParent();
	if (rdp->bConnected)
		rdp->EnableFocus(FALSE);

	event.Skip();
}

void wxContRFB::on_mouse_events(wxMouseEvent& event) {
	// RFBConnection * rdp = (RFBConnection *)GetParent();
	// if (event.Entering())
	// {
	//    rdp->CheckFocus();
	// }
	event.Skip();
}

void RFBConnection::EnableFocus(BOOL_L state) {
	if (state) {
		std::auto_ptr<wxWindowUpdateLocker> lock (new wxWindowUpdateLocker(this));
		std::auto_ptr<wxWindowUpdateLocker> lock_cnt(new wxWindowUpdateLocker(cnt));

		gtk_widget_set_can_focus(sock, true);
		gdk_window_focus(gtk_widget_get_window(sock),GDK_CURRENT_TIME);
		gtk_widget_grab_focus(sock);

		SendKey();
		CentreRFB();
	} else {
		gtk_widget_set_can_focus(sock, false);
		gdk_keyboard_ungrab(GDK_CURRENT_TIME);
	}
}


void RFBConnection::on_set_focus(wxFocusEvent& event) {
	if (bConnected)
		CheckFocus();

	event.Skip();
}

void RFBConnection::on_leave_focus(wxFocusEvent& event) {
	if (bConnected)
		EnableFocus(FALSE);

	event.Skip();
}

void RFBConnection::CheckFocus() {
	gboolean bFocus = gtk_widget_is_focus(GTK_WIDGET(sock));
	if (!bFocus) {
		if (bGrab)
			GrabAll(TRUE);
		else
			EnableFocus(TRUE);
	}
}


void RFBConnection::on_any_mouse_event(wxMouseEvent& event) {
	// if (event.Entering())
	// {
	//    CheckFocus();
	// }
	event.Skip();
}

void RFBConnection::CheckWindow() {
	const wxSize cnt_size = cnt->GetSize();

	const wxSize rdp_size = GetSize();
	if (rdp_size.x < cnt_size.x || rdp_size.y < cnt_size.y)
		SetScrollbars(1,1,cnt_size.x,cnt_size.y);
	else {
		FitInside();
		Refresh();
	}

	Update();
}

void RFBConnection::wait_exit(GPid pid, gint status, gpointer data) {
	RFBConnection * rdp = (RFBConnection *)data;
	g_spawn_close_pid (pid);
	rdp->bConnected = false;

//	if (rdp->info_uniq_name != 0)
//		rdp->main_frame->m_panel_tree->rdptree->from_wxrdp(rdp->info_uniq_name,TREEDATA_DEC_CONNCOUNT);
	rdp->SendDisconnectEvent();

	if (status != 0 && status != SIGTERM) {
		char buffer[4096];
		memset(&buffer, 0, sizeof(buffer));
		int len;

		lseek(rdp->error_fd, 0, SEEK_SET);
		len = read(rdp->error_fd, &buffer, sizeof(buffer) - 1);

		if (len > 0) {
			// const wxString error_string = wxString::From8BitData(&buffer, len);

			//// TEST
			rdp->EnableFocus(FALSE);

			//wxMessageBox(error_string,wxT("Error"),wxICON_ERROR);
		}
	}

	close(rdp->output_fd);
	close(rdp->error_fd);

	if (rdp->bFullScreen && rdp->fullscreen_frame)
		rdp->fullscreen_frame->Leave();

	rdp->fullscreen_frame = NULL;


	if (!rdp->bClosing) {
		wxCommandEvent evt;
		evt.SetId(ID_CONNECTION_STATUS_UPDATE);
		rdp->main_frame->connectionStatusUpdate(evt);
	}

	rdp->sock_id = 0;
	if (rdp->sock) {
		gtk_widget_destroy(rdp->sock);
		rdp->sock = NULL;
	}

	if (rdp->bClosing) {
		const int lc = rdp->main_frame->nb->GetPageCount();
		if (lc > 0) {
			for (int i = 0; i < lc ; i ++) {
				if (rdp->conn_splitter == (ConnSplitter *)rdp->main_frame->nb->GetPage(i)) {
					rdp->main_frame->current_page_for_delete = i;
					wxUpdateUIEvent evt;
					evt.Check(true);
					evt.SetId(ID_ERRORHOSTNAME);
					wxPostEvent(rdp->main_frame, evt);
					rdp->bClosing = FALSE;
					return;
				}
			}
		}
	}

	rdp->bClosing = FALSE;
}

bool RFBConnection::DoRdp() {
	cnt->SetPosition(wxPoint(0,0));
	wxString hostname, port;
	bool ret = false;
	wxSize rdp_size;

	sock = gtk_socket_new();
	gtk_widget_show(sock);
	GtkWidget *widget = this->m_wxwindow;
	GtkWidget * cnt_widget = cnt->m_wxwindow;
	gtk_container_add(GTK_CONTAINER(cnt_widget),sock);

	gtk_widget_realize( widget );
	gtk_widget_realize(sock);
	rdp_size = GetClientSize();
	cnt->SetSize(rdp_size);
	gtk_widget_set_size_request (GTK_WIDGET (sock), rdp_size.x,rdp_size.y);
	sock_id = gtk_socket_get_id(GTK_SOCKET(sock));
//   RFBConn rdp_settings;
//   std::cout << __LINE__ << " " << __func__ << std::endl;
   //RFBConn * settings = &rdp_settings;

	gchar *argv[50];
	int argc = 0;
	argv[argc++] = g_strdup ("rdpdesk.vncviewer");

   //if (settings != NULL)
   //{
   // if ( local_options[wxT("username")] != wxT(""))
   // {
   //    argv[argc++] = g_strdup("-u");
   //    argv[argc++] = g_strdup (local_options[wxT("username")].ToUTF8());
   // }
   // if (local_options[wxT("domain")] != wxT(""))
   // {
   //    argv[argc++] = g_strdup("-d");
   //    argv[argc++] = g_strdup(local_options[wxT("domain")].ToUTF8());
   // }
   // if (local_options[wxT("shell")] != wxT(""))
   // {
   //    argv[argc++] = g_strdup("-s");
   //    argv[argc++] = g_strdup(local_options[wxT("shell")].ToUTF8());
   // }
   // if (local_options[wxT("directory")] != wxT(""))
   // {
   //    argv[argc++] = g_strdup(" -c ");
   //    argv[argc++] = g_strdup(local_options[wxT("directory")].ToUTF8());
   // }
   // if (local_options[wxT("password")] != wxT(""))
   // {
   //    argv[argc++] = g_strdup("-p");
   //    argv[argc++] = g_strdup(local_options[wxT("password")].ToUTF8());
   // }
   // if (local_options[wxT("keyboard_map")].IsEmpty() == false)
   // {
   //    GETRDESKTOPMAPPATH();
   //    wxArrayString list_keyboard_map;
   //    wxString temp_str;
   //    wxDir temp(RDESKTOPMAPPATH);
   //    if ((RDESKTOPMAPPATH != wxT("")) && (temp.Exists(RDESKTOPMAPPATH)))
   //    {
   //    bool check = temp.GetFirst(&temp_str);
   //    list_keyboard_map.Add( wxT("en-us"));
   //    while (check == true)
   //    {
   //       if (temp_str != wxT("en-us"))
   //       {
   //          list_keyboard_map.Add(temp_str);
   //       }
   //       check = temp.GetNext(&temp_str);
   //    }
   //    }
   //    else
   //    {
   //    list_keyboard_map.Add( wxT("en-us"));
   //    list_keyboard_map.Add( wxT("ru-ru"));
   //    }
   //    argv[argc++] = g_strdup("-k");
   //    argv[argc++] = g_strdup(list_keyboard_map[wxAtoi(local_options[wxT("keyboard_map")])].ToUTF8());
   // }
   // if (wxAtoi(local_options[wxT("control_size")]))
   // {
   //    int width = 0;
   //    int height = 0;
   //    conn_splitter->GetSize(&width, &height);
   //    GetClientSize(&width, &height);
   //    cnt->SetSize(wxSize(width,height));
   //    gtk_widget_set_size_request (GTK_WIDGET (sock), width,height);
   //    if ((width >= 0) && (height >= 0))
   //    {
   //    argv[argc++] = g_strdup ("-g");
   //    argv[argc++] = g_strdup_printf ("%ix%i", width, height);
   //    }
   //    else
   //    {
   //    return false;
   //    }
   // }
   // else if (wxAtoi(local_options[wxT("full_screen")]))
   // {
   //    wxSize disp_size = wxGetDisplaySize();
   //    argv[argc++] = g_strdup ("-g");
   //    argv[argc++] = g_strdup_printf ("%ix%i", disp_size.x, disp_size.y);

   //    cnt->SetSize(disp_size);
   //    if (rdp_size.x < disp_size.x || rdp_size.y < disp_size.y)
   //    {
   //    SetScrollbars(1,1,disp_size.x,disp_size.y);
   //    }
   //    gtk_widget_set_size_request (GTK_WIDGET (sock), disp_size.x, disp_size.y);

   // }
   // else if ((wxAtoi(local_options[wxT("width")]) > 0) &&
   //       (wxAtoi(local_options[wxT("heigth")])) > 0)
   // {
   //    argv[argc++] = g_strdup ("-g");
   //    argv[argc++] = g_strdup_printf ("%ix%i",  wxAtoi(local_options[wxT("width")]),
   //                                 wxAtoi(local_options[wxT("heigth")]));
   //    cnt->SetSize(wxSize(wxAtoi(local_options[wxT("width")]),
   //                     wxAtoi(local_options[wxT("heigth")])));
   //    if ((rdp_size.x < wxAtoi(local_options[wxT("width")])) ||
   //     (rdp_size.y < wxAtoi(local_options[wxT("heigth")])))
   //    {
   //    SetScrollbars(1,1,wxAtoi(local_options[wxT("width")]),
   //                  wxAtoi(local_options[wxT("heigth")]));
   //    }
   //    gtk_widget_set_size_request (GTK_WIDGET (sock), wxAtoi(local_options[wxT("width")]),
   //                              wxAtoi(local_options[wxT("heigth")]));
   // }
   // if (wxAtoi(local_options[wxT("force_update_screen")]))
   // {
   //    argv[argc++] = g_strdup("-f");
   // }
   // if (wxAtoi(local_options[wxT("backing_store")]))
   // {
   //    argv[argc++] = g_strdup("-B");
   // }
   // if (wxAtoi(local_options[wxT("encription_enable_french")]))
   // {
   //    argv[argc++] = g_strdup("-e");
   // }
   // if (wxAtoi(local_options[wxT("encription_enable_new")]))
   // {
   //    argv[argc++] = g_strdup("-E");
   // }
   // if (wxAtoi(local_options[wxT("send_mouse_event")]))
   // {
   //    argv[argc++] = g_strdup("-m");
   // }
   // if (wxAtoi(local_options[wxT("private_color_map")]))
   // {
   //    argv[argc++] = g_strdup("-C");
   // }
   // if (wxAtoi(local_options[wxT("settings->numlock_sync")]))
   // {
   //    argv[argc++] = g_strdup("-N");
   // }
   // if (wxAtoi(local_options[wxT("color_depth")]) == 0)
   // {
   //    argv[argc++] = g_strdup("-a");
   //    argv[argc++] = g_strdup_printf(("%i"), 8);
   // }
   // else if (wxAtoi(local_options[wxT("color_depth")]) == 1)
   // {
   //    argv[argc++] = g_strdup("-a");
   //    argv[argc++] = g_strdup_printf(("%i"), 15);
   // }
   // else if (wxAtoi(local_options[wxT("color_depth")]) == 2)
   // {
   //    argv[argc++] = g_strdup("-a");
   //    argv[argc++] = g_strdup_printf(("%i"), 16);
   // }
   // else if (wxAtoi(local_options[wxT("color_depth")]) == 3)
   // {
   //    argv[argc++] = g_strdup("-a");
   //    argv[argc++] = g_strdup_printf(("%i"), 24);
   // }
   // else
   // {
   //    argv[argc++] = g_strdup("-a");
   //    argv[argc++] = g_strdup_printf(("%i"), 8);
   // }
   // if (wxAtoi(local_options[wxT("enable_compres")]))
   // {
   //    argv[argc++] = g_strdup("-z");
   // }
   // if (wxAtoi(local_options[wxT("bandwidth")]) == 0 ||
   //     wxAtoi(local_options[wxT("bandwidth")]) == 1)
   // {
   //    argv[argc++] = g_strdup("-x");
   //    argv[argc++] = g_strdup("m");
   // }
   // else if (wxAtoi(local_options[wxT("bandwidth")]) == 2)
   // {
   //    argv[argc++] = g_strdup("-x");
   //    argv[argc++] = g_strdup("b");
   // }
   // else if (wxAtoi(local_options[wxT("bandwidth")]) == 3)
   // {
   //    argv[argc++] = g_strdup("-x");
   //    argv[argc++] = g_strdup("l");
   // }

   // if (wxAtoi(local_options[wxT("enable_bitmap_caching")]))
   // {
   //    argv[argc++] = g_strdup("-P");
   // }
   // if (wxAtoi(local_options[wxT("attach_to_console")]))
   // {
   //    argv[argc++] = g_strdup("-0");
   // }
   // if (wxAtoi(local_options[wxT("use_rdp_version")]) == 1)
   // {
   //    argv[argc++] = g_strdup("-5");
   // }
   // else if (wxAtoi(local_options[wxT("use_rdp_version")]) == 0)
   // {
   //    argv[argc++] = g_strdup("-4");
   // }
   // if (wxAtoi(local_options[wxT("sound_type")]) == 0)
   // {
   //    argv[argc++] = g_strdup ("-r");
   //    argv[argc++] = g_strdup ("sound:local");
   // }
   // else if (wxAtoi(local_options[wxT("sound_type")]) == 1)
   // {
   //    argv[argc++] = g_strdup ("-r");
   //    argv[argc++] = g_strdup ("sound:off");
   // }
   // else if (wxAtoi(local_options[wxT("SoundType")]) == 2)
   // {
   //    argv[argc++] = g_strdup ("-r");
   //    argv[argc++] = g_strdup ("sound:remote");
   // }
   // if (local_options[wxT("linux_devices")] != wxT(""))
   // {
   //    //     wxMessageBox(settings->redirect_devices_nix);
   //    wxString devices(local_options[wxT("linux_devices")]);
   //    for (size_t i = 0; i < devices.Length() - 2; i ++)
   //    {
   //    if (devices.Mid(i,3) == wxT("-r "))
   //    {
   //       for (size_t j = i + 3; j <= devices.Length(); j ++)
   //       {
   //          if (devices.Mid(j,3) == wxT("-r "))
   //          {
   //             wxString device = devices.Mid(i + 3, j - i - 4);
   //             //                            wxMessageBox(device);
   //             argv[argc++] = g_strdup ("-r");
   //             argv[argc++] = g_strdup (device.ToUTF8());
   //             break;
   //          }
   //          else if (j == devices.Length())
   //          {
   //             wxString device = devices.Mid(i + 3, j - i - 3);
   //             //wxMessageBox(device);
   //             argv[argc++] = g_strdup ("-r");
   //             argv[argc++] = g_strdup (device.ToUTF8());
   //             break;
   //          }
   //       }
   //    }
   //    }
   // }
   //}
	if ((local_options[wxT("port")].IsEmpty() == false) &&
	    (local_options[wxT("hostname")].IsEmpty() == false)) {
		hostname = local_options[wxT("hostname")] + wxT("::") + local_options[wxT("port")];
//      wxMessageBox(local_options[wxT("hostname")]);
//      wxMessageBox(local_options[wxT("port")]);
		argv[argc++] = g_strdup (hostname.ToUTF8());
	} else if (local_options[wxT("hostname")].IsEmpty() == false) {
		hostname = local_options[wxT("hostname")];
		argv[argc++] = g_strdup (hostname.ToUTF8());
	}

	const wxString title = wxString::Format(wxT("0x%x RFB"),uniq_conn);
	argv[argc++] = g_strdup("-title");
	argv[argc++] = g_strdup(title.ToUTF8());

	if (local_options[wxT("proxy_host")] != wxT("")) {
		argv[argc++] = g_strdup("-via");
		argv[argc++] = g_strdup(local_options[wxT("proxy_host")].ToUTF8());
	}
	if(wxAtoi(local_options[wxT("shared")]))
		argv[argc++] = g_strdup ("-shared");
	if(wxAtoi(local_options[wxT("viewonly")]))
		argv[argc++] = g_strdup ("-viewonly");

	argv[argc++] = g_strdup ("-window_id");
	argv[argc++] = g_strdup_printf ("%i", sock_id );
	//int window = atoi(g_strdup_printf ("%i",
	//GDK_WINDOW_XID(widget->window) ));
	int window = atoi(g_strdup_printf ("%i", sock_id));
	//std::cout << window << std::endl;

	// argv[argc++] = g_strdup ("-window_parent");
	// argv[argc++] = g_strdup_printf ("%d", this->m_wxwindow);

	argv[argc++] = g_strdup ("-fullscreen");
	argv[argc++] = g_strdup ("-x11cursor");

	argv[argc++] = NULL;

	if (local_options[wxT("hostname")] != wxT("")) {
		GSpawnFlags spawn_flags = (GSpawnFlags)(G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD);

		ret = g_spawn_async_with_pipes (NULL, argv, NULL, spawn_flags, NULL,
		                                NULL, &sock_id, NULL, &output_fd, &error_fd, NULL);

		for (int i = 0; i < argc ; i++)
			g_free(argv[i]);

		if (ret) {
			bConnected = true;
			g_child_watch_add(sock_id, RFBConnection::wait_exit, this);

//		if (info_uniq_name != 0)
//			main_frame->m_panel_tree->rdptree->from_wxrdp(info_uniq_name,TREEDATA_INC_CONNCOUNT);

			SendConnectEvent();
			wxCommandEvent eventCustom(ID_CONNECTION_STATUS_UPDATE);
			wxPostEvent(main_frame, eventCustom);
			wxThread::Sleep(5);
/*			finder = new RFBFindWindow(window, (int *)&rfb_window, this);
			finder->Create();
			finder->Run();*/
		}
		gtk_widget_show(sock);
		CentreRFB();
	}
	//wxThread::Sleep(5);

	//rfb_window = GetRFBWindowId();
	// Display * dpy = XOpenDisplay(NULL);
	// XWindowAttributes win_attributes;
	// if (!XGetWindowAttributes(dpy, sock_id + 4, &win_attributes))
	// {
	// }
	// else
	// {
	//    local_options[wxT("width")] = wxString::Format(wxT("%d"), win_attributes.width);
	//    local_options[wxT("heigth")] = wxString::Format(wxT("%d"), win_attributes.height);
	//    cnt->SetSize(wxSize(win_attributes.width, win_attributes.height));
	//    gtk_widget_set_size_request (GTK_WIDGET (sock), win_attributes.width, win_attributes.height);
	// }

	//wxMessageBox(wxString::Format(wxT("%d"), sock_id));

	return ret;
}


wxString RFBConnection::get_error_string()
{
   return Error_mgs;
}

void RFBConnection::drop_error_string()
{
   Error_mgs.Clear();
   Error_mgs = wxT("");
}



void RFBConnection::close_rdesktop_prg(wxCommandEvent& event)
{
   wxCommandEvent eventCustom(ID_CLOSE_RDESKTOP_PRG);
   wxPostEvent(main_frame, eventCustom);
}

bool RFBConnection::request_close(BOOL_L bDetach)
{
   if (sock_id)
   {
      EnableFocus(FALSE);

      bClosing = bDetach;
      //        kill(sock_id,SIGKILL);
      kill(sock_id,SIGTERM);
   }

   return true;
}

void RFBConnection::event_dispatcher(wxCommandEvent& event)
{

}

void RFBConnection::OnIdle(wxIdleEvent& event)
{

}


void RFBConnection::OnTimer(wxTimerEvent& event)
{

}

bool RFBConnection::get_state()
{
   return bConnected;
}

bool RFBConnection::get_proc_state()
{
   return wxrdp_status;
}


void RFBConnection::set_state(bool state_to_set)
{
   bConnected = state_to_set;
}

void RFBConnection::set_proc_state(bool state_to_set)
{
   wxrdp_status = state_to_set;
}


void RFBConnection::rdesktop_not_found(wxCommandEvent& event)
{

}

int RFBConnection::get_state_wxrdp()
{
   return 0;
}




bool RFBConnection::DoConnection()
{
   return DoRdp();
}

bool RFBConnection::Connect()
{
   return DoRdp();

}

void RFBConnection::Disconnect()
{
   request_close();

}


bool RFBConnection::DisconnectClose()
{
   return request_close(TRUE);

}

void RFBConnection::CentreConnection()
{
   if (cnt != NULL)
      CentreRFB();

}

void RFBConnection::WindowRFB (wxCommandEvent& event)
{
   if (rfb_window > 0)
   {
      Display * dpy = XOpenDisplay(NULL);
      XWindowAttributes win_attributes;
      if (!XGetWindowAttributes(dpy, rfb_window + 4, &win_attributes))
      {
      }
      else
      {
      	 local_options[wxT("width")] = wxString::Format(wxT("%d"), win_attributes.width);
      	 local_options[wxT("heigth")] = wxString::Format(wxT("%d"), win_attributes.height);
      	 cnt->SetSize(wxSize(win_attributes.width, win_attributes.height));
      	 gtk_widget_set_size_request (GTK_WIDGET (sock), win_attributes.width, win_attributes.height);
      }
      CentreRFB();
      XCloseDisplay (dpy);
  }
}

// Window RFBConnection::GetRFBWindowId()
// {
//    wxString title = wxString::Format(wxT("0x%x RFB"),uniq_conn);

//    char * window_title = new char[256];
//    memset(window_title,0,256);
//    if (title.Length() > 256) return 0;
//    strcpy(window_title,title.To8BitData());

//    Display * dpy = XOpenDisplay(NULL);
//    int  screen = XDefaultScreen(dpy);
//    Window w = 0;
//    int pause_count = 40;
//    int time = 50;
//    while (!w && pause_count)
//    {
//       w = GetWindowByTitle(dpy, RootWindow(dpy, screen),window_title);
//       if (!w)
//       {
//          pause_count --;
//          wxThread::Sleep(time);
//       }
//    }
//    XCloseDisplay(dpy);
//    if (window_title) delete window_title;
//    return w;
// }

// Window RFBConnection::GetWindowByTitle(Display * dpy, Window top, char * name)
// {
//    Window *children, dummy;
//    unsigned int nchildren;
//    size_t i;
//    Window w=0;
// //      char *window_name; //test
//    char *window_name = NULL;


//    if (XFetchName(dpy, top, &window_name) && !strcmp(window_name, name))
//       return(top);

//    if (!XQueryTree(dpy, top, &dummy, &dummy, &children, &nchildren))
//       return(0);

//    for (i=0; i<nchildren; i++)
//    {
//       w = GetWindowByTitle(dpy, children[i], name);
//       if (w)
//          break;
//    }
//    if (children) XFree ((char *)children);
//    return(w);


// }




RFBFindWindow::RFBFindWindow(int parent_window_id, int *child_window_id, RFBConnection *parent)
{
   local_parent_window_id = parent_window_id;
   local_child_window_id = child_window_id;
   if (parent !=NULL)
   {
      local_parent = parent;
   }
   else
   {
      local_parent = NULL;
   }

   //wxMessageBox(wxString::Format(wxT("%i"), parent_window_id));
}


void *RFBFindWindow::Entry()
{
   //wxMessageBox(wxT("Thread"));
   //wxThread::Sleep(300);

   int pause = 10;
   while ( *local_child_window_id == 0  )
   {
      //time_out -= pause;
      *local_child_window_id = display_tree_info_1(local_parent_window_id, 1, 0);
      //std::cout << "THREAD" << std::endl;
      //wxMessageBox(wxT("Thread"));
      wxThread::Sleep(pause * 10);
   }
   wxCommandEvent eventCustom(WXRFB_WINDOW_RFV);
   wxPostEvent(local_parent, eventCustom);
   return NULL;
}

void RFBFindWindow::OnExit()
{
   //wxMessageBox(wxT("Thread exit"));

}



int RFBFindWindow::display_tree_info_1(Window window, int recurse, int level)
{
   Window root_win, parent_win;
   unsigned int num_children = 0;
   Window *child_list;

   Display * dpy = XOpenDisplay(NULL);

   if (!XQueryTree(dpy, window, &root_win, &parent_win, &child_list,
    		   &num_children))
   {
   //wxMessageBox(wxT("EXP"));
    }
   XCloseDisplay(dpy);

   // //wxMessageBox(wxString::Format(wxT("%i, %i"), num_children, num_children));
   // if (level == 0) {
   //    //std::cout << "Root window id:" << std::endl;
   //    //Display_Window_Id(root_win, True);
   //    //std::cout <<  "Parent window id:" << std::endl;
   //    //Display_Window_Id(parent_win, True);
   // }

   // if (level == 0  ||  num_children > 0) {
   //    //printf("     ");
   //    for (j=0; j<level; j++) printf("   ");
   //    //printf("%d child%s%s\n", num_children, num_children == 1 ? "" : "ren",
   //    //    num_children ? ":" : ".");
   //    //std::cout << num_children << num_children  << num_children << std::endl;
   // }
   if (num_children >= 1)
   {
      //turn display_tree_info_1(child_list[(int)num_children - 1], 1, level+1);
      return child_list[(int)num_children - 1];
   }
   else
   {
      return 0;
   }

// //for (i = (int)num_children - 1; i >= 0; i--) {
   // // printf("     ");
   // // for (j=0; j<level; j++) printf("   ");
   // // Display_Window_Id(child_list[i], False);
   // // printf(": (");
   // // if(XGetClassHint(dpy, child_list[i], &classhint)) {
   // // 	 if(classhint.res_name) {
   // //       printf("\"%s\" ", classhint.res_name);
   // //       XFree(classhint.res_name);
   // // 	 } else
   // //       printf("(none) ");
   // // 	 if(classhint.res_class) {
   // //       printf("\"%s\") ", classhint.res_class);
   // //       XFree(classhint.res_class);
   // // 	 } else
   // //       printf("(none)) ");
   // // } else
   // // 	 printf(") ");

   // // if (XGetGeometry(dpy, child_list[i], &root_win,
   // //                  &rel_x, &rel_y, &width, &height, &border, &depth)) {
   // //     Window child;

   // //     printf (" %ux%u+%d+%d", width, height, rel_x, rel_y);
   // //     if (XTranslateCoordinates (dpy, child_list[i], root_win,
   // //                                0 ,0, &abs_x, &abs_y, &child)) {
   // //         printf ("  +%d+%d", abs_x - border, abs_y - border);
   // //     }
   // // }
   // // printf("\n");
   // if (recurse)
   //
}
