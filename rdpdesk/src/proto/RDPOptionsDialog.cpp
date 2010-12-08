///////////////////////////////////////////////////////////////////////////////
// File name:   RDPOptionsDialog.cpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-12-08 14:12:10"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include "RDPOptionsDialog.hpp"
#include <wx/display.h>
#include <wx/dir.h>
#include <wx/font.h>

BEGIN_EVENT_TABLE(RDPOptionsDialog_General, wxPanel)
EVT_CHECKBOX (ID_RDP_CHECKBOX_CUSTOMPORT, RDPOptionsDialog_General::checkbox_customport_func)
EVT_TEXT(ID_RDP_TEXTCTRL_PASSWORD, RDPOptionsDialog_General::text_passwordentry_func)
EVT_CHECKBOX (ID_RDP_CHECKBOX_AUTOLOGON, RDPOptionsDialog_General::text_passwordentry_func)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(RDPOptionsDialog_Display, wxPanel)
EVT_RADIOBUTTON (ID_RDP_CHK_CONTROLSIZE, RDPOptionsDialog_Display::checkbox_controlsize_func)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(RDPOptionsDialog_Program, wxPanel)
EVT_CHECKBOX(ID_RDP_CHECKBOX_USE_PROGRAM, RDPOptionsDialog_Program::checkbox_program_func)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(RDPOptionsDialog_Performance, wxPanel)
EVT_COMBOBOX(ID_RDP_COMBOBOX_SPEED, RDPOptionsDialog_Performance::choise_speed_func)
END_EVENT_TABLE()

//WX_DEFINE_OBJARRAY(GeneralOptionsArray);
///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
RDPOptionsDialog::RDPOptionsDialog(wxWindow *parent_element)
{
   parent = NULL;
   rdp_general = NULL;
   rdp_display = NULL;
   rdp_share = NULL;
   rdp_program = NULL;
   rdp_perfomance = NULL;
   rdp_advanced = NULL;
   save_options.clear();

   error = 0;
   if (parent_element != NULL)
   {
      parent = parent_element;
   }
   else
   {
      error = ERROR_PARENT_IS_NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
RDPOptionsDialog::~RDPOptionsDialog()
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
int RDPOptionsDialog::Build()
{
   if ( error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize);
      if ( state == false)
      {
	 error = ERROR_CANT_CREATE_NOTEBOOK;
      }
      else
      {
	 rdp_general = new RDPOptionsDialog_General (this);
	 rdp_general->SetGroupList(array_string_group_list);

	 rdp_general->Build();
	 rdp_display = new RDPOptionsDialog_Display (this);
	 rdp_display->Build();
	 rdp_share = new RDPOptionsDialog_Share (this);
	 rdp_share->Build();
	 rdp_program = new RDPOptionsDialog_Program (this);
	 rdp_program->Build();
	 rdp_perfomance = new RDPOptionsDialog_Performance (this);
	 rdp_perfomance->Build();
	 rdp_advanced = new RDPOptionsDialog_Advanced (this);
	 rdp_advanced->Build();

	 this->AddPage(rdp_general, _("General"));
	 this->AddPage(rdp_display, _("Display"));
	 this->AddPage(rdp_share, _("Share"));
	 this->AddPage(rdp_program, _("Program"));
	 this->AddPage(rdp_perfomance, _("Perfomance"));
	 this->AddPage(rdp_advanced, _("Advanced"));
	 //this->Show();
#ifdef __WXMSW__
	 rdp_advanced->Enable(false);
#endif

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
bool RDPOptionsDialog::SetOptions (const Options_HashMap *options) {
   bool state = false;

   if (options != NULL) {
      save_options = *options;
      //wxMessageBox(save_options[wxT("uniq_name")]);
      if (rdp_general != NULL)
	 rdp_general->SetOptions(options);
      if (rdp_display != NULL)
	 rdp_display->SetOptions(options);
      if (rdp_share != NULL)
	 rdp_share->SetOptions(options);
      if (rdp_program != NULL)
	 rdp_program->SetOptions(options);
      if (rdp_perfomance != NULL)
	 rdp_perfomance->SetOptions(options);
      if (rdp_advanced != NULL)
	 rdp_advanced->SetOptions(options);
   }
   return state;
}

void RDPOptionsDialog::SetGroupList(const wxArrayString &group_list)
{
   array_string_group_list = group_list;

//   if (rdp_general != NULL)
//   {
//	  rdp_general->SetGroupList(group_list);
//   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap RDPOptionsDialog::Get_Options ()
{
   //
   Options_HashMap options, temp_options;
   Options_HashMap::iterator it;



   if (rdp_general != NULL)
   {
      options.clear();
      options = rdp_general->Get_Options();
   }
   if ((rdp_display != NULL) &&
       (options.size() > 0))
   {
      temp_options.clear();
      temp_options = rdp_display->Get_Options();
      for( it = temp_options.begin(); it != temp_options.end(); ++it )
      {
	 wxString key = it->first, value = it->second;
	 options[key] = value;
      }
   }
   if ((rdp_share != NULL) &&
       (options.size() > 0))
   {
      temp_options.clear();
      temp_options = rdp_share->Get_Options();
      for( it = temp_options.begin(); it != temp_options.end(); ++it )
      {
	 wxString key = it->first, value = it->second;
	 options[key] = value;
      }
   }
   if ((rdp_program != NULL) &&
       (options.size() > 0))
   {
      temp_options.clear();
      temp_options = rdp_program->Get_Options();
      for( it = temp_options.begin(); it != temp_options.end(); ++it )
      {
	 wxString key = it->first, value = it->second;
	 options[key] = value;
      }
   }
   if ((rdp_perfomance != NULL) &&
       (options.size() > 0))
   {
      temp_options.clear();
      temp_options = rdp_perfomance->Get_Options();
      for( it = temp_options.begin(); it != temp_options.end(); ++it )
      {
	 wxString key = it->first, value = it->second;
	 options[key] = value;
      }
   }
   if ((rdp_advanced != NULL) &&
       (options.size() > 0))
   {
      temp_options.clear();
      temp_options = rdp_advanced->Get_Options();
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
   //	max_count = options.size();
   //	min_count = save_options.size();
   // }
   // else
   // {
   //	max_count = save_options.size();
   //	min_count = options.size();
   // }
   // if (options.count() > 0)
   // {
   //	for (int i = 0 ; i < max_count; i++)
   //	{
   // 	 option_found = false;
   // 	 for (int f = 0; f < max_count; f++)
   // 	 {
   // 		if ((options.count() > f) && (save_options.count() > i ))
   // 		{
   // 		   if (options.Item(f).Item(0).name == save_options.Item(i).Item(0).name)
   // 		   {
   // 		  option_found = true;
   // 		  break;
   // 		   }
   // 		}
   // 		else if ((options.Count() == f) && (save_options.Count() > i ))
   // 		{
   // 		   option_found = false;
   // 		}
   // 	 }
   // 	 if ((save_options.Count() > i ) &&
   // 		 (option_found == false))
   // 	 {
   // 		options_struct temp_struct;
   // 		GeneralOptionsArray temp_array;
   // 		temp_struct.type = wxT("s");
   // 		temp_struct.name = save_options.Item(i).Item(0).name;
   // 		temp_struct.value= save_options.Item(i).Item(0).value;
   // 		temp_array.Add (temp_struct);
   // 		options.Add(temp_array);
   // 		temp_struct.type.Clear();
   // 		temp_struct.name.Clear();
   // 		temp_struct.value.Clear();
   // 		option_found = false;
   // 	 }
   //	}
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
RDPOptionsDialog_General::RDPOptionsDialog_General(RDPOptionsDialog *parent_element)
{
   parent = NULL;

   m_flexgrid_sizer_1 = NULL;
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
   m_checkbox_attachtoconsole = NULL;

   m_static_groupname = NULL;
   m_combobox_groupname = NULL;

   m_static_connectionname = NULL;
   m_text_connectionname = NULL;

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
      error = ERROR_PARENT_IS_NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
RDPOptionsDialog_General::~RDPOptionsDialog_General()
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
int RDPOptionsDialog_General::Build()
{
   if ( error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
      if ( state == false)
      {
	 error = ERROR_CANT_CREATE_PANEL;
      }
      else
      {
	 m_boxsizer_1 = new wxBoxSizer(wxVERTICAL);
	 m_flexgrid_sizer_1 = new wxFlexGridSizer(0, 2, 0, 0);
	 m_flexgrid_sizer_1->AddGrowableCol(1);

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
//	 if (array_string_group_list.Count() > 0)
//	 {
	 m_combobox_groupname = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,
					       wxDefaultSize,array_string_group_list);
//	 }
//	 else
//	 {
//		m_combobox_groupname = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,
//						  wxDefaultSize,array_string_group_list);
//		m_combobox_groupname->SetValue(wxT("Main"));
//	 }
	 m_flexgrid_sizer_1->Add(m_static_connectionname, 1,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(m_text_connectionname, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(m_static_groupname, 1,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(m_combobox_groupname, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);


	 m_static_server = new wxStaticText(this, wxID_ANY, _("Server"),wxDefaultPosition,
					    wxDefaultSize);
	 m_text_server = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition,
					 wxDefaultSize, wxTE_PROCESS_ENTER);
	 m_checkbox_customport = new wxCheckBox( this, ID_RDP_CHECKBOX_CUSTOMPORT,
						 _("&Use custom port"), wxDefaultPosition,
						 wxDefaultSize );
	 m_flexgrid_sizer_2 = new wxFlexGridSizer(0, 3, 0, 0);
	 m_flexgrid_sizer_2->AddGrowableCol(1);
	 m_flexgrid_sizer_2->Add(m_static_server, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_text_server, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_checkbox_customport, 0,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);


	 m_static_username = new wxStaticText(this, wxID_ANY, _("Username"),wxDefaultPosition,
					      wxDefaultSize);
	 m_text_username = new wxTextCtrl( this, wxID_ANY, _("Administrator"), wxDefaultPosition,
					   wxDefaultSize, wxTE_PROCESS_ENTER);

	 m_static_port = new wxStaticText(this, wxID_ANY, _("Port"),wxDefaultPosition,wxDefaultSize);
	 m_text_port = new wxTextCtrl( this, wxID_ANY, wxT("3389"), wxDefaultPosition,
				       wxDefaultSize, wxTE_PROCESS_ENTER,
				       wxTextValidator(wxFILTER_NUMERIC) );

	 m_boxsizer_2 = new wxBoxSizer(wxHORIZONTAL);
	 m_flexgrid_sizer_2->Add(m_static_username, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_text_username, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_2->Add(m_static_port, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_2->Add(m_text_port, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_boxsizer_2, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	 m_static_password = new wxStaticText(this, wxID_ANY, _("Password"),wxDefaultPosition,
					      wxDefaultSize);
	 m_text_password = new wxTextCtrl( this, ID_RDP_TEXTCTRL_PASSWORD, wxT(""),wxDefaultPosition,
					   wxDefaultSize, wxTE_PASSWORD);
	 m_checkbox_autologon = new wxCheckBox( this, ID_RDP_CHECKBOX_AUTOLOGON, _("&Auto logon"),
						wxDefaultPosition,wxDefaultSize );

	 m_flexgrid_sizer_2->Add(m_static_password, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_text_password, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_checkbox_autologon, 1,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

	 m_static_domain = new wxStaticText(this, wxID_ANY, _("Domain"),wxDefaultPosition,
					    wxDefaultSize);
	 m_text_domain = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition,
					 wxDefaultSize, wxTE_PROCESS_ENTER);
	 m_checkbox_attachtoconsole = new wxCheckBox( this, wxID_ANY, _("&Attach to console"),
						      wxDefaultPosition,wxDefaultSize);

	 m_flexgrid_sizer_2->Add(m_static_domain, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_text_domain, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_2->Add(m_checkbox_attachtoconsole, 1,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);


	 m_text_port->Enable(false);
	 m_static_port->Enable(false);
	 m_combobox_groupname->SetValue(wxT("Main"));
	 //m_combobox_groupname->SetValue(wxT("Main1"));
	 m_combobox_groupname->SetSelection(0);



	 m_boxsizer_1->Add(m_flexgrid_sizer_1, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_flexgrid_sizer_2, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

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
void RDPOptionsDialog_General::get_group_list()
{
   // Benc bc;

   // RDPConn lrdpconn;
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


void RDPOptionsDialog_General::checkbox_customport()
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

void RDPOptionsDialog_General::checkbox_customport_func(wxCommandEvent &event)
{
   checkbox_customport();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPOptionsDialog_General::text_passwordentry_func(wxCommandEvent &event)
{
   wxString strTemp;
   switch(event.GetId())
   {
      case ID_RDP_TEXTCTRL_PASSWORD:
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

      case ID_RDP_CHECKBOX_AUTOLOGON:
	 if (!m_checkbox_autologon->IsChecked()) m_text_password->SetValue(wxT(""));
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
Options_HashMap RDPOptionsDialog_General::Get_Options()
{
   Options_HashMap local_options;
   ERRORS error = ERROR_OK;

   local_options.clear();

   if ((m_text_server != NULL) &&
       (m_text_server->GetValue().IsEmpty() == false))
   {
      local_options[wxT("hostname")] = m_text_server->GetValue();
   }
   else
   {
      error = ERROR_EMPTY_HOSTNAME;
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
   if(m_checkbox_attachtoconsole != NULL)
   {
      local_options[wxT("attach_to_console")] = wxString::Format(wxT("%i"),
								 m_checkbox_attachtoconsole->GetValue());
   }
   if (m_combobox_groupname != NULL) {
      local_options[wxT("group_name")] = m_combobox_groupname->GetValue();
   }
   if (m_text_connectionname != NULL)
   {
      local_options[wxT("connection_name")] = m_text_connectionname->GetValue();
   }
   if ( error != ERROR_OK)
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
bool RDPOptionsDialog_General::SetOptions(const Options_HashMap *all_options)
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
      m_checkbox_customport->SetValue(wxAtoi(local_options[wxT("custom_port")]));

   }
   if ((m_text_port != NULL))
   {
      m_text_port->SetValue(local_options[wxT("port")]);
   }
   if ((m_checkbox_attachtoconsole != NULL))
   {
      /*
	bool bVal;
	if (wxAtoi(local_options[wxT("attach_to_console")]) == 0)
	{
	bVal = false;
	}
	else
	{
	bVal = true;
	}

	m_checkbox_attachtoconsole->SetValue(bVal);
      */

      m_checkbox_attachtoconsole->SetValue(wxAtoi(local_options[wxT("attach_to_console")]));
      //m_checkbox_attachtoconsole->SetValue(local_options[wxT("attach_to_console")]);
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
   return state;
}

void RDPOptionsDialog_General::SetGroupList(const wxArrayString &group_list)
{
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
RDPOptionsDialog_Display::RDPOptionsDialog_Display(RDPOptionsDialog *parent_element)
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
      error = ERROR_PARENT_IS_NULL;
   }

}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
int RDPOptionsDialog_Display::Build()
{
   if (error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
      if ( state == false)
      {
	 error = ERROR_CANT_CREATE_PANEL;
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
	 //				   wxDefaultPosition, wxDefaultSize, 0, 0, 1, 0, wxDefaultValidator);
	 m_radiobutton_controlsize = new wxRadioButton(this, ID_RDP_CHK_CONTROLSIZE,
						       _("&Control size"),
						       wxDefaultPosition, wxDefaultSize, 0,
						       wxDefaultValidator);
	 m_radiobutton_controlsize->SetValue(true);
	 m_radiobutton_fullscreen = new wxRadioButton(this, ID_RDP_CHK_CONTROLSIZE,
						      _("&Full screen"),
						      wxDefaultPosition, wxDefaultSize, 0,
						      wxDefaultValidator);
	 m_radiobutton_customgeometry = new wxRadioButton(this, ID_RDP_CHK_CONTROLSIZE,
							  _("&Custom geometry"),
							  wxDefaultPosition, wxDefaultSize, 0,
							  wxDefaultValidator);
	 m_radiobutton_presetsize = new wxRadioButton(this, ID_RDP_CHK_CONTROLSIZE,
						      _("&Preset geometry"),
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
	 m_static_width = new wxStaticText(this, wxID_ANY, _("Screen width"),wxDefaultPosition,
					   wxDefaultSize);
	 m_text_width = new wxTextCtrl( this, wxID_ANY, wxT("800"), wxDefaultPosition, wxDefaultSize,
					wxTE_PROCESS_ENTER , wxTextValidator(wxFILTER_NUMERIC) );
	 m_static_height = new wxStaticText(this, wxID_ANY, _("Screen height"),wxDefaultPosition,
					    wxDefaultSize);
	 m_text_height = new wxTextCtrl( this, wxID_ANY, wxT("600"), wxDefaultPosition, wxDefaultSize,
					 wxTE_PROCESS_ENTER , wxTextValidator(wxFILTER_NUMERIC) );

	 m_static_colordepth = new wxStaticText(this, wxID_ANY, _("Color depth"),wxDefaultPosition,
						wxDefaultSize);
	 wxString Choises[] = {_("8-bit"),_("15-bit"),_("16-bit"),_("24-bit")};
	 m_choise_colordepth = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,wxDefaultSize,
					      4,Choises,wxCB_READONLY);
	 m_choise_colordepth->SetValue(Choises[2]);
	 m_choise_colordepth->SetSelection(2);

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

	 m_flexgrid_sizer_1->Add(m_static_colordepth, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(m_choise_colordepth, 0,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_2->Add(m_boxsizer_3, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_2->Add(m_flexgrid_sizer_1, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);

	 //m_boxsizer_4->Add(m_static_colordepth, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 //m_boxsizer_4->Add(m_choise_colordepth, 1,
	 //			 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	 m_checkbox_updatescreen = new wxCheckBox( this, wxID_ANY, _("&Force update screen"),
						   wxDefaultPosition, wxDefaultSize );
//		 m_checkbox_fullscreen = new wxCheckBox( this, , wxT(""),
//												 wxDefaultPosition, wxDefaultSize );
	 m_checkbox_smartsizing = new wxCheckBox( this, -1, _("&Smart sizing"), wxDefaultPosition,
						  wxDefaultSize );
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
#ifdef __WXMSW__
	 //sizer_main->Add(m_checkbox_smartsizing,0,wxALIGN_LEFT | wxALL,3);
#endif
#ifdef __WXGTK__
	 //sizer_main->Add(m_checkbox_updatescreen,0,wxALIGN_LEFT | wxALL,3);
#endif

#ifdef __WXMSW__
	 m_checkbox_updatescreen->Show(false);
#endif
#ifdef __WXGTK__
	 m_checkbox_smartsizing->Show(false);
//		 m_checkbox_controlsize->Show(true);
#endif
	 this->SetSizer(m_boxsizer_1);
	 m_boxsizer_1->Fit(this);
	 m_boxsizer_1->SetSizeHints(this);
	 CheckRDPDisplay();
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
wxArrayString RDPOptionsDialog_Display::GetResolutionsList()
{
   wxDisplay disp;
   wxArrayString resolutions;
#ifdef __WXMSW__
   wxArrayVideoModes avm = disp.GetModes();
   for (int i = 0; i < (int)avm.Count(); i++)
   {
      if (avm.Item(i).w < 800 || avm.Item(i).h < 600 || avm.Item(i).h > avm.Item(i).w)
	 continue;
      const wxString temp = wxString::Format(wxT("%i x %i"),avm.Item(i).w,avm.Item(i).h);
      BOOL flag = FALSE;
      for (int i = 0; i < (int)resolutions.Count(); i++)
      {
	 if (resolutions.Item(i) == temp) flag = TRUE;
      }
      if (!flag)	resolutions.Add(temp);

   }

   if (resolutions.Index(wxT("800 x 600")) == wxNOT_FOUND)
   {
      resolutions.Add(wxT("800 x 600"));
   }
   if (resolutions.Index(wxT("1024 x 768")) == wxNOT_FOUND)
   {
      resolutions.Add(wxT("1024 x 768"));
   }
   if (resolutions.Index(wxT("1152 x 864")) == wxNOT_FOUND)
   {
      resolutions.Add(wxT("1152 x 864"));
   }
   if (resolutions.Index(wxT("1280 x 960")) == wxNOT_FOUND)
   {
      resolutions.Add(wxT("1280 x 960"));
   }
   if (resolutions.Index(wxT("1280 x 1024")) == wxNOT_FOUND)
   {
      resolutions.Add(wxT("1280 x 1024"));
   }
   resolutions.Sort();
   resolutions.Shrink();
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
wxSize RDPOptionsDialog_Display::GetResolution(int i)
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

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
int RDPOptionsDialog_Display::GetDefaultResolution()
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
void RDPOptionsDialog_Display::CheckRDPDisplay()
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
void RDPOptionsDialog_Display::checkbox_customgeometry_func(wxCommandEvent& event)
{
   CheckRDPDisplay();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPOptionsDialog_Display::checkbox_fullscreen_func(wxCommandEvent &event)
{
   CheckRDPDisplay();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPOptionsDialog_Display::checkbox_controlsize_func(wxCommandEvent &event)
{
   CheckRDPDisplay();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
RDPOptionsDialog_Display::~RDPOptionsDialog_Display()
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


///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap RDPOptionsDialog_Display::Get_Options()
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

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
bool RDPOptionsDialog_Display::SetOptions(const Options_HashMap *all_options)
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
   CheckRDPDisplay();
   return state;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
RDPOptionsDialog_Share::RDPOptionsDialog_Share(RDPOptionsDialog *parent_element)
{
   m_boxsizer_1 = NULL;
   m_boxsizer_2 = NULL;
   m_boxsizer_3 = NULL;
   m_boxsizer_4 = NULL;
   m_boxsizer_5 = NULL;
   m_boxsizer_6 = NULL;
   m_boxsizer_7 = NULL;
   m_flexgrid_sizer_1 = NULL;

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


   error = 0;
   if (parent_element != NULL)
   {
      parent = parent_element;
   }
   else
   {
      error = ERROR_PARENT_IS_NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
int RDPOptionsDialog_Share::Build()
{
   if ( error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
      if ( state == false)
      {
	 error = ERROR_CANT_CREATE_PANEL;
      }
      else
      {
	 m_boxsizer_1 = new wxBoxSizer(wxVERTICAL);
	 m_flexgrid_sizer_1 = new wxFlexGridSizer(0, 2, 0, 0);
	 m_flexgrid_sizer_1->AddGrowableCol(0);
	 m_boxsizer_2 = new wxBoxSizer(wxVERTICAL);
	 m_boxsizer_3 = new wxBoxSizer(wxVERTICAL);
	 m_boxsizer_4 = new wxBoxSizer(wxVERTICAL);
	 m_boxsizer_5 = new wxBoxSizer(wxVERTICAL);
	 m_boxsizer_6 = new wxBoxSizer(wxVERTICAL);

	 m_static_sound  = new wxStaticText(this, wxID_ANY, _("Redirect sound"),wxDefaultPosition,
					    wxDefaultSize);
	 wxString ChoisesSound[] = {_("Bring to this computer"),_("Do not play"),
				    _("Leave at remote computer")};
	 m_choise_sound = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,
					 wxDefaultSize,3,ChoisesSound,wxCB_READONLY);
	 m_choise_sound->SetValue(ChoisesSound[0]);
	 m_choise_sound->SetSelection(0);

	 m_static_keyboard  = new wxStaticText(this, wxID_ANY, _("Using windows key combinations"),
					       wxDefaultPosition, wxDefaultSize);
	 wxString ChoisesKeyboard[] = {_("On local computer"),_("On remote computer"),
				       _("In full screen only")};
	 m_choise_keyboard = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,wxDefaultSize,
					    3,ChoisesKeyboard,wxCB_READONLY);
	 m_choise_keyboard->SetValue(ChoisesKeyboard[2]);
	 m_choise_keyboard->SetSelection(2);

	 m_checkbox_drives = new wxCheckBox( this, wxID_ANY, _("&Redirect drives"), wxDefaultPosition,
					     wxDefaultSize );
	 m_checkbox_printers = new wxCheckBox( this, wxID_ANY, _("&Redirect printers"),
					       wxDefaultPosition, wxDefaultSize );
	 m_checkbox_comports = new wxCheckBox( this, wxID_ANY, _("&Redirect COM ports"),
					       wxDefaultPosition, wxDefaultSize );
	 m_checkbox_smartcards = new wxCheckBox( this, wxID_ANY, _("&Redirect smartcards"),
						 wxDefaultPosition, wxDefaultSize );

	 m_static_linux_devices = new wxStaticText(this, wxID_ANY,
						   _("&Redirect devices for linux (see man for rdesktop)"),
						   wxDefaultPosition, wxDefaultSize);
	 m_text_linux_devices = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize,
					       wxTE_MULTILINE );
	 m_boxsizer_4->Add(m_static_sound, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_4->Add(m_choise_sound, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_2->Add(m_boxsizer_4, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_5->Add(m_static_keyboard, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_5->Add(m_choise_keyboard, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_2->Add(m_boxsizer_5, 0, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(m_boxsizer_2, 0,
				 wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(m_checkbox_drives, 1,
			   wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(m_checkbox_printers, 1,
			   wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(m_checkbox_comports, 1,
			   wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(m_checkbox_smartcards, 1,
			   wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_flexgrid_sizer_1->Add(m_boxsizer_3, 0,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_flexgrid_sizer_1, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_6->Add(m_static_linux_devices, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_6->Add(m_text_linux_devices, 1,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_boxsizer_6, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 this->SetSizer(m_boxsizer_1);
	 m_boxsizer_1->Fit(this);
	 m_boxsizer_1->SetSizeHints(this);

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
   }
   return error;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
RDPOptionsDialog_Share::~RDPOptionsDialog_Share()
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
   // if (m_boxsizer_4 != NULL) {delete m_boxsizer_4; m_boxsizer_4 = NULL;}
   // if (m_boxsizer_5 != NULL) {delete m_boxsizer_5; m_boxsizer_5 = NULL;}
   // if (m_boxsizer_2 != NULL) {delete m_boxsizer_2; m_boxsizer_2 = NULL;}
   // if (m_boxsizer_3 != NULL) {delete m_boxsizer_3; m_boxsizer_3 = NULL;}
   // if (m_flexgrid_sizer_1 != NULL) {delete m_flexgrid_sizer_1; m_flexgrid_sizer_1 = NULL;}
   // if (m_boxsizer_6 != NULL) {delete m_boxsizer_6; m_boxsizer_6 = NULL;}
   // if (m_boxsizer_7 != NULL) {delete m_boxsizer_7; m_boxsizer_7 = NULL;}
   // if (m_boxsizer_1 != NULL) {delete m_boxsizer_1; m_boxsizer_1 = NULL;}
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap RDPOptionsDialog_Share::Get_Options()
{
   Options_HashMap local_options;
   if(m_choise_sound != NULL)
   {
      local_options[wxT("sound_type")] = wxString::Format(wxT("%i"),
							  m_choise_sound->GetCurrentSelection());
   }
   if (m_choise_keyboard !=NULL)
   {
      local_options[wxT("keyboard_type")] = wxString::Format(wxT("%i"),
							     m_choise_keyboard->GetCurrentSelection());
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
   if (m_text_linux_devices !=NULL)
   {
      local_options[wxT("linux_devices")] = m_text_linux_devices->GetValue();
   }
   return local_options;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
bool RDPOptionsDialog_Share::SetOptions(const Options_HashMap *all_options)
{
   Options_HashMap local_options;
   local_options = *all_options;

   if (m_choise_sound != NULL)
   {
      m_choise_sound->SetSelection(wxAtoi(local_options[wxT("sound_type")]));
   }
   if (m_choise_keyboard !=NULL)
   {
      m_choise_keyboard->SetSelection(wxAtoi(local_options[wxT("keyboard_type")]));
   }
   if (m_checkbox_drives !=NULL)
   {
      m_checkbox_drives->SetValue(wxAtoi(local_options[wxT("share_drives")]));
   }
   if (m_checkbox_printers !=NULL)
   {
      m_checkbox_printers->SetValue(wxAtoi(local_options[wxT("share_printers")]));

   }
   if (m_checkbox_comports !=NULL)
   {
      m_checkbox_comports->SetValue(wxAtoi(local_options[wxT("share_com_ports")]));

   }
   if (m_checkbox_smartcards !=NULL)
   {
      m_checkbox_smartcards->SetValue(wxAtoi(local_options[wxT("share_smart_cards")]));
   }
   if (m_text_linux_devices !=NULL)
   {
      m_text_linux_devices->SetValue(local_options[wxT("linux_devices")]);
   }
   return true;
}



///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
RDPOptionsDialog_Program::RDPOptionsDialog_Program(RDPOptionsDialog *parent_element)
{
   m_boxsizer_1 = NULL;

   m_static_program = NULL;
   m_text_program = NULL;

   m_static_workdir = NULL;
   m_text_workdir = NULL;

   m_checkbox_program = NULL;
   m_checkbox_maximized = NULL;

   error = 0;
   if (parent_element != NULL)
   {
      parent = parent_element;
   }
   else
   {
      error = ERROR_PARENT_IS_NULL;
   }

}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
int RDPOptionsDialog_Program::Build()
{
   if ( error == 0 )
   {
      bool state = false;
      state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
      if ( state == false)
      {
	 error = ERROR_CANT_CREATE_PANEL;
      }
      else
      {
	 m_boxsizer_1= new wxBoxSizer(wxVERTICAL);
	 m_static_program  = new wxStaticText(this, wxID_ANY, _("Program path"),
					      wxDefaultPosition, wxDefaultSize);
	 m_text_program = new wxTextCtrl( this, wxID_ANY, wxT(""),  wxDefaultPosition, wxDefaultSize,
					  wxTE_PROCESS_ENTER);
	 m_static_workdir  = new wxStaticText(this, wxID_ANY, _("Working dir"), wxDefaultPosition, wxDefaultSize);
	 m_text_workdir = new wxTextCtrl( this, wxID_ANY, wxT(""),  wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	 m_checkbox_program = new wxCheckBox( this, ID_RDP_CHECKBOX_USE_PROGRAM, _("&Start the following program:"),  wxDefaultPosition, wxDefaultSize );
	 m_checkbox_maximized = new wxCheckBox( this, wxID_ANY, _("&Start maximized"),  wxDefaultPosition, wxDefaultSize);


	 m_boxsizer_1->Add(m_checkbox_program, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_checkbox_maximized, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_static_program, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_text_program, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_static_workdir, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_text_workdir, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

	 this->SetSizer( m_boxsizer_1);
	 m_boxsizer_1->Fit(this);
	 m_boxsizer_1->SetSizeHints(this);

	 m_checkbox_maximized->Enable(false);
	 m_static_program->Enable(false);
	 m_text_program->Enable(false);
	 m_static_workdir->Enable(false);
	 m_text_workdir->Enable(false);
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
RDPOptionsDialog_Program::~RDPOptionsDialog_Program()
{
   if (m_static_program) {delete m_static_program; m_static_program = NULL;}
   if (m_text_program) {delete m_text_program; m_text_program = NULL;}

   if (m_static_workdir) {delete m_static_workdir; m_static_workdir = NULL;}
   if (m_text_workdir) {delete m_text_workdir; m_text_workdir = NULL;}

   if (m_checkbox_program) {delete m_checkbox_program; m_checkbox_program = NULL;}
   if (m_checkbox_maximized) {delete m_checkbox_maximized; m_checkbox_maximized = NULL;}
   // if (m_boxsizer_1 != NULL) {delete m_boxsizer_1; m_boxsizer_1 = NULL;}
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap RDPOptionsDialog_Program::Get_Options()
{
   Options_HashMap local_options;

   if (m_text_program)
   {
      local_options[wxT("shell")] = m_text_program->GetValue();
   }
   if (m_text_workdir)
   {
      local_options[wxT("directory")] = m_text_workdir->GetValue();
   }
   if (m_checkbox_program)
   {
      local_options[wxT("use_program")] = wxString::Format(wxT("%i"),
							   m_checkbox_program->GetValue());
   }
   if (m_checkbox_maximized)
   {
      local_options[wxT("program_maximized")] = wxString::Format(wxT("%i"),
								 m_checkbox_maximized->GetValue());
   }
   return local_options;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
bool RDPOptionsDialog_Program::SetOptions(const Options_HashMap *all_options)
{
   Options_HashMap local_options;
   local_options = *all_options;
   if (m_text_program)
   {
      m_text_program->SetValue(local_options[wxT("shell")]);
   }
   if (m_text_workdir)
   {
      m_text_workdir->SetValue(local_options[wxT("directory")]);
   }
   if (m_checkbox_program)
   {
      m_checkbox_program->SetValue(wxAtoi(local_options[wxT("use_program")]));
   }
   if (m_checkbox_maximized)
   {
      m_checkbox_maximized->SetValue(wxAtoi(local_options[wxT("program_maximized")]));
   }
   checkbox_program();
   return true;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPOptionsDialog_Program::checkbox_program()
{
   if(!m_checkbox_program->IsChecked() )
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

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPOptionsDialog_Program::checkbox_program_func(wxCommandEvent &event)
{
   checkbox_program();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
RDPOptionsDialog_Performance::RDPOptionsDialog_Performance(RDPOptionsDialog *parent_element)
{
   m_boxsizer_1 = NULL;
   m_boxsizer_2 = NULL;
   m_boxsizer_3 = NULL;
   m_boxsizer_4 = NULL;
   m_boxsizer_5 = NULL;
   m_boxsizer_6 = NULL;

   m_static_speed = NULL;
   m_choise_speed = NULL;

   m_static_performance = NULL;

   m_checkbox_enablewallpaper = NULL;
   m_checkbox_fullwindowdrag = NULL;
   m_checkbox_animation = NULL;
   m_checkbox_themes = NULL;
   m_checkbox_bitmapcaching = NULL;

   error = 0;
   if (parent_element != NULL)
   {
      parent = parent_element;
   }
   else
   {
      error = ERROR_PARENT_IS_NULL;
   }

}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
int RDPOptionsDialog_Performance::Build() {
   if (error == 0) {
      const bool state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
      if (state == false) {
	 error = ERROR_CANT_CREATE_PANEL;
      } else {
	 m_boxsizer_1 = new wxBoxSizer(wxVERTICAL);
	 m_boxsizer_2 = new wxBoxSizer(wxHORIZONTAL);
	 m_boxsizer_3 = new wxBoxSizer(wxHORIZONTAL);
	 m_boxsizer_4 = new wxBoxSizer(wxHORIZONTAL);
	 m_boxsizer_5 = new wxBoxSizer(wxHORIZONTAL);
	 m_boxsizer_6 = new wxBoxSizer(wxHORIZONTAL);

     m_static_speed = new wxStaticText(this, wxID_ANY, _("Connection speed"),
					    wxDefaultPosition, wxDefaultSize);
	 // TODO: move to array
	 ChoisesSpeed[0] = _("Modem 28.8 Kbps");
	 ChoisesSpeed[1] = _("Modem 56 Kbps");
	 ChoisesSpeed[2] = _("Broadband 128Kbps - 1.5 Mbps");
	 ChoisesSpeed[3] = _("LAN 10 Mbps or higher");
	 ChoisesSpeed[4] = _("Custom");
	 m_choise_speed = new wxComboBox(this,
					 ID_RDP_COMBOBOX_SPEED,wxT(""),
					 wxDefaultPosition,
					 wxSize(220,-1),
					 5,
					 ChoisesSpeed,wxCB_READONLY) ;
	 m_choise_speed->SetValue(ChoisesSpeed[1]);
	 m_choise_speed->SetSelection(1);
	 m_static_performance = new wxStaticText(this, wxID_ANY, _("Performance settings"));
	    wxFont m_static_performance_font;
	    m_static_performance_font.SetWeight(wxFONTWEIGHT_BOLD);
	    m_static_performance->SetFont(m_static_performance_font);
	 m_checkbox_bitmapcaching = new wxCheckBox(this, wxID_ANY, _("&Bitmap caching"));
	 m_checkbox_bitmapcaching->SetValue(true);
	 m_checkbox_enablewallpaper = new wxCheckBox( this, wxID_ANY, _("&Desktop wallpaper"),
						      wxDefaultPosition, wxDefaultSize );
	 m_checkbox_fullwindowdrag = new wxCheckBox( this, wxID_ANY,
						     _("&Show contents of window while dragging"),
						     wxDefaultPosition, wxDefaultSize);
	 m_checkbox_animation = new wxCheckBox( this, wxID_ANY, _("&Animation"),
						wxDefaultPosition, wxDefaultSize );
	 m_checkbox_themes = new wxCheckBox( this, wxID_ANY, _("&Themes"), wxDefaultPosition,
					     wxDefaultSize );
	 m_checkbox_themes->SetValue(true);

	 m_boxsizer_2->Add(m_static_speed, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	    m_boxsizer_2->Add(m_choise_speed, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	    m_boxsizer_2->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_boxsizer_2, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(m_static_performance, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_3->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_boxsizer_3, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_4->Add(m_checkbox_fullwindowdrag, 1,
			   wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_5->Add(m_checkbox_bitmapcaching, 1,
			   wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_5->Add(m_checkbox_themes, 1,
			   wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_6->Add(m_checkbox_enablewallpaper, 1,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_6->Add(m_checkbox_animation, 1,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_boxsizer_4, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_boxsizer_5, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_boxsizer_6, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 this->SetSizer(m_boxsizer_1);
	 m_boxsizer_1->Fit(this);
	 m_boxsizer_1->SetSizeHints(this);
	 choise_speed();
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
RDPOptionsDialog_Performance::~RDPOptionsDialog_Performance()
{
	if (m_static_speed) {delete m_static_speed; m_static_speed = NULL;}
	if (m_choise_speed) {delete m_choise_speed; m_choise_speed = NULL;}

	if (m_static_performance) {delete m_static_performance; m_static_performance = NULL;}

	if (m_checkbox_enablewallpaper)
	{delete m_checkbox_enablewallpaper; m_checkbox_enablewallpaper = NULL;}
	if (m_checkbox_fullwindowdrag)
	{delete m_checkbox_fullwindowdrag; m_checkbox_fullwindowdrag = NULL;}
	if (m_checkbox_animation) {delete m_checkbox_animation; m_checkbox_animation = NULL;}
	if (m_checkbox_themes) {delete m_checkbox_themes; m_checkbox_themes = NULL;}
	if (m_checkbox_bitmapcaching) {delete m_checkbox_bitmapcaching; m_checkbox_bitmapcaching = NULL;}
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap RDPOptionsDialog_Performance::Get_Options()
{
	Options_HashMap local_options;
	if (m_choise_speed)
	{
		local_options[wxT("bandwidth")] = wxString::Format(wxT("%i"),
														   m_choise_speed->GetCurrentSelection());
	}
	if (m_checkbox_enablewallpaper)
	{
		local_options[wxT("enable_wallpaper")] = wxString::Format(wxT("%i"),
																  m_checkbox_enablewallpaper->GetValue());
	}
	if (m_checkbox_fullwindowdrag)
	{
		local_options[wxT("enable_full_window_drag")] = wxString::Format(wxT("%i"),
																		 m_checkbox_fullwindowdrag->GetValue());
	}
	if (m_checkbox_animation)
	{
		local_options[wxT("enable_animation")] = wxString::Format(wxT("%i"),
																  m_checkbox_animation->GetValue());
	}
	if (m_checkbox_themes)
	{
		local_options[wxT("enable_themes")] = wxString::Format(wxT("%i"),
															   m_checkbox_themes->GetValue());
	}
	if (m_checkbox_bitmapcaching)
	{
		local_options[wxT("enable_bitmap_caching")] = wxString::Format(wxT("%i"),
																	   m_checkbox_bitmapcaching->GetValue());
	}
	return local_options;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
bool RDPOptionsDialog_Performance::SetOptions(const Options_HashMap *all_options)
{
	Options_HashMap local_options;
	local_options = *all_options;

	if (m_choise_speed)
	{
		m_choise_speed->SetSelection(wxAtoi(local_options[wxT("bandwidth")]));
	}
	if (m_checkbox_enablewallpaper)
	{
		m_checkbox_enablewallpaper->SetValue(wxAtoi(local_options[wxT("enable_wallpaper")]));
	}
	if (m_checkbox_fullwindowdrag)
	{
		m_checkbox_fullwindowdrag->SetValue(wxAtoi(local_options[wxT("enable_full_window_drag")]));
	}
	if (m_checkbox_animation)
	{
		m_checkbox_animation->SetValue(wxAtoi(local_options[wxT("enable_animation")]));
	}
	if (m_checkbox_themes)
	{
		m_checkbox_themes->SetValue(wxAtoi(local_options[wxT("enable_themes")]));
	}
	if (m_checkbox_bitmapcaching)
	{
		m_checkbox_bitmapcaching->SetValue(wxAtoi(local_options[wxT("enable_bitmap_caching")]));
	}

	choise_speed();
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPOptionsDialog_Performance::choise_speed() {
	if (!m_choise_speed) return;

	const int selection = m_choise_speed->GetSelection();

	m_checkbox_enablewallpaper->Disable();
	m_checkbox_fullwindowdrag->Disable();
	m_checkbox_animation->Disable();
	m_checkbox_themes->Disable();
	m_checkbox_bitmapcaching->Disable();

	switch(selection) {
		case 0: m_checkbox_enablewallpaper->SetValue(false);
			m_checkbox_fullwindowdrag->SetValue(false);
			m_checkbox_animation->SetValue(false);
			m_checkbox_themes->SetValue(false);
			m_checkbox_bitmapcaching->SetValue(true);
			break;
		case 1: m_checkbox_enablewallpaper->SetValue(false);
			m_checkbox_fullwindowdrag->SetValue(false);
			m_checkbox_animation->SetValue(false);
			m_checkbox_themes->SetValue(true);
			m_checkbox_bitmapcaching->SetValue(true);
			break;
		case 2: m_checkbox_enablewallpaper->SetValue(false);
			m_checkbox_fullwindowdrag->SetValue(true);
			m_checkbox_animation->SetValue(true);
			m_checkbox_themes->SetValue(true);
			m_checkbox_bitmapcaching->SetValue(true);
			break;
		case 3: m_checkbox_enablewallpaper->SetValue(true);
			m_checkbox_fullwindowdrag->SetValue(true);
			m_checkbox_animation->SetValue(true);
			m_checkbox_themes->SetValue(true);
			m_checkbox_bitmapcaching->SetValue(true);
			break;
		case 4:	m_checkbox_enablewallpaper->Enable();
			m_checkbox_fullwindowdrag->Enable();
			m_checkbox_animation->Enable();
			m_checkbox_themes->Enable();
			m_checkbox_bitmapcaching->Enable();
			break;
	}

	// if (m_choise_speed->GetValue() == this->ChoisesSpeed[4])
	// {
	//	m_checkbox_enablewallpaper->SetValue(false);
	//	m_checkbox_fullwindowdrag->SetValue(false);
	//	m_checkbox_animation->SetValue(false);
	//	m_checkbox_themes->SetValue(true);
	//	m_checkbox_bitmapcaching->SetValue(true);
	//	return;
	// }

	// m_checkbox_enablewallpaper->SetValue(false);
	// m_checkbox_fullwindowdrag->SetValue(false);
	// m_checkbox_animation->SetValue(false);
	// m_checkbox_themes->SetValue(true);
	// m_checkbox_bitmapcaching->SetValue(true);
//   std::cout << "choise_speed_func" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPOptionsDialog_Performance::choise_speed_func(wxCommandEvent &event)
{
	choise_speed();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
RDPOptionsDialog_Advanced::RDPOptionsDialog_Advanced(RDPOptionsDialog *parent_element)
{
	m_boxsizer_1 = NULL;
	m_boxsizer_2 = NULL;
	m_boxsizer_3 = NULL;
	m_boxsizer_4 = NULL;
	m_gridsizer_1 = NULL;

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

	error = 0;
	if (parent_element != NULL)
		parent = parent_element;
	else
		error = ERROR_PARENT_IS_NULL;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
int RDPOptionsDialog_Advanced::Build() {
	if ( error == 0 ) {
		const bool state = Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
		if ( state == false) {
			error = ERROR_CANT_CREATE_PANEL;
		} else {
			m_boxsizer_1 = new wxBoxSizer(wxVERTICAL);
			m_boxsizer_2 = new wxBoxSizer(wxHORIZONTAL);
			m_boxsizer_3 = new wxBoxSizer(wxHORIZONTAL);
			m_boxsizer_4 = new wxBoxSizer(wxHORIZONTAL);
			m_gridsizer_1 = new wxGridSizer(0, 2, 0, 0);

			m_static_rdpversion = new wxStaticText(this, wxID_ANY, _("RDP version"),
												   wxDefaultPosition, wxDefaultSize);
			wxString ChoiseVersion[] = {_("Use RDP 4"),_("Use RDP 5"),_("Use RDP 6")};
			m_choise_rdpversion = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,
												 wxDefaultSize,2,ChoiseVersion,wxCB_READONLY) ;
			m_choise_rdpversion->SetValue(ChoiseVersion[1]);
			m_choise_rdpversion->SetSelection(1);
			m_checkbox_french = new wxCheckBox( this, wxID_ANY,
												_("&Enable encrypt for French version"),
												wxDefaultPosition, wxDefaultSize );
			m_checkbox_encryption_enable = new wxCheckBox( this, wxID_ANY, _("&Enable encryption"),
														   wxDefaultPosition, wxDefaultSize );
			m_checkbox_backingstore = new wxCheckBox( this, wxID_ANY, _("&Use BackingStore"),
													  wxDefaultPosition, wxDefaultSize )	;
			m_checkbox_usemouse = new wxCheckBox( this, wxID_ANY, _("&Use mouse"), wxDefaultPosition,
												  wxDefaultSize );
			m_checkbox_privatecolormap = new wxCheckBox( this, wxID_ANY, _("&Use private color map"),
														 wxDefaultPosition, wxDefaultSize );
			m_checkbox_numlock = new wxCheckBox(this,wxID_ANY,_("&Num Lock sync"),wxDefaultPosition,
												wxDefaultSize);
			m_checkbox_enablecompress = new wxCheckBox(this,wxID_ANY,_("&Enable compress"),
													   wxDefaultPosition, wxDefaultSize);
			m_static_keyboard_map = new wxStaticText(this, wxID_ANY, _("Keyboard map"),
													 wxDefaultPosition, wxDefaultSize);
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
					if ((temp_str != wxT("en-us")) &&
						(temp_str != wxT("common"))&&
						(temp_str != wxT("modifiers")))
					{
						list_keyboard_map.Add(temp_str);
					}
					check = temp.GetNext(&temp_str);
				}
			}
			else
			{
				list_keyboard_map.Add( wxT("en-us"));
				list_keyboard_map.Add( wxT("ru-ru"));
			}

			m_combobox_keyboard_map = new wxComboBox(this, wxID_ANY,wxT(""),wxDefaultPosition, wxDefaultSize,
													 list_keyboard_map,wxCB_READONLY,
													 wxDefaultValidator,wxT(""));
			m_combobox_keyboard_map->SetValue(list_keyboard_map[0]);
			m_combobox_keyboard_map->SetSelection(0);
#endif

#ifdef __WXMSW__
			wxString list_keyboard_map[] = {wxT("en"),wxT("ru")};
			m_combobox_keyboard_map = new wxComboBox(this,wxID_ANY, wxT("") , wxDefaultPosition,
													 wxDefaultSize,2,list_keyboard_map,wxCB_READONLY);
			m_combobox_keyboard_map->SetValue(ChoiseVersion[1]);
			m_combobox_keyboard_map->SetSelection(1);
#endif
			m_boxsizer_2->Add(m_static_rdpversion, 1,
							  wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_2->Add(m_choise_rdpversion, 1,
							  wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_1->Add(m_boxsizer_2, 0,
							  wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_3->Add(m_checkbox_french, 1,
							  wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_1->Add(m_boxsizer_3, 0,
							  wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_gridsizer_1->Add(m_checkbox_encryption_enable, 1,
							   wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_gridsizer_1->Add(m_checkbox_backingstore, 1,
							   wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_gridsizer_1->Add(m_checkbox_usemouse, 1,
							   wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_gridsizer_1->Add(m_checkbox_privatecolormap, 1,
							   wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_gridsizer_1->Add(m_checkbox_numlock, 1,
							   wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_gridsizer_1->Add(m_checkbox_enablecompress, 1,
							   wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_1->Add(m_gridsizer_1, 0,
							  wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_4->Add(m_static_keyboard_map, 0,
							  wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_4->Add(m_combobox_keyboard_map, 0,
							  wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_4->Add(0,0,1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_1->Add(m_boxsizer_4, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			m_boxsizer_1->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
			this->SetSizer(m_boxsizer_1);
			m_boxsizer_1->Fit(this);
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
RDPOptionsDialog_Advanced::~RDPOptionsDialog_Advanced()
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
	// if (m_boxsizer_1 != NULL) {delete m_boxsizer_1; m_boxsizer_1 = NULL;}
	// if (m_boxsizer_2 != NULL) {delete m_boxsizer_2; m_boxsizer_2 = NULL;}
	// if (m_boxsizer_3 != NULL) {delete m_boxsizer_3; m_boxsizer_3 = NULL;}
	// if (m_boxsizer_4 != NULL) {delete m_boxsizer_4; m_boxsizer_4 = NULL;}
	// if (m_gridsizer_1 != NULL) {delete m_gridsizer_1; m_gridsizer_1  = NULL;}
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap RDPOptionsDialog_Advanced::Get_Options()
{
	Options_HashMap local_options;
	if (m_choise_rdpversion != NULL)
	{
		local_options[wxT("use_rdp_version")] = wxString::Format(wxT("%i"),
																 m_choise_rdpversion->GetCurrentSelection());
	}
	if (m_checkbox_french != NULL)
	{
		local_options[wxT("encription_enable_french")] = wxString::Format(wxT("%i"),
																		  m_checkbox_french->GetValue());
	}
	if (m_checkbox_encryption_enable != NULL)
	{
		local_options[wxT("encription_enable_new")] = wxString::Format(wxT("%i"),
																	   m_checkbox_encryption_enable->GetValue());
	}
	if (m_checkbox_backingstore != NULL)
	{
		local_options[wxT("backing_store")] = wxString::Format(wxT("%i"),
															   m_checkbox_backingstore->GetValue());
	}
	if (m_checkbox_usemouse != NULL)
	{
		local_options[wxT("send_mouse_event")] = wxString::Format(wxT("%i"),
																  m_checkbox_usemouse->GetValue());
	}
	if (m_checkbox_privatecolormap != NULL)
	{
		local_options[wxT("private_color_map")] = wxString::Format(wxT("%i"),
																   m_checkbox_privatecolormap->GetValue());
	}
	if (m_checkbox_numlock != NULL)
	{
		local_options[wxT("numlock_sync")] = wxString::Format(wxT("%i"),
															  m_checkbox_numlock->GetValue());
	}
	if (m_checkbox_enablecompress != NULL)
	{
		local_options[wxT("enable_compres")] = wxString::Format(wxT("%i"),
																m_checkbox_enablecompress->GetValue());
	}
	if (m_combobox_keyboard_map)
	{
		local_options[wxT("keyboard_map")] = wxString::Format(wxT("%i"),
															  m_combobox_keyboard_map->GetCurrentSelection());
	}
//	// result += GetParamInt (wxT("connection_count"), (int)(rdpc.dwConnectionCount));

//	// // ICA Citrix
//	// result += GetParamStr (wxT("ica_server_ini"), (rdpc.server_ini));
//	// result += GetParamStr (wxT("ica_client_ini"), (rdpc.client_ini));
//	// result += GetParamInt (wxT("ICASound"), (int)(rdpc.bIcaSound));
//	// result += GetParamInt (wxT("ICASoundType"), (int)(rdpc.bIcaSoundType));
//	// result += GetParamInt (wxT("ICAEncryption"), (int)(rdpc.bIcaEncryption));
//	// result += GetParamInt (wxT("ICAEncryptionType"), (int)(rdpc.ica_encryption));

//	// result += GetParamStr (wxT("ica_connection_file"), (rdpc.ica_file));
//	// result += GetParamInt (wxT("bUseApplication"), (int)(rdpc.bUseApplication));
//	// result += GetParamStr (wxT("ICAApplication"), (rdpc.IcaApplication));

//	// result += GetParamInt (wxT("bProxyType"), (int)(rdpc.bProxyType));
//	// result += GetParamStr (wxT("ProxyAddr"), (rdpc.ProxyAddr));
//	// result += GetParamStr (wxT("ProxyPort"), (rdpc.ProxyPort));
//	// result += GetParamStr (wxT("ProxyUserId"), (rdpc.ProxyUserId));
//	// result += GetParamStr (wxT("ProxyPassword"),
//	// (rdpc.ProxyPassword));

   return local_options;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
bool RDPOptionsDialog_Advanced::SetOptions(const Options_HashMap *all_options) {
	Options_HashMap local_options = *all_options;

	m_choise_rdpversion->SetSelection(wxAtoi(local_options[wxT("use_rdp_version")]));

	m_checkbox_french->SetValue(wxAtoi(local_options[wxT("encription_enable_french")]));

	m_checkbox_encryption_enable->SetValue(wxAtoi(local_options[wxT("encription_enable_new")]));

	m_checkbox_backingstore->SetValue(wxAtoi(local_options[wxT("backing_store")]));

	m_checkbox_usemouse->SetValue(wxAtoi(local_options[wxT("send_mouse_event")]));

	m_checkbox_privatecolormap->SetValue(wxAtoi(local_options[wxT("private_color_map")]));

	m_checkbox_numlock->SetValue(wxAtoi(local_options[wxT("numlock_sync")]));

	m_checkbox_enablecompress->SetValue(wxAtoi(local_options[wxT("enable_compres")]));

	m_combobox_keyboard_map->SetSelection(wxAtoi(local_options[wxT("keyboard_map")]));

	return true;
}


