///////////////////////////////////////////////////////////////////////////////
// File name:   rdp_dialogs.cpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-21 20:44:51" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#include "rdp_dialogs.hpp"

//#include "main_window.hpp"

//BEGIN_EVENT_TABLE(RDPDialog, wxPanel)


BEGIN_EVENT_TABLE(RDPDialog, wxDialog)
//EVT_BUTTON (ID_BUTTON_OK, RDPDialog::button_ok_func)
EVT_BUTTON    (ID_BUTTON_CANCEL,        RDPDialog::button_cancel_func)
EVT_BUTTON    (ID_BUTTON_SAVE,        RDPDialog::button_save_func)
EVT_COMBOBOX (ID_COMBOBOX_PROTO_CHANGE, RDPDialog::change_proto_func)
//EVT_CHAR_HOOK(RDPDialog::dialog_hotkeys)
END_EVENT_TABLE()



///////////////////////////////////////////////////////////////////////////////
//! \brief Constructor for Dialog connection configurations
//! \param wxWindow* parent - parant element
///////////////////////////////////////////////////////////////////////////////
RDPDialog::RDPDialog(wxWindow* parent) : 
wxDialog(parent, wxID_ANY, _T("Connection settings"),
	 wxDefaultPosition, wxSize(650, 542))
{
   main_frame = (Main_Frame *)parent;
   Options_HashMap local_options;
   bool found_duplicate;
   

 
   //new_uniq_name = not_change;

   // notebook = NULL;
   // m_rdpgeneral = NULL;
   // m_rdpdisplay = NULL;
   // m_rdpshare = NULL;
   // m_rdpprogram = NULL;
   // m_rdpperformance = NULL;

   //m_rdpadvanced = NULL;
   m_button_ok = NULL;
   m_button_cancel = NULL;
   m_button_save = NULL;

   sizer_main = NULL;
   sizer_btn = NULL;

   m_static_protocol = NULL;
   m_combobox_protocol = NULL;
   m_line_buttons = NULL;
   m_line_protocol = NULL;

   sizer_protocol = new wxBoxSizer(wxHORIZONTAL);

   m_static_protocol = new wxStaticText(this,-1,wxT("Protocol     "));
   wxString arr_proto[] = {wxT("RDP Protocol"),wxT("ICA Protocol")};
   m_combobox_protocol = new wxComboBox(this,ID_COMBOBOX_PROTO_CHANGE,wxT(""),wxDefaultPosition,
					wxSize(120,-1) , 2 , arr_proto,wxCB_READONLY);
   m_combobox_protocol->SetSelection(0);
   m_combobox_protocol->SetValue(arr_proto[0]); 

   sizer_protocol->Add(m_static_protocol,0,wxALL,7);
   sizer_protocol->Add(m_combobox_protocol,0,wxALL | wxEXPAND,4);

   m_line_protocol = new wxStaticLine(this,wxID_ANY,wxDefaultPosition, wxSize(-1,2) ,wxLI_HORIZONTAL);

   // if (prdpc == NULL)
   // {
   //    wxMessageBox(_("Error data"),_("Error"),wxICON_ERROR);
   //    this->EndModal(0); 
   // }
   for (int i = 0; i < main_frame->all_connection_records.Count(); i++)
   {
      local_options = main_frame->all_connection_records.Item(i);
      for (int f = 0 ; f < group_list.Count() ; f++)
      {
   	 if ( group_list.Item(f) == local_options[wxT("group_name")])
   	 {
   	    found_duplicate = true;
   	 }
      }
      if (found_duplicate == false)
      {
//   	 wxMessageBox(local_options[wxT("group_name")]);
   	 group_list.Add(local_options[wxT("group_name")]);
   	 found_duplicate = false;
      }

      //group_list.Add(local_options[wxT("group_name")]);
   }
   group_list.Sort();
   group_list.Shrink();

   rdp_options_dialog = new RDPOptionsDialog (this);
   rdp_options_dialog->Show(false);
   rdp_options_dialog->Set_Group_List(group_list);
   rdp_options_dialog->Build();

   ica_options_dialog = new ICAOptionsDialog (this);
   ica_options_dialog->Show(false);
   ica_options_dialog->Set_Group_List(group_list);
   ica_options_dialog->Build();
   
   //wxPanel *testpanel = new wxPanel(testdialog, wxID_ANY);
   //testdialog->Show();

   //prdpconn = prdpc;
   //BaseFile = frameBaseFile;
//   ica_options_dialog = NULL;
   
   sizer_main = new wxBoxSizer(wxVERTICAL);
   //notebook = new wxNotebook(this, this->GetId(), wxDefaultPosition, wxDefaultSize, wxNB_TOP);

   sizer_main->Add(sizer_protocol,0, wxALL | wxEXPAND,0 );
   sizer_main->Add(m_line_protocol,0,wxALL | wxALIGN_CENTER | wxEXPAND,5);
   sizer_main->Add(ica_options_dialog,1,wxALIGN_CENTER | wxALL |wxEXPAND, 5 );
   sizer_main->Add(rdp_options_dialog,1,wxALIGN_CENTER | wxALL | wxEXPAND, 5 );

   // m_rdpgeneral = new RDPGeneral(notebook,wxDefaultPosition,wxDefaultSize,rdp_base);
   // notebook->AddPage(m_rdpgeneral,wxT("General"), true);

   // m_rdpdisplay = new RDPDisplay(notebook,wxDefaultPosition,wxDefaultSize);
   // notebook->AddPage(m_rdpdisplay,wxT("Display"), true);

   // m_rdpshare = new RDPShare(notebook,wxDefaultPosition,wxDefaultSize);
   // notebook->AddPage(m_rdpshare,wxT("Share"), true);

   // m_rdpprogram = new RDPProgram(notebook,wxDefaultPosition,wxDefaultSize);
   // notebook->AddPage(m_rdpprogram,wxT("Program"), true);

   // m_rdpperformance = new RDPPerformance(notebook,wxDefaultPosition,wxDefaultSize);
   // notebook->AddPage(m_rdpperformance,wxT("Performance"), true);

   // m_rdpadvanced = new RDPAdvanced(notebook,wxDefaultPosition,wxDefaultSize);
   // notebook->AddPage(m_rdpadvanced,wxT("Advanced"), true);
	
   // m_icaadvanced = new ICAAdvanced(notebook,wxDefaultPosition,wxDefaultSize);
   // notebook->AddPage(m_icaadvanced,wxT("ICA"), true);

   // m_proxysettings = new ProxySettings(notebook,wxDefaultPosition,wxDefaultSize);
   // notebook->AddPage(m_proxysettings,wxT("Proxy"), true);

#ifdef __WXMSW__
//   m_rdpadvanced->Enable(false);
#endif
   //notebook->SetSelection(0);

   sizer_btn = new wxBoxSizer(wxHORIZONTAL);

   m_line_buttons = new wxStaticLine(this,wxID_ANY,wxDefaultPosition, wxSize(-1,2) ,wxLI_HORIZONTAL);

   if (new_uniq_name)
   {
      m_button_ok = new wxButton(this,ID_BUTTON_OK, wxT("Connect"), wxDefaultPosition, wxSize(100,-1) );
      sizer_btn->Add(m_button_ok,0,wxALL,5);
   }
   m_button_cancel = new wxButton(this,ID_BUTTON_CANCEL, wxT("Cancel"), wxDefaultPosition, wxSize(100,-1) );
   m_button_save = new wxButton(this,ID_BUTTON_SAVE, wxT("Save"), wxDefaultPosition, wxSize(100,-1));
   sizer_btn->Add(m_button_cancel,0,wxALL,5);
   sizer_btn->Add(m_button_save,0,wxALL,5);

   sizer_main->Add(m_line_buttons,0,wxALL | wxALIGN_CENTER | wxEXPAND,5);
   sizer_main->Add(sizer_btn,0,wxALIGN_CENTER | wxALL|wxBOTTOM /*| wxEXPAND*/,5 );
   SetSizer(sizer_main);
   //sizer_main->SetSizeHints(this);
   change_proto();

   // std::cout << group_list.Count() << std::endl;
   // for (int k = 0 ; k < group_list.Count(); k++)
   // {
   //    wxMessageBox(group_list.Item(k));
   // }

   // for (int i = 0; i < main_frame->all_connection_records.Count(); i++)
   // {
   //    local_options = main_frame->all_connection_records.Item(i);
   //    for (int f = 0 ; f < group_list.Count() ; f++)
   //    {
   // 	 if ( group_list.Item(f) == local_options[wxT("group_name")])
   // 	 {
   // 	    found_duplicate = true;
   // 	 }
   //    }
   //    if (found_duplicate == false)
   //    {
   // 	 wxMessageBox(local_options[wxT("group_name")]);
   // 	 group_list.Add(local_options[wxT("group_name")]);
   // 	 found_duplicate = false;
   //    }
   // }
   // group_list.Sort();
   // group_list.Shrink();
   // std::cout << group_list.Count() << std::endl;

}

///////////////////////////////////////////////////////////////////////////////
//! \brief Setup options
//! \param const Options_HashMap *options - pointer to options array
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPDialog::Set_Options(const Options_HashMap *options)
{
   //Options_HashMap local_options;
   if (options != NULL)
   {
      local_options = *options; 
      //m_combobox_protocol->SetSelection(wxAtoi(local_options[wxT("proto")]));
      if (local_options[wxT("proto")] == wxT("rdp"))
      {
	 m_combobox_protocol->SetSelection(0);
      }
      else if (local_options[wxT("proto")] == wxT("ica"))
      {
	 m_combobox_protocol->SetSelection(1);
      }
      else
      {
	 m_combobox_protocol->SetSelection(0);
      }
      change_proto();

      for (int j = 0; j < group_list.Count(); j++ )
      {
	 if ((group_list.Item(j) == wxT("")) || (group_list.Item(j) == wxT("Main")))
	 {
	    group_list.RemoveAt(j);
	 }
      }
      //std::cout << group_list.Count() << std::endl;


      if ((rdp_options_dialog != NULL) &&
	  (rdp_options_dialog->IsShown()))
      {
	 rdp_options_dialog->Set_Options(options);
	 rdp_options_dialog->Set_Group_List(group_list);
      }
      if ((ica_options_dialog != NULL) &&
	  (ica_options_dialog->IsShown()))
      {
	 ica_options_dialog->Set_Options(options);
	 ica_options_dialog->Set_Group_List(group_list);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Return Options array
///////////////////////////////////////////////////////////////////////////////
Options_HashMap RDPDialog::Get_Options()
{
   Options_HashMap local_options;
   //FIXME: Add checking for proto record
   local_options.clear();
   if ((rdp_options_dialog != NULL) &&
       (rdp_options_dialog->IsShown()))
   {
      local_options = rdp_options_dialog->Get_Options();
   }
   if ((ica_options_dialog != NULL) &&
       (ica_options_dialog->IsShown()))
   {
      local_options = ica_options_dialog->Get_Options();
   }
   
   //wxMessageBox(local_options[wxT("uniq_name")]);
   //local_options[wxT("proto")] = wxString::Format(wxT("%"),
//						  m_combobox_protocol->GetSelection());
   if (m_combobox_protocol->GetSelection() == 0)
   {
      local_options[wxT("proto")] = wxT("rdp");
   }
   else if (m_combobox_protocol->GetSelection() == 1)
   {
      local_options[wxT("proto")] = wxT("ica");
   }
   else
   {
      local_options[wxT("proto")] = wxT("rdp");
   }

 
   //std::cout << local_options.size() << std::endl;
   //wxMessageBox( local_options[wxT("hostname")]);
   
   return local_options;
}


///////////////////////////////////////////////////////////////////////////////
//! \brief Destructor
///////////////////////////////////////////////////////////////////////////////
RDPDialog::~RDPDialog()
{


   // if(notebook != NULL)
   // {
   // 	end = notebook->GetPageCount();
   // 	notebook->DeleteAllPages();
   // 	delete notebook;
   // 	notebook = NULL;
   // }
   if (rdp_options_dialog != NULL)
   {
      delete rdp_options_dialog;
      rdp_options_dialog = NULL;
   }
   if (ica_options_dialog != NULL)
   {
      delete ica_options_dialog;
      ica_options_dialog = NULL;
   }

   if(m_button_ok != NULL) {delete m_button_ok; m_button_ok = NULL;}
   if(m_button_cancel != NULL) {delete m_button_cancel; m_button_cancel = NULL;}
   if(m_button_save != NULL) {delete m_button_save; m_button_save = NULL;}
	
   sizer_protocol->Clear();
   wxDELETE(m_line_buttons);
   wxDELETE(m_line_protocol);
	
}
///////////////////////////////////////////////////////////////////////////////
//! \brief Processe chnage proto combobox
//! \sa change_proto
///////////////////////////////////////////////////////////////////////////////
void RDPDialog::change_proto_func(wxCommandEvent &event)
{
   change_proto();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Processe chnage proto combobox
///////////////////////////////////////////////////////////////////////////////
void RDPDialog::change_proto()
{
   if (m_combobox_protocol->GetSelection() == 0)
   {
      rdp_options_dialog->Show();
      ica_options_dialog->Hide();
      sizer_main->Layout();
   }
   else if (m_combobox_protocol->GetSelection() == 1)
   {
      ica_options_dialog->Show();
      rdp_options_dialog->Hide();
      sizer_main->Layout();
   }
}



// RDPGeneral::RDPGeneral(wxNotebook *frame, const wxPoint& point, const wxSize& size, base_conn * rdp_base_main): wxPanel(frame, wxID_ANY, point, size ) 
// {
// 	rdp_base = rdp_base_main;

// 	m_static_server = NULL;
// 	m_text_server = NULL;

// 	m_static_port = NULL;
// 	m_text_port = NULL;

// 	m_static_username = NULL;
// 	m_text_username = NULL;

// 	m_static_password = NULL;
// 	m_text_password = NULL;

// 	m_static_domain = NULL;
// 	m_text_domain = NULL;

// 	m_checkbox_autologon = NULL;
// 	m_checkbox_customport = NULL;
// 	m_checkbox_attachtoconsole = NULL;

// 	m_static_groupname = NULL;
// 	m_combobox_groupname = NULL;

// 	m_static_connectionname = NULL;
// 	m_text_connectionname = NULL;

// 	groupcount = 0;
// 	get_group_list();



// 	wxFlexGridSizer * sizer_top = new wxFlexGridSizer(4,10,10);
// 	sizer_top->AddGrowableCol(2,0);
// 	sizer_top->AddGrowableCol(3,0);

// 	m_static_connectionname = new wxStaticText(this, wxID_ANY, wxT("Connection name"),wxDefaultPosition,wxDefaultSize);
// 	m_text_connectionname = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(200,-1), wxTE_PROCESS_ENTER);
// 	m_static_groupname = new wxStaticText(this, wxID_ANY, wxT("Group name"),wxDefaultPosition,wxDefaultSize);
// 	m_combobox_groupname = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,wxSize(200,-1) ,array_string_group_list);
	
// 	sizer_top->Add(m_static_connectionname,0,wxALL | wxALIGN_LEFT  ,0);
// 	sizer_top->Add(20,20,0,wxALL | wxALIGN_CENTER,0);
// 	sizer_top->Add(m_text_connectionname,0,wxALL  |wxALIGN_RIGHT | wxEXPAND,0);
// 	sizer_top->Add(10,20,0,wxALL | wxALIGN_CENTER,0);
// 	sizer_top->Add(m_static_groupname,0,wxALL | wxALIGN_LEFT  ,0);
// 	sizer_top->Add(20,20,0,wxALL | wxALIGN_CENTER,0);
// 	sizer_top->Add(m_combobox_groupname,0,wxALL |wxALIGN_RIGHT | wxEXPAND ,0);
// 	sizer_top->Add(10,20,0,wxALL | wxALIGN_CENTER,0);

// 	wxBoxSizer * sizer_main = new wxBoxSizer(wxVERTICAL);

// 	wxFlexGridSizer * sizer_bottom = new wxFlexGridSizer(3,10,10);
// 	sizer_bottom->AddGrowableCol(0,1); 


// 	m_static_server = new wxStaticText(this, wxID_ANY, wxT("Server"),wxDefaultPosition,wxDefaultSize);
// 	m_text_server = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(130,-1), wxTE_PROCESS_ENTER);
// 	m_checkbox_customport = new wxCheckBox( this, ID_CHECKBOX_CUSTOMPORT, wxT("&Use custom port"), wxDefaultPosition,wxDefaultSize );

// 	m_static_username = new wxStaticText(this, wxID_ANY, wxT("Username"),wxDefaultPosition,wxDefaultSize);
// 	m_text_username = new wxTextCtrl( this, wxID_ANY, wxT("Administrator"), wxDefaultPosition, wxSize(130,-1), wxTE_PROCESS_ENTER);

// 	wxBoxSizer * sizer_port = new wxBoxSizer(wxHORIZONTAL);
// 	m_static_port = new wxStaticText(this, wxID_ANY, _("Port"),wxDefaultPosition,wxDefaultSize);
// 	m_text_port = new wxTextCtrl( this, wxID_ANY, wxT("3389"), wxDefaultPosition, wxSize(50,-1), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC) );
// 	//m_text_port = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(50,-1), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC) );
	
// 	sizer_port->Add(m_static_port,1,wxALL | wxEXPAND ,2);
// 	sizer_port->Add(m_text_port,1,wxALL | wxEXPAND ,2);


// 	m_static_password = new wxStaticText(this, wxID_ANY, wxT("Password"),wxDefaultPosition,wxDefaultSize);
// 	m_text_password = new wxTextCtrl( this, ID_TEXTCTRL_PASSWORD, wxT(""),wxDefaultPosition, wxSize(130,-1), wxTE_PASSWORD);
// 	m_checkbox_autologon = new wxCheckBox( this, ID_CHECKBOX_AUTOLOGON, wxT("&Auto logon"), wxDefaultPosition,wxDefaultSize );

// 	m_static_domain = new wxStaticText(this, wxID_ANY, wxT("Domain"),wxDefaultPosition,wxDefaultSize);
// 	m_text_domain = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(130,-1), wxTE_PROCESS_ENTER);
// 	m_checkbox_attachtoconsole = new wxCheckBox( this, wxID_ANY, wxT("&Attach to console"), wxDefaultPosition,wxDefaultSize);

	
// 	sizer_bottom->Add(m_static_server,0,wxALIGN_LEFT | wxALL,0);
// 	sizer_bottom->Add(m_text_server,0,wxALL,0);
// 	sizer_bottom->Add(m_checkbox_customport,0,wxALIGN_LEFT |wxALL,3);
// 	sizer_bottom->Add(m_static_username,0,wxALL,0);
// 	sizer_bottom->Add(m_text_username,0,wxALL,0);
// 	sizer_bottom->Add(sizer_port,0,wxALL,0);
// 	sizer_bottom->Add(m_static_password,0,wxALL,0);
// 	sizer_bottom->Add(m_text_password,0,wxALL,0);
// 	sizer_bottom->Add(m_checkbox_autologon,0,wxALL,3);
// 	sizer_bottom->Add(m_static_domain,0,wxALL,0);
// 	sizer_bottom->Add(m_text_domain,0,wxALL,0);
// 	sizer_bottom->Add(m_checkbox_attachtoconsole,0,wxALL,3);

// 	sizer_main->Add(sizer_top,0,wxALL | wxEXPAND /*wxSHAPED*/,5);
// 	sizer_main->Add(sizer_bottom,0,wxALL | wxSHAPED,5);	

// 	SetSizer(sizer_main);   
// 	sizer_main->SetSizeHints(this);
// 	FitInside();
// 	sizer_main->Fit(this); 

// 	m_text_port->Enable(false);
// 	m_static_port->Enable(false);
// 	m_combobox_groupname->SetValue(wxT("Main"));
// 	m_combobox_groupname->SetSelection(0);
	

// }

// RDPGeneral::~RDPGeneral()
// {
// 	if(m_static_server != NULL) {delete m_static_server; m_static_server = NULL;}
// 	if(m_text_server != NULL) {delete m_text_server; m_text_server = NULL;}
// 	if(m_static_port != NULL) {delete m_static_port; m_static_port = NULL;}
// 	if(m_text_port != NULL) {delete m_text_port; m_text_port = NULL;}
// 	if(m_static_username != NULL) {delete m_static_username; m_static_username = NULL;}
// 	if(m_text_username != NULL) {delete m_text_username; m_text_username = NULL;}

// 	if(m_static_password != NULL) {delete m_static_password ; m_static_password = NULL;}
// 	if(m_text_password != NULL) {delete m_text_password; m_text_password = NULL;}
// 	if(m_static_domain != NULL) {delete m_static_domain; m_static_domain = NULL;}
// 	if(m_text_domain != NULL) {delete m_text_domain; m_text_domain = NULL;}
// 	if(m_checkbox_autologon != NULL) {delete m_checkbox_autologon; m_checkbox_autologon = NULL;}
// 	if(m_checkbox_customport != NULL) {delete m_checkbox_customport; m_checkbox_customport = NULL; }
// 	if(m_checkbox_attachtoconsole != NULL)
// 	{delete m_checkbox_attachtoconsole; m_checkbox_attachtoconsole = NULL;}


// 	if (m_static_groupname) {delete m_static_groupname; m_static_groupname = NULL;}
// 	if (m_combobox_groupname) {delete m_combobox_groupname; m_combobox_groupname = NULL;}

// 	if (m_static_connectionname) {delete m_static_connectionname; m_static_connectionname = NULL;}
// 	if (m_text_connectionname) {delete m_text_connectionname; m_text_connectionname = NULL;}
	
// }

// void RDPGeneral::get_group_list()
// {
//    // Benc bc;

//    // Connections_List 
//    // //RDPConn lrdpconn;
//    // // int lc = bc.Count(rdp_base);
//    // // array_string_group_list.Alloc(lc);
//    // // wxString currgroupname;

//    // // for (int i = 0; i < lc; i++)
//    // // {
//    // //    lrdpconn = bc.Get(rdp_base,i);

//    // //    if (lrdpconn.group_name)
//    // //    {
//    // // 	 if (lrdpconn.group_name.Length() == 0)
//    // // 	 {
//    // // 	    lrdpconn.group_name.assign(wxT("Main")); 
//    // // 	 }
//    // // 	 int arr_size = (int)array_string_group_list.Count();
//    // // 	 bool flag = true;
//    // // 	 for (int i = 0; i < arr_size; i++)
//    // // 	 {
//    // // 	    if (lrdpconn.group_name == array_string_group_list.Item(i)) 
//    // // 	    {
//    // // 	       flag = false;
//    // // 	       break;
//    // // 	    }
//    // // 	 }

//    // // 	 if (flag)
//    // // 	 {
//    // // 	    array_string_group_list.Add(lrdpconn.group_name); 
//    // // 	 }
//    // //   }

//    // // }
//    // array_string_group_list.Sort();
//    // array_string_group_list.Shrink();
// }

// void RDPGeneral::checkbox_customport_func(wxCommandEvent &event)
// {
// 		if (!this->m_checkbox_customport->IsChecked())
// 		{
// 			m_text_port->Enable(false);
// 			m_static_port->Enable(false);
// 		}
// 		else
// 		{
// 			m_text_port->Enable(true);
// 			m_static_port->Enable(true);
// 		}
	
// }

// void RDPGeneral::text_passwordentry_func(wxCommandEvent &event)
// {
// 	wxString strTemp;
// 	switch(event.GetId())
// 	{
// 	case ID_TEXTCTRL_PASSWORD:
// 		strTemp = this->m_text_password->GetValue();
// 		if (strTemp.Length() == 0)
// 		{
// 			m_checkbox_autologon->SetValue(false);
// 		} 
// 		else 
// 		{
// 			m_checkbox_autologon->SetValue(true);
// 		}
// 		break;

// 	case ID_CHECKBOX_AUTOLOGON:
// 		if (!m_checkbox_autologon->IsChecked()) m_text_password->SetValue(_T(""));
// 		break;

// 	default:
// 		break;
// 	}
// }

// RDPDisplay::RDPDisplay(wxNotebook *frame, const wxPoint& point, const wxSize& size) :  wxPanel(frame, wxID_ANY, point , size ) 
// {
// 	m_static_width = NULL;
// 	m_text_width = NULL;

// 	m_static_height = NULL;
// 	m_text_height = NULL;

// 	m_static_colordepth = NULL;
// 	m_choise_colordepth = NULL;

// 	m_checkbox_fullscreen = NULL;
// 	m_checkbox_smartsizing = NULL;
// 	m_checkbox_controlsize = NULL;

// 	m_checkbox_updatescreen = NULL;

// 	m_static_resolution = NULL;
// 	m_combobox_resolution = NULL;
// 	m_checkbox_customgeometry = NULL;
	
// 	wxArrayString resolutions = GetResolutionsList();

// 	sizer_top = new wxBoxSizer(wxVERTICAL);
// 	sizer_main = new wxFlexGridSizer(3,10,10);

// 	m_static_resolution = new wxStaticText(this, wxID_ANY, wxT("Screen resolution"),wxDefaultPosition, wxDefaultSize);
// 	m_combobox_resolution = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,wxSize(120,-1),resolutions,wxCB_READONLY);
// 	int sel = GetDefaultResolution();
// 	if (sel >=0)
// 	{
// 		m_combobox_resolution->SetValue(resolutions.Item(sel));
// 		m_combobox_resolution->SetSelection(sel);
// 	}

// 	m_checkbox_customgeometry = new wxCheckBox( this, ID_CHK_CUSTOMGEOMETRY, _T("&Custom geometry"), wxDefaultPosition, wxDefaultSize );
// 	m_static_width = new wxStaticText(this, wxID_ANY, wxT("Screen width"),wxDefaultPosition, wxDefaultSize);
// 	m_text_width = new wxTextCtrl( this, wxID_ANY, wxT("800"), wxDefaultPosition, wxSize(120,-1), wxTE_PROCESS_ENTER , wxTextValidator(wxFILTER_NUMERIC) );
// 	m_static_height = new wxStaticText(this, wxID_ANY, wxT("Screen height"),wxDefaultPosition, wxDefaultSize);
// 	m_text_height = new wxTextCtrl( this, wxID_ANY, wxT("600"), wxDefaultPosition, wxSize(120,-1), wxTE_PROCESS_ENTER , wxTextValidator(wxFILTER_NUMERIC) );

// 	m_static_colordepth = new wxStaticText(this, wxID_ANY, wxT("Color depth"),wxDefaultPosition, wxDefaultSize);
// 	wxString Choises[] = {_("8-bit"),_("15-bit"),_("16-bit"),_("24-bit")};
// 	m_choise_colordepth = new wxComboBox(this,wxID_ANY,_(""),wxDefaultPosition,wxSize(120,-1),4,Choises,wxCB_READONLY);
// 	m_choise_colordepth->SetValue(Choises[2]);
// 	m_choise_colordepth->SetSelection(2);

// 	m_checkbox_updatescreen = new wxCheckBox( this, wxID_ANY, wxT("&Force update screen"), wxDefaultPosition, wxDefaultSize );
// 	m_checkbox_fullscreen = new wxCheckBox( this, ID_CHK_FULLSCREEN, wxT("&Full screen"), wxDefaultPosition, wxDefaultSize );
// 	m_checkbox_smartsizing = new wxCheckBox( this, -1, wxT("&Smart sizing"), wxDefaultPosition, wxDefaultSize );
// 	m_checkbox_controlsize = new wxCheckBox( this, ID_CHK_CONTROLSIZE, wxT("&Control size"),wxDefaultPosition, wxDefaultSize);

// 	m_checkbox_controlsize->SetValue(true);
	


// 	sizer_main->AddSpacer(1);
// 	sizer_main->AddSpacer(1);
// 	sizer_main->AddSpacer(1);
// 	s1 = new wxBoxSizer(wxHORIZONTAL);

// 	s1->Add(m_static_resolution,0,wxALL |  wxALIGN_LEFT  /*wxEXPAND*/,0);

// 	s1->Add(m_static_width,0,wxALL | wxALIGN_LEFT  /*wxEXPAND*/,0);

// 	sizer_main->Add(s1,0,wxALIGN_LEFT | wxALL,0);
// 	s2 = new wxBoxSizer(wxHORIZONTAL);
// 	s2->Add(m_combobox_resolution,0,wxALL | wxALIGN_CENTER | wxEXPAND,0);
// 	s2->Add(m_text_width,0,wxALL | wxALIGN_CENTER | wxEXPAND,0);
// 	sizer_main->Add(s2,0,wxALIGN_CENTER | wxALL,0);

// 	sizer_main->Add(m_checkbox_customgeometry,0,wxALIGN_LEFT | wxALL,3);
// 	sizer_main->Add(m_static_height,0,wxALIGN_LEFT | wxALL,0);
// 	sizer_main->Add(m_text_height,0,wxALIGN_CENTER | wxALL,0);
// 	sizer_main->Add(m_checkbox_fullscreen,0,wxALIGN_LEFT | wxALL,3);
// 	sizer_main->Add(m_static_colordepth,0,wxALIGN_LEFT | wxALL,0);
// 	sizer_main->Add(m_choise_colordepth,0,wxALIGN_CENTER | wxALL,0);
// #ifdef __WXMSW__
// 	sizer_main->Add(m_checkbox_smartsizing,0,wxALIGN_LEFT | wxALL,3);
// #endif
// #ifdef __WXGTK__
// 	sizer_main->Add(m_checkbox_updatescreen,0,wxALIGN_LEFT | wxALL,3);
// #endif
// 	sizer_main->AddSpacer(1);
// 	sizer_main->AddSpacer(1);
// 	sizer_main->Add(m_checkbox_controlsize,0,wxALIGN_LEFT | wxALL,3);


// 	sizer_top->Add(sizer_main,0,wxALL | wxALIGN_CENTER , 5);
// 	SetSizer(sizer_top);
// 	sizer_top->SetSizeHints(this);
// 	sizer_top->Fit(this); 

// #ifdef __WXMSW__
// 	m_checkbox_updatescreen->Show(false);
	
// #endif
// #ifdef __WXGTK__
// 	m_checkbox_smartsizing->Show(false);
// 	m_checkbox_controlsize->Show(true);
// #endif
// 	CheckRDPDisplay();


// }

// wxArrayString RDPDisplay::GetResolutionsList()
// {
// 	wxDisplay disp;
// 	wxArrayString resolutions;
// #ifdef __WXMSW__
// 	wxArrayVideoModes avm = disp.GetModes();
// 	for (int i = 0; i < (int)avm.Count(); i++)
// 	{
// 		if (avm.Item(i).w < 800 || avm.Item(i).h < 600 || avm.Item(i).h > avm.Item(i).w)
// 			continue;
// 		wxString temp;
// 		temp = wxString::Format(wxT("%i x %i"),avm.Item(i).w,avm.Item(i).h);
// 		BOOL flag = FALSE;
// 		for (int i = 0; i < (int)resolutions.Count(); i++)
// 		{	
// 			if (resolutions.Item(i) == temp) flag = TRUE;
// 		}
// 		if (!flag)	resolutions.Add(temp); 
// 	}
// #endif

// #ifdef __WXGTK__
	
// 	resolutions.Add(wxT("800 x 600"));
// 	resolutions.Add(wxT("1024 x 768"));
// 	resolutions.Add(wxT("1152 x 864"));
// 	resolutions.Add(wxT("1280 x 960"));
// 	resolutions.Add(wxT("1280 x 1024"));

// #endif
// 	return resolutions;
// }

// wxSize RDPDisplay::GetResolution(int i)
// {
// 	wxSize error_size(0,0);
// 	if (i < 0) return error_size;
// 	wxArrayString resolutions = GetResolutionsList();
// 	if (i >= (int)resolutions.Count())  return error_size;
// 	wxString resolution_string = resolutions.Item(i); 
// 	size_t len = resolution_string.Length(); 
// 	int pos = resolution_string.Find(wxT("x"));
	
// 	wxString width_string = resolution_string.Left(pos - 1);
// 	wxString heigth_string = resolution_string.Right(len - pos - 1); 

// 	wxSize result_size(wxAtoi(width_string),wxAtoi(heigth_string));
// 	return result_size;
// }

// int RDPDisplay::GetDefaultResolution()
// {
// 	wxSize disp_size = wxGetDisplaySize();
// 	wxArrayString resolutions = GetResolutionsList();
// 	if (resolutions.Count() <= 0) return -1; 
// 	int default_pos = 0;
// 	for (int i = 0; i < (int)resolutions.Count(); i ++)
// 	{
// 		wxSize size = GetResolution(i);
// 		if (size.x == 800) return i;
// 		else 
// 		{
// 			if (size.x >= disp_size.x || size.y >= disp_size.y)
// 			{
// 				if (default_pos > 0)
// 				{
// 					return default_pos - 1;
// 				}
// 				else
// 				{
// 					return default_pos;
// 				}
// 			}
// 			else
// 			{
// 				default_pos ++;
// 			}
// 		}
// 	}
// 	return default_pos - 1;
// }

// void RDPDisplay::CheckRDPDisplay()
// {
// 	if (m_checkbox_controlsize->IsChecked())
// 	{
// 		m_static_resolution->Enable(false);
// 		m_combobox_resolution->Enable(false);
		
// 		m_static_width->Enable(false);
// 		m_text_width->Enable(false);
// 		m_static_height->Enable(false);
// 		m_text_height->Enable(false);

// 		if (m_checkbox_customgeometry->IsChecked())
// 		{
// 			m_static_resolution->Show(false);
// 			m_combobox_resolution->Show(false);
// 			s1->Hide(m_static_resolution);
// 			s1->Show(m_static_width);
// 			s2->Hide(m_combobox_resolution);
// 			s2->Show(m_text_width);
		
// 		}
// 		else
// 		{
// 			m_static_width->Show(false);
// 			m_text_width->Show(false);
// 			m_static_height->Show(false);
// 			m_text_height->Show(false);
// 			s1->Hide(m_static_width);
// 			s1->Show(m_static_resolution);
// 			s2->Show(m_combobox_resolution);
// 			s2->Hide(m_text_width);
		
// 		}

// 		m_checkbox_customgeometry->Enable(false);
// 		m_checkbox_fullscreen->Enable(false);
// 	}
// 	else
// 	{
// 		m_checkbox_fullscreen->Enable(true);
// 		if (m_checkbox_fullscreen->IsChecked())
// 		{
// 			m_static_resolution->Enable(false);
// 			m_combobox_resolution->Enable(false);
		
// 			m_static_width->Enable(false);
// 			m_text_width->Enable(false);
// 			m_static_height->Enable(false);
// 			m_text_height->Enable(false);

// 			m_checkbox_customgeometry->Enable(false);
// 		}
// 		else
// 		{
// 			m_checkbox_customgeometry->Enable(true);
// 			if (m_checkbox_customgeometry->IsChecked())
// 			{
// 				m_static_width->Enable(true);
// 				m_text_width->Enable(true);
// 				m_static_height->Enable(true);
// 				m_text_height->Enable(true);
// 				m_static_width->Show(true);
// 				m_text_width->Show(true);
// 				m_static_height->Show(true);
// 				m_text_height->Show(true);
// 				m_static_resolution->Enable(false);
// 				m_combobox_resolution->Enable(false);
// 				m_static_resolution->Show(false);
// 				m_combobox_resolution->Show(false);
// 				s1->Hide(m_static_resolution);
// 				s1->Show(m_static_width);
// 				s2->Hide(m_combobox_resolution);
// 				s2->Show(m_text_width);


// 			}
// 			else
// 			{
// 				m_static_width->Enable(false);
// 				m_text_width->Enable(false);
// 				m_static_height->Enable(false);
// 				m_text_height->Enable(false);
// 				m_static_width->Show(false);
// 				m_text_width->Show(false);
// 				m_static_height->Show(false);
// 				m_text_height->Show(false);
// 				m_static_resolution->Enable(true);
// 				m_combobox_resolution->Enable(true);
// 				m_static_resolution->Show(true);
// 				m_combobox_resolution->Show(true);
// 				s1->Hide(m_static_width);
// 				s1->Show(m_static_resolution);
// 				s2->Show(m_combobox_resolution);
// 				s2->Hide(m_text_width);

// 			}
// 		}
// 	}
// 	s1->Layout();
// 	s2->Layout();
// 	sizer_main->Layout();
// }

// void RDPDisplay::checkbox_customgeometry_func(wxCommandEvent& event)
// {
// 	CheckRDPDisplay();
// }


// void RDPDisplay::checkbox_fullscreen_func(wxCommandEvent &event)
// {
// 	CheckRDPDisplay();
// }

// void RDPDisplay::checkbox_controlsize_func(wxCommandEvent &event)
// {
// 	CheckRDPDisplay();
// }


// RDPDisplay::~RDPDisplay()
// {
// 	if(m_static_width != NULL) {delete m_static_width; m_static_width = NULL;}
// 	if(m_text_width != NULL) {delete m_text_width; m_text_width = NULL;}
// 	if(m_static_height != NULL) {delete m_static_height; m_static_height = NULL;}
// 	if(m_text_height != NULL) {delete m_text_height; m_text_height = NULL;}
// 	if(m_static_colordepth != NULL) {delete m_static_colordepth; m_static_colordepth = NULL;}
// 	if(m_choise_colordepth != NULL) {delete m_choise_colordepth; m_choise_colordepth = NULL;}

// 	if(m_checkbox_fullscreen != NULL) {delete m_checkbox_fullscreen; m_checkbox_fullscreen = NULL;}
// 	if(m_checkbox_smartsizing != NULL) {delete m_checkbox_smartsizing; m_checkbox_smartsizing = NULL;}
// 	if(m_checkbox_controlsize != NULL) {delete m_checkbox_controlsize; m_checkbox_controlsize = NULL;}

// 	if(m_checkbox_updatescreen != NULL) {delete m_checkbox_updatescreen; m_checkbox_updatescreen = NULL;}

// 	if (m_static_resolution != NULL) {delete m_static_resolution; m_static_resolution = NULL;}
// 	if (m_combobox_resolution != NULL) {delete m_combobox_resolution; m_combobox_resolution = NULL;}

// 	if (m_checkbox_customgeometry != NULL) {delete m_checkbox_customgeometry; m_checkbox_customgeometry = NULL;}

// }

// RDPShare::RDPShare(wxNotebook *frame, const wxPoint& point, const wxSize& size) : wxPanel(frame, wxID_ANY, point , size ) 
// {
// 	m_static_sound = NULL;
// 	m_choise_sound = NULL;

// 	m_static_keyboard = NULL;
// 	m_choise_keyboard = NULL;

// 	m_checkbox_drives = NULL;
// 	m_checkbox_printers = NULL;
// 	m_checkbox_comports = NULL;
// 	m_checkbox_smartcards = NULL;

// 	m_static_linux_devices = NULL;
// 	m_text_linux_devices = NULL;

// 	sizer_top = new wxBoxSizer(wxVERTICAL);
// 	sizer_main = new wxFlexGridSizer(2,10,10);
// 	sizer_devices = new wxBoxSizer(wxVERTICAL);

// 	m_static_sound  = new wxStaticText(this, wxID_ANY, wxT("Redirect sound"),wxDefaultPosition, wxDefaultSize);
// 	wxString ChoisesSound[] = {wxT("Bring to this computer"),wxT("Do not play"),wxT("Leave at remote computer")};
// 	m_choise_sound = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition, wxSize(200,-1),3,ChoisesSound,wxCB_READONLY);
// 	m_choise_sound->SetValue(ChoisesSound[0]);
// 	m_choise_sound->SetSelection(0);
	

// 	m_static_keyboard  = new wxStaticText(this, wxID_ANY, wxT("Using windows key combinations"),wxDefaultPosition, wxDefaultSize);
// 	wxString ChoisesKeyboard[] = {wxT("On local computer"),wxT("On remote computer"),wxT("In full screen only")};

// 	m_choise_keyboard = new wxComboBox(this,wxID_ANY,_(""),wxDefaultPosition,wxSize(200,-1),3,ChoisesKeyboard,wxCB_READONLY);
// 	m_choise_keyboard->SetValue(ChoisesKeyboard[2]);
// 	m_choise_keyboard->SetSelection(2);
	
// 	m_checkbox_drives = new wxCheckBox( this, wxID_ANY, wxT("&Redirect drives"), wxDefaultPosition, wxDefaultSize );
// 	m_checkbox_printers = new wxCheckBox( this, wxID_ANY, wxT("&Redirect printers"), wxDefaultPosition, wxDefaultSize );
// 	m_checkbox_comports = new wxCheckBox( this, wxID_ANY, wxT("&Redirect COM ports"),wxDefaultPosition, wxDefaultSize );
// 	m_checkbox_smartcards = new wxCheckBox( this, wxID_ANY, wxT("&Redirect smartcards"), wxDefaultPosition, wxDefaultSize );

// 	m_static_linux_devices = new wxStaticText(this, wxID_ANY, wxT("&Redirect devices for linux (see man for rdesktop)"),wxDefaultPosition, wxDefaultSize);
// 	m_text_linux_devices = new wxTextCtrl(this, wxID_ANY, wxT(""),wxDefaultPosition,wxSize(280,-1),wxTE_MULTILINE);

// 	sizer_main->AddSpacer(1);
// 	sizer_main->AddSpacer(1);
// 	sizer_main->Add(m_static_sound,0,wxALIGN_LEFT | wxALL,0);
// 	sizer_main->Add(m_checkbox_drives,0,wxALIGN_LEFT | wxALL,3);
// 	sizer_main->Add(m_choise_sound,0,wxALIGN_LEFT | wxALL,0);
// 	sizer_main->Add(m_checkbox_printers,0,wxALIGN_LEFT | wxALL,3);
// 	sizer_main->Add(m_static_keyboard,0,wxALIGN_LEFT | wxALL,0);
// 	sizer_main->Add(m_checkbox_comports,0,wxALIGN_LEFT | wxALL,3);
// 	sizer_main->Add(m_choise_keyboard,0,wxALIGN_LEFT | wxALL,0);
// 	sizer_main->Add(m_checkbox_smartcards,0,wxALIGN_LEFT | wxALL,3);


// //	sizer_devices->Add(m_text_linux_devices,0,wxALIGN_CENTER | wxEXPAND| wxALL,0);

// 	sizer_top->Add(sizer_main,0,wxALL | wxALIGN_CENTER , 5);
// 	sizer_top->Add(m_static_linux_devices,0,wxALL | wxALIGN_LEFT | wxEXPAND, 5);
// 	sizer_top->Add(m_text_linux_devices,0,wxALL | wxALIGN_LEFT | wxEXPAND, 5);
// 	SetSizer(sizer_top);
// 	sizer_top->SetSizeHints(this);
// 	sizer_top->Fit(this); 


	


// #ifdef __WXMSW__
// 	m_static_linux_devices->Show(false);
// 	m_text_linux_devices->Show(false);
// #endif
// #ifdef __WXGTK__
// 	m_checkbox_drives->Show(false);
// 	m_checkbox_printers->Show(false);
// 	m_checkbox_comports->Show(false);
// 	m_checkbox_smartcards->Show(false);
// #endif
// }

// RDPShare::~RDPShare()
// {
// 	if (m_static_sound) {delete m_static_sound; m_static_sound = NULL;}
// 	if (m_choise_sound) {delete m_choise_sound; m_choise_sound = NULL;}

// 	if (m_static_keyboard) {delete m_static_keyboard; m_static_keyboard = NULL;}
// 	if (m_choise_keyboard) {delete m_choise_keyboard; m_choise_keyboard = NULL;}

// 	if (m_checkbox_drives) {delete m_checkbox_drives; m_checkbox_drives = NULL;}
// 	if (m_checkbox_printers) {delete m_checkbox_printers; m_checkbox_printers = NULL;}
// 	if (m_checkbox_comports) {delete m_checkbox_comports; m_checkbox_comports = NULL;}
// 	if (m_checkbox_smartcards) {delete m_checkbox_smartcards; m_checkbox_smartcards = NULL;}

// 	if (m_static_linux_devices) {delete m_static_linux_devices; m_static_linux_devices = NULL;}
// 	if (m_text_linux_devices) {delete m_text_linux_devices; m_text_linux_devices = NULL;}

// }


// RDPProgram::RDPProgram(wxNotebook *frame, const wxPoint& point, const wxSize& size) : wxPanel(frame, wxID_ANY, point , size ) 
// {
// 	m_static_program = NULL;
// 	m_text_program = NULL;

// 	m_static_workdir = NULL;
// 	m_text_workdir = NULL;

// 	m_checkbox_program = NULL;
// 	m_checkbox_maximized = NULL;

// 	sizer_top = new wxBoxSizer(wxVERTICAL);
// 	sizer_main = new wxBoxSizer(wxVERTICAL);

// 	m_static_program  = new wxStaticText(this, wxID_ANY, wxT("Program path"), wxDefaultPosition, wxDefaultSize);
// 	m_text_program = new wxTextCtrl( this, wxID_ANY, wxT(""),  wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
// 	m_static_workdir  = new wxStaticText(this, wxID_ANY, wxT("Working dir"), wxDefaultPosition, wxDefaultSize);
// 	m_text_workdir = new wxTextCtrl( this, wxID_ANY, wxT(""),  wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
// 	m_checkbox_program = new wxCheckBox( this, ID_CHECKBOX_USE_PROGRAM, wxT("&Start following program"),  wxDefaultPosition, wxDefaultSize );
// 	m_checkbox_maximized = new wxCheckBox( this, wxID_ANY, wxT("&Start maximized"),  wxDefaultPosition, wxDefaultSize);

	
// 	sizer_main->Add(m_checkbox_program,0,wxALL | wxALIGN_LEFT , 5);
// 	sizer_main->AddSpacer(10);
// 	sizer_main->Add(m_checkbox_maximized,0,wxALL | wxALIGN_LEFT , 5);
// 	sizer_main->Add(m_static_program,0,wxALL | wxALIGN_LEFT , 5);
// 	sizer_main->Add(m_text_program,0,wxALL | wxALIGN_LEFT | wxEXPAND, 5);
// 	sizer_main->Add(m_static_workdir,0,wxALL | wxALIGN_LEFT , 5);
// 	sizer_main->Add(m_text_workdir,0,wxALL | wxALIGN_LEFT | wxEXPAND, 5);
// 	sizer_top->Add(sizer_main,0,wxALL| wxALIGN_CENTER | wxEXPAND,5);
// 	SetSizer(sizer_top);
// 	sizer_top->SetSizeHints(this);
// 	sizer_top->Fit(this); 


// 	m_checkbox_maximized->Enable(false);
// 	m_static_program->Enable(false);
// 	m_text_program->Enable(false);
// 	m_static_workdir->Enable(false);
// 	m_text_workdir->Enable(false);

// }

// RDPProgram::~RDPProgram()
// {
// 	if (m_static_program) {delete m_static_program; m_static_program = NULL;}
// 	if (m_text_program) {delete m_text_program; m_text_program = NULL;}

// 	if (m_static_workdir) {delete m_static_workdir; m_static_workdir = NULL;}
// 	if (m_text_workdir) {delete m_text_workdir; m_text_workdir = NULL;}

// 	if (m_checkbox_program) {delete m_checkbox_program; m_checkbox_program = NULL;}
// 	if (m_checkbox_maximized) {delete m_checkbox_maximized; m_checkbox_maximized = NULL;}
// }

// void RDPProgram::checkbox_program_func(wxCommandEvent &event) 
// {
// 	if(!this->m_checkbox_program->IsChecked() )
// 	{
// 		m_checkbox_maximized->Enable(false);
// 		m_static_program->Enable(false);
// 		m_text_program->Enable(false);
// 		m_static_workdir->Enable(false);
// 		m_text_workdir->Enable(false);
// 	}
// 	else
// 	{
// 		m_checkbox_maximized->Enable(true);
// 		m_static_program->Enable(true);
// 		m_text_program->Enable(true);
// 		m_static_workdir->Enable(true);
// 		m_text_workdir->Enable(true);
// 	}
// }

// RDPPerformance::RDPPerformance(wxNotebook *frame, const wxPoint& point, const wxSize& size) : wxPanel(frame, wxID_ANY, point , size ) 
// {
// 	m_static_speed = NULL;
// 	m_choise_speed = NULL;

// 	m_static_performance = NULL;

// 	m_checkbox_enablewallpaper = NULL;
// 	m_checkbox_fullwindowdrag = NULL;
// 	m_checkbox_animation = NULL;
// 	m_checkbox_themes = NULL;
// 	m_checkbox_bitmapcaching = NULL;

// 	sizer_top = new wxBoxSizer(wxVERTICAL);

// 	sizer_speed = new wxBoxSizer(wxHORIZONTAL);
// 	m_static_speed  = new wxStaticText(this, wxID_ANY, wxT("Choise connection speed"),wxDefaultPosition, wxDefaultSize);
// 	ChoisesSpeed[0] = wxT("Modem 28.8 Kbps");
// 	ChoisesSpeed[1] = wxT("Modem 56 Kbps");
// 	ChoisesSpeed[2] = wxT("Broadband 128Kbps - 1.5 Mbps");
// 	ChoisesSpeed[3] = wxT("LAN 10 Mbps or higher");
// 	ChoisesSpeed[4] = wxT("Custom");
// 	m_choise_speed = new wxComboBox(this,ID_COMBOBOX_SPEED,wxT(""),wxDefaultPosition,wxSize(220,-1),5,ChoisesSpeed,wxCB_READONLY) ;
// 	m_choise_speed->SetValue(ChoisesSpeed[1]);
// 	m_choise_speed->SetSelection(1);

// 	sizer_speed->Add(m_static_speed,0,wxALL, 5);
// 	sizer_speed->Add(m_choise_speed,0,wxALL , 2);

// 	sizer_perf = new wxFlexGridSizer(2,10,10);
// 	sizer_window = new wxBoxSizer(wxHORIZONTAL);
	
// 	m_checkbox_bitmapcaching = new wxCheckBox( this, wxID_ANY, wxT("&Bitmap caching"), wxDefaultPosition, wxDefaultSize);
// 	m_checkbox_bitmapcaching->SetValue(true);

// 	sizer_static = new wxBoxSizer(wxHORIZONTAL);
// 	m_static_performance  = new wxStaticText(this, wxID_ANY, wxT("Performance settings"),	wxDefaultPosition, wxDefaultSize);
// 	sizer_static->Add(m_static_performance,0,wxALL | wxALIGN_CENTER, 5);


// 	m_checkbox_enablewallpaper = new wxCheckBox( this, wxID_ANY, wxT("&Desktop wallpaper"),wxDefaultPosition, wxDefaultSize );
// 	m_checkbox_fullwindowdrag = new wxCheckBox( this, wxID_ANY, wxT("&Show contents of window while dragging"),wxDefaultPosition, wxDefaultSize);
// 	m_checkbox_animation = new wxCheckBox( this, wxID_ANY, wxT("&Animation"), wxDefaultPosition, wxDefaultSize );
// 	m_checkbox_themes = new wxCheckBox( this, wxID_ANY, wxT("&Themes"), wxDefaultPosition, wxDefaultSize );
// 	m_checkbox_themes->SetValue(true);

// 	sizer_perf->Add(m_checkbox_bitmapcaching, 0, wxALL | wxALIGN_LEFT ,5);
// 	sizer_perf->Add(m_checkbox_themes , 0, wxALL | wxALIGN_LEFT ,5);
// 	sizer_perf->Add(m_checkbox_enablewallpaper, 0, wxALL | wxALIGN_LEFT ,5);
// 	sizer_perf->Add(m_checkbox_animation, 0, wxALL | wxALIGN_LEFT ,5);

// 	sizer_window->Add(m_checkbox_fullwindowdrag,0,wxALL | wxALIGN_LEFT, 5);
	

// 	sizer_top->Add(sizer_speed,0,wxALL | wxALIGN_LEFT ,5);
// 	sizer_top->Add(sizer_static,0,wxALL | wxALIGN_CENTER ,5);
// 	sizer_top->Add(sizer_perf,0,wxALL | wxALIGN_LEFT ,5);
// 	sizer_top->Add(sizer_window,0,wxALL | wxALIGN_LEFT ,5);

// 	SetSizer(sizer_top);
// 	sizer_top->SetSizeHints(this);
// 	sizer_top->Fit(this); 

// }

// RDPPerformance::~RDPPerformance()
// {
// 	if (m_static_speed) {delete m_static_speed; m_static_speed = NULL;}
// 	if (m_static_speed) {delete m_static_speed; m_choise_speed = NULL;}

// 	if (m_static_speed) {delete m_static_speed; m_static_performance = NULL;}

// 	if (m_static_speed) {delete m_static_speed; m_checkbox_enablewallpaper = NULL;}
// 	if (m_static_speed) {delete m_static_speed; m_checkbox_fullwindowdrag = NULL;}
// 	if (m_static_speed) {delete m_static_speed; m_checkbox_animation = NULL;}
// 	if (m_static_speed) {delete m_static_speed; m_checkbox_themes = NULL;}
// 	if (m_static_speed) {delete m_static_speed; m_checkbox_bitmapcaching = NULL;}
// }

// void RDPPerformance::choise_speed_func(wxCommandEvent &event)
// {
// 	if (!m_choise_speed)	return;

// 	if (m_choise_speed->GetValue() == this->ChoisesSpeed[0])
// 	{
// 		m_checkbox_enablewallpaper->SetValue(false); 
// 		m_checkbox_fullwindowdrag->SetValue(false); 
// 		m_checkbox_animation->SetValue(false);
// 		m_checkbox_themes->SetValue(false);
// 		m_checkbox_bitmapcaching->SetValue(true);
// 		return;
// 	}
// 	if (m_choise_speed->GetValue() == this->ChoisesSpeed[1])
// 	{
// 		m_checkbox_enablewallpaper->SetValue(false); 
// 		m_checkbox_fullwindowdrag->SetValue(false); 
// 		m_checkbox_animation->SetValue(false);
// 		m_checkbox_themes->SetValue(true);
// 		m_checkbox_bitmapcaching->SetValue(true);
// 		return;
// 	}
// 	if (m_choise_speed->GetValue() == this->ChoisesSpeed[2])
// 	{
// 		m_checkbox_enablewallpaper->SetValue(false); 
// 		m_checkbox_fullwindowdrag->SetValue(true); 
// 		m_checkbox_animation->SetValue(true);
// 		m_checkbox_themes->SetValue(true);
// 		m_checkbox_bitmapcaching->SetValue(true);
// 		return;
// 	}
// 	if (m_choise_speed->GetValue() == this->ChoisesSpeed[3])
// 	{
// 		m_checkbox_enablewallpaper->SetValue(true); 
// 		m_checkbox_fullwindowdrag->SetValue(true); 
// 		m_checkbox_animation->SetValue(true);
// 		m_checkbox_themes->SetValue(true);
// 		m_checkbox_bitmapcaching->SetValue(true);
// 		return;
// 	}
// 	if (m_choise_speed->GetValue() == this->ChoisesSpeed[4])
// 	{
// 		m_checkbox_enablewallpaper->SetValue(false); 
// 		m_checkbox_fullwindowdrag->SetValue(false); 
// 		m_checkbox_animation->SetValue(false);
// 		m_checkbox_themes->SetValue(true);
// 		m_checkbox_bitmapcaching->SetValue(true);
// 		return;
// 	}
	
// 	m_checkbox_enablewallpaper->SetValue(false); 
// 	m_checkbox_fullwindowdrag->SetValue(false); 
// 	m_checkbox_animation->SetValue(false);
// 	m_checkbox_themes->SetValue(true);
// 	m_checkbox_bitmapcaching->SetValue(true);
// 	return;

// }
// RDPAdvanced::RDPAdvanced(wxNotebook *frame, const wxPoint& point, const wxSize& size)
// 	: wxPanel(frame, wxID_ANY, point, size ) 
// {
// 	m_static_rdpversion = NULL;
// 	m_choise_rdpversion = NULL; 
// 	m_checkbox_french = NULL;  
// 	m_checkbox_encryption_enable = NULL;	
// 	m_checkbox_backingstore = NULL;
// 	m_checkbox_usemouse = NULL;
// 	m_checkbox_privatecolormap = NULL; 
// 	m_checkbox_numlock = NULL;
// 	m_checkbox_enablecompress = NULL;

// 	m_combobox_keyboard_map = NULL;
// 	m_static_keyboard_map = NULL;

// 	sizer_top = new wxBoxSizer(wxVERTICAL);
// 	sizer_version = new wxBoxSizer(wxHORIZONTAL);

// 	m_static_rdpversion = new wxStaticText(this, wxID_ANY, wxT("Choise RDP version"),wxDefaultPosition, wxDefaultSize);
// 	wxString ChoiseVersion[] = {wxT("Use RDP 4"),wxT("Use RDP 5"),wxT("Use RDP 6")};
// 	m_choise_rdpversion = new wxComboBox(this,wxID_ANY,_(""),wxDefaultPosition,wxSize(220,-1),2,ChoiseVersion,wxCB_READONLY) ;
// 	m_choise_rdpversion->SetValue(ChoiseVersion[1]);
// 	m_choise_rdpversion->SetSelection(1);
// 	sizer_version->Add(m_static_rdpversion,0,wxALL, 5);
// 	sizer_version->Add(m_choise_rdpversion,0,wxALL | wxALIGN_CENTER , 2);

// 	sizer_crypt = new wxBoxSizer(wxHORIZONTAL);
// 	m_checkbox_french	= new wxCheckBox( this, wxID_ANY, wxT("&Enable encrypt for French version"), wxDefaultPosition, wxDefaultSize );
// 	sizer_crypt->Add(m_checkbox_french,0,wxALL, 5);

// 	sizer_settings = new wxFlexGridSizer(2,10,10);

// 	m_checkbox_encryption_enable = new wxCheckBox( this, wxID_ANY, wxT("&Enable encryption"), wxDefaultPosition, wxDefaultSize );	
// 	m_checkbox_backingstore = new wxCheckBox( this, wxID_ANY, wxT("&Use BackingStore"), wxDefaultPosition, wxDefaultSize )	;
// 	m_checkbox_usemouse = new wxCheckBox( this, wxID_ANY, wxT("&Use mouse"), wxDefaultPosition, wxDefaultSize );
// 	m_checkbox_privatecolormap = new wxCheckBox( this, wxID_ANY, wxT("&Use private color map"),wxDefaultPosition, wxDefaultSize );
// 	m_checkbox_numlock = new wxCheckBox(this,wxID_ANY,wxT("&Num Lock sync"),wxDefaultPosition, wxDefaultSize);
// 	m_checkbox_enablecompress = new wxCheckBox(this,wxID_ANY,wxT("&Enable compress"),wxDefaultPosition, wxDefaultSize);

// 	sizer_settings->Add(m_checkbox_encryption_enable, 0, wxALL | wxALIGN_LEFT ,5);
// 	sizer_settings->Add(m_checkbox_backingstore, 0, wxALL | wxALIGN_LEFT ,5);
// 	sizer_settings->Add(m_checkbox_usemouse, 0, wxALL | wxALIGN_LEFT ,5);
// 	sizer_settings->Add(m_checkbox_privatecolormap, 0, wxALL | wxALIGN_LEFT ,5);
// 	sizer_settings->Add(m_checkbox_numlock, 0, wxALL | wxALIGN_LEFT ,5);
// 	sizer_settings->Add(m_checkbox_enablecompress, 0, wxALL | wxALIGN_LEFT ,5);

// 	sizer_keymap = new wxBoxSizer(wxHORIZONTAL);

// 	m_static_keyboard_map = new wxStaticText(this, wxID_ANY, wxT("Choise keyboard map"),wxDefaultPosition, wxDefaultSize);
// #ifdef __WXGTK__
// 	GETRDESKTOPMAPPATH();
// 	wxArrayString list_keyboard_map;
// 	wxString temp_str;
// 	wxDir temp(RDESKTOPMAPPATH);
// 	if ((RDESKTOPMAPPATH != wxT("")) && (temp.Exists(RDESKTOPMAPPATH)))
// 	{
// 		bool check = temp.GetFirst(&temp_str);
// 		list_keyboard_map.Add( wxT("en-us"));
// 		while (check == true)
// 		{
// 			if (temp_str != wxT("en-us"))
// 			{
// 				list_keyboard_map.Add(temp_str);
// 			}
// 			check = temp.GetNext(&temp_str);
// 		}
// 	}
// 	else 
// 	{
// 		list_keyboard_map.Add( wxT("en-us"));
// 	}

// 	m_combobox_keyboard_map = new wxComboBox(this, wxID_ANY,wxT(""),wxPoint(145 + 0.8*SHIFT_W,160), wxSize(120 + 0.8*SHIFT_W,CONTROL_H),
// 		list_keyboard_map,wxCB_READONLY, wxDefaultValidator,wxT(""));
// 	m_combobox_keyboard_map->SetValue(list_keyboard_map[0]);
// 	m_combobox_keyboard_map->SetSelection(0);

// #endif

// #ifdef __WXMSW__
// 	wxString list_keyboard_map[] = {_T("en"),_T("ru")};
// 	m_combobox_keyboard_map = new wxComboBox(this,wxID_ANY, wxT("") , wxDefaultPosition, wxSize(120,-1),2,list_keyboard_map,wxCB_READONLY);
// 	m_combobox_keyboard_map->SetValue(ChoiseVersion[1]);
// 	m_combobox_keyboard_map->SetSelection(1);
// #endif

// 	sizer_keymap->Add(m_static_keyboard_map,0,wxALL, 5);
// 	sizer_keymap->Add(m_combobox_keyboard_map,0,wxALL | wxALIGN_CENTER , 2);

// 	sizer_top->Add(sizer_version,0,wxALL | wxALIGN_LEFT ,5);
// 	sizer_top->Add(sizer_crypt,0,wxALL | wxALIGN_LEFT ,5);
// 	sizer_top->Add(sizer_settings,0,wxALL | wxALIGN_LEFT ,5);
// 	sizer_top->Add(sizer_keymap,0,wxALL | wxALIGN_LEFT ,5);

// 	SetSizer(sizer_top);
// 	sizer_top->SetSizeHints(this);
// 	sizer_top->Fit(this); 

// }

// RDPAdvanced::~RDPAdvanced()
// {
// 	if (m_static_rdpversion != NULL) {delete m_static_rdpversion; m_static_rdpversion = NULL;}
// 	if (m_choise_rdpversion != NULL) {delete m_choise_rdpversion; m_choise_rdpversion = NULL;}
// 	if (m_checkbox_french != NULL) {delete m_checkbox_french; m_checkbox_french = NULL;}
// 	if (m_checkbox_encryption_enable != NULL) {delete m_checkbox_encryption_enable; m_checkbox_encryption_enable = NULL;}
// 	if (m_checkbox_backingstore != NULL) {delete m_checkbox_backingstore; m_checkbox_backingstore = NULL;}
// 	if (m_checkbox_usemouse != NULL) {delete m_checkbox_usemouse; m_checkbox_usemouse = NULL;}
// 	if (m_checkbox_privatecolormap != NULL) {delete m_checkbox_privatecolormap; m_checkbox_privatecolormap = NULL;}
// 	if (m_checkbox_numlock != NULL) {delete m_checkbox_numlock; m_checkbox_numlock = NULL;}
// 	if (m_checkbox_enablecompress != NULL) {delete m_checkbox_enablecompress; m_checkbox_enablecompress = NULL;}

// 	if (m_static_keyboard_map) {delete m_static_keyboard_map; m_static_keyboard_map = NULL;}
// 	if (m_combobox_keyboard_map) {delete m_combobox_keyboard_map; m_combobox_keyboard_map = NULL;}
// }


// ICAAdvanced::ICAAdvanced(wxNotebook *frame, const wxPoint& point, const wxSize& size)
// 	: wxPanel(frame, wxID_ANY, point, size ) 
// {
// 	m_checkbox_ica_application = NULL;
// 	m_text_ica_application = NULL;

// 	m_checkbox_ica_encryption = NULL;
// 	m_static_ica_encryption = NULL;
// 	m_combobox_ica_encryption = NULL;
	
// 	m_static_client_ini = NULL;
// 	m_text_client_ini = NULL;
// 	m_button_client_ini = NULL;
	
// 	m_static_server_ini = NULL;
// 	m_text_server_ini = NULL;
// 	m_button_server_ini = NULL;

// 	m_checkbox_ica_sound = NULL;
// 	m_combobox_ica_sound = NULL;

// 	m_static_file_ica = NULL;
// 	m_text_file_ica = NULL;
// 	m_button_file_ica = NULL;

// /*	m_staticbox_proxy = NULL;
// 	m_radiobox_proxy = NULL;
// 	m_static_proxyhost = NULL;
// 	m_text_proxyhost = NULL;
// 	m_static_proxyport = NULL;
// 	m_text_proxyport = NULL;

// 	m_static_note = NULL;
// 	m_static_username = NULL;
// 	m_text_username = NULL;
// 	m_static_password = NULL;
// 	m_text_password = NULL;
// 	m_button_cert = NULL;
// 	sizer_proxy = NULL;
// */

// 	sizer_main = new wxBoxSizer(wxVERTICAL);
// 	//sizer_client_ini = new wxBoxSizer(wxHORIZONTAL);
// 	//sizer_server_ini = new wxBoxSizer(wxHORIZONTAL);
// 	//sizer_ini_files = new wxFlexGridSizer(2,5,5);
// 	sizer_ini_files = new wxFlexGridSizer(3,5,5);
// 	sizer_ica_encryption = new wxBoxSizer(wxHORIZONTAL);
// 	sizer_ica_sound = new wxBoxSizer(wxHORIZONTAL);

// 	sizer_ica_application = new wxBoxSizer(wxHORIZONTAL);


// 	m_checkbox_ica_application = new wxCheckBox(this,ID_CHECKBOX_ICA_APPLICATION,wxT("Connect to published application:"));
// 	m_text_ica_application = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxSize(140,-1),wxTE_PROCESS_ENTER);
	

// 	m_checkbox_ica_encryption = new wxCheckBox(this,ID_CHECKBOX_ICA_ENCRYPT,wxT("Use encryption"));
// 	m_static_ica_encryption = new wxStaticText(this,wxID_ANY,wxT("Encryption type"));
// 	wxString ica_encryption_types[] = {wxT("Basic (default)"),wxT("RC5 128-bit (only logon)"),wxT("RC5 40-bit"),wxT("RC5 56-bit"),wxT("RC5 128-bit")};
// 	m_combobox_ica_encryption = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,wxDefaultSize,5,ica_encryption_types,wxCB_READONLY);
// 	m_combobox_ica_encryption->SetSelection(0);
// 	m_combobox_ica_encryption->SetValue(ica_encryption_types[0]);
	
// 	m_static_client_ini = new wxStaticText(this,wxID_ANY,wxT("Client *.ini file"));
// 	m_text_client_ini = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxSize(220,-1),wxTE_PROCESS_ENTER);
// 	m_button_client_ini = new wxButton(this,ID_BUTTON_INI_CLIENT,wxT(""),wxDefaultPosition,wxSize(CONTROL_H,CONTROL_H));

// 	m_static_server_ini = new wxStaticText(this,wxID_ANY,wxT("Server *.ini file"));
// 	m_text_server_ini = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxSize(220,-1),wxTE_PROCESS_ENTER);
// 	m_button_server_ini = new wxButton(this,ID_BUTTON_INI_SERVER,wxT(""),wxDefaultPosition,wxSize(CONTROL_H,CONTROL_H));

// 	m_static_file_ica = new wxStaticText(this,wxID_ANY,wxT("*.ica file"));
// 	m_text_file_ica = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxSize(220,-1),wxTE_PROCESS_ENTER);
// 	m_button_file_ica = new wxButton(this,ID_BUTTON_ICA_FILE,wxT(""),wxDefaultPosition,wxSize(CONTROL_H,CONTROL_H));

// 	m_checkbox_ica_encryption->SetValue(true);
// 	//m_static_ica_encryption->Enable(false);
// 	//m_combobox_ica_encryption->Enable(false);

// 	m_checkbox_ica_sound = new wxCheckBox(this,ID_CHECKBOX_ICA_SOUND,wxT("Enable sound"));
// 	wxString ica_sound_types[] = {wxT("Low sound quality"),wxT("Medium sound quality"),wxT("High sound quality")};
// 	m_combobox_ica_sound = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,wxDefaultSize,3,ica_sound_types,wxCB_READONLY); 
	
// 	m_combobox_ica_sound->SetSelection(1);
// 	m_combobox_ica_sound->SetValue(ica_sound_types[1]);
// 	//sizer_file_ica = new wxBoxSizer(wxHORIZONTAL);
	
// 	//m_static_file_ica = new wxStaticText(this,wxID_ANY,wxT("*.ica file"));
// 	//m_text_file_ica = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxSize(220,-1),wxTE_PROCESS_ENTER);
// 	//m_button_file_ica = new wxButton(this,ID_BUTTON_ICA_FILE,wxT(""),wxDefaultPosition,wxSize(25,25));

// 	sizer_ini_files->Add(m_static_client_ini,0,wxALL, 5);
// 	sizer_ini_files->Add(m_text_client_ini,0,wxALL | wxALIGN_RIGHT  , 2);
// 	sizer_ini_files->Add(m_button_client_ini,0,wxALL | wxALIGN_RIGHT, 2);

// 	sizer_ini_files->Add(m_static_server_ini,0,wxALL, 5);
// 	sizer_ini_files->Add(m_text_server_ini,0,wxALL | wxALIGN_RIGHT  , 2);
// 	sizer_ini_files->Add(m_button_server_ini,0,wxALL | wxALIGN_RIGHT, 2);

// 	sizer_ini_files->Add(m_static_file_ica,0,wxALL, 5);
// 	sizer_ini_files->Add(m_text_file_ica,0,wxALL | wxALIGN_RIGHT  , 2);
// 	sizer_ini_files->Add(m_button_file_ica,0,wxALL | wxALIGN_RIGHT, 2);
	
// 	//sizer_file_ica->Add(m_static_file_ica,0,wxALL,5);
// 	//sizer_file_ica->Add(m_text_file_ica,0,wxALL,5);
// 	//sizer_file_ica->Add(m_button_file_ica,0,wxALL,5);
	
// /*
// 	m_staticbox_proxy = new wxStaticBox(this,-1,wxT("Proxy settings"));
// 	sizer_proxy = new wxStaticBoxSizer(m_staticbox_proxy,wxVERTICAL);

// 	sizer_radio_addr = new wxBoxSizer(wxHORIZONTAL);
// 	sizer_proxyaddr = new wxBoxSizer(wxHORIZONTAL);
	
// 	sizer_note_cert = new wxBoxSizer(wxHORIZONTAL);
// 	sizer_radio_addr2 = new wxBoxSizer(wxVERTICAL);

// 	sizer_creds = new wxFlexGridSizer(2,10,10);
// 	sizer_low = new wxBoxSizer(wxHORIZONTAL);

// 	wxString proxy_types[] = {wxT("No proxy"), wxT("SOCKS"), wxT("SSL") };
// 	m_radiobox_proxy = new wxRadioBox(this,ID_RADIOBOX_PROXY,wxT("Using proxy"),wxDefaultPosition,wxDefaultSize,3,proxy_types,0,wxVERTICAL);
// 	m_static_proxyhost = new wxStaticText(this,wxID_ANY,wxT("Address"));
// 	m_text_proxyhost = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition, wxSize(130,-1),wxTE_PROCESS_ENTER);
// 	m_static_proxyport = new wxStaticText(this,wxID_ANY,wxT("Port"));
// 	m_text_proxyport = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxSize(50,-1),wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC));
// 	sizer_proxyaddr->Add(m_static_proxyhost,0,wxALL,5);
// 	sizer_proxyaddr->Add(m_text_proxyhost,0,wxALL,2);
// 	sizer_proxyaddr->Add(m_static_proxyport,0,wxALL,5);
// 	sizer_proxyaddr->Add(m_text_proxyport,0,wxALL,2);

// 	m_bitmap_proxy = new wxStaticBitmap(this,-1,wxArtProvider::GetBitmap(wxART_TICK_MARK));
	

// 	m_static_note = new wxStaticText(this,-1,wxT(""));
// 	m_static_username = new wxStaticText(this,-1,wxT(""));
// 	m_text_username = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxSize(150,-1),wxTE_PROCESS_ENTER);
// 	m_static_password = new wxStaticText(this,-1,wxT("Password"));
// 	m_text_password = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxSize(150,-1),wxTE_PASSWORD);
// 	m_button_cert = new wxButton(this,ID_BUTTON_ICA_CERT,wxT("Select Certificate"),wxDefaultPosition,wxDefaultSize);
	
// 	sizer_creds->Add(m_static_username,0,wxALL,5);
// 	sizer_creds->Add(m_text_username,0,wxALL,2);
// 	sizer_creds->Add(m_static_password,0,wxALL,5);
// 	sizer_creds->Add(m_text_password,0,wxALL,2);

// 	sizer_low->Add(sizer_creds,0,wxALL,5);
// 	sizer_low->Add(m_bitmap_proxy,0,wxALL,5);

// 	sizer_note_cert->Add(m_static_note,0,wxALL | wxALIGN_CENTER_VERTICAL,5);
// 	sizer_note_cert->Add(m_button_cert,0,wxALL,5);

// 	sizer_radio_addr2->Add(sizer_proxyaddr,0,wxALL,1);
// 	sizer_radio_addr2->Add(sizer_note_cert,0,wxALL,1);

// 	sizer_radio_addr->Add(m_radiobox_proxy,0,wxALL,5);
// 	sizer_radio_addr->Add(sizer_radio_addr2,0,wxALL,5);

// 	sizer_proxy->Add(sizer_radio_addr,0,wxALL,5);
// 	//sizer_proxy->Add(m_static_note,0,wxALL,5);
// 	//sizer_proxy->Add(sizer_creds,0,wxALL,5);
// 	sizer_proxy->Add(sizer_low,0,wxALL,5);
// 	//sizer_proxy->Add(m_button_cert,0,wxALL,5);
	
// */
// 	m_checkbox_ica_sound->SetValue(false);
// 	m_combobox_ica_sound->Enable(false);

// 	m_checkbox_ica_application->SetValue(false); 
// 	m_text_ica_application->Enable(false);

// 	sizer_ica_application->Add(m_checkbox_ica_application,0,wxALL,5);
// 	sizer_ica_application->Add(m_text_ica_application,0,wxALL,2);

// 	sizer_ica_encryption->Add(m_checkbox_ica_encryption,0, wxALL,5);
// 	sizer_ica_encryption->Add(m_static_ica_encryption,0, wxALL,6);
// 	sizer_ica_encryption->Add(m_combobox_ica_encryption,0, wxALL | wxEXPAND ,3);

// //	sizer_client_ini->Add(m_static_client_ini,0, wxALL,5);
// //	sizer_client_ini->Add(m_text_client_ini,0, wxALL | wxEXPAND ,5);
// //	sizer_server_ini->Add(m_static_server_ini,0, wxALL,5);
// //	sizer_server_ini->Add(m_text_server_ini,0, wxALL | wxEXPAND ,5);

// /*
// 	sizer_ini_files->Add(m_static_client_ini,0, wxALL,5);
// 	sizer_ini_files->Add(m_text_client_ini,0, wxALL | wxEXPAND ,2);
// 	sizer_ini_files->Add(m_static_server_ini,0, wxALL,5);
// 	sizer_ini_files->Add(m_text_server_ini,0, wxALL | wxEXPAND ,2);
// */
// 	sizer_ica_sound->Add(m_checkbox_ica_sound,0, wxALL,5);
// 	sizer_ica_sound->Add(m_combobox_ica_sound,0, wxALL | wxEXPAND ,2);
	
// 	//sizer_main->Add(m_checkbox_ica_encryption,0,wxALL,5);
	
// 	sizer_main->Add(sizer_ica_application,0,wxALL,5);
// 	sizer_main->Add(sizer_ica_encryption,0,wxALL,5);
// 	sizer_main->Add(sizer_ica_sound,0,wxALL,5);
// 	sizer_main->Add(sizer_ini_files,0,wxALL,5);

// 	//sizer_main->Add(sizer_file_ica,0,wxALL,5);
// 	//sizer_main->Add(sizer_proxy,0,wxALL,5);

// 	//sizer_main->Add(sizer_client_ini,0,wxALL,5);
// 	//sizer_main->Add(sizer_server_ini,0,wxALL,5);
	
// 	SetSizer(sizer_main);
// 	sizer_main->SetSizeHints(this);

// 	//CheckProxySettings();
// }

// ICAAdvanced::~ICAAdvanced()
// {
// 	wxDELETE(m_checkbox_ica_application);
// 	wxDELETE(m_text_ica_application);

// 	wxDELETE(m_checkbox_ica_encryption);
// 	wxDELETE(m_static_ica_encryption);
// 	wxDELETE(m_combobox_ica_encryption);
	
// 	wxDELETE(m_static_client_ini);
// 	wxDELETE(m_text_client_ini);
	
// 	wxDELETE(m_static_server_ini);
// 	wxDELETE(m_text_server_ini);

// 	wxDELETE(m_checkbox_ica_sound);
// 	wxDELETE(m_combobox_ica_sound);

	
// 	wxDELETE(m_static_file_ica);
// 	wxDELETE(m_text_file_ica);
// 	wxDELETE(m_button_file_ica);

// //	wxDELETE(m_staticbox_proxy);
// //	wxDELETE(m_radiobox_proxy);
// //	wxDELETE(m_static_proxyhost);
// //	wxDELETE(m_text_proxyhost);
// //	wxDELETE(m_static_proxyport);
// //	wxDELETE(m_text_proxyport);

// //	wxDELETE(m_static_note);
// //	wxDELETE(m_static_username);
// //	wxDELETE(m_text_username);
// //	wxDELETE(m_button_cert);
	

// }

// void ICAAdvanced::on_button_ica_file(wxCommandEvent & event)
// {
// 	wxFileDialog * openfiledialog = new wxFileDialog(this,wxT("Choose a *.ica connection file"),wxT(""),wxT(""),wxT("*.ica"),wxFD_OPEN);
// 	int iRes;
// 	iRes = openfiledialog->ShowModal();
// 	if (iRes != wxID_OK){ openfiledialog->Destroy() ; return;}
// 	this->m_text_file_ica->SetValue(openfiledialog->GetPath());
// 	openfiledialog->Destroy();

// }

// void ICAAdvanced::on_button_ini_server(wxCommandEvent & event)
// {
// 	wxFileDialog * openfiledialog = new wxFileDialog(this,wxT("Choose a server *.ini file"),wxT(""),wxT(""),wxT("*.ini"),wxFD_OPEN);
// 	int iRes;
// 	iRes = openfiledialog->ShowModal();
// 	if (iRes != wxID_OK){ openfiledialog->Destroy() ; return;}
// 	this->m_text_server_ini->SetValue(openfiledialog->GetPath());
// 	openfiledialog->Destroy();
// }

// void ICAAdvanced::on_button_ini_client(wxCommandEvent & event)
// {
// 	wxFileDialog * openfiledialog = new wxFileDialog(this,wxT("Choose a client *.ini file"),wxT(""),wxT(""),wxT("*.ini"),wxFD_OPEN);
// 	int iRes;
// 	iRes = openfiledialog->ShowModal();
// 	if (iRes != wxID_OK){ openfiledialog->Destroy() ; return;}
// 	this->m_text_client_ini->SetValue(openfiledialog->GetPath());
// 	openfiledialog->Destroy();
// }

// void ICAAdvanced::on_checkbox_ica_application(wxCommandEvent & event)
// {
// 	if (m_checkbox_ica_application->IsChecked())
// 	{	
// 		m_text_ica_application->Enable(true);
// 	}
// 	else
// 	{
// 		m_text_ica_application->Enable(false);
// 	}
// }

// void ICAAdvanced::on_checkbox_ica_encryption(wxCommandEvent & event)
// {
// 	if (m_checkbox_ica_encryption->IsChecked()) 
// 	{
// 		m_static_ica_encryption->Enable(true);
// 		m_combobox_ica_encryption->Enable(true);
// 	}
// 	else
// 	{
// 		m_static_ica_encryption->Enable(false);
// 		m_combobox_ica_encryption->Enable(false);
// 	}
// }

// void ICAAdvanced::on_checkbox_ica_sound(wxCommandEvent & event)
// {
// 	if (m_checkbox_ica_sound->IsChecked()) 
// 	{
// 		m_combobox_ica_sound->Enable(true);
// 	}
// 	else
// 	{
// 		m_combobox_ica_sound->Enable(false);
// 	}
// }
// /*
// void ICAAdvanced::CheckProxySettings()
// {
// 	if (m_radiobox_proxy->GetSelection() == 0)
// 	{
// 		m_static_note->Show(false);
// 		m_static_username->SetLabel(wxT("Username"));
// 		m_static_username->Enable(false);
// 		m_text_username->Enable(false);
// 		m_static_password->Enable(false);
// 		m_text_password->Enable(false);
// 		m_button_cert->Show(false);

// 		m_text_proxyport->SetValue(wxT(""));

// 		m_static_proxyhost->Enable(false);
// 		m_text_proxyhost->Enable(false);
// 		m_static_proxyport->Enable(false);
// 		m_text_proxyport->Enable(false);

// 		sizer_note_cert->Layout(); 
// 	}
// 	else if (m_radiobox_proxy->GetSelection() == 1)
// 	{
// 		m_static_note->Show(true);
// 		m_static_note->SetLabel(wxT("Enter credentials.\nNote about using SOCKS"));
// 		m_static_username->SetLabel(wxT("Username"));
// 		m_static_username->Enable(true);
// 		m_text_username->Enable(true);
// 		m_static_password->Enable(true);
// 		m_text_password->Enable(true);
// 		m_button_cert->Show(false);

// 		m_text_proxyport->SetValue(wxT("1080")); 

// 		m_static_proxyhost->Enable(true);
// 		m_text_proxyhost->Enable(true);
// 		m_static_proxyport->Enable(true);
// 		m_text_proxyport->Enable(true);

// 		sizer_note_cert->Layout();
// 	}
// 	else if (m_radiobox_proxy->GetSelection() == 2)
// 	{
// 		m_static_note->Show(true);
// 		m_static_note->SetLabel(wxT("Select certificate.\nNote about using SSL"));
// 		m_static_username->SetLabel(wxT("Certificate"));
// 		m_static_username->Enable(true);
// 		m_text_username->Enable(true);
// 		m_static_password->Enable(false);
// 		m_text_password->Enable(false);
// 		m_button_cert->Show(true);
		
// 		m_text_proxyport->SetValue(wxT("443"));
		
// 		m_static_proxyhost->Enable(true);
// 		m_text_proxyhost->Enable(true);
// 		m_static_proxyport->Enable(true);
// 		m_text_proxyport->Enable(true);

// 		sizer_note_cert->Layout();
// 	}

// }

// void ICAAdvanced::on_radiobox_proxy(wxCommandEvent & event)
// {
// 	//wxMessageBox(wxT("OK"));
// 	CheckProxySettings();

// }

// void ICAAdvanced::on_button_ica_cert(wxCommandEvent& event)
// {
// #ifdef __WXMSW__
	
// 	LPTSTR pszCertificate = TEXT("");
// 	DWORD cbCertificate = sizeof(pszCertificate);
// 	BOOL fSuccess = FALSE;
// 	HMODULE hModule;
// 	HCERTSTORE hCertStore;
// 	PCCERT_CONTEXT pCertContext;
// 	LPCryptUIDlgSelectCertificateA pCryptUIDlgSelectCertificateA;
// 	CRYPTUI_SELECTCERTIFICATE_STRUCTA SelectCertificate;
	
// 	// Load the CryptUI library.
// 	hModule = LoadLibrary("CryptUI.DLL");
// 	pCryptUIDlgSelectCertificateA = (LPCryptUIDlgSelectCertificateA)GetProcAddress(hModule, "CryptUIDlgSelectCertificateA");
	
// 	// Open the certificate store.
// 	hCertStore = CertOpenStore(
// 		CERT_STORE_PROV_SYSTEM,
// 		0,
// 		NULL,
// 		CERT_SYSTEM_STORE_LOCAL_MACHINE | CERT_STORE_OPEN_EXISTING_FLAG,
// 		L"MY");
// 	if (hCertStore != NULL)
// 	{
// 		// Display the certificate selection dialog.
// 		ZeroMemory(&SelectCertificate, sizeof(SelectCertificate));
// 		SelectCertificate.dwSize = sizeof(SelectCertificate);
// 		SelectCertificate.hwndParent =  (HWND)this->GetHandle() ;
// 		SelectCertificate.dwDontUseColumn = CRYPTUI_SELECT_INTENDEDUSE_COLUMN | CRYPTUI_SELECT_LOCATION_COLUMN;
// 		SelectCertificate.cDisplayStores = 1;
// 		SelectCertificate.rghDisplayStores = &hCertStore;
// 		pCertContext = pCryptUIDlgSelectCertificateA(&SelectCertificate);

// 		if (pCertContext)
// 		{
// 			// Get the certificate name.
// 			CertGetNameString(
// 				pCertContext,
// 				CERT_NAME_FRIENDLY_DISPLAY_TYPE,
// 				0,
// 				NULL,
// 				pszCertificate,
// 				cbCertificate);
			
// 			// Free the certificate context.
// 			CertFreeCertificateContext(pCertContext);
			
// 			// Signal success.
// 			fSuccess = TRUE;
// 		}
		
// 		CertCloseStore(hCertStore, 0);
// 	}
	
// 	// Unload the CryptUI library.
// 	FreeLibrary(hModule);
// 	if (fSuccess)
// 		this->m_text_username->SetValue(wxString(pszCertificate, cbCertificate));
// 	//return fSuccess;

// #endif


// }
// */

// ProxySettings::ProxySettings(wxNotebook *frame, const wxPoint& point, const wxSize& size)
// 	: wxPanel(frame, wxID_ANY, point, size ) 
// {
// 	m_static_proxytype = NULL;
// 	m_radio_noproxy = NULL;
// 	m_radio_socks = NULL;
// 	m_radio_ssl = NULL;
// 	m_hyperlink_cert = NULL;
// 	m_static_proxyhost = NULL;
// 	m_text_proxyhost = NULL;
// 	m_static_proxyport = NULL;
// 	m_text_proxyport = NULL;
// 	m_static_username = NULL;
// 	m_text_username = NULL;
// 	m_static_password = NULL;
// 	m_text_password = NULL;
// 	m_line_options  = NULL;
	
// 	sizer_main = new wxBoxSizer(wxHORIZONTAL);
// 	sizer_options = new wxBoxSizer(wxVERTICAL);

// 	sizer_proxy = new wxBoxSizer(wxVERTICAL);
// 	m_static_proxytype = new wxStaticText(this,-1,wxT("Using proxy:"));
// 	m_radio_noproxy = new wxRadioButton(this,ID_RADIOBUTTON_NOPROXY,wxT("No proxy"),wxDefaultPosition,wxDefaultSize,wxRB_GROUP);
// 	m_radio_socks = new wxRadioButton(this,ID_RADIOBUTTON_SOCKS,wxT("SOCKS"));
// 	m_radio_ssl = new wxRadioButton(this,ID_RADIOBUTTON_SSL,wxT("SSL"));
// 	//m_button_cert = new wxButton(this,ID_BUTTON_CERT,wxT("Select Certificate"));
// 	m_hyperlink_cert = new wxHyperlinkCtrl(this,ID_HYPERLINK_CERT,wxT("Select Certificate"),wxT(""));
// 	sizer_proxy->Add(m_static_proxytype,0,wxALL,5);
// 	sizer_proxy->Add(m_radio_noproxy,0,wxALL,5);
// 	sizer_proxy->Add(m_radio_socks,0,wxALL,5);
// 	sizer_proxy->Add(m_radio_ssl,0,wxALL,5);
// 	sizer_proxy->Add(m_hyperlink_cert,0,wxALL,5);
	
// 	sizer_proxyaddr = new wxBoxSizer(wxHORIZONTAL);
// 	m_static_proxyhost = new wxStaticText(this,wxID_ANY,wxT("Address:"));
// 	m_text_proxyhost = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition, wxSize(120,-1),wxTE_PROCESS_ENTER);
// 	m_static_proxyport = new wxStaticText(this,wxID_ANY,wxT("Port:"));
// 	m_text_proxyport = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxSize(45,-1),wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC));
// 	sizer_proxyaddr->Add(m_static_proxyhost,0,wxALL,5);
// 	sizer_proxyaddr->Add(m_text_proxyhost,0,wxALL,2);
// 	sizer_proxyaddr->Add(m_static_proxyport,0,wxALL,5);
// 	sizer_proxyaddr->Add(m_text_proxyport,0,wxALL,2);

// 	sizer_creds = new wxFlexGridSizer(2,10,10);
// 	m_static_username = new wxStaticText(this,-1,wxT("Username  "));
// 	m_text_username = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxSize(150,-1),wxTE_PROCESS_ENTER);
// 	m_static_password = new wxStaticText(this,-1,wxT("Password  "));
// 	m_text_password = new wxTextCtrl(this,-1,wxT(""),wxDefaultPosition,wxSize(150,-1),wxTE_PASSWORD);
// 	sizer_creds->Add(m_static_username,0,wxALL,5);
// 	sizer_creds->Add(m_text_username,0,wxALL,2);
// 	sizer_creds->Add(m_static_password,0,wxALL,5);
// 	sizer_creds->Add(m_text_password,0,wxALL,2);

// 	//m_line_options = new wxStaticLine(this,wxID_ANY,wxDefaultPosition, wxSize(-1,2) ,wxLI_HORIZONTAL);
// 	sizer_options->Add(sizer_proxyaddr, 0, wxALL, 5);
// 	sizer_options->AddSpacer(20); 
// 	//sizer_options->Add(m_line_options,0,wxALL | wxALIGN_CENTER | wxEXPAND,5);
// 	sizer_options->Add(sizer_creds, 0, wxALL, 5);

// 	sizer_main->AddSpacer(10);
// 	sizer_main->Add(sizer_proxy, 0, wxALL, 10);
// 	sizer_main->Add(sizer_options, 0, wxALL, 10);
	
// 	SetSizer(sizer_main);
// 	sizer_main->SetSizeHints(this);

// 	CheckProxySettings();
// }


// ProxySettings::~ProxySettings()
// {
// 	wxDELETE(m_static_proxytype);
// 	wxDELETE(m_radio_noproxy);
// 	wxDELETE(m_radio_socks);
// 	wxDELETE(m_radio_ssl);
// 	wxDELETE(m_hyperlink_cert);
	
// 	wxDELETE(m_static_proxyhost);
// 	wxDELETE(m_text_proxyhost);
// 	wxDELETE(m_static_proxyport);
// 	wxDELETE(m_text_proxyport);
	
// 	wxDELETE(m_static_username);
// 	wxDELETE(m_text_username);
// 	wxDELETE(m_static_password);
// 	wxDELETE(m_text_password);
// 	wxDELETE(m_line_options);
// }

// void ProxySettings::on_hyperlink_cert(wxHyperlinkEvent& event)
// {
// #ifdef __WXMSW__

// 	LPTSTR pszCertificate = new TCHAR[128];
// 	memset(pszCertificate,0,sizeof(TCHAR) * 128);
// 	DWORD cbCertificate = 128;
// 	BOOL fSuccess = FALSE;
// 	HMODULE hModule;
// 	HCERTSTORE hCertStore;
// 	PCCERT_CONTEXT pCertContext;
// 	LPCryptUIDlgSelectCertificateA pCryptUIDlgSelectCertificateA;
// 	CRYPTUI_SELECTCERTIFICATE_STRUCTA SelectCertificate;
	
// 	hModule = LoadLibrary("CryptUI.DLL");  	// Load the CryptUI library.
// 	pCryptUIDlgSelectCertificateA = (LPCryptUIDlgSelectCertificateA)GetProcAddress(hModule, "CryptUIDlgSelectCertificateA");
	
// 	// Open the certificate store.
// 	hCertStore = CertOpenStore(CERT_STORE_PROV_SYSTEM, 0, NULL, CERT_SYSTEM_STORE_LOCAL_MACHINE | CERT_STORE_OPEN_EXISTING_FLAG, L"MY");
// 	if (hCertStore != NULL)
// 	{
// 		// Display the certificate selection dialog.
// 		ZeroMemory(&SelectCertificate, sizeof(SelectCertificate));
// 		SelectCertificate.dwSize = sizeof(SelectCertificate);
// 		SelectCertificate.hwndParent = /*hWndParent*/ (HWND)this->GetHandle() ;
// 		SelectCertificate.dwDontUseColumn = CRYPTUI_SELECT_INTENDEDUSE_COLUMN | CRYPTUI_SELECT_LOCATION_COLUMN;
// 		SelectCertificate.cDisplayStores = 1;
// 		SelectCertificate.rghDisplayStores = &hCertStore;
// 		pCertContext = pCryptUIDlgSelectCertificateA(&SelectCertificate);

// 		if (pCertContext)
// 		{
// 			//wxMessageBox(wxT("OK"));
// 			// Get the certificate name.
// 			CertGetNameString(pCertContext,CERT_NAME_FRIENDLY_DISPLAY_TYPE,0,NULL,pszCertificate,cbCertificate);
// 			CertFreeCertificateContext(pCertContext); // Free the certificate context.
// 			fSuccess = TRUE;
// 		}
// 		CertCloseStore(hCertStore, 0);
// 	}
// 	FreeLibrary(hModule);

// 	if (fSuccess)
// 	{
// 		this->m_text_username->SetValue(wxString(pszCertificate, cbCertificate));
// 		//MessageBox(NULL,pszCertificate,TEXT("1"),0);
// 	}
// 	delete pszCertificate;

// #endif
// #ifdef __WXGTK__
// 	wxFileDialog * openfiledialog = new wxFileDialog(this,wxT("Choose a installed certificate"),wxT("/usr/share"),wxT(""),wxT("*.cer"),wxFD_OPEN);
// 	int iRes;
// 	iRes = openfiledialog->ShowModal();
// 	if (iRes != wxID_OK){ openfiledialog->Destroy() ; return;}
// 	this->m_text_username->SetValue(openfiledialog->GetPath());
// 	openfiledialog->Destroy();
// #endif

// }

// void ProxySettings::on_radiobutton_noproxy(wxCommandEvent& event)
// {
// 	CheckProxySettings();
// }

// void ProxySettings::on_radiobutton_socks(wxCommandEvent& event)
// {
// 	CheckProxySettings();
// }

// void ProxySettings::on_radiobutton_ssl(wxCommandEvent& event)
// {
// 	CheckProxySettings();
// }

// void ProxySettings::CheckProxySettings(bool set_ports)
// {
// 	if (m_radio_noproxy->GetValue())
// 	{
// 		m_static_username->SetLabel(wxT("Username"));
// 		m_static_username->Enable(false);
// 		m_text_username->Enable(false);
// 		m_static_password->Enable(false);
// 		m_text_password->Enable(false);
// 		m_hyperlink_cert->Show(false);

// 		if (set_ports)
// 			m_text_proxyport->SetValue(wxT(""));

// 		m_static_proxyhost->Enable(false);
// 		m_text_proxyhost->Enable(false);
// 		m_static_proxyport->Enable(false);
// 		m_text_proxyport->Enable(false);

// 		sizer_proxy->Layout();
// 	}
// 	else if (m_radio_socks->GetValue())
// 	{
// 		m_static_username->SetLabel(wxT("Username"));
// 		m_static_username->Enable(true);
// 		m_text_username->Enable(true);
// 		m_static_password->Enable(true);
// 		m_text_password->Enable(true);
// 		m_hyperlink_cert->Show(false);

// 		if (set_ports)
// 			m_text_proxyport->SetValue(wxT("1080")); 

// 		m_static_proxyhost->Enable(true);
// 		m_text_proxyhost->Enable(true);
// 		m_static_proxyport->Enable(true);
// 		m_text_proxyport->Enable(true);

// 		sizer_proxy->Layout();
// 	}
// 	else if (m_radio_ssl->GetValue())
// 	{
// 		m_static_username->SetLabel(wxT("Certificate"));
// 		m_static_username->Enable(true);
// 		m_text_username->Enable(true);
// 		m_static_password->Enable(false);
// 		m_text_password->Enable(false);
// 		m_hyperlink_cert->Show(true);
		
// 		if (set_ports)
// 			m_text_proxyport->SetValue(wxT("443"));
		
// 		m_static_proxyhost->Enable(true);
// 		m_text_proxyhost->Enable(true);
// 		m_static_proxyport->Enable(true);
// 		m_text_proxyport->Enable(true);

// 		sizer_proxy->Layout();
// 	}


// }
///////////////////////////////////////////////////////////////////////////////
//! \brief Processed Cansel button
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPDialog::button_cancel_func(wxCommandEvent &event)
{
   	if (rdp_options_dialog != NULL)
	{
	   delete rdp_options_dialog;
	   rdp_options_dialog = NULL;
	}
   	if (ica_options_dialog != NULL)
	{
	   delete ica_options_dialog;
	   ica_options_dialog = NULL;
	}

	this->EndModal(0); 
}

// // BOOL RDPDialog::FillRDPConn()
// // { 
// // 	wxString strTemp;
// // 	long ltemp = 0;
// // 	strTemp.Empty();
// // 	wxDateTime time = wxDateTime::UNow();
	
// // 	if (new_uniq_name)
// // 	{

// // 		Benc bc;
// // 		prdpconn->uniq_name = bc.generate_uniq_name(rdp_base);
// // //		std::cout << "new uniq name = " << prdpconn->uniq_name << std::endl;

			
// // 	}

// // 	// Protocol type
// // 	if (this->m_combobox_protocol->GetSelection() == 0)
// // 	{
// // 		prdpconn->conn_type = ConnectionType_RDP;
// // 	}
// // 	else if  (this->m_combobox_protocol->GetSelection() == 1)
// // 	{
// // 		prdpconn->conn_type = ConnectionType_ICA;
// // 	}
// // 	else
// // 	{
// // 		prdpconn->conn_type = ConnectionType_Unknown;
// // 	}



// // 	if (this->m_rdpgeneral != NULL)
// // 	{
// // 		if (this->m_rdpgeneral->m_text_server != NULL)
// // 		{
// // 			strTemp = this->m_rdpgeneral->m_text_server->GetValue();
// // 			if (strTemp.Length() == 0)
// // 			{
// // 				wxMessageBox(_("Enter server name or IP address"),_("Error"),wxICON_ERROR);
// // 				return FALSE;
// // 			}
// // 			strTemp.Empty();
// // 		}
// // 		// General settings
// // 		if (this->m_rdpgeneral->m_text_server != NULL)
// // 		{
// // 			prdpconn->hostname = this->m_rdpgeneral->m_text_server->GetValue();
// // 		}
// // 		if (this->m_rdpgeneral->m_text_username != NULL)
// // 		{
// // 			prdpconn->username = this->m_rdpgeneral->m_text_username->GetValue();
// // 		}
// // 		if (this->m_rdpgeneral->m_checkbox_autologon != NULL)
// // 		{
// // 			if (this->m_rdpgeneral->m_checkbox_autologon->IsChecked())
// // 			{
// // 				prdpconn->password = this->m_rdpgeneral->m_text_password->GetValue();
// // 			}
// // 		}
// // 		if (this->m_rdpgeneral->m_text_domain != NULL)
// // 		{
// // 			prdpconn->domain = this->m_rdpgeneral->m_text_domain->GetValue();
// // 		}
// // 		if (this->m_rdpgeneral->m_checkbox_customport != NULL)
// // 		{
// // 			if (this->m_rdpgeneral->m_checkbox_customport->IsChecked())
// // 			{
// // 				prdpconn->port = this->m_rdpgeneral->m_text_port->GetValue();
// // 			}
// // 			else
// // 			{
// // 				if (prdpconn->conn_type == ConnectionType_RDP)
// // 				{
// // 					prdpconn->port = wxString(_("3389"));
// // 				}
// // 				else if (prdpconn->conn_type == ConnectionType_ICA)
// // 				{
// // 					prdpconn->port = wxString(_("1494"));
// // 				}

// // 			}
// // 		}
// // 		if (this->m_rdpgeneral->m_checkbox_attachtoconsole != NULL)
// // 		{
// // 			if (this->m_rdpgeneral->m_checkbox_attachtoconsole->IsChecked())
// // 			{
// // 				prdpconn->attach_to_console = TRUE;
// // 			}
// // 			else
// // 			{
// // 				prdpconn->attach_to_console = FALSE;
// // 			}
// // 		}

// // 		if (this->m_rdpgeneral->m_text_connectionname != NULL)
// // 		{
// // 			prdpconn->connection_name = this->m_rdpgeneral->m_text_connectionname->GetValue();
// // 		}

// // 		if (this->m_rdpgeneral->m_combobox_groupname != NULL)
// // 		{
// // 			prdpconn->group_name = this->m_rdpgeneral->m_combobox_groupname->GetValue();
// // 		}


// // 	}
// // 	// Display settings
// // 	if (this->m_rdpdisplay != NULL)
// // 	{
// // 		if (!m_rdpdisplay->m_checkbox_customgeometry->IsChecked())
// // 		{
// // 			int index = m_rdpdisplay->m_combobox_resolution->GetSelection();
// // 			wxArrayString resolutions = m_rdpdisplay->GetResolutionsList();
// // 			wxSize error_size(0,0);
// // 			if (index >= 0)
// // 			{
// // 				wxSize rdp_size = m_rdpdisplay->GetResolution(index);
// // 				if (rdp_size != error_size)
// // 				{
// // 					wxString str;
// // 					str.Printf(wxT("%d"),rdp_size.x);
// // 					m_rdpdisplay->m_text_width->SetValue(str);
// // 					str.Printf(wxT("%d"),rdp_size.y); 
// // 					m_rdpdisplay->m_text_height->SetValue(str);
// // 				}

// // 			}


// // 		}
// // 		if (this->m_rdpdisplay->m_text_height != NULL)
// // 		{
// // 			strTemp = this->m_rdpdisplay->m_text_height->GetValue();
// // 			if (strTemp.Length() == 0) 
// // 			{
// // 				wxMessageBox(_("Enter display height"),_("Error"));
// // 				return FALSE;
// // 			}
// // 			strTemp.Empty();
// // 		}
// // 		if (this->m_rdpdisplay->m_text_width != NULL)
// // 		{
// // 			strTemp = this->m_rdpdisplay->m_text_width->GetValue();
// // 			if (strTemp.Length() == 0) 
// // 			{
// // 				wxMessageBox(_("Enter display width"),_("Error"));
// // 				return FALSE;
// // 			}
// // 		}
// // 		if (this->m_rdpdisplay->m_text_width != NULL)
// // 		{
// // 			(this->m_rdpdisplay->m_text_width->GetValue()).ToLong(&ltemp);
// // 			prdpconn->width = (ushort)ltemp;
// // 		}
// // 		if (this->m_rdpdisplay->m_text_height != NULL)
// // 		{
// // 			(this->m_rdpdisplay->m_text_height->GetValue()).ToLong(&ltemp);
// // 			prdpconn->heigth = (ushort)ltemp;
// // 		}
// // 		if (this->m_rdpdisplay->m_choise_colordepth != NULL)
// // 		{
// // 			strTemp = this->m_rdpdisplay->m_choise_colordepth->GetValue();
// // 			if (strTemp == _("8-bit")) prdpconn->color_depth = 8;
// // 			else if (strTemp == _("15-bit")) prdpconn->color_depth = 15;
// // 			else if (strTemp == _("16-bit")) prdpconn->color_depth = 16;
// // 			else if (strTemp == _("24-bit")) prdpconn->color_depth = 24;
// // 			else  prdpconn->color_depth = 16;
// // 			strTemp.Empty();
// // 		}

// // 		if (this->m_rdpdisplay->m_checkbox_controlsize != NULL)
// // 		{
// // 			if (this->m_rdpdisplay->m_checkbox_controlsize->IsChecked()) 
// // 			{
// // 				prdpconn->bControlSize = TRUE;
// // 				prdpconn->bFullScreen = FALSE;
// // 				prdpconn->bSmartSizing = FALSE;
// // 			}
// // 			else 
// // 			{
// // 				prdpconn->bControlSize = FALSE;

// // 				if (this->m_rdpdisplay->m_checkbox_fullscreen->IsChecked()) 
// // 				{
// // 					prdpconn->bFullScreen = TRUE;
					
// // 				}
// // 				else prdpconn->bFullScreen = FALSE;
// // 				if (this->m_rdpdisplay->m_checkbox_smartsizing->IsChecked()) prdpconn->bSmartSizing = TRUE;
// // 				else prdpconn->bSmartSizing = FALSE;
// // 			}
// // 		}
// // 	}

// // 	if (this->m_rdpdisplay->m_checkbox_updatescreen != NULL)
// // 	{
// // 		if (this->m_rdpdisplay->m_checkbox_updatescreen->IsChecked())
// // 		{
// // 			prdpconn->force_update_screen = TRUE;
// // 		}
// // 		else
// // 		{
// // 			prdpconn->force_update_screen = FALSE;
// // 		}
// // 	}

// // 	// Share settings

// // 	if (this->m_rdpshare != NULL)
// // 	{
// // 		if (this->m_rdpshare->m_checkbox_drives != NULL)
// // 		{
// // 			if (this->m_rdpshare->m_checkbox_drives->IsChecked())
// // 			{
// // 				prdpconn->bShareDrives = TRUE;
// // 			}
// // 			else
// // 			{
// // 				prdpconn->bShareDrives = FALSE;
// // 			}
// // 		}
// // 		if (this->m_rdpshare->m_checkbox_printers != NULL)
// // 		{
// // 			if (this->m_rdpshare->m_checkbox_printers->IsChecked())
// // 			{
// // 				prdpconn->bSharePrinters  = TRUE;
// // 			}
// // 			else
// // 			{
// // 				prdpconn->bSharePrinters  = FALSE;
// // 			}
// // 		}
// // 		if (this->m_rdpshare->m_checkbox_comports != NULL)
// // 		{
// // 			if (this->m_rdpshare->m_checkbox_comports->IsChecked())
// // 			{
// // 				prdpconn->bShareComPorts = TRUE;
// // 			}
// // 			else
// // 			{
// // 				prdpconn->bShareComPorts = FALSE;
// // 			}
// // 		}
// // 		if (this->m_rdpshare->m_checkbox_smartcards != NULL)
// // 		{
// // 			if (this->m_rdpshare->m_checkbox_smartcards->IsChecked())
// // 			{
// // 				prdpconn->bShareSmartCards = TRUE;
// // 			}
// // 			else
// // 			{
// // 				prdpconn->bShareSmartCards = FALSE;
// // 			}
// // 		}
// // 		if (this->m_rdpshare->m_choise_sound != NULL)
// // 		{
// // 			prdpconn->SoundType = this->m_rdpshare->m_choise_sound->GetCurrentSelection();
// // 		}
// // 		if (this->m_rdpshare->m_choise_keyboard != NULL)
// // 		{
// // 			prdpconn->keyboard = this->m_rdpshare->m_choise_keyboard->GetCurrentSelection();
// // 		}

// // 		if (this->m_rdpshare->m_text_linux_devices != NULL)
// // 		{
// // 			prdpconn->redirect_devices_nix = this->m_rdpshare->m_text_linux_devices->GetValue();
// // 		}
// // 	}

// // 	// Program settings
// // 	if (this->m_rdpprogram != NULL)
// // 	{
// // 		if (this->m_rdpprogram->m_checkbox_program != NULL)
// // 		{
// // 			if (this->m_rdpprogram->m_checkbox_program->IsChecked())
// // 			{
// // 				prdpconn->bUseProgram = TRUE;
// // 				prdpconn->shell = this->m_rdpprogram->m_text_program->GetValue();
// // 				prdpconn->directory = this->m_rdpprogram->m_text_workdir->GetValue();

// // 				if (this->m_rdpprogram->m_checkbox_maximized != NULL)
// // 				{
// // 					if (this->m_rdpprogram->m_checkbox_maximized->IsChecked())
// // 						prdpconn->bProgramMaximized = TRUE;
// // 					else	
// // 						prdpconn->bProgramMaximized = FALSE;

// // 				}
// // 			}
// // 			else
// // 			{
// // 				prdpconn->bUseProgram = FALSE;
// // 			}
// // 		}
// // 	}

// // 	// Performance settings

// // 	if (this->m_rdpperformance != NULL)
// // 	{
// // 		if (this->m_rdpperformance->m_checkbox_bitmapcaching != NULL)
// // 		{
// // 			if (this->m_rdpperformance->m_checkbox_bitmapcaching->IsChecked())
// // 				prdpconn->bEnableBitmapCaching = TRUE;
// // 			else
// // 				prdpconn->bEnableBitmapCaching = FALSE;
// // 		}

// // 		if (this->m_rdpperformance->m_checkbox_animation != NULL)
// // 		{
// // 			if (this->m_rdpperformance->m_checkbox_animation->IsChecked())
// // 				prdpconn->bEnableAnimation = TRUE;
// // 			else
// // 				prdpconn->bEnableAnimation = FALSE;
// // 		}
// // 		if (this->m_rdpperformance->m_checkbox_enablewallpaper != NULL)
// // 		{
// // 			if (this->m_rdpperformance->m_checkbox_enablewallpaper->IsChecked())
// // 				prdpconn->bEnableWallpaper = TRUE;
// // 			else
// // 				prdpconn->bEnableWallpaper = FALSE;
// // 		}
// // 		if (this->m_rdpperformance->m_checkbox_fullwindowdrag != NULL)
// // 		{
// // 			if (this->m_rdpperformance->m_checkbox_fullwindowdrag->IsChecked())
// // 				prdpconn->bEnableFullWindowDrag = TRUE;
// // 			else
// // 				prdpconn->bEnableFullWindowDrag = FALSE;
// // 		}
// // 		if (this->m_rdpperformance->m_checkbox_themes != NULL)
// // 		{
// // 			if (this->m_rdpperformance->m_checkbox_themes->IsChecked())
// // 				prdpconn->bEnableThemes = TRUE;
// // 			else
// // 				prdpconn->bEnableThemes = FALSE;
// // 		}

// // 		if (this->m_rdpperformance->m_choise_speed != NULL)
// // 		{
// // 			prdpconn->bandwidth = this->m_rdpperformance->m_choise_speed->GetCurrentSelection();
// // 		}
// // 	}
// // 	// Advanced settings

// // 	if (this->m_rdpadvanced != NULL)
// // 	{
// // 		if (this->m_rdpadvanced->m_choise_rdpversion != NULL)
// // 		{
// // 			prdpconn->use_rdp_version = this->m_rdpadvanced->m_choise_rdpversion->GetCurrentSelection();
// // 		}
// // 		if (this->m_rdpadvanced->m_checkbox_backingstore != NULL)
// // 		{
// // 			if (this->m_rdpadvanced->m_checkbox_backingstore->IsChecked())
// // 			{
// // 				prdpconn->backing_store = TRUE;
// // 			}
// // 			else
// // 			{
// // 				prdpconn->backing_store = FALSE;
// // 			}
// // 		}
// // 		if (m_rdpadvanced->m_checkbox_enablecompress != NULL)
// // 		{
// // 			if (m_rdpadvanced->m_checkbox_enablecompress->IsChecked()) prdpconn->enable_compres = TRUE; 
// // 			else prdpconn->enable_compres = FALSE;
// // 		}
// // 		if (m_rdpadvanced->m_checkbox_encryption_enable != NULL)
// // 		{
// // 			if (m_rdpadvanced->m_checkbox_encryption_enable->IsChecked()) prdpconn->encription_enable_new = TRUE;
// // 			else prdpconn->encription_enable_new = FALSE;
// // 		}
// // 		if (m_rdpadvanced->m_checkbox_french != NULL)
// // 		{
// // 			if (m_rdpadvanced->m_checkbox_french->IsChecked()) prdpconn->encription_enable_french = TRUE;
// // 			else prdpconn->encription_enable_french = FALSE;
// // 		}
// // 		if (m_rdpadvanced->m_checkbox_numlock != NULL)
// // 		{
// // 			if (m_rdpadvanced->m_checkbox_numlock->IsChecked()) prdpconn->numlock_sync = TRUE;
// // 			else prdpconn->numlock_sync = FALSE;
// // 		}
// // 		if (m_rdpadvanced->m_checkbox_privatecolormap != NULL)
// // 		{
// // 			if (m_rdpadvanced->m_checkbox_privatecolormap->IsChecked() ) prdpconn->private_color_map  = TRUE;
// // 			else  prdpconn->private_color_map  = FALSE;
// // 		}
// // 		if ( m_rdpadvanced->m_checkbox_usemouse != NULL )
// // 		{
// // 			if (m_rdpadvanced->m_checkbox_usemouse->IsChecked()  ) prdpconn->send_mouse_event = TRUE;
// // 			else prdpconn->send_mouse_event = FALSE;
// // 		}
// // 		if ( m_rdpadvanced->m_combobox_keyboard_map != NULL )
// // 		{
// // 			prdpconn->keyboard_map = m_rdpadvanced->m_combobox_keyboard_map->GetValue(); 
// // 		}
// // 	}

// // 	// ICA settings
// // 	if (this->m_icaadvanced != NULL)
// // 	{
// // 		if (this->m_icaadvanced->m_text_server_ini != NULL)
// // 		{
// // 			prdpconn->server_ini = m_icaadvanced->m_text_server_ini->GetValue();
// // 		}
// // 		if (this->m_icaadvanced->m_text_client_ini != NULL)
// // 		{
// // 			prdpconn->client_ini = m_icaadvanced->m_text_client_ini->GetValue();
// // 		}
// // 		if (this->m_icaadvanced->m_text_file_ica != NULL)
// // 		{
// // 			prdpconn->ica_file = m_icaadvanced->m_text_file_ica->GetValue();
// // 		}

// // 		if (this->m_icaadvanced->m_checkbox_ica_application != NULL)
// // 		{
// // 			if (m_icaadvanced->m_checkbox_ica_application->IsChecked()  ) prdpconn->bUseApplication = TRUE;
// // 			else prdpconn->bUseApplication = FALSE;
// // 		}

// // 		if (this->m_icaadvanced->m_text_ica_application != NULL)
// // 		{
// // 			prdpconn->IcaApplication = m_icaadvanced->m_text_ica_application->GetValue();
// // 		}


// // 		if (this->m_icaadvanced->m_checkbox_ica_sound != NULL)
// // 		{
// // 			if (m_icaadvanced->m_checkbox_ica_sound->IsChecked()  ) prdpconn->bIcaSound = TRUE;
// // 			else prdpconn->bIcaSound = FALSE;
// // 		}

// // 		if (this->m_icaadvanced->m_combobox_ica_sound != NULL)
// // 		{
// // 			int iIcaSoundMode = m_icaadvanced->m_combobox_ica_sound->GetCurrentSelection();
// // 			if (iIcaSoundMode == 0)
// // 			{
// // 				prdpconn->bIcaSoundType = 0;
// // 			}
// // 			else if (iIcaSoundMode == 1)
// // 			{
// // 				prdpconn->bIcaSoundType = 1;
// // 			}
// // 			else if (iIcaSoundMode == 2)
// // 			{
// // 				prdpconn->bIcaSoundType = 2;
// // 			}
// // 			else
// // 			{
// // 				prdpconn->bIcaSoundType = 1;
// // 			}
// // 		}

// // 		if (this->m_icaadvanced->m_checkbox_ica_encryption != NULL)
// // 		{
// // 			if (m_icaadvanced->m_checkbox_ica_encryption->IsChecked()  ) prdpconn->bIcaEncryption = TRUE;
// // 			else prdpconn->bIcaEncryption = FALSE;
// // 		}

// // 		if (this->m_icaadvanced->m_combobox_ica_encryption != NULL)
// // 		{
// // 			int iIcaEncryptionType = m_icaadvanced->m_combobox_ica_encryption->GetCurrentSelection();
// // 			if (iIcaEncryptionType < 0 || iIcaEncryptionType > 4) iIcaEncryptionType = 0;
// // 			prdpconn->ica_encryption = iIcaEncryptionType;

// // 			/*
// // 			if (iIcaEncryptionType == 0)
// // 			{
// // 				prdpconn->ica_encryption = 0;
// // 			}
// // 			else if (iIcaEncryptionType == 1)
// // 			{
// // 				prdpconn->ica_encryption = RC5Logon;
// // 			}
// // 			else if (iIcaEncryptionType == 2)
// // 			{
// // 				prdpconn->ica_encryption = RC5_40;
// // 			}
// // 			else if (iIcaEncryptionType == 3)
// // 			{
// // 				prdpconn->ica_encryption = RC5_56;
// // 			}
// // 			else if (iIcaEncryptionType == 4)
// // 			{
// // 				prdpconn->ica_encryption = RC5_128;
// // 			}
// // 			else
// // 			{
// // 				prdpconn->ica_encryption = Basic;
// // 			}
// // 			*/
// // 		}

// // 	}

// // 	// Proxy settings
// // 	if (this->m_proxysettings != NULL)
// // 	{
// // 		if (m_proxysettings->m_radio_noproxy->GetValue())
// // 		{	
// // 			prdpconn->bProxyType = 0;
// // 		}
// // 		else if (m_proxysettings->m_radio_socks->GetValue())
// // 		{	
// // 			prdpconn->bProxyType = 1;
// // 		}
// // 		else if (m_proxysettings->m_radio_ssl->GetValue())
// // 		{	
// // 			prdpconn->bProxyType = 2;
// // 		}
// // 		else
// // 		{
// // 			prdpconn->bProxyType = 0;
// // 		}


// // 		if (this->m_proxysettings->m_text_proxyhost != NULL)
// // 		{
// // 			prdpconn->ProxyAddr = m_proxysettings->m_text_proxyhost->GetValue();
// // 		}
// // 		if (this->m_proxysettings->m_text_proxyport != NULL)
// // 		{
// // 			prdpconn->ProxyPort = m_proxysettings->m_text_proxyport->GetValue();
// // 		}
// // 		if (this->m_proxysettings->m_text_username != NULL)
// // 		{
// // 			prdpconn->ProxyUserId = m_proxysettings->m_text_username->GetValue();
// // 		}
// // 		if (this->m_proxysettings->m_text_password != NULL)
// // 		{
// // 			prdpconn->ProxyPassword = m_proxysettings->m_text_password->GetValue();
// // 		}

// // 	}

// // 	return TRUE;

// // }


// void RDPDialog::button_ok_func(wxCommandEvent &event) 
// {
//    //if (!FillRDPConn())
// //	{
// //		return;
// //	}
//    //
   
//    this->EndModal(1);
// }

// // void RDPDialog::LoadRDPConn()
// // {
// // 	// Protocol
// // 	if (prdpconn->conn_type == ConnectionType_RDP)
// // 	{
// // 		m_combobox_protocol->SetSelection(0); 
// // 		m_combobox_protocol->SetValue(wxT("RDP Protocol")); 
// // 	}
// // 	else if (prdpconn->conn_type == ConnectionType_ICA)
// // 	{
// // 		m_combobox_protocol->SetSelection(1);
// // 		m_combobox_protocol->SetValue(wxT("ICA Protocol"));
// // 	}


// // 	// General settings
// // 	m_rdpgeneral->m_text_server->SetValue(prdpconn->hostname);
// // 	m_rdpgeneral->m_text_username->SetValue(prdpconn->username);
// // 	if (prdpconn->password.Length() > 0)
// // 	{
// // 		m_rdpgeneral->m_checkbox_autologon->SetValue(true);	
// // 		m_rdpgeneral->m_text_password->SetValue(prdpconn->password);
// // 	}
// // 	else m_rdpgeneral->m_checkbox_autologon->SetValue(false);	

// // 	m_rdpgeneral->m_text_domain->SetValue(prdpconn->domain);

// // 	if (prdpconn->port != wxString(_("3389")))
// // 	{
// // 		m_rdpgeneral->m_checkbox_customport->SetValue(true);
// // 		m_rdpgeneral->m_text_port->SetValue(prdpconn->port);
// // 	}
// // 	else m_rdpgeneral->m_checkbox_customport->SetValue(false);

	
// // 	if (prdpconn->attach_to_console) m_rdpgeneral->m_checkbox_attachtoconsole->SetValue(true); 
// // 	else m_rdpgeneral->m_checkbox_attachtoconsole->SetValue(false); 

	
// // 	if (prdpconn->group_name.Length() > 0)
// // 	{
// // 		m_rdpgeneral->m_combobox_groupname->SetValue(prdpconn->group_name);
// // 	}

// // 	if (prdpconn->connection_name.Length() > 0)
// // 	{
// // 		m_rdpgeneral->m_text_connectionname->SetValue(prdpconn->connection_name);
// // 	}

// // 	// Display settings
// // 	wxString str;
// // 	str.Printf(wxT("%d"),prdpconn->width);
// // 	m_rdpdisplay->m_text_width->SetValue(str);
// // 	str.Printf(wxT("%d"),prdpconn->heigth); 
// // 	m_rdpdisplay->m_text_height->SetValue(str);

// // 	wxSize rdp_size = wxSize(prdpconn->width,prdpconn->heigth);
// // 	wxArrayString resolutions = m_rdpdisplay->GetResolutionsList();
// // 	bool bCustomGeometryFlag = true;
// // 	for (int i = 0; i < (int)resolutions.Count(); i ++)
// // 	{
// // 		wxSize temp_size = m_rdpdisplay->GetResolution(i);
// // 		if (temp_size == rdp_size)
// // 		{
// // 			m_rdpdisplay->m_combobox_resolution->SetSelection(i);
// // 			m_rdpdisplay->m_combobox_resolution->SetValue(resolutions.Item(i));
// // 			bCustomGeometryFlag = false;
// // 		}
// // 	}
// // 	m_rdpdisplay->m_checkbox_customgeometry->SetValue(bCustomGeometryFlag);

// // 	BYTE bTemp = prdpconn->color_depth;
// // 	if (bTemp == 8) m_rdpdisplay->m_choise_colordepth->SetValue(_("8-bit"));
// // 	else if (bTemp == 15) m_rdpdisplay->m_choise_colordepth->SetValue(_("15-bit"));
// // 	else if (bTemp == 16) m_rdpdisplay->m_choise_colordepth->SetValue(_("16-bit"));
// // 	else if (bTemp == 24) m_rdpdisplay->m_choise_colordepth->SetValue(_("24-bit"));
// // 	else m_rdpdisplay->m_choise_colordepth->SetValue(_("16-bit"));

// // 	if (prdpconn->bFullScreen) m_rdpdisplay->m_checkbox_fullscreen->SetValue(true);
// // 	else  m_rdpdisplay->m_checkbox_fullscreen->SetValue(false);

// // 	if (prdpconn->bSmartSizing) m_rdpdisplay->m_checkbox_smartsizing->SetValue(true);
// // 	else  m_rdpdisplay->m_checkbox_smartsizing->SetValue(false);
	
// // 	if (prdpconn->bControlSize) m_rdpdisplay->m_checkbox_controlsize->SetValue(true);
// // 	else
// // 	{
// // 		m_rdpdisplay->m_checkbox_controlsize->SetValue(false);
// // 	}
	
// // 	if (!prdpconn->bControlSize)
// // 	{
// // 		m_rdpdisplay->m_checkbox_fullscreen->Enable(true);
// // 		if (!prdpconn->bFullScreen)
// // 		{
// // 			m_rdpdisplay->m_static_width->Enable(true);
// // 			m_rdpdisplay->m_text_width->Enable(true);
// // 			m_rdpdisplay->m_static_height->Enable(true);
// // 			m_rdpdisplay->m_text_height->Enable(true);
// // 		}
// // 		else
// // 		{
// // 			m_rdpdisplay->m_static_width->Enable(false);
// // 			m_rdpdisplay->m_text_width->Enable(false);
// // 			m_rdpdisplay->m_static_height->Enable(false);
// // 			m_rdpdisplay->m_text_height->Enable(false);
// // 		}
// // 	}
// // 	else
// // 	{
// // 		m_rdpdisplay->m_checkbox_fullscreen->Enable(false);
// // 		m_rdpdisplay->m_static_width->Enable(false);
// // 		m_rdpdisplay->m_text_width->Enable(false);
// // 		m_rdpdisplay->m_static_height->Enable(false);
// // 		m_rdpdisplay->m_text_height->Enable(false);
// // 	}
// // 	m_rdpdisplay->CheckRDPDisplay();

// // 	// Share settings
// // 	if (prdpconn->bShareDrives) m_rdpshare->m_checkbox_drives->SetValue(true);
// // 	else m_rdpshare->m_checkbox_drives->SetValue(false);

// // 	if (prdpconn->bSharePrinters) m_rdpshare->m_checkbox_printers->SetValue(true);
// // 	else m_rdpshare->m_checkbox_printers->SetValue(false);

// // 	if (prdpconn->bShareComPorts) m_rdpshare->m_checkbox_comports->SetValue(true);
// // 	else m_rdpshare->m_checkbox_comports->SetValue(false);

// // 	if (prdpconn->bShareSmartCards) m_rdpshare->m_checkbox_smartcards->SetValue(true);
// // 	else m_rdpshare->m_checkbox_smartcards->SetValue(false);

// // 	m_rdpshare->m_choise_sound->SetSelection(prdpconn->SoundType);
// // 	m_rdpshare->m_choise_keyboard->SetSelection(prdpconn->keyboard);

// // 	if (prdpconn->redirect_devices_nix.Length() > 0)
// // 	{
// // 		m_rdpshare->m_text_linux_devices->SetValue(prdpconn->redirect_devices_nix);
// // 	}

// // 	// Program settings
	
// // 	if (prdpconn->bUseProgram)
// // 	{
// // 		m_rdpprogram->m_checkbox_program->SetValue(true);
// // 		m_rdpprogram->m_text_program->SetValue(prdpconn->shell);
// // 		m_rdpprogram->m_text_workdir->SetValue(prdpconn->directory);
// // 		m_rdpprogram->m_checkbox_maximized->Enable(true);
// // 		m_rdpprogram->m_static_program->Enable(true);
// // 		m_rdpprogram->m_text_program->Enable(true);
// // 		m_rdpprogram->m_static_workdir->Enable(true);
// // 		m_rdpprogram->m_text_workdir->Enable(true);
// // 		if (prdpconn->bProgramMaximized)
// // 		{
// // 			m_rdpprogram->m_checkbox_maximized->SetValue(true);
// // 		}
// // 		else
// // 		{
// // 			m_rdpprogram->m_checkbox_maximized->SetValue(false);
// // 		}
// // 	}
// // 	else
// // 	{
// // 		m_rdpprogram->m_checkbox_program->SetValue(false);
// // 		m_rdpprogram->m_checkbox_maximized->Enable(false);
// // 		m_rdpprogram->m_static_program->Enable(false);
// // 		m_rdpprogram->m_text_program->Enable(false);
// // 		m_rdpprogram->m_static_workdir->Enable(false);
// // 		m_rdpprogram->m_text_workdir->Enable(false);
// // 	}


// // 	// Performance settings
// // 	if (prdpconn->bEnableBitmapCaching) m_rdpperformance->m_checkbox_bitmapcaching->SetValue(true);
// // 	else m_rdpperformance->m_checkbox_bitmapcaching->SetValue(false);

// // 	if (prdpconn->bEnableAnimation) m_rdpperformance->m_checkbox_animation->SetValue(true);
// // 	else m_rdpperformance->m_checkbox_animation->SetValue(false);

// // 	if (prdpconn->bEnableWallpaper) m_rdpperformance->m_checkbox_enablewallpaper->SetValue(true);
// // 	else m_rdpperformance->m_checkbox_enablewallpaper->SetValue(false);

// // 	if (prdpconn->bEnableFullWindowDrag) m_rdpperformance->m_checkbox_fullwindowdrag->SetValue(true);
// // 	else m_rdpperformance->m_checkbox_fullwindowdrag->SetValue(false);

// // 	if (prdpconn->bEnableThemes) m_rdpperformance->m_checkbox_themes->SetValue(true);
// // 	else m_rdpperformance->m_checkbox_themes->SetValue(false);

// // 	m_rdpperformance->m_choise_speed->SetValue(m_rdpperformance->ChoisesSpeed[4]);

// // 	//Advanced settings
// // 	if (prdpconn->backing_store) m_rdpadvanced->m_checkbox_backingstore->SetValue(true);
// // 	else m_rdpadvanced->m_checkbox_backingstore->SetValue(false);

// // 	if (prdpconn->enable_compres) m_rdpadvanced->m_checkbox_enablecompress->SetValue(true);
// // 	else m_rdpadvanced->m_checkbox_enablecompress->SetValue(false);

// // 	if (prdpconn->encription_enable_new) m_rdpadvanced->m_checkbox_encryption_enable->SetValue(true);
// // 	else m_rdpadvanced->m_checkbox_encryption_enable->SetValue(false);

// // 	if (prdpconn->encription_enable_french) m_rdpadvanced->m_checkbox_french->SetValue(true);
// // 	else m_rdpadvanced->m_checkbox_french->SetValue(false);

// // 	if (prdpconn->numlock_sync) m_rdpadvanced->m_checkbox_numlock->SetValue(true);
// // 	else m_rdpadvanced->m_checkbox_backingstore->SetValue(false);

// // 	if (prdpconn->private_color_map) m_rdpadvanced->m_checkbox_privatecolormap->SetValue(true);
// // 	else m_rdpadvanced->m_checkbox_privatecolormap->SetValue(false);

// // 	if (prdpconn->send_mouse_event) m_rdpadvanced->m_checkbox_usemouse->SetValue(true);
// // 	else m_rdpadvanced->m_checkbox_usemouse->SetValue(false);

// // 	m_rdpadvanced->m_choise_rdpversion->SetSelection(prdpconn->use_rdp_version);
	
// // 	m_rdpadvanced->m_combobox_keyboard_map->SetValue(prdpconn->keyboard_map);

// // 	// ICA settings
// // 	if (prdpconn->server_ini.Length() > 0) m_icaadvanced->m_text_server_ini->SetValue(prdpconn->server_ini);
// // 	if (prdpconn->client_ini.Length() > 0) m_icaadvanced->m_text_client_ini->SetValue(prdpconn->client_ini);
// // 	if (prdpconn->ica_file.Length() > 0) m_icaadvanced->m_text_file_ica->SetValue(prdpconn->ica_file);

// // 	if (prdpconn->bUseApplication) m_icaadvanced->m_checkbox_ica_application->SetValue(true);
// // 	else m_icaadvanced->m_checkbox_ica_application->SetValue(false);

// // 	if (prdpconn->IcaApplication.Length() > 0) m_icaadvanced->m_text_ica_application->SetValue(prdpconn->IcaApplication);
// // 	if (m_icaadvanced->m_checkbox_ica_application->IsChecked())
// // 	{
// // 		m_icaadvanced->m_text_ica_application->Enable(true);
// // 	}
// // 	else
// // 	{
// // 		m_icaadvanced->m_text_ica_application->Enable(false);
// // 	}	


// // 	if (prdpconn->bIcaEncryption) m_icaadvanced->m_checkbox_ica_encryption->SetValue(true);
// // 	else m_icaadvanced->m_checkbox_ica_encryption->SetValue(false);

// // 	if (prdpconn->ica_encryption == 0/*Basic*/)
// // 	{
// // 		m_icaadvanced->m_combobox_ica_encryption->SetValue(wxT("Basic (default)"));
// // 	}
// // 	else if (prdpconn->ica_encryption == 1/*RC5Logon*/)
// // 	{
// // 		m_icaadvanced->m_combobox_ica_encryption->SetValue(wxT("RC5 128-bit (only logon)"));
// // 	}
// // 	else if (prdpconn->ica_encryption == 2/*RC5_40*/)
// // 	{
// // 		m_icaadvanced->m_combobox_ica_encryption->SetValue(wxT("RC5 40-bit"));
// // 	}
// // 	else if (prdpconn->ica_encryption == 3/*RC5_56*/)
// // 	{
// // 		m_icaadvanced->m_combobox_ica_encryption->SetValue(wxT("RC5 56-bit"));
// // 	}
// // 	else if (prdpconn->ica_encryption == 4/*RC5_128*/)
// // 	{
// // 		m_icaadvanced->m_combobox_ica_encryption->SetValue(wxT("RC5 128-bit"));
// // 	}
// // 	else
// // 	{
// // 		m_icaadvanced->m_combobox_ica_encryption->SetValue(wxT("Basic (default)"));
// // 	}

// // 	if (m_icaadvanced->m_checkbox_ica_encryption->IsChecked())
// // 	{
// // 		m_icaadvanced->m_combobox_ica_encryption->Enable(true);
// // 	}
// // 	else
// // 	{
// // 		m_icaadvanced->m_combobox_ica_encryption->Enable(false);
// // 	}

// // 	if (prdpconn->bIcaSound) m_icaadvanced->m_checkbox_ica_sound->SetValue(true);
// // 	else m_icaadvanced->m_checkbox_ica_sound->SetValue(false);

// // 	if (m_icaadvanced->m_checkbox_ica_sound->IsChecked())
// // 	{
// // 		m_icaadvanced->m_combobox_ica_sound->Enable(true);
// // 	}
// // 	else
// // 	{
// // 		m_icaadvanced->m_combobox_ica_sound->Enable(false);
// // 	}

// // 	if (prdpconn->bIcaSoundType == 0)
// // 	{
// // 		m_icaadvanced->m_combobox_ica_sound->SetValue(wxT("Low sound quality"));
// // 	}
// // 	else if (prdpconn->bIcaSoundType == 1)
// // 	{
// // 		m_icaadvanced->m_combobox_ica_sound->SetValue(wxT("Medium sound quality"));
// // 	}
// // 	else if (prdpconn->bIcaSoundType == 2)
// // 	{
// // 		m_icaadvanced->m_combobox_ica_sound->SetValue(wxT("High sound quality"));
// // 	}
// // 	else
// // 	{
// // 		m_icaadvanced->m_combobox_ica_sound->SetValue(wxT("Medium sound quality"));
// // 	}
	
// // 	//Proxy settings
// // 	if (prdpconn->bProxyType == 0)
// // 	{
// // 		m_proxysettings->m_radio_noproxy->SetValue(true);
// // 	}
// // 	else if (prdpconn->bProxyType == 1)
// // 	{
// // 		m_proxysettings->m_radio_socks->SetValue(true);
// // 	}
// // 	else if (prdpconn->bProxyType == 2)
// // 	{
// // 		m_proxysettings->m_radio_ssl->SetValue(true);
// // 	}
// // 	else
// // 	{
// // 		m_proxysettings->m_radio_noproxy->SetValue(true);
// // 	}

// // 	m_proxysettings->m_text_proxyhost->SetValue(prdpconn->ProxyAddr);
// // 	m_proxysettings->m_text_proxyport->SetValue(prdpconn->ProxyPort);
// // 	m_proxysettings->m_text_username->SetValue(prdpconn->ProxyUserId);
// // 	m_proxysettings->m_text_password->SetValue(prdpconn->ProxyPassword);

// // 	m_proxysettings->CheckProxySettings(false);
// // }

///////////////////////////////////////////////////////////////////////////////
//! \brief Process Save button
//! \param 
//! \return 
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPDialog::button_save_func(wxCommandEvent &event)
{
   Benc bc;
//   RDPConn lrdpconn;
// #ifdef __WXMSW__
//    lrdpconn.uniq_name = prdpconn->uniq_name;
// #endif
// #ifdef __WXGTK__
//    if (new_uniq_name)
//    {
//       lrdpconn.uniq_name = 0;
//    }
//    else
//    {
//       lrdpconn.uniq_name = prdpconn->uniq_name;
//    }
// #endif	

   Options_HashMap local_options = Get_Options();
   if (local_options.size() > 0 )
   {
      //wxMessageBox(local_options[wxT("uniq_name")]);
      main_frame->Add_Connections_Record(&local_options);
      //bc.Save(&main_frame->all_connection_records);
      
      this->EndModal(2);
   }

}

// void RDPDialog::dialog_hotkeys(wxKeyEvent &event)
// {
//    wxCommandEvent evt;
//    evt.SetId(1); 
//    switch(event.GetKeyCode())
//    {
//       case WXK_RETURN:
// 	 //this->button_ok_func(evt);


// 	 break;
//       case WXK_ESCAPE:
// 	 //this->button_cancel_func(evt);
// 	 break;

//       // case WXK_TAB:
//       // 	 if (event.GetModifiers() == wxMOD_SHIFT)
//       // 	 {
//       // 	    if (notebook)
//       // 	    {
//       // 	       size_t iSel = (notebook->GetSelection() + 1) % (notebook->GetPageCount());
//       // 	       notebook->SetSelection(iSel);
//       // 	    }
//       // 	 }
//       // 	 break;
//       default:
// 	 break;

//    }
//    event.Skip();
// }
