///////////////////////////////////////////////////////////////////////////////
// File name:   ICAConnection_nix.cpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-22 11:52:31" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#include "ICAConnection_nix.hpp"
#include <wx/textfile.h>
// ICAConnection::ICAConnection(Main_Frame *m_frame,RDPConn rdpsettings ,wxWindow *parent, wxWindowID id,
// 			 const wxPoint& pos,
// 			 const wxSize& size,
// 			 long style,
// 			 const wxString& name)
// 	:BasicConnection(m_frame,rdpsettings,parent,id,pos,size,0,name)
// //	:wxScrolledWindow (parent, id, pos, size, 0, name)
// {
// 	if (!wxFileExists(wxT("/usr/bin/wfica")))
// 	{
// 		wxMessageBox(wxT("wfica (ICA client for Linux) not found.\nPlease install ICA Citrix receiver (www.citrix.com)"),wxT("Error"),wxICON_ERROR);
// 		return;
// 	}

// 	sock = NULL;
// 	bClosing = FALSE;

// //	fullscreen_frame = NULL;
		
// 	lock = NULL;
// 	lock_cnt = NULL;
// 	cnt = NULL;
		
// 	Info.Clear();
// 	info_uniq_name = 0;
// //	main_frame = m_frame;
// //	bConnected = false;
// //	bFullScreen = false;
// 	conn_splitter = (ConnSplitter *)parent;
// 	rdpconn.pid = 0;

// //	bGrab = FALSE;
		
// 	wxFile temp;
	
// 	GtkWidget *widget = this->m_wxwindow;
// 	cnt = new wxContICA(this,-1,wxPoint(0,0),wxDefaultSize);
// 	gtk_widget_realize( widget );
// 	GdkWindow *gdkWin = GTK_PIZZA (widget)->bin_window;
// 	Window wid = GDK_WINDOW_XWINDOW(gdkWin);
// 	xid =  wid;
// //	rdp_settings = rdpsettings;
// //	rdpconn = rdpsettings;

// 	bObjectOk = TRUE;
// }

ICAConnection::ICAConnection(Main_Frame *m_frame,Options_HashMap options ,wxWindow *parent, wxWindowID id,
			 const wxPoint& pos,
			 const wxSize& size,
			 long style,
			 const wxString& name)
	:BasicConnection(m_frame,options,parent,id,pos,size,0,name)
//	:wxScrolledWindow (parent, id, pos, size, 0, name)
{
	if (!wxFileExists(wxT("/usr/bin/wfica")))
	{
		wxMessageBox(wxT("wfica (ICA client for Linux) not found.\nPlease install ICA Citrix receiver (www.citrix.com)"),wxT("Error"),wxICON_ERROR);
		return;
	}

	sock = NULL;
	bClosing = FALSE;

//	fullscreen_frame = NULL;
	lock = NULL;
	lock_cnt = NULL;
	cnt = NULL;
	Info.Clear();
	info_uniq_name = 0;
//	main_frame = m_frame;
//	bConnected = false;
//	bFullScreen = false;
	conn_splitter = (ConnSplitter *)parent;
	//rdpconn.pid = 0;

//	bGrab = FALSE;
	wxFile temp;
	GtkWidget *widget = this->m_wxwindow;
	cnt = new wxContICA(this,-1,wxPoint(0,0),wxDefaultSize);
	gtk_widget_realize( widget );
	GdkWindow *gdkWin = GTK_PIZZA (widget)->bin_window;
	Window wid = GDK_WINDOW_XWINDOW(gdkWin);
	xid =  wid;
	//rdp_settings = rdpsettings;
	local_options = options;
//	rdpconn = rdpsettings;

	bObjectOk = TRUE;
	wxString home_path;
	wxGetEnv(STANDARD_LINUX_ENV, &home_path);
	ICALaunch = home_path + wxT("/.ICAClient");
	//wxMessageBox(home_path);
	if (!wxDirExists(ICALaunch.GetData())) wxMkdir(ICALaunch);
	appsrv_ini = ICALaunch + wxT("/appsrv.ini");

}


ICAConnection::~ICAConnection()
{

//	wxMessageBox(wxT("wxRDP dtor"));
	
//	if (rdp->sock) {gtk_widget_destroy(rdp->sock); rdp->sock = NULL;}
	if (cnt)
	{
//		wxMessageBox(wxT("delete cnt"));
		
		//	delete cnt;
		cnt->Destroy();
		
		cnt = NULL;	}
//	wxMessageBox(wxT("wxRDP dtor2"));
	
		
}

void ICAConnection::CentreRDP()
{
//	wxMessageBox(wxT("centre rdp"));
	
	CheckWindow();
	
	wxSize sz_cnt = cnt->GetSize();
	wxSize sz_this = conn_splitter->GetClientSize();

	cnt->SetPosition(wxPoint(0,0));
	
	if ((sz_cnt.x < sz_this.x) || (sz_cnt.y < sz_this.y))
	{
		cnt->CenterOnParent();
	}
}

void ICAConnection::photo()
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

void ICAConnection::FullScreen(BOOL bRestore)
{
   if (bRestore)
   {
      //	RDPFullScreen * win = NULL;
      fullscreen_frame = new ICAFullScreen((wxContICA *)cnt, local_options);
      if (fullscreen_frame)
      {
	 conn_splitter->m_timer_screenshot.Stop();
	 bFullScreen = TRUE;
      }
   }

   /*
     if (bRestore)
     {
     //	RDPFullScreen * win = NULL;
     fullscreen_frame = new RDPFullScreen((wxContRDP *)cnt,rdp_settings);
     if (fullscreen_frame)
     {
     splitter_rdp->m_timer_screenshot.Stop();
     bFullScreen = TRUE;
     }
     }
   */
}


void ICAConnection::SendKey(BOOL cad)
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


void ICAConnection::SendXKeyEvent(XKeyEvent * event)
{
	 XSync(event->display, FALSE);
	 XSendEvent(event->display, event->window, TRUE, KeyPressMask, (XEvent *)event);
	 XSync(event->display, FALSE);
}

void ICAConnection::GrabAll(BOOL state)
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




wxContICA::wxContICA(wxWindow * parent,wxWindowID id,const wxPoint& point,const wxSize& size):
	wxWindow(parent,id,point,size)
{
}


void wxContICA::on_set_focus(wxFocusEvent& event)
{
	ICAConnection * conn = (ICAConnection *)GetParent();
	if (conn->bConnected)
	{
		conn->CheckFocus();
	}
	
	event.Skip();
}

void wxContICA::on_kill_focus(wxFocusEvent& event)
{
	ICAConnection * conn = (ICAConnection *)GetParent();
	if (conn->bConnected)
	{
		conn->EnableFocus(FALSE);
	}
	
	event.Skip();
	

}

void wxContICA::on_mouse_events(wxMouseEvent& event)
{
	ICAConnection * conn = (ICAConnection *)GetParent();
	if (event.Entering())
	{
		conn->CheckFocus();
	}
}




void ICAConnection::EnableFocus(BOOL state)
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


void ICAConnection::on_set_focus(wxFocusEvent& event)
{
	if (bConnected)
	{
		CheckFocus();
	}
	
	event.Skip();
	
}

void ICAConnection::on_leave_focus(wxFocusEvent& event)
{

	if (bConnected)
	{
		EnableFocus(FALSE);
	}
	
	event.Skip();
	
	
}

void ICAConnection::CheckFocus()
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


void ICAConnection::on_any_mouse_event(wxMouseEvent& event)
{
	if (event.Entering())
	{
		CheckFocus();
	}
		
}

void ICAConnection::CheckWindow()
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



void ICAConnection::wait_exit(GPid pid, gint status, gpointer data)
{
	ICAConnection * conn = (ICAConnection *)data;
	g_spawn_close_pid (pid);
	conn->bConnected = false;

	conn->SendDisconnectEvent();
	

	
	if (status != 0 && status != SIGTERM)
	{
		char * buffer = new char[4096];
		memset(buffer,0,4096);
		int len;
		
	    lseek (conn->error_fd, 0, SEEK_SET);
	    len = read (conn->error_fd, buffer, 4095);
	
		if (len > 0)
		{
			buffer[len] = '\0';
			wxString error_string;
			error_string = wxString::From8BitData(buffer,len);

			//// TEST
			conn->EnableFocus(FALSE);
			
			
			wxMessageBox(error_string,wxT("Error"),wxICON_ERROR);
		}
	
		delete buffer;
	 }
	
	close(conn->output_fd);
	close(conn->error_fd);
/*
	if (rdp->bFullScreen && rdp->fullscreen_frame)
	{
		rdp->fullscreen_frame->Leave();
	}
	rdp->fullscreen_frame = NULL;
	
*/	
	if (!conn->bClosing)
	{
		wxCommandEvent evt;
		evt.SetId(ID_CONNECTION_STATUS_UPDATE);
		conn->main_frame->connection_status_update(evt);
	}

	conn->sock_id = 0;
	if (conn->sock) {gtk_widget_destroy(conn->sock); conn->sock = NULL;}

	if (conn->bClosing)
	{
		int lc = conn->main_frame->nb->GetPageCount();
		if (lc > 0)
		{
			for (int i = 0; i < lc ; i ++)
			{
				if (conn->conn_splitter == (ConnSplitter *)conn->main_frame->nb->GetPage(i))
				{
					conn->main_frame->current_page_for_delete = i;
					wxUpdateUIEvent evt;
					evt.Check(true); 
					evt.SetId(ID_ERRORHOSTNAME);
					wxPostEvent(conn->main_frame,evt);
					conn->bClosing = FALSE;
					return ;
				}

			}
		}
	}
	
	conn->bClosing = FALSE;
}




bool ICAConnection::DoConnection()
{

   //wxString ICALaunch = wxGetCwd() + wxT("~/.ICAClient");
   //wxString appsrv_ini = ICALaunch + wxT("/appsrv.ini");

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
   //RDPConn * settings = &rdp_settings; 

   gchar *argv[50];
   int argc = 0;
   argv[argc++] = g_strdup ("wfica"); // launch wfica with .ini files

   if (local_options[wxT("username")] != wxT(""))
   {
      argv[argc++] = g_strdup("-username");
      argv[argc++] = g_strdup (local_options[wxT("username")].ToUTF8());
   }
   if (local_options[wxT("domain")] != wxT(""))
   {
      argv[argc++] = g_strdup("-domain");
      argv[argc++] = g_strdup(local_options[wxT("domain")].ToUTF8());
   }
   if (local_options[wxT("shell")] != wxT(""))
   {
      argv[argc++] = g_strdup("-program");
      argv[argc++] = g_strdup(local_options[wxT("shell")].ToUTF8());
   }
   if (local_options[wxT("directory")] != wxT(""))
   {
      argv[argc++] = g_strdup("-directory");
      argv[argc++] = g_strdup(local_options[wxT("directory")].ToUTF8());
   }
   if (local_options[wxT("password")] != wxT(""))
   {
      argv[argc++] = g_strdup("-clearpassword");
      argv[argc++] = g_strdup(local_options[wxT("password")].ToUTF8());
   }

   argv[argc++] = g_strdup("-quiet");  // disable ICA Citrix
   // receiver window
//		int connection_height = 0;
//		int connection_width = 0;

   wxSize connection_size;
   if (wxAtoi(local_options[wxT("control_size")]))
   {
      connection_size = GetClientSize();

      local_options[wxT("width")] = wxString::Format(wxT("%i"), connection_size.x);
      local_options[wxT("heigth")] = wxString::Format(wxT("%i"), connection_size.y);
      //std::cout  <<  <<  std::endl;
   }
   else
   {
      if (wxAtoi(local_options[wxT("full_screen")]))
      {
	 connection_size = wxGetDisplaySize(); // ?
	 local_options[wxT("width")] = wxString::Format(wxT("%i"), connection_size.x);
	 local_options[wxT("heigth")] = wxString::Format(wxT("%i"), connection_size.y);
	 wxMessageBox(local_options[wxT("width")]);
	 wxMessageBox(local_options[wxT("heigth")]);

      }
      else
      {
	 connection_size = wxSize(wxAtoi(local_options[wxT("width")]),
				  wxAtoi(local_options[wxT("heigth")]));
      }
   }
   // set container size and scrollbars 
   cnt->SetSize(connection_size);
   if (rdp_size.x < connection_size.x || rdp_size.y < connection_size.y)
   {
      SetScrollbars(1,1,connection_size.x, connection_size.y);
   }
   gtk_widget_set_size_request (GTK_WIDGET (sock), connection_size.x, connection_size.y);
   wxPoint connection_position = GetParent()->GetScreenPosition();
   wxString geometry = wxString::Format(wxT("%ix%i+%i+%i"),connection_size.x,connection_size.y,
					connection_position.x,connection_position.y);
   if (!wxAtoi(local_options[wxT("full_screen")]))
   {
      argv[argc++] = g_strdup("-geometry");
      argv[argc++] = g_strdup(geometry.ToUTF8());
   }

   argv[argc++] = g_strdup("-depth");
   if (wxAtoi(local_options[wxT("color_depth")]) == 0)
   {
      argv[argc++] = g_strdup("8");
   }
   else if (local_options[wxT("color_depth")] == 1 ||
	    local_options[wxT("color_depth")] == 2 )
   {
      argv[argc++] = g_strdup("16");
   }
   else if(local_options[wxT("color_depth")] == 3)
   {
      argv[argc++] = g_strdup("24");
   }
   else
   {
      argv[argc++] = g_strdup("auto");
   }
   // connection file this settings
   argv[argc++] = g_strdup("-file");
   argv[argc++] = g_strdup(appsrv_ini.ToUTF8());

   //client name
   argv[argc++] = g_strdup("-clientname");
   wxString clientname = (local_options[wxT("connection_name")].Length() < 1) ? local_options[wxT("hostname")] : local_options[wxT("connection_name")] ;
   argv[argc++] = g_strdup(clientname.ToUTF8());

   if (!MakeIniFile())
   {
      wxMessageBox(wxT("Error: create *.ini connection files failed"),wxT("Error"),wxICON_ERROR);
      return false;
   }

/*		
			
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
*/

	

/*
  argv[argc++] = g_strdup ("-X");
  argv[argc++] = g_strdup_printf ("%i", sock_id);
  argv[argc++] = g_strdup(rdpconn.hostname.ToUTF8());
*/
//   if ()
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

      g_child_watch_add (sock_id, ICAConnection::wait_exit, this);

//			if (info_uniq_name != 0)
//			{
//				main_frame->m_panel_tree->rdptree->from_wxrdp(info_uniq_name,TREEDATA_INC_CONNCOUNT);  
//			}
//			if (info_uniq_name != 0)
//				wxMessageBox(wxT("0"));
			
      //		SendConnectEvent();
			

      wxCommandEvent eventCustom(ID_CONNECTION_STATUS_UPDATE);
      wxPostEvent(main_frame, eventCustom);
			
   }
				
   gtk_widget_show(sock);
   CentreRDP();

// TEST
   wxThread::Sleep(2000);
		
		
   ica_window = GetICAWindowId();
   if (!ica_window)
   {
      wxMessageBox(wxT("ICA Connection failed.\nClosing..."),wxT("Error"),wxICON_ERROR);
      Disconnect();
      return false;
   }

//		if (!InjectWindow())
//		{
//			wxMessageBox(wxT("ICA Connection failed.\nClosing..."),wxT("Error"),wxICON_ERROR);
//			Disconnect();
//			return false;
//		}


   // try receive real window geometry
/*
  wxSize	_connection_size = GetRealGeometry(ica_window);
  std::cout << "window: " << _connection_size.x << "x" << _connection_size.y << std::endl; 
  cnt->SetSize(_connection_size);
  if (rdp_size.x < _connection_size.x || rdp_size.y < _connection_size.y)
  {
  SetScrollbars(1,1,_connection_size.x, _connection_size.y);
  }
  gtk_widget_set_size_request (GTK_WIDGET (sock), _connection_size.x, _connection_size.y);
*/		

   wxThread::Sleep(2000);
		
   //int xid = ica_window;


//TEST
   Display * dpy = XOpenDisplay(NULL);
   XUnmapWindow(dpy,ica_window);
   XSync(dpy,0);
		
		

   GdkWindow * socket_plug_window;
   socket_plug_window = gtk_socket_get_plug_window(GTK_SOCKET(sock));
   //std::cout << "plug window = " << socket_plug_window << std::endl;
		
		
//		int count =
//		10;  //////////////////////////////////////////////////////////////////
   int count = 5; // 1 sec for inject connection
   while (count --)
   {
      socket_plug_window = gtk_socket_get_plug_window(GTK_SOCKET(sock));
      //std::cout << "plug window = " << socket_plug_window << std::endl;

      gtk_socket_steal(GTK_SOCKET(sock),ica_window);
      wxThread::Sleep(2000);
   }

   XMapWindow(dpy,ica_window);
   XSync(dpy,0);
		
   XCloseDisplay(dpy);
		
		
   SendConnectEvent();
		
		
//	GtkWidget * widget = conn->cnt->m_wxwindow;
//	GtkSocket * sock_gtk = GTK_SOCKET(sock);

//	GdkWindow *gdkWin = GTK_PIZZA (widget)->bin_window;

//	Window wid_socket = gtk_socket_get_id(sock_gtk);
//	std::cout <<  "wid_socket = " << wid_socket << std::endl;


//	gtk_socket_steal(sock_gtk,xid);
		
//	GdkWindow *gdkWin_sock = GTK_PIZZA (conn->sock)->bin_window;
//	Window wid_sock = GDK_WINDOW_XWINDOW(gdkWin_sock); // container
/*
  Display * dpy = XOpenDisplay(NULL);
	
  XGrabServer(dpy);
  XSync(dpy,0);
  XSelectInput(dpy, xid, SW_EVENTS);
  XSync(dpy,0);
  XUnmapWindow(dpy,xid);
  XSync(dpy,0);
  XReparentWindow(dpy,xid,wid_socket,10,10);
  XSync(dpy,0);
  XUngrabServer(dpy);
  XSync(dpy,0);
	
  XMapWindow(dpy,xid);
  XSync(dpy,0);
  XRaiseWindow(dpy,xid);
  XSync(dpy,0);

  XCloseDisplay(dpy);
*/
			
   return ret;
}

bool ICAConnection::InjectWindow()
{
	/*
		GdkWindow * socket_plug_window;
//		socket_plug_window = gtk_socket_get_plug_window(GTK_SOCKET(sock));
//		std::cout << "plug window = " << socket_plug_window << std::endl;
		
		int count = 10;
		while (count --)
		{
			//	if( gtk_socket_get_plug_window(GTK_SOCKET(sock))) count = 1;
			//std::cout << "plug window = " << socket_plug_window << std::endl;
			gtk_socket_steal(GTK_SOCKET(sock),ica_window);
			wxThread::Sleep(100);
		}
		gtk_socket_steal(GTK_SOCKET(sock),ica_window);
		if( gtk_socket_get_plug_window(GTK_SOCKET(sock))) return true;
	*/
		return false;
}


bool ICAConnection::MakeIniFile()
{
   if (wxFileExists(appsrv_ini))
   {
      wxRemoveFile(appsrv_ini);
   }
   wxTextFile * tf = new wxTextFile();
   wxString temp_string;
   
   //wxMessageBox(appsrv_ini);
   tf->Create(appsrv_ini);

   tf->AddLine(wxT("[WFClient]"));
   tf->AddLine(wxT("Version = 2"));
   tf->AddLine(wxT(""));
   tf->AddLine(wxT("[ApplicationServers]"));
   wxString conn_name = (local_options[wxT("connection_name")].Length() < 1)? local_options[wxT("hostname")] : local_options[wxT("connection_name")];
   //wxString server_name = wxString::Format(wxT("%s ICA Citrix connection"),conn_name.data());
/*
  wxString server_name = wxString::Format(wxT("0x%x ICA"),rdpconn.uniq_name); // Test
  // window
  */
   wxString server_name = wxString::Format(wxT("0x%x ICA"),uniq_conn);
//   wxMessageBox(server_name);
   tf->AddLine(wxString::Format(wxT("%s= "),server_name.data()));
   tf->AddLine(wxT(""));
   //server_name = wxT("[") + server_name + wxT("]");
   //tf->AddLine(wxString::Format(wxT("[%s]"),server_name.data());
   tf->AddLine(wxT("[") + server_name + wxT("]"));
   if (local_options[wxT("port")] != wxT(""))
   {
      tf->AddLine(wxT("Address = ") + local_options[wxT("hostname")]);
   }
   else
   {
      tf->AddLine(wxT("Address = ") + local_options[wxT("hostname")] + wxT(":") + local_options[wxT("port")]);
   }
   if (local_options[wxT("proxy_autoconfig_url"]) != wxT(""))
   {
      tf->AddLine(wxT("ProxyAutoConfigURL=") + local_options[wxT("proxy_autoconfig_url"]));
   }
   if (local_options[wxT("proxy_username"]) != wxT(""))
   {
      tf->AddLine(wxT("ProxyUsername=") + local_options[wxT("proxy_username"]));
   }
   if (local_options[wxT("proxy_password"]) != wxT(""))
   {
      tf->AddLine(wxT("ProxyPassword=") + local_options[wxT("proxy_password"]));
   }
   if (local_options[wxT("proxy_host"]) != wxT(""))
   {
      tf->AddLine(wxT("ProxyHost=") + local_options[wxT("proxy_host"]));
   }
   if (local_options[wxT("proxy_type"]) != wxT(""))
   {
      wxString proxy_type_array[] = {wxT("None") ,_T("Auto"), _T("Socks"), _T("SocksV4"),
					_T("SocksV5"), _T("Secure"), _T("Script")};
      tf->AddLine(wxT("ProxyType=") + proxy_type_array[wxAtoi(local_options[wxT("proxy_type"]))]);
   }
   if (local_options[wxT("proxy_bypass_list"]) != wxT(""))
   {
      tf->AddLine(wxT("ProxyBypassList=") + local_options[wxT("proxy_bypass_list"]));
   }
   if (local_options[wxT("proxy_timeout"]) != wxT(""))
   {
      tf->AddLine(wxT("ProxyTimeout=") + local_options[wxT("proxy_timeout"]));
   }
   if (local_options[wxT("ssl_proxy_host"]) != wxT(""))
   {
      tf->AddLine(wxT("SSLProxyHost=") + local_options[wxT("ssl_proxy_host"]));
   }
   if (local_options[wxT("ssl_enable"]) != wxT(""))
   {
      tf->AddLine(wxT("SSLEnable=") + local_options[wxT("ssl_enable"]));
   }
   if (local_options[wxT("proxy_use_default"]) != wxT(""))
   {
      tf->AddLine(wxT("ProxyUseDefault=") + local_options[wxT("proxy_use_default"]));
   }

   if (local_options[wxT("desired_color")] != wxT(""))
   {
      tf->AddLine(wxT("DesiredColor=") + wxString::Format(wxT("%i"), 1 + wxAtoi(local_options[wxT("desired_color"])))) ;
   }

// if(m_checkbox_updatescreen != NULL)
// {
//    local_options[wxT("force_update_screen")] = wxString::Format(wxT("%i"),
// 								   m_checkbox_updatescreen->GetValue());
// }
   if (wxAtoi(local_options[wxT("full_screen")]))
   {
      tf->AddLine(wxT("UseFullScreen=Yes"));
      tf->AddLine(wxT("NoWindowManager=0"));
      if (wxAtoi(local_options[wxT("width")]))
      {
      	 tf->AddLine(wxT("DesiredHRES = ") + local_options[wxT("width")]);
      }
      if (wxAtoi(local_options[wxT("heigth")]))
      {
      	 tf->AddLine(wxT("DesiredVRES = ") + local_options[wxT("heigth")]);
      }

   }
   if (wxAtoi(local_options[wxT("custom_geometry")]))
   {
      // if (wxAtoi(local_options[wxT("width")]))
      // {
      // 	 tf->AddLine(wxT("DesiredVRES=") + local_options[wxT("width")]);
      // }
      // if (wxAtoi(local_options[wxT("heigth")]))
      // {
      // 	 tf->AddLine(wxT("DesiredHRES=") + local_options[wxT("heigth")]);
      // }
   }
   if (wxAtoi(local_options[wxT("preset_screen_size")]))
   {
      //tf->AddLine(wxT("") + local_options[wxT("preset_screen_size")]);
      // if (wxAtoi(local_options[wxT("width")]))
      // {
      // 	 tf->AddLine(wxT("DesiredVRES=") + local_options[wxT("width")]);
      // }
      // if (wxAtoi(local_options[wxT("heigth")]))
      // {
      // 	 tf->AddLine(wxT("DesiredHRES=") + local_options[wxT("heigth")]);
      // }
 
   }
   if (local_options[wxT("allow_degraded_colors")] != wxT(""))
   {
      tf->AddLine(wxT("AllowDegradedColors=") + local_options[wxT("allow_degraded_colors")]);
   }
   if (local_options[wxT("enable_oss")] != wxT(""))
   {
      tf->AddLine(wxT("EnableOSS=") + local_options[wxT("enable_oss")]);
   }
   if (local_options[wxT("enable_session_sharing_screen")] != wxT(""))
   {
      tf->AddLine(wxT("EnableSessionSharingScreen=") + local_options[wxT("enable_session_sharing_screen")]);
   }
   if (local_options[wxT("approximate_colors")] != wxT(""))
   {
      tf->AddLine(wxT("ApproximateColors=") + local_options[wxT("approximate_colors")]);
   }
   if (local_options[wxT("resize_session")] != wxT(""))
   {
      tf->AddLine(wxT("ResizeSession=") + local_options[wxT("resize_session")]);
   }
   if (local_options[wxT("use_sdlvb")] != wxT(""))
   {
      tf->AddLine(wxT("UseSDLVB=") + local_options[wxT("use_sdlvb")]);
   }
   if (local_options[wxT("font_smoothing_type")] != wxT(""))
   {
      tf->AddLine(wxT("FontSmoothingType=") + local_options[wxT("font_smoothing_type")]);
   }
   if (local_options[wxT("screen_percent")] != wxT(""))
   {
      tf->AddLine(wxT("ScreenPercent=") + local_options[wxT("screen_percent")]);
   }

   if (local_options[wxT("start_initial_program")] != wxT(""))
   {
      if (wxAtoi(local_options[wxT("start_initial_program")]))
      {
	 if (local_options[wxT("initial_program")] != wxT(""))
	 {
	    tf->AddLine(wxT("InitialProgram=\"") + local_options[wxT("initial_program")] + wxT("\""));
	 }
	 if (local_options[wxT("long_command_line")] != wxT(""))
	 {
	    if (local_options[wxT("long_command_line")].Len() < 254)
	    {
	       tf->AddLine(wxT("LongCommandLine=") + local_options[wxT("long_command_line")]);
	    }
	    else
	    {
	       int i = 1;
	       tf->AddLine(wxT("LongCommandLine=") + local_options[wxT("long_command_line")].Left(254));
	       temp_string = local_options[wxT("long_command_line")].Mid(254, local_options[wxT("long_command_line")].Len());
	       
	       for (size_t f = 254 ; f < local_options[wxT("long_command_line")].Len(); f++)
	       {
		  tf->AddLine(wxT("LongCommandLine") + wxString::Format(wxT("%03i="), i) + temp_string.Left(254));
		  temp_string = temp_string.Mid(254, temp_string.Len());
		  f += 253;
		  i++;
		  //std::cout << f << " of "<< f + 254 << std::endl;
	       }
	    }
	 }
	 if (local_options[wxT("work_directory")] != wxT(""))
	 {
	    tf->AddLine(wxT("WorkDirectory=") + local_options[wxT("work_directory")]);
	 }
      }
   }
      if (local_options[wxT("compress")] != wxT(""))
      {
	 tf->AddLine(wxT("Compress=") + local_options[wxT("compress")]);
      }
      if (local_options[wxT("allow_duplicate_sessions")] != wxT(""))
      {
	 tf->AddLine(wxT("AllowDuplicateSessions=") + local_options[wxT("allow_duplicate_sessions")]);
      }
      if (local_options[wxT("persistent_cache_enabled")] != wxT(""))
      {
	 tf->AddLine(wxT("PersistentCacheEnabled=") + local_options[wxT("persistent_cache_enabled")]);
      }
      if (local_options[wxT("desktop_restart_allowed")] != wxT(""))
      {
	 tf->AddLine(wxT("DesktopRestartAllowed=") + local_options[wxT("desktop_restart_allowed")]);
      }
      if (local_options[wxT("mouse_sends_control_v")] != wxT(""))
      {
	 tf->AddLine(wxT("MouseSendsControlV=") + local_options[wxT("mouse_sends_control_v")]);
      }
      if (local_options[wxT("echo_shift_keys")] != wxT(""))
      {
	 tf->AddLine(wxT("EchoShiftKeys=") + local_options[wxT("echo_shift_keys")]);
      }
      if (local_options[wxT("client_audio")] != wxT(""))
      {
	 tf->AddLine(wxT("ClientAudio=") + local_options[wxT("client_audio")]);
      }
      if (local_options[wxT("enable_audio_input")] != wxT(""))
      {
	 tf->AddLine(wxT("EnableAudioInput=") + local_options[wxT("enable_audio_input")]);
      }
      if (local_options[wxT("audio_bandwidth_limit")] != wxT(""))
      {
	 tf->AddLine(wxT("AudioBandwidthLimit=") + local_options[wxT("audio_bandwidth_limit")]);
      }
      if (local_options[wxT("encryption_level_session")] != wxT(""))
      {
	 wxString encryption_level_session[] = {wxT("None"), wxT("Basic"),
						wxT("RC5 (128 bit 0 Login Only)"),
						wxT("RC5 (40 bit)"),
						wxT("RC5 (56 bit)"), wxT("RC5 (128 bit)"), };

	 tf->AddLine(wxT("EncryptionLevelSession=") + encryption_level_session[wxAtoi(local_options[wxT("encryption_level_session")])]);
      }
      if (local_options[wxT("secure_channel_protocol")] != wxT(""))
      {
	 wxString secure_channel_protocol[] = {wxT("ssl"), wxT("tls"), wxT("detect"), };
	 tf->AddLine(wxT("SecureChannelProtocol=") + secure_channel_protocol[wxAtoi(local_options[wxT("secure_channel_protocol")])]);
      }

   tf->Write();
   tf->Close();
   delete tf;

   return true;
}


Window ICAConnection::GetWindowByTitle(Display * dpy, Window top, char * name)
{
	Window *children, dummy;
	unsigned int nchildren;
	size_t i;
	Window w=0;
//	char *window_name; //test
	char *window_name = NULL;
	

	if (XFetchName(dpy, top, &window_name) && !strcmp(window_name, name))
	  return(top);

	if (!XQueryTree(dpy, top, &dummy, &dummy, &children, &nchildren))
	  return(0);

	for (i=0; i<nchildren; i++)
	{
		w = GetWindowByTitle(dpy, children[i], name);
		if (w)
		  break;
	}
	if (children) XFree ((char *)children);
	return(w);

	
}

Window ICAConnection::GetICAWindowId()
{
//	wxMessageBox(wxT("pause"));
	
	// Get ICA Title and xwininfo args

//	wxString title = wxString::Format(wxT("0x%x ICA
//	"),rdpconn.uniq_name);
	
	wxString title = wxString::Format(wxT("0x%x ICA "),uniq_conn);
	
	char * window_title = new char[256];
	memset(window_title,0,256);
	if (title.Length() > 256) return 0;
	strcpy(window_title,title.To8BitData());
	//std::cout << "title = " << window_title << std::endl;
	
	
	Display * dpy = XOpenDisplay(NULL);
	int  screen = XDefaultScreen(dpy);

//	if (!dpy || !screen) return 0;
//	if (!dpy) return 0;
	

	
	// Wait 2 secs while ICA window not exists
	Window w = 0;
	int pause_count = 40;
	int time = 50;
	while (!w && pause_count)
	{
		//std::cout << "loop " << pause_count << std::endl;
		
		w = GetWindowByTitle(dpy, RootWindow(dpy, screen),window_title);


	
		if (!w)
		{
			pause_count --;
			wxThread::Sleep(time);
		}
		
	}
//	Window	w = GetWindowByTitle(dpy, RootWindow(dpy, screen),window_title);

	//if (!w) std::cout << "window not exists" << std::endl;
	//else std::cout << "Window id: " << w << std::endl;
	
	
	XCloseDisplay(dpy);
	if (window_title) delete window_title;
	
	
/*
	wxString ICALaunch = wxGetCwd() + wxT("/ICALaunch");
	if (!wxDirExists(ICALaunch.GetData())) wxMkdir(ICALaunch);
	wxString xwin = ICALaunch + wxT("/xwin");
	if (wxFileExists(xwin)) wxRemoveFile(xwin);
	
	//wxString xwininfo_args = wxString::Format(wxT("xwininfo -name \"%s\" -int |grep 'Window id:' |cut -d\" \" -f4"),title.data());
	wxString xwininfo_args = wxString::Format(wxT("xwininfo -name \"%s\" -int"),title.data());
	wxMessageBox(xwininfo_args);
	wxExecute(xwininfo_args,wxEXEC_SYNC);
*/

	
	

	


	return w;
}


wxSize ICAConnection::GetRealGeometry(Window id)
{
	Display * dpy = XOpenDisplay(NULL);
	Window temp_root;
	int x,y;
	unsigned int w,h,b,d = 0;
	XGetGeometry(dpy,id,&temp_root,&x,&y,&w,&h,&b,&d);
	XCloseDisplay(dpy);
	return wxSize(x,y);
}


/*
wxString wxRDP::get_error_string()
{
	return Error_mgs;
}
void wxRDP::drop_error_string()
{
	Error_mgs.Clear();
	Error_mgs = wxT("");
}
*/


void ICAConnection::close_rdesktop_prg(wxCommandEvent& event)
{
   wxCommandEvent eventCustom(ID_CLOSE_RDESKTOP_PRG);
   wxPostEvent(main_frame, eventCustom);
   if ((bFullScreen == true) && (fullscreen_frame != NULL))
   {
      fullscreen_frame->Leave();
   }
}

bool ICAConnection::request_close(BOOL bDetach)
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
/*
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




bool wxRDP::DoConnection()
{
	return DoRdp();
}
*/

bool ICAConnection::Connect()
{
	return DoConnection();
}

void ICAConnection::Disconnect()
{
   request_close();
   if ((bFullScreen == true) && (fullscreen_frame != NULL))
   {
      fullscreen_frame->Leave();
   }

}


bool ICAConnection::DisconnectClose()
{
	return request_close(TRUE);
}

void ICAConnection::CentreConnection()
{
	if (cnt != NULL)
		CentreRDP();
	
}









ICAFullScreen::ICAFullScreen(wxContICA * cnt,Options_HashMap options):
   wxTopLevelWindow(NULL,-1,wxT(""),wxDefaultPosition,wxDefaultSize)
{
   ica = (ICAConnection *)cnt->GetParent();
   cnt_old = cnt;
   //rdpc = rdpconn;
   local_options = options;
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

   tb = new FullScreenToolBar(local_options[wxT("connection_name")],win2,-1,
			      wxPoint(100,100),wxSize(200,22));

   win2->fs = tb;

   GtkWidget * widget_fullscreen = win->m_wxwindow;
   GtkWidget * widget_cont = cnt->m_wxwindow;
   gtk_container_add(GTK_CONTAINER(widget_fullscreen),widget_cont);
   gtk_widget_reparent(widget_cont,widget_fullscreen);

   wxSize fs = wxGetDisplaySize();

   if (wxAtoi(local_options[wxT("control_size")]))
   {
      ICAConnection * ica_temp = (ICAConnection *)cnt->GetParent();
      wxSize sz = ica_temp->GetVirtualSize();
      gtk_widget_set_size_request(widget_cont,sz.x,sz.y);
   }
   else if (wxAtoi(local_options[wxT("full_screen")]))
   {
      wxSize disp_size = wxGetDisplaySize();
      gtk_widget_set_size_request(widget_cont, disp_size.x, disp_size.y);
   }
   else
   {
      gtk_widget_set_size_request(widget_cont,wxAtoi(local_options[wxT("width")]),
				  wxAtoi(local_options[wxT("heigth")]));
   }
	
   if (!wxAtoi(local_options[wxT("control_size")]))
   {
      if ((wxAtoi(local_options[wxT("width")]) > fs.x) ||
	  (wxAtoi(local_options[wxT("heigth")]) > fs.y))
      {
	 if (!wxAtoi(local_options[wxT("full_screen")]))
	 {
	    win->SetScrollbars(1,1,wxAtoi(local_options[wxT("width")]),
			       wxAtoi(local_options[wxT("heigth")]));
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



ICAFullScreen::~ICAFullScreen()
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

void ICAFullScreen::Leave()
{
	ShowFullScreen(false);

	ICAConnection * ica = (ICAConnection *)cnt_old->GetParent();
	GtkWidget * rdp_w = ica->m_wxwindow;
	GtkWidget * rdp_c = cnt_old->m_wxwindow;

	gtk_widget_reparent(rdp_c,rdp_w);
	if (wxAtoi(local_options[wxT("control_size")]))
	{
		wxSize sz = ica->GetVirtualSize();
		gtk_widget_set_size_request(rdp_c,sz.x,sz.y);
	}
	else if (wxAtoi(local_options[wxT("full_screen")]))
	{
		wxSize disp_size = wxGetDisplaySize();
		gtk_widget_set_size_request(rdp_c, disp_size.x, disp_size.y);
	}
	else
	{
		gtk_widget_set_size_request(rdp_c,wxAtoi(local_options[wxT("width")]),
					    wxAtoi(local_options[wxT("heigth")]));
	}
	ica->bFullScreen = FALSE;
	ica->conn_splitter->m_timer_screenshot.Start();
	Close();
	Destroy();
}


void ICAFullScreen::on_toolbar(wxCommandEvent& event)
{

	switch(event.GetId())
	{
	case ID_FULLSCREEN_PHOTO:
		ica->photo();
		break;
		
	case ID_FULLSCREEN_SENDCAD:
		ica->SendKey(TRUE);
		break;
		
	case ID_FULLSCREEN_GRABINPUT:
		if (tb->GetToolToggled(ID_FULLSCREEN_GRABINPUT))
		{
			ica->GrabAll(TRUE);
		}
		else
		{
			ica->GrabAll(FALSE);
		}
		break;	
				
	case ID_FULLSCREEN_CLOSE:
		Leave();
		wxThread::Sleep(50); // Wait while connection leave fullscreen		 
		ica->Disconnect();
		break;
		
	case ID_FULLSCREEN_LEAVE:	
		Leave();
		
		break;
	default:
		break;
		
	}
}




