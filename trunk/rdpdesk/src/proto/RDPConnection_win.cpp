///////////////////////////////////////////////////////////////////////////////
// File name:   RDPConnection_win.cpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-03 18:20:39" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#include "RDPConnection_win.hpp"
#include "network_helper.hpp"
#include "main_window.hpp"


RDPConnection::RDPConnection(Main_Frame * main,Options_HashMap options,wxWindow * parent, wxWindowID id,
        const wxPoint& pos, const wxSize& size,long style ,	const wxString& name):
//wxWindow(parent,id,pos,size,style,name)
//,cnt(NULL)
BasicConnection(main,options,parent,id,pos,size,style,name),
hTestHostThread(INVALID_HANDLE_VALUE)
{

	//rdpconn = rdpsettings;
	local_options = options;
	HRESULT hr = ::CoCreateInstance(__uuidof(MSTSCLib::MsRdpClient), NULL, CLSCTX_INPROC_SERVER,
                    __uuidof(MSTSCLib::IMsRdpClient), (void **)&pUnknown);

    if ( FAILED(hr) )
    {
		wxMessageBox(wxT("RDP connection is not available."),wxT("Error"),wxICON_ERROR);
        return ;
    }
	if (wxAtoi(local_options[wxT("full_screen")]))
	{
		bNeedReconnect = false;
	}
	else
	{
		bNeedReconnect = true;
	}
	cnt = (wxWindow *)(new wxActiveXContainer(this,__uuidof(MSTSCLib::IMsRdpClientPtr),pUnknown));
	if (cnt == NULL)
	{
		wxMessageBox(wxT("RDP connection is not available."),wxT("Error"),wxICON_ERROR);
	}
	else
	{
		bObjectOk = TRUE;
	}
	
}

RDPConnection::~RDPConnection()
{
//	if (cnt) 
//	{
//		cnt->Destroy(); //delete cnt; 
//		cnt = NULL;
//	}
	
	if (hTestHostThread != INVALID_HANDLE_VALUE)
	{
		TerminateThread(hTestHostThread,1);
		CloseHandle(hTestHostThread);
	}
	
}

void RDPConnection::on_enter_focus(wxFocusEvent& event)
{
//	cnt->SetFocus();
	event.Skip();
}


void RDPConnection::FullScreen(BOOL bRestore)
{

	try
	{

	MSTSCLib::IMsRdpClient* refRDP;
	HRESULT hres;
	hres = pUnknown->QueryInterface(__uuidof(MSTSCLib::IMsRdpClientPtr),(void**)&refRDP);
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
		if (wxAtoi(local_options[wxT("control_size")]))
		{
			//wxMessageBox(wxT("222"));
			wxSize rdp_size = GetClientSize();
			refRDP->put_DesktopWidth(rdp_size.x);
			refRDP->put_DesktopHeight(rdp_size.y);
			refRDP->put_FullScreen(VARIANT_FALSE);
		}
		else
		{
			//wxMessageBox(wxT("111"));
			refRDP->put_DesktopWidth(wxAtoi(local_options[wxT("width")]));
			refRDP->put_DesktopHeight(wxAtoi(local_options[wxT("heigth")]));
		}
		refRDP->put_FullScreen(VARIANT_FALSE);
	}
	refRDP->Release();
	
	}
	catch(...)
	{
	}

	
}
/*
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
*/


DWORD WINAPI RDPConnection::TestHost(RDPConnection * rdp)
{
	if (!rdp) return 0;
	NetworkHelper nh;
	rdp->bWaitFlag = nh.Test(rdp->local_options[wxT("hostname")],rdp->local_options[wxT("port")]);



	size_t Count = rdp->main_frame->nb->GetPageCount();
	for (int i = 0; i < (int)Count; i ++)
	{
		ConnSplitter * splitter_rdp = (ConnSplitter *)rdp->main_frame->nb->GetPage(i);
		if (rdp == splitter_rdp->conn)
		{
			if (rdp->bWaitFlag)
			{
				rdp->main_frame->SetStatusText(rdp->local_options[wxT("hostname")],2); 
								
				if (rdp->local_options[wxT("connection_name")].Length() > 0)
				{
					rdp->main_frame->nb->SetPageText(i,rdp->local_options[wxT("connection_name")]);
				}
				else
				{
					rdp->main_frame->nb->SetPageText(i,rdp->local_options[wxT("hostname")]);
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

void RDPConnection::DispatcherActiveX(wxActiveXEvent& event)
{

	switch ( event.GetDispatchId() )
    {
	case RDP_EVENT_CONNECTED:
		
		this->conn_splitter->switch_state();  
		
		/*
		if (this->info_uniq_name != 0)
		{
			this->main_frame->m_panel_tree->rdptree->from_wxrdp(this->info_uniq_name,TREEDATA_INC_CONNCOUNT);  
		}
		*/
		SendConnectEvent();
		this->bConnected = TRUE;
		CheckOptions();
		/*
		if ((wxSplitterRDP *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->splitter_rdp)
		{
			this->main_frame->CheckCurrentConnectionMenu();
		}
		*/

		if (bNeedReconnect)
		{
			Disconnect();
		}

		main_frame->FocusCurrentPage();

		break;
	case RDP_EVENT_DISCONNECTED:
		
		this->conn_splitter->switch_state(); 
		/*	
		if (info_uniq_name != 0)
		{
			main_frame->m_panel_tree->rdptree->from_wxrdp(this->info_uniq_name,TREEDATA_DEC_CONNCOUNT);  
		}
		*/
		SendDisconnectEvent();
		this->bConnected = FALSE;
		CheckOptions();

		/*	
		if ((wxSplitterRDP *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->splitter_rdp)
		{
			this->main_frame->CheckCurrentConnectionMenu();
		}
		*/
		if (bNeedReconnect)
		{
			bNeedReconnect = FALSE; 
			Connect();
		}

		break;

	case RDP_EVENT_ENTERFULLSCREEN:
		
		this->bFullScreen = TRUE;
		/*		
		if ((wxSplitterRDP *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->splitter_rdp)
		{
			this->main_frame->CheckCurrentConnectionMenu();
		}
		*/
		SendEnterFullscreenEvent();
		break;

	case RDP_EVENT_LEAVEFULLSCREEN:
		//FullScreen(0);
		this->bFullScreen = FALSE;
		/*
		if ((wxSplitterRDP *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->splitter_rdp)
		{
			this->main_frame->CheckCurrentConnectionMenu();
		}
		*/
		SendLeaveFullscreenEvent();
		break;
	default:
		//FullScreen(0);
		break;

	}
	event.StopPropagation(); 
}

bool RDPConnection::Connect()
{
	try
	{
		MSTSCLib::IMsRdpClient* refRDP;
		HRESULT hres;
		hres = pUnknown->QueryInterface(__uuidof(MSTSCLib::IMsRdpClientPtr),(void**)&refRDP);
		if (FAILED(hres)) return false;

		//SHORT ConnState = 0;
		//refRDP->get_Connected(&ConnState);
		//if (ConnState != 0) Disconnect();
		refRDP->Connect();
		refRDP->Release();
		
	}
	catch(...)
	{
	}
	return true;
}

void RDPConnection::Disconnect()
{
	try
	{
		MSTSCLib::IMsRdpClient* refRDP;
		HRESULT hres;
		hres = pUnknown->QueryInterface(__uuidof(MSTSCLib::IMsRdpClientPtr),(void**)&refRDP);
		if (FAILED(hres)) return;
		refRDP->RequestClose();
		refRDP->Disconnect();
		refRDP->Release();
	}
	catch(...)
	{
	}
}

bool RDPConnection::DoConnection()
{
	DWORD thId;
	hTestHostThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)this->TestHost, this,0,&thId);
	
	MSTSCLib::IMsRdpClient* refRDP;
	HRESULT hres;
	hres = pUnknown->QueryInterface(__uuidof(MSTSCLib::IMsRdpClientPtr),(void**)&refRDP);
	if (FAILED(hres)) return FALSE;

	wxString conn_text(wxT("Try establish connection"));
	refRDP->put_ConnectingText(SysAllocString( conn_text.wc_str(*wxConvCurrent) )); 

	refRDP->put_Server(SysAllocString(AnsiToUnicode(local_options[wxT("hostname")])));
	refRDP->put_UserName(SysAllocString(AnsiToUnicode(local_options[wxT("username")])));
	
	if (local_options[wxT("password")] != wxT("")) 
	{
		refRDP->GetAdvancedSettings2()->put_ClearTextPassword(SysAllocString(AnsiToUnicode(local_options[wxT("password")]))); 
	}

	refRDP->put_Domain(SysAllocString(AnsiToUnicode(local_options[wxT("domain")])));

	
	if (wxAtoi(local_options[wxT("attach_to_console")]))
	{
		refRDP->GetAdvancedSettings2()->put_ConnectToServerConsole(VARIANT_TRUE);  
	}
	else
		refRDP->GetAdvancedSettings2()->put_ConnectToServerConsole(VARIANT_FALSE); 
		

	if (local_options[wxT("port")].Length() == 0)	refRDP->GetAdvancedSettings2()->put_RDPPort(3389);
	else
	{
		LONG lPort;
		local_options[wxT("port")].ToLong(&lPort);
		refRDP->GetAdvancedSettings2()->put_RDPPort(lPort);  
	}

	wxString Choises[] = {_("8"),_("15"),_("16"),_("24")};
	refRDP->put_ColorDepth(wxAtoi(Choises[wxAtoi(local_options[wxT("color_depth")])]));
		
	if (wxAtoi(local_options[wxT("control_size")]))
	{
		refRDP->put_DesktopWidth(0);
		refRDP->put_DesktopHeight(0);
	}
	else
	{
		if (wxAtoi(local_options[wxT("full_screen")]))
		{
			refRDP->put_DesktopWidth(GetSystemMetrics(SM_CXSCREEN));
			refRDP->put_DesktopHeight(GetSystemMetrics(SM_CYSCREEN));
			refRDP->put_FullScreen(VARIANT_TRUE);
			refRDP->put_FullScreenTitle(SysAllocString(L"Full screen RDP"));
		}
		else
		{
			refRDP->put_DesktopWidth(wxAtoi(local_options[wxT("width")]));
			refRDP->put_DesktopHeight(wxAtoi(local_options[wxT("heigth")]));
			refRDP->put_FullScreen(VARIANT_FALSE);
		}


		if (wxAtoi(local_options[wxT("smart_sizing")]))
			refRDP->GetAdvancedSettings2()->put_SmartSizing(VARIANT_TRUE);
		else
			refRDP->GetAdvancedSettings2()->put_SmartSizing(VARIANT_FALSE);
	}



	
	
	
	if (wxAtoi(local_options[wxT("use_program")]))
	{
		refRDP->GetSecuredSettings2()->put_StartProgram(SysAllocString(AnsiToUnicode(local_options[wxT("shell")]))); 
		refRDP->GetSecuredSettings2()->put_WorkDir(SysAllocString(AnsiToUnicode(local_options[wxT("directory")])));  
		if (wxAtoi(local_options[wxT("program_maximized")]))
			refRDP->GetAdvancedSettings2()->put_MaximizeShell(1);
		else
			refRDP->GetAdvancedSettings2()->put_MaximizeShell(0);
	}


	if (wxAtoi(local_options[wxT("share_drives")]))
	{
		refRDP->GetAdvancedSettings2()->put_RedirectDrives(VARIANT_TRUE);
		
	}
	else
		refRDP->GetAdvancedSettings2()->put_RedirectDrives(VARIANT_FALSE);

	if (wxAtoi(local_options[wxT("share_printers")]))
		refRDP->GetAdvancedSettings2()->put_RedirectPrinters(VARIANT_TRUE);
	else
		refRDP->GetAdvancedSettings2()->put_RedirectPrinters(VARIANT_FALSE);

	if (wxAtoi(local_options[wxT("share_com_ports")]))
		refRDP->GetAdvancedSettings2()->put_RedirectPorts(VARIANT_TRUE);
	else
		refRDP->GetAdvancedSettings2()->put_RedirectPorts(VARIANT_FALSE);  

	if (wxAtoi(local_options[wxT("share_smart_cards")]))
		refRDP->GetAdvancedSettings2()->put_RedirectSmartCards(VARIANT_TRUE);
	else
		refRDP->GetAdvancedSettings2()->put_RedirectSmartCards(VARIANT_FALSE);

	refRDP->GetSecuredSettings2()->put_AudioRedirectionMode(wxAtoi(local_options[wxT("sound_type")]));
	refRDP->GetSecuredSettings2()->put_KeyboardHookMode(wxAtoi(local_options[wxT("keyboard")]));


	LONG lDisableList = 0;

	if (!wxAtoi(local_options[wxT("enable_wallpaper")]))
		lDisableList = lDisableList | (TS_PERF_DISABLE_WALLPAPER);

	if (wxAtoi(local_options[wxT("enable_animation")]))
		lDisableList = lDisableList | TS_PERF_ENABLE_ENHANCED_GRAPHICS;

	if (!wxAtoi(local_options[wxT("enable_full_window_drag")]))
		lDisableList = lDisableList | TS_PERF_DISABLE_FULLWINDOWDRAG;

	if (!wxAtoi(local_options[wxT("enable_themes")]))
		lDisableList = lDisableList | TS_PERF_DISABLE_THEMING;

	refRDP->GetAdvancedSettings2()->put_PerformanceFlags(lDisableList); 

	if (wxAtoi(local_options[wxT("enable_bitmap_caching")]))
		refRDP->GetAdvancedSettings2()->put_BitmapPersistence(1);
	else
		refRDP->GetAdvancedSettings2()->put_BitmapPersistence(0);

	

	refRDP->Connect();

	refRDP->Release();
	return TRUE;
}



void RDPConnection::SendKey(BOOL cad)
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

void RDPConnection::GrabAll(BOOL state)
{
}

void RDPConnection::CentreConnection()
{
}

bool RDPConnection::DisconnectClose()
{
	Disconnect();
	return true;
}
