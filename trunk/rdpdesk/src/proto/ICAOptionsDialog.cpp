///////////////////////////////////////////////////////////////////////////////
// File name:   ICAOptionsDialog.cpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-17 16:04:22" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#include "ICAOptionsDialog.hpp"
#include <wx/display.h>
#include <wx/dir.h>
BEGIN_EVENT_TABLE(ICAOptionsDialog_General, wxPanel)
EVT_CHECKBOX (ID_ICA_CHECKBOX_CUSTOMPORT, ICAOptionsDialog_General::checkbox_customport_func)
EVT_TEXT(ID_ICA_TEXTCTRL_PASSWORD, ICAOptionsDialog_General::text_passwordentry_func)
EVT_CHECKBOX (ID_ICA_CHECKBOX_AUTOLOGON, ICAOptionsDialog_General::text_passwordentry_func)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ICAOptionsDialog_Display, wxPanel)
EVT_RADIOBUTTON (ID_ICA_CHK_CONTROLSIZE, ICAOptionsDialog_Display::checkbox_controlsize_func)
EVT_COMBOBOX (ID_ICA_DESIRED_COLOR, ICAOptionsDialog_Display::change_desired_color_func)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(ICAOptionsDialog_Advanced, wxPanel)
EVT_CHECKBOX(ID_ICA_CHECKBOX_USE_PROGRAM, ICAOptionsDialog_Advanced::checkbox_program_func)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ICAOptionsDialog_Security, wxPanel)
END_EVENT_TABLE()


//WX_DEFINE_OBJARRAY(GeneralOptionsArray);
///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
ICAOptionsDialog::ICAOptionsDialog(wxWindow *parent_element)
{
   parent = NULL;
   ica_general = NULL;
   ica_display = NULL;
   //ica_share = NULL;
   ica_advanced = NULL;
   ica_sound = NULL;
   ica_security = NULL;
   
   save_options.clear();
   
   error = 0;
   if (parent_element != NULL)
   {
   	  parent = parent_element;
   } 
   else 
   {
   	  error = ICA_ERROR_PARENT_IS_NULL; 
   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
ICAOptionsDialog::~ICAOptionsDialog()
{
//   size_t end;
//
//   if(this != NULL)
//   {
//      end = this->GetPageCount();
      this->DeleteAllPages();
      //delete notebook;
      //notebook = NULL;
//   }
	
   // if (rdp_general != NULL) {delete rdp_general; rdp_general = NULL;}
   // if (rdp_display != NULL) {delete rdp_display; rdp_display = NULL;}
   // if (rdp_share != NULL) {delete rdp_share; rdp_share = NULL;}
   // if (rdp_program != NULL) {delete rdp_program; rdp_program = NULL;}
   // if (rdp_perfomance != NULL) {delete rdp_perfomance; rdp_perfomance = NULL;}
   // if (rdp_advanced != NULL) {delete rdp_advanced; rdp_advanced = NULL;}
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param
//! \return
//! \sa 
///////////////////////////////////////////////////////////////////////////////
int ICAOptionsDialog::Build()
{
   if ( error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize);
      if ( state == false) 
      {
	 error = ICA_ERROR_CANT_CREATE_NOTEBOOK;
      }
      else 
      {
	 ica_general = new ICAOptionsDialog_General (this);
	 ica_general->Set_Group_List(array_string_group_list);
	 ica_general->Build();
	 ica_display = new ICAOptionsDialog_Display (this);
	 ica_display->Build();
	 ica_sound = new ICAOptionsDialog_Sound (this);
	 ica_sound->Build();
	 ica_advanced = new ICAOptionsDialog_Advanced (this);
	 ica_advanced->Build();
	 ica_security = new ICAOptionsDialog_Security (this);
	 ica_security->Build();

	 // ica_share = new ICAOptionsDialog_Share (this);
	 // ica_share->Build();
	 // ica_program = new ICAOptionsDialog_Program (this);
	 // ica_program->Build();
	 // ica_perfomance = new ICAOptionsDialog_Performance (this);
	 // ica_perfomance->Build();

	 this->AddPage(ica_general, wxT("General"));
	 this->AddPage(ica_display, wxT("Display"));
	 this->AddPage(ica_sound, wxT("Share"));

// this->AddPage(ica_share, wxT("Share"));
	 // this->AddPage(ica_program, wxT("Program"));
	 // this->AddPage(ica_perfomance, wxT("Perfomance"));
	 this->AddPage(ica_advanced, wxT("Advance"));
	 this->AddPage(ica_security, wxT("Security"));

	 //this->Show();
      }
   } 
   return error;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
bool ICAOptionsDialog::Set_Options (const Options_HashMap *options)
{
   bool state = false;
   if (options != NULL)
   {
      
      save_options = *options;
      //wxMessageBox(save_options[wxT("uniq_name")]);
      if (ica_general != NULL)
      {
	 ica_general->Set_Options(options);
      }
      if (ica_display != NULL)
      {
	 ica_display->Set_Options(options);
      }
      // if (ica_share != NULL)
      // {
      // 	 ica_share->Set_Options(options);
      // }
      if (ica_sound != NULL)
      {
	 ica_sound->Set_Options(options);
      }
      if (ica_security != NULL)
      {
	 ica_security->Set_Options(options);
      }
      if (ica_advanced != NULL)
      {
	 ica_advanced->Set_Options(options);
      }
   }
   return state;
}

void ICAOptionsDialog::Set_Group_List(wxArrayString group_list)
{
   array_string_group_list = group_list;
   
//   if (rdp_general != NULL)
//   {
//      rdp_general->Set_Group_List(group_list);
//   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap ICAOptionsDialog::Get_Options ()
{
   //
   Options_HashMap options, temp_options;
   Options_HashMap::iterator it;
   int max_count = 0, min_count = 0;
//   bool option_found;

   if (ica_general != NULL)
   {
      options.clear();
      options = ica_general->Get_Options();
   }
   if ((ica_display != NULL) &&
       (options.size() > 0))
   {
      temp_options.clear();
      temp_options = ica_display->Get_Options();
      for( it = temp_options.begin(); it != temp_options.end(); ++it )
      {
	 wxString key = it->first, value = it->second;
	 options[key] = value;
      }
   }
   // if ((ica_share != NULL) &&
   //     (options.size() > 0))
   // {
   //    temp_options.clear();
   //    temp_options = ica_share->Get_Options();
   //    for( it = temp_options.begin(); it != temp_options.end(); ++it )
   //    {
   // 	 wxString key = it->first, value = it->second;
   // 	 options[key] = value;
   //    }
   // }
   if ((ica_sound != NULL) &&
       (options.size() > 0))
   {
      temp_options.clear();
      temp_options = ica_sound->Get_Options();
      for( it = temp_options.begin(); it != temp_options.end(); ++it )
      {
   	 wxString key = it->first, value = it->second;
   	 options[key] = value;
      }
   }

   if ((ica_advanced != NULL) &&
       (options.size() > 0))
   {
      temp_options.clear();
      temp_options = ica_advanced->Get_Options();
      for( it = temp_options.begin(); it != temp_options.end(); ++it )
      {
   	 wxString key = it->first, value = it->second;
   	 options[key] = value;
      }
   }
   if ((ica_security != NULL) &&
       (options.size() > 0))
   {
      temp_options.clear();
      temp_options = ica_security->Get_Options();
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
   //wxMessageBox((save_options)[wxT("uniq_name")]);
   // if (options.size() > save_options.size())
   // {
   //    max_count = options.size();
   //    min_count = save_options.size();
   // }
   // else
   // {
   //    max_count = save_options.size();
   //    min_count = options.size();
   // }
   // if (options.count() > 0)
   // {
   //    for (int i = 0 ; i < max_count; i++)
   //    {
   // 	 option_found = false;
   // 	 for (int f = 0; f < max_count; f++)
   // 	 {
   // 	    if ((options.count() > f) && (save_options.count() > i ))
   // 	    {
   // 	       if (options.Item(f).Item(0).name == save_options.Item(i).Item(0).name)
   // 	       {
   // 		  option_found = true;
   // 		  break;
   // 	       }
   // 	    }
   // 	    else if ((options.Count() == f) && (save_options.Count() > i ))
   // 	    {
   // 	       option_found = false;
   // 	    }
   // 	 }
   // 	 if ((save_options.Count() > i ) &&
   // 	     (option_found == false))
   // 	 {
   // 	    options_struct temp_struct;
   // 	    GeneralOptionsArray temp_array;
   // 	    temp_struct.type = wxT("s");
   // 	    temp_struct.name = save_options.Item(i).Item(0).name;
   // 	    temp_struct.value= save_options.Item(i).Item(0).value;
   // 	    temp_array.Add (temp_struct);
   // 	    options.Add(temp_array);
   // 	    temp_struct.type.Clear();
   // 	    temp_struct.name.Clear();
   // 	    temp_struct.value.Clear();
   // 	    option_found = false;
   // 	 }
   //    }
   //wxMessageBox(options.Item(0).Item(0).name);
//   std::cout << options.size() << std::endl;
//   wxMessageBox( options[wxT("hostname")]);

   return save_options;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
ICAOptionsDialog_General::ICAOptionsDialog_General(ICAOptionsDialog *parent_element)
{
   parent = NULL;

   m_flexgrid_sizer_1 = NULL;
   m_flexgrid_sizer_2 = NULL;
   m_flexgrid_sizer_2 = NULL;

   m_boxsizer_1 = NULL;
   m_boxsizer_2 = NULL;

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
   //m_checkbox_attachtoconsole = NULL;

   m_static_groupname = NULL;
   m_combobox_groupname = NULL;

   m_static_connectionname = NULL;
   m_text_connectionname = NULL;

   m_static_proxy_autoconfig_url = NULL;
   m_text_proxy_autoconfig_url = NULL;

   m_static_proxy_username = NULL;
   m_text_proxy_username = NULL;

   m_static_proxy_password = NULL;
   m_text_proxy_password = NULL;

   m_static_proxy_host = NULL;
   m_text_proxy_host = NULL;

   m_static_proxy_type = NULL;
   m_combobox_proxy_type = NULL;

   m_static_proxy_bypass_list = NULL;
   m_text_proxy_bypass_list = NULL;

   m_static_proxy_timeout = NULL;
   m_spinctrl_proxy_timeout = NULL;

   m_static_ssl_proxy_host = NULL;
   m_text_ssl_proxy_host = NULL;

   m_checkbox_ssl_enable = NULL;
   m_checkbox_proxy_use_default = NULL;

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
   	  error = ICA_ERROR_PARENT_IS_NULL; 
   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
ICAOptionsDialog_General::~ICAOptionsDialog_General()
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
   //if(m_checkbox_attachtoconsole != NULL)
   //{delete m_checkbox_attachtoconsole; m_checkbox_attachtoconsole = NULL;}
   if (m_static_groupname) {delete m_static_groupname; m_static_groupname = NULL;}
   if (m_combobox_groupname) {delete m_combobox_groupname; m_combobox_groupname = NULL;}
   if (m_static_connectionname) {delete m_static_connectionname; m_static_connectionname = NULL;}
   if (m_text_connectionname) {delete m_text_connectionname; m_text_connectionname = NULL;}
   if (m_static_proxy_autoconfig_url != NULL)
   {delete m_static_proxy_autoconfig_url; m_static_proxy_autoconfig_url = NULL;}
   if (m_text_proxy_autoconfig_url != NULL)
   {delete m_text_proxy_autoconfig_url; m_text_proxy_autoconfig_url = NULL;}
   if (m_static_proxy_username != NULL)
   {delete m_static_proxy_username; m_static_proxy_username = NULL;}
   if (m_text_proxy_username != NULL){delete m_text_proxy_username; m_text_proxy_username = NULL;}
   if (m_static_proxy_password != NULL){delete m_static_proxy_password; m_static_proxy_password = NULL;}
   if (m_text_proxy_password != NULL){delete m_text_proxy_password; m_text_proxy_password = NULL;}
   if (m_static_proxy_host != NULL){delete m_static_proxy_host; m_static_proxy_host = NULL;}
   if (m_text_proxy_host != NULL){delete m_text_proxy_host; m_text_proxy_host = NULL;}
   if (m_static_proxy_type != NULL){delete m_static_proxy_type; m_static_proxy_type = NULL;}
   if (m_combobox_proxy_type != NULL){delete m_combobox_proxy_type; m_combobox_proxy_type = NULL;}
   if (m_static_proxy_bypass_list != NULL)
   {delete m_static_proxy_bypass_list; m_static_proxy_bypass_list = NULL;}
   if (m_text_proxy_bypass_list != NULL)
   {delete m_text_proxy_bypass_list; m_text_proxy_bypass_list = NULL;}
   if (m_static_proxy_timeout != NULL){delete m_static_proxy_timeout; m_static_proxy_timeout = NULL;}
   if (m_spinctrl_proxy_timeout != NULL)
   {delete m_spinctrl_proxy_timeout; m_spinctrl_proxy_timeout = NULL;}
   if (m_static_ssl_proxy_host != NULL){delete m_static_ssl_proxy_host; m_static_ssl_proxy_host = NULL;}
   if (m_text_ssl_proxy_host != NULL){delete m_text_ssl_proxy_host; m_text_ssl_proxy_host = NULL;}
   if (m_checkbox_ssl_enable != NULL){delete m_checkbox_ssl_enable; m_checkbox_ssl_enable = NULL;}
   if (m_checkbox_proxy_use_default != NULL)
   {delete m_checkbox_proxy_use_default; m_checkbox_proxy_use_default = NULL;}

   //if (m_flexgrid_sizer_1 != NULL){delete m_flexgrid_sizer_1; m_flexgrid_sizer_1 = NULL;}
   //if (m_boxsizer_2 != NULL) {delete m_boxsizer_2; m_boxsizer_2 = NULL;}
   // if (m_flexgrid_sizer_2 != NULL){delete m_flexgrid_sizer_2; m_flexgrid_sizer_2 = NULL;}
   // if (m_boxsizer_1 != NULL){delete m_boxsizer_1; m_boxsizer_1 = NULL;}
   
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
int ICAOptionsDialog_General::Build()
{
   if ( error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
      if ( state == false) 
      {
	 error = ICA_ERROR_CANT_CREATE_PANEL;
      } 
      else 
      {
	 m_boxsizer_1 = new wxBoxSizer(wxVERTICAL);
	 m_flexgrid_sizer_1 = new wxFlexGridSizer(0, 2, 0, 0);
	 m_flexgrid_sizer_1->AddGrowableCol(1);

	 m_static_connectionname = new wxStaticText(this, wxID_ANY, wxT("Connection name"),
						    wxDefaultPosition,wxDefaultSize);
	 m_text_connectionname = new wxTextCtrl( this, wxID_ANY, wxT(""),
						 wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	 m_static_groupname = new wxStaticText(this, wxID_ANY, wxT("Group name"),
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
//	 if (array_string_group_list.Count() > 0)
//	 { 
	 m_combobox_groupname = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,
					       wxDefaultSize,array_string_group_list);
//	 }
//	 else 
//	 {
//	    m_combobox_groupname = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,
//						  wxDefaultSize,array_string_group_list);
//	    m_combobox_groupname->SetValue(wxT("Main"));
//	 }
	 m_flexgrid_sizer_1->Add(m_static_connectionname, 1, 
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(m_text_connectionname, 1, 
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(m_static_groupname, 1, 
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(m_combobox_groupname, 1, 
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);


	 m_static_server = new wxStaticText(this, wxID_ANY, wxT("Server"),wxDefaultPosition,
					    wxDefaultSize);
	 m_text_server = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, 
					 wxDefaultSize, wxTE_PROCESS_ENTER);
	 m_checkbox_customport = new wxCheckBox( this, ID_ICA_CHECKBOX_CUSTOMPORT, 
						 wxT("&Use custom port"), wxDefaultPosition,
						 wxDefaultSize );
	 m_flexgrid_sizer_2 = new wxFlexGridSizer(0, 3, 0, 0);
	 m_flexgrid_sizer_2->AddGrowableCol(1);
	 m_flexgrid_sizer_2->Add(m_static_server, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_text_server, 1, 
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_checkbox_customport, 0, 
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);


	 m_static_username = new wxStaticText(this, wxID_ANY, wxT("Username"),wxDefaultPosition,
					      wxDefaultSize);
	 m_text_username = new wxTextCtrl( this, wxID_ANY, wxT("Administrator"), wxDefaultPosition,
					   wxDefaultSize, wxTE_PROCESS_ENTER);

	 m_static_port = new wxStaticText(this, wxID_ANY, _("Port"),wxDefaultPosition,wxDefaultSize);
	 m_text_port = new wxTextCtrl( this, wxID_ANY, wxT("1494"), wxDefaultPosition, 
				       wxDefaultSize, wxTE_PROCESS_ENTER, 
				       wxTextValidator(wxFILTER_NUMERIC) );
	  
	 m_boxsizer_2 = new wxBoxSizer(wxHORIZONTAL);
	 m_flexgrid_sizer_2->Add(m_static_username, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_text_username, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_2->Add(m_static_port, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_2->Add(m_text_port, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_boxsizer_2, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	  
	 m_static_password = new wxStaticText(this, wxID_ANY, wxT("Password"),wxDefaultPosition,
					      wxDefaultSize);
	 m_text_password = new wxTextCtrl( this, ID_ICA_TEXTCTRL_PASSWORD, wxT(""),wxDefaultPosition, 
					   wxDefaultSize, wxTE_PASSWORD);
	 m_checkbox_autologon = new wxCheckBox( this, ID_ICA_CHECKBOX_AUTOLOGON, wxT("&Auto logon"), 
						wxDefaultPosition,wxDefaultSize );

	 m_flexgrid_sizer_2->Add(m_static_password, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_text_password, 1, 
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_checkbox_autologon, 1, 
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	 m_static_domain = new wxStaticText(this, wxID_ANY, wxT("Domain"),wxDefaultPosition,
					    wxDefaultSize);
	 m_text_domain = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, 
					 wxDefaultSize, wxTE_PROCESS_ENTER);
	 //m_checkbox_attachtoconsole = new wxCheckBox( this, wxID_ANY, wxT("&Attach to console"), 
	 //					      wxDefaultPosition,wxDefaultSize);

	 m_flexgrid_sizer_2->Add(m_static_domain, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_text_domain, 1, 
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 // m_flexgrid_sizer_2->Add(m_checkbox_attachtoconsole, 1, 
	 // 			 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);


	 m_text_port->Enable(false);
	 m_static_port->Enable(false);
	 m_combobox_groupname->SetValue(wxT("Main"));
	 //m_combobox_groupname->SetValue(wxT("Main1"));
	 m_combobox_groupname->SetSelection(0);

	 m_boxsizer_3 = new wxBoxSizer(wxHORIZONTAL);
	 m_static_proxy_autoconfig_url = new wxStaticText(this, wxID_ANY, wxT("Proxy AutoConfig URL"),
							  wxDefaultPosition, wxDefaultSize);
	 m_text_proxy_autoconfig_url = new wxTextCtrl(this, wxID_ANY, wxT(""),
						      wxDefaultPosition, wxDefaultSize);
	 m_boxsizer_3->Add(m_static_proxy_autoconfig_url, 0,
			   wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(m_text_proxy_autoconfig_url, 1,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_3 = new wxFlexGridSizer(0, 4, 0, 0);
	 m_flexgrid_sizer_3->AddGrowableCol(1);
	 m_static_proxy_username = new wxStaticText(this, wxID_ANY, wxT("Proxy Username"),
						    wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_3->Add(m_static_proxy_username, 0,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_text_proxy_username = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition,
						wxDefaultSize);
	 m_flexgrid_sizer_3->Add(m_text_proxy_username, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_proxy_password = new wxStaticText(this, wxID_ANY, wxT("Proxy Password"),
						    wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_3->Add(m_static_proxy_password, 0,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_text_proxy_password = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition,
						wxDefaultSize, wxTE_PASSWORD);
	 m_flexgrid_sizer_3->Add(m_text_proxy_password, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_proxy_host = new wxStaticText(this, wxID_ANY, wxT("Proxy Host"),
						wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_3->Add(m_static_proxy_host, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_text_proxy_host = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition,
					    wxDefaultSize);
	 m_flexgrid_sizer_3->Add(m_text_proxy_host, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_proxy_type = new wxStaticText(this, wxID_ANY, wxT("Proxy Type"),
						wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_3->Add(m_static_proxy_type, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 wxString proxy_type_array[] = {wxT("None") ,_T("Auto"), _T("Socks"), _T("SocksV4"),
					_T("SocksV5"), _T("Secure"), _T("Script")};
	 m_combobox_proxy_type = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition,
						wxDefaultSize,6,proxy_type_array,wxCB_READONLY);
	 m_combobox_proxy_type->SetSelection(0);

	 m_flexgrid_sizer_3->Add(m_combobox_proxy_type, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_proxy_bypass_list = new wxStaticText(this, wxID_ANY, wxT("Proxy Bypass List"),
						       wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_3->Add(m_static_proxy_bypass_list, 0,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_text_proxy_bypass_list = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition,
						   wxDefaultSize);
	 m_flexgrid_sizer_3->Add(m_text_proxy_bypass_list, 0,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_proxy_timeout = new wxStaticText(this, wxID_ANY, wxT("Proxy Timeout"),
						   wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_3->Add(m_static_proxy_timeout, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_spinctrl_proxy_timeout = new wxSpinCtrl(this, wxID_ANY, wxT("20"), wxDefaultPosition,
						   wxDefaultSize, 0, 0, 100, 0);
	 m_spinctrl_proxy_timeout->SetValue(wxT("20"));
	 m_flexgrid_sizer_3->Add(m_spinctrl_proxy_timeout, 0,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_ssl_proxy_host = new wxStaticText(this, wxID_ANY, wxT("SSL Proxy Host"),
						    wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_3->Add(m_static_ssl_proxy_host, 1,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_text_ssl_proxy_host = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition,
						wxDefaultSize);
	 m_flexgrid_sizer_3->Add(m_text_ssl_proxy_host, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_checkbox_ssl_enable = new wxCheckBox(this, ID_ICA_SSL_ENABLE, wxT("SSL Enable"),
						wxDefaultPosition, wxDefaultSize);
	 m_checkbox_ssl_enable->SetValue(false);
	 m_flexgrid_sizer_3->Add(m_checkbox_ssl_enable, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_checkbox_proxy_use_default = new wxCheckBox(this, wxID_ANY, wxT("Proxy Use Default"),
						       wxDefaultPosition, wxDefaultSize);
	 m_checkbox_proxy_use_default->SetValue(false);
	 m_flexgrid_sizer_3->Add(m_checkbox_proxy_use_default, 0,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	 m_boxsizer_1->Add(m_flexgrid_sizer_1, 0, 
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_flexgrid_sizer_2, 0, 
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_boxsizer_3, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_flexgrid_sizer_3, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 this->SetSizer(m_boxsizer_1);
	 m_boxsizer_1->SetSizeHints(this);

      }
   } 

   return error;
}


///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
void ICAOptionsDialog_General::get_group_list()
{
   // Benc bc;

   // ICAConn lrdpconn;
   // int lc = bc.Count(rdp_base);
   // array_string_group_list.Alloc(lc);
	
   // wxString currgroupname;

   // for (int i = 0; i < lc; i++)
   // {
   // 	lrdpconn = bc.Get(rdp_base,i);

   // 	if (lrdpconn.group_name)
   // 	{
   // 		if (lrdpconn.group_name.Length() == 0)
   // 		{
   // 			lrdpconn.group_name.assign(wxT("Main")); 
   // 		}
   // 		int arr_size = (int)array_string_group_list.Count();
   // 		bool flag = true;
   // 		for (int i = 0; i < arr_size; i++)
   // 		{
   // 			if (lrdpconn.group_name == array_string_group_list.Item(i)) 
   // 			{
   // 				flag = false;				
   // 				break;
   // 			}
   // 		}

   // 		if (flag)
   // 		{
   // 			array_string_group_list.Add(lrdpconn.group_name); 
   // 		}
			
   // 	}

   // }
   // array_string_group_list.Sort();
   // array_string_group_list.Shrink();
}


void ICAOptionsDialog_General::checkbox_customport()
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

void ICAOptionsDialog_General::checkbox_customport_func(wxCommandEvent &event)
{
   checkbox_customport();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
void ICAOptionsDialog_General::text_passwordentry_func(wxCommandEvent &event)
{
   wxString strTemp;
   switch(event.GetId())
   {
      case ID_ICA_TEXTCTRL_PASSWORD:
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

      case ID_ICA_CHECKBOX_AUTOLOGON:
	 if (!m_checkbox_autologon->IsChecked()) m_text_password->SetValue(_T(""));
	 break;

      default:
	 break;
   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap ICAOptionsDialog_General::Get_Options()
{
   Options_HashMap local_options;
   ICA_ERRORS error = ICA_ERROR_OK;
 
   local_options.clear();

   if ((m_text_server != NULL) &&
       (m_text_server->GetValue().IsEmpty() == false))
   {
      local_options[wxT("hostname")] = m_text_server->GetValue();
   }
   else
   {
      error = ICA_ERROR_EMPTY_HOSTNAME;
   }
   if(m_text_username != NULL) {
      local_options[wxT("username")] = m_text_username->GetValue();
   }
   if(m_text_password != NULL)
   {
      local_options[wxT("password")] = m_text_password->GetValue();
   }
   if(m_text_domain != NULL)
   {
      local_options[wxT("domain")] = m_text_domain->GetValue();
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
   //if(m_checkbox_attachtoconsole != NULL)
   // {
   //    local_options[wxT("attach_to_console")] = wxString::Format(wxT("%i"),
   // 								m_checkbox_attachtoconsole->GetValue());
   // }
   if (m_combobox_groupname != NULL) {
      local_options[wxT("group_name")] = m_combobox_groupname->GetValue();
   }
   if (m_text_connectionname != NULL)
   {
      local_options[wxT("connection_name")] = m_text_connectionname->GetValue();
   }
   if (m_text_proxy_autoconfig_url != NULL)
   {
      local_options[wxT("proxy_autoconfig_url"]) = m_text_proxy_autoconfig_url->GetValue();
   }
   if (m_text_proxy_username != NULL)
   {
      local_options[wxT("proxy_username"]) = m_text_proxy_username->GetValue();
   }
   if (m_text_proxy_password != NULL)
   {
      local_options[wxT("proxy_password"]) = m_text_proxy_password->GetValue();
   }
   if (m_text_proxy_host != NULL)
   {
      local_options[wxT("proxy_host"]) = m_text_proxy_host->GetValue();
   }
   if (m_combobox_proxy_type != NULL)
   {
      local_options[wxT("proxy_type"]) = wxString::Format(wxT("%i"),
							      m_combobox_proxy_type->GetCurrentSelection());
   }
   if (m_text_proxy_bypass_list != NULL)
   {
      local_options[wxT("proxy_bypass_list"]) = m_text_proxy_bypass_list->GetValue();
   }
   if (m_spinctrl_proxy_timeout != NULL)
   {
      local_options[wxT("proxy_timeout"]) = wxString::Format(wxT("%i"),
									m_spinctrl_proxy_timeout->GetValue());
   }
   if (m_text_ssl_proxy_host != NULL)
   {
      local_options[wxT("ssl_proxy_host"]) = m_text_ssl_proxy_host->GetValue();
   }
   if (m_checkbox_ssl_enable != NULL)
   {
      local_options[wxT("ssl_enable"]) = wxString::Format(wxT("%i"),m_checkbox_ssl_enable->GetValue());
   }
   if (m_checkbox_proxy_use_default != NULL)
   {
      local_options[wxT("proxy_use_default"]) = wxString::Format(wxT("%i"),
								 m_checkbox_proxy_use_default->GetValue());
   }

   if ( error != ICA_ERROR_OK)
   {
      local_options.clear();
   }
   return local_options;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
bool ICAOptionsDialog_General::Set_Options(const Options_HashMap *all_options)
{
   bool state = false;
   Options_HashMap local_options;
   local_options = *all_options;
   
//   size_t count, index = 0;
//   count = all_options->GetCount();
//   for (index = 0; index < count ; index++)
//   {

   if ( m_text_server != NULL)
   {
      m_text_server->SetValue(local_options[wxT("hostname")]);
   }
   if (m_text_username != NULL)
   {
      m_text_username->SetValue(local_options[wxT("username")]);
   }
   if ((m_text_password != NULL))
   {
      m_text_password->SetValue(local_options[wxT("password")]);
   }
   if ((m_text_domain != NULL))
   {
      m_text_domain->SetValue(local_options[wxT("domain")]);
   }
   if ((m_checkbox_customport != NULL))
   {
      m_checkbox_customport->SetValue(local_options[wxT("custom_port")]);
   }
   if ((m_text_port != NULL))
   {
      m_text_port->SetValue(local_options[wxT("port")]);
   }
   // if ((m_checkbox_attachtoconsole != NULL))
   // {
   //    m_checkbox_attachtoconsole->SetValue(local_options[wxT("attach_to_console")]);
   // }
   if ((m_combobox_groupname != NULL))
   {
      m_combobox_groupname->SetValue(local_options[wxT("group_name")]);
   }
   if ((m_text_connectionname != NULL))
   {
      m_text_connectionname->SetValue(local_options[wxT("connection_name")]);
   }
   if (m_text_proxy_autoconfig_url != NULL)
   {
      m_text_proxy_autoconfig_url->SetValue(local_options[wxT("proxy_autoconfig_url"]));
   }
   if (m_text_proxy_username != NULL)
   {
      m_text_proxy_username->SetValue(local_options[wxT("proxy_username"]));
   }
   if (m_text_proxy_password != NULL)
   {
      m_text_proxy_password->SetValue(local_options[wxT("proxy_password"]));
   }
   if (m_text_proxy_host != NULL)
   {
      m_text_proxy_host->SetValue(local_options[wxT("proxy_host"]));
   }
   if (m_combobox_proxy_type != NULL)
   {
      m_combobox_proxy_type->SetSelection(wxAtoi(local_options[wxT("proxy_type"])));
   }
   if (m_text_proxy_bypass_list != NULL)
   {
      m_text_proxy_bypass_list->SetValue(local_options[wxT("proxy_bypass_list"]));
   }
   if (m_spinctrl_proxy_timeout != NULL)
   {
      m_spinctrl_proxy_timeout->SetValue(wxAtoi(local_options[wxT("proxy_timeout"])));
   }
   if (m_text_ssl_proxy_host != NULL)
   {
      m_text_ssl_proxy_host->SetValue(local_options[wxT("ssl_proxy_host"]));
   }
   if (m_checkbox_ssl_enable != NULL)
   {
      m_checkbox_ssl_enable->SetValue(wxAtoi(local_options[wxT("ssl_enable"])));
   }
   if (m_checkbox_proxy_use_default != NULL)
   {
      m_checkbox_proxy_use_default->SetValue(wxAtoi(local_options[wxT("proxy_use_default"])));
   }

   return state;
}

void ICAOptionsDialog_General::Set_Group_List(wxArrayString group_list)
{
//   wxString group[256];
   
//   std::cout << group_list.Count() << std::endl;

//   if ((m_combobox_groupname != NULL))
//   {

   for (size_t i = 0; i < group_list.Count(); i++)
   {
      array_string_group_list.Add(group_list.Item(i));
   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
ICAOptionsDialog_Display::ICAOptionsDialog_Display(ICAOptionsDialog *parent_element)
{
   m_boxsizer_1 = NULL;
   m_boxsizer_2 = NULL;
   m_boxsizer_3 = NULL;
   m_boxsizer_4 = NULL;
   m_boxsizer_5 = NULL;
   m_boxsizer_6 = NULL;
   m_boxsizer_7 = NULL;
   m_boxsizer_8 = NULL;
   m_boxsizer_9 = NULL;
   m_boxsizer_10 = NULL;
   m_boxsizer_11 = NULL;
   m_boxsizer_12 = NULL;

   m_flexgrid_sizer_1 = NULL;
   m_radiobox_1 = NULL;
   
   m_static_width = NULL;
   m_text_width = NULL;

   m_static_height = NULL;
   m_text_height = NULL;

//   m_static_colordepth = NULL;
//   m_choise_colordepth = NULL;

   m_radiobutton_controlsize = NULL;
   m_radiobutton_fullscreen = NULL;
   m_radiobutton_customgeometry = NULL;
   m_radiobutton_presetsize = NULL;
   
   m_checkbox_fullscreen = NULL;
//   m_checkbox_smartsizing = NULL;
   m_checkbox_controlsize = NULL;

//   m_checkbox_updatescreen = NULL;

   m_static_resolution = NULL;
   m_combobox_resolution = NULL;
   m_checkbox_customgeometry = NULL;

   m_checkbox_allow_degraded_colors = NULL;
   m_checkbox_enable_oss = NULL;
   m_checkbox_enable_session_sharing_screen = NULL;
   m_checkbox_approximate_colors = NULL;
   m_checkbox_resize_session = NULL;

   m_static_desired_color = NULL;
   m_combo_desired_color = NULL;

   m_static_use_sdlvb = NULL;
   m_combo_use_sdlvb = NULL;

   m_static_font_smoothing_type = NULL;
   m_combo_font_smoothing_type = NULL;

   m_static_screen_percent = NULL;
   m_combo_screen_percent = NULL;

   
   error = 0;
   if (parent_element != NULL)
   {
	  parent = parent_element;
   } 
   else 
   {
	  error = ICA_ERROR_PARENT_IS_NULL; 
   }

}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
int ICAOptionsDialog_Display::Build()
{
   if (error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
      if ( state == false) 
      {
	 error = ICA_ERROR_CANT_CREATE_PANEL;
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
	 m_radiobutton_controlsize = new wxRadioButton(this, ID_ICA_CHK_CONTROLSIZE,
						       wxT("&Control size"),
						       wxDefaultPosition, wxDefaultSize, 0,
						       wxDefaultValidator);
	 m_radiobutton_controlsize->SetValue(true);
	 m_radiobutton_fullscreen = new wxRadioButton(this, ID_ICA_CHK_CONTROLSIZE,
						      wxT("&Full screen"),
						      wxDefaultPosition, wxDefaultSize, 0,
						      wxDefaultValidator);
	 m_radiobutton_customgeometry = new wxRadioButton(this, ID_ICA_CHK_CONTROLSIZE,
							  wxT("&Custom geometry"),
							  wxDefaultPosition, wxDefaultSize, 0,
							  wxDefaultValidator);
	 m_radiobutton_presetsize = new wxRadioButton(this, ID_ICA_CHK_CONTROLSIZE,
						      wxT("&Preset geometry"),
						      wxDefaultPosition, wxDefaultSize, 0,
						      wxDefaultValidator);

	 //m_boxsizer_3->Add(m_radiobox_1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(m_radiobutton_controlsize, 1,
			   wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(m_radiobutton_fullscreen, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(m_radiobutton_customgeometry, 1,
			   wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(m_radiobutton_presetsize, 1,
			   wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
		 
//	 m_boxsizer_2->Add(m_boxsizer_3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	 m_static_resolution = new wxStaticText(this, wxID_ANY, wxT("Screen resolution"),
						wxDefaultPosition, wxDefaultSize);
	 m_combobox_resolution = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,
						wxDefaultSize,resolutions,wxCB_READONLY);
	 int sel = GetDefaultResolution();
	 if (sel >=0)
	 {
	    m_combobox_resolution->SetValue(resolutions.Item(sel));
	    m_combobox_resolution->SetSelection(sel);
	 }

//		 m_checkbox_customgeometry = new wxCheckBox( this, , _T(""), 
//													 wxDefaultPosition, wxDefaultSize );
	 m_static_width = new wxStaticText(this, wxID_ANY, wxT("Screen width"),wxDefaultPosition, 
					   wxDefaultSize);
	 m_text_width = new wxTextCtrl( this, wxID_ANY, wxT("800"), wxDefaultPosition, wxDefaultSize, 
					wxTE_PROCESS_ENTER , wxTextValidator(wxFILTER_NUMERIC) );
	 m_static_height = new wxStaticText(this, wxID_ANY, wxT("Screen height"),wxDefaultPosition, 
					    wxDefaultSize);
	 m_text_height = new wxTextCtrl( this, wxID_ANY, wxT("600"), wxDefaultPosition, wxDefaultSize, 
					 wxTE_PROCESS_ENTER , wxTextValidator(wxFILTER_NUMERIC) );

	 m_static_desired_color = new wxStaticText(this, wxID_ANY, wxT("Desired Color"),
						   wxDefaultPosition, wxDefaultSize);
	 wxString Choises[] = {_("16 colors"),_("256 colors"),_("15-bit color"),_("24-bit color"),
			       _("Auto")};
	 m_combo_desired_color = new wxComboBox(this,ID_ICA_DESIRED_COLOR,_(""),wxDefaultPosition,
						wxDefaultSize,
						5,Choises,wxCB_READONLY);
	 m_combo_desired_color->SetValue(Choises[4]);
	 m_combo_desired_color->SetSelection(4);

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

	 m_flexgrid_sizer_1->Add(m_static_desired_color, 1,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(m_combo_desired_color, 0,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_2->Add(m_boxsizer_3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_2->Add(m_flexgrid_sizer_1, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);

	 //m_boxsizer_4->Add(m_static_colordepth, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 //m_boxsizer_4->Add(m_choise_colordepth, 1,
	 //			 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 
	 // m_checkbox_updatescreen = new wxCheckBox( this, wxID_ANY, wxT("&Force update screen"), 
	 // 					   wxDefaultPosition, wxDefaultSize );


//		 m_checkbox_fullscreen = new wxCheckBox( this, , wxT(""), 
//												 wxDefaultPosition, wxDefaultSize );
	 // m_checkbox_smartsizing = new wxCheckBox( this, -1, wxT("&Smart sizing"), wxDefaultPosition, 
	 // 					  wxDefaultSize );
//		 m_checkbox_controlsize = new wxCheckBox( this, , wxT(""),
//												  wxDefaultPosition,
//												  wxDefaultSize);

	 m_boxsizer_6 = new wxBoxSizer(wxHORIZONTAL);
	 m_boxsizer_7 = new wxBoxSizer(wxVERTICAL);
	 m_boxsizer_10 = new wxBoxSizer(wxVERTICAL);
	 m_checkbox_allow_degraded_colors = new wxCheckBox(this, wxID_ANY, _("Allow Degraded Colors"),
							   wxDefaultPosition, wxDefaultSize);
	 m_checkbox_allow_degraded_colors->SetValue(true);
	 m_boxsizer_10->Add(m_checkbox_allow_degraded_colors, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	 m_checkbox_enable_oss = new wxCheckBox(this, wxID_ANY, _("Enable OSS"), wxDefaultPosition,
						wxDefaultSize);
	 m_checkbox_enable_oss->SetValue(true);
	 m_boxsizer_10->Add(m_checkbox_enable_oss, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	 m_checkbox_enable_session_sharing_screen = new wxCheckBox(this, wxID_ANY,
								   _("Enable Session Sharing Screen"),
								   wxDefaultPosition, wxDefaultSize);
	 m_checkbox_enable_session_sharing_screen->SetValue(false);

	 m_boxsizer_10->Add(m_checkbox_enable_session_sharing_screen, 0,
			    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 // m_boxsizer_10->Add(m_checkbox_updatescreen, 1,
	 // 		    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 // m_boxsizer_10->Add(m_checkbox_smartsizing, 1,
	 // 		    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_10->Add(-1,-1,1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	 m_boxsizer_7->Add(m_boxsizer_10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_6->Add(m_boxsizer_7, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_8 = new wxBoxSizer(wxVERTICAL);
	 m_boxsizer_9 = new wxBoxSizer(wxHORIZONTAL);
	 // m_static_desired_color = new wxStaticText(this, wxID_ANY, _("Desired Color"),
	 // 					   wxDefaultPosition, wxDefaultSize);
	 // m_boxsizer_9->Add(m_static_desired_color, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 // m_combo_desired_color = new wxComboBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	 // m_boxsizer_9->Add(m_combo_desired_color, 0,
	 // 		   wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 // m_boxsizer_9->Add(-1,-1,1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_8->Add(m_boxsizer_9, 0, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	 m_checkbox_approximate_colors = new wxCheckBox(this, wxID_ANY, _("Approximate Colors"),
							wxDefaultPosition, wxDefaultSize);
	 m_checkbox_approximate_colors->SetValue(false);
	 m_boxsizer_8->Add(m_checkbox_approximate_colors, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	 m_boxsizer_12 = new wxBoxSizer(wxHORIZONTAL);
	 m_static_use_sdlvb = new wxStaticText(this, wxID_ANY, _("Use SDLVB"),
					       wxDefaultPosition, wxDefaultSize);
	 m_boxsizer_12->Add(m_static_use_sdlvb, 0,
			    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 wxString use_sdlvb[] ={wxT("0"), wxT("1"),wxT("2"),wxT("3"),wxT("4"),wxT("5")};
	 m_combo_use_sdlvb = new wxComboBox(this,wxID_ANY,_(""),wxDefaultPosition,wxDefaultSize,
					      5,use_sdlvb,wxCB_READONLY);
	 m_combo_use_sdlvb->SetValue(Choises[3]);
	 m_combo_use_sdlvb->SetSelection(3);

	 m_boxsizer_12->Add(m_combo_use_sdlvb, 0,
			    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_12->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_8->Add(m_boxsizer_12, 0, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	 m_boxsizer_6->Add(m_boxsizer_8, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	 //
	 m_boxsizer_11 = new wxBoxSizer(wxHORIZONTAL);
	 m_static_font_smoothing_type = new wxStaticText(this, wxID_ANY, _("Font Smoothing Type"),
							 wxDefaultPosition, wxDefaultSize);
	 m_boxsizer_11->Add(m_static_font_smoothing_type, 0,
			    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 wxString font_smoothing_type[] = {wxT(""), wxT("no smoothing"), wxT("standard smoothing"),
					   wxT("ClearType")};
	 m_combo_font_smoothing_type = new wxComboBox(this,wxID_ANY,_(""),wxDefaultPosition,wxDefaultSize,
						      4,font_smoothing_type,wxCB_READONLY);
	 m_combo_font_smoothing_type->SetValue(Choises[0]);
	 m_combo_font_smoothing_type->SetSelection(0);

	 m_boxsizer_11->Add(m_combo_font_smoothing_type, 0,
			    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_screen_percent = new wxStaticText(this, wxID_ANY, _("Screen Percent"),
						    wxDefaultPosition, wxDefaultSize);
	 m_boxsizer_11->Add(m_static_screen_percent, 0,
			    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_combo_screen_percent = new wxSpinCtrl(this, wxID_ANY, _T("100"), wxDefaultPosition,
						 wxDefaultSize, 0, 0, 100, 100);
	 m_combo_screen_percent->SetValue(_T("100"));
	 m_boxsizer_11->Add(m_combo_screen_percent, 0,
			    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_checkbox_resize_session = new wxCheckBox(this, wxID_ANY, _("Resize Session"),
						    wxDefaultPosition, wxDefaultSize);
	 m_checkbox_resize_session->SetValue(false);
	 //m_boxsizer_11->Add(m_checkbox_resize_session, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	 //BoxSizer3->Add(-1,-1,1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	
	 m_boxsizer_1->Add(m_boxsizer_2, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
//	 m_boxsizer_1->Add(m_boxsizer_4, 0,
//			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_boxsizer_5, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_boxsizer_6, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_boxsizer_11, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_checkbox_resize_session, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 
	 //m_boxsizer_1->Add(m_boxsizer_12, 0,
	 //		   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	 m_boxsizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
#ifdef __WXMSW__
	 //sizer_main->Add(m_checkbox_smartsizing,0,wxALIGN_LEFT | wxALL,3);
#endif
#ifdef __WXGTK__
	 //sizer_main->Add(m_checkbox_updatescreen,0,wxALIGN_LEFT | wxALL,3);
#endif

#ifdef __WXMSW__
//	 m_checkbox_updatescreen->Show(false);
#endif
#ifdef __WXGTK__
//	 m_checkbox_smartsizing->Show(false);
//		 m_checkbox_controlsize->Show(true);
#endif
	 this->SetSizer(m_boxsizer_1);
	 m_boxsizer_1->Fit(this);
	 m_boxsizer_1->SetSizeHints(this);
	 CheckICADisplay();
	 change_desired_color();
      }
   }
   return error;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
wxArrayString ICAOptionsDialog_Display::GetResolutionsList()
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

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
wxSize ICAOptionsDialog_Display::GetResolution(int i)
{
   wxSize error_size(0,0);
   if (i < 0) return error_size;
   wxArrayString resolutions = GetResolutionsList();
   if (i >= (int)resolutions.Count())  return error_size;
   wxString resolution_string = resolutions.Item(i);
   //wxMessageBox (resolution_string);
   
   size_t len = resolution_string.Length(); 
   int pos = resolution_string.Find(wxT("x"));
	
   wxString width_string = resolution_string.Left(pos - 1);
   wxString heigth_string = resolution_string.Right(len - pos - 1); 

   wxSize result_size(wxAtoi(width_string),wxAtoi(heigth_string));
   return result_size;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
int ICAOptionsDialog_Display::GetDefaultResolution()
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

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
void ICAOptionsDialog_Display::CheckICADisplay()
{

   if (m_radiobutton_controlsize->GetValue() == true)
   {
      m_text_width->Enable(false);
      m_text_height->Enable(false);
      m_combobox_resolution->Enable(false);
   }
   else if (m_radiobutton_fullscreen->GetValue() == true)
   {
      m_text_width->Enable(false);
      m_text_height->Enable(false);
      m_combobox_resolution->Enable(false);
   }
   else if (m_radiobutton_customgeometry->GetValue() == true)
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

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
void ICAOptionsDialog_Display::checkbox_customgeometry_func(wxCommandEvent& event)
{
   CheckICADisplay();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
void ICAOptionsDialog_Display::checkbox_fullscreen_func(wxCommandEvent &event)
{
   CheckICADisplay();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
void ICAOptionsDialog_Display::checkbox_controlsize_func(wxCommandEvent &event)
{
   CheckICADisplay();
}

void ICAOptionsDialog_Display::change_desired_color_func(wxCommandEvent &event)
{
   change_desired_color();
}

void ICAOptionsDialog_Display::change_desired_color()
{
   if (m_combo_desired_color->GetCurrentSelection() == 1)
   {
      m_checkbox_approximate_colors->Enable(true);
   }
   else
   {
      m_checkbox_approximate_colors->Enable(false);
   }

}


///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
ICAOptionsDialog_Display::~ICAOptionsDialog_Display()
{
   if(m_static_width != NULL) {delete m_static_width; m_static_width = NULL;}
   if(m_text_width != NULL) {delete m_text_width; m_text_width = NULL;}
   if(m_static_height != NULL) {delete m_static_height; m_static_height = NULL;}
   if(m_text_height != NULL) {delete m_text_height; m_text_height = NULL;}
//   if(m_static_colordepth != NULL) {delete m_static_colordepth; m_static_colordepth = NULL;}
//   if(m_choise_colordepth != NULL) {delete m_choise_colordepth; m_choise_colordepth = NULL;}
//   if(m_checkbox_smartsizing != NULL) {delete m_checkbox_smartsizing; m_checkbox_smartsizing = NULL;}
//   if(m_checkbox_updatescreen != NULL) {delete m_checkbox_updatescreen; m_checkbox_updatescreen = NULL;}
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

   if (m_checkbox_allow_degraded_colors != NULL)
   {delete m_checkbox_allow_degraded_colors; m_checkbox_allow_degraded_colors = NULL;}
   if (m_checkbox_enable_oss != NULL){delete m_checkbox_enable_oss; m_checkbox_enable_oss = NULL;}
   if (m_checkbox_enable_session_sharing_screen != NULL)
   {delete m_checkbox_enable_session_sharing_screen; m_checkbox_enable_session_sharing_screen = NULL;}
   if (m_checkbox_approximate_colors != NULL)
   {delete m_checkbox_approximate_colors; m_checkbox_approximate_colors = NULL;}
   if (m_checkbox_resize_session != NULL)
   {delete m_checkbox_resize_session; m_checkbox_resize_session = NULL;}

   if (m_static_desired_color != NULL){delete m_static_desired_color; m_static_desired_color = NULL;}
   if (m_combo_desired_color != NULL){delete m_combo_desired_color; m_combo_desired_color = NULL;}

   if (m_static_use_sdlvb != NULL){delete m_static_use_sdlvb; m_static_use_sdlvb = NULL;}
   if (m_combo_use_sdlvb != NULL){delete m_combo_use_sdlvb; m_combo_use_sdlvb = NULL;}

   if (m_static_font_smoothing_type != NULL)
   {delete m_static_font_smoothing_type; m_static_font_smoothing_type = NULL;}
   if (m_combo_font_smoothing_type != NULL)
   {delete m_combo_font_smoothing_type; m_combo_font_smoothing_type = NULL;}

   if (m_static_screen_percent != NULL){delete m_static_screen_percent; m_static_screen_percent = NULL;}
   if (m_combo_screen_percent != NULL){delete m_combo_screen_percent; m_combo_screen_percent = NULL;}

}


///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap ICAOptionsDialog_Display::Get_Options()
{
   Options_HashMap local_options;

   local_options.clear();
   if(m_combo_desired_color != NULL)
   {
      local_options[wxT("desired_color")] = wxString::Format(wxT("%i"),
							   m_combo_desired_color->GetCurrentSelection());
   }
   // if(m_checkbox_updatescreen != NULL)
   // {
   //    local_options[wxT("force_update_screen")] = wxString::Format(wxT("%i"),
   // 								   m_checkbox_updatescreen->GetValue());
   // }
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

   if (m_checkbox_allow_degraded_colors != NULL)
   {
      local_options[wxT("allow_degraded_colors")] = wxString::Format(wxT("%i"),
								     m_checkbox_allow_degraded_colors->GetValue());
   }
   if (m_checkbox_enable_oss != NULL)
   {
      local_options[wxT("enable_oss")] = wxString::Format(wxT("%i"),
							  m_checkbox_enable_oss->GetValue());
   }
   if (m_checkbox_enable_session_sharing_screen != NULL)
   {
      local_options[wxT("enable_session_sharing_screen")] = wxString::Format(wxT("%i"),
									     m_checkbox_enable_session_sharing_screen->GetValue());
   }
   if (m_checkbox_approximate_colors != NULL)
   {
      local_options[wxT("approximate_colors")] = wxString::Format(wxT("%i"),
								  m_checkbox_approximate_colors->GetValue());
   }
   if (m_checkbox_resize_session != NULL)
   {
      local_options[wxT("resize_session")] = wxString::Format(wxT("%i"),
							      m_checkbox_resize_session->GetValue());
   }

   if (m_combo_desired_color != NULL)
   {
      local_options[wxT("desired_color")] = wxString::Format(wxT("%i"),
							     m_combo_desired_color->GetCurrentSelection());
   }
   if (m_combo_use_sdlvb != NULL)
   {
      local_options[wxT("use_sdlvb")] = wxString::Format(wxT("%i"),
							 m_combo_use_sdlvb->GetCurrentSelection());
   }

   if (m_combo_font_smoothing_type != NULL)
   {
      local_options[wxT("font_smoothing_type")] = wxString::Format(wxT("%i"),
								   m_combo_font_smoothing_type->GetCurrentSelection());
   }

   if (m_combo_screen_percent != NULL)
   {
      local_options[wxT("screen_percent")] = wxString::Format(wxT("%i"),
							      m_combo_screen_percent->GetValue());
   }

  // if(m_checkbox_smartsizing != NULL)
   // {
   //    local_options[wxT("smart_sizing")] = wxString::Format(wxT("%i"),
   // 							    m_checkbox_smartsizing->GetValue());
   // }

   return local_options;
}
      
///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
bool ICAOptionsDialog_Display::Set_Options(const Options_HashMap *all_options)
{
   bool state = false;
   Options_HashMap local_options;
   local_options = *all_options;
   
   if ((m_combo_desired_color != NULL))
   {
      if ((wxAtoi(local_options[wxT("desired_color")]) > -1) &&
	  (wxAtoi(local_options[wxT("desired_color")]) < 4))
      {
	 m_combo_desired_color->SetSelection(wxAtoi(local_options[wxT("desired_color")]));
      }
      else
      {
	 m_combo_desired_color->SetValue(wxT("15-bit color"));
      }
   }
   // if((m_checkbox_updatescreen != NULL))
   // {
   //    m_checkbox_updatescreen->SetValue(wxAtoi(local_options[wxT("force_update_screen")]));
   // }
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
   // if ((m_checkbox_smartsizing != NULL))
   // {
   //    m_radiobutton_presetsize->SetValue(wxAtoi(local_options[wxT("smart_sizing")])); 
   // }

   if (m_checkbox_allow_degraded_colors != NULL)
   {
      m_checkbox_allow_degraded_colors->SetValue(wxAtoi(local_options[wxT("allow_degraded_colors")]));
   }
   if (m_checkbox_enable_oss != NULL)
   {
      m_checkbox_enable_oss->SetValue(wxAtoi(local_options[wxT("enable_oss")]));
   }
   if (m_checkbox_enable_session_sharing_screen != NULL)
   {
      m_checkbox_enable_session_sharing_screen->SetValue(wxAtoi(local_options[wxT("enable_session_sharing_screen")]));
   }
   if (m_checkbox_approximate_colors != NULL)
   {
      m_checkbox_approximate_colors->SetValue(wxAtoi(local_options[wxT("approximate_colors")]));
   }
   if (m_checkbox_resize_session != NULL)
   {
      m_checkbox_resize_session->SetValue(wxAtoi(local_options[wxT("resize_session")]));
   }

   if (m_combo_desired_color != NULL)
   {
      m_combo_desired_color->SetSelection(wxAtoi(local_options[wxT("desired_color")]));
   }

   if (m_combo_use_sdlvb != NULL)
   {
      m_combo_use_sdlvb->SetSelection(wxAtoi(local_options[wxT("use_sdlvb")]));
   }
   if (m_combo_font_smoothing_type != NULL)
   {
      m_combo_font_smoothing_type->SetSelection(wxAtoi(local_options[wxT("font_smoothing_type")]));
   }

   if (m_combo_screen_percent != NULL) {
      m_combo_screen_percent->SetValue(local_options[wxT("screen_percent")]);
   }

   CheckICADisplay();
   change_desired_color();
   return state;
}


///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
ICAOptionsDialog_Advanced::ICAOptionsDialog_Advanced(ICAOptionsDialog *parent_element)
{
   m_boxsizer_1 = NULL;
   m_flexgrid_sizer_1 = NULL;
   m_gridsizer_1 = NULL;

   m_checkbox_start_initial_program = NULL;

   m_static_initial_program = NULL;
   m_text_initial_program = NULL;

   m_static_long_command_line = NULL;
   m_text_long_command_line = NULL;

   m_static_work_directory = NULL;
   m_text_work_directory = NULL;

   m_checkbox_compress = NULL;
   m_checkbox_allow_duplicate_sessions = NULL;
   m_checkbox_persistent_cache_enabled = NULL;
   m_checkbox_desktop_restart_allowed = NULL;
   m_checkbox_mouse_sends_control_v = NULL;
   m_checkbox_echo_shift_keys = NULL; 

   error = 0;
   if (parent_element != NULL)
   {
      parent = parent_element;
   }
   else
   {
      error = ICA_ERROR_PARENT_IS_NULL; 
   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
int ICAOptionsDialog_Advanced::Build()
{
   if ( error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
      if ( state == false) 
      {
	 error = ICA_ERROR_CANT_CREATE_PANEL;
      } 
      else 
      {
	 m_boxsizer_1 = new wxBoxSizer(wxVERTICAL);
	 m_flexgrid_sizer_1 = new wxFlexGridSizer(0, 2, 0, 0);
	 m_flexgrid_sizer_1->AddGrowableCol(1);
	 m_checkbox_start_initial_program = new wxCheckBox(this, ID_ICA_CHECKBOX_USE_PROGRAM,
							   _("Start following program"),
							   wxDefaultPosition, wxDefaultSize);
	 m_checkbox_start_initial_program->SetValue(false);
	 m_flexgrid_sizer_1->Add(m_checkbox_start_initial_program, 1,
				 wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(-1,-1,1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_initial_program = new wxStaticText(this, wxID_ANY, _("Initial Program"),
						     wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_1->Add(m_static_initial_program, 1,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_text_initial_program = new wxTextCtrl(this, wxID_ANY, _(""), wxDefaultPosition,
						 wxDefaultSize);
	 m_flexgrid_sizer_1->Add(m_text_initial_program, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_long_command_line = new wxStaticText(this, wxID_ANY, _("Long Command Line"),
						       wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_1->Add(m_static_long_command_line, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	 m_text_long_command_line = new wxTextCtrl(this, wxID_ANY, _(""), wxDefaultPosition,
						   wxDefaultSize, wxTE_MULTILINE);
	 m_flexgrid_sizer_1->Add(m_text_long_command_line, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_work_directory = new wxStaticText(this, wxID_ANY, _("Work Directory"),
						    wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_1->Add(m_static_work_directory, 1,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_text_work_directory= new wxTextCtrl(this, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_1->Add(m_text_work_directory, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_flexgrid_sizer_1, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_gridsizer_1 = new wxGridSizer(0, 2, 0, 0);
	 m_checkbox_compress = new wxCheckBox(this, wxID_ANY, _("Compress"), wxDefaultPosition,
					      wxDefaultSize);
	 m_checkbox_compress->SetValue(true);
	 m_gridsizer_1->Add(m_checkbox_compress, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_checkbox_desktop_restart_allowed = new wxCheckBox(this, wxID_ANY, _("Desktop Restart Allowed"),
							      wxDefaultPosition, wxDefaultSize);
	 m_checkbox_desktop_restart_allowed->SetValue(false);
	 m_gridsizer_1->Add(m_checkbox_desktop_restart_allowed, 0,
			    wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_checkbox_allow_duplicate_sessions = new wxCheckBox(this, wxID_ANY, _("Allow Duplicate Sessions"),
							      wxDefaultPosition, wxDefaultSize);
	 m_checkbox_allow_duplicate_sessions->SetValue(false);
	 m_gridsizer_1->Add(m_checkbox_allow_duplicate_sessions, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_checkbox_mouse_sends_control_v = new wxCheckBox(this, wxID_ANY, _("Mouse Sends Control+V"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX13"));
	 m_checkbox_mouse_sends_control_v->SetValue(false);
	 m_gridsizer_1->Add(m_checkbox_mouse_sends_control_v, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_checkbox_persistent_cache_enabled = new wxCheckBox(this, wxID_ANY, _("Persistent Cache Enabled"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX16"));
	 m_checkbox_persistent_cache_enabled->SetValue(false);
	 m_gridsizer_1->Add(m_checkbox_persistent_cache_enabled, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_checkbox_echo_shift_keys = new wxCheckBox(this, wxID_ANY, _("Echo Shift Keys"),
						     wxDefaultPosition, wxDefaultSize);
	 m_checkbox_echo_shift_keys->SetValue(false);
	 m_gridsizer_1->Add(m_checkbox_echo_shift_keys, 1,
			    wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_gridsizer_1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 this->SetSizer(m_boxsizer_1);
	 m_boxsizer_1->Fit(this);
	 m_boxsizer_1->SetSizeHints(this);

	 checkbox_program();
	 
      }
   }
   return error;
  
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
ICAOptionsDialog_Advanced::~ICAOptionsDialog_Advanced()
{
   if (m_checkbox_start_initial_program != NULL)
   {delete m_checkbox_start_initial_program; m_checkbox_start_initial_program = NULL;}
   if (m_static_initial_program != NULL)
   {delete m_static_initial_program; m_static_initial_program = NULL;}
   if (m_text_initial_program != NULL)
   {delete m_text_initial_program; m_text_initial_program = NULL;}
   if (m_static_long_command_line != NULL)
   {delete m_static_long_command_line; m_static_long_command_line = NULL;}
   if (m_text_long_command_line != NULL)
   {delete m_text_long_command_line; m_text_long_command_line = NULL;}
   if (m_static_work_directory != NULL)
   {delete m_static_work_directory; m_static_work_directory = NULL;}
   if (m_text_work_directory != NULL)
   {delete m_text_work_directory; m_text_work_directory = NULL;}
   if (m_checkbox_compress != NULL)
   {delete m_checkbox_compress; m_checkbox_compress = NULL;}
   if (m_checkbox_allow_duplicate_sessions != NULL)
   {delete m_checkbox_allow_duplicate_sessions; m_checkbox_allow_duplicate_sessions = NULL;}
   if (m_checkbox_persistent_cache_enabled != NULL)
   {delete m_checkbox_persistent_cache_enabled; m_checkbox_persistent_cache_enabled = NULL;}
   if (m_checkbox_desktop_restart_allowed != NULL)
   {delete m_checkbox_desktop_restart_allowed; m_checkbox_desktop_restart_allowed = NULL;}
   if (m_checkbox_mouse_sends_control_v != NULL)
   {delete m_checkbox_mouse_sends_control_v; m_checkbox_mouse_sends_control_v = NULL;}
   if (m_checkbox_echo_shift_keys != NULL)
   {delete m_checkbox_echo_shift_keys; m_checkbox_echo_shift_keys = NULL;} 
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap ICAOptionsDialog_Advanced::Get_Options()
{
   Options_HashMap local_options;
   if (m_checkbox_start_initial_program != NULL)
   {
      local_options[wxT("start_initial_program")] = wxString::Format(wxT("%i"),
								     m_checkbox_start_initial_program->GetValue());
   }
   if (m_text_initial_program != NULL)
   {
      local_options[wxT("initial_program")] = m_text_initial_program->GetValue();
   }
   if (m_text_long_command_line != NULL)
   {
      local_options[wxT("long_command_line")] = m_text_long_command_line->GetValue();
   }
   if (m_text_work_directory != NULL)
   {
      local_options[wxT("work_directory")] = m_text_work_directory->GetValue();
   }
   if (m_checkbox_compress != NULL)
   {
      local_options[wxT("compress")] = wxString::Format(wxT("%i"),
							m_checkbox_compress->GetValue());
   }
   if (m_checkbox_allow_duplicate_sessions != NULL)
   {
      local_options[wxT("allow_duplicate_sessions")] = wxString::Format(wxT("%i"),
									m_checkbox_allow_duplicate_sessions->GetValue());
   }
   if (m_checkbox_persistent_cache_enabled != NULL)
   {
      local_options[wxT("persistent_cache_enabled")] = wxString::Format(wxT("%i"),
									m_checkbox_persistent_cache_enabled->GetValue());
   }
   if (m_checkbox_desktop_restart_allowed != NULL)
   {
      local_options[wxT("desktop_restart_allowed")] = wxString::Format(wxT("%i"),
								       m_checkbox_desktop_restart_allowed->GetValue());
   }
   if (m_checkbox_mouse_sends_control_v != NULL)
   {
      local_options[wxT("mouse_sends_control_v")] = wxString::Format(wxT("%i"),
								     m_checkbox_mouse_sends_control_v->GetValue());
   }
   if (m_checkbox_echo_shift_keys != NULL)
   {
      local_options[wxT("echo_shift_keys")] = wxString::Format(wxT("%i"),
							       m_checkbox_echo_shift_keys->GetValue());
   }
//    // result += GetParamInt (wxT("connection_count"), (int)(rdpc.dwConnectionCount));

//    // // ICA Citrix
//    // result += GetParamStr (wxT("ica_server_ini"), (rdpc.server_ini));
//    // result += GetParamStr (wxT("ica_client_ini"), (rdpc.client_ini));
//    // result += GetParamInt (wxT("ICASound"), (int)(rdpc.bIcaSound));
//    // result += GetParamInt (wxT("ICASoundType"), (int)(rdpc.bIcaSoundType));
//    // result += GetParamInt (wxT("ICAEncryption"), (int)(rdpc.bIcaEncryption));
//    // result += GetParamInt (wxT("ICAEncryptionType"), (int)(rdpc.ica_encryption));

//    // result += GetParamStr (wxT("ica_connection_file"), (rdpc.ica_file));
//    // result += GetParamInt (wxT("bUseApplication"), (int)(rdpc.bUseApplication));
//    // result += GetParamStr (wxT("ICAApplication"), (rdpc.IcaApplication));

//    // result += GetParamInt (wxT("bProxyType"), (int)(rdpc.bProxyType));
//    // result += GetParamStr (wxT("ProxyAddr"), (rdpc.ProxyAddr));
//    // result += GetParamStr (wxT("ProxyPort"), (rdpc.ProxyPort));
//    // result += GetParamStr (wxT("ProxyUserId"), (rdpc.ProxyUserId));
//    // result += GetParamStr (wxT("ProxyPassword"),
//    // (rdpc.ProxyPassword));

   return local_options;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
bool ICAOptionsDialog_Advanced::Set_Options(const Options_HashMap *all_options)
{
   Options_HashMap local_options;
   if (all_options != NULL)
   {
      local_options = *all_options;
      if (m_checkbox_start_initial_program != NULL)
      {
	 m_checkbox_start_initial_program->SetValue(wxAtoi(local_options[wxT("start_initial_program")]));
      }

      if (m_text_initial_program != NULL)
      {
	 m_text_initial_program->SetValue(local_options[wxT("initial_program")]);
      }
      if (m_text_long_command_line != NULL)
      {
	 m_text_long_command_line->SetValue(local_options[wxT("long_command_line")]);
      }
      if (m_text_work_directory != NULL)
      {
	 m_text_work_directory->SetValue(local_options[wxT("work_directory")]);
      }
      if (m_checkbox_compress != NULL)
      {
	 m_checkbox_compress->SetValue(wxAtoi(local_options[wxT("compress")]));
      }
      if (m_checkbox_allow_duplicate_sessions != NULL)
      {
	 m_checkbox_allow_duplicate_sessions->SetValue(wxAtoi(local_options[wxT("allow_duplicate_sessions")]));
      }
      if (m_checkbox_persistent_cache_enabled != NULL)
      {
	 m_checkbox_persistent_cache_enabled->SetValue(wxAtoi(local_options[wxT("persistent_cache_enabled")]));
      }
      if (m_checkbox_desktop_restart_allowed != NULL)
      {
	 m_checkbox_desktop_restart_allowed->SetValue(wxAtoi(local_options[wxT("desktop_restart_allowed")]));
      }
      if (m_checkbox_mouse_sends_control_v != NULL)
      {
	 m_checkbox_mouse_sends_control_v->SetValue(wxAtoi(local_options[wxT("mouse_sends_control_v")]));
      }
      if (m_checkbox_echo_shift_keys != NULL)
      {
	 m_checkbox_echo_shift_keys->SetValue(wxAtoi(local_options[wxT("echo_shift_keys")]));
      } 
      checkbox_program();
   }
   return true;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
void ICAOptionsDialog_Advanced::checkbox_program() 
{
   if(!m_checkbox_start_initial_program->IsChecked() )
   {
      m_static_initial_program->Enable(false);
      m_text_initial_program->Enable(false);
      m_static_long_command_line->Enable(false);
      m_text_long_command_line->Enable(false);
      m_static_work_directory->Enable(false);
      m_text_work_directory->Enable(false);
   }
   else
   {
      m_static_initial_program->Enable(true);
      m_text_initial_program->Enable(true);
      m_static_long_command_line->Enable(true);
      m_text_long_command_line->Enable(true);
      m_static_work_directory->Enable(true);
      m_text_work_directory->Enable(true);
   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief 
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
void ICAOptionsDialog_Advanced::checkbox_program_func(wxCommandEvent &event) 
{
   checkbox_program();
}


ICAOptionsDialog_Sound::ICAOptionsDialog_Sound(ICAOptionsDialog *parent_element)
{
   error = 0;
   
   if (parent_element !=NULL)
   {
      parent = parent_element;
   }
   else
   {
      parent = NULL;
      error = ICA_ERROR_PARENT_IS_NULL; 
   }

   m_boxsizer_1 = NULL;
   m_boxsizer_2 = NULL;
   m_boxsizer_3 = NULL;

   m_checkbox_client_audio = NULL;
   m_checkbox_enable_audio_input = NULL;

   m_static_audio_bandwidth_limit = NULL;
   m_combo_audio_bandwidth_limit = NULL;

	m_checkbox_drives = NULL;
	m_checkbox_printers = NULL;
	 m_checkbox_comports = NULL;
	 m_checkbox_smartcards = NULL;

}

ICAOptionsDialog_Sound::~ICAOptionsDialog_Sound()
{
   if (m_checkbox_client_audio != NULL)
   {delete m_checkbox_client_audio; m_checkbox_client_audio = NULL;}
   if (m_checkbox_enable_audio_input != NULL)
   {delete m_checkbox_enable_audio_input; m_checkbox_enable_audio_input = NULL;}
   if (m_static_audio_bandwidth_limit != NULL)
   {delete m_static_audio_bandwidth_limit; m_static_audio_bandwidth_limit = NULL;}
   if (m_combo_audio_bandwidth_limit != NULL)
   {delete m_combo_audio_bandwidth_limit; m_combo_audio_bandwidth_limit = NULL;}
      if (m_checkbox_drives) {delete m_checkbox_drives; m_checkbox_drives = NULL;}
   if (m_checkbox_printers) {delete m_checkbox_printers; m_checkbox_printers = NULL;}
   if (m_checkbox_comports) {delete m_checkbox_comports; m_checkbox_comports = NULL;}
   if (m_checkbox_smartcards) {delete m_checkbox_smartcards; m_checkbox_smartcards = NULL;}


}

int ICAOptionsDialog_Sound::Build()
{
   if (error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
      if ( state == false) 
      {
	 error = ICA_ERROR_CANT_CREATE_PANEL;
      } 
      else 
      {
	 m_boxsizer_1 = new wxBoxSizer(wxVERTICAL);
	 m_boxsizer_2 = new wxBoxSizer(wxVERTICAL);

	 m_checkbox_client_audio = new wxCheckBox(this, wxID_ANY, _("Enables client audio"),
						  wxDefaultPosition, wxDefaultSize);
	 m_checkbox_client_audio->SetValue(true);
	 m_boxsizer_2->Add(m_checkbox_client_audio, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_checkbox_enable_audio_input = new wxCheckBox(this, wxID_ANY,
	 						     _("Enables audio input for a connection"),
	 						     wxDefaultPosition, wxDefaultSize);
	 m_checkbox_enable_audio_input->SetValue(true);
	 m_boxsizer_2->Add(m_checkbox_enable_audio_input, 1,
			   wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3 = new wxBoxSizer(wxHORIZONTAL);
   
	 m_static_audio_bandwidth_limit = new wxStaticText(this, wxID_ANY, _("Audio quality"),
	 							wxDefaultPosition, wxDefaultSize);
	 m_boxsizer_3->Add(m_static_audio_bandwidth_limit, 0,
	 			wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 wxString audio_bandwidth_limit[] ={wxT("high"), wxT("medium"), wxT("low")};
	 m_combo_audio_bandwidth_limit = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
	 						     wxDefaultSize, 3, audio_bandwidth_limit);
	 m_combo_audio_bandwidth_limit->SetValue(audio_bandwidth_limit[0]);
	 m_combo_audio_bandwidth_limit->SetSelection(0);
	 m_boxsizer_3->Add(m_combo_audio_bandwidth_limit, 0,
	 			wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_2->Add(m_boxsizer_3, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_boxsizer_2, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 
	 m_checkbox_drives = new wxCheckBox( this, wxID_ANY, wxT("&Redirect drives"), wxDefaultPosition,
					     wxDefaultSize );
	 m_checkbox_printers = new wxCheckBox( this, wxID_ANY, wxT("&Redirect printers"),
					       wxDefaultPosition, wxDefaultSize );
	 m_checkbox_comports = new wxCheckBox( this, wxID_ANY, wxT("&Redirect COM ports"),
					       wxDefaultPosition, wxDefaultSize );
	 m_checkbox_smartcards = new wxCheckBox( this, wxID_ANY, wxT("&Redirect smartcards"),
						 wxDefaultPosition, wxDefaultSize );
	 m_boxsizer_4 = new wxBoxSizer(wxVERTICAL);
	 m_boxsizer_4->Add(m_checkbox_drives, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_4->Add(m_checkbox_printers, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_4->Add(m_checkbox_comports, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_4->Add(m_checkbox_smartcards, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_4->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_boxsizer_4, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	 m_boxsizer_1->Add(-1,-1,1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 this->SetSizer(m_boxsizer_1);
      }
   }
#ifdef __WXGTK__
	 m_checkbox_drives->Show(false);
	 m_checkbox_printers->Show(false);
	 m_checkbox_comports->Show(false);
	 m_checkbox_smartcards->Show(false);
#endif
   return error;
}

Options_HashMap ICAOptionsDialog_Sound::Get_Options()
{
   Options_HashMap local_options;
   
   if (m_checkbox_client_audio != NULL)
   {
      local_options[wxT("client_audio")] = wxString::Format(wxT("%i"),
							    m_checkbox_client_audio->GetValue());
   }
   if (m_checkbox_enable_audio_input != NULL)
   {
      local_options[wxT("enable_audio_input")] = wxString::Format(wxT("%i"),
								  m_checkbox_enable_audio_input->GetValue());
   }
   if (m_combo_audio_bandwidth_limit != NULL)
   {
      local_options[wxT("audio_bandwidth_limit")] = wxString::Format(wxT("%i"),
								     m_combo_audio_bandwidth_limit->GetCurrentSelection());
   }
      if (m_checkbox_drives !=NULL)
   {
      local_options[wxT("share_drives")] = wxString::Format(wxT("%i"),
							    m_checkbox_drives->GetValue());
   }
   if (m_checkbox_printers !=NULL)
   {
      local_options[wxT("share_printers")] = wxString::Format(wxT("%i"),
							      m_checkbox_printers->GetValue());
   }
   if (m_checkbox_comports !=NULL)
   {
      local_options[wxT("share_com_ports")]= wxString::Format(wxT("%i"),
							      m_checkbox_comports->GetValue());
   }
   if (m_checkbox_smartcards !=NULL)
   {
      local_options[wxT("share_smart_cards")] = wxString::Format(wxT("%i"),
								 m_checkbox_smartcards->GetValue());
   }
   return local_options;
}

bool ICAOptionsDialog_Sound::Set_Options(const Options_HashMap *all_options)
{
   Options_HashMap local_options;
   if (all_options != NULL)
   {
      local_options = *all_options;
      if (m_checkbox_client_audio != NULL)
      {
	 m_checkbox_client_audio->SetValue(wxAtoi(local_options[wxT("client_audio")]));
      }
      if (m_checkbox_enable_audio_input != NULL)
      {
	 m_checkbox_enable_audio_input->SetValue(wxAtoi(local_options[wxT("enable_audio_input")]));
      }
      if (m_combo_audio_bandwidth_limit != NULL)
      {
	 m_combo_audio_bandwidth_limit->SetSelection(wxAtoi(local_options[wxT("audio_bandwidth_limit")]));
      }
	     if (m_checkbox_drives !=NULL)
   {
      m_checkbox_drives->SetValue(local_options[wxT("share_drives")]);
   }
   if (m_checkbox_printers !=NULL)
   {
      m_checkbox_printers->SetValue(local_options[wxT("share_printers")]);

   }
   if (m_checkbox_comports !=NULL)
   {
      m_checkbox_comports->SetValue(local_options[wxT("share_com_ports")]);

   }
   if (m_checkbox_smartcards !=NULL)
   {
      m_checkbox_smartcards->SetValue(local_options[wxT("share_smart_cards")]);
   }
   }

   return true;
}

ICAOptionsDialog_Security::ICAOptionsDialog_Security(ICAOptionsDialog *parent_element)
{
   m_boxsizer_1 = NULL;
   m_flexgrid_sizer_1 = NULL;
   m_static_encryption_level_session = NULL;
   m_combo_encryption_level_session = NULL;
   m_static_secure_channel_protocol = NULL;
   m_combo_secure_channel_protocol = NULL;

   error = 0;
   if (parent_element != NULL)
   {
      parent = parent_element;
   }
   else
   {
      error = ICA_ERROR_PARENT_IS_NULL; 
   }

}

ICAOptionsDialog_Security::~ICAOptionsDialog_Security()
{
   if (m_static_encryption_level_session != NULL)
   {delete m_static_encryption_level_session; m_static_encryption_level_session = NULL;}
   if (m_combo_encryption_level_session != NULL)
   {delete m_combo_encryption_level_session; m_combo_encryption_level_session = NULL;}
   if (m_static_secure_channel_protocol != NULL)
   {delete m_static_secure_channel_protocol; m_static_secure_channel_protocol = NULL;}
   if (m_combo_secure_channel_protocol != NULL)
   {delete m_combo_secure_channel_protocol; m_combo_secure_channel_protocol = NULL;}
}

int ICAOptionsDialog_Security::Build()
{
   if ( error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
      if ( state == false)
      {
	 error = ICA_ERROR_CANT_CREATE_PANEL;
      }
      else
      {
	 m_boxsizer_1 = new wxBoxSizer(wxVERTICAL);
	 m_flexgrid_sizer_1 = new wxFlexGridSizer(0, 2, 0, 0);
	 m_flexgrid_sizer_1->AddGrowableCol(1);
	 m_static_encryption_level_session = new wxStaticText(this, wxID_ANY, _("EncryptionLevelSession"),
							      wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_1->Add(m_static_encryption_level_session, 0,
				 wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 wxString encryption_level_session[] = {wxT("None"), wxT("Basic"),
						wxT("RC5 (128 bit 0 Login Only)"),
						wxT("RC5 (40 bit)"),
						wxT("RC5 (56 bit)"), wxT("RC5 (128 bit)"), };
	 m_combo_encryption_level_session = new wxComboBox(this, wxID_ANY, wxEmptyString,
							   wxDefaultPosition, wxDefaultSize,
							   6, encryption_level_session);
	 m_combo_encryption_level_session->SetValue(encryption_level_session[1]);
	 m_combo_encryption_level_session->SetSelection(1);

	 m_flexgrid_sizer_1->Add(m_combo_encryption_level_session, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_secure_channel_protocol = new wxStaticText(this, wxID_ANY, _("SecureChannelProtocol"),
							     wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_1->Add(m_static_secure_channel_protocol, 0,
				 wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 wxString secure_channel_protocol[] = {wxT("ssl"), wxT("tls"), wxT("detect"), };
	 m_combo_secure_channel_protocol = new wxComboBox(this, wxID_ANY, wxEmptyString,
							  wxDefaultPosition, wxDefaultSize,
							  3, secure_channel_protocol);
	 m_combo_secure_channel_protocol->SetValue(secure_channel_protocol[2]);
	 m_combo_secure_channel_protocol->SetSelection(2);

	 m_flexgrid_sizer_1->Add(m_combo_secure_channel_protocol, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_flexgrid_sizer_1, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(-1,-1,1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 this->SetSizer(m_boxsizer_1);
      }
   }
   return error;
}

Options_HashMap ICAOptionsDialog_Security::Get_Options()
{
   Options_HashMap local_options;
   if (m_combo_encryption_level_session != NULL)
   {
      local_options[wxT("encryption_level_session")] = wxString::Format(wxT("%i"),
   									m_combo_encryption_level_session->GetCurrentSelection());
   }
   if (m_combo_secure_channel_protocol != NULL)
   {
      local_options[wxT("secure_channel_protocol")] = wxString::Format(wxT("%i"),
   								       m_combo_secure_channel_protocol->GetCurrentSelection());
   }

   return local_options;
}

bool ICAOptionsDialog_Security::Set_Options(const Options_HashMap *all_options)
{
   Options_HashMap local_options;
   if (all_options != NULL)
   {
      local_options = *all_options;
      if (m_combo_encryption_level_session != NULL)
      {
	 m_combo_encryption_level_session->SetSelection(wxAtoi(local_options[wxT("encryption_level_session")]));
      }
      if (m_combo_secure_channel_protocol != NULL)
      {
	 m_combo_secure_channel_protocol->SetSelection(wxAtoi(local_options[wxT("secure_channel_protocol")]));
      }
   }
	return true;
}

wxString ICAOptionsDialog_Security::Get_Options_String()
{
}

