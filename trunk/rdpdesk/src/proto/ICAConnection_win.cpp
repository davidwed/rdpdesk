///////////////////////////////////////////////////////////////////////////////
// File name:   ICAConnection_win.cpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-12-08 19:14:55"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include "ICAConnection_win.hpp"
#include "main_window.hpp"

#include "network_helper.hpp"


BEGIN_EVENT_TABLE(ICAConnection,wxWindow)
EVT_ACTIVEX(wxID_ANY,ICAConnection::DispatcherActiveX)
EVT_SET_FOCUS(ICAConnection::on_enter_focus)
END_EVENT_TABLE()

inline wxString bstr2wx(const _bstr_t& bstr)
{
   return wxString(static_cast<const wchar_t *>(bstr));
}

inline _bstr_t wx2bstr(const wxString& str)
{
   return _bstr_t(str.wc_str(*wxConvCurrent));
}

ICAConnection::ICAConnection(MainFrame * main, Options_HashMap options ,wxWindow * parent, wxWindowID id,
			     const wxPoint& pos, const wxSize& size,long style ,	const wxString& name):
//wxWindow(parent,id,pos,size,style,name)
//,cnt(NULL)
   BasicConnection(main,options,parent,id,pos,size,style,name),
   hTestHostThread(INVALID_HANDLE_VALUE)
{

	local_options = options;
   HRESULT hr = ::CoCreateInstance(__uuidof(WFICALib::ICAClient), NULL, CLSCTX_INPROC_SERVER,
				   __uuidof(WFICALib::IICAClient), (void **)&pUnknown);

   if ( FAILED(hr) )
   {
      wxMessageBox(_("ICA connection is not available. Please install ICA client from http://www.citrix.com"),_("Error"),wxICON_ERROR);
      return ;
   }


//	bNeedReconnect = !rdpsettings.bFullScreen;

   cnt = new wxActiveXContainer(this,__uuidof(WFICALib::IICAClientPtr),pUnknown);
   if (cnt == NULL)
   {
	   wxMessageBox(_("ICA connection is not available. Please install ICA client from http://www.citrix.com"),_("Error"),wxICON_ERROR);
   }
   else
   {
      bObjectOk = TRUE;
   }
}

ICAConnection::~ICAConnection()
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

void ICAConnection::FullScreen(BOOL bRestore)
{
	try
	{

		WFICALib::IICAClient * refICA;
		HRESULT hres;
		hres = pUnknown->QueryInterface(__uuidof(WFICALib::IICAClientPtr),(void**)&refICA);
		if (FAILED(hres)) return;


		if (bRestore)
		{
			//int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
			//int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

			//refRDP->put_DesktopWidth(iScreenWidth);
			//refRDP->put_DesktopHeight(iScreenHeight);

			refICA->FullScreenWindow();
			//refICA->FullScreenWindow();
			//refICA->ShowTitleBar();
			//refRDP->put_FullScreen(VARIANT_TRUE);
			//refRDP->FullScreenTitle = SysAllocString(L"Full screen RDP");

		}
		else if (this->bFullScreen)
		{
			if (wxAtoi(local_options[wxT("control_size")]))
			{
				refICA->ScaleToFit();
				wxSize sz_window = GetClientSize();
				refICA->SetWindowSize(WFICALib::WindowTypeContainer ,(long)sz_window.x ,(long)sz_window.y ,0);
			}
			else
			{
				//wxMessageBox(wxT("2"));
				refICA->SetWindowSize(WFICALib::WindowTypeControl,(long)wxAtoi(local_options[wxT("width")]),(long)wxAtoi(local_options[wxT("heigth")]),0);
				refICA->SetProp(wx2bstr(wxT("Width")), wx2bstr(wxString::Format(wxT("%i"),wxAtoi(local_options[wxT("width")]))));
				refICA->SetProp(wx2bstr(wxT("Height")), wx2bstr(wxString::Format(wxT("%i"),wxAtoi(local_options[wxT("heigth")]))));
			}
			refICA->HideTitleBar();
			refICA->DockWindow();

		}
		refICA->Release();

	}
	catch(...)
	{
	}
}

bool ICAConnection::Connect()
{
   try
   {
      //MSTSCLib::IMsRdpClient* refRDP;
      WFICALib::IICAClient * refICA;
      HRESULT hres;
      //hres = pRDP->QueryInterface(__uuidof(MSTSCLib::IMsRdpClientPtr),(void**)&refRDP);
      hres = pUnknown->QueryInterface(__uuidof(WFICALib::IICAClientPtr),(void**)&refICA);
      if (FAILED(hres)) return false;

      //SHORT ConnState = 0;
      //refRDP->get_Connected(&ConnState);
      //if (ConnState != 0) Disconnect();
      refICA->Connect();
      refICA->Release();
   }
   catch(...)
   {
   }
   return true;
}

void ICAConnection::Disconnect()
{
   try
   {
      //MSTSCLib::IMsRdpClient* refRDP;
      WFICALib::IICAClient * refICA;
      HRESULT hres;
      //hres = pRDP->QueryInterface(__uuidof(MSTSCLib::IMsRdpClientPtr),(void**)&refRDP);
      hres = pUnknown->QueryInterface(__uuidof(WFICALib::IICAClientPtr),(void**)&refICA);
      if (FAILED(hres)) return;
      refICA->Logoff();
      refICA->Disconnect();
      refICA->Release();
   }
   catch(...)
   {
   }
}




bool ICAConnection::DoConnection()
{
   DWORD thId;
   hTestHostThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)this->TestHost, this,0,&thId);


   cnt->CenterOnParent();
   cnt->Show(false);
   WFICALib::IICAClient * refICA;
   HRESULT hres;
   hres = pUnknown->QueryInterface(__uuidof(WFICALib::IICAClientPtr),(void**)&refICA);
   if (FAILED(hres)) return FALSE;

   refICA->ClearProps();
   refICA->ResetProps();

   // General Settings
   refICA->Address = wx2bstr(local_options[wxT("hostname")]);

//	refICA->Domain = SysAllocString(AnsiToUnicode(rdpconn.domain));
//  refICA->Username = SysAllocString(AnsiToUnicode(rdpconn.username));
   if (wxAtoi(local_options[wxT("custom_port")]))
   {
	   if (local_options[wxT("port")] != wxT("1494"))
	   {
		   long long_port;
		   local_options[wxT("port")].ToLong(&long_port);
		   if (long_port > 0 && long_port < 65536)
		   {
			   refICA->put_ICAPortNumber(long_port);
		   }
	   }
   }
   if (local_options[wxT("password")] != wxT(""))
   {
      refICA->SetProp(wx2bstr(wxT("ClearPassword")), wx2bstr(local_options[wxT("password")]));
      refICA->put_AutoLogonAllowed(VARIANT_TRUE);
   }
   if (local_options[wxT("username")] != wxT(""))
   {
	   refICA->Username = SysAllocString(AnsiToUnicode(local_options[wxT("username")]));
   }
   if (local_options[wxT("domain")] != wxT(""))
   {
	   refICA->Domain = SysAllocString(AnsiToUnicode(local_options[wxT("domain")]));
   }
   //if (m_text_proxy_autoconfig_url != NULL)
   //{
   //   m_text_proxy_autoconfig_url->SetValue(local_options[wxT("proxy_autoconfig_url"]));
   //}
   //wxString proxy_type_array[] = {wxT("None") ,wxT("Auto"), wxT("Socks"), wxT("SocksV4"),
//						wxT("SocksV5"), wxT("Secure"), wxT("Script")};
   if ((wxAtoi(local_options[wxT("proxy_type")]) == 2) ||
		(wxAtoi(local_options[wxT("proxy_type")]) == 3) ||
		(wxAtoi(local_options[wxT("proxy_type")]) == 4))
   {
	   //wxMessageBox(wxT("proxy type = 1"));
	   refICA->SetProp(wx2bstr(wxT("UseAlternateAddress")), wx2bstr(wxT("ON")));
	   if (local_options[wxT("proxy_host"]) != wxT(""))
	   {
		   refICA->ICASOCKSProxyHost = wx2bstr(local_options[wxT("proxy_host"]));
		   long proxy_port = 1080;
		   /*if (rdpconn.ProxyPort.Length() > 0)
		   {
			   rdpconn.ProxyPort.ToLong(&proxy_port);
		   }*/
		   refICA->ICASOCKSProxyPortNumber = (proxy_port <= 0 || proxy_port > 65536) ? 1080 : proxy_port;
		   refICA->SetProp(wx2bstr(wxT("ICASOCKSRFC1929Username")),wx2bstr(local_options[wxT("proxy_username")]));
		   refICA->SetProp(wx2bstr(wxT("ICASOCKSRFC1929Password")),wx2bstr(local_options[wxT("proxy_password")]));
	   }
   }
   else if ((wxAtoi(local_options[wxT("proxy_type")]) == 5))
   {
	   refICA->SetProp(wx2bstr(wxT("UseAlternateAddress")), wx2bstr(wxT("ON")));
	   refICA->SSLProxyHost = wx2bstr(local_options[wxT("ssl_proxy_host"]));
	   refICA->SetProp(wx2bstr(wxT("SSLCACert0")),wx2bstr(local_options[wxT("proxy_username")]));
   }
   //if (m_text_proxy_bypass_list != NULL)
   //{
   //   m_text_proxy_bypass_list->SetValue(local_options[wxT("proxy_bypass_list"]));
   //}
   //if (m_spinctrl_proxy_timeout != NULL)
   //{
   //   m_spinctrl_proxy_timeout->SetValue(wxAtoi(local_options[wxT("proxy_timeout"])));
   //}
   //if (m_text_ssl_proxy_host != NULL)
   //{
   //   m_text_ssl_proxy_host->SetValue(local_options[wxT("ssl_proxy_host"]));
   //}
   //if (m_checkbox_ssl_enable != NULL)
   //{
   //   m_checkbox_ssl_enable->SetValue(wxAtoi(local_options[wxT("ssl_enable"])));
   //}
   //if (m_checkbox_proxy_use_default != NULL)
   //{
   //   m_checkbox_proxy_use_default->SetValue(wxAtoi(local_options[wxT("proxy_use_default"])));
   //}
   if (wxAtoi(local_options[wxT("desired_color")]))
   {
	   if (local_options[wxT("color_depth")] == wxT("0"))
	   {
		   refICA->DesiredColor = WFICALib::Color256;
	   }
	   else if ((local_options[wxT("color_depth")] == wxT("1")) ||
		   (local_options[wxT("color_depth")] == wxT("2")))
	   {
		   refICA->DesiredColor = WFICALib::Color16Bit;
	   }
	   else if (local_options[wxT("color_depth")] == wxT("3"))
	   {
		   refICA->DesiredColor = WFICALib::Color24Bit;
	   }
	   else
	   {
		   refICA->DesiredColor = WFICALib::Color16Bit;
	   }
   }
   if (wxAtoi(local_options[wxT("control_size")]))
   {
	   refICA->ScaleToFit();
	   wxSize sz_window = GetClientSize();
	   refICA->SetWindowSize(WFICALib::WindowTypeControl ,(long)sz_window.x ,(long)sz_window.y ,0);
   }
   else
   {
	   if (wxAtoi(local_options[wxT("full_screen")]))
	   {
		   refICA->SetProp(wx2bstr(wxT("Width")), wx2bstr(wxString::Format(wxT("%i"),GetSystemMetrics(SM_CXSCREEN))));
		   refICA->SetProp(wx2bstr(wxT("Height")), wx2bstr(wxString::Format(wxT("%i"),GetSystemMetrics(SM_CYSCREEN))));
		   refICA->SetProp(wx2bstr(wxT("DesiredHRes")), wx2bstr(wxString::Format(wxT("%i"),GetSystemMetrics(SM_CXSCREEN))));
		   refICA->SetProp(wx2bstr(wxT("DesiredVRes")), wx2bstr(wxString::Format(wxT("%i"),GetSystemMetrics(SM_CYSCREEN))));
		   refICA->FullScreenWindow();
	   }
	   else
	   {
		   refICA->SetWindowSize(WFICALib::WindowTypeControl,(long)wxAtoi(local_options[wxT("width")]),(long)wxAtoi(local_options[wxT("heigth")]),0);
		   refICA->SetProp(wx2bstr(wxT("Width")), wx2bstr(wxString::Format(wxT("%i"),wxAtoi(local_options[wxT("width")]))));
		   refICA->SetProp(wx2bstr(wxT("Height")), wx2bstr(wxString::Format(wxT("%i"),wxAtoi(local_options[wxT("heigth")]))));
	   }
   }
   if (wxAtoi(local_options[wxT("resize_session")]))
   {
	   refICA->ScaleEnable();
	   refICA->AutoScale = VARIANT_TRUE;
	   refICA->SetProp(wx2bstr(wxT("ScalingMode")), wx2bstr(wxT("3")));
   }
   else
   {
	   refICA->ScaleDisable();
   }

   //if (m_checkbox_allow_degraded_colors != NULL)
   //{
   //   local_options[wxT("allow_degraded_colors")] = wxString::Format(wxT("%i"),
			//					     m_checkbox_allow_degraded_colors->GetValue());
   //}
   //if (m_checkbox_enable_oss != NULL)
   //{
   //   local_options[wxT("enable_oss")] = wxString::Format(wxT("%i"),
			//				  m_checkbox_enable_oss->GetValue());
   //}
   if (wxAtoi(local_options[wxT("enable_session_sharing_screen")]))
   {
	   refICA->SetProp(wx2bstr(wxT("EnableSessionSharingClient")), wx2bstr(wxT("On")));
   }
   //if (m_checkbox_approximate_colors != NULL)
   //{
   //   local_options[wxT("approximate_colors")] = wxString::Format(wxT("%i"),
			//					  m_checkbox_approximate_colors->GetValue());
   //}
   //if (m_combo_use_sdlvb != NULL)
   //{
   //   local_options[wxT("use_sdlvb")] = wxString::Format(wxT("%i"),
			//				 m_combo_use_sdlvb->GetCurrentSelection());
   //}

   //if (m_combo_font_smoothing_type != NULL)
   //{
   //   local_options[wxT("font_smoothing_type")] = wxString::Format(wxT("%i"),
			//					   m_combo_font_smoothing_type->GetCurrentSelection());
   //}
	if (wxAtoi(local_options[wxT("screen_percent")]))
	{
		refICA->SetProp(wx2bstr(wxT("ScreenPercent")), wx2bstr(local_options[wxT("screen_percent")]));
	}

	refICA->SetProp(wx2bstr(wxT("Hotkey4Char")), wx2bstr(wxT("F8")));
	refICA->SetProp(wx2bstr(wxT("Hotkey4Shift")), wx2bstr(wxT("Alt")));

	if(wxAtoi(local_options[wxT("share_drives")]))
	{
		refICA->SetProp(wx2bstr(wxT("CDMAllowed")), wx2bstr(wxT("ON")));
	}
	else
	{
		refICA->SetProp(wx2bstr(wxT("CDMAllowed")), wx2bstr(wxT("OFF")));
	}
	if (wxAtoi(local_options[wxT("share_com_ports")]))
	{
		refICA->SetProp(wx2bstr(wxT("COMAllowed")), wx2bstr(wxT("ON")));
	}
	else
	{
		refICA->SetProp(wx2bstr(wxT("COMAllowed")), wx2bstr(wxT("OFF")));
	}
	if (wxAtoi(local_options[wxT("share_printers")]))
	{
		refICA->SetProp(wx2bstr(wxT("CPMAllowed")), wx2bstr(wxT("ON")));
	}
	else
	{
		refICA->SetProp(wx2bstr(wxT("CPMAllowed")), wx2bstr(wxT("OFF")));
	}
	if (wxAtoi(local_options[wxT("client_audio")]))
	{
		refICA->ClientAudio = VARIANT_TRUE;
		if (wxAtoi(local_options[wxT("audio_bandwidth_limit")]) == 2) 	refICA->AudioBandwidthLimit = WFICALib::SoundQualityLow;
		else if (wxAtoi(local_options[wxT("audio_bandwidth_limit")]) == 1) 	refICA->AudioBandwidthLimit = WFICALib::SoundQualityMedium;
		else if (wxAtoi(local_options[wxT("audio_bandwidth_limit")]) == 0) 	refICA->AudioBandwidthLimit = WFICALib::SoundQualityHigh;
		else 	refICA->AudioBandwidthLimit = WFICALib::SoundQualityMedium;
	}
	else
	{
		refICA->ClientAudio = VARIANT_FALSE;
	}
	if (wxAtoi(local_options[wxT("start_initial_program")]))
	{
		refICA->AutoAppResize = VARIANT_TRUE;
		refICA->put_WorkDirectory(wx2bstr(local_options[wxT("work_directory")]));
		refICA->put_InitialProgram(wx2bstr(local_options[wxT("initial_program")]));
		refICA->put_LongCommandLine(wx2bstr(local_options[wxT("long_command_line")]));
	}

   if (wxAtoi(local_options[wxT("compress")]))
   {
	   refICA->Compress = VARIANT_TRUE;
   }
   //if (m_checkbox_allow_duplicate_sessions != NULL)
   //{
   //   local_options[wxT("allow_duplicate_sessions")] = wxString::Format(wxT("%i"),
			//						m_checkbox_allow_duplicate_sessions->GetValue());
   //}
   if (wxAtoi(local_options[wxT("persistent_cache_enabled")]))
   {
	   refICA->PersistentCacheEnabled = VARIANT_TRUE;
   }
   if (wxAtoi(local_options[wxT("desktop_restart_allowed")]))
   {
	   //refICA->SessionEndAction(WFICALib::SessionEndRestart);
	   refICA->put_SessionEndAction(WFICALib::SessionEndRestart);
   }
   //if (m_checkbox_mouse_sends_control_v != NULL)
   //{
   //   local_options[wxT("mouse_sends_control_v")] = wxString::Format(wxT("%i"),
			//					     m_checkbox_mouse_sends_control_v->GetValue());
   //}
   //if (m_checkbox_echo_shift_keys != NULL)
   //{
   //   local_options[wxT("echo_shift_keys")] = wxString::Format(wxT("%i"),
			//				       m_checkbox_echo_shift_keys->GetValue());
   //}
   //if (m_checkbox_enable_audio_input != NULL)
   //{
   //   local_options[wxT("enable_audio_input")] = wxString::Format(wxT("%i"),
			//					  m_checkbox_enable_audio_input->GetValue());
   //}
   if (wxAtoi(local_options[wxT("encryption_level_session")]))
   {
        refICA->Encrypt = VARIANT_TRUE;
      if (wxAtoi(local_options[wxT("encryption_level_session")]) == 1) refICA->SetProp(wx2bstr(wxT("EncryptionLevelSession")), wx2bstr(wxT("Encrypt")));
      else if (wxAtoi(local_options[wxT("encryption_level_session")]) == 2) refICA->SetProp(wx2bstr(wxT("EncryptionLevelSession")), wx2bstr(wxT("EncRC5-0")));
      else if (wxAtoi(local_options[wxT("encryption_level_session")]) == 3) refICA->SetProp(wx2bstr(wxT("EncryptionLevelSession")), wx2bstr(wxT("EncRC5-40")));
      else if (wxAtoi(local_options[wxT("encryption_level_session")]) == 4) refICA->SetProp(wx2bstr(wxT("EncryptionLevelSession")), wx2bstr(wxT("EncRC5-56")));
      else if (wxAtoi(local_options[wxT("encryption_level_session")]) == 5) refICA->SetProp(wx2bstr(wxT("EncryptionLevelSession")), wx2bstr(wxT("EncRC5-128")));
      else refICA->SetProp(wx2bstr(wxT("EncryptionLevelSession")), wx2bstr(wxT("Encrypt")));
   }
   //if (m_combo_secure_channel_protocol != NULL)
   //{
	  //  wxString secure_channel_protocol[] = {wxT("ssl"), wxT("tls"), wxT("detect"), };
   //   local_options[wxT("secure_channel_protocol")] = wxString::Format(wxT("%i"),
   //								       m_combo_secure_channel_protocol->GetCurrentSelection());
   //}


//
////Program settings
//   if (rdpconn.bUseProgram)
//   {
//      refICA->AutoAppResize = VARIANT_TRUE;
//      refICA->put_WorkDirectory(wx2bstr(rdpconn.directory));
//      refICA->put_InitialProgram(wx2bstr(rdpconn.shell));
//   }
//
////Performance settings
//   if (rdpconn.bEnableBitmapCaching)
//   {
//      refICA->PersistentCacheEnabled = VARIANT_TRUE;
//   }
//   else
//   {
//      refICA->PersistentCacheEnabled = VARIANT_FALSE;
//   }
//
//// ICA Settings
//
//   if (rdpconn.server_ini.Length() > 0)
//   {
//      refICA->WfclientIni = wx2bstr(rdpconn.server_ini);
//   }
//   if (rdpconn.client_ini.Length() > 0)
//   {
//      refICA->AppsrvIni = wx2bstr(rdpconn.server_ini);
//   }
//   if (rdpconn.ica_file.Length() > 0)
//   {
//      refICA->ICAFile = wx2bstr(rdpconn.ica_file);
//   }
//
//   if (rdpconn.bIcaEncryption)
//   {	wxMessageBox(wxT("OK"));
//      refICA->Encrypt = VARIANT_TRUE;
//
//      if (rdpconn.ica_encryption == 0) refICA->SetProp(wx2bstr(wxT("EncryptionLevelSession")), wx2bstr(wxT("Encrypt")));
//      else if (rdpconn.ica_encryption == 1) refICA->SetProp(wx2bstr(wxT("EncryptionLevelSession")), wx2bstr(wxT("EncRC5-0")));
//      else if (rdpconn.ica_encryption == 2) refICA->SetProp(wx2bstr(wxT("EncryptionLevelSession")), wx2bstr(wxT("EncRC5-40")));
//      else if (rdpconn.ica_encryption == 3) refICA->SetProp(wx2bstr(wxT("EncryptionLevelSession")), wx2bstr(wxT("EncRC5-56")));
//      else if (rdpconn.ica_encryption == 4) refICA->SetProp(wx2bstr(wxT("EncryptionLevelSession")), wx2bstr(wxT("EncRC5-128")));
//      else refICA->SetProp(wx2bstr(wxT("EncryptionLevelSession")), wx2bstr(wxT("Encrypt")));
//
//   }
//   else
//   {
//      refICA->Encrypt = VARIANT_FALSE;
//   }
//
//   if (rdpconn.bUseApplication)
//   {
//      refICA->TCPBrowserAddress = wx2bstr(rdpconn.hostname);
//      refICA->Application = wx2bstr(rdpconn.IcaApplication);
//   }
//


   refICA->Launch = VARIANT_FALSE;
   refICA->PutTitle(wx2bstr(_("ICA Citrix connection")));
   refICA->PutTransportDriver(wx2bstr(wxT("TCP/IP")));

   refICA->Connect();
  //refICA->FocusWindow();
   //refICA->Focus();
   refICA->Release();

   cnt->CenterOnParent();
   cnt->Show(true);

   //refICA->EncryptionLevelSession = SysAllocString(AnsiToUnicode(wxT("Basic")));

   //refICA->SetProp("ScalingMode", "3");
   //refICA->SetProp(SysAllocString(AnsiToUnicode(wxT("Width"))), "800");
   //refICA->SetProp(SysAllocString(AnsiToUnicode(wxT("Height"))), "600");

   long ltemp = 0;


   //refICA->SetWindowSize(WFICALib::WindowTypeControl,800,600,0);
   //refICA->SetProp(wx2bstr(wxT("Width")), wx2bstr(wxT("800")));
   //refICA->SetProp(wx2bstr(wxT("Height")), wx2bstr(wxT("600")));

   //refICA->SetWindowSize(WFICALib::WindowTypeControl,(long)rdpconn.width,(long)rdpconn.heigth,0);
   //refICA->SetProp(wx2bstr(wxT("Width")), wx2bstr(wxString::Format(wxT("%i"),rdpconn.width)));
   //refICA->SetProp(wx2bstr(wxT("Height")), wx2bstr(wxString::Format(wxT("%i"),rdpconn.heigth)));





/*WFICALib::WindowTypeContainer*/
/*
	if (rdpconn.bControlSize)
	{
	refICA->ScaleToFit();
	wxSize sz_window = GetClientSize();
	refICA->SetWindowSize(WFICALib::WindowTypeControl ,(long)sz_window.x ,(long)sz_window.y ,0);
	}
	else
	{
	refICA->SetWindowSize(WFICALib::WindowTypeControl,(long)rdpconn.width,(long)rdpconn.heigth,0);
	refICA->SetProp(wx2bstr(wxT("Width")), wx2bstr(wxString::Format(wxT("%i"),rdpconn.width)));
	refICA->SetProp(wx2bstr(wxT("Height")), wx2bstr(wxString::Format(wxT("%i"),rdpconn.heigth)));

	}
*/
   //if (refICA->IsPassThrough() == VARIANT_TRUE)
   //{
   //	wxMessageBox(wxT("PassThrough"));
   //}


//	refICA->DesiredVRes = 1000;
//	refICA->DesiredHRes = 750;
//	else
//	{
//		refICA->SetProp(wx2bstr(wxT("Autologon​Allowed")), wx2bstr(wxT("OFF")));
//	}

/*
  if (rdpconn.bSmartSizing)
  {
  //	refICA->SetProp("ScalingMode", "3");
  refICA->ScaleEnable();
  }
*/
   //refICA->Width = 800;
   //refICA->Height = 600;

   //WFICALib::IICAClient::DisplayWindow
   //	refICA->ConnectionEntry =  wx2bstr(rdpconn.password);


   //refICA->PutICAPortNumber(1494);
   //  refICA->Launch = VARIANT_FALSE;
   //refICA->PutLocTCPBrowserAddress(wx2bstr(wxT("1494")));
   //refICA->TransportDriver = wx2bstr(wxT("TCP/IP"));
   //wxMessageBox(wxT("OK"));
   //wxMessageBox(bstr2wx(refICA->GetTransportDriver()));

   //wxMessageBox(bstr2wx(refICA->GetTransportDriver()));
   // refICA->TransportDriver = SysAllocString(AnsiToUnicode(wxT("TCP/IP")));

/*
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

  refRDP->Release();*/
   return TRUE;
}


void ICAConnection::on_enter_focus(wxFocusEvent& event)
{
//	cnt->SetFocus();
   event.Skip();
}

void ICAConnection::DispatcherActiveX(wxActiveXEvent& event)
{
	//wxMessageBox(wxT("ICA event"));
	//wxMessageBox(wxString::Format(wxT("ICA Event : %i"),event.GetDispatchId()));
	switch ( event.GetDispatchId() )
	{

	case ICA_EVENT_CONNECTED:

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
		//if (bNeedReconnect)
		//{
		//	Disconnect();
		//}

		main_frame->FocusCurrentPage();

		break;
	case ICA_EVENT_DISCONNECTED:

		this->conn_splitter->switch_state();
		/*
		if (info_uniq_name != 0)
		{
		main_frame->m_panel_tree->rdptree->from_wxrdp(this->info_uniq_name,TREEDATA_DEC_CONNCOUNT);
		}
		*/
		SendDisconnectEvent();
		this->bConnected = FALSE;
		//wxCommandEvent eventCustom(ID_FULLSCREEN);
		//wxPostEvent(main_frame, eventCustom);

		//if (this->bFullScreen == true)
		//{
			//wxCommandEvent event;
			//event.SetId(ID_FULLSCREEN);
		//}

		CheckOptions();
		/*
		if ((wxSplitterRDP *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->splitter_rdp)
		{
		this->main_frame->CheckCurrentConnectionMenu();
		}
		*/
		//if (bNeedReconnect)
		//{
		//	bNeedReconnect = FALSE;
		//	Connect();
		//}

		break;

	case ICA_EVENT_ENTERFULLSCREEN:
		//wxMessageBox(wxT("ICA_EVENT_ENTERFULLSCREEN1"));

		this->bFullScreen = TRUE;

		//if ((wxSplitterRDP *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->splitter_rdp)
		//{
		//	this->main_frame->CheckCurrentConnectionMenu();
		//}
		break;
	case 21:
		//wxMessageBox(wxT("ICA_EVENT_LEAVEFULLSCREEN2"));

		FullScreen(0);
		//if ((wxSplitterRDP *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->splitter_rdp)
		//{
		//	this->main_frame->CheckCurrentConnectionMenu();
		//}
		this->bFullScreen = FALSE;
		break;

	case ICA_EVENT_LEAVEFULLSCREEN:
		//wxMessageBox(wxT("ICA_EVENT_LEAVEFULLSCREEN"));

		FullScreen(0);
		//if ((wxSplitterRDP *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->splitter_rdp)
		//{
		//	this->main_frame->CheckCurrentConnectionMenu();
		//}
		this->bFullScreen = FALSE;
		break;
	default:
		//wxMessageBox(wxString::Format(wxT("%i"), event.GetDispatchId()));
		break;

	}
	event.StopPropagation();
}




DWORD WINAPI ICAConnection::TestHost(ICAConnection * ica)
{
	if (!ica) return 0;
	NetworkHelper nh;
	ica->bWaitFlag = nh.Test(ica->local_options[wxT("hostname")],ica->local_options[wxT("port")]);



	size_t Count = ica->main_frame->nb->GetPageCount();
	for (int i = 0; i < (int)Count; i ++)
	{
		ConnSplitter * conn_splitter = (ConnSplitter *)ica->main_frame->nb->GetPage(i);
		if (ica == conn_splitter->conn)
		{
			if (ica->bWaitFlag)
			{
				ica->main_frame->SetStatusText(ica->local_options[wxT("hostname")],2);

				if (ica->local_options[wxT("connection_name")] != wxT(""))
				{
					ica->main_frame->nb->SetPageText(i,ica->local_options[wxT("connection_name")]);
				}
				else
				{
					ica->main_frame->nb->SetPageText(i,ica->local_options[wxT("hostname")]);
				}
				ica->main_frame->menu_bar->EnableTop(1,true);
				return 0;
			}
			else
			{
				ica->main_frame->current_page_for_delete = i;
				wxUpdateUIEvent evt;
				evt.Check(true);
				evt.SetId(ID_ERRORHOSTNAME);
				wxMessageBox(_("This computer can't connect to the remote server. Type the server name or IP address again, and then try connecting. If the problem continues, contact your network administrator."));
				wxPostEvent(ica->main_frame,evt);
				return 0;
			}
		}
	}

	return 0;


}


void ICAConnection::SendKey(BOOL cad)
{
   try
   {
      WFICALib::IICAClient * refICA;
      HRESULT hres;
      hres = pUnknown->QueryInterface(__uuidof(WFICALib::IICAClientPtr),(void**)&refICA);
      if (FAILED(hres)) return;

      HWND hwnd = (HWND)cnt->GetHandle();
      EnableWindow(hwnd,true);
      SetActiveWindow(hwnd);
      //SetForegroundWindow(hwnd);
      //refICA->FocusWindow();
      //keybd_event( VkKeyScan(TEXT('A')),0x1E, 0, 0 );
      //keybd_event( VkKeyScan(TEXT('A')), 0x1E, KEYEVENTF_KEYUP, 0 );


      keybd_event( VkKeyScan(VK_F1),	0x3B, 0,0 );
      keybd_event( VkKeyScan(VK_F1),	0x3B, KEYEVENTF_KEYUP,0 );
/*
  INPUT key_input[2];
  key_input[0].type = INPUT_KEYBOARD;
  key_input[0].ki.wVk =  VkKeyScan(TEXT('A'));
  key_input[0].ki.wScan = 0x1E;
  key_input[0].ki.dwFlags = NULL;
  key_input[0].ki.time = 0;
  key_input[1].type = INPUT_KEYBOARD;
  key_input[1].ki.wVk =  VkKeyScan(TEXT('A'));
  key_input[1].ki.wScan = 0x1E;
  key_input[1].ki.dwFlags = KEYEVENTF_KEYUP;
  key_input[1].ki.time = 0;
  UINT res = SendInput(2,key_input,sizeof(INPUT));
  if (res <= 0) wxMessageBox(wxT("input failed"));
*/
      wxMessageBox(_("send ica cad"));
      wxWindowList lst_win = cnt->GetChildren();
      for (size_t i = 0; i < lst_win.GetCount(); i ++)
      {

	 wxWindow * win = (lst_win.Item(i))->GetData();
	 HWND hwnd = (HWND)cnt->GetHandle();
	 EnableWindow(hwnd,true);
	 SetActiveWindow(hwnd);
	 SetForegroundWindow(hwnd);
	 ::SetFocus(hwnd);

/*
  keybd_event( VkKeyScan(VK_CONTROL),	0x1D, 0,0 );
  keybd_event( VkKeyScan(VK_MENU),0x38,0,0 );
  keybd_event( VkKeyScan(VK_DELETE),0x53, 0, 0 );
  keybd_event( VkKeyScan(VK_DELETE), 0x53, KEYEVENTF_KEYUP, 0 );
  keybd_event( VkKeyScan(VK_CONTROL), 0x1D, KEYEVENTF_KEYUP, 0 );
  keybd_event( VkKeyScan(VK_MENU), 0x38, KEYEVENTF_KEYUP, 0);
*/
/*
  keybd_event( VkKeyScan(VK_MENU),0x38,0,0 );
  keybd_event( VkKeyScan(VK_MENU), 0x38, KEYEVENTF_KEYUP, 0);
  keybd_event( VkKeyScan(VK_F8),	0x42, 0,0 );
  keybd_event( VkKeyScan(VK_F8),	0x42, KEYEVENTF_KEYUP,0 );
*/
	 keybd_event( VkKeyScan(VK_MENU),0x38,0,0 );
	 keybd_event( VkKeyScan(VK_F8),	0x42, 0,0 );
	 keybd_event( VkKeyScan(VK_F8),	0x42, KEYEVENTF_KEYUP,0 );
	 keybd_event( VkKeyScan(VK_MENU), 0x38, KEYEVENTF_KEYUP, 0);

      }
      //refICA->FocusWindow();
      //keybd_event( VkKeyScan(VK_CONTROL),	0x1D, 0,0 );
      //keybd_event( VkKeyScan(VK_CONTROL),	0x1D, 0,0 );
      //keybd_event( VkKeyScan(VK_F1),	0x3B, 0,0 );
      //keybd_event( VkKeyScan(VK_F1),	0x3B, KEYEVENTF_KEYUP,0 );
      //keybd_event( VkKeyScan(VK_CONTROL), 0x1D, KEYEVENTF_KEYUP, 0 );
      //keybd_event( VkKeyScan(VK_CONTROL), 0x1D, KEYEVENTF_KEYUP, 0 );

      refICA->Release();
   }
   catch(...)
   {
   }
}

void ICAConnection::GrabAll(BOOL state)
{
}

void ICAConnection::CentreConnection()
{
}

bool ICAConnection::DisconnectClose()
{
   Disconnect();
   return true;
}
