///////////////////////////////////////////////////////////////////////////////
// File name:   fastconn_dialog.cpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-21 20:24:38" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#include "fastconn_dialog.hpp"

//#include "main_window.hpp"
#include "rdp_dialogs.hpp"

///////////////////////////////////////////////////////////////////////////////
//! \brief "Construcror for Fast Config Dialog"
//! \param wxWindow* parent - parent element
//! \param int dialogType - dialog type 
//! \param RDPConn * rdpc - pointer to rdp struct (need to remove)
//! \param wxTextFile * frameTextFile (neet to remove)
//! \param base_conn rdp_base_main - programm options
///////////////////////////////////////////////////////////////////////////////
FastConnDialog::FastConnDialog(wxWindow* parent, int dialogType, RDPConn * rdpc, wxTextFile * frameTextFile, base_conn rdp_base_main) :
wxDialog(parent, wxID_ANY, _T("Fast RDP Connection"),
                 wxDefaultPosition, wxDefaultSize/*wxSize(335 + 1.9*SHIFT_W, 120 - 0.6*SHIFT_W)*/,dialogType)
{
	rdp_base = rdp_base_main;
	main_frame = (Main_Frame *)parent;
	
	GETBASEPATH();
	wxString temp = BASEPATH;
	BaseFile = new wxTextFile(temp);
	
	string_server_list = NULL;
	
	LoadFromBase();

	sizer_top = new wxBoxSizer(wxVERTICAL);

	sizer_server = new wxBoxSizer(wxHORIZONTAL);
	sizer_btn = new wxBoxSizer(wxHORIZONTAL);

	m_static_protocol = new wxStaticText(this,-1,wxT("Protocol "));
	wxString arr_proto[] = {wxT("RDP Protocol"),wxT("ICA Protocol")};
	m_combobox_protocol = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,wxSize(120,-1) , 2 , arr_proto,wxCB_READONLY);
	m_combobox_protocol->SetSelection(0);
	m_combobox_protocol->SetValue(arr_proto[0]); 

	m_static_server = new wxStaticText(this,wxID_ANY,wxT("Server name or IP address"),wxDefaultPosition, wxDefaultSize);
	m_combobox_server = new wxComboBox(this,ID_COMBOBOX_FAST,_(""),wxDefaultPosition,wxSize(170,-1),lineCount,string_server_list);
	sizer_server->Add(m_static_server,0,wxALL, 5);
	sizer_server->Add(m_combobox_server,0,wxALL | wxALIGN_CENTER| wxEXPAND , 2);

	sizer_server->Add(m_static_protocol,0,wxALL, 5);
	sizer_server->Add(m_combobox_protocol,0,wxALL | wxALIGN_CENTER| wxEXPAND , 2);

	m_line_buttons = new wxStaticLine(this,wxID_ANY,wxDefaultPosition, wxSize(-1,2) ,wxLI_HORIZONTAL);

	m_button_cancel = new wxButton(this,ID_FASTCONN_CANCEL,wxT("Cancel"),wxDefaultPosition,wxSize(100,-1));
	m_button_connect = new wxButton(this,ID_FASTCONN_CONNECT,wxT("Connect"),wxDefaultPosition,wxSize(100,-1));
	m_button_details = new wxButton(this,ID_FASTCONN_DETAILS,wxT("Details"),wxDefaultPosition,wxSize(100,-1));

	sizer_btn->Add(m_button_connect,0,wxALL,5);
	sizer_btn->Add(m_button_details,0,wxALL,5);
	sizer_btn->Add(m_button_cancel,0,wxALL,5);

	sizer_top->Add(sizer_server,0,wxALL | wxALIGN_LEFT | wxEXPAND,5);
	sizer_top->Add(m_line_buttons,0,wxALL | wxALIGN_CENTER | wxEXPAND,5);
	sizer_top->Add(sizer_btn,0,wxALL | wxALIGN_CENTER ,5);
	
	SetSizer(sizer_top);
	sizer_top->SetSizeHints(this);
	sizer_top->Fit(this); 

	m_button_details->Enable(false);
	
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Destructor
///////////////////////////////////////////////////////////////////////////////
FastConnDialog::~FastConnDialog()
{
	if (m_static_server) {delete m_static_server; m_static_server = NULL;}
	if (m_combobox_server) {delete m_combobox_server; m_combobox_server = NULL;}
	if (m_button_cancel) {delete m_button_cancel; m_button_cancel = NULL;}
	if (m_button_connect) {delete m_button_connect ; m_button_connect = NULL;}
	if (m_button_details) {delete m_button_details ; m_button_details = NULL;}
	wxDELETE(m_line_buttons);

}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process event on Cancel button
///////////////////////////////////////////////////////////////////////////////
void FastConnDialog::button_cancel_func(wxCommandEvent &event)
{
	this->EndModal(0);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Create clean configuration
//! \param wxString server - server name
//! \return Options_HashMap
///////////////////////////////////////////////////////////////////////////////
Options_HashMap FastConnDialog::SetDefaultRDPConn(wxString server)
{
   Options_HashMap local_options;
   
   local_options[wxT("uniq_name")] = wxT("0");
   local_options[wxT("hostname")] = server;
   
//	rdpconn.uniq_name = 0;
//	rdpconn.hostname = server;
   // Protocol type
   if (this->m_combobox_protocol->GetSelection() == 0)
   {
      //rdpconn.conn_type = ConnectionType_RDP;
      local_options[wxT("proto")] = wxT("rdp");
   }
   else if  (this->m_combobox_protocol->GetSelection() == 1)
   {
      //rdpconn.conn_type = ConnectionType_ICA;
      local_options[wxT("proto")] = wxT("ica");
   }
   else
   {
      //rdpconn.conn_type = ConnectionType_Unknown;
      local_options[wxT("proto")] = wxT("unk");
   }

   local_options[wxT("username")] = wxT("Administrator");
   local_options[wxT("password")] = wxT("");
   local_options[wxT("domain")] = wxT("");
   local_options[wxT("group_name")] = wxT("");
   if (local_options[wxT("proto")] == wxT("rdp"))
   {
       local_options[wxT("port")] = wxT("3389");
   }
   else if (local_options[wxT("proto")] == wxT("rdp"))
   {
      local_options[wxT("port")] = wxT("1494");
   }
   else
   {
      local_options[wxT("port")] = wxT("");
   }
   //local_options[wxT("attach_to_console")] = wxT("0");

   // rdpconn.width = 800;
   // rdpconn.heigth = 600;
   local_options[wxT("color_depth")] = wxT("1");

   local_options[wxT("control_size")] = wxT("1") ;
   // rdpconn.bFullScreen = FALSE;
   // rdpconn.bSmartSizing = FALSE;

   // rdpconn.force_update_screen = FALSE;

   // // Share settings
   // rdpconn.bShareDrives = FALSE;
   // rdpconn.bSharePrinters  = FALSE;
   // rdpconn.bShareComPorts = FALSE;
   // rdpconn.bShareSmartCards = FALSE;
   // rdpconn.redirect_devices_nix = wxT("");

   // rdpconn.SoundType = 0;
   // rdpconn.keyboard = 2;

   // // Program settings
   // rdpconn.bUseProgram = FALSE;
   // rdpconn.shell = wxT("");
   // rdpconn.directory = wxT("");
   // rdpconn.bProgramMaximized = TRUE;

   // // Performance settings
   // rdpconn.bEnableBitmapCaching = TRUE;
   // rdpconn.bEnableAnimation = FALSE;
   // rdpconn.bEnableWallpaper = FALSE;
   // rdpconn.bEnableFullWindowDrag = FALSE;
   // rdpconn.bEnableThemes = TRUE;

   // rdpconn.bandwidth = 2;

   // // Advanced settings
   // rdpconn.use_rdp_version = 1;
   // rdpconn.backing_store = FALSE;
   // rdpconn.enable_compres = TRUE;
   // rdpconn.encription_enable_new = TRUE;
   // rdpconn.encription_enable_french = FALSE;
   // rdpconn.numlock_sync = TRUE;
   // rdpconn.private_color_map  = FALSE;
   // rdpconn.send_mouse_event = TRUE;

   // rdpconn.connection_name = server;
   // rdpconn.dwConnectionCount = 0;
   // rdpconn.keyboard_map.Empty();

   // // ICA settings
   // rdpconn.server_ini = wxT("");
   // rdpconn.client_ini = wxT("");
   // rdpconn.ica_file = wxT("");
   // rdpconn.bUseApplication = FALSE;
   // rdpconn.IcaApplication = wxT("");
   // rdpconn.bIcaEncryption = TRUE;
   // rdpconn.ica_encryption = 0;
   // rdpconn.bIcaSound = FALSE;
   // rdpconn.bIcaSoundType = 1;

   // // Proxy settings (ICA Citrix)
   // rdpconn.bProxyType = 0;
   // rdpconn.ProxyAddr = wxT("");
   // rdpconn.ProxyPort = wxT("");
   // rdpconn.ProxyUserId = wxT("");
   // rdpconn.ProxyPassword = wxT("");

   return local_options;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process Connect button
///////////////////////////////////////////////////////////////////////////////
void FastConnDialog::button_connect_func(wxCommandEvent &event)
{
   Options_HashMap local_options;
   wxString str;
   str = m_combobox_server->GetValue();
   bool flag = false;

   if (str.Length() == 0) return;

   for (int i = 0; i < lineCount; i++)
   {
      if (str == string_server_list[i] && m_combobox_server->GetSelection() == i)
      {
	 flag = true;
	 break;
      }
   }

   if (!flag)
   {
      local_options = SetDefaultRDPConn(str);
   }
   else
   {
      int iSelection = m_combobox_server->GetSelection();
      if (iSelection < 0)
      {
	 local_options = SetDefaultRDPConn(str);
      }
      else
      {
	 local_options = ParseString(iSelection,options);
      }
   }
   options = local_options;
   
   this->EndModal(1);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Return Options_HashMap valiable by line number of All
//! connections record
//! \param int iLineNumber - index position 
//! \param Options_HashMap options (nee remove)
///////////////////////////////////////////////////////////////////////////////
Options_HashMap FastConnDialog::ParseString(int iLineNumber, Options_HashMap options)
{
   Options_HashMap local_options;

//   if (BaseFile != NULL)
//   {
//      for (int i = 0; main_frame->all_connection_records.Count(); i++)
//      {
   local_options = main_frame->all_connection_records.Item(iLineNumber);
   //
   
//	 if (local_options[wxT("uniq_name")] = options[wxT("uniq_name")])
//	 {
//	    break;
//	 }
//	 else
//	 {
//	    local_options.clear();
//	 }
      //wxString temp = wxString::Format(wxT("%s"), BaseFile->GetName());
      //Benc bc;
      //rdpconn = bc.Get(&rdp_base,iLineNumber);
//      }
//   }
   return local_options;
}



bool FastConnDialog::LoadFromBase()
{
   Options_HashMap local_options;
   wxString Settings;
   lineCount = main_frame->all_connection_records.Count();
   string_server_list = new wxString[lineCount];
   wxString currstring;
   for (int i = 0; i < main_frame->all_connection_records.Count(); i++)
   {
      local_options = ParseString(i,options);
      //local_options = main_frame->all_connection_records.Item(i);

      currstring.Clear();
      if (local_options[wxT("connection_name")].Length() > 0)
      {
	 currstring.assign(local_options[wxT("connection_name")]);
      }
      else
      {
	 currstring.assign(local_options[wxT("hostname")]);
      }
      string_server_list[i] = currstring;

   }
   return true;
}
///////////////////////////////////////////////////////////////////////////////
//! \brief Processe keys
///////////////////////////////////////////////////////////////////////////////
void FastConnDialog::dialog_hotkeys(wxKeyEvent &event)
{
	wxCommandEvent evt;
	evt.SetId(1); 
	int iKeyCode = event.GetKeyCode();
	switch(iKeyCode)
	{
	case WXK_RETURN:
		this->button_connect_func(evt);


		break;
	case WXK_ESCAPE:
		this->button_cancel_func(evt);
		break;
	default:
		break;

	}

	event.Skip(); 
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process connections list change
///////////////////////////////////////////////////////////////////////////////
void FastConnDialog::combobox_change_func(wxCommandEvent &event)
{
   if (!m_combobox_server) return;

   for (int i = 0; i < lineCount; i++)
   {
      if (m_combobox_server->GetValue() == string_server_list[i] &&
	  m_combobox_server->GetSelection() == i)
      {
	 m_button_details->Enable(true);
	 return;
      }
   }
   m_button_details->Enable(false);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process connections list edit
///////////////////////////////////////////////////////////////////////////////
void FastConnDialog::combobox_text_func(wxCommandEvent &event)
{
   if (!m_combobox_server) return;

   for (int i = 0; i < lineCount; i++)
   {
      if (m_combobox_server->GetValue() == string_server_list[i] && m_combobox_server->GetSelection() == i)
      {
	 m_button_details->Enable(true);
	 return;
      }
   }
   m_button_details->Enable(false);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process button Details
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
void FastConnDialog::button_details_func(wxCommandEvent &event)
{
   Options_HashMap local_options;
   int iSelection = m_combobox_server->GetSelection();
   //rdpconn = ParseString(iSelection,rdpconn);
   //

   local_options = ParseString(iSelection,options);
   //for (int i = 0; i < main_frame->all_connection_records.Count(); i++)
   //{
   //   local_options
   //RDPDialog * prdpdialog = new
   //RDPDialog(main_frame,wxCAPTION,(&rdpconn),NULL,&rdp_base,FALSE/*this->BaseFile*/);
   RDPDialog * prdpdialog = new RDPDialog(main_frame);
   //prdpdialog->LoadRDPConn();
   //
   prdpdialog->Set_Options(&local_options);
   //std::cout << __func__ << std::endl;
   int iRes = prdpdialog->ShowModal();
   delete prdpdialog;

   if (iRes)
   {
      main_frame->base = rdp_base;
      int iSel = m_combobox_server->GetSelection();
      TREEPANEL(main_frame->m_panel_tree)->rdptree->ReloadSettings();
      LoadFromBase();
      if (m_combobox_server) 
      {
	 delete m_combobox_server;
      }
      m_combobox_server =  new wxComboBox(this,ID_COMBOBOX_FAST,_(""),wxPoint(160,10),wxSize(140,20),lineCount,string_server_list);
      m_combobox_server->SetSelection(iSel);
      m_combobox_server->Refresh();
   }
}


int FastConnDialog::getlinecount()
{
	int line_count = 0;
	if (BaseFile != NULL)
	{
		wxString temp = wxString::Format(wxT("%s"), BaseFile->GetName());
		
		Benc bc;
		line_count = bc.Count(&rdp_base);	
	}
	return line_count;
}





