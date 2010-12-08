#include "RFBOptionsDialog.hpp"
#include <wx/display.h>
#include <wx/dir.h>

BEGIN_EVENT_TABLE(RFBOptionsDialog_General, wxPanel)
EVT_CHECKBOX (ID_RFB_CHECKBOX_CUSTOMPORT, RFBOptionsDialog_General::checkbox_customport_func)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(RFBOptionsDialog_Display, wxPanel)
EVT_RADIOBUTTON (ID_RFB_CHK_CONTROLSIZE, RFBOptionsDialog_Display::checkbox_controlsize_func)
END_EVENT_TABLE()


RFBOptionsDialog::RFBOptionsDialog(wxWindow *parent_element) :
	parent(parent_element),
	rfb_general(NULL)
{
	save_options.clear();
	error = parent != NULL ? 0 : RFB_ERROR_PARENT_IS_NULL;
}

RFBOptionsDialog::~RFBOptionsDialog() {
	DeleteAllPages();
}

bool RFBOptionsDialog::SetOptions (const Options_HashMap *options) {
	bool state = false;
	if (options != NULL) {
		save_options = *options;
		//wxMessageBox(save_options[wxT("uniq_name")]);
		if (rfb_general != NULL)
			rfb_general->SetOptions(options);
		if(rfb_display != NULL)
		  rfb_display->SetOptions(options);
	}
	return state;
}

void RFBOptionsDialog::SetGroupList(const wxArrayString &group_list) {
	array_string_group_list = group_list;
}

Options_HashMap RFBOptionsDialog::GetOptions ()
{
   Options_HashMap options, temp_options;
   Options_HashMap::iterator it;

   if (rfb_general != NULL)
   {
      options.clear();
      options = rfb_general->GetOptions();
   }
   if ((rfb_display != NULL) &&
       (options.size() > 0))
   {
	   temp_options.clear();
	   temp_options = rfb_display->GetOptions();
	   for( it = temp_options.begin(); it != temp_options.end(); ++it )
	   {
		   wxString key = it->first, value = it->second;
		   options[key] = value;
	   }
   }

   if (options.size() > 0)
   {
      for( it = options.begin(); it != options.end(); ++it )
      {
   		 wxString key = it->first, value = it->second;
   		 save_options[key] = value;
      }
   }
   return save_options;
}

int RFBOptionsDialog::Build()
{
   if ( error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY);
      if ( state == false)
      {
		error = RFB_ERROR_CANT_CREATE_NOTEBOOK;
      }
      else
      {
		rfb_general = new RFBOptionsDialog_General (this);
		rfb_general->SetGroupList(array_string_group_list);
		rfb_general->Build();
		rfb_display = new RFBOptionsDialog_Display(this);
		rfb_display->Build();

		this->AddPage(rfb_general, _("General"));
		this->AddPage(rfb_display, _("Display"));
      }
   }
   return error;
}

RFBOptionsDialog_General::RFBOptionsDialog_General(RFBOptionsDialog *parent_element)
{
   parent = NULL;

   m_flexgrid_sizer = NULL;
   m_boxsizer = NULL;

   m_static_server = NULL;
   m_text_server = NULL;

   m_static_connectionname = NULL;
   m_text_connectionname = NULL;

   m_static_port = NULL;
   m_text_port = NULL;

   groupcount = 0;
   //get_group_list();
   //options.empty();
   error = 0;
   if (parent_element != NULL)
   {
   	  parent = parent_element;

   }
   else
   {
   	  error = RFB_ERROR_PARENT_IS_NULL;
   }
}

RFBOptionsDialog_General::~RFBOptionsDialog_General()
{
   if(m_static_server != NULL) {delete m_static_server; m_static_server = NULL;}
   if(m_text_server != NULL) {delete m_text_server; m_text_server = NULL;}

   if (m_static_groupname) {delete m_static_groupname; m_static_groupname = NULL;}
   if (m_combobox_groupname) {delete m_combobox_groupname; m_combobox_groupname = NULL;}
   if (m_static_connectionname) {delete m_static_connectionname; m_static_connectionname = NULL;}
   if (m_text_connectionname) {delete m_text_connectionname; m_text_connectionname = NULL;}
   if (m_static_port != NULL) {delete m_static_port; m_static_port = NULL;}
   if (m_text_port != NULL) {delete m_text_port; m_text_port = NULL;}
}

int RFBOptionsDialog_General::Build()
{
   if ( error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
      if ( state == false)
      {
		error = RFB_ERROR_CANT_CREATE_PANEL;
      }
      else
      {
	 m_boxsizer = new wxBoxSizer(wxVERTICAL);
	 m_flexgrid_sizer = new wxFlexGridSizer(0, 2, 0, 0);
	 m_flexgrid_sizer->AddGrowableCol(1);

	 m_static_connectionname = new wxStaticText(this, wxID_ANY, _("Connection name"),
						    wxDefaultPosition,wxDefaultSize);
	 m_text_connectionname = new wxTextCtrl( this, wxID_ANY, wxT(""),
						 wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	 m_static_groupname = new wxStaticText(this, wxID_ANY, _("Group name"),
					       wxDefaultPosition, wxDefaultSize);
	 //std::cout << array_string_group_list.Count() << std::endl;
	 for (size_t i = 0; i < array_string_group_list.Count(); i++)
	 {
	    if (array_string_group_list.Item(i) == wxT("Main"))
	    {
	       array_string_group_list.RemoveAt(i);
	    }
	 }
	 array_string_group_list.Insert(wxT("Main"), 0);

	    m_combobox_groupname = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,
						  wxDefaultSize,array_string_group_list);
	 m_flexgrid_sizer->Add(m_static_connectionname, 1,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer->Add(m_text_connectionname, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer->Add(m_static_groupname, 1,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer->Add(m_combobox_groupname, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);


	 m_static_server = new wxStaticText(this, wxID_ANY, _("Server"));
	 m_text_server = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition,
					 wxDefaultSize, wxTE_PROCESS_ENTER);

	 m_checkbox_customport = new wxCheckBox( this, ID_RFB_CHECKBOX_CUSTOMPORT,
						 _("&Use custom port"), wxDefaultPosition,
						 wxDefaultSize );
	  m_checkbox_shared = new wxCheckBox( this, wxID_ANY,
						 _("&Shared"), wxDefaultPosition,
						 wxDefaultSize );
	    m_checkbox_viewonly = new wxCheckBox( this, wxID_ANY,
						 _("&View only"), wxDefaultPosition,
						 wxDefaultSize );

	 m_flexgrid_sizer_2 = new wxFlexGridSizer(0, 3, 0, 0);
	 m_flexgrid_sizer_2->AddGrowableCol(1);
	 m_flexgrid_sizer_2->Add(m_static_server, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_text_server, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	  m_flexgrid_sizer_2->Add(m_checkbox_customport, 0,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	 m_static_port = new wxStaticText(this, wxID_ANY, _("Port"));
	 m_text_port = new wxTextCtrl( this, wxID_ANY, wxT("5900"), wxDefaultPosition,
				       wxDefaultSize, wxTE_PROCESS_ENTER,
				       wxTextValidator(wxFILTER_NUMERIC) );

	 m_boxsizer_2 = new wxBoxSizer(wxHORIZONTAL);

	 m_static_password = new wxStaticText(this, wxID_ANY, _("Password"));
	 m_text_password = new wxTextCtrl( this, ID_RFB_TEXTCTRL_PASSWORD, wxT(""),wxDefaultPosition,
					   wxDefaultSize, wxTE_PASSWORD);

	 m_flexgrid_sizer_2->Add(m_static_password, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_text_password, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	 m_static_proxy = new wxStaticText(this, wxID_ANY, _("Proxy/Repeater"));
	 m_text_proxy = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition,
					 wxDefaultSize, wxTE_PROCESS_ENTER);

	  m_boxsizer_3 = new wxBoxSizer(wxHORIZONTAL);

	 m_static_proxy_port = new wxStaticText(this, wxID_ANY, _("Proxy port"));
	 m_text_proxy_port = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition,
					 wxDefaultSize, wxTE_PROCESS_ENTER,
				       wxTextValidator(wxFILTER_NUMERIC) );
	 m_boxsizer_3->Add(m_static_proxy_port, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(m_text_proxy_port, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	 m_boxsizer_2->Add(m_static_port, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_2->Add(m_text_port, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	 m_flexgrid_sizer_2->Add(m_boxsizer_2, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);


	 m_flexgrid_sizer_2->Add(m_static_proxy, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_text_proxy, 0,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	  m_flexgrid_sizer_2->Add(m_boxsizer_3, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	   m_flexgrid_sizer_2->Add(m_checkbox_shared, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	      m_flexgrid_sizer_2->Add(m_checkbox_viewonly, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	 m_text_port->Enable(false);
	 m_static_port->Enable(false);

	 m_combobox_groupname->SetSelection(0);
	 m_combobox_groupname->SetValue(wxT("Main"));

	 m_boxsizer->Add(m_flexgrid_sizer, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer->Add(m_flexgrid_sizer_2, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	 this->SetSizer(m_boxsizer);
	 m_boxsizer->SetSizeHints(this);

      }
   }

   return error;
}

void RFBOptionsDialog_General::get_group_list()
{

}

Options_HashMap RFBOptionsDialog_General::GetOptions()
{
   Options_HashMap local_options;
   RFB_ERRORS error = RFB_ERROR_OK;

   local_options.clear();

   if ((m_text_server != NULL) &&
       (m_text_server->GetValue().IsEmpty() == false))
   {
      local_options[wxT("hostname")] = m_text_server->GetValue();
   }
   else
   {
      error = RFB_ERROR_EMPTY_HOSTNAME;
   }

   if(m_text_password != NULL)
   {
      local_options[wxT("password")] = m_text_password->GetValue();
   }

   if (m_combobox_groupname != NULL) {
      local_options[wxT("group_name")] = m_combobox_groupname->GetValue();
   }
   if (m_text_connectionname != NULL)
   {
      local_options[wxT("connection_name")] = m_text_connectionname->GetValue();
   }

   if(m_checkbox_customport != NULL)
   {
      local_options[wxT("custom_port")] = wxString::Format(wxT("%i"),
							  m_checkbox_customport->GetValue());
   }
   if(m_text_port != NULL)
   {
      local_options[wxT("port")] = m_text_port->GetValue();
   }

   if(m_text_proxy != NULL)
   {
	   local_options[wxT("proxy_host")] = m_text_proxy->GetValue();
   }

   if(m_text_proxy_port != NULL)
   {
	  local_options[wxT("proxy_port")] = m_text_proxy_port->GetValue();
   }

   if(m_checkbox_shared != NULL)
   {
      local_options[wxT("shared")] = wxString::Format(wxT("%i"),
							  m_checkbox_shared->GetValue());
   }
   if(m_checkbox_viewonly != NULL)
   {
      local_options[wxT("viewonly")] = wxString::Format(wxT("%i"),
							  m_checkbox_viewonly->GetValue());
   }

   if ( error != RFB_ERROR_OK)
   {
      local_options.clear();
   }
   return local_options;
}

bool RFBOptionsDialog_General::SetOptions(const Options_HashMap *all_options)
{
	bool state = false;
   Options_HashMap local_options;
   local_options = *all_options;

   if ( m_text_server != NULL)
   {
      m_text_server->SetValue(local_options[wxT("hostname")]);
   }

   if ((m_text_password != NULL))
   {
      m_text_password->SetValue(local_options[wxT("password")]);
   }

   if ((m_combobox_groupname != NULL))
   {
	   for (int i = 0; i < (int)array_string_group_list.Count(); i++)
      {
	 if (array_string_group_list.Item(i) == local_options[wxT("group_name")])
	 {
	    array_string_group_list.RemoveAt(i);
	 }
      }
      m_combobox_groupname->SetValue(local_options[wxT("group_name")]);
   }
   if ((m_text_connectionname != NULL))
   {
      m_text_connectionname->SetValue(local_options[wxT("connection_name")]);
   }
   if ((m_checkbox_customport != NULL))
   {
      m_checkbox_customport->SetValue(wxAtoi(local_options[wxT("custom_port")]));
	  if(wxAtoi(local_options[wxT("custom_port")]))
	  {
		m_static_port->Enable(true);
		m_text_port->Enable(true);
	  }
   }

	if ((m_checkbox_shared != NULL))
   {
      m_checkbox_shared->SetValue(wxAtoi(local_options[wxT("shared")]));
   }

	if ((m_checkbox_viewonly!= NULL))
   {
      m_checkbox_viewonly->SetValue(wxAtoi(local_options[wxT("viewonly")]));
   }


   if ((m_text_port != NULL))
   {
      m_text_port->SetValue(local_options[wxT("port")]);
   }

   if((m_text_proxy != NULL))
   {
	m_text_proxy->SetValue(local_options[wxT("proxy_host")]);
   }

   if((m_text_proxy_port != NULL))
   {
	m_text_proxy_port->SetValue(local_options[wxT("proxy_port")]);
   }

   return state;
}

void RFBOptionsDialog_General::SetGroupList(const wxArrayString &groupList) {
	for (size_t i = 0; i < groupList.Count(); i++)
		array_string_group_list.Add(groupList.Item(i));
}

void RFBOptionsDialog_General::checkbox_customport_func(wxCommandEvent &event)
{
   checkbox_customport();
}

void RFBOptionsDialog_General::checkbox_customport() {
	m_text_port->Enable(this->m_checkbox_customport->IsChecked());
	m_static_port->Enable(this->m_checkbox_customport->IsChecked());
}

int RFBOptionsDialog_Display::Build()
{
	if (error == 0 )
	{
		bool state = false;
		state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
		if ( state == false)
		{
			error = RFB_ERROR_CANT_CREATE_PANEL;
		}
		else
		{
			wxArrayString resolutions = GetResolutionsList();

			m_boxsizer_1 = new wxBoxSizer(wxVERTICAL);
			m_boxsizer_2 = new wxBoxSizer(wxHORIZONTAL);
			m_boxsizer_3 = new wxBoxSizer(wxVERTICAL);
			m_boxsizer_4 = new wxBoxSizer(wxHORIZONTAL);
			m_boxsizer_5 = new wxBoxSizer(wxHORIZONTAL);
			m_flexgrid_sizer_1 = new wxFlexGridSizer(0, 3, 0, 0);
			m_flexgrid_sizer_1->AddGrowableCol(1);

			//m_radiobox_1 = new wxRadioBox(this, wxID_ANY, wxT("Screen resolution"),
			//			       wxDefaultPosition, wxDefaultSize, 0, 0, 1, 0, wxDefaultValidator);
		//	m_radiobutton_controlsize = new wxRadioButton(this, ID_RFB_CHK_CONTROLSIZE,
		//		wxT("&Control size"),
		//		wxDefaultPosition, wxDefaultSize, 0,
		//		wxDefaultValidator);
		//	m_radiobutton_controlsize->SetValue(true);
		//	m_radiobutton_fullscreen = new wxRadioButton(this, ID_RFB_CHK_CONTROLSIZE,
		//		wxT("&Full screen"),
		//		wxDefaultPosition, wxDefaultSize, 0,
		//		wxDefaultValidator);
			m_radiobutton_customgeometry = new wxRadioButton(this, ID_RFB_CHK_CONTROLSIZE,
				_("&Custom geometry"),
				wxDefaultPosition, wxDefaultSize, 0,
				wxDefaultValidator);
			m_radiobutton_customgeometry->SetValue(true);
			m_radiobutton_presetsize = new wxRadioButton(this, ID_RFB_CHK_CONTROLSIZE,
				_("&Preset geometry"),
				wxDefaultPosition, wxDefaultSize, 0,
				wxDefaultValidator);

			//m_boxsizer_3->Add(m_radiobox_1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
		//	m_boxsizer_3->Add(m_radiobutton_controlsize, 1,
		//		wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
		//	m_boxsizer_3->Add(m_radiobutton_fullscreen, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_3->Add(m_radiobutton_customgeometry, 1,
				wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_3->Add(m_radiobutton_presetsize, 1,
				wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

			//	 m_boxsizer_2->Add(m_boxsizer_3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

			m_static_resolution = new wxStaticText(this, wxID_ANY, _("Screen resolution"),
				wxDefaultPosition, wxDefaultSize);
			m_combobox_resolution = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,
				wxDefaultSize,resolutions,wxCB_READONLY);
			int sel = GetDefaultResolution();
			if (sel >=0)
			{
				m_combobox_resolution->SetValue(resolutions.Item(sel));
				m_combobox_resolution->SetSelection(sel);
			}

			//		 m_checkbox_customgeometry = new wxCheckBox( this, , wxT(""),
			//													 wxDefaultPosition, wxDefaultSize );
			m_static_width = new wxStaticText(this, wxID_ANY, _("Screen width"));
			m_text_width = new wxTextCtrl( this, wxID_ANY, wxT("800"), wxDefaultPosition, wxDefaultSize,
				wxTE_PROCESS_ENTER , wxTextValidator(wxFILTER_NUMERIC) );
			m_static_height = new wxStaticText(this, wxID_ANY, _("Screen height"));
			m_text_height = new wxTextCtrl( this, wxID_ANY, wxT("600"), wxDefaultPosition, wxDefaultSize,
				wxTE_PROCESS_ENTER , wxTextValidator(wxFILTER_NUMERIC) );

			m_flexgrid_sizer_1->Add(m_static_width, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_flexgrid_sizer_1->Add(m_text_width, 0,
				wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_flexgrid_sizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

			m_flexgrid_sizer_1->Add(m_static_height, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_flexgrid_sizer_1->Add(m_text_height, 0,
				wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_flexgrid_sizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);


			m_flexgrid_sizer_1->Add(m_static_resolution, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_flexgrid_sizer_1->Add(m_combobox_resolution, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_flexgrid_sizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

			m_flexgrid_sizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_2->Add(m_boxsizer_3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_2->Add(m_flexgrid_sizer_1, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);

			//m_boxsizer_4->Add(m_static_colordepth, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			//m_boxsizer_4->Add(m_choise_colordepth, 1,
			//			 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

			m_checkbox_updatescreen = new wxCheckBox(this, wxID_ANY,
                                                  _("&Force update screen"));
			//		 m_checkbox_fullscreen = new wxCheckBox( this, , wxT(""),
			//												 wxDefaultPosition, wxDefaultSize );
			m_checkbox_smartsizing = new wxCheckBox( this, -1, _("&Smart sizing"));
			//		 m_checkbox_controlsize = new wxCheckBox( this, , wxT(""),
			//												  wxDefaultPosition,
			//												  wxDefaultSize);

			m_boxsizer_5->Add(m_checkbox_updatescreen, 1,
				wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_5->Add(m_checkbox_smartsizing, 1,
				wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_1->Add(m_boxsizer_2, 0,
				wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			//	 m_boxsizer_1->Add(m_boxsizer_4, 0,
			//			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_1->Add(m_boxsizer_5, 0,
				wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

			m_checkbox_updatescreen->Show(false);
			this->SetSizer(m_boxsizer_1);
			m_boxsizer_1->Fit(this);
			m_boxsizer_1->SetSizeHints(this);
			CheckRFBDisplay();
		}
	}
	return error;
}

RFBOptionsDialog_Display::RFBOptionsDialog_Display(RFBOptionsDialog *parent_element)
{
   m_boxsizer_1 = NULL;
   m_boxsizer_2 = NULL;
   m_boxsizer_3 = NULL;
   m_boxsizer_4 = NULL;
   m_boxsizer_5 = NULL;
   m_flexgrid_sizer_1 = NULL;
   m_radiobox_1 = NULL;

   m_static_width = NULL;
   m_text_width = NULL;

   m_static_height = NULL;
   m_text_height = NULL;

   m_static_colordepth = NULL;
   m_choise_colordepth = NULL;

   m_radiobutton_controlsize = NULL;
   m_radiobutton_fullscreen = NULL;
   m_radiobutton_customgeometry = NULL;
   m_radiobutton_presetsize = NULL;

   m_checkbox_fullscreen = NULL;
   m_checkbox_smartsizing = NULL;
   m_checkbox_controlsize = NULL;

   m_checkbox_updatescreen = NULL;

   m_static_resolution = NULL;
   m_combobox_resolution = NULL;
   m_checkbox_customgeometry = NULL;

   error = 0;
   if (parent_element != NULL)
   {
	  parent = parent_element;
   }
   else
   {
	  error = RFB_ERROR_PARENT_IS_NULL;
   }

}

wxArrayString RFBOptionsDialog_Display::GetResolutionsList()
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
   resolutions.Add(wxT("800 x 600"));
   resolutions.Add(wxT("1024 x 768"));
   resolutions.Add(wxT("1152 x 864"));
   resolutions.Add(wxT("1280 x 960"));
   resolutions.Add(wxT("1280 x 1024"));
	resolutions.Sort();
	resolutions.Shrink();
#endif
   return resolutions;
}

wxSize RFBOptionsDialog_Display::GetResolution(int i)
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

int RFBOptionsDialog_Display::GetDefaultResolution()
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

void RFBOptionsDialog_Display::CheckRFBDisplay()
{
   if (m_radiobutton_customgeometry->GetValue() == true)
   {
      m_text_width->Enable(true);
      m_text_height->Enable(true);
      m_combobox_resolution->Enable(false);
   }
   else if (m_radiobutton_presetsize->GetValue() == true)
   {
      m_text_width->Enable(false);
      m_text_height->Enable(false);
      m_combobox_resolution->Enable(true);
   }

}

void RFBOptionsDialog_Display::checkbox_customgeometry_func(wxCommandEvent& event)
{
   CheckRFBDisplay();
}

void RFBOptionsDialog_Display::checkbox_fullscreen_func(wxCommandEvent &event)
{
   CheckRFBDisplay();
}

void RFBOptionsDialog_Display::checkbox_controlsize_func(wxCommandEvent &event)
{
   CheckRFBDisplay();
}

RFBOptionsDialog_Display::~RFBOptionsDialog_Display()
{
   if(m_static_width != NULL) {delete m_static_width; m_static_width = NULL;}
   if(m_text_width != NULL) {delete m_text_width; m_text_width = NULL;}
   if(m_static_height != NULL) {delete m_static_height; m_static_height = NULL;}
   if(m_text_height != NULL) {delete m_text_height; m_text_height = NULL;}
   if(m_static_colordepth != NULL) {delete m_static_colordepth; m_static_colordepth = NULL;}
   if(m_choise_colordepth != NULL) {delete m_choise_colordepth; m_choise_colordepth = NULL;}
   if(m_checkbox_smartsizing != NULL) {delete m_checkbox_smartsizing; m_checkbox_smartsizing = NULL;}
   if(m_checkbox_updatescreen != NULL) {delete m_checkbox_updatescreen; m_checkbox_updatescreen = NULL;}
   if(m_combobox_resolution != NULL) {delete m_combobox_resolution; m_combobox_resolution = NULL;}

   if(m_radiobutton_controlsize != NULL)
   {delete m_radiobutton_controlsize; m_radiobutton_controlsize = NULL;}
   if(m_radiobutton_fullscreen != NULL)
   {delete m_radiobutton_fullscreen; m_radiobutton_fullscreen = NULL;}
   if(m_radiobutton_customgeometry != NULL)
   {delete m_radiobutton_customgeometry; m_radiobutton_customgeometry = NULL;}
   if(m_radiobutton_presetsize != NULL)
   {delete m_radiobutton_presetsize; m_radiobutton_presetsize = NULL;}
   if(m_radiobox_1 != NULL) {delete m_radiobox_1; m_radiobox_1 = NULL;}

   // if(m_flexgrid_sizer_1 != NULL) {delete m_flexgrid_sizer_1; m_flexgrid_sizer_1 = NULL;}
   // if(m_boxsizer_3 != NULL) {delete m_boxsizer_3; m_boxsizer_3 = NULL;}
   // if(m_boxsizer_4 != NULL) {delete m_boxsizer_4; m_boxsizer_4 = NULL;}
   // if(m_boxsizer_2 != NULL) {delete m_boxsizer_2; m_boxsizer_2 = NULL;}
   // if(m_boxsizer_5 != NULL) {delete m_boxsizer_5; m_boxsizer_5 = NULL;}
   // if(m_boxsizer_1 != NULL) {delete m_boxsizer_1; m_boxsizer_1 = NULL;}


}

bool RFBOptionsDialog_Display::SetOptions(const Options_HashMap *all_options)
{
	bool state = false;
	Options_HashMap local_options;
	local_options = *all_options;

	if ((m_choise_colordepth != NULL))
	{
		if ((wxAtoi(local_options[wxT("color_depth")]) > -1) &&
			(wxAtoi(local_options[wxT("color_depth")]) < 4))
		{
			m_choise_colordepth->SetSelection(wxAtoi(local_options[wxT("color_depth")]));
		}
		else
		{
			m_choise_colordepth->SetValue(wxT("15-bit"));
		}
	}
	if((m_checkbox_updatescreen != NULL))
	{
		m_checkbox_updatescreen->SetValue(wxAtoi(local_options[wxT("force_update_screen")]));
	}
	if((m_radiobutton_controlsize != NULL))
	{
		m_radiobutton_controlsize->SetValue(wxAtoi(local_options[wxT("control_size")]));
	}
	if((m_radiobutton_fullscreen != NULL))
	{
		m_radiobutton_fullscreen->SetValue(wxAtoi(local_options[wxT("full_screen")]));
	}
	if((m_radiobutton_customgeometry != NULL))
	{
		m_radiobutton_customgeometry->SetValue(wxAtoi(local_options[wxT("custom_geometry")]));
	}
	if ((m_text_width != NULL))
	{
		m_text_width->SetValue(local_options[wxT("width")]);
	}
	if ((m_text_height != NULL))
	{
		m_text_height->SetValue(local_options[wxT("heigth")]);
	}
	if ((m_radiobutton_presetsize != NULL))
	{
		m_radiobutton_presetsize->SetValue(wxAtoi(local_options[wxT("preset_screen_size")]));
	}
	if (m_combobox_resolution != NULL)
	{
		m_combobox_resolution->SetSelection(wxAtoi(local_options[wxT("resolution")]));
	}

	if ((m_checkbox_smartsizing != NULL))
	{
		m_checkbox_smartsizing->SetValue(wxAtoi(local_options[wxT("smart_sizing")]));
	}
	CheckRFBDisplay();
	return state;
}

Options_HashMap RFBOptionsDialog_Display::GetOptions()
{
   Options_HashMap local_options;

   local_options.clear();
   if(m_choise_colordepth != NULL)
   {
      local_options[wxT("color_depth")] = wxString::Format(wxT("%i"),
							   m_choise_colordepth->GetCurrentSelection());
   }
   if(m_checkbox_updatescreen != NULL)
   {
      local_options[wxT("force_update_screen")] = wxString::Format(wxT("%i"),
								   m_checkbox_updatescreen->GetValue());
   }
   if(m_radiobutton_controlsize != NULL)
   {
      local_options[wxT("control_size")] = wxString::Format(wxT("%i"),
							    m_radiobutton_controlsize->GetValue());
   }
   if(m_radiobutton_fullscreen != NULL)
   {
      local_options[wxT("full_screen")] = wxString::Format(wxT("%i"),
							   m_radiobutton_fullscreen->GetValue());
   }
   if(m_radiobutton_customgeometry != NULL)
   {
      local_options[wxT("custom_geometry")] = wxString::Format(wxT("%i"),
							       m_radiobutton_customgeometry->GetValue());
      if(m_text_width != NULL)
      {
	 local_options[wxT("width")] = m_text_width->GetValue();
      }
      if(m_text_height != NULL)
      {
	 local_options[wxT("heigth")] = m_text_height->GetValue();
      }
   }
   if(m_radiobutton_presetsize != NULL)
   {
      local_options[wxT("preset_screen_size")] = wxString::Format(wxT("%i"),
								  m_radiobutton_presetsize->GetValue());

      if (m_combobox_resolution != NULL)
      {
	 wxSize resolution;
	 resolution = GetResolution(m_combobox_resolution->GetCurrentSelection());
	 //wxArrayString resolution = GetResolutionsList();
      	 local_options[wxT("width")] = wxString::Format(wxT("%i"), resolution.x);
      	 local_options[wxT("heigth")] = wxString::Format(wxT("%i"), resolution.y);
      }

   }
   if (m_combobox_resolution != NULL)
   {
      local_options[wxT("resolution")] = wxString::Format(wxT("%i"),
							       m_combobox_resolution->GetCurrentSelection());
   }

   if(m_checkbox_smartsizing != NULL)
   {
      local_options[wxT("smart_sizing")] = wxString::Format(wxT("%i"),
							    m_checkbox_smartsizing->GetValue());
   }

   return local_options;
}
