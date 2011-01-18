///////////////////////////////////////////////////////////////////////////////
// File name:   fastconn_dialog.cpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include <wx/ptr_scpd.h>

#include "fastconn_dialog.hpp"

//#include "main_window.hpp"
#include "rdp_dialogs.hpp"

BEGIN_EVENT_TABLE(FastConnDialog, wxDialog)
EVT_BUTTON     (ID_FASTCONN_CONNECT, FastConnDialog::OnButtonConnect)
EVT_BUTTON     (ID_FASTCONN_CANCEL,  FastConnDialog::OnButtonCancel)
EVT_BUTTON     (ID_FASTCONN_DETAILS, FastConnDialog::OnButtonDetails)
EVT_COMBOBOX   (ID_COMBOBOX_FAST,    FastConnDialog::OnComboboxChange)
EVT_TEXT       (ID_COMBOBOX_FAST,    FastConnDialog::OnComboboxText)
EVT_CHAR_HOOK(FastConnDialog::dialog_hotkeys)
END_EVENT_TABLE()

wxDECLARE_SCOPED_PTR(RDPDialog, wxRdpDialogScopedPtr)

///////////////////////////////////////////////////////////////////////////////
//! \brief "Construcror for Fast Config Dialog"
//! \param wxWindow* parent - parent element
//! \param int dialogType - dialog type
//! \param RDPConn * rdpc - pointer to rdp struct (need to remove)
//! \param wxTextFile * frameTextFile (neet to remove)
//! \param base_conn rdpBase_main - programm options
///////////////////////////////////////////////////////////////////////////////
FastConnDialog::FastConnDialog(MainFrame* parent,
                               int dialogType,
                               RDPConn * rdpc,
                               wxTextFile * frameTextFile,
                               base_conn rdpBase_main)
	: wxDialog(parent, wxID_ANY, _("Fast RDP Connection"),
	           wxDefaultPosition,
	           wxDefaultSize/*wxSize(335 + 1.9*SHIFT_W, 120 - 0.6*SHIFT_W)*/,
	           dialogType),
	rdpBase(rdpBase_main),
	btnCancel(new wxButton(this, ID_FASTCONN_CANCEL, _("Cancel"),
	                       wxDefaultPosition, wxSize(100, -1))),
	btnConnect(new wxButton(this, ID_FASTCONN_CONNECT, _("Connect"),
	                        wxDefaultPosition, wxSize(100, -1))),
	btnDetails(new wxButton(this, ID_FASTCONN_DETAILS, _("Details"),
	                        wxDefaultPosition, wxSize(100, -1))),
	staticTxtServer(new wxStaticText(this, wxID_ANY,
	                                 _("Server name or IP address"),
	                                 wxDefaultPosition,
	                                 wxDefaultSize)),
	comboboxServer(new wxComboBox(this,
	                              ID_COMBOBOX_FAST,
	                              wxT(""),
	                              wxDefaultPosition,
	                              wxSize(170, -1),
				      NULL,
				      wxCB_READONLY)), // TODO: magic numbers
  mainFrame(parent)
{

	GETBASEPATH();
	const wxString temp = BASEPATH; // WTF?? what does 'temp' mean?
	BaseFile = new wxTextFile(temp);

	LoadFromBase();

	sizer_top    = new wxBoxSizer(wxVERTICAL);
	sizer_server = new wxBoxSizer(wxHORIZONTAL);
	sizer_btn    = new wxBoxSizer(wxHORIZONTAL);

	for (size_t i = 0; i < servers.Count(); ++i)
		comboboxServer->Append(servers[i]);

	m_static_protocol = new wxStaticText(this, wxID_ANY, _("Protocol "));
	const wxString arrProto[] = { _("RDP Protocol"),
	                              _("ICA Protocol"),
	                              _("VNC Protocol")
#ifdef __WXGTK__
				      ,_("RDP Protocol (freerdp)")
#endif
				      };
	m_combobox_protocol = new wxComboBox(this,
	                                     ID_COMBOBOX_PROTOCOL,
	                                     wxT(""),
	                                     wxDefaultPosition,
	                                     wxSize(160, -1),
	                                     WXSIZEOF(arrProto),
	                                     arrProto,
	                                     wxCB_READONLY);

	m_combobox_protocol->SetSelection(0);

	sizer_server->Add(staticTxtServer.get(), 0, wxALL,                             5);
	sizer_server->Add(comboboxServer.get(),  0, wxALL | wxALIGN_CENTER | wxEXPAND, 2);

	sizer_server->Add(m_static_protocol,   0, wxALL,                             5);
	sizer_server->Add(m_combobox_protocol, 0, wxALL | wxALIGN_CENTER | wxEXPAND, 2);

	lineButtons = new wxStaticLine(this,
	                               wxID_ANY,
	                               wxDefaultPosition,
	                               wxSize(-1, 2),
	                               wxLI_HORIZONTAL);

	sizer_btn->Add(btnConnect.get(), 0, wxALL, 5);
	sizer_btn->Add(btnDetails.get(), 0, wxALL, 5);
	sizer_btn->Add(btnCancel.get(),  0, wxALL, 5);

	sizer_top->Add(sizer_server, 0, wxALL | wxALIGN_LEFT   | wxEXPAND, 5);
	sizer_top->Add(lineButtons,  0, wxALL | wxALIGN_CENTER | wxEXPAND, 5);
	sizer_top->Add(sizer_btn,    0, wxALL | wxALIGN_CENTER,            5);

	SetSizer(sizer_top);
	sizer_top->SetSizeHints(this);
	sizer_top->Fit(this);

	btnDetails->Enable(false);
	btnConnect->Enable(false);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Destructor
///////////////////////////////////////////////////////////////////////////////
FastConnDialog::~FastConnDialog() {
	wxDELETE(lineButtons);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process event on Cancel button
///////////////////////////////////////////////////////////////////////////////
void FastConnDialog::OnButtonCancel(wxCommandEvent &event) {
	EndModal(FASTCONN_RET_CANCEL);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Create clean configuration
//! \param wxString server - server name
//! \return Options_HashMap
///////////////////////////////////////////////////////////////////////////////
Options_HashMap FastConnDialog::SetDefaultRDPConn(const wxString &server)
{
	Options_HashMap local_options;

	local_options[wxT("uniq_name")] = wxT("0");
	local_options[wxT("hostname")]  = server;

//	rdpconn.uniq_name = 0;
//	rdpconn.hostname = server;
	// TODO: rewrite in OOP-style!
/*	switch(m_combobox_protocol->GetSelection()) {
	case 0: local_options[wxT("proto")] = wxT("rdp");
	        local_options[wxT("port")] = wxT("3389");
	        break;
	case 1: local_options[wxT("proto")] = wxT("ica");
	        local_options[wxT("port")] = wxT("1494");
	        break;
	case 2: local_options[wxT("proto")] = wxT("rfb");
	        local_options[wxT("port")] = wxT("5900");
	        break;
	default: local_options[wxT("proto")] = wxT("unk");
	         local_options[wxT("port")] = wxT("");
	}*/

	local_options[wxT("username")]   = wxT("Administrator");
	local_options[wxT("password")]   = wxT("");
	local_options[wxT("domain")]     = wxT("");
	local_options[wxT("group_name")] = wxT("");

	//local_options[wxT("attach_to_console")] = wxT("0");

	// rdpconn.width = 800;
	// rdpconn.heigth = 600;
	local_options[wxT("color_depth")]  = wxT("1");
	local_options[wxT("control_size")] = wxT("1") ;
	// rdpconn.bFullScreen = FALSE;
	// rdpconn.bSmartSizing = FALSE;

	// rdpconn.force_update_screen = FALSE;

	// Share settings
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
void FastConnDialog::OnButtonConnect(wxCommandEvent &event) {
	const wxString server(comboboxServer->GetValue());
	const int selection = comboboxServer->GetSelection();
	const int idxInServerLst =
		wxNOT_FOUND != selection ? selection : servers.Index(server, false);

	Options_HashMap localOptions;
	if (wxNOT_FOUND == idxInServerLst)
		localOptions = SetDefaultRDPConn(server);
	else
		localOptions = mainFrame->all_connection_records.Item(idxInServerLst);

	// TODO: rewrite in OOP-style!
	switch(m_combobox_protocol->GetSelection()) {
	case 0: localOptions[wxT("proto")] = wxT("rdp");
	        localOptions[wxT("port")] = wxT("3389");
	        break;
	case 1: localOptions[wxT("proto")] = wxT("ica");
	        localOptions[wxT("port")] = wxT("1494");
	        break;
	case 2: localOptions[wxT("proto")] = wxT("rfb");
	        localOptions[wxT("port")] = wxT("5900");
	        break;
	default: localOptions[wxT("proto")] = wxT("unk");
	         localOptions[wxT("port")] = wxT("");
	}
	options = localOptions;

	EndModal(FASTCONN_RET_CONNECT);
}

void FastConnDialog::LoadFromBase() {
	Options_HashMap localOptions;
	wxString Settings;
	const int availConnCount = mainFrame->all_connection_records.Count();
	servers.Clear();
	servers.Alloc(availConnCount);
	for (int i = 0; i < availConnCount; ++i) {
		localOptions = mainFrame->all_connection_records.Item(i);
		if (localOptions[wxT("connection_name")].Len() > 0)
			servers.Add(localOptions[wxT("connection_name")]);
		else
			servers.Add(localOptions[wxT("hostname")]);
	}
	servers.Shrink();
}
///////////////////////////////////////////////////////////////////////////////
//! \brief Processe keys
///////////////////////////////////////////////////////////////////////////////
void FastConnDialog::dialog_hotkeys(wxKeyEvent &event) {
	wxCommandEvent evt;
	evt.SetId(1); // WTF??
	int iKeyCode = event.GetKeyCode();
	switch(iKeyCode) {
	case WXK_RETURN: OnButtonConnect(evt);
	                 break;
	case WXK_ESCAPE: OnButtonCancel(evt);
	                 break;
	default:         break;
	}

	event.Skip();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process connections list change
///////////////////////////////////////////////////////////////////////////////
void FastConnDialog::OnComboboxChange(wxCommandEvent &event) {

  //Set proto from chose object
  Options_HashMap
    localOptions(mainFrame->all_connection_records.Item(comboboxServer->GetSelection()));
  if (localOptions[wxT("proto")] == wxT("rdp")) {
    m_combobox_protocol->SetSelection(0);
  }
  else if (localOptions[wxT("proto")] == wxT("ica")) {
    m_combobox_protocol->SetSelection(1);
  }
  else if (localOptions[wxT("proto")] == wxT("rfb")) {
    m_combobox_protocol->SetSelection(2);
  }

  else if (localOptions[wxT("proto")] == wxT("frdp")) {
    m_combobox_protocol->SetSelection(3);
  }
  else {
    m_combobox_protocol->SetSelection(0);
  }

  for (size_t i = 0; i < servers.Count(); ++i) {
    if (comboboxServer->GetValue()     == servers[i] &&
	(size_t)comboboxServer->GetSelection() == i) {
      btnDetails->Enable(true);
      btnConnect->Enable(true);
      return;
    }
  }
  if (comboboxServer->GetValue().IsEmpty())
    { btnConnect->Enable(false); }
  else { btnConnect->Enable(true); }
  btnDetails->Enable(false);
  //wxMessageBox();

}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process connections list edit
///////////////////////////////////////////////////////////////////////////////
void FastConnDialog::OnComboboxText(wxCommandEvent &event) {
	for (size_t i = 0; i < servers.Count(); i++) {
		if (comboboxServer->GetValue() == servers[i] &&
		    (size_t)comboboxServer->GetSelection() == i) {
			btnDetails->Enable(true);
			btnConnect->Enable(true);
			return;
		}
	}
	if (comboboxServer->GetValue().IsEmpty())
	  { btnConnect->Enable(false); }
	else { btnConnect->Enable(true); }
	btnDetails->Enable(false);

}

///////////////////////////////////////////////////////////////////////////////
//! \brief Process button Details
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void FastConnDialog::OnButtonDetails(wxCommandEvent &event) {
	int iSelection = comboboxServer->GetSelection();
	const Options_HashMap
		localOptions(mainFrame->all_connection_records.Item(iSelection));
	//for (int i = 0; i < mainFrame->all_connection_records.Count(); i++)

	std::auto_ptr<RDPDialog> rdpDialog(new RDPDialog(mainFrame));
	//prdpdialog->LoadRDPConn();
	rdpDialog->SetOptions(&localOptions);

	const int iRes = rdpDialog->ShowModal();
	if (RDPDLG_RET_CANCEL != iRes) {
		mainFrame->base = rdpBase;
		const int iSel = comboboxServer->GetSelection();
		TREEPANEL(mainFrame->m_panel_tree)->rdptree->ReloadSettings();
		LoadFromBase();
		comboboxServer->Clear();
		for (size_t i = 0; i < servers.Count(); ++i)
			comboboxServer->Append(servers[i]);

		comboboxServer->SetSelection(iSel);
		comboboxServer->Refresh();
	}
}

/*
int FastConnDialog::GetavailConnCount() const {
	int availConnCount = 0;
	if (NULL != BaseFile) {
		wxString temp(wxString::Format(wxT("%s"), BaseFile->GetName()));

		Benc bc;
		availConnCount = bc.Count(&rdpBase);
	}
	return line_count;
}
*/

