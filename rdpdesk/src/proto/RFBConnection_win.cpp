#include "RFBConnection_win.hpp"
#include "network_helper.hpp"
#include "main_window.hpp"

RFBConnection::RFBConnection(MainFrame *main, Options_HashMap options, wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name) : BasicConnection(main, options, parent, id, pos, size, style, name)
{
		
	inner_status = true;
	local_options = options;
	HRESULT hr;
	hr = CoInitialize(0);
	if(FAILED(hr)) 
	{
		wxMessageBox(_("VNC connection is not available - 1"), _("Error"), wxICON_ERROR);
		inner_status = false;
		//wxMessageBox(wxT("BLA1"));
		return;
	}

	hr = CoCreateInstance(CLSID_UltraVncAxObj, NULL, CLSCTX_INPROC_SERVER, IID_IUltraVncAxObj, (void**) &vncax);

	if(FAILED(hr))
	{
		bObjectOk = FALSE;
		//wxMessageBox(wxT("BLA2"));
	//	return;
	}
	else
	{
		bObjectOk = TRUE;
		//wxMessageBox(wxT("BLA3"));
	}
	if(bObjectOk)
	{
		cnt = (wxWindow *)(new wxActiveXContainer(this, IID_IUltraVncAxObj, vncax));
	}
	
	if (cnt == NULL)
	{
	//	wxMessageBox(wxT("RFB connection is not available."),wxT("Error"),wxICON_ERROR);
		bObjectOk = FALSE;
		//wxMessageBox(wxT("BLA4"));
	}
	else
	{
		bObjectOk = TRUE;
		//wxMessageBox(wxT("BLA5"));
	}
	
}

RFBConnection::~RFBConnection()
{
	//delete vncax;
	if(vncax) {
		vncax->Release();
	}
}

bool RFBConnection::Connect()
{
	//wxMessageBox(wxT("BLA"));
	wxString connection_string = local_options[wxT("hostname")];

	if(local_options[wxT("custom_port")] == "1")
	{
		connection_string += wxT(":") + wxT(local_options["port"]);
	}
	
		HRESULT hr;
		hr = vncax->put_Server(SysAllocString(AnsiToUnicode(connection_string)));
		if(FAILED(hr)) return false;
		vncax->put_Password(SysAllocString(AnsiToUnicode(local_options[wxT("password")])));
		
		if(local_options[wxT("proxy_host")].Trim().Len() > 0)
		{
			vncax->put_ProxyHost(SysAllocString(AnsiToUnicode(local_options[wxT("proxy_host")])));
			vncax->put_ProxyPort(wxAtoi(local_options[wxT("proxy_port")]));
		}
		
		if(wxAtoi(local_options[wxT("shared")]))
		{
			vncax->put_Shared(TRUE);
		}
		else
		{
			vncax->put_Shared(FALSE);
		}

		if(wxAtoi(local_options[wxT("viewonly")]))
		{
			vncax->put_ViewOnly(TRUE);
		}
		else
		{
			vncax->put_ViewOnly(FALSE);
		}
		//vncax->put_FullScreen(1);
		hr = vncax->Connect();
		if(FAILED(hr)) return false;
	return true;
}

bool RFBConnection::DoConnection()
{
	//wxMessageBox(wxT("Test"));
	/*if(wxAtoi(local_options[wxT("full_screen")]))
	{
		this->FullScreen(TRUE);
		this->bFullScreen = TRUE;
	}*/
	return Connect();
}

void RFBConnection::GrabAll(BOOL state)
{

}

void RFBConnection::Disconnect()
{

	HRESULT hr;
	hr = vncax->Disconnect();
}

bool RFBConnection::DisconnectClose()
{
	if (!bObjectOk)
	{
		return true;
	}
	Disconnect();
	return true;
}

void RFBConnection::CentreConnection()
{

}

void RFBConnection::SendKey(BOOL cad)
{ 
	vncax->sendCAD();
}

void RFBConnection::FullScreen(BOOL bRestore)
{
	if (bRestore)
	{
		fullscreen_frame = new RFBFullScreen(cnt,options);
		if (fullscreen_frame)
		{
			conn_splitter->m_timer_screenshot.Stop();
			bFullScreen = TRUE;
		}
	}
}

void RFBConnection::DispatcherActiveX(wxActiveXEvent& event)
{
	switch(event.GetDispatchId())
	{
	case RFB_EVENT_CONNECTED:
		this->conn_splitter->switch_state();
		SendConnectEvent();
		this->bConnected = TRUE;

		CheckOptions();
		main_frame->FocusCurrentPage();
		this->SetFocus();

		break;
	case RFB_EVENT_DISCONNECTED:
		this->conn_splitter->switch_state();
		SendDisconnectEvent();
		this->bConnected = FALSE;
		CheckOptions();
	}

	event.StopPropagation(); 
}

void RFBConnection::on_enter_focus(wxFocusEvent& event)
{
	cnt->SetFocus();
	event.Skip();
}

RFBFullScreen::RFBFullScreen(wxWindow *cnt, Options_HashMap options) : wxTopLevelWindow(NULL,-1,wxT(""),wxDefaultPosition,wxDefaultSize)
{
	rfb = (RFBConnection *)cnt->GetParent();
	cnt_old = cnt;
	local_options = options;
	
	win = NULL;
	win2 = NULL;
	tb = NULL;
	///
	//options[wxT("full_screen")] = wxT("1");
	///

	SetSize(wxGetDisplaySize());
	SetBackgroundColour(wxColour(wxT("black")));

	Show();

	win = new wxScrolledWindow(this,-1,wxPoint(0,0),GetClientSize());
	ShowFullScreen(true);
	win->FitInside();

	win2 = new ToolbarCnt(this);

	int heigth = wxAtoi(local_options[wxT("heigth")]);
	int width = wxAtoi(local_options[wxT("width")]);
	wxString connection_name, control_size, full_screen;
	
	tb = new FullScreenToolBar(connection_name,win2,-1,wxPoint(100,100),wxSize(200,22));
	tb->DeleteTool(ID_FULLSCREEN_CLOSE);
	tb->DeleteTool(ID_FULLSCREEN_GRABINPUT);
	win2->fs = tb;

	cnt->Reparent(win);

	wxSize fs = wxGetDisplaySize();

		if ((width > fs.x) || (heigth > fs.y))
		{
				win->SetScrollbars(1,1,width,heigth);
		}
		else  ////////////////
		{
			//win->SetSize(cnt->GetSize());
			win->SetScrollbars(1,1,width,heigth);
		}     ////////


	win->Show(true);
	tb->fullscreen = this;
	tb->win = win;

	cnt->CenterOnParent();
	cnt->SetFocus();
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

RFBFullScreen::~RFBFullScreen()
{
/*	if (win)
	{
		delete win;
		win = NULL;
	}
	if (win2)
	{
		delete win2;
		win2 = NULL;
	}
	if (tb)
	{
		delete tb;
		tb = NULL;
	}*/
}

void RFBFullScreen::Leave()
{
	ShowFullScreen(false);
	cnt_old->Reparent(rfb);
	cnt_old->CenterOnParent();
	
	rfb->bFullScreen = FALSE;
	//rfb->Update();
	rfb->conn_splitter->m_timer_screenshot.Start();
	
	Close();
	Destroy();
}

void RFBFullScreen::on_enter_focus(wxFocusEvent& event)
{
	cnt_old->SetFocus();
	event.Skip();
}

void RFBFullScreen::on_toolbar(wxCommandEvent& event)
{

	switch(event.GetId())
	{
	case ID_FULLSCREEN_PHOTO:
		rfb->photo();
		break;
		
	case ID_FULLSCREEN_SENDCAD:
		rfb->SendKey(TRUE);
		break;
		
	case ID_FULLSCREEN_GRABINPUT:
		if (tb->GetToolToggled(ID_FULLSCREEN_GRABINPUT))
		{
			rfb->GrabAll(TRUE);
		}
		else
		{
			rfb->GrabAll(FALSE);
		}
		break;	
				
	case ID_FULLSCREEN_CLOSE:
		Leave();
		//rfb->Disconnect();
	//	rfb->SendDisconnectEvent();
		break;
		
	case ID_FULLSCREEN_LEAVE:	
		Leave();
		break;
	default:
		break;
		
	}
}
