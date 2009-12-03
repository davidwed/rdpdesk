#include "wxRDP_win.hpp"
#include "network_helper.hpp"
#include "main_window.hpp"


wxRDP::wxRDP(Main_Frame * main,RDPConn rdpsettings ,wxWindow * parent, REFCLSID clsid, wxWindowID id,
        const wxPoint& pos, const wxSize& size,long style ,	const wxString& name):
wxWindow(parent,id,pos,size,style,name)
,cnt(NULL)
,hTestHostThread(INVALID_HANDLE_VALUE)
{

	rdpconn = rdpsettings;
	HRESULT hr = ::CoCreateInstance(__uuidof(MSTSCLib::MsRdpClient), NULL, CLSCTX_INPROC_SERVER,
                    __uuidof(MSTSCLib::IMsRdpClient), (void **)&pRDP);

    if ( FAILED(hr) )
    {
        return ;
    }
	bNeedReconnect = !rdpsettings.bFullScreen; 

	cnt = new wxActiveXContainer(this,__uuidof(MSTSCLib::IMsRdpClientPtr),pRDP);
	
	main_frame = main;
	
	bFullScreen = FALSE;
	bWaitFlag = FALSE;
	Info = wxEmptyString;
	info_uniq_name = 0;

}

wxRDP::~wxRDP()
{
	if (cnt) 
	{
		cnt->Destroy(); //delete cnt; 
		cnt = NULL;
	}
	
	if (hTestHostThread != INVALID_HANDLE_VALUE)
	{
		TerminateThread(hTestHostThread,1);
		CloseHandle(hTestHostThread);
	}
	
}

void wxRDP::on_enter_focus(wxFocusEvent& event)
{
//	cnt->SetFocus();
	event.Skip();
}


void wxRDP::FullScreen(BOOL bRestore)
{

	try
	{

	MSTSCLib::IMsRdpClient* refRDP;
	HRESULT hres;
	hres = pRDP->QueryInterface(__uuidof(MSTSCLib::IMsRdpClientPtr),(void**)&refRDP);
	if (FAILED(hres)) return;


	if (bRestore)
	{
		int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
			
		refRDP->put_DesktopWidth(iScreenWidth);
		refRDP->put_DesktopHeight(iScreenHeight);

		refRDP->put_FullScreen(VARIANT_TRUE);
		refRDP->FullScreenTitle = SysAllocString(L"Full screen RDP");
					
	}
	else
	{
		if (rdpconn.bControlSize)
		{
			wxSize rdp_size = GetClientSize();
			refRDP->put_DesktopWidth(rdp_size.x);
			refRDP->put_DesktopHeight(rdp_size.y);
		}
		else
		{
			refRDP->put_DesktopWidth(rdpconn.width);
			refRDP->put_DesktopHeight(rdpconn.heigth);
		}
				
			
		refRDP->put_FullScreen(VARIANT_FALSE);
	}
	refRDP->Release();

	}
	catch(...)
	{
	}

	
}

void wxRDP::SendKey(BOOL cad)
{
	HWND hwnd = (HWND)cnt->GetHandle();
	EnableWindow(hwnd,true);
	SetActiveWindow(hwnd);

	keybd_event( VkKeyScan(VK_CONTROL),	0x1D, 0,0 );
	keybd_event( VkKeyScan(VK_MENU),0x38,0,0 );
	keybd_event( VkKeyScan(VK_DELETE),0x53, 0, 0 );
	keybd_event( VkKeyScan(VK_DELETE), 0x53, KEYEVENTF_KEYUP, 0 );
	keybd_event( VkKeyScan(VK_CONTROL), 0x1D, KEYEVENTF_KEYUP, 0 );
	keybd_event( VkKeyScan(VK_MENU), 0x38, KEYEVENTF_KEYUP, 0);
}

void wxRDP::photo()
{
	wxClientDC client_dc(this);
	wxBitmap bmp(client_dc.GetSize().GetWidth(), client_dc.GetSize().GetHeight()); 
	wxMemoryDC mdc(bmp); 
	int x_bmp = bmp.GetWidth();
	int y_bmp =  bmp.GetHeight();
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

void wxRDP::screenshot(wxRDP * rdp)
{
	wxClientDC client_dc(rdp);
	wxBitmap bmp(client_dc.GetSize().GetWidth(), client_dc.GetSize().GetHeight()); 
	wxMemoryDC mdc(bmp); 
	int x_bmp = bmp.GetWidth();
	int y_bmp =  bmp.GetHeight();
	mdc.Blit(0, 0, bmp.GetWidth(), bmp.GetHeight(), &client_dc, 0, 0); 
	mdc.SelectObject(wxNullBitmap); 
	

	wxImage im = bmp.ConvertToImage(); 
	bmp = (wxBitmap)im.Scale(0.5*x_bmp,0.5*y_bmp,wxIMAGE_QUALITY_HIGH); 
	im.Rescale(0.5*x_bmp,0.5*y_bmp,wxIMAGE_QUALITY_HIGH); 
	bmp = (wxBitmap)im;
	
	m_screenshot = bmp;
	
	wxPaintDC dc(splitter_rdp->screenshot);
	splitter_rdp->screenshot->PrepareDC(dc);
	dc.Clear(); 
	dc.DrawBitmap(m_screenshot,wxPoint(0,0));
	splitter_rdp->screenshot->Refresh();
}



DWORD WINAPI wxRDP::TestHost(wxRDP * rdp)
{
	if (!rdp) return 0;
	NetworkHelper nh;
	rdp->bWaitFlag = nh.Test(rdp->rdpconn.hostname,rdp->rdpconn.port);



	size_t Count = rdp->main_frame->nb->GetPageCount();
	for (int i = 0; i < (int)Count; i ++)
	{
		wxSplitterRDP * splitter_rdp = (wxSplitterRDP *)rdp->main_frame->nb->GetPage(i);
		if (rdp == splitter_rdp->rdp)
		{
			if (rdp->bWaitFlag)
			{
				rdp->main_frame->SetStatusText(rdp->rdpconn.hostname,2); 
								
				if (rdp->rdpconn.connection_name.Length() > 0)
				{
					rdp->main_frame->nb->SetPageText(i,rdp->rdpconn.connection_name);
				}
				else
				{
					rdp->main_frame->nb->SetPageText(i,rdp->rdpconn.hostname);
				}
				rdp->main_frame->menu_bar->EnableTop(1,true); 
				return 0;
			}
			else
			{
				rdp->main_frame->current_page_for_delete = i;
				wxUpdateUIEvent evt;
				evt.Check(true); 
				evt.SetId(ID_ERRORHOSTNAME);
				wxPostEvent(rdp->main_frame,evt); 
				return 0;
			}
		}
	}

	return 0;
	

}

void wxRDP::DispatcherActiveX(wxActiveXEvent& event)
{
	switch ( event.GetDispatchId() )
    {
	case RDP_EVENT_CONNECTED:
		
		this->splitter_rdp->switch_state();  
		 
		if (this->info_uniq_name != 0)
		{
			this->main_frame->m_panel_tree->rdptree->from_wxrdp(this->info_uniq_name,TREEDATA_INC_CONNCOUNT);  
		}
			
		this->bConnected = TRUE;

		if ((wxSplitterRDP *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->splitter_rdp)
		{
			this->main_frame->CheckCurrentConnectionMenu();
		}

		if (bNeedReconnect)
		{
			Disconnect();
		}

		main_frame->FocusCurrentPage();

		break;
	case RDP_EVENT_DISCONNECTED:
		
		this->splitter_rdp->switch_state(); 
			
		if (info_uniq_name != 0)
		{
			main_frame->m_panel_tree->rdptree->from_wxrdp(this->info_uniq_name,TREEDATA_DEC_CONNCOUNT);  
		}

		this->bConnected = FALSE;
			
		if ((wxSplitterRDP *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->splitter_rdp)
		{
			this->main_frame->CheckCurrentConnectionMenu();
		}

		if (bNeedReconnect)
		{
			bNeedReconnect = FALSE; 
			Connect();
		}

		break;

	case RDP_EVENT_ENTERFULLSCREEN:
		
		this->bFullScreen = TRUE;
			
		if ((wxSplitterRDP *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->splitter_rdp)
		{
			this->main_frame->CheckCurrentConnectionMenu();
		}
		break;

	case RDP_EVENT_LEAVEFULLSCREEN:
		
		this->bFullScreen = FALSE;
		if ((wxSplitterRDP *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->splitter_rdp)
		{
			this->main_frame->CheckCurrentConnectionMenu();
		}
		break;
	default:

		break;

	}
	event.StopPropagation(); 
}

void wxRDP::Connect()
{
	try
	{
		MSTSCLib::IMsRdpClient* refRDP;
		HRESULT hres;
		hres = pRDP->QueryInterface(__uuidof(MSTSCLib::IMsRdpClientPtr),(void**)&refRDP);
		if (FAILED(hres)) return;

		//SHORT ConnState = 0;
		//refRDP->get_Connected(&ConnState);
		//if (ConnState != 0) Disconnect();
		refRDP->Connect();
		refRDP->Release();
	}
	catch(...)
	{
	}
}

void wxRDP::Disconnect()
{
	try
	{
		MSTSCLib::IMsRdpClient* refRDP;
		HRESULT hres;
		hres = pRDP->QueryInterface(__uuidof(MSTSCLib::IMsRdpClientPtr),(void**)&refRDP);
		if (FAILED(hres)) return;
		refRDP->RequestClose();
		refRDP->Disconnect();
		refRDP->Release();
	}
	catch(...)
	{
	}
}

BOOL wxRDP::DoRdp()
{
	DWORD thId;
	hTestHostThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)this->TestHost, this,0,&thId);
	
	MSTSCLib::IMsRdpClient* refRDP;
	HRESULT hres;
	hres = pRDP->QueryInterface(__uuidof(MSTSCLib::IMsRdpClientPtr),(void**)&refRDP);
	if (FAILED(hres)) return FALSE;

	wxString conn_text(wxT("Try establish connection"));
	refRDP->put_ConnectingText(SysAllocString( conn_text.wc_str(*wxConvCurrent) )); 

	refRDP->put_Server(SysAllocString(AnsiToUnicode(rdpconn.hostname)));
	refRDP->put_UserName(SysAllocString(AnsiToUnicode(rdpconn.username)));
	
	if (rdpconn.password.Length() != NULL) 
	{
		refRDP->GetAdvancedSettings2()->put_ClearTextPassword(SysAllocString(AnsiToUnicode(rdpconn.password))); 
	}

	refRDP->put_Domain(SysAllocString(AnsiToUnicode(rdpconn.domain)));

	
	if (rdpconn.attach_to_console)
	{
		refRDP->GetAdvancedSettings2()->put_ConnectToServerConsole(VARIANT_TRUE);  
	}
	else
		refRDP->GetAdvancedSettings2()->put_ConnectToServerConsole(VARIANT_FALSE); 
		

	if (rdpconn.port.Length() == 0)	refRDP->GetAdvancedSettings2()->put_RDPPort(3389);
	else
	{
		LONG lPort;
		rdpconn.port.ToLong(&lPort);
		refRDP->GetAdvancedSettings2()->put_RDPPort(lPort);  
	}

	
	refRDP->put_ColorDepth(rdpconn.color_depth);
		
	if (rdpconn.bControlSize)
	{
		refRDP->put_DesktopWidth(0);
		refRDP->put_DesktopHeight(0);
	}
	else
	{
		if (rdpconn.bFullScreen)
		{
			refRDP->put_DesktopWidth(GetSystemMetrics(SM_CXSCREEN));
			refRDP->put_DesktopHeight(GetSystemMetrics(SM_CYSCREEN));
			refRDP->put_FullScreen(VARIANT_TRUE);
			refRDP->put_FullScreenTitle(SysAllocString(L"Full screen RDP"));
		}
		else
		{
			refRDP->put_DesktopWidth(rdpconn.width);
			refRDP->put_DesktopHeight(rdpconn.heigth);
			refRDP->put_FullScreen(VARIANT_FALSE);
		}


		if (rdpconn.bSmartSizing)
			refRDP->GetAdvancedSettings2()->put_SmartSizing(VARIANT_TRUE);
		else
			refRDP->GetAdvancedSettings2()->put_SmartSizing(VARIANT_FALSE);
	}



	
	
	
	if (rdpconn.bUseProgram)
	{
		refRDP->GetSecuredSettings2()->put_StartProgram(SysAllocString(AnsiToUnicode(rdpconn.shell))); 
		refRDP->GetSecuredSettings2()->put_WorkDir(SysAllocString(AnsiToUnicode(rdpconn.directory)));  
		if (rdpconn.bProgramMaximized)
			refRDP->GetAdvancedSettings2()->put_MaximizeShell(1);
		else
			refRDP->GetAdvancedSettings2()->put_MaximizeShell(0);
	}


	if (rdpconn.bShareDrives)
	{
		refRDP->GetAdvancedSettings2()->put_RedirectDrives(VARIANT_TRUE);
		
	}
	else
		refRDP->GetAdvancedSettings2()->put_RedirectDrives(VARIANT_FALSE);

	if (rdpconn.bSharePrinters)
		refRDP->GetAdvancedSettings2()->put_RedirectPrinters(VARIANT_TRUE);
	else
		refRDP->GetAdvancedSettings2()->put_RedirectPrinters(VARIANT_FALSE);

	if (rdpconn.bShareComPorts)
		refRDP->GetAdvancedSettings2()->put_RedirectPorts(VARIANT_TRUE);
	else
		refRDP->GetAdvancedSettings2()->put_RedirectPorts(VARIANT_FALSE);  

	if (rdpconn.bShareSmartCards)
		refRDP->GetAdvancedSettings2()->put_RedirectSmartCards(VARIANT_TRUE);
	else
		refRDP->GetAdvancedSettings2()->put_RedirectSmartCards(VARIANT_FALSE);

	refRDP->GetSecuredSettings2()->put_AudioRedirectionMode(rdpconn.SoundType);
	refRDP->GetSecuredSettings2()->put_KeyboardHookMode(rdpconn.keyboard);


	LONG lDisableList = 0;

	if (!rdpconn.bEnableWallpaper)
		lDisableList = lDisableList | (TS_PERF_DISABLE_WALLPAPER);

	if (rdpconn.bEnableAnimation)
		lDisableList = lDisableList | TS_PERF_ENABLE_ENHANCED_GRAPHICS;

	if (!rdpconn.bEnableFullWindowDrag)
		lDisableList = lDisableList | TS_PERF_DISABLE_FULLWINDOWDRAG;

	if (!rdpconn.bEnableThemes)
		lDisableList = lDisableList | TS_PERF_DISABLE_THEMING;

	refRDP->GetAdvancedSettings2()->put_PerformanceFlags(lDisableList); 

	if (rdpconn.bEnableBitmapCaching)
		refRDP->GetAdvancedSettings2()->put_BitmapPersistence(1);
	else
		refRDP->GetAdvancedSettings2()->put_BitmapPersistence(0);

	

	refRDP->Connect();

	refRDP->Release();
	return TRUE;
}
