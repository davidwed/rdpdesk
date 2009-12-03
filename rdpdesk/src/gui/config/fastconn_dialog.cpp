#include "fastconn_dialog.hpp"

#include "main_window.hpp"


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

	m_static_server = new wxStaticText(this,wxID_ANY,wxT("Server name or IP address"),wxDefaultPosition, wxDefaultSize);
	m_combobox_server = new wxComboBox(this,ID_COMBOBOX_FAST,_(""),wxDefaultPosition,wxSize(170,-1),lineCount,string_server_list);
	sizer_server->Add(m_static_server,0,wxALL, 5);
	sizer_server->Add(m_combobox_server,0,wxALL | wxALIGN_CENTER| wxEXPAND , 2);


	m_button_cancel = new wxButton(this,ID_FASTCONN_CANCEL,wxT("Cancel"),wxDefaultPosition,wxSize(100,-1));
	m_button_connect = new wxButton(this,ID_FASTCONN_CONNECT,wxT("Connect"),wxDefaultPosition,wxSize(100,-1));
	m_button_details = new wxButton(this,ID_FASTCONN_DETAILS,wxT("Details"),wxDefaultPosition,wxSize(100,-1));

	sizer_btn->Add(m_button_connect,0,wxALL,5);
	sizer_btn->Add(m_button_details,0,wxALL,5);
	sizer_btn->Add(m_button_cancel,0,wxALL,5);

	sizer_top->Add(sizer_server,0,wxALL | wxALIGN_LEFT | wxEXPAND,5);
	sizer_top->Add(sizer_btn,0,wxALL | wxALIGN_CENTER ,5);
	
	SetSizer(sizer_top);
	sizer_top->SetSizeHints(this);
	sizer_top->Fit(this); 

	m_button_details->Enable(false);
	
}

FastConnDialog::~FastConnDialog()
{
	if (m_static_server) {delete m_static_server; m_static_server = NULL;}
	if (m_combobox_server) {delete m_combobox_server; m_combobox_server = NULL;}
	if (m_button_cancel) {delete m_button_cancel; m_button_cancel = NULL;}
	if (m_button_connect) {delete m_button_connect ; m_button_connect = NULL;}
	if (m_button_details) {delete m_button_details ; m_button_details = NULL;}

}

void FastConnDialog::button_cancel_func(wxCommandEvent &event)
{
	this->EndModal(0);
}

RDPConn FastConnDialog::SetDefaultRDPConn(wxString server)
{
	rdpconn.uniq_name = 0;
	rdpconn.hostname = server;

	rdpconn.username = wxT("Administrator");
	rdpconn.password = wxT("");
	rdpconn.domain = wxT("");
	rdpconn.group_name = wxT("");
			
	rdpconn.port = wxString(_("3389"));
	rdpconn.attach_to_console = FALSE;

	rdpconn.width = 800;
	rdpconn.heigth = 600;
	rdpconn.color_depth = 16;

	rdpconn.bControlSize = TRUE;
	rdpconn.bFullScreen = FALSE;
	rdpconn.bSmartSizing = FALSE;

	rdpconn.force_update_screen = FALSE;

	// Share settings
	rdpconn.bShareDrives = FALSE;
	rdpconn.bSharePrinters  = FALSE;
	rdpconn.bShareComPorts = FALSE;
	rdpconn.bShareSmartCards = FALSE;
	rdpconn.redirect_devices_nix = wxT("");

	rdpconn.SoundType = 0;
	rdpconn.keyboard = 2;

	// Program settings
	rdpconn.bUseProgram = FALSE;
	rdpconn.shell = wxT("");
	rdpconn.directory = wxT("");
	rdpconn.bProgramMaximized = TRUE;

	// Performance settings
	rdpconn.bEnableBitmapCaching = TRUE;
	rdpconn.bEnableAnimation = FALSE;
	rdpconn.bEnableWallpaper = FALSE;
	rdpconn.bEnableFullWindowDrag = FALSE;
	rdpconn.bEnableThemes = TRUE;

	rdpconn.bandwidth = 2;

	// Advanced settings
	rdpconn.use_rdp_version = 1;
	rdpconn.backing_store = FALSE;
	rdpconn.enable_compres = TRUE;
	rdpconn.encription_enable_new = TRUE;
	rdpconn.encription_enable_french = FALSE;
	rdpconn.numlock_sync = TRUE;
	rdpconn.private_color_map  = FALSE;
	rdpconn.send_mouse_event = TRUE;

	rdpconn.connection_name = server;
	rdpconn.dwConnectionCount = 0;
	
	rdpconn.keyboard_map.Empty();
	return rdpconn;
}

void FastConnDialog::button_connect_func(wxCommandEvent &event)
{
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
		rdpconn = SetDefaultRDPConn(str);
	}
	else
	{
		int iSelection = m_combobox_server->GetSelection();
		if (iSelection < 0)
		{
			rdpconn = SetDefaultRDPConn(str);
		}
		else
		{
			RDPConn rdpconn2 = ParseString(iSelection,rdpconn);
			this->rdpconn = rdpconn2;
		}
	}
	this->EndModal(1);
}


RDPConn FastConnDialog::ParseString(int iLineNumber, RDPConn rdpconn)
{
	if (BaseFile != NULL)
	{
		wxString temp = wxString::Format(wxT("%s"), BaseFile->GetName());

		Benc bc;
		rdpconn = bc.Get(&rdp_base,iLineNumber);

	}
	return rdpconn;
}



bool FastConnDialog::LoadFromBase()
{
	wxString Settings;
	lineCount = getlinecount();
	string_server_list = new wxString[lineCount];
	wxString currstring;
	for (int i = 0; i < lineCount; i++)
	{
		rdpconn = ParseString(i,rdpconn);

		currstring.Clear();
		if (rdpconn.connection_name.Length() > 0)
		{
			currstring.assign(rdpconn.connection_name);
		}
		else
		{
			currstring.assign(rdpconn.hostname);
		}
		
		string_server_list[i] = currstring;

	}
	return true;
}

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

void FastConnDialog::combobox_change_func(wxCommandEvent &event)
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

void FastConnDialog::button_details_func(wxCommandEvent &event)
{
	int iSelection = m_combobox_server->GetSelection();
	rdpconn = ParseString(iSelection,rdpconn);

	RDPDialog * prdpdialog = new RDPDialog(main_frame,wxCAPTION,(&rdpconn),NULL,&rdp_base,FALSE/*this->BaseFile*/);
	prdpdialog->LoadRDPConn();
	int iRes = prdpdialog->ShowModal();
	delete prdpdialog;
	
	if (iRes)
	{
		main_frame->base = rdp_base;
		int iSel = m_combobox_server->GetSelection();
		main_frame->m_panel_tree->rdptree->ReloadSettings();
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





