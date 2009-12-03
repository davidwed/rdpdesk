#include "wxRDP_nix.hpp"
#include <wx/stattext.h>

FullScreenToolBar::FullScreenToolBar(wxString caption, wxWindow * parent, wxWindowID id, const wxPoint& point, const wxSize& size):
	wxAuiToolBar(parent,id,point,size, wxAUI_TB_DEFAULT_STYLE )
{
	SetToolBitmapSize(wxSize(8,8));
	m_caption = new wxStaticText(this,-1,caption,wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);

	AddTool(ID_FULLSCREEN_AUTOHIDE,LOAD_XPM(media_eject),NULL,true,NULL,wxT("Auto hide"));
	AddControl(m_caption,wxT(""));
	AddSeparator();
	AddTool(ID_FULLSCREEN_PHOTO,wxT("Make screenshot"),LOAD_XPM(camera_photo16),wxT("Make screnshot"));
	AddTool(ID_FULLSCREEN_GRABINPUT,LOAD_XPM(input_keyboard16),NULL,true,NULL,wxT("Grab Input"));
	
	AddTool(ID_FULLSCREEN_SENDCAD,wxT("Send Ctrl+Alt+Del"),LOAD_XPM(go_down16),wxT("Send Ctrl+Alt+Del"));
	AddTool(ID_FULLSCREEN_LEAVE,wxT("Leave full screen"),LOAD_XPM(preferences_system_windows),wxT("Leave full screen"));
	
	AddTool(ID_FULLSCREEN_CLOSE,wxT("Disconnect"),LOAD_XPM(emblem_unreadable),wxT("Disconnect"));
	
	SetPosition(wxPoint(0,0));
	CenterOnParent(wxHORIZONTAL);
	 
	Realize();
}

FullScreenToolBar::~FullScreenToolBar()
{
	delete m_caption;
}


RDPFullScreen::RDPFullScreen(wxContRDP * cnt,RDPConn rdpconn):
	wxTopLevelWindow(NULL,-1,wxT(""),wxDefaultPosition,wxDefaultSize)
{
	rdp = (wxRDP *)cnt->GetParent();
	cnt_old = cnt;
	rdpc = rdpconn;
	win = NULL;
	win2 = NULL;
	tb = NULL;
	
	SetSize(wxGetDisplaySize());
	Show();
	win = new wxScrolledWindow(this,-1,wxPoint(0,0),GetClientSize());
	ShowFullScreen(true);
//	win->SetSize(GetClientSize());
	win->FitInside();
	
	win2 = new ToolbarCnt(this);
	
	tb = new FullScreenToolBar(rdpc.connection_name,win2,-1,wxPoint(100,100),wxSize(200,22));

	win2->fs = tb;

	GtkWidget * widget_fullscreen = win->m_wxwindow;
	GtkWidget * widget_cont = cnt->m_wxwindow;
	gtk_container_add(GTK_CONTAINER(widget_fullscreen),widget_cont);
	gtk_widget_reparent(widget_cont,widget_fullscreen);

	wxSize fs = wxGetDisplaySize();

	if (rdpc.bControlSize)
	{
		wxRDP * rdp_temp = (wxRDP *)cnt->GetParent();
		wxSize sz = rdp_temp->GetVirtualSize();
		gtk_widget_set_size_request(widget_cont,sz.x,sz.y);
	}
	else if (rdpc.bFullScreen)
	{
		wxSize disp_size = wxGetDisplaySize();
		gtk_widget_set_size_request(widget_cont, disp_size.x, disp_size.y);
	}
	else
	{
		gtk_widget_set_size_request(widget_cont,rdpc.width,rdpc.heigth);
	}
	
	if (!rdpc.bControlSize)
	{
		if ((rdpc.width > fs.x) || (rdpc.heigth > fs.y))
		{
			if (!rdpc.bFullScreen)
			{
				win->SetScrollbars(1,1,rdpc.width,rdpc.heigth);
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

	Update();
	
}



RDPFullScreen::~RDPFullScreen()
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

void RDPFullScreen::Leave()
{
	ShowFullScreen(false);

	wxRDP * rdp = (wxRDP *)cnt_old->GetParent();
	GtkWidget * rdp_w = rdp->m_wxwindow;
	GtkWidget * rdp_c = cnt_old->m_wxwindow;

	gtk_widget_reparent(rdp_c,rdp_w);
	if (rdpc.bControlSize)
	{
		wxSize sz = rdp->GetVirtualSize();
		gtk_widget_set_size_request(rdp_c,sz.x,sz.y);
	}
	else if (rdpc.bFullScreen)
	{
		wxSize disp_size = wxGetDisplaySize();
		gtk_widget_set_size_request(rdp_c, disp_size.x, disp_size.y);
	}
	else
	{
		gtk_widget_set_size_request(rdp_c,rdpc.width,rdpc.heigth);
	}
	rdp->bFullScreen = FALSE;
	rdp->splitter_rdp->m_timer_screenshot.Start();
	Close();
	Destroy();
	
}


void RDPFullScreen::on_toolbar(wxCommandEvent& event)
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
	default:
		break;
		
	}
}

void ToolbarCnt::on_enter(wxMouseEvent& event)
{
	SetPosition(wxPoint(0,0));
	CenterOnParent(wxHORIZONTAL);
	
}

void ToolbarCnt::on_leave(wxMouseEvent& event)
{
	if (fs->GetToolToggled(ID_FULLSCREEN_AUTOHIDE))
	{
		wxSize size = GetClientSize();
		SetPosition(wxPoint(0,(-1)*size.y + 2));
		CenterOnParent(wxHORIZONTAL);
	}
}

ToolbarCnt::ToolbarCnt(wxWindow * parent):
	wxWindow(parent,-1,wxPoint(0,0),wxSize(200,22))	
{

}


wxRDP::wxRDP(Main_Frame *m_frame,RDPConn rdpsettings ,wxSplitterRDP *parent, wxWindowID id,
			 const wxPoint& pos,
			 const wxSize& size,
			 long style,
			 const wxString& name)
	:wxScrolledWindow (parent, id, pos, size, 0, name)
{
	sock = NULL;
	bClosing = FALSE;

	fullscreen_frame = NULL;
		
	lock = NULL;
	lock_cnt = NULL;
	cnt = NULL;
		
	Info.Clear();
	info_uniq_name = 0;
	main_frame = m_frame;
	bConnected = false;
	bFullScreen = false;
	splitter_rdp = parent;
	rdpconn.pid = 0;

	bGrab = FALSE;
		
	wxFile temp;
	
	GtkWidget *widget = this->m_wxwindow;
	cnt = new wxContRDP(this,-1,wxPoint(0,0),wxDefaultSize);
	gtk_widget_realize( widget );
	GdkWindow *gdkWin = GTK_PIZZA (widget)->bin_window;
	Window wid = GDK_WINDOW_XWINDOW(gdkWin);
	xid =  wid;
	rdp_settings = rdpsettings;
	rdpconn = rdpsettings;
	
	GETRDESKTOPPATH();
	
	if ((RDESKTOPPATH != wxT("")) && (temp.Exists(RDESKTOPPATH)))
	{
		wxrdp_status = WXRDP_OK;
	}
	else
	{
		Error_mgs = wxT("Can't found \"rdesktop\" file.\nPlease press Alt+O, and edit patch to \"rdesktop\" executable file");
		wxrdp_status = RDESKTOP_NOT_FOUND;
	}
}
wxRDP::~wxRDP()
{
//	if (rdp->sock) {gtk_widget_destroy(rdp->sock); rdp->sock = NULL;}
	if (cnt) { delete cnt; cnt = NULL;	}
	
		
}

void wxRDP::CentreRDP()
{
	CheckWindow();
	
	wxSize sz_cnt = cnt->GetSize();
	wxSize sz_this = splitter_rdp->GetClientSize();

	cnt->SetPosition(wxPoint(0,0));
	
	if ((sz_cnt.x < sz_this.x) || (sz_cnt.y < sz_this.y))
	{
		cnt->CenterOnParent();
	}
}

void wxRDP::photo()
{
	wxClientDC client_dc(this->cnt);
	wxBitmap bmp(client_dc.GetSize().GetWidth(), client_dc.GetSize().GetHeight()); 
	wxMemoryDC mdc(bmp); 
	mdc.Blit(0, 0, bmp.GetWidth(), bmp.GetHeight(), &client_dc, 0, 0); 
	mdc.SelectObject(wxNullBitmap);

	wxFileDialog * savefiledialog = new wxFileDialog(this,wxT("Save screenshot"),wxT(""),wxT(""),wxT("*.bmp"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	int iRes;
	iRes = savefiledialog->ShowModal();
	if (iRes == wxID_CANCEL){ delete savefiledialog; return;}


	wxString filepath = savefiledialog->GetPath();
	if (!filepath.IsEmpty())
	{
		bmp.SaveFile(filepath,wxBITMAP_TYPE_BMP); 
	}
	delete savefiledialog;

}

void wxRDP::FullScreen(BOOL bRestore)
{
	if (bRestore)
	{
		//	RDPFullScreen * win = NULL;
		fullscreen_frame = new RDPFullScreen(cnt,rdp_settings);
		if (fullscreen_frame)
		{
			splitter_rdp->m_timer_screenshot.Stop();
			bFullScreen = TRUE;
		}
	}
}


void wxRDP::SendKey(BOOL cad)
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


void wxRDP::SendXKeyEvent(XKeyEvent * event)
{
	 XSync(event->display, FALSE);
	 XSendEvent(event->display, event->window, TRUE, KeyPressMask, (XEvent *)event);
	 XSync(event->display, FALSE);
}

void wxRDP::GrabAll(BOOL state)
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




wxContRDP::wxContRDP(wxWindow * parent,wxWindowID id,const wxPoint& point,const wxSize& size):
	wxWindow(parent,id,point,size)
{
}


void wxContRDP::on_set_focus(wxFocusEvent& event)
{
	wxRDP * rdp = (wxRDP *)GetParent();
	if (rdp->bConnected)
	{
		rdp->CheckFocus();
	}
	
	event.Skip();
}

void wxContRDP::on_kill_focus(wxFocusEvent& event)
{
	wxRDP * rdp = (wxRDP *)GetParent();
	if (rdp->bConnected)
	{
		rdp->EnableFocus(FALSE);
	}
	
	event.Skip();
	

}

void wxContRDP::on_mouse_events(wxMouseEvent& event)
{
	wxRDP * rdp = (wxRDP *)GetParent();
	if (event.Entering())
	{
		rdp->CheckFocus();
	}
}




void wxRDP::EnableFocus(BOOL state)
{
	if (state)
	{
		lock = new wxWindowUpdateLocker(this);
		lock_cnt = new wxWindowUpdateLocker(cnt);
		
		gtk_widget_set_can_focus(sock,true);
		gdk_window_focus(gtk_widget_get_window(sock),GDK_CURRENT_TIME);
		gtk_widget_grab_focus (sock);
	
		
		SendKey();
		CentreRDP();
		
	
	}
	else
	{
		gtk_widget_set_can_focus(sock,false);
		gdk_keyboard_ungrab (GDK_CURRENT_TIME);

		if (lock) { delete lock; lock = NULL;}
		if (lock_cnt) { delete lock_cnt; lock_cnt = NULL;}	
	}
}


void wxRDP::on_set_focus(wxFocusEvent& event)
{
	if (bConnected)
	{
		CheckFocus();
	}
	
	event.Skip();
	
}

void wxRDP::on_leave_focus(wxFocusEvent& event)
{

	if (bConnected)
	{
		EnableFocus(FALSE);
	}
	
	event.Skip();
	
	
}

void wxRDP::CheckFocus()
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


void wxRDP::on_any_mouse_event(wxMouseEvent& event)
{
	if (event.Entering())
	{
		CheckFocus();
	}
		
}

void wxRDP::CheckWindow()
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



void wxRDP::wait_exit(GPid pid, gint status, gpointer data)
{
	wxRDP * rdp = (wxRDP *)data;
	g_spawn_close_pid (pid);
	rdp->bConnected = false;

	if (rdp->info_uniq_name != 0)
	{
		rdp->main_frame->m_panel_tree->rdptree->from_wxrdp(rdp->info_uniq_name,TREEDATA_DEC_CONNCOUNT);  
	}

	
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
			
			
			wxMessageBox(error_string,wxT("Error"),wxICON_ERROR);
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
		rdp->main_frame->connection_status_update(evt);
	}

	rdp->sock_id = 0;
	if (rdp->sock) {gtk_widget_destroy(rdp->sock); rdp->sock = NULL;}

	if (rdp->bClosing)
	{
		int lc = rdp->main_frame->nb->GetPageCount();
		if (lc > 0)
		{
			for (int i = 0; i < lc ; i ++)
			{
				if (rdp->splitter_rdp == (wxSplitterRDP *)rdp->main_frame->nb->GetPage(i))
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




bool wxRDP::DoRdp()
{

	cnt->SetPosition(wxPoint(0,0));
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
	
	sock_id =  gtk_socket_get_id(GTK_SOCKET(sock));
	RDPConn * settings = &rdp_settings;
	
	gchar *argv[50];
	int argc = 0;
    argv[argc++] = g_strdup ("rdesktop");

	if (settings != NULL)
	{
		if ( settings->username != wxT(""))
		{
			 argv[argc++] = g_strdup("-u");
			 argv[argc++] = g_strdup (settings->username.ToUTF8());
		}
		
		if (settings->domain != wxT(""))
		{
			 argv[argc++] = g_strdup("-d");
			 argv[argc++] = g_strdup(settings->domain.ToUTF8());
		}
		if (settings->shell != wxT(""))
		{
			 argv[argc++] = g_strdup("-s");
			 argv[argc++] = g_strdup(settings->shell.ToUTF8());
		}
		if (settings->directory != wxT(""))
		{
			 argv[argc++] = g_strdup(" -c ");
			 argv[argc++] = g_strdup(settings->directory.ToUTF8());
		}
		if (settings->password != wxT(""))
		{
			 argv[argc++] = g_strdup("-p");
			 argv[argc++] = g_strdup(settings->password.ToUTF8());
		}
		
			
		if (settings->keyboard_map != wxT(""))
		{
			 argv[argc++] = g_strdup("-k");
			 argv[argc++] = g_strdup(settings->keyboard_map.ToUTF8());
		}
		
		if (settings->bControlSize == 1)
		{
			int width = 0;
			int height = 0;
			splitter_rdp->GetSize(&width, &height);
			GetClientSize(&width, &height);
			
			cnt->SetSize(wxSize(width,height));
							 
			gtk_widget_set_size_request (GTK_WIDGET (sock), width,height);
			
			if ((width >= 0) && (height >= 0))
			{
				 argv[argc++] = g_strdup ("-g");
				 argv[argc++] = g_strdup_printf ("%ix%i", width, height);
			}
			else
			{
				return false;
			}
			
		}
		else if (settings->bFullScreen == 1)
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

		}
		else if ((settings->width != 0) && (settings->heigth != 0))
		{
			 argv[argc++] = g_strdup ("-g");
			 argv[argc++] = g_strdup_printf ("%ix%i",  settings->width, settings->heigth);
			 cnt->SetSize(wxSize(settings->width,settings->heigth));
			 if (rdp_size.x < settings->width || rdp_size.y < settings->heigth)
			 {
				SetScrollbars(1,1,settings->width,settings->heigth);
			 }
				 
			 gtk_widget_set_size_request (GTK_WIDGET (sock), settings->width,settings->heigth);
			 
			 
		}
		if (settings->force_update_screen != false)
		{
			 argv[argc++] = g_strdup("-f");
		
		}
		if (settings->backing_store != false)
		{
			argv[argc++] = g_strdup("-B");
				
		}
		if (settings->encription_enable_french != false)
		{
			 argv[argc++] = g_strdup("-e");
		}
		if (settings->encription_enable_new != false)
		{
			 argv[argc++] = g_strdup("-E");
		}
		if (settings->send_mouse_event != false)
		{
			 argv[argc++] = g_strdup("-m");
		}
		if (settings->private_color_map != false)
		{
			 argv[argc++] = g_strdup("-C");
		}
			
		if (settings->numlock_sync != false)
		{
			 argv[argc++] = g_strdup("-N");
		}
		if (settings->color_depth != 0)
		{
			 argv[argc++] = g_strdup("-a");
			 argv[argc++] = g_strdup_printf(("%i"),settings->color_depth);
		}
		if (settings->enable_compres != false )
		{
			argv[argc++] = g_strdup("-z");
		}
		if (settings->bandwidth == 0 || settings->bandwidth == 1)
		{
			argv[argc++] = g_strdup("-x");
			argv[argc++] = g_strdup("m");
		}
		else if (settings->bandwidth == 2)
		{
			argv[argc++] = g_strdup("-x");
			argv[argc++] = g_strdup("b");
		}
		else if (settings->bandwidth == 3)
		{
			argv[argc++] = g_strdup("-x");
			argv[argc++] = g_strdup("l");
		}

		if (settings->bEnableBitmapCaching != true)
		{
			argv[argc++] = g_strdup("-P");
		}
		if (settings->attach_to_console != false)
		{
			argv[argc++] = g_strdup("-0");
		}
		if (settings->use_rdp_version == 1)
		{
			argv[argc++] = g_strdup("-5");
		}
		else if (settings->use_rdp_version == 0)
		{
			argv[argc++] = g_strdup("-4");
		}
		if (settings->SoundType == 0)
		{
			argv[argc++] = g_strdup ("-r");
			argv[argc++] = g_strdup ("sound:local");
		} 
		else if (settings->SoundType == 1)
		{
			argv[argc++] = g_strdup ("-r");
			argv[argc++] = g_strdup ("sound:off");			
		}
		else if (settings->SoundType == 2)
		{
			argv[argc++] = g_strdup ("-r");
			argv[argc++] = g_strdup ("sound:remote");
		}
		if (settings->redirect_devices_nix != wxT(""))
		{
			//	wxMessageBox(settings->redirect_devices_nix);
			
			wxString devices(settings->redirect_devices_nix);
			for (size_t i = 0; i < devices.Length() - 2; i ++)
			{
				if (devices.Mid(i,3) == wxT("-r "))
				{
					for (size_t j = i + 3; j <= devices.Length(); j ++)
					{
						if (devices.Mid(j,3) == wxT("-r "))
						{
							wxString device = devices.Mid(i + 3, j - i - 4);
							//				wxMessageBox(device);
							
							argv[argc++] = g_strdup ("-r");
							argv[argc++] = g_strdup (device.ToUTF8());
							break;
						}
						else if (j == devices.Length())
						{
							wxString device = devices.Mid(i + 3, j - i - 3);
							//wxMessageBox(device);
							
							argv[argc++] = g_strdup ("-r");
							argv[argc++] = g_strdup (device.ToUTF8());
							break;
						}

					}
				}
				
			}
			
		}
		
	}
	


	argv[argc++] = g_strdup ("-X");
    argv[argc++] = g_strdup_printf ("%i", sock_id);
	argv[argc++] = g_strdup(rdpconn.hostname.ToUTF8());
	argv[argc++] = NULL;


	GSpawnFlags spawn_flags =  (GSpawnFlags)(G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD);

		bool ret = g_spawn_async_with_pipes (NULL,argv,NULL,spawn_flags,NULL, 
        NULL,&sock_id,NULL,&output_fd, &error_fd,NULL);


		for (int i = 0; i < argc ; i++)
		{
			g_free(argv[i]);
			
		}
		
		
		if (ret)
		{
			bConnected = true;

			g_child_watch_add (sock_id, wxRDP::wait_exit, this);

			if (info_uniq_name != 0)
			{
				main_frame->m_panel_tree->rdptree->from_wxrdp(info_uniq_name,TREEDATA_INC_CONNCOUNT);  
			}

			wxCommandEvent eventCustom(ID_CONNECTION_STATUS_UPDATE);
			wxPostEvent(main_frame, eventCustom);
			
		}
				
		gtk_widget_show(sock);
		CentreRDP();
		
		return ret;
}


wxString wxRDP::get_error_string()
{
	return Error_mgs;
}
void wxRDP::drop_error_string()
{
	Error_mgs.Clear();
	Error_mgs = wxT("");
}



void wxRDP::close_rdesktop_prg(wxCommandEvent& event)
{
	wxCommandEvent eventCustom(ID_CLOSE_RDESKTOP_PRG);
	wxPostEvent(main_frame, eventCustom);
}

bool wxRDP::request_close(BOOL bDetach)
{
	if (sock_id)
	{
		EnableFocus(FALSE);
				
		bClosing = bDetach;
		//	kill(sock_id,SIGKILL);
		kill(sock_id,SIGTERM);
	}

	return true;
}

void wxRDP::event_dispatcher(wxCommandEvent& event)
{

}

void wxRDP::OnIdle(wxIdleEvent& event)
{

}


void wxRDP::OnTimer(wxTimerEvent& event)
{

}

bool wxRDP::get_state()
{
	return bConnected;
}

bool wxRDP::get_proc_state()
{
	return wxrdp_status;
}


void wxRDP::set_state(bool state_to_set)
{
	bConnected = state_to_set;
}

void wxRDP::set_proc_state(bool state_to_set)
{
	wxrdp_status = state_to_set;
}


void wxRDP::rdesktop_not_found(wxCommandEvent& event)
{

}

int wxRDP::get_state_wxrdp()
{
	return 0;
}





