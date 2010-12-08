///////////////////////////////////////////////////////////////////////////////
// File name:   FRDPConnection_nix.cpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-12-07 20:18:01"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include "FRDPConnection_nix.hpp"
#include "FRDPKeyboard.hpp"
#include <wx/stattext.h>

BEGIN_EVENT_TABLE(FRDPConnection, wxWindow)
EVT_IDLE(FRDPConnection::OnIdle)
EVT_TIMER(wxID_ANY, FRDPConnection::OnTimer)
EVT_SET_FOCUS(FRDPConnection::on_set_focus)
EVT_MOUSE_EVENTS(FRDPConnection::on_any_mouse_event)
EVT_COMMAND(wxID_ANY, ID_XRDP_FINDED_WINDOW, FRDPConnection::ReparetnClientWindow)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxContFRDP,wxWindow)
EVT_SET_FOCUS(wxContFRDP::on_set_focus)
EVT_KILL_FOCUS(wxContFRDP::on_kill_focus)
EVT_MOUSE_EVENTS(wxContFRDP::on_mouse_events)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(FRDPFullScreen,wxTopLevelWindow)
EVT_TOOL(-1,FRDPFullScreen::on_toolbar)
EVT_CHAR_HOOK(FRDPFullScreen::dialog_hotkeys)
EVT_KEY_DOWN(FRDPFullScreen::dialog_hotkeys)
END_EVENT_TABLE()


FRDPFullScreen::FRDPFullScreen(MainFrame * main, wxContFRDP * cnt, Options_HashMap options):
wxTopLevelWindow(NULL,wxID_ANY,wxT(""),wxDefaultPosition,wxDefaultSize)
{
   rdp = (FRDPConnection *)cnt->GetParent();
   cnt_old = cnt;
   local_options = options;
   win = NULL;
   win2 = NULL;
   tb = NULL;
   SetSize(wxGetDisplaySize());
   Show();
   win = new wxScrolledWindow(this,-1,wxPoint(0,0),GetClientSize());
   ShowFullScreen(true);
   win->FitInside();
   win2 = new ToolbarCnt(this);
   if (main != NULL)
   {
      main_frame = main;
   } else {
      main_frame = NULL;
   }


   int heigth = wxAtoi(options[wxT("heigth")]);
   int width = wxAtoi(options[wxT("width")]);
   wxString connection_name, control_size, full_screen;

   if (local_options[wxT("connection_name")] != wxT(""))
   {
      connection_name = local_options[wxT("connection_name")];
   } else {
      connection_name = local_options[wxT("hostname")];
   }

   tb = new FullScreenToolBar(main_frame, connection_name,win2,-1,wxPoint(100,100),wxSize(200,22));

   win2->fs = tb;

   GtkWidget * widget_fullscreen = win->m_wxwindow;
   GtkWidget * widget_cont = cnt->m_wxwindow;
   gtk_container_add(GTK_CONTAINER(widget_fullscreen),widget_cont);
   gtk_widget_reparent(widget_cont,widget_fullscreen);
   wxSize fs = wxGetDisplaySize();

   if (wxAtoi(options[wxT("control_size")]))
   {
      FRDPConnection * rdp_temp = (FRDPConnection *)cnt->GetParent();
      wxSize sz = rdp_temp->GetVirtualSize();
      gtk_widget_set_size_request(widget_cont,sz.x,sz.y);
   }
   else if (wxAtoi(options[wxT("full_screen")]))
   {
      wxSize disp_size = wxGetDisplaySize();
      gtk_widget_set_size_request(widget_cont, disp_size.x, disp_size.y);
   }
   else
   {
      gtk_widget_set_size_request(widget_cont,width,heigth);
   }

   if (!wxAtoi(options[wxT("control_size")]))
   {
      if ((width > fs.x) || (heigth > fs.y))
      {
	 if (!wxAtoi(full_screen))
	 {
	    win->SetScrollbars(1,1,width,heigth);
	 }

      }
      else  ////////////////
      {
	 int rdp_x = 0;
	 int rdp_y = 0;
	 gtk_widget_get_size_request(widget_cont,&rdp_x,&rdp_y);
	 win->SetSize(wxSize(rdp_x,rdp_y));
      }     ////////

   }
   else
   {
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



FRDPFullScreen::~FRDPFullScreen()
{
	if (win)
	{
		delete win;
		win = NULL;
	}
	if (tb)
	{
		delete tb;
		tb = NULL;
	}

	if (win2)
	{
		delete win2;
		win2 = NULL;

	}




}

void FRDPFullScreen::dialog_hotkeys(wxKeyEvent &event)
{
	wxCommandEvent evt;
	evt.SetId(1);
	switch(event.GetKeyCode())
	{
		case WXK_RETURN + WXK_ALT + WXK_CONTROL:

			break;
		case WXK_ESCAPE:
			//this->button_cancel_func(evt);
			break;

			// case WXK_TAB:
			// 	 if (event.GetModifiers() == wxMOD_SHIFT)
			// 	 {
			// 	    if (notebook)
			// 	    {
			// 	       size_t iSel = (notebook->GetSelection() + 1) % (notebook->GetPageCount());
			// 	       notebook->SetSelection(iSel);
			// 	    }
			// 	 }
			// 	 break;
		default:

			break;

	}
	event.Skip();
}

void FRDPFullScreen::Leave()
{
	ShowFullScreen(false);

	FRDPConnection * rdp = (FRDPConnection *)cnt_old->GetParent();
	GtkWidget * rdp_w = rdp->m_wxwindow;
	GtkWidget * rdp_c = cnt_old->m_wxwindow;
	int heigth = wxAtoi(local_options[wxT("heigth")]);
	int width = wxAtoi(local_options[wxT("width")]);
	wxString connection_name, control_size, full_screen;
	// for (int i = 0 ; i < local_options.Count(); i++)
	// {
	//    if (local_options.Item(i).Item(0).name == wxT("connection_name"))
	//    {
	// 	 connection_name = local_options.Item(i).Item(0).value;
	//    }
	//    if (local_options.Item(i).Item(0).name == wxT("control_size"))
	//    {
	// 	 control_size = local_options.Item(i).Item(0).value;
	//    }
	//    if (local_options.Item(i).Item(0).name == wxT("full_screen"))
	//    {
	// 	 full_screen = local_options.Item(i).Item(0).value;
	//    }
	//    if (local_options.Item(i).Item(0).name == wxT("heigth"))
	//    {
	// 	 heigth = wxAtoi(local_options.Item(i).Item(0).value);
	//    }
	//    if (local_options.Item(i).Item(0).name == wxT("width"))
	//    {
	// 	 width = wxAtoi(local_options.Item(i).Item(0).value);
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
	// 	 local_options.Item(j).Item(0).value = wxT("0");
	//    }
	// }

	rdp->bFullScreen = FALSE;
	rdp->conn_splitter->m_timer_screenshot.Start();
	Close();
	Destroy();
}


void FRDPFullScreen::on_toolbar(wxCommandEvent& event)
{
	switch(event.GetId())
	{
		case ID_FULLSCREEN_PHOTO:
			rdp->photo();
			break;

		case ID_FULLSCREEN_SENDCAD:
			rdp->SendKey(TRUE);
			break;

		case ID_FULLSCREEN_GRABINPUT:
			if (tb->GetToolToggled(ID_FULLSCREEN_GRABINPUT))
			{
				rdp->GrabAll(TRUE);
			}
			else
			{
				rdp->GrabAll(FALSE);
			}
			break;

		case ID_FULLSCREEN_CLOSE:
			Leave();
			rdp->request_close();
			break;

		case ID_FULLSCREEN_LEAVE:
			Leave();
			break;
		case ID_FULLSCREEN_TABSMENU:
			PopupMenu(tb->EstablishedConnectionsMenu,
					  tb->GetScreenPosition().x + tb->GetToolRect(ID_FULLSCREEN_TABSMENU).GetBottomLeft().x,
					  tb->GetToolRect(ID_FULLSCREEN_TABSMENU).GetBottomLeft().y + 3);

			break;
		case ID_FULLSCREEN_CONNTREE:
			PopupMenu(tb->ConnectionsMenu,
					  tb->GetScreenPosition().x + tb->GetToolRect(ID_FULLSCREEN_CONNTREE).GetBottomLeft().x,
					  tb->GetToolRect(ID_FULLSCREEN_CONNTREE).GetBottomLeft().y + 3);

			break;

		default:
			break;

	}
}


FRDPConnection::FRDPConnection(MainFrame *m_frame,Options_HashMap all_options,
			       wxWindow *parent, wxWindowID id,
			       const wxPoint& pos,
			       const wxSize& size,
			       long style,
			       const wxString& name)
   :BasicConnection(m_frame,all_options,parent,id,pos,size,0,name),
    windowSercherThread(NULL)
{
   wxFile temp;
   GETFREERDPPATH();

   if ((!FREERDPPATH.IsEmpty()))
      //&& (temp.Exists(FREERDPPATH)))
   {
      wxrdp_status = WXFRDP_OK;
   }
   else
   {
      Error_mgs = _("Can't found \"xfreerdp\" file.\nPlease press Alt+O, and edit patch to \"xfreerdp\" executable file");
      wxMessageBox(Error_mgs);
      wxrdp_status = FRDP_RDESKTOP_NOT_FOUND;
      return;
   }
   local_options = all_options;
   sock = NULL;
   bClosing = FALSE;

   fullscreen_frame = NULL;
   lock = NULL;
   lock_cnt = NULL;
   cnt = NULL;
   Info.Clear();
   info_uniq_name = 0;
   conn_splitter = (ConnSplitter *)parent;

   GtkWidget *widget = this->m_wxwindow;
   cnt = new wxContFRDP(this,-1,wxPoint(0,0),wxDefaultSize);
   gtk_widget_realize( widget );
   GdkWindow *gdkWin = GTK_PIZZA (widget)->bin_window;
   Window wid = GDK_WINDOW_XWINDOW(gdkWin);
   xid =  wid;
   bObjectOk = TRUE;

}
FRDPConnection::~FRDPConnection()
{
	if (cnt) { delete cnt; cnt = NULL;	}
	if (windowSercherThread){delete windowSercherThread; windowSercherThread = NULL;}
}

void FRDPConnection::CentreFRDP()
{
	CheckWindow();

	wxSize sz_cnt = cnt->GetSize();
	wxSize sz_this = conn_splitter->GetClientSize();
	cnt->SetPosition(wxPoint(0,0));

	if ((sz_cnt.x < sz_this.x) || (sz_cnt.y < sz_this.y))
	{
		cnt->CenterOnParent();
	}
}

void FRDPConnection::photo()
{
	wxClientDC client_dc(this->cnt);
	wxBitmap bmp(client_dc.GetSize().GetWidth(), client_dc.GetSize().GetHeight());
	wxMemoryDC mdc(bmp);
	mdc.Blit(0, 0, bmp.GetWidth(), bmp.GetHeight(), &client_dc, 0, 0);
	mdc.SelectObject(wxNullBitmap);

	wxFileDialog * savefiledialog = new wxFileDialog(this,_("Save screenshot"),wxT(""),wxT(""),wxT("*.bmp"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	int iRes;
	iRes = savefiledialog->ShowModal();
	if (iRes == wxID_CANCEL){ delete savefiledialog; return;}


	wxString filepath = savefiledialog->GetPath();
	if (!filepath.IsEmpty())
	{
	   bmp.SaveFile(filepath + wxT(".bmp"),wxBITMAP_TYPE_BMP);
	}
	delete savefiledialog;

}

void FRDPConnection::FullScreen(BOOL_L bRestore)
{
	if (bRestore)
	{
		fullscreen_frame = new FRDPFullScreen(main_frame, (wxContFRDP *)cnt,options);
		if (fullscreen_frame)
		{
			conn_splitter->m_timer_screenshot.Stop();
			bFullScreen = TRUE;
		}
	}
}


void FRDPConnection::SendKey(BOOL_L cad)
{
//	GtkWidget *widget = main_frame->m_wxwindow;
	GtkWidget *widget = cnt->m_wxwindow;

	GdkWindow *gdkWin = GTK_PIZZA (widget)->bin_window;
	Window wid = GDK_WINDOW_XWINDOW(gdkWin);
	Display * disp = XOpenDisplay(NULL);

	Window cur_focus;
	XKeyEvent event;

	cur_focus = wid;

	event.display = disp;
	event.window = cur_focus;
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


void FRDPConnection::SendXKeyEvent(XKeyEvent * event)
{
	XSync(event->display, FALSE);
	XSendEvent(event->display, event->window, TRUE, KeyPressMask, (XEvent *)event);
	XSync(event->display, FALSE);
}

void FRDPConnection::GrabAll(BOOL_L state)
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




wxContFRDP::wxContFRDP(wxWindow * parent,wxWindowID id,const wxPoint& point,const wxSize& size):
	wxWindow(parent,id,point,size)
{
}


void wxContFRDP::on_set_focus(wxFocusEvent& event)
{
	FRDPConnection * rdp = (FRDPConnection *)GetParent();
	if (rdp->bConnected)
	{
		rdp->CheckFocus();
	}

	event.Skip();
}

void wxContFRDP::on_kill_focus(wxFocusEvent& event)
{
	FRDPConnection * rdp = (FRDPConnection *)GetParent();
	if (rdp->bConnected)
	{
		rdp->EnableFocus(FALSE);
	}

	event.Skip();


}

void wxContFRDP::on_mouse_events(wxMouseEvent& event)
{
	FRDPConnection * rdp = (FRDPConnection *)GetParent();
	if (event.Entering())
	{
		rdp->CheckFocus();
	}
}




void FRDPConnection::EnableFocus(BOOL_L state)
{
	if (state)
	{
		lock = new wxWindowUpdateLocker(this);
		lock_cnt = new wxWindowUpdateLocker(cnt);

		gtk_widget_set_can_focus(sock,true);
		gdk_window_focus(gtk_widget_get_window(sock),GDK_CURRENT_TIME);
		gtk_widget_grab_focus (sock);


		SendKey();
		CentreFRDP();


	}
	else
	{
		gtk_widget_set_can_focus(sock,false);
		gdk_keyboard_ungrab (GDK_CURRENT_TIME);

		if (lock) { delete lock; lock = NULL;}
		if (lock_cnt) { delete lock_cnt; lock_cnt = NULL;}
	}
}


void FRDPConnection::on_set_focus(wxFocusEvent& event)
{
	if (bConnected)
	{
		CheckFocus();
	}

	event.Skip();

}

void FRDPConnection::on_leave_focus(wxFocusEvent& event)
{

	if (bConnected)
	{
		EnableFocus(FALSE);
	}

	event.Skip();


}

void FRDPConnection::CheckFocus()
{
	gboolean bFocus = gtk_widget_is_focus(GTK_WIDGET(sock));
	if (!bFocus)
	{

		if (bGrab)
		{
			GrabAll(TRUE);
		}
		else
		{
			EnableFocus(TRUE);
		}
	}
}


void FRDPConnection::on_any_mouse_event(wxMouseEvent& event)
{
	if (event.Entering())
	{
		CheckFocus();
	}

}

void FRDPConnection::CheckWindow()
{
	wxSize cnt_size = cnt->GetSize();

	wxSize rdp_size = GetSize();
	if (rdp_size.x < cnt_size.x || rdp_size.y < cnt_size.y)
	{
		SetScrollbars(1,1,cnt_size.x,cnt_size.y);
	}
	else
	{
		FitInside();
		Refresh();


	}

	Update();

}



void FRDPConnection::wait_exit(GPid pid, gint status, gpointer data)
{
	FRDPConnection * rdp = (FRDPConnection *)data;
	g_spawn_close_pid (pid);
	rdp->bConnected = false;

	rdp->SendDisconnectEvent();

	if (status != 0 && status != SIGTERM)
	{
		char * buffer = new char[4096];
		memset(buffer,0,4096);
		int len;

	    lseek (rdp->error_fd, 0, SEEK_SET);
	    len = read (rdp->error_fd, buffer, 4095);
		if (len > 0)
		{
			buffer[len] = '\0';
			wxString error_string;
			error_string = wxString::From8BitData(buffer,len);

			//// TEST
			rdp->EnableFocus(FALSE);


			wxMessageBox(error_string,_("Error"),wxICON_ERROR);
		}
		delete buffer;
	}

	close(rdp->output_fd);
	close(rdp->error_fd);

	if (rdp->bFullScreen && rdp->fullscreen_frame)
	{
		rdp->fullscreen_frame->Leave();
	}
	rdp->fullscreen_frame = NULL;


	if (!rdp->bClosing)
	{
		wxCommandEvent evt;
		evt.SetId(ID_CONNECTION_STATUS_UPDATE);
		rdp->main_frame->connectionStatusUpdate(evt);
	}
	rdp->sock_id = 0;
	if (rdp->sock)
	{
		gtk_widget_destroy(rdp->sock);
		rdp->sock = NULL;
	}
	if (rdp->bClosing)
	{
		int lc = rdp->main_frame->nb->GetPageCount();
		if (lc > 0)
		{
			for (int i = 0; i < lc ; i ++)
			{
				if (rdp->conn_splitter == (ConnSplitter *)rdp->main_frame->nb->GetPage(i))
				{
					rdp->main_frame->current_page_for_delete = i;
					wxUpdateUIEvent evt;
					evt.Check(true);
					evt.SetId(ID_ERRORHOSTNAME);
					wxPostEvent(rdp->main_frame,evt);
					rdp->bClosing = FALSE;
					return ;
				}

			}
		}
	}
	rdp->bClosing = FALSE;
}




bool FRDPConnection::DoFRdp()
{
   cnt->SetPosition(wxPoint(0,0));
   wxString hostname, port;
   bool ret = false;
   wxSize rdp_size;
   int localWidth;
   int localHeight;
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
   sock_id =  gtk_socket_get_id(GTK_SOCKET(sock));

   gchar *argv[50];
   int argc = 0;

//FIXME: change to real path to rdesktop
   GETFREERDPPATH();
   wxDir temp(FREERDPPATH);
   if (!FREERDPPATH.IsEmpty())
   {
      argv[argc++] = g_strdup (FREERDPPATH.fn_str());
      if (!local_options[wxT("username")].IsEmpty())
      {
	 argv[argc++] = g_strdup("-u");
	 argv[argc++] = g_strdup (local_options[wxT("username")].ToUTF8());
      }
      if (!local_options[wxT("domain")].IsEmpty())
      {
	 argv[argc++] = g_strdup("-d");
	 argv[argc++] = g_strdup(local_options[wxT("domain")].ToUTF8());
      }
      if (!local_options[wxT("shell")].IsEmpty())
      {
	 argv[argc++] = g_strdup("-s");
	 argv[argc++] = g_strdup(local_options[wxT("shell")].ToUTF8());
      }
      if (!local_options[wxT("directory")].IsEmpty())
      {
	 argv[argc++] = g_strdup(" -c ");
	 argv[argc++] = g_strdup(local_options[wxT("directory")].ToUTF8());
      }
      if (!local_options[wxT("password")].IsEmpty())
      {
	 argv[argc++] = g_strdup("-p");
	 argv[argc++] = g_strdup(local_options[wxT("password")].ToUTF8());
      }
      if (wxAtoi(local_options[wxT("clipboard_sync")]))
      {
	 argv[argc++] = g_strdup("--plugin");
	 argv[argc++] = g_strdup("cliprdr");
      }
      wxString kbdId;
      FRDPKeyboard kbdList;
      //int kbdListCount = 0;
      if (kbdList.Init())
      {
//	 kbdListCount = kbdList.GetCount();
//	 for (int index = 0; index <= kbdListCount; ++index)
//	 {
//	    if (!kbdList.GetKeyNameByIndex(index).IsEmpty())
	 kbdId = kbdList.GetKeyNameByValue(local_options[wxT("keyboard")]);

//	 }
      }
      if (!kbdId.IsEmpty())
      {
	 argv[argc++] = g_strdup("-k");
	 argv[argc++] = g_strdup(kbdId.ToUTF8());
      }
      // if (local_options[wxT("keyboard_map")].IsEmpty() == false)
      // {
      // 	 GETRDESKTOPMAPPATH();
      // 	 wxArrayString list_keyboard_map;
      // 	 wxString temp_str;
      // 	 wxDir temp(RDESKTOPMAPPATH);
      // 	 if ((RDESKTOPMAPPATH != wxT("")) && (temp.Exists(RDESKTOPMAPPATH)))
      // 	 {
      // 	    bool check = temp.GetFirst(&temp_str);
      // 	    list_keyboard_map.Add( wxT("en-us"));
      // 	    while (check == true)
      // 	    {
      // 	       if ((temp_str != wxT("en-us")) &&
      // 		   (temp_str != wxT("common")) &&
      // 		   (temp_str != wxT("modifiers")))
      // 	       {
      // 		  list_keyboard_map.Add(temp_str);
      // 	       }
      // 	       check = temp.GetNext(&temp_str);
      // 	    }
      // 	 }
      // 	 else
      // 	 {
      // 	    list_keyboard_map.Add( wxT("en-us"));
      // 	    list_keyboard_map.Add( wxT("ru-ru"));
      // 	 }
      // 	 argv[argc++] = g_strdup("-k");
      // 	 argv[argc++] = g_strdup(list_keyboard_map[wxAtoi(local_options[wxT("keyboard_map")])].ToUTF8());
      // }
      if (wxAtoi(local_options[wxT("control_size")]))
      {
      	 int width = 0;
      	 int height = 0;
      	 conn_splitter->GetSize(&width, &height);
      	 cnt->SetSize(wxSize(width,height));
      	 gtk_widget_set_size_request (GTK_WIDGET (sock), width,height);
      	 if ((width >= 0) && (height >= 0))
      	 {
      	    argv[argc++] = g_strdup ("-g");
      	    argv[argc++] = g_strdup_printf ("%ix%i", width, height);
	    localWidth	 = width;
	    localHeight	 = height;
      	 }
      	 else
      	 {
      	    return false;
      	 }
      }
      else if (wxAtoi(local_options[wxT("full_screen")]))
      {
      	 wxSize disp_size = wxGetDisplaySize();
      	 argv[argc++] = g_strdup ("-g");
      	 argv[argc++] = g_strdup_printf ("%ix%i", disp_size.x, disp_size.y);

      	 cnt->SetSize(disp_size);
      	 if (rdp_size.x < disp_size.x || rdp_size.y < disp_size.y)
      	 {
      	    SetScrollbars(1,1,disp_size.x,disp_size.y);
      	 }
      	 gtk_widget_set_size_request (GTK_WIDGET (sock), disp_size.x, disp_size.y);
	 localWidth	 = disp_size.x;
	 localHeight	 = disp_size.y;

      }
      else if ((wxAtoi(local_options[wxT("width")]) > 0) &&
      	       (wxAtoi(local_options[wxT("heigth")])) > 0)
      {
      	 argv[argc++] = g_strdup ("-g");
      	 argv[argc++] = g_strdup_printf ("%ix%i",  wxAtoi(local_options[wxT("width")]),
      					 wxAtoi(local_options[wxT("heigth")]));
	 localWidth	 = wxAtoi(local_options[wxT("width")]);
	 localHeight	 = wxAtoi(local_options[wxT("heigth")]);

      	 cnt->SetSize(wxSize(wxAtoi(local_options[wxT("width")]),
      			     wxAtoi(local_options[wxT("heigth")])));
      	 if ((rdp_size.x < wxAtoi(local_options[wxT("width")])) ||
      	     (rdp_size.y < wxAtoi(local_options[wxT("heigth")])))
      	 {
      	    SetScrollbars(1,1,wxAtoi(local_options[wxT("width")]),
      			  wxAtoi(local_options[wxT("heigth")]));
      	 }
      	 gtk_widget_set_size_request (GTK_WIDGET (sock), wxAtoi(local_options[wxT("width")]),
      				      wxAtoi(local_options[wxT("heigth")]));
      }
      // if (wxAtoi(local_options[wxT("force_update_screen")]))
      // {
      // 	 argv[argc++] = g_strdup("-f");
      // }
      // if (wxAtoi(local_options[wxT("backing_store")]))
      // {
      // 	 argv[argc++] = g_strdup("-B");
      // }
      // if (wxAtoi(local_options[wxT("encription_enable_french")]))
      // {
      // 	 argv[argc++] = g_strdup("-e");
      // }
      // if (wxAtoi(local_options[wxT("encription_enable_new")]))
      // {
      // 	 argv[argc++] = g_strdup("-E");
      // }
      // if (wxAtoi(local_options[wxT("send_mouse_event")]))
      // {
      // 	 argv[argc++] = g_strdup("-m");
      // }
      // if (wxAtoi(local_options[wxT("private_color_map")]))
      // {
      // 	 argv[argc++] = g_strdup("-C");
      // }
      // if (wxAtoi(local_options[wxT("settings->numlock_sync")]))
      // {
      // 	 argv[argc++] = g_strdup("-N");
      // }
      // if (wxAtoi(local_options[wxT("color_depth")]) == 0)
      // {
      // 	 argv[argc++] = g_strdup("-a");
      // 	 argv[argc++] = g_strdup_printf(("%i"), 8);
      // }
      // else if (wxAtoi(local_options[wxT("color_depth")]) == 1)
      // {
      // 	 argv[argc++] = g_strdup("-a");
      // 	 argv[argc++] = g_strdup_printf(("%i"), 15);
      // }
      // else if (wxAtoi(local_options[wxT("color_depth")]) == 2)
      // {
      // 	 argv[argc++] = g_strdup("-a");
      // 	 argv[argc++] = g_strdup_printf(("%i"), 16);
      // }
      // else if (wxAtoi(local_options[wxT("color_depth")]) == 3)
      // {
      // 	 argv[argc++] = g_strdup("-a");
      // 	 argv[argc++] = g_strdup_printf(("%i"), 24);
      // }
      // else
      // {
      // 	 argv[argc++] = g_strdup("-a");
      // 	 argv[argc++] = g_strdup_printf(("%i"), 8);
      // }
      // if (wxAtoi(local_options[wxT("enable_compres")]))
      // {
      // 	 argv[argc++] = g_strdup("-z");
      // }
      // if (wxAtoi(local_options[wxT("bandwidth")]) == 0 ||
      // 	  wxAtoi(local_options[wxT("bandwidth")]) == 1)
      // {
      // 	 argv[argc++] = g_strdup("-x");
      // 	 argv[argc++] = g_strdup("m");
      // }
      // else if (wxAtoi(local_options[wxT("bandwidth")]) == 2)
      // {
      // 	 argv[argc++] = g_strdup("-x");
      // 	 argv[argc++] = g_strdup("b");
      // }
      // else if (wxAtoi(local_options[wxT("bandwidth")]) == 3)
      // {
      // 	 argv[argc++] = g_strdup("-x");
      // 	 argv[argc++] = g_strdup("l");
      // }

      // if (wxAtoi(local_options[wxT("enable_bitmap_caching")]))
      // {
      // 	 argv[argc++] = g_strdup("-P");
      // }
      // if (wxAtoi(local_options[wxT("attach_to_console")]))
      // {
      // 	 argv[argc++] = g_strdup("-0");
      // }
      // if (wxAtoi(local_options[wxT("use_rdp_version")]) == 1)
      // {
      // 	 argv[argc++] = g_strdup("-5");
      // }
      // else if (wxAtoi(local_options[wxT("use_rdp_version")]) == 0)
      // {
      // 	 argv[argc++] = g_strdup("-4");
      // }
      if (wxAtoi(local_options[wxT("sound_type")]) == 0)
      {
      	 argv[argc++] = g_strdup ("--plugin");
	 argv[argc++] = g_strdup ("rdpsnd");
      }
      else if (wxAtoi(local_options[wxT("sound_type")]) == 1)
      {
      	 argv[argc++] = g_strdup ("-o");
      }
      //FIXME: Add parser for plugins redirect devices
      if (!local_options[wxT("linux_devices")].IsEmpty())
      {
	 //argv[argc++] =
	 //g_strdup(local_options[wxT("linux_devices")].ToUTF8());
	 // argv[argc++] = g_strdup("--plugin");
	 // argv[argc++] = g_strdup("rdpdr");
	 // argv[argc++] = g_strdup("disk:Projects://home//stress//Projects//");
      }
      if ((local_options[wxT("port")].IsEmpty() == false) &&
	  (local_options[wxT("hostname")].IsEmpty() == false))
      {
	 hostname = local_options[wxT("hostname")] + wxT(":") + local_options[wxT("port")];
	 argv[argc++] = g_strdup (hostname.ToUTF8());
      }
      else if (local_options[wxT("hostname")].IsEmpty() == false)
      {
	 hostname = local_options[wxT("hostname")];
	 argv[argc++] = g_strdup (hostname.ToUTF8());
      }

      // argv[argc++] = g_strdup ("-X");
      //argv[argc++] = g_strdup_printf ("%i", sock_id);
      argv[argc++] = NULL;

      if (!local_options[wxT("hostname")].IsEmpty())
      {

	 GSpawnFlags spawn_flags =  (GSpawnFlags)(G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD);
	 ret = g_spawn_async_with_pipes (NULL,argv,NULL,spawn_flags,NULL,
					 NULL,&sock_id,NULL,&output_fd, &error_fd,NULL);
	 if (ret)
	 {
	   CatcherWindowID *windowSercherThread = new CatcherWindowID(this,
								      localWidth,
								      localHeight,
								      ID_XRDP_FINDED_WINDOW);
	    if (windowSercherThread)
	    {
	       windowSercherThread->Create();
	       windowSercherThread->Run();
	    }

	    bConnected = true;

	    g_child_watch_add (sock_id, FRDPConnection::wait_exit, this);

	    SendConnectEvent();
	    wxCommandEvent eventCustom(ID_CONNECTION_STATUS_UPDATE);
	    wxPostEvent(main_frame, eventCustom);
	 }
	 gtk_widget_show(sock);
	 CentreFRDP();
	 for (int i = 0; i < argc ; i++)
	 {
	    g_free(argv[i]);
	 }

      }
   }
   return ret;
}


wxString FRDPConnection::get_error_string()
{
	return Error_mgs;
}
void FRDPConnection::drop_error_string()
{
	Error_mgs.Clear();
	Error_mgs = wxT("");
}



void FRDPConnection::close_rdesktop_prg(wxCommandEvent& event)
{
	wxCommandEvent eventCustom(ID_CLOSE_RDESKTOP_PRG);
	wxPostEvent(main_frame, eventCustom);
}

bool FRDPConnection::request_close(BOOL_L bDetach)
{
	if (sock_id)
	{
		EnableFocus(FALSE);
		bClosing = bDetach;
		kill(sock_id,SIGTERM);
	}
	return true;
}

void FRDPConnection::event_dispatcher(wxCommandEvent& event)
{

}

void FRDPConnection::OnIdle(wxIdleEvent& event)
{

}


void FRDPConnection::OnTimer(wxTimerEvent& event)
{

}

bool FRDPConnection::get_state()
{
	return bConnected;
}

bool FRDPConnection::get_proc_state()
{
	return wxrdp_status;
}


void FRDPConnection::set_state(bool state_to_set)
{
	bConnected = state_to_set;
}

void FRDPConnection::set_proc_state(bool state_to_set)
{
	wxrdp_status = state_to_set;
}


void FRDPConnection::rdesktop_not_found(wxCommandEvent& event)
{

}

int FRDPConnection::get_state_wxrdp()
{
	return 0;
}




bool FRDPConnection::DoConnection()
{
	return DoFRdp();
}

bool FRDPConnection::Connect()
{
	return DoFRdp();

}

void FRDPConnection::Disconnect()
{
	request_close();

}


bool FRDPConnection::DisconnectClose()
{
	return request_close(TRUE);

}

void FRDPConnection::CentreConnection()
{
	if (cnt != NULL)
		CentreFRDP();

}

void FRDPConnection::ReparetnClientWindow(wxCommandEvent& event)
{
   Display * dpy = XOpenDisplay(NULL);
   XReparentWindow(dpy, event.GetExtraLong(), gtk_socket_get_id((GtkSocket *)sock), 0, 0);
   XMapWindow(dpy,event.GetExtraLong());
   XSync(dpy,0);
   XCloseDisplay(dpy);
}





