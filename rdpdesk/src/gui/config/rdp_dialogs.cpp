#include "rdp_dialogs.hpp"

#include "main_window.hpp"

RDPDialog::RDPDialog(wxWindow* parent, int dialogType, RDPConn * prdpc, wxTextFile * frameBaseFile,base_conn * rdp_base_main, BOOL not_change) : 
wxDialog(parent, wxID_ANY, _T("Connection settings"),
                 wxDefaultPosition, wxSize(DIALOG_NEWCONN_WIDTH, 322),dialogType)
{
	rdp_base = rdp_base_main;

	main_frame = (Main_Frame *)parent;

	new_uniq_name = not_change;
	
	notebook = NULL;
	m_rdpgeneral = NULL;
	m_rdpdisplay = NULL;
	m_rdpshare = NULL;
	m_rdpprogram = NULL;
	m_rdpperformance = NULL;

	m_rdpadvanced = NULL;
	m_button_ok = NULL;
	m_button_cancel = NULL;
	m_button_save = NULL;

	sizer_main = NULL;
	sizer_btn = NULL;


	if (prdpc == NULL)
	{
		wxMessageBox(_("Error data"),_("Error"),wxICON_ERROR);
		this->EndModal(0); 
	}
	
	prdpconn = prdpc;
	BaseFile = frameBaseFile;

	sizer_main = new wxBoxSizer(wxVERTICAL);
	notebook = new wxNotebook(this, this->GetId(), wxDefaultPosition, wxDefaultSize, wxNB_TOP);

	sizer_main->Add(notebook,0,wxALIGN_CENTER | wxALL /*| wxEXPAND*/,5 );

 	m_rdpgeneral = new RDPGeneral(notebook,wxDefaultPosition,wxDefaultSize,rdp_base);
  	notebook->AddPage(m_rdpgeneral,wxT("General"), true);

  	m_rdpdisplay = new RDPDisplay(notebook,wxDefaultPosition,wxDefaultSize);
	notebook->AddPage(m_rdpdisplay,wxT("Display"), true);

   	m_rdpshare = new RDPShare(notebook,wxDefaultPosition,wxDefaultSize);
  	notebook->AddPage(m_rdpshare,wxT("Share"), true);

  	m_rdpprogram = new RDPProgram(notebook,wxDefaultPosition,wxDefaultSize);
  	notebook->AddPage(m_rdpprogram,wxT("Program"), true);

  	m_rdpperformance = new RDPPerformance(notebook,wxDefaultPosition,wxDefaultSize);
  	notebook->AddPage(m_rdpperformance,wxT("Performance"), true);

  	m_rdpadvanced = new RDPAdvanced(notebook,wxDefaultPosition,wxDefaultSize);
  	notebook->AddPage(m_rdpadvanced,wxT("Advanced"), true);
#ifdef __WXMSW__
 	m_rdpadvanced->Enable(false);
#endif
  	notebook->SetSelection(0);

	sizer_btn = new wxBoxSizer(wxHORIZONTAL);



	if (new_uniq_name)
	{
		m_button_ok = new wxButton(this,ID_BUTTON_OK, wxT("Connect"), wxDefaultPosition, wxSize(100,-1) );
		sizer_btn->Add(m_button_ok,0,wxALL,5);
	}
	m_button_cancel = new wxButton(this,ID_BUTTON_CANCEL, wxT("Cancel"), wxDefaultPosition, wxSize(100,-1) );
	m_button_save = new wxButton(this,ID_BUTTON_SAVE, wxT("Save"), wxDefaultPosition, wxSize(100,-1));
	sizer_btn->Add(m_button_cancel,0,wxALL,5);
	sizer_btn->Add(m_button_save,0,wxALL,5);

	sizer_main->Add(sizer_btn,0,wxALIGN_CENTER | wxALL /*| wxEXPAND*/,5 );
	SetSizer(sizer_main);     
	sizer_main->SetSizeHints(this);
}


RDPDialog::~RDPDialog()
{
	size_t end;

	if(notebook != NULL)
	{
		end = notebook->GetPageCount();
		notebook->DeleteAllPages();
		delete notebook;
		notebook = NULL;
	}

	if(m_button_ok != NULL) {delete m_button_ok; m_button_ok = NULL;}
	if(m_button_cancel != NULL) {delete m_button_cancel; m_button_cancel = NULL;}
	if(m_button_save != NULL) {delete m_button_save; m_button_save = NULL;}

	
	
}



RDPGeneral::RDPGeneral(wxNotebook *frame, const wxPoint& point, const wxSize& size, base_conn * rdp_base_main): wxPanel(frame, wxID_ANY, point, size ) 
{
	rdp_base = rdp_base_main;

	m_static_server = NULL;
	m_text_server = NULL;

	m_static_port = NULL;
	m_text_port = NULL;

	m_static_username = NULL;
	m_text_username = NULL;

	m_static_password = NULL;
	m_text_password = NULL;

	m_static_domain = NULL;
	m_text_domain = NULL;

	m_checkbox_autologon = NULL;
	m_checkbox_customport = NULL;
	m_checkbox_attachtoconsole = NULL;

	m_static_groupname = NULL;
	m_combobox_groupname = NULL;

	m_static_connectionname = NULL;
	m_text_connectionname = NULL;

	groupcount = 0;
	get_group_list();



	wxFlexGridSizer * sizer_top = new wxFlexGridSizer(4,10,10);
	sizer_top->AddGrowableCol(2,0);
	sizer_top->AddGrowableCol(3,0);

	m_static_connectionname = new wxStaticText(this, wxID_ANY, wxT("Connection name"),wxDefaultPosition,wxDefaultSize);
	m_text_connectionname = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,-1), wxTE_PROCESS_ENTER);
	m_static_groupname = new wxStaticText(this, wxID_ANY, wxT("Group name"),wxDefaultPosition,wxDefaultSize);
	m_combobox_groupname = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,wxSize(200,-1) ,array_string_group_list);
	
	sizer_top->Add(m_static_connectionname,0,wxALL | wxALIGN_LEFT  ,0);
	sizer_top->Add(20,20,0,wxALL | wxALIGN_CENTER,0);
	sizer_top->Add(m_text_connectionname,0,wxALL  |wxALIGN_RIGHT | wxEXPAND,0);
	sizer_top->Add(10,20,0,wxALL | wxALIGN_CENTER,0);
	sizer_top->Add(m_static_groupname,0,wxALL | wxALIGN_LEFT  ,0);
	sizer_top->Add(20,20,0,wxALL | wxALIGN_CENTER,0);
	sizer_top->Add(m_combobox_groupname,0,wxALL |wxALIGN_RIGHT | wxEXPAND ,0);
	sizer_top->Add(10,20,0,wxALL | wxALIGN_CENTER,0);

	wxBoxSizer * sizer_main = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer * sizer_bottom = new wxFlexGridSizer(3,10,10);
	sizer_bottom->AddGrowableCol(0,1); 


	m_static_server = new wxStaticText(this, wxID_ANY, wxT("Server"),wxDefaultPosition,wxDefaultSize);
	m_text_server = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(130,-1), wxTE_PROCESS_ENTER);
	m_checkbox_customport = new wxCheckBox( this, ID_CHECKBOX_CUSTOMPORT, wxT("&Use custom port"), wxDefaultPosition,wxDefaultSize );

	m_static_username = new wxStaticText(this, wxID_ANY, wxT("Username"),wxDefaultPosition,wxDefaultSize);
	m_text_username = new wxTextCtrl( this, wxID_ANY, wxT("Administrator"), wxDefaultPosition, wxSize(130,-1), wxTE_PROCESS_ENTER);

	wxBoxSizer * sizer_port = new wxBoxSizer(wxHORIZONTAL);
	m_static_port = new wxStaticText(this, wxID_ANY, _("Port"),wxDefaultPosition,wxDefaultSize);
	m_text_port = new wxTextCtrl( this, wxID_ANY, wxT("3389"), wxDefaultPosition, wxSize(50,-1), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC) );
	sizer_port->Add(m_static_port,1,wxALL | wxEXPAND ,2);
	sizer_port->Add(m_text_port,1,wxALL | wxEXPAND ,2);


	m_static_password = new wxStaticText(this, wxID_ANY, wxT("Password"),wxDefaultPosition,wxDefaultSize);
	m_text_password = new wxTextCtrl( this, ID_TEXTCTRL_PASSWORD, wxT(""),wxDefaultPosition, wxSize(130,-1), wxTE_PASSWORD);
	m_checkbox_autologon = new wxCheckBox( this, ID_CHECKBOX_AUTOLOGON, wxT("&Auto logon"), wxDefaultPosition,wxDefaultSize );

	m_static_domain = new wxStaticText(this, wxID_ANY, wxT("Domain"),wxDefaultPosition,wxDefaultSize);
	m_text_domain = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(130,-1), wxTE_PROCESS_ENTER);
	m_checkbox_attachtoconsole = new wxCheckBox( this, wxID_ANY, wxT("&Attach to console"), wxDefaultPosition,wxDefaultSize);

	
	sizer_bottom->Add(m_static_server,0,wxALIGN_LEFT | wxALL,0);
	sizer_bottom->Add(m_text_server,0,wxALL,0);
	sizer_bottom->Add(m_checkbox_customport,0,wxALIGN_LEFT |wxALL,3);
	sizer_bottom->Add(m_static_username,0,wxALL,0);
	sizer_bottom->Add(m_text_username,0,wxALL,0);
	sizer_bottom->Add(sizer_port,0,wxALL,0);
	sizer_bottom->Add(m_static_password,0,wxALL,0);
	sizer_bottom->Add(m_text_password,0,wxALL,0);
	sizer_bottom->Add(m_checkbox_autologon,0,wxALL,3);
	sizer_bottom->Add(m_static_domain,0,wxALL,0);
	sizer_bottom->Add(m_text_domain,0,wxALL,0);
	sizer_bottom->Add(m_checkbox_attachtoconsole,0,wxALL,3);

	sizer_main->Add(sizer_top,0,wxALL | wxEXPAND /*wxSHAPED*/,5);
	sizer_main->Add(sizer_bottom,0,wxALL | wxSHAPED,5);	

	SetSizer(sizer_main);   
	sizer_main->SetSizeHints(this);
	FitInside();
	sizer_main->Fit(this); 

	m_text_port->Enable(false);
	m_static_port->Enable(false);
	m_combobox_groupname->SetValue(wxT("Main"));
	m_combobox_groupname->SetSelection(0);
	

}

RDPGeneral::~RDPGeneral()
{
	if(m_static_server != NULL) {delete m_static_server; m_static_server = NULL;}
	if(m_text_server != NULL) {delete m_text_server; m_text_server = NULL;}
	if(m_static_port != NULL) {delete m_static_port; m_static_port = NULL;}
	if(m_text_port != NULL) {delete m_text_port; m_text_port = NULL;}
	if(m_static_username != NULL) {delete m_static_username; m_static_username = NULL;}
	if(m_text_username != NULL) {delete m_text_username; m_text_username = NULL;}

	if(m_static_password != NULL) {delete m_static_password ; m_static_password = NULL;}
	if(m_text_password != NULL) {delete m_text_password; m_text_password = NULL;}
	if(m_static_domain != NULL) {delete m_static_domain; m_static_domain = NULL;}
	if(m_text_domain != NULL) {delete m_text_domain; m_text_domain = NULL;}
	if(m_checkbox_autologon != NULL) {delete m_checkbox_autologon; m_checkbox_autologon = NULL;}
	if(m_checkbox_customport != NULL) {delete m_checkbox_customport; m_checkbox_customport = NULL; }
	if(m_checkbox_attachtoconsole != NULL)
	{delete m_checkbox_attachtoconsole; m_checkbox_attachtoconsole = NULL;}


	if (m_static_groupname) {delete m_static_groupname; m_static_groupname = NULL;}
	if (m_combobox_groupname) {delete m_combobox_groupname; m_combobox_groupname = NULL;}

	if (m_static_connectionname) {delete m_static_connectionname; m_static_connectionname = NULL;}
	if (m_text_connectionname) {delete m_text_connectionname; m_text_connectionname = NULL;}
	
}

void RDPGeneral::get_group_list()
{
	Benc bc;

	RDPConn lrdpconn;
	int lc = bc.Count(rdp_base);
	array_string_group_list.Alloc(lc);
	
	wxString currgroupname;

	for (int i = 0; i < lc; i++)
	{
		lrdpconn = bc.Get(rdp_base,i);

		if (lrdpconn.group_name)
		{
			if (lrdpconn.group_name.Length() == 0)
			{
				lrdpconn.group_name.assign(wxT("Main")); 
			}
			int arr_size = (int)array_string_group_list.Count();
			bool flag = true;
			for (int i = 0; i < arr_size; i++)
			{
				if (lrdpconn.group_name == array_string_group_list.Item(i)) 
				{
					flag = false;				
					break;
				}
			}

			if (flag)
			{
				array_string_group_list.Add(lrdpconn.group_name); 
			}
			
		}

	}
	array_string_group_list.Sort();
	array_string_group_list.Shrink();
}

void RDPGeneral::checkbox_customport_func(wxCommandEvent &event)
{
		if (!this->m_checkbox_customport->IsChecked())
		{
			m_text_port->Enable(false);
			m_static_port->Enable(false);
		}
		else
		{
			m_text_port->Enable(true);
			m_static_port->Enable(true);
		}
	
}

void RDPGeneral::text_passwordentry_func(wxCommandEvent &event)
{
	wxString strTemp;
	switch(event.GetId())
	{
	case ID_TEXTCTRL_PASSWORD:
		strTemp = this->m_text_password->GetValue();
		if (strTemp.Length() == 0)
		{
			m_checkbox_autologon->SetValue(false);
		} 
		else 
		{
			m_checkbox_autologon->SetValue(true);
		}
		break;

	case ID_CHECKBOX_AUTOLOGON:
		if (!m_checkbox_autologon->IsChecked()) m_text_password->SetValue(_T(""));
		break;

	default:
		break;
	}
}

RDPDisplay::RDPDisplay(wxNotebook *frame, const wxPoint& point, const wxSize& size) :  wxPanel(frame, wxID_ANY, point , size ) 
{
	m_static_width = NULL;
	m_text_width = NULL;

	m_static_height = NULL;
	m_text_height = NULL;

	m_static_colordepth = NULL;
	m_choise_colordepth = NULL;

	m_checkbox_fullscreen = NULL;
	m_checkbox_smartsizing = NULL;
	m_checkbox_controlsize = NULL;

	m_checkbox_updatescreen = NULL;

	m_static_resolution = NULL;
	m_combobox_resolution = NULL;
	m_checkbox_customgeometry = NULL;
	
	wxArrayString resolutions = GetResolutionsList();

	sizer_top = new wxBoxSizer(wxVERTICAL);
	sizer_main = new wxFlexGridSizer(3,10,10);

	m_static_resolution = new wxStaticText(this, wxID_ANY, wxT("Screen resolution"),wxDefaultPosition, wxDefaultSize);
	m_combobox_resolution = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,wxSize(120,-1),resolutions,wxCB_READONLY);
	int sel = GetDefaultResolution();
	if (sel >=0)
	{
		m_combobox_resolution->SetValue(resolutions.Item(sel));
		m_combobox_resolution->SetSelection(sel);
	}

	m_checkbox_customgeometry = new wxCheckBox( this, ID_CHK_CUSTOMGEOMETRY, _T("&Custom geometry"), wxDefaultPosition, wxDefaultSize );
	m_static_width = new wxStaticText(this, wxID_ANY, wxT("Screen width"),wxDefaultPosition, wxDefaultSize);
	m_text_width = new wxTextCtrl( this, wxID_ANY, wxT("800"), wxDefaultPosition, wxSize(120,-1), wxTE_PROCESS_ENTER , wxTextValidator(wxFILTER_NUMERIC) );
	m_static_height = new wxStaticText(this, wxID_ANY, wxT("Screen height"),wxDefaultPosition, wxDefaultSize);
	m_text_height = new wxTextCtrl( this, wxID_ANY, wxT("600"), wxDefaultPosition, wxSize(120,-1), wxTE_PROCESS_ENTER , wxTextValidator(wxFILTER_NUMERIC) );

	m_static_colordepth = new wxStaticText(this, wxID_ANY, wxT("Color depth"),wxDefaultPosition, wxDefaultSize);
	wxString Choises[] = {_("8-bit"),_("15-bit"),_("16-bit"),_("24-bit")};
	m_choise_colordepth = new wxComboBox(this,wxID_ANY,_(""),wxDefaultPosition,wxSize(120,-1),4,Choises,wxCB_READONLY);
	m_choise_colordepth->SetValue(Choises[2]);
	m_choise_colordepth->SetSelection(2);

	m_checkbox_updatescreen = new wxCheckBox( this, wxID_ANY, wxT("&Force update screen"), wxDefaultPosition, wxDefaultSize );
	m_checkbox_fullscreen = new wxCheckBox( this, ID_CHK_FULLSCREEN, wxT("&Full screen"), wxDefaultPosition, wxDefaultSize );
	m_checkbox_smartsizing = new wxCheckBox( this, -1, wxT("&Smart sizing"), wxDefaultPosition, wxDefaultSize );
	m_checkbox_controlsize = new wxCheckBox( this, ID_CHK_CONTROLSIZE, wxT("&Control size"),wxDefaultPosition, wxDefaultSize);

	m_checkbox_controlsize->SetValue(true);
	


	sizer_main->AddSpacer(1);
	sizer_main->AddSpacer(1);
	sizer_main->AddSpacer(1);
	s1 = new wxBoxSizer(wxHORIZONTAL);

	s1->Add(m_static_resolution,0,wxALL |  wxALIGN_LEFT  /*wxEXPAND*/,0);

	s1->Add(m_static_width,0,wxALL | wxALIGN_LEFT  /*wxEXPAND*/,0);

	sizer_main->Add(s1,0,wxALIGN_LEFT | wxALL,0);
	s2 = new wxBoxSizer(wxHORIZONTAL);
	s2->Add(m_combobox_resolution,0,wxALL | wxALIGN_CENTER | wxEXPAND,0);
	s2->Add(m_text_width,0,wxALL | wxALIGN_CENTER | wxEXPAND,0);
	sizer_main->Add(s2,0,wxALIGN_CENTER | wxALL,0);

	sizer_main->Add(m_checkbox_customgeometry,0,wxALIGN_LEFT | wxALL,3);
	sizer_main->Add(m_static_height,0,wxALIGN_LEFT | wxALL,0);
	sizer_main->Add(m_text_height,0,wxALIGN_CENTER | wxALL,0);
	sizer_main->Add(m_checkbox_fullscreen,0,wxALIGN_LEFT | wxALL,3);
	sizer_main->Add(m_static_colordepth,0,wxALIGN_LEFT | wxALL,0);
	sizer_main->Add(m_choise_colordepth,0,wxALIGN_CENTER | wxALL,0);
#ifdef __WXMSW__
	sizer_main->Add(m_checkbox_smartsizing,0,wxALIGN_LEFT | wxALL,3);
#endif
#ifdef __WXGTK__
	sizer_main->Add(m_checkbox_updatescreen,0,wxALIGN_LEFT | wxALL,3);
#endif
	sizer_main->AddSpacer(1);
	sizer_main->AddSpacer(1);
	sizer_main->Add(m_checkbox_controlsize,0,wxALIGN_LEFT | wxALL,3);


	sizer_top->Add(sizer_main,0,wxALL | wxALIGN_CENTER , 5);
	SetSizer(sizer_top);
	sizer_top->SetSizeHints(this);
	sizer_top->Fit(this); 

#ifdef __WXMSW__
	m_checkbox_updatescreen->Show(false);
	
#endif
#ifdef __WXGTK__
	m_checkbox_smartsizing->Show(false);
	m_checkbox_controlsize->Show(true);
#endif
	CheckRDPDisplay();


}

wxArrayString RDPDisplay::GetResolutionsList()
{
	wxDisplay disp;
	wxArrayString resolutions;
#ifdef __WXMSW__
	wxArrayVideoModes avm = disp.GetModes();
	for (int i = 0; i < (int)avm.Count(); i++)
	{
		if (avm.Item(i).w < 800 || avm.Item(i).h < 600 || avm.Item(i).h > avm.Item(i).w)
			continue;
		wxString temp;
		temp = wxString::Format(wxT("%i x %i"),avm.Item(i).w,avm.Item(i).h);
		BOOL flag = FALSE;
		for (int i = 0; i < (int)resolutions.Count(); i++)
		{	
			if (resolutions.Item(i) == temp) flag = TRUE;
		}
		if (!flag)	resolutions.Add(temp); 
	}
#endif

#ifdef __WXGTK__
	
	resolutions.Add(wxT("800 x 600"));
	resolutions.Add(wxT("1024 x 768"));
	resolutions.Add(wxT("1152 x 864"));
	resolutions.Add(wxT("1280 x 960"));
	resolutions.Add(wxT("1280 x 1024"));

#endif
	return resolutions;
}

wxSize RDPDisplay::GetResolution(int i)
{
	wxSize error_size(0,0);
	if (i < 0) return error_size;
	wxArrayString resolutions = GetResolutionsList();
	if (i >= (int)resolutions.Count())  return error_size;
	wxString resolution_string = resolutions.Item(i); 
	size_t len = resolution_string.Length(); 
	int pos = resolution_string.Find(wxT("x"));
	
	wxString width_string = resolution_string.Left(pos - 1);
	wxString heigth_string = resolution_string.Right(len - pos - 1); 

	wxSize result_size(wxAtoi(width_string),wxAtoi(heigth_string));
	return result_size;
}

int RDPDisplay::GetDefaultResolution()
{
	wxSize disp_size = wxGetDisplaySize();
	wxArrayString resolutions = GetResolutionsList();
	if (resolutions.Count() <= 0) return -1; 
	int default_pos = 0;
	for (int i = 0; i < (int)resolutions.Count(); i ++)
	{
		wxSize size = GetResolution(i);
		if (size.x == 800) return i;
		else 
		{
			if (size.x >= disp_size.x || size.y >= disp_size.y)
			{
				if (default_pos > 0)
				{
					return default_pos - 1;
				}
				else
				{
					return default_pos;
				}
			}
			else
			{
				default_pos ++;
			}
		}
	}
	return default_pos - 1;
}

void RDPDisplay::CheckRDPDisplay()
{
	if (m_checkbox_controlsize->IsChecked())
	{
		m_static_resolution->Enable(false);
		m_combobox_resolution->Enable(false);
		
		m_static_width->Enable(false);
		m_text_width->Enable(false);
		m_static_height->Enable(false);
		m_text_height->Enable(false);

		if (m_checkbox_customgeometry->IsChecked())
		{
			m_static_resolution->Show(false);
			m_combobox_resolution->Show(false);
			s1->Hide(m_static_resolution);
			s1->Show(m_static_width);
			s2->Hide(m_combobox_resolution);
			s2->Show(m_text_width);
		
		}
		else
		{
			m_static_width->Show(false);
			m_text_width->Show(false);
			m_static_height->Show(false);
			m_text_height->Show(false);
			s1->Hide(m_static_width);
			s1->Show(m_static_resolution);
			s2->Show(m_combobox_resolution);
			s2->Hide(m_text_width);
		
		}

		m_checkbox_customgeometry->Enable(false);
		m_checkbox_fullscreen->Enable(false);
	}
	else
	{
		m_checkbox_fullscreen->Enable(true);
		if (m_checkbox_fullscreen->IsChecked())
		{
			m_static_resolution->Enable(false);
			m_combobox_resolution->Enable(false);
		
			m_static_width->Enable(false);
			m_text_width->Enable(false);
			m_static_height->Enable(false);
			m_text_height->Enable(false);

			m_checkbox_customgeometry->Enable(false);
		}
		else
		{
			m_checkbox_customgeometry->Enable(true);
			if (m_checkbox_customgeometry->IsChecked())
			{
				m_static_width->Enable(true);
				m_text_width->Enable(true);
				m_static_height->Enable(true);
				m_text_height->Enable(true);
				m_static_width->Show(true);
				m_text_width->Show(true);
				m_static_height->Show(true);
				m_text_height->Show(true);
				m_static_resolution->Enable(false);
				m_combobox_resolution->Enable(false);
				m_static_resolution->Show(false);
				m_combobox_resolution->Show(false);
				s1->Hide(m_static_resolution);
				s1->Show(m_static_width);
				s2->Hide(m_combobox_resolution);
				s2->Show(m_text_width);


			}
			else
			{
				m_static_width->Enable(false);
				m_text_width->Enable(false);
				m_static_height->Enable(false);
				m_text_height->Enable(false);
				m_static_width->Show(false);
				m_text_width->Show(false);
				m_static_height->Show(false);
				m_text_height->Show(false);
				m_static_resolution->Enable(true);
				m_combobox_resolution->Enable(true);
				m_static_resolution->Show(true);
				m_combobox_resolution->Show(true);
				s1->Hide(m_static_width);
				s1->Show(m_static_resolution);
				s2->Show(m_combobox_resolution);
				s2->Hide(m_text_width);

			}
		}
	}
	s1->Layout();
	s2->Layout();
	sizer_main->Layout();
}

void RDPDisplay::checkbox_customgeometry_func(wxCommandEvent& event)
{
	CheckRDPDisplay();
}


void RDPDisplay::checkbox_fullscreen_func(wxCommandEvent &event)
{
	CheckRDPDisplay();
}

void RDPDisplay::checkbox_controlsize_func(wxCommandEvent &event)
{
	CheckRDPDisplay();
}


RDPDisplay::~RDPDisplay()
{
	if(m_static_width != NULL) {delete m_static_width; m_static_width = NULL;}
	if(m_text_width != NULL) {delete m_text_width; m_text_width = NULL;}
	if(m_static_height != NULL) {delete m_static_height; m_static_height = NULL;}
	if(m_text_height != NULL) {delete m_text_height; m_text_height = NULL;}
	if(m_static_colordepth != NULL) {delete m_static_colordepth; m_static_colordepth = NULL;}
	if(m_choise_colordepth != NULL) {delete m_choise_colordepth; m_choise_colordepth = NULL;}

	if(m_checkbox_fullscreen != NULL) {delete m_checkbox_fullscreen; m_checkbox_fullscreen = NULL;}
	if(m_checkbox_smartsizing != NULL) {delete m_checkbox_smartsizing; m_checkbox_smartsizing = NULL;}
	if(m_checkbox_controlsize != NULL) {delete m_checkbox_controlsize; m_checkbox_controlsize = NULL;}

	if(m_checkbox_updatescreen != NULL) {delete m_checkbox_updatescreen; m_checkbox_updatescreen = NULL;}

	if (m_static_resolution != NULL) {delete m_static_resolution; m_static_resolution = NULL;}
	if (m_combobox_resolution != NULL) {delete m_combobox_resolution; m_combobox_resolution = NULL;}

	if (m_checkbox_customgeometry != NULL) {delete m_checkbox_customgeometry; m_checkbox_customgeometry = NULL;}

}

RDPShare::RDPShare(wxNotebook *frame, const wxPoint& point, const wxSize& size) : wxPanel(frame, wxID_ANY, point , size ) 
{
	m_static_sound = NULL;
	m_choise_sound = NULL;

	m_static_keyboard = NULL;
	m_choise_keyboard = NULL;

	m_checkbox_drives = NULL;
	m_checkbox_printers = NULL;
	m_checkbox_comports = NULL;
	m_checkbox_smartcards = NULL;

	m_static_linux_devices = NULL;
	m_text_linux_devices = NULL;

	sizer_top = new wxBoxSizer(wxVERTICAL);
	sizer_main = new wxFlexGridSizer(2,10,10);
	sizer_devices = new wxBoxSizer(wxVERTICAL);

	m_static_sound  = new wxStaticText(this, wxID_ANY, wxT("Redirect sound"),wxDefaultPosition, wxDefaultSize);
	wxString ChoisesSound[] = {wxT("Bring to this computer"),wxT("Do not play"),wxT("Leave at remote computer")};
	m_choise_sound = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition, wxSize(200,-1),3,ChoisesSound,wxCB_READONLY);
	m_choise_sound->SetValue(ChoisesSound[0]);
	m_choise_sound->SetSelection(0);
	

	m_static_keyboard  = new wxStaticText(this, wxID_ANY, wxT("Using windows key combinations"),wxDefaultPosition, wxDefaultSize);
	wxString ChoisesKeyboard[] = {wxT("On local computer"),wxT("On remote computer"),wxT("In full screen only")};

	m_choise_keyboard = new wxComboBox(this,wxID_ANY,_(""),wxDefaultPosition,wxSize(200,-1),3,ChoisesKeyboard,wxCB_READONLY);
	m_choise_keyboard->SetValue(ChoisesKeyboard[2]);
	m_choise_keyboard->SetSelection(2);
	
	m_checkbox_drives = new wxCheckBox( this, wxID_ANY, wxT("&Redirect drives"), wxDefaultPosition, wxDefaultSize );
	m_checkbox_printers = new wxCheckBox( this, wxID_ANY, wxT("&Redirect printers"), wxDefaultPosition, wxDefaultSize );
	m_checkbox_comports = new wxCheckBox( this, wxID_ANY, wxT("&Redirect COM ports"),wxDefaultPosition, wxDefaultSize );
	m_checkbox_smartcards = new wxCheckBox( this, wxID_ANY, wxT("&Redirect smartcards"), wxDefaultPosition, wxDefaultSize );

	m_static_linux_devices = new wxStaticText(this, wxID_ANY, wxT("&Redirect devices for linux (see man for rdesktop)"),wxDefaultPosition, wxDefaultSize);
	m_text_linux_devices = new wxTextCtrl(this, wxID_ANY, wxT(""),wxDefaultPosition,wxSize(280,-1),wxTE_MULTILINE);

	sizer_main->AddSpacer(1);
	sizer_main->AddSpacer(1);
	sizer_main->Add(m_static_sound,0,wxALIGN_LEFT | wxALL,0);
	sizer_main->Add(m_checkbox_drives,0,wxALIGN_LEFT | wxALL,3);
	sizer_main->Add(m_choise_sound,0,wxALIGN_LEFT | wxALL,0);
	sizer_main->Add(m_checkbox_printers,0,wxALIGN_LEFT | wxALL,3);
	sizer_main->Add(m_static_keyboard,0,wxALIGN_LEFT | wxALL,0);
	sizer_main->Add(m_checkbox_comports,0,wxALIGN_LEFT | wxALL,3);
	sizer_main->Add(m_choise_keyboard,0,wxALIGN_LEFT | wxALL,0);
	sizer_main->Add(m_checkbox_smartcards,0,wxALIGN_LEFT | wxALL,3);


//	sizer_devices->Add(m_text_linux_devices,0,wxALIGN_CENTER | wxEXPAND| wxALL,0);

	sizer_top->Add(sizer_main,0,wxALL | wxALIGN_CENTER , 5);
	sizer_top->Add(m_static_linux_devices,0,wxALL | wxALIGN_LEFT | wxEXPAND, 5);
	sizer_top->Add(m_text_linux_devices,0,wxALL | wxALIGN_LEFT | wxEXPAND, 5);
	SetSizer(sizer_top);
	sizer_top->SetSizeHints(this);
	sizer_top->Fit(this); 


	


#ifdef __WXMSW__
	m_static_linux_devices->Show(false);
	m_text_linux_devices->Show(false);
#endif
#ifdef __WXGTK__
	m_checkbox_drives->Show(false);
	m_checkbox_printers->Show(false);
	m_checkbox_comports->Show(false);
	m_checkbox_smartcards->Show(false);
#endif
}

RDPShare::~RDPShare()
{
	if (m_static_sound) {delete m_static_sound; m_static_sound = NULL;}
	if (m_choise_sound) {delete m_choise_sound; m_choise_sound = NULL;}

	if (m_static_keyboard) {delete m_static_keyboard; m_static_keyboard = NULL;}
	if (m_choise_keyboard) {delete m_choise_keyboard; m_choise_keyboard = NULL;}

	if (m_checkbox_drives) {delete m_checkbox_drives; m_checkbox_drives = NULL;}
	if (m_checkbox_printers) {delete m_checkbox_printers; m_checkbox_printers = NULL;}
	if (m_checkbox_comports) {delete m_checkbox_comports; m_checkbox_comports = NULL;}
	if (m_checkbox_smartcards) {delete m_checkbox_smartcards; m_checkbox_smartcards = NULL;}

	if (m_static_linux_devices) {delete m_static_linux_devices; m_static_linux_devices = NULL;}
	if (m_text_linux_devices) {delete m_text_linux_devices; m_text_linux_devices = NULL;}

}


RDPProgram::RDPProgram(wxNotebook *frame, const wxPoint& point, const wxSize& size) : wxPanel(frame, wxID_ANY, point , size ) 
{
	m_static_program = NULL;
	m_text_program = NULL;

	m_static_workdir = NULL;
	m_text_workdir = NULL;

	m_checkbox_program = NULL;
	m_checkbox_maximized = NULL;

	sizer_top = new wxBoxSizer(wxVERTICAL);
	sizer_main = new wxBoxSizer(wxVERTICAL);

	m_static_program  = new wxStaticText(this, wxID_ANY, wxT("Program path"), wxDefaultPosition, wxDefaultSize);
	m_text_program = new wxTextCtrl( this, wxID_ANY, wxT(""),  wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_static_workdir  = new wxStaticText(this, wxID_ANY, wxT("Working dir"), wxDefaultPosition, wxDefaultSize);
	m_text_workdir = new wxTextCtrl( this, wxID_ANY, wxT(""),  wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	m_checkbox_program = new wxCheckBox( this, ID_CHECKBOX_USE_PROGRAM, wxT("&Start following program"),  wxDefaultPosition, wxDefaultSize );
	m_checkbox_maximized = new wxCheckBox( this, wxID_ANY, wxT("&Start maximized"),  wxDefaultPosition, wxDefaultSize);

	
	sizer_main->Add(m_checkbox_program,0,wxALL | wxALIGN_LEFT , 5);
	sizer_main->AddSpacer(10);
	sizer_main->Add(m_checkbox_maximized,0,wxALL | wxALIGN_LEFT , 5);
	sizer_main->Add(m_static_program,0,wxALL | wxALIGN_LEFT , 5);
	sizer_main->Add(m_text_program,0,wxALL | wxALIGN_LEFT | wxEXPAND, 5);
	sizer_main->Add(m_static_workdir,0,wxALL | wxALIGN_LEFT , 5);
	sizer_main->Add(m_text_workdir,0,wxALL | wxALIGN_LEFT | wxEXPAND, 5);
	sizer_top->Add(sizer_main,0,wxALL| wxALIGN_CENTER | wxEXPAND,5);
	SetSizer(sizer_top);
	sizer_top->SetSizeHints(this);
	sizer_top->Fit(this); 


	m_checkbox_maximized->Enable(false);
	m_static_program->Enable(false);
	m_text_program->Enable(false);
	m_static_workdir->Enable(false);
	m_text_workdir->Enable(false);

}

RDPProgram::~RDPProgram()
{
	if (m_static_program) {delete m_static_program; m_static_program = NULL;}
	if (m_text_program) {delete m_text_program; m_text_program = NULL;}

	if (m_static_workdir) {delete m_static_workdir; m_static_workdir = NULL;}
	if (m_text_workdir) {delete m_text_workdir; m_text_workdir = NULL;}

	if (m_checkbox_program) {delete m_checkbox_program; m_checkbox_program = NULL;}
	if (m_checkbox_maximized) {delete m_checkbox_maximized; m_checkbox_maximized = NULL;}
}

void RDPProgram::checkbox_program_func(wxCommandEvent &event) 
{
	if(!this->m_checkbox_program->IsChecked() )
	{
		m_checkbox_maximized->Enable(false);
		m_static_program->Enable(false);
		m_text_program->Enable(false);
		m_static_workdir->Enable(false);
		m_text_workdir->Enable(false);
	}
	else
	{
		m_checkbox_maximized->Enable(true);
		m_static_program->Enable(true);
		m_text_program->Enable(true);
		m_static_workdir->Enable(true);
		m_text_workdir->Enable(true);
	}
}

RDPPerformance::RDPPerformance(wxNotebook *frame, const wxPoint& point, const wxSize& size) : wxPanel(frame, wxID_ANY, point , size ) 
{
	m_static_speed = NULL;
	m_choise_speed = NULL;

	m_static_performance = NULL;

	m_checkbox_enablewallpaper = NULL;
	m_checkbox_fullwindowdrag = NULL;
	m_checkbox_animation = NULL;
	m_checkbox_themes = NULL;
	m_checkbox_bitmapcaching = NULL;

	sizer_top = new wxBoxSizer(wxVERTICAL);

	sizer_speed = new wxBoxSizer(wxHORIZONTAL);
	m_static_speed  = new wxStaticText(this, wxID_ANY, wxT("Choise connection speed"),wxDefaultPosition, wxDefaultSize);
	ChoisesSpeed[0] = wxT("Modem 28.8 Kbps");
	ChoisesSpeed[1] = wxT("Modem 56 Kbps");
	ChoisesSpeed[2] = wxT("Broadband 128Kbps - 1.5 Mbps");
	ChoisesSpeed[3] = wxT("LAN 10 Mbps or higher");
	ChoisesSpeed[4] = wxT("Custom");
	m_choise_speed = new wxComboBox(this,ID_COMBOBOX_SPEED,wxT(""),wxDefaultPosition,wxSize(220,-1),5,ChoisesSpeed,wxCB_READONLY) ;
	m_choise_speed->SetValue(ChoisesSpeed[1]);
	m_choise_speed->SetSelection(1);

	sizer_speed->Add(m_static_speed,0,wxALL, 5);
	sizer_speed->Add(m_choise_speed,0,wxALL , 2);

	sizer_perf = new wxFlexGridSizer(2,10,10);
	sizer_window = new wxBoxSizer(wxHORIZONTAL);
	
	m_checkbox_bitmapcaching = new wxCheckBox( this, wxID_ANY, wxT("&Bitmap caching"), wxDefaultPosition, wxDefaultSize);
	m_checkbox_bitmapcaching->SetValue(true);

	sizer_static = new wxBoxSizer(wxHORIZONTAL);
	m_static_performance  = new wxStaticText(this, wxID_ANY, wxT("Performance settings"),	wxDefaultPosition, wxDefaultSize);
	sizer_static->Add(m_static_performance,0,wxALL | wxALIGN_CENTER, 5);


	m_checkbox_enablewallpaper = new wxCheckBox( this, wxID_ANY, wxT("&Desktop wallpaper"),wxDefaultPosition, wxDefaultSize );
	m_checkbox_fullwindowdrag = new wxCheckBox( this, wxID_ANY, wxT("&Show contents of window while dragging"),wxDefaultPosition, wxDefaultSize);
	m_checkbox_animation = new wxCheckBox( this, wxID_ANY, wxT("&Animation"), wxDefaultPosition, wxDefaultSize );
	m_checkbox_themes = new wxCheckBox( this, wxID_ANY, wxT("&Themes"), wxDefaultPosition, wxDefaultSize );
	m_checkbox_themes->SetValue(true);

	sizer_perf->Add(m_checkbox_bitmapcaching, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_perf->Add(m_checkbox_themes , 0, wxALL | wxALIGN_LEFT ,5);
	sizer_perf->Add(m_checkbox_enablewallpaper, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_perf->Add(m_checkbox_animation, 0, wxALL | wxALIGN_LEFT ,5);

	sizer_window->Add(m_checkbox_fullwindowdrag,0,wxALL | wxALIGN_LEFT, 5);
	

	sizer_top->Add(sizer_speed,0,wxALL | wxALIGN_LEFT ,5);
	sizer_top->Add(sizer_static,0,wxALL | wxALIGN_CENTER ,5);
	sizer_top->Add(sizer_perf,0,wxALL | wxALIGN_LEFT ,5);
	sizer_top->Add(sizer_window,0,wxALL | wxALIGN_LEFT ,5);

	SetSizer(sizer_top);
	sizer_top->SetSizeHints(this);
	sizer_top->Fit(this); 

}

RDPPerformance::~RDPPerformance()
{
	if (m_static_speed) {delete m_static_speed; m_static_speed = NULL;}
	if (m_static_speed) {delete m_static_speed; m_choise_speed = NULL;}

	if (m_static_speed) {delete m_static_speed; m_static_performance = NULL;}

	if (m_static_speed) {delete m_static_speed; m_checkbox_enablewallpaper = NULL;}
	if (m_static_speed) {delete m_static_speed; m_checkbox_fullwindowdrag = NULL;}
	if (m_static_speed) {delete m_static_speed; m_checkbox_animation = NULL;}
	if (m_static_speed) {delete m_static_speed; m_checkbox_themes = NULL;}
	if (m_static_speed) {delete m_static_speed; m_checkbox_bitmapcaching = NULL;}
}

void RDPPerformance::choise_speed_func(wxCommandEvent &event)
{
	if (!m_choise_speed)	return;

	if (m_choise_speed->GetValue() == this->ChoisesSpeed[0])
	{
		m_checkbox_enablewallpaper->SetValue(false); 
		m_checkbox_fullwindowdrag->SetValue(false); 
		m_checkbox_animation->SetValue(false);
		m_checkbox_themes->SetValue(false);
		m_checkbox_bitmapcaching->SetValue(true);
		return;
	}
	if (m_choise_speed->GetValue() == this->ChoisesSpeed[1])
	{
		m_checkbox_enablewallpaper->SetValue(false); 
		m_checkbox_fullwindowdrag->SetValue(false); 
		m_checkbox_animation->SetValue(false);
		m_checkbox_themes->SetValue(true);
		m_checkbox_bitmapcaching->SetValue(true);
		return;
	}
	if (m_choise_speed->GetValue() == this->ChoisesSpeed[2])
	{
		m_checkbox_enablewallpaper->SetValue(false); 
		m_checkbox_fullwindowdrag->SetValue(true); 
		m_checkbox_animation->SetValue(true);
		m_checkbox_themes->SetValue(true);
		m_checkbox_bitmapcaching->SetValue(true);
		return;
	}
	if (m_choise_speed->GetValue() == this->ChoisesSpeed[3])
	{
		m_checkbox_enablewallpaper->SetValue(true); 
		m_checkbox_fullwindowdrag->SetValue(true); 
		m_checkbox_animation->SetValue(true);
		m_checkbox_themes->SetValue(true);
		m_checkbox_bitmapcaching->SetValue(true);
		return;
	}
	if (m_choise_speed->GetValue() == this->ChoisesSpeed[4])
	{
		m_checkbox_enablewallpaper->SetValue(false); 
		m_checkbox_fullwindowdrag->SetValue(false); 
		m_checkbox_animation->SetValue(false);
		m_checkbox_themes->SetValue(true);
		m_checkbox_bitmapcaching->SetValue(true);
		return;
	}
	
	m_checkbox_enablewallpaper->SetValue(false); 
	m_checkbox_fullwindowdrag->SetValue(false); 
	m_checkbox_animation->SetValue(false);
	m_checkbox_themes->SetValue(true);
	m_checkbox_bitmapcaching->SetValue(true);
	return;

}
RDPAdvanced::RDPAdvanced(wxNotebook *frame, const wxPoint& point, const wxSize& size)
	: wxPanel(frame, wxID_ANY, point, size ) 
{
	m_static_rdpversion = NULL;
	m_choise_rdpversion = NULL; 
	m_checkbox_french = NULL;  
	m_checkbox_encryption_enable = NULL;	
	m_checkbox_backingstore = NULL;
	m_checkbox_usemouse = NULL;
	m_checkbox_privatecolormap = NULL; 
	m_checkbox_numlock = NULL;
	m_checkbox_enablecompress = NULL;

	m_combobox_keyboard_map = NULL;
	m_static_keyboard_map = NULL;

	sizer_top = new wxBoxSizer(wxVERTICAL);
	sizer_version = new wxBoxSizer(wxHORIZONTAL);

	m_static_rdpversion = new wxStaticText(this, wxID_ANY, wxT("Choise RDP version"),wxDefaultPosition, wxDefaultSize);
	wxString ChoiseVersion[] = {wxT("Use RDP 4"),wxT("Use RDP 5"),wxT("Use RDP 6")};
	m_choise_rdpversion = new wxComboBox(this,wxID_ANY,_(""),wxDefaultPosition,wxSize(220,-1),2,ChoiseVersion,wxCB_READONLY) ;
	m_choise_rdpversion->SetValue(ChoiseVersion[1]);
	m_choise_rdpversion->SetSelection(1);
	sizer_version->Add(m_static_rdpversion,0,wxALL, 5);
	sizer_version->Add(m_choise_rdpversion,0,wxALL | wxALIGN_CENTER , 2);

	sizer_crypt = new wxBoxSizer(wxHORIZONTAL);
	m_checkbox_french	= new wxCheckBox( this, wxID_ANY, wxT("&Enable encrypt for French version"), wxDefaultPosition, wxDefaultSize );
	sizer_crypt->Add(m_checkbox_french,0,wxALL, 5);

	sizer_settings = new wxFlexGridSizer(2,10,10);

	m_checkbox_encryption_enable = new wxCheckBox( this, wxID_ANY, wxT("&Enable encryption"), wxDefaultPosition, wxDefaultSize );	
	m_checkbox_backingstore = new wxCheckBox( this, wxID_ANY, wxT("&Use BackingStore"), wxDefaultPosition, wxDefaultSize )	;
	m_checkbox_usemouse = new wxCheckBox( this, wxID_ANY, wxT("&Use mouse"), wxDefaultPosition, wxDefaultSize );
	m_checkbox_privatecolormap = new wxCheckBox( this, wxID_ANY, wxT("&Use private color map"),wxDefaultPosition, wxDefaultSize );
	m_checkbox_numlock = new wxCheckBox(this,wxID_ANY,wxT("&Num Lock sync"),wxDefaultPosition, wxDefaultSize);
	m_checkbox_enablecompress = new wxCheckBox(this,wxID_ANY,wxT("&Enable compress"),wxDefaultPosition, wxDefaultSize);

	sizer_settings->Add(m_checkbox_encryption_enable, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_settings->Add(m_checkbox_backingstore, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_settings->Add(m_checkbox_usemouse, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_settings->Add(m_checkbox_privatecolormap, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_settings->Add(m_checkbox_numlock, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_settings->Add(m_checkbox_enablecompress, 0, wxALL | wxALIGN_LEFT ,5);

	sizer_keymap = new wxBoxSizer(wxHORIZONTAL);

	m_static_keyboard_map = new wxStaticText(this, wxID_ANY, wxT("Choise keyboard map"),wxDefaultPosition, wxDefaultSize);
#ifdef __WXGTK__
	GETRDESKTOPMAPPATH();
	wxArrayString list_keyboard_map;
	wxString temp_str;
	wxDir temp(RDESKTOPMAPPATH);
	if ((RDESKTOPMAPPATH != wxT("")) && (temp.Exists(RDESKTOPMAPPATH)))
	{
		bool check = temp.GetFirst(&temp_str);
		list_keyboard_map.Add( wxT("en-us"));
		while (check == true)
		{
			if (temp_str != wxT("en-us"))
			{
				list_keyboard_map.Add(temp_str);
			}
			check = temp.GetNext(&temp_str);
		}
	}
	else 
	{
		list_keyboard_map.Add( wxT("en-us"));
	}

	m_combobox_keyboard_map = new wxComboBox(this, wxID_ANY,wxT(""),wxPoint(145 + 0.8*SHIFT_W,160), wxSize(120 + 0.8*SHIFT_W,CONTROL_H),
		list_keyboard_map,wxCB_READONLY, wxDefaultValidator,wxT(""));
	m_combobox_keyboard_map->SetValue(list_keyboard_map[0]);
	m_combobox_keyboard_map->SetSelection(0);

#endif

#ifdef __WXMSW__
	wxString list_keyboard_map[] = {_T("en"),_T("ru")};
	m_combobox_keyboard_map = new wxComboBox(this,wxID_ANY, wxT("") , wxDefaultPosition, wxSize(120,-1),2,list_keyboard_map,wxCB_READONLY);
	m_combobox_keyboard_map->SetValue(ChoiseVersion[1]);
	m_combobox_keyboard_map->SetSelection(1);
#endif

	sizer_keymap->Add(m_static_keyboard_map,0,wxALL, 5);
	sizer_keymap->Add(m_combobox_keyboard_map,0,wxALL | wxALIGN_CENTER , 2);

	sizer_top->Add(sizer_version,0,wxALL | wxALIGN_LEFT ,5);
	sizer_top->Add(sizer_crypt,0,wxALL | wxALIGN_LEFT ,5);
	sizer_top->Add(sizer_settings,0,wxALL | wxALIGN_LEFT ,5);
	sizer_top->Add(sizer_keymap,0,wxALL | wxALIGN_LEFT ,5);

	SetSizer(sizer_top);
	sizer_top->SetSizeHints(this);
	sizer_top->Fit(this); 

}

RDPAdvanced::~RDPAdvanced()
{
	if (m_static_rdpversion != NULL) {delete m_static_rdpversion; m_static_rdpversion = NULL;}
	if (m_choise_rdpversion != NULL) {delete m_choise_rdpversion; m_choise_rdpversion = NULL;}
	if (m_checkbox_french != NULL) {delete m_checkbox_french; m_checkbox_french = NULL;}
	if (m_checkbox_encryption_enable != NULL) {delete m_checkbox_encryption_enable; m_checkbox_encryption_enable = NULL;}
	if (m_checkbox_backingstore != NULL) {delete m_checkbox_backingstore; m_checkbox_backingstore = NULL;}
	if (m_checkbox_usemouse != NULL) {delete m_checkbox_usemouse; m_checkbox_usemouse = NULL;}
	if (m_checkbox_privatecolormap != NULL) {delete m_checkbox_privatecolormap; m_checkbox_privatecolormap = NULL;}
	if (m_checkbox_numlock != NULL) {delete m_checkbox_numlock; m_checkbox_numlock = NULL;}
	if (m_checkbox_enablecompress != NULL) {delete m_checkbox_enablecompress; m_checkbox_enablecompress = NULL;}

	if (m_static_keyboard_map) {delete m_static_keyboard_map; m_static_keyboard_map = NULL;}
	if (m_combobox_keyboard_map) {delete m_combobox_keyboard_map; m_combobox_keyboard_map = NULL;}
}

void RDPDialog::button_cancel_func(wxCommandEvent &event)
{
	this->EndModal(0); 
}

BOOL RDPDialog::FillRDPConn()
{ 
	wxString strTemp;
	long ltemp = 0;
	strTemp.Empty();
	wxDateTime time = wxDateTime::UNow();
	
	if (new_uniq_name)
	{
		Benc bc;
		prdpconn->uniq_name = bc.generate_uniq_name(rdp_base);		
	}

	if (this->m_rdpgeneral != NULL)
	{
		if (this->m_rdpgeneral->m_text_server != NULL)
		{
			strTemp = this->m_rdpgeneral->m_text_server->GetValue();
			if (strTemp.Length() == 0)
			{
				wxMessageBox(_("Enter server name or IP address"),_("Error"),wxICON_ERROR);
				return FALSE;
			}
			strTemp.Empty();
		}
		// General settings
		if (this->m_rdpgeneral->m_text_server != NULL)
		{
			prdpconn->hostname = this->m_rdpgeneral->m_text_server->GetValue();
		}
		if (this->m_rdpgeneral->m_text_username != NULL)
		{
			prdpconn->username = this->m_rdpgeneral->m_text_username->GetValue();
		}
		if (this->m_rdpgeneral->m_checkbox_autologon != NULL)
		{
			if (this->m_rdpgeneral->m_checkbox_autologon->IsChecked())
			{
				prdpconn->password = this->m_rdpgeneral->m_text_password->GetValue();
			}
		}
		if (this->m_rdpgeneral->m_text_domain != NULL)
		{
			prdpconn->domain = this->m_rdpgeneral->m_text_domain->GetValue();
		}
		if (this->m_rdpgeneral->m_checkbox_customport != NULL)
		{
			if (this->m_rdpgeneral->m_checkbox_customport->IsChecked())
			{
				prdpconn->port = this->m_rdpgeneral->m_text_port->GetValue();
			}
			else
			{
				prdpconn->port = wxString(_("3389"));
			}
		}
		if (this->m_rdpgeneral->m_checkbox_attachtoconsole != NULL)
		{
			if (this->m_rdpgeneral->m_checkbox_attachtoconsole->IsChecked())
			{
				prdpconn->attach_to_console = TRUE;
			}
			else
			{
				prdpconn->attach_to_console = FALSE;
			}
		}

		if (this->m_rdpgeneral->m_text_connectionname != NULL)
		{
			prdpconn->connection_name = this->m_rdpgeneral->m_text_connectionname->GetValue();
		}

		if (this->m_rdpgeneral->m_combobox_groupname != NULL)
		{
			prdpconn->group_name = this->m_rdpgeneral->m_combobox_groupname->GetValue();
		}


	}
	// Display settings
	if (this->m_rdpdisplay != NULL)
	{
		if (!m_rdpdisplay->m_checkbox_customgeometry->IsChecked())
		{
			int index = m_rdpdisplay->m_combobox_resolution->GetSelection();
			wxArrayString resolutions = m_rdpdisplay->GetResolutionsList();
			wxSize error_size(0,0);
			if (index >= 0)
			{
				wxSize rdp_size = m_rdpdisplay->GetResolution(index);
				if (rdp_size != error_size)
				{
					wxString str;
					str.Printf(wxT("%d"),rdp_size.x);
					m_rdpdisplay->m_text_width->SetValue(str);
					str.Printf(wxT("%d"),rdp_size.y); 
					m_rdpdisplay->m_text_height->SetValue(str);
				}

			}


		}
		if (this->m_rdpdisplay->m_text_height != NULL)
		{
			strTemp = this->m_rdpdisplay->m_text_height->GetValue();
			if (strTemp.Length() == 0) 
			{
				wxMessageBox(_("Enter display height"),_("Error"));
				return FALSE;
			}
			strTemp.Empty();
		}
		if (this->m_rdpdisplay->m_text_width != NULL)
		{
			strTemp = this->m_rdpdisplay->m_text_width->GetValue();
			if (strTemp.Length() == 0) 
			{
				wxMessageBox(_("Enter display width"),_("Error"));
				return FALSE;
			}
		}
		if (this->m_rdpdisplay->m_text_width != NULL)
		{
			(this->m_rdpdisplay->m_text_width->GetValue()).ToLong(&ltemp);
			prdpconn->width = (ushort)ltemp;
		}
		if (this->m_rdpdisplay->m_text_height != NULL)
		{
			(this->m_rdpdisplay->m_text_height->GetValue()).ToLong(&ltemp);
			prdpconn->heigth = (ushort)ltemp;
		}
		if (this->m_rdpdisplay->m_choise_colordepth != NULL)
		{
			strTemp = this->m_rdpdisplay->m_choise_colordepth->GetValue();
			if (strTemp == _("8-bit")) prdpconn->color_depth = 8;
			else if (strTemp == _("15-bit")) prdpconn->color_depth = 15;
			else if (strTemp == _("16-bit")) prdpconn->color_depth = 16;
			else if (strTemp == _("24-bit")) prdpconn->color_depth = 24;
			else  prdpconn->color_depth = 16;
			strTemp.Empty();
		}

		if (this->m_rdpdisplay->m_checkbox_controlsize != NULL)
		{
			if (this->m_rdpdisplay->m_checkbox_controlsize->IsChecked()) 
			{
				prdpconn->bControlSize = TRUE;
				prdpconn->bFullScreen = FALSE;
				prdpconn->bSmartSizing = FALSE;
			}
			else 
			{
				prdpconn->bControlSize = FALSE;

				if (this->m_rdpdisplay->m_checkbox_fullscreen->IsChecked()) 
				{
					prdpconn->bFullScreen = TRUE;
					
				}
				else prdpconn->bFullScreen = FALSE;
				if (this->m_rdpdisplay->m_checkbox_smartsizing->IsChecked()) prdpconn->bSmartSizing = TRUE;
				else prdpconn->bSmartSizing = FALSE;
			}
		}
	}

	if (this->m_rdpdisplay->m_checkbox_updatescreen != NULL)
	{
		if (this->m_rdpdisplay->m_checkbox_updatescreen->IsChecked())
		{
			prdpconn->force_update_screen = TRUE;
		}
		else
		{
			prdpconn->force_update_screen = FALSE;
		}
	}

	// Share settings

	if (this->m_rdpshare != NULL)
	{
		if (this->m_rdpshare->m_checkbox_drives != NULL)
		{
			if (this->m_rdpshare->m_checkbox_drives->IsChecked())
			{
				prdpconn->bShareDrives = TRUE;
			}
			else
			{
				prdpconn->bShareDrives = FALSE;
			}
		}
		if (this->m_rdpshare->m_checkbox_printers != NULL)
		{
			if (this->m_rdpshare->m_checkbox_printers->IsChecked())
			{
				prdpconn->bSharePrinters  = TRUE;
			}
			else
			{
				prdpconn->bSharePrinters  = FALSE;
			}
		}
		if (this->m_rdpshare->m_checkbox_comports != NULL)
		{
			if (this->m_rdpshare->m_checkbox_comports->IsChecked())
			{
				prdpconn->bShareComPorts = TRUE;
			}
			else
			{
				prdpconn->bShareComPorts = FALSE;
			}
		}
		if (this->m_rdpshare->m_checkbox_smartcards != NULL)
		{
			if (this->m_rdpshare->m_checkbox_smartcards->IsChecked())
			{
				prdpconn->bShareSmartCards = TRUE;
			}
			else
			{
				prdpconn->bShareSmartCards = FALSE;
			}
		}
		if (this->m_rdpshare->m_choise_sound != NULL)
		{
			prdpconn->SoundType = this->m_rdpshare->m_choise_sound->GetCurrentSelection();
		}
		if (this->m_rdpshare->m_choise_keyboard != NULL)
		{
			prdpconn->keyboard = this->m_rdpshare->m_choise_keyboard->GetCurrentSelection();
		}

		if (this->m_rdpshare->m_text_linux_devices != NULL)
		{
			prdpconn->redirect_devices_nix = this->m_rdpshare->m_text_linux_devices->GetValue();
		}
	}

	// Program settings
	if (this->m_rdpprogram != NULL)
	{
		if (this->m_rdpprogram->m_checkbox_program != NULL)
		{
			if (this->m_rdpprogram->m_checkbox_program->IsChecked())
			{
				prdpconn->bUseProgram = TRUE;
				prdpconn->shell = this->m_rdpprogram->m_text_program->GetValue();
				prdpconn->directory = this->m_rdpprogram->m_text_workdir->GetValue();

				if (this->m_rdpprogram->m_checkbox_maximized != NULL)
				{
					if (this->m_rdpprogram->m_checkbox_maximized->IsChecked())
						prdpconn->bProgramMaximized = TRUE;
					else	
						prdpconn->bProgramMaximized = FALSE;

				}
			}
			else
			{
				prdpconn->bUseProgram = FALSE;
			}
		}
	}

	// Performance settings

	if (this->m_rdpperformance != NULL)
	{
		if (this->m_rdpperformance->m_checkbox_bitmapcaching != NULL)
		{
			if (this->m_rdpperformance->m_checkbox_bitmapcaching->IsChecked())
				prdpconn->bEnableBitmapCaching = TRUE;
			else
				prdpconn->bEnableBitmapCaching = FALSE;
		}

		if (this->m_rdpperformance->m_checkbox_animation != NULL)
		{
			if (this->m_rdpperformance->m_checkbox_animation->IsChecked())
				prdpconn->bEnableAnimation = TRUE;
			else
				prdpconn->bEnableAnimation = FALSE;
		}
		if (this->m_rdpperformance->m_checkbox_enablewallpaper != NULL)
		{
			if (this->m_rdpperformance->m_checkbox_enablewallpaper->IsChecked())
				prdpconn->bEnableWallpaper = TRUE;
			else
				prdpconn->bEnableWallpaper = FALSE;
		}
		if (this->m_rdpperformance->m_checkbox_fullwindowdrag != NULL)
		{
			if (this->m_rdpperformance->m_checkbox_fullwindowdrag->IsChecked())
				prdpconn->bEnableFullWindowDrag = TRUE;
			else
				prdpconn->bEnableFullWindowDrag = FALSE;
		}
		if (this->m_rdpperformance->m_checkbox_themes != NULL)
		{
			if (this->m_rdpperformance->m_checkbox_themes->IsChecked())
				prdpconn->bEnableThemes = TRUE;
			else
				prdpconn->bEnableThemes = FALSE;
		}

		if (this->m_rdpperformance->m_choise_speed != NULL)
		{
			prdpconn->bandwidth = this->m_rdpperformance->m_choise_speed->GetCurrentSelection();
		}
	}
	// Advanced settings

	if (this->m_rdpadvanced != NULL)
	{
		if (this->m_rdpadvanced->m_choise_rdpversion != NULL)
		{
			prdpconn->use_rdp_version = this->m_rdpadvanced->m_choise_rdpversion->GetCurrentSelection();
		}
		if (this->m_rdpadvanced->m_checkbox_backingstore != NULL)
		{
			if (this->m_rdpadvanced->m_checkbox_backingstore->IsChecked())
			{
				prdpconn->backing_store = TRUE;
			}
			else
			{
				prdpconn->backing_store = FALSE;
			}
		}
		if (m_rdpadvanced->m_checkbox_enablecompress != NULL)
		{
			if (m_rdpadvanced->m_checkbox_enablecompress->IsChecked()) prdpconn->enable_compres = TRUE; 
			else prdpconn->enable_compres = FALSE;
		}
		if (m_rdpadvanced->m_checkbox_encryption_enable != NULL)
		{
			if (m_rdpadvanced->m_checkbox_encryption_enable->IsChecked()) prdpconn->encription_enable_new = TRUE;
			else prdpconn->encription_enable_new = FALSE;
		}
		if (m_rdpadvanced->m_checkbox_french != NULL)
		{
			if (m_rdpadvanced->m_checkbox_french->IsChecked()) prdpconn->encription_enable_french = TRUE;
			else prdpconn->encription_enable_french = FALSE;
		}
		if (m_rdpadvanced->m_checkbox_numlock != NULL)
		{
			if (m_rdpadvanced->m_checkbox_numlock->IsChecked()) prdpconn->numlock_sync = TRUE;
			else prdpconn->numlock_sync = FALSE;
		}
		if (m_rdpadvanced->m_checkbox_privatecolormap != NULL)
		{
			if (m_rdpadvanced->m_checkbox_privatecolormap->IsChecked() ) prdpconn->private_color_map  = TRUE;
			else  prdpconn->private_color_map  = FALSE;
		}
		if ( m_rdpadvanced->m_checkbox_usemouse != NULL )
		{
			if (m_rdpadvanced->m_checkbox_usemouse->IsChecked()  ) prdpconn->send_mouse_event = TRUE;
			else prdpconn->send_mouse_event = FALSE;
		}
		if ( m_rdpadvanced->m_combobox_keyboard_map != NULL )
		{
			prdpconn->keyboard_map = m_rdpadvanced->m_combobox_keyboard_map->GetValue(); 
		}
	}
	return TRUE;

}


void RDPDialog::button_ok_func(wxCommandEvent &event) 
{
	if (!FillRDPConn())
	{
		return;
	}
	this->EndModal(1);
}

void RDPDialog::LoadRDPConn()
{
	// General settings
	m_rdpgeneral->m_text_server->SetValue(prdpconn->hostname);
	m_rdpgeneral->m_text_username->SetValue(prdpconn->username);
	if (prdpconn->password.Length() > 0)
	{
		m_rdpgeneral->m_checkbox_autologon->SetValue(true);	
		m_rdpgeneral->m_text_password->SetValue(prdpconn->password);
	}
	else m_rdpgeneral->m_checkbox_autologon->SetValue(false);	

	m_rdpgeneral->m_text_domain->SetValue(prdpconn->domain);

	if (prdpconn->port != wxString(_("3389")))
	{
		m_rdpgeneral->m_checkbox_customport->SetValue(true);
		m_rdpgeneral->m_text_port->SetValue(prdpconn->port);
	}
	else m_rdpgeneral->m_checkbox_customport->SetValue(false);

	
	if (prdpconn->attach_to_console) m_rdpgeneral->m_checkbox_attachtoconsole->SetValue(true); 
	else m_rdpgeneral->m_checkbox_attachtoconsole->SetValue(false); 

	
	if (prdpconn->group_name.Length() > 0)
	{
		m_rdpgeneral->m_combobox_groupname->SetValue(prdpconn->group_name);
	}

	if (prdpconn->connection_name.Length() > 0)
	{
		m_rdpgeneral->m_text_connectionname->SetValue(prdpconn->connection_name);
	}

	// Display settings
	wxString str;
	str.Printf(wxT("%d"),prdpconn->width);
	m_rdpdisplay->m_text_width->SetValue(str);
	str.Printf(wxT("%d"),prdpconn->heigth); 
	m_rdpdisplay->m_text_height->SetValue(str);

	wxSize rdp_size = wxSize(prdpconn->width,prdpconn->heigth);
	wxArrayString resolutions = m_rdpdisplay->GetResolutionsList();
	bool bCustomGeometryFlag = true;
	for (int i = 0; i < (int)resolutions.Count(); i ++)
	{
		wxSize temp_size = m_rdpdisplay->GetResolution(i);
		if (temp_size == rdp_size)
		{
			m_rdpdisplay->m_combobox_resolution->SetSelection(i);
			m_rdpdisplay->m_combobox_resolution->SetValue(resolutions.Item(i));
			bCustomGeometryFlag = false;
		}
	}
	m_rdpdisplay->m_checkbox_customgeometry->SetValue(bCustomGeometryFlag);

	BYTE bTemp = prdpconn->color_depth;
	if (bTemp == 8) m_rdpdisplay->m_choise_colordepth->SetValue(_("8-bit"));
	else if (bTemp == 15) m_rdpdisplay->m_choise_colordepth->SetValue(_("15-bit"));
	else if (bTemp == 16) m_rdpdisplay->m_choise_colordepth->SetValue(_("16-bit"));
	else if (bTemp == 24) m_rdpdisplay->m_choise_colordepth->SetValue(_("24-bit"));
	else m_rdpdisplay->m_choise_colordepth->SetValue(_("16-bit"));

	if (prdpconn->bFullScreen) m_rdpdisplay->m_checkbox_fullscreen->SetValue(true);
	else  m_rdpdisplay->m_checkbox_fullscreen->SetValue(false);

	if (prdpconn->bSmartSizing) m_rdpdisplay->m_checkbox_smartsizing->SetValue(true);
	else  m_rdpdisplay->m_checkbox_smartsizing->SetValue(false);
	
	if (prdpconn->bControlSize) m_rdpdisplay->m_checkbox_controlsize->SetValue(true);
	else
	{
		m_rdpdisplay->m_checkbox_controlsize->SetValue(false);
	}
	
	if (!prdpconn->bControlSize)
	{
		m_rdpdisplay->m_checkbox_fullscreen->Enable(true);
		if (!prdpconn->bFullScreen)
		{
			m_rdpdisplay->m_static_width->Enable(true);
			m_rdpdisplay->m_text_width->Enable(true);
			m_rdpdisplay->m_static_height->Enable(true);
			m_rdpdisplay->m_text_height->Enable(true);
		}
		else
		{
			m_rdpdisplay->m_static_width->Enable(false);
			m_rdpdisplay->m_text_width->Enable(false);
			m_rdpdisplay->m_static_height->Enable(false);
			m_rdpdisplay->m_text_height->Enable(false);
		}
	}
	else
	{
		m_rdpdisplay->m_checkbox_fullscreen->Enable(false);
		m_rdpdisplay->m_static_width->Enable(false);
		m_rdpdisplay->m_text_width->Enable(false);
		m_rdpdisplay->m_static_height->Enable(false);
		m_rdpdisplay->m_text_height->Enable(false);
	}
	m_rdpdisplay->CheckRDPDisplay();

	// Share settings
	if (prdpconn->bShareDrives) m_rdpshare->m_checkbox_drives->SetValue(true);
	else m_rdpshare->m_checkbox_drives->SetValue(false);

	if (prdpconn->bSharePrinters) m_rdpshare->m_checkbox_printers->SetValue(true);
	else m_rdpshare->m_checkbox_printers->SetValue(false);

	if (prdpconn->bShareComPorts) m_rdpshare->m_checkbox_comports->SetValue(true);
	else m_rdpshare->m_checkbox_comports->SetValue(false);

	if (prdpconn->bShareSmartCards) m_rdpshare->m_checkbox_smartcards->SetValue(true);
	else m_rdpshare->m_checkbox_smartcards->SetValue(false);

	m_rdpshare->m_choise_sound->SetSelection(prdpconn->SoundType);
	m_rdpshare->m_choise_keyboard->SetSelection(prdpconn->keyboard);

	if (prdpconn->redirect_devices_nix.Length() > 0)
	{
		m_rdpshare->m_text_linux_devices->SetValue(prdpconn->redirect_devices_nix);
	}

	// Program settings
	
	if (prdpconn->bUseProgram)
	{
		m_rdpprogram->m_checkbox_program->SetValue(true);
		m_rdpprogram->m_text_program->SetValue(prdpconn->shell);
		m_rdpprogram->m_text_workdir->SetValue(prdpconn->directory);
		m_rdpprogram->m_checkbox_maximized->Enable(true);
		m_rdpprogram->m_static_program->Enable(true);
		m_rdpprogram->m_text_program->Enable(true);
		m_rdpprogram->m_static_workdir->Enable(true);
		m_rdpprogram->m_text_workdir->Enable(true);
		if (prdpconn->bProgramMaximized)
		{
			m_rdpprogram->m_checkbox_maximized->SetValue(true);
		}
		else
		{
			m_rdpprogram->m_checkbox_maximized->SetValue(false);
		}
	}
	else
	{
		m_rdpprogram->m_checkbox_program->SetValue(false);
		m_rdpprogram->m_checkbox_maximized->Enable(false);
		m_rdpprogram->m_static_program->Enable(false);
		m_rdpprogram->m_text_program->Enable(false);
		m_rdpprogram->m_static_workdir->Enable(false);
		m_rdpprogram->m_text_workdir->Enable(false);
	}


	// Performance settings
	if (prdpconn->bEnableBitmapCaching) m_rdpperformance->m_checkbox_bitmapcaching->SetValue(true);
	else m_rdpperformance->m_checkbox_bitmapcaching->SetValue(false);

	if (prdpconn->bEnableAnimation) m_rdpperformance->m_checkbox_animation->SetValue(true);
	else m_rdpperformance->m_checkbox_animation->SetValue(false);

	if (prdpconn->bEnableWallpaper) m_rdpperformance->m_checkbox_enablewallpaper->SetValue(true);
	else m_rdpperformance->m_checkbox_enablewallpaper->SetValue(false);

	if (prdpconn->bEnableFullWindowDrag) m_rdpperformance->m_checkbox_fullwindowdrag->SetValue(true);
	else m_rdpperformance->m_checkbox_fullwindowdrag->SetValue(false);

	if (prdpconn->bEnableThemes) m_rdpperformance->m_checkbox_themes->SetValue(true);
	else m_rdpperformance->m_checkbox_themes->SetValue(false);

	m_rdpperformance->m_choise_speed->SetValue(m_rdpperformance->ChoisesSpeed[4]);

	//Advanced settings
	if (prdpconn->backing_store) m_rdpadvanced->m_checkbox_backingstore->SetValue(true);
	else m_rdpadvanced->m_checkbox_backingstore->SetValue(false);

	if (prdpconn->enable_compres) m_rdpadvanced->m_checkbox_enablecompress->SetValue(true);
	else m_rdpadvanced->m_checkbox_enablecompress->SetValue(false);

	if (prdpconn->encription_enable_new) m_rdpadvanced->m_checkbox_encryption_enable->SetValue(true);
	else m_rdpadvanced->m_checkbox_encryption_enable->SetValue(false);

	if (prdpconn->encription_enable_french) m_rdpadvanced->m_checkbox_french->SetValue(true);
	else m_rdpadvanced->m_checkbox_french->SetValue(false);

	if (prdpconn->numlock_sync) m_rdpadvanced->m_checkbox_numlock->SetValue(true);
	else m_rdpadvanced->m_checkbox_backingstore->SetValue(false);

	if (prdpconn->private_color_map) m_rdpadvanced->m_checkbox_privatecolormap->SetValue(true);
	else m_rdpadvanced->m_checkbox_privatecolormap->SetValue(false);

	if (prdpconn->send_mouse_event) m_rdpadvanced->m_checkbox_usemouse->SetValue(true);
	else m_rdpadvanced->m_checkbox_usemouse->SetValue(false);

	m_rdpadvanced->m_choise_rdpversion->SetSelection(prdpconn->use_rdp_version);
	
	m_rdpadvanced->m_combobox_keyboard_map->SetValue(prdpconn->keyboard_map);
}


void RDPDialog::button_save_func(wxCommandEvent &event)
{
	RDPConn lrdpconn;
#ifdef __WXMSW__
	lrdpconn.uniq_name = prdpconn->uniq_name;
#endif
#ifdef __WXGTK__
	if (new_uniq_name)
	{
		lrdpconn.uniq_name = 0;
	}
	else
	{
		lrdpconn.uniq_name = prdpconn->uniq_name;
	}
#endif	
	lrdpconn.dwConnectionCount =  prdpconn->dwConnectionCount;

	prdpconn = &lrdpconn;

	if(!FillRDPConn()) 
	{
		return;
	}   
	if (new_uniq_name)
	{
		prdpconn->dwConnectionCount = 0;
	}
	
	Benc bc;
	bc.Add(rdp_base,*prdpconn);

	this->EndModal(2); 

}

void RDPDialog::dialog_hotkeys(wxKeyEvent &event)
{
	wxCommandEvent evt;
	evt.SetId(1); 
	switch(event.GetKeyCode())
	{
	case WXK_RETURN:
		this->button_ok_func(evt);


		break;
	case WXK_ESCAPE:
		this->button_cancel_func(evt);
		break;

	case WXK_TAB:
		if (event.GetModifiers() == wxMOD_SHIFT)
		{
			if (notebook)
			{
				size_t iSel = (notebook->GetSelection() + 1) % (notebook->GetPageCount());
				notebook->SetSelection(iSel);
			}
		}
		break;
	default:
		break;

	}
	
	event.Skip();
}
