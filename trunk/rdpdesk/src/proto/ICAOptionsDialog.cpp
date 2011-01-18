///////////////////////////////////////////////////////////////////////////////
// File name:   ICAOptionsDialog.cpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
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
ICAOptionsDialog::ICAOptionsDialog(wxWindow *parent_element) :
parent(parent_element),
	ica_general(NULL),
	ica_display(NULL),
	ica_advanced(NULL),
	ica_sound(NULL),
	ica_security(NULL)
{
	save_options.clear();

	error = parent != NULL ? 0 : ICA_ERROR_PARENT_IS_NULL;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
ICAOptionsDialog::~ICAOptionsDialog() {
//   size_t end;
//
//   if(this != NULL)
//   {
//      end = this->GetPageCount();
	DeleteAllPages();
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
int ICAOptionsDialog::Build() {
	if (error != 0)
		return error;

	const bool state = Create(parent, wxID_ANY);
	if (state == false) {
		error = ICA_ERROR_CANT_CREATE_NOTEBOOK;
		return error;
	}

	ica_general = new ICAOptionsDialog_General(this);
	ica_general->SetGroupList(arrayStringGroupList);
	ica_general->Build();
	ica_display = new ICAOptionsDialog_Display(this);
	ica_display->Build();
	ica_sound = new ICAOptionsDialog_Sound(this);
	ica_sound->Build();
	ica_advanced = new ICAOptionsDialog_Advanced(this);
	ica_advanced->Build();
	ica_security = new ICAOptionsDialog_Security(this);
	ica_security->Build();
	AddPage(ica_general,  _("General"));
	AddPage(ica_display,  _("Display"));
	AddPage(ica_sound,    _("Share"));
	AddPage(ica_advanced, _("Advance"));
	AddPage(ica_security, _("Security"));

	return error;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
bool ICAOptionsDialog::SetOptions (const Options_HashMap *options) {
	bool state = false;
	if (options != NULL) {
		save_options = *options;
		//wxMessageBox(save_options[wxT("uniq_name")]);
		if (ica_general != NULL)
			ica_general->SetOptions(options);

		if (ica_display != NULL)
			ica_display->SetOptions(options);
		// if (ica_share != NULL)
		// 		ica_share->SetOptions(options);
		if (ica_sound != NULL)
			ica_sound->SetOptions(options);
		if (ica_security != NULL)
			ica_security->SetOptions(options);
		if (ica_advanced != NULL)
			ica_advanced->SetOptions(options);
	}
	return state;
}

void ICAOptionsDialog::SetGroupList(const wxArrayString &group_list) {
	arrayStringGroupList = group_list;

//   if (rdp_general != NULL)
//   {
//      rdp_general->SetGroupList(group_list);
//   }
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap ICAOptionsDialog::Get_Options () {
	//
	Options_HashMap options, temp_options;
	Options_HashMap::iterator it;

//   bool option_found;

	if (ica_general != NULL) {
		options.clear();
		options = ica_general->Get_Options();
	}
	if (ica_display != NULL && options.size() > 0) {
		temp_options.clear();
		temp_options = ica_display->Get_Options();
		for (it = temp_options.begin(); it != temp_options.end(); ++it) {
			const wxString key = it->first, value = it->second;
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
	if (ica_sound != NULL && options.size() > 0) {
		temp_options.clear();
		temp_options = ica_sound->Get_Options();
		for (it = temp_options.begin(); it != temp_options.end(); ++it) {
			const wxString key = it->first, value = it->second;
			options[key] = value;
		}
	}

	if (ica_advanced != NULL && options.size() > 0) {
		temp_options.clear();
		temp_options = ica_advanced->Get_Options();
		for (it = temp_options.begin(); it != temp_options.end(); ++it) {
			const wxString key = it->first, value = it->second;
			options[key] = value;
		}
	}
	if (ica_security != NULL && options.size() > 0) {
		temp_options.clear();
		temp_options = ica_security->Get_Options();
		for (it = temp_options.begin(); it != temp_options.end(); ++it) {
			const wxString key = it->first, value = it->second;
			options[key] = value;
		}
	}

	if (options.size() > 0) {
		for (it = options.begin(); it != options.end(); ++it) {
			const wxString key = it->first, value = it->second;
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
  : parent(parent_element),
	m_flexgrid_sizer_1(NULL), m_flexgrid_sizer_2(NULL), m_flexgrid_sizer_3(NULL),

	m_boxsizer_1(NULL), m_boxsizer_2(NULL),

	staticServer(NULL), textServer(NULL),

	staticPort(NULL), textPort(NULL),

	staticUsername(NULL), textUsername(NULL),

	staticPasswd(NULL), textPasswd(NULL),

	staticDomain(NULL), textDomain(NULL),

	checkboxAutologon(NULL), checkboxCustomPort(NULL),

	staticGroupName(NULL), comboboxGroupName(NULL),

	staticConnectionName(NULL), textConnectionName(NULL),

	staticProxyAutoconfigUrl(NULL), textProxyAutoconfigUrl(NULL),

	staticProxyUsername(NULL), textProxyUsername(NULL),

	staticProxyPassword(NULL), textProxyPassword(NULL),

	staticProxyHost(NULL), textProxyHost(NULL),

	staticProxyType(NULL), comboboxProxyType(NULL),

	staticProxyBypassList(NULL), textProxyBypassList(NULL),

	staticProxyTimeout(NULL), spinctrlProxyTimeout(NULL),

	staticSslProxyHost(NULL), textSslProxyHost(NULL),

	checkboxSslEnable(NULL), checkboxProxyUseDefault(NULL),

	groupcount(0)
{
	error = parent != NULL ? 0 : ICA_ERROR_PARENT_IS_NULL;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
ICAOptionsDialog_General::~ICAOptionsDialog_General() {
	delete staticServer;
	delete textServer;
	delete staticPort;
	delete textPort;
	delete staticUsername;
	delete textUsername;
	delete staticPasswd;
	delete textPasswd;
	delete staticDomain;
	delete textDomain;
	delete checkboxAutologon;
	delete checkboxCustomPort;
	delete staticGroupName;
	delete comboboxGroupName;
	delete staticConnectionName;
	delete textConnectionName;
	delete staticProxyAutoconfigUrl;
	delete textProxyAutoconfigUrl;
	delete staticProxyUsername;
	delete textProxyUsername;
	delete staticProxyPassword;
	delete textProxyPassword;
	delete staticProxyHost;
	delete textProxyHost;
	delete staticProxyType;
	delete comboboxProxyType;
	delete staticProxyBypassList;
	delete textProxyBypassList;
	delete staticProxyTimeout;
	delete spinctrlProxyTimeout;
	delete staticSslProxyHost;
	delete textSslProxyHost;
	delete checkboxSslEnable;
	delete checkboxProxyUseDefault;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
int ICAOptionsDialog_General::Build() {
	if (error != 0)
		return error;

	const bool state = Create(parent, wxID_ANY, wxDefaultPosition,
	                          wxDefaultSize, wxTAB_TRAVERSAL);
	if (state == false) {
		error = ICA_ERROR_CANT_CREATE_PANEL;
		return error;
	}
	m_boxsizer_1 = new wxBoxSizer(wxVERTICAL);
	m_flexgrid_sizer_1 = new wxFlexGridSizer(0, 2, 0, 0);
	m_flexgrid_sizer_1->AddGrowableCol(1);

	staticConnectionName = new wxStaticText(this, wxID_ANY, _("Connection name"));
	textConnectionName = new wxTextCtrl(this, wxID_ANY, wxT(""),
	                                    wxDefaultPosition, wxDefaultSize,
	                                    wxTE_PROCESS_ENTER);
	staticGroupName = new wxStaticText(this, wxID_ANY, _("Group name"));

	for (size_t i = 0; i < arrayStringGroupList.Count(); ++i)
		if (arrayStringGroupList.Item(i) == wxT("Main"))
			arrayStringGroupList.RemoveAt(i);

	arrayStringGroupList.Insert(wxT("Main"), 0);
	comboboxGroupName = new wxComboBox(this, wxID_ANY, wxT(""),
	                                   wxDefaultPosition, wxDefaultSize,
	                                   arrayStringGroupList);
	m_flexgrid_sizer_1->Add(staticConnectionName, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
	                        5);
	m_flexgrid_sizer_1->Add(textConnectionName, 1,
	                        wxALL|wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	m_flexgrid_sizer_1->Add(staticGroupName, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_flexgrid_sizer_1->Add(comboboxGroupName, 1,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	staticServer = new wxStaticText(this, wxID_ANY, _("Server"));
	textServer = new wxTextCtrl(this, wxID_ANY, wxT(""),
	                            wxDefaultPosition, wxDefaultSize,
	                            wxTE_PROCESS_ENTER);
	checkboxCustomPort = new wxCheckBox(this, ID_ICA_CHECKBOX_CUSTOMPORT,
	                                    _("&Use custom port"));
	m_flexgrid_sizer_2 = new wxFlexGridSizer(0, 3, 0, 0);
	m_flexgrid_sizer_2->AddGrowableCol(1);
	m_flexgrid_sizer_2->Add(staticServer, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_flexgrid_sizer_2->Add(textServer, 1,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	m_flexgrid_sizer_2->Add(checkboxCustomPort, 0,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	staticUsername = new wxStaticText(this, wxID_ANY, _("Username"));
	textUsername = new wxTextCtrl(this, wxID_ANY, wxT("Administrator"),
	                              wxDefaultPosition, wxDefaultSize,
	                              wxTE_PROCESS_ENTER);
	staticPort = new wxStaticText(this, wxID_ANY, _("Port"));
	textPort = new wxTextCtrl(this, wxID_ANY, wxT("1494"),
	                          wxDefaultPosition, wxDefaultSize,
	                          wxTE_PROCESS_ENTER,
	                          wxTextValidator(wxFILTER_NUMERIC));
	m_boxsizer_2 = new wxBoxSizer(wxHORIZONTAL);
	m_flexgrid_sizer_2->Add(staticUsername, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_flexgrid_sizer_2->Add(textUsername, 1,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	m_boxsizer_2->Add(staticPort, 0,
	                  wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                  5);
	m_boxsizer_2->Add(textPort, 1,
	                  wxALL |wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,
	                  5);
	m_flexgrid_sizer_2->Add(m_boxsizer_2, 0,
	                        wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	staticPasswd = new wxStaticText(this, wxID_ANY, _("Password"));
	textPasswd = new wxTextCtrl(this, ID_ICA_TEXTCTRL_PASSWORD, wxT(""),
	                            wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	checkboxAutologon = new wxCheckBox(this, ID_ICA_CHECKBOX_AUTOLOGON,
	                                   _("&Auto logon"));
	m_flexgrid_sizer_2->Add(staticPasswd, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_flexgrid_sizer_2->Add(textPasswd, 1,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	m_flexgrid_sizer_2->Add(checkboxAutologon, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
	                        5);
	staticDomain = new wxStaticText(this, wxID_ANY, _("Domain"));
	textDomain = new wxTextCtrl(this, wxID_ANY, wxT(""),
	                            wxDefaultPosition, wxDefaultSize,
	                            wxTE_PROCESS_ENTER);
	m_flexgrid_sizer_2->Add(staticDomain, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_flexgrid_sizer_2->Add(textDomain, 1,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	textPort->Enable(false);
	staticPort->Enable(false);
	comboboxGroupName->SetValue(wxT("Main"));
	comboboxGroupName->SetSelection(0);

	m_flexgrid_sizer_3 = new wxFlexGridSizer(0, 4, 0, 0);
	m_flexgrid_sizer_3->AddGrowableCol(1);
	m_boxsizer_3 = new wxBoxSizer(wxHORIZONTAL);
	staticProxyAutoconfigUrl = new wxStaticText(this, wxID_ANY,
	                                            _("Proxy AutoConfig URL"));
	textProxyAutoconfigUrl = new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_flexgrid_sizer_3->Add(staticProxyAutoconfigUrl, 0,
							wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_flexgrid_sizer_3->Add(textProxyAutoconfigUrl, 1,
							wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
							5);
	staticProxyHost = new wxStaticText(this, wxID_ANY, _("Proxy Host"));
	m_flexgrid_sizer_3->Add(staticProxyHost, 0,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	textProxyHost = new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_flexgrid_sizer_3->Add(textProxyHost, 1,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	staticProxyUsername = new wxStaticText(this, wxID_ANY, _("Proxy Username"));
	m_flexgrid_sizer_3->Add(staticProxyUsername, 0,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	textProxyUsername = new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_flexgrid_sizer_3->Add(textProxyUsername, 1,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);

	// Proxy type
	staticProxyType = new wxStaticText(this, wxID_ANY, _("Proxy Type"));
	m_flexgrid_sizer_3->Add(staticProxyType, 0,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	const wxString proxy_type_array[] = { _("None"),
	                                      _("Auto"),
	                                      _("Socks"),
	                                      _("SocksV4"),
	                                      _("SocksV5"),
	                                      _("Secure"),
	                                      _("Script") };
	comboboxProxyType = new wxComboBox(this, wxID_ANY, wxT(""),
	                                   wxDefaultPosition, wxDefaultSize,
	                                   6,
	                                   proxy_type_array,
	                                   wxCB_READONLY);
	comboboxProxyType->SetSelection(0);
	m_flexgrid_sizer_3->Add(comboboxProxyType, 1,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	// Proxy password
	staticProxyPassword = new wxStaticText(this, wxID_ANY, _("Proxy Password"));
	m_flexgrid_sizer_3->Add(staticProxyPassword, 0,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	textProxyPassword = new wxTextCtrl(this, wxID_ANY, wxT(""),
	                                   wxDefaultPosition, wxDefaultSize,
	                                   wxTE_PASSWORD);
	m_flexgrid_sizer_3->Add(textProxyPassword, 1,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	staticProxyBypassList = new wxStaticText(this, wxID_ANY, _("Proxy Bypass List"));
	m_flexgrid_sizer_3->Add(staticProxyBypassList, 0,
	                        wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	textProxyBypassList = new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_flexgrid_sizer_3->Add(textProxyBypassList, 0,
	                        wxALL| wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	staticProxyTimeout = new wxStaticText(this, wxID_ANY, _("Proxy Timeout"));
	m_flexgrid_sizer_3->Add(staticProxyTimeout, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	spinctrlProxyTimeout = new wxSpinCtrl(this, wxID_ANY, wxT("20"),
	                                      wxDefaultPosition, wxDefaultSize,
	                                      0, 0, 100, 0);
	spinctrlProxyTimeout->SetValue(wxT("20"));
	m_flexgrid_sizer_3->Add(spinctrlProxyTimeout, 0,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	staticSslProxyHost = new wxStaticText(this, wxID_ANY, _("SSL Proxy Host"));
	m_flexgrid_sizer_3->Add(staticSslProxyHost, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	textSslProxyHost = new wxTextCtrl(this, wxID_ANY, wxT(""));
	m_flexgrid_sizer_3->Add(textSslProxyHost, 1,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	checkboxSslEnable = new wxCheckBox(this, ID_ICA_SSL_ENABLE, _("SSL Enable"));
	checkboxSslEnable->SetValue(false);
	m_flexgrid_sizer_3->Add(checkboxSslEnable, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	checkboxProxyUseDefault = new wxCheckBox(this, wxID_ANY, _("Proxy Use Default"));
	checkboxProxyUseDefault->SetValue(false);
	m_flexgrid_sizer_3->Add(checkboxProxyUseDefault, 0,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);

	m_boxsizer_1->Add(m_flexgrid_sizer_1, 0,
	                  wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                  5);
	m_boxsizer_1->Add(m_flexgrid_sizer_2, 0,
	                  wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                  5);
	m_boxsizer_1->Add(m_boxsizer_3, 0,
	                  wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                  5);
	m_boxsizer_1->Add(-1, -1, 1,
	                  wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                  5);
	m_boxsizer_1->Add(m_flexgrid_sizer_3, 0,
	                  wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                  5);
	SetSizer(m_boxsizer_1);
	m_boxsizer_1->SetSizeHints(this);

	return error;
}


///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void ICAOptionsDialog_General::GetGroupList() {
}

void ICAOptionsDialog_General::checkbox_customport() {
	textPort->Enable(checkboxCustomPort->IsChecked());
	staticPort->Enable(checkboxCustomPort->IsChecked());
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
void ICAOptionsDialog_General::text_passwordentry_func(wxCommandEvent &event) {
	switch(event.GetId()) {
		case ID_ICA_TEXTCTRL_PASSWORD:
		{
			const wxString strTemp(this->textPasswd->GetValue());
			checkboxAutologon->SetValue(strTemp.Len() != 0);
			break;
		}
		case ID_ICA_CHECKBOX_AUTOLOGON:
			if (!checkboxAutologon->IsChecked())
	            textPasswd->SetValue(wxT(""));
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap ICAOptionsDialog_General::Get_Options() {
	Options_HashMap local_options;
	ICA_ERRORS error = ICA_ERROR_OK;

	local_options.clear();

	if (textServer != NULL && !textServer->GetValue().IsEmpty())
		local_options[wxT("hostname")] = textServer->GetValue();
	else
		error = ICA_ERROR_EMPTY_HOSTNAME;

	if (textUsername != NULL)
		local_options[wxT("username")] = textUsername->GetValue();

	if(textPasswd != NULL)
		local_options[wxT("password")] = textPasswd->GetValue();

	if(textDomain != NULL)
		local_options[wxT("domain")] = textDomain->GetValue();
	if(checkboxCustomPort != NULL) {
		local_options[wxT("custom_port")] =
			wxString::Format(wxT("%i"), checkboxCustomPort->GetValue());
	}

	if(textPort != NULL)
		local_options[wxT("port")] = textPort->GetValue();

	//if(m_checkbox_attachtoconsole != NULL)
	// {
	//    local_options[wxT("attach_to_console")] = wxString::Format(wxT("%i"),
	// 								m_checkbox_attachtoconsole->GetValue());
	// }
	if (comboboxGroupName != NULL)
		local_options[wxT("group_name")] = comboboxGroupName->GetValue();

	if (textConnectionName != NULL)
		local_options[wxT("connection_name")] = textConnectionName->GetValue();
	if (textProxyAutoconfigUrl != NULL)
		local_options[wxT("proxy_autoconfig_url"]) = textProxyAutoconfigUrl->GetValue();
	if (textProxyUsername != NULL)
		local_options[wxT("proxy_username"]) = textProxyUsername->GetValue();
	if (textProxyPassword != NULL)
		local_options[wxT("proxy_password"]) = textProxyPassword->GetValue();
	if (textProxyHost != NULL)
		local_options[wxT("proxy_host"]) = textProxyHost->GetValue();
	if (comboboxProxyType != NULL)
		local_options[wxT("proxy_type"]) =
			wxString::Format(wxT("%i"), comboboxProxyType->GetCurrentSelection());
	if (textProxyBypassList != NULL)
		local_options[wxT("proxy_bypass_list"]) = textProxyBypassList->GetValue();

	if (spinctrlProxyTimeout != NULL)
		local_options[wxT("proxy_timeout"]) =
			wxString::Format(wxT("%i"), spinctrlProxyTimeout->GetValue());

	if (textSslProxyHost != NULL)
		local_options[wxT("ssl_proxy_host"]) = textSslProxyHost->GetValue();

	if (checkboxSslEnable != NULL)
		local_options[wxT("ssl_enable"]) =
			wxString::Format(wxT("%i"), checkboxSslEnable->GetValue());
	if (checkboxProxyUseDefault != NULL)
		local_options[wxT("proxy_use_default"]) =
			wxString::Format(wxT("%i"), checkboxProxyUseDefault->GetValue());

	if (error != ICA_ERROR_OK)
		local_options.clear();
	return local_options;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
bool ICAOptionsDialog_General::SetOptions(const Options_HashMap *all_options) {
	bool state = false;
	Options_HashMap local_options(*all_options);

//   size_t count, index = 0;
//   count = all_options->GetCount();
//   for (index = 0; index < count ; index++)
//   {

	if (textServer != NULL)
		textServer->SetValue(local_options[wxT("hostname")]);
	if (textUsername != NULL)
		textUsername->SetValue(local_options[wxT("username")]);
	if ((textPasswd != NULL))
		textPasswd->SetValue(local_options[wxT("password")]);
	if ((textDomain != NULL))
		textDomain->SetValue(local_options[wxT("domain")]);
	if ((checkboxCustomPort != NULL))
		checkboxCustomPort->SetValue(wxAtoi(local_options[wxT("custom_port")]));
	if ((textPort != NULL))
		textPort->SetValue(local_options[wxT("port")]);
	// if ((m_checkbox_attachtoconsole != NULL))
	// {
	//    m_checkbox_attachtoconsole->SetValue(local_options[wxT("attach_to_console")]);
	// }
	if ((comboboxGroupName != NULL))
		comboboxGroupName->SetValue(local_options[wxT("group_name")]);
	if ((textConnectionName != NULL))
		textConnectionName->SetValue(local_options[wxT("connection_name")]);
	if (textProxyAutoconfigUrl != NULL)
		textProxyAutoconfigUrl->SetValue(local_options[wxT("proxy_autoconfig_url"]));
	if (textProxyUsername != NULL)
		textProxyUsername->SetValue(local_options[wxT("proxy_username"]));
	if (textProxyPassword != NULL)
		textProxyPassword->SetValue(local_options[wxT("proxy_password"]));
	if (textProxyHost != NULL)
		textProxyHost->SetValue(local_options[wxT("proxy_host"]));
	if (comboboxProxyType != NULL)
		comboboxProxyType->SetSelection(wxAtoi(local_options[wxT("proxy_type"])));
	if (textProxyBypassList != NULL)
		textProxyBypassList->SetValue(local_options[wxT("proxy_bypass_list"]));
	if (spinctrlProxyTimeout != NULL)
		spinctrlProxyTimeout->SetValue(wxAtoi(local_options[wxT("proxy_timeout"])));
	if (textSslProxyHost != NULL)
		textSslProxyHost->SetValue(local_options[wxT("ssl_proxy_host"]));
	if (checkboxSslEnable != NULL)
		checkboxSslEnable->SetValue(wxAtoi(local_options[wxT("ssl_enable"])));
	if (checkboxProxyUseDefault != NULL)
		checkboxProxyUseDefault->SetValue(wxAtoi(local_options[wxT("proxy_use_default"])));

	return state;
}

void ICAOptionsDialog_General::SetGroupList(const wxArrayString &groupList) {
//   wxString group[256];

//   std::cout << group_list.Count() << std::endl;

//   if ((comboboxGroupName != NULL))
//   {

	for (size_t i = 0; i < groupList.Count(); ++i)
		arrayStringGroupList.Add(groupList.Item(i));
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
ICAOptionsDialog_Display::ICAOptionsDialog_Display(ICAOptionsDialog *parent_element)
  : parent(parent_element)
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
	checkboxEnableSessionSharingScreen = NULL;
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

	error = parent != NULL ? 0 : ICA_ERROR_PARENT_IS_NULL;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
int ICAOptionsDialog_Display::Build() {
	if (error != 0)
		return error;

	const bool state = Create(parent, wxID_ANY,
	                          wxDefaultPosition, wxDefaultSize,
	                          wxTAB_TRAVERSAL);
	if (state == false) {
		error = ICA_ERROR_CANT_CREATE_PANEL;
		return error;
	}

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
	                                              _("&Control size"));
	m_radiobutton_controlsize->SetValue(true);
	m_radiobutton_fullscreen = new wxRadioButton(this, ID_ICA_CHK_CONTROLSIZE,
	                                             _("&Full screen"));
	m_radiobutton_customgeometry = new wxRadioButton(this, ID_ICA_CHK_CONTROLSIZE,
	                                                 _("&Custom geometry"));
	m_radiobutton_presetsize = new wxRadioButton(this, ID_ICA_CHK_CONTROLSIZE,
	                                             _("&Preset geometry"));
	m_boxsizer_3->Add(m_radiobutton_controlsize, 1,
	                  wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_boxsizer_3->Add(m_radiobutton_fullscreen, 1,
	                  wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_boxsizer_3->Add(m_radiobutton_customgeometry, 1,
	                  wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_boxsizer_3->Add(m_radiobutton_presetsize, 1,
	                  wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);

	m_static_resolution = new wxStaticText(this, wxID_ANY, _("Screen resolution"));
	m_combobox_resolution = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition,
										   wxDefaultSize,resolutions,wxCB_READONLY);
	const int sel = GetDefaultResolution();
	if (sel >=0) {
		m_combobox_resolution->SetValue(resolutions.Item(sel));
		m_combobox_resolution->SetSelection(sel);
	}

	m_static_width = new wxStaticText(this, wxID_ANY, _("Screen width"));
	m_text_width = new wxTextCtrl(this, wxID_ANY, wxT("800"),
	                              wxDefaultPosition, wxDefaultSize,
	                              wxTE_PROCESS_ENTER,
	                              wxTextValidator(wxFILTER_NUMERIC));
	m_static_height = new wxStaticText(this, wxID_ANY, _("Screen height"));
	m_text_height = new wxTextCtrl(this, wxID_ANY, wxT("600"),
	                               wxDefaultPosition,
	                               wxDefaultSize,
	                               wxTE_PROCESS_ENTER,
	                               wxTextValidator(wxFILTER_NUMERIC));
	m_static_desired_color = new wxStaticText(this, wxID_ANY, _("Desired Color"));
	wxString Choises[] = {_("16 colors"),
	                      _("256 colors"),
	                      _("15-bit color"),
	                      _("24-bit color"),
	                      _("Auto")};
	m_combo_desired_color = new wxComboBox(this, ID_ICA_DESIRED_COLOR, wxT(""),
	                                       wxDefaultPosition,
	                                       wxDefaultSize,
	                                       5,
	                                       Choises,
	                                       wxCB_READONLY);
	m_combo_desired_color->SetValue(Choises[4]);
	m_combo_desired_color->SetSelection(4);

	m_flexgrid_sizer_1->Add(m_static_width, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
	                        5);
	m_flexgrid_sizer_1->Add(m_text_width, 0,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	m_flexgrid_sizer_1->Add(-1, -1, 1,
	                        wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);

	m_flexgrid_sizer_1->Add(m_static_height, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_flexgrid_sizer_1->Add(m_text_height, 0,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	m_flexgrid_sizer_1->Add(-1, -1, 1,
	                        wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);

	m_flexgrid_sizer_1->Add(m_static_resolution, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_flexgrid_sizer_1->Add(m_combobox_resolution, 0,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_flexgrid_sizer_1->Add(-1, -1, 1,
	                        wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);

	m_flexgrid_sizer_1->Add(m_static_desired_color, 1,
	                        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_flexgrid_sizer_1->Add(m_combo_desired_color, 0,
	                        wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	m_flexgrid_sizer_1->Add(-1, -1, 1,
	                        wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                        5);
	m_boxsizer_2->Add(m_boxsizer_3, 0,
	                  wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
	m_boxsizer_2->Add(m_flexgrid_sizer_1, 1,
	                  wxALL | wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL, 5);

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
	m_checkbox_allow_degraded_colors = new wxCheckBox(this, wxID_ANY,
	                                                  _("Allow Degraded Colors"));
	m_checkbox_allow_degraded_colors->SetValue(true);
	m_boxsizer_10->Add(m_checkbox_allow_degraded_colors, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	m_checkbox_enable_oss = new wxCheckBox(this, wxID_ANY, _("Enable OSS"));
	m_checkbox_enable_oss->SetValue(true);
	m_boxsizer_10->Add(m_checkbox_enable_oss, 0, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	checkboxEnableSessionSharingScreen =
		new wxCheckBox(this, wxID_ANY, _("Enable Session Sharing Screen"));
	checkboxEnableSessionSharingScreen->SetValue(false);

	m_boxsizer_10->Add(checkboxEnableSessionSharingScreen, 0,
	                   wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                   5);
	// m_boxsizer_10->Add(m_checkbox_updatescreen, 1,
	// 		    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	// m_boxsizer_10->Add(m_checkbox_smartsizing, 1,
	// 		    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_boxsizer_10->Add(-1, -1, 1,
	                   wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                   5);

	m_boxsizer_7->Add(m_boxsizer_10, 1,
	                  wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                  5);
	m_boxsizer_6->Add(m_boxsizer_7, 0,
	                  wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                  5);
	m_boxsizer_8 = new wxBoxSizer(wxVERTICAL);
	m_boxsizer_9 = new wxBoxSizer(wxHORIZONTAL);
	// m_static_desired_color = new wxStaticText(this, wxID_ANY, wxT("Desired Color"),
	// 					   wxDefaultPosition, wxDefaultSize);
	// m_boxsizer_9->Add(m_static_desired_color, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	// m_combo_desired_color = new wxComboBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	// m_boxsizer_9->Add(m_combo_desired_color, 0,
	// 		   wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	// m_boxsizer_9->Add(-1,-1,1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_boxsizer_8->Add(m_boxsizer_9, 0, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	m_checkbox_approximate_colors = new wxCheckBox(this, wxID_ANY,
	                                               _("Approximate Colors"));
	m_checkbox_approximate_colors->SetValue(false);
	m_boxsizer_8->Add(m_checkbox_approximate_colors, 0,
	                  wxALL | wxALIGN_LEFT | wxALIGN_TOP, 5);
	m_boxsizer_12 = new wxBoxSizer(wxHORIZONTAL);
	m_static_use_sdlvb = new wxStaticText(this, wxID_ANY, _("Use SDLVB"));
	m_boxsizer_12->Add(m_static_use_sdlvb, 0,
	                   wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                   5);
	wxString use_sdlvb[] ={wxT("0"),
	                       wxT("1"),
	                       wxT("2"),
	                       wxT("3"),
	                       wxT("4"),
	                       wxT("5")};
	m_combo_use_sdlvb = new wxComboBox(this,wxID_ANY,wxT(""),
	                                   wxDefaultPosition,
	                                   wxDefaultSize,
	                                   5,
	                                   use_sdlvb,
	                                   wxCB_READONLY);
	m_combo_use_sdlvb->SetValue(Choises[3]);
	m_combo_use_sdlvb->SetSelection(3);
	m_boxsizer_12->Add(m_combo_use_sdlvb, 0,
	                   wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                   5);
	m_boxsizer_12->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_boxsizer_8->Add(m_boxsizer_12, 0, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	m_boxsizer_6->Add(m_boxsizer_8, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	//
	m_boxsizer_11 = new wxBoxSizer(wxHORIZONTAL);
	m_static_font_smoothing_type = new wxStaticText(this, wxID_ANY,
	                                                _("Font Smoothing Type"));
	m_boxsizer_11->Add(m_static_font_smoothing_type, 0,
	                   wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,
	                   5);
	wxString font_smoothing_type[] = {wxT(""),
	                                  _("no smoothing"),
	                                  _("standard smoothing"),
	                                  _("ClearType")};
	m_combo_font_smoothing_type = new wxComboBox(this,
	                                             wxID_ANY,
	                                             wxT(""),
	                                             wxDefaultPosition,
	                                             wxDefaultSize,
	                                             4,
	                                             font_smoothing_type,
	                                             wxCB_READONLY);
	m_combo_font_smoothing_type->SetValue(Choises[0]);
	m_combo_font_smoothing_type->SetSelection(0);
	m_boxsizer_11->Add(m_combo_font_smoothing_type, 0,
	                   wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_static_screen_percent = new wxStaticText(this, wxID_ANY, _("Screen Percent"));
	m_boxsizer_11->Add(m_static_screen_percent, 0,
	                   wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_combo_screen_percent = new wxSpinCtrl(this, wxID_ANY, wxT("100"), wxDefaultPosition,
	                                        wxDefaultSize, 0, 0, 100, 100);
	m_combo_screen_percent->SetValue(wxT("100"));
	m_boxsizer_11->Add(m_combo_screen_percent, 0,
	                   wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_checkbox_resize_session = new wxCheckBox(this, wxID_ANY, _("Resize Session"));
	m_checkbox_resize_session->SetValue(false);
	m_boxsizer_1->Add(m_boxsizer_2, 0,
					  wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_boxsizer_1->Add(m_boxsizer_5, 0,
					  wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_boxsizer_1->Add(m_boxsizer_6, 0,
					  wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_boxsizer_1->Add(m_boxsizer_11, 0,
					  wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_boxsizer_1->Add(m_checkbox_resize_session, 0,
					  wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_boxsizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(m_boxsizer_1);
	m_boxsizer_1->Fit(this);
	m_boxsizer_1->SetSizeHints(this);
	CheckICADisplay();
	change_desired_color();

	return error;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
wxArrayString ICAOptionsDialog_Display::GetResolutionsList() {
	wxDisplay disp;
	wxArrayString resolutions;
#ifdef __WXMSW__
	wxArrayVideoModes avm = disp.GetModes();
	for (int i = 0; i < (int)avm.Count(); ++i) {
		if (avm.Item(i).w < 800 ||
		    avm.Item(i).h < 600 ||
		    avm.Item(i).h > avm.Item(i).w)
			continue;
	  const wxString temp(
			wxString::Format(wxT("%i x %i"),avm.Item(i).w,avm.Item(i).h));
		BOOL flag = FALSE;
		for (int j = 0; j < (int)resolutions.Count(); j++)
			if (resolutions.Item(j) == temp)
				flag = TRUE;
	  if (!flag)
			resolutions.Add(temp);
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
wxSize ICAOptionsDialog_Display::GetResolution(int i) {
	const wxSize error_size(0,0);
	if (i < 0)
		return error_size;
	const wxArrayString resolutions = GetResolutionsList();
	if (i >= (int)resolutions.Count())
		return error_size;
	const wxString resolution_string = resolutions.Item(i);
	//wxMessageBox (resolution_string);

	const size_t len = resolution_string.Len();
	const int pos = resolution_string.Find(wxT("x"));

	const wxString width_string(resolution_string.Left(pos - 1));
	const wxString heigth_string(resolution_string.Right(len - pos - 1));

  const wxSize result_size(wxAtoi(width_string),wxAtoi(heigth_string));
	return result_size;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
int ICAOptionsDialog_Display::GetDefaultResolution() {
	const wxSize disp_size = wxGetDisplaySize();
	const wxArrayString resolutions = GetResolutionsList();
	if (resolutions.Count() <= 0)
		return -1;  // WTF? magic number
	int default_pos = 0;
	for (int i = 0; i < (int)resolutions.Count(); ++i) {
		const wxSize size = GetResolution(i);
		if (size.x == 800)
			return i;

		if (size.x >= disp_size.x || size.y >= disp_size.y)
			return default_pos > 0 ? default_pos - 1 : default_pos;
		else
			++default_pos;
	}
	return default_pos - 1;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void ICAOptionsDialog_Display::CheckICADisplay() {
	if (m_radiobutton_controlsize->GetValue() == true) {
		m_text_width->Enable(false);
		m_text_height->Enable(false);
		m_combobox_resolution->Enable(false);
	}
	else if (m_radiobutton_fullscreen->GetValue() == true) {
		m_text_width->Enable(false);
		m_text_height->Enable(false);
		m_combobox_resolution->Enable(false);
	}
	else if (m_radiobutton_customgeometry->GetValue() == true) {
		m_text_width->Enable(true);
		m_text_height->Enable(true);
		m_combobox_resolution->Enable(false);
	}
	else if (m_radiobutton_presetsize->GetValue() == true) {
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

void ICAOptionsDialog_Display::change_desired_color() {
	const bool selectedOne(1 == m_combo_desired_color->GetCurrentSelection());
	m_checkbox_approximate_colors->Enable(selectedOne);
}


///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
ICAOptionsDialog_Display::~ICAOptionsDialog_Display()
{
	delete m_static_width;
	delete m_text_width;
	delete m_static_height;
	delete m_text_height;
//   if(m_static_colordepth != NULL) {delete m_static_colordepth; m_static_colordepth = NULL;}
//   if(m_choise_colordepth != NULL) {delete m_choise_colordepth; m_choise_colordepth = NULL;}
//   if(m_checkbox_smartsizing != NULL) {delete m_checkbox_smartsizing; m_checkbox_smartsizing = NULL;}
//   if(m_checkbox_updatescreen != NULL) {delete m_checkbox_updatescreen; m_checkbox_updatescreen = NULL;}
	delete m_combobox_resolution;
	delete m_radiobutton_controlsize;
	delete m_radiobutton_fullscreen;
	delete m_radiobutton_customgeometry;
	delete m_radiobutton_presetsize;
	delete m_radiobox_1;
	delete m_checkbox_allow_degraded_colors;
	delete m_checkbox_enable_oss;
	delete checkboxEnableSessionSharingScreen;
	delete m_checkbox_approximate_colors;
	delete m_checkbox_resize_session;

	delete m_static_desired_color;
	delete m_combo_desired_color;

	delete m_static_use_sdlvb;
	delete m_combo_use_sdlvb;

	delete m_static_font_smoothing_type;
	delete m_combo_font_smoothing_type;

	delete m_static_screen_percent;
	delete m_combo_screen_percent;
}


///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
Options_HashMap ICAOptionsDialog_Display::Get_Options() {
	Options_HashMap local_options;

	local_options.clear();
	if(m_combo_desired_color != NULL)
		local_options[wxT("desired_color")] =
			wxString::Format(wxT("%i"), m_combo_desired_color->GetCurrentSelection());
   // if(m_checkbox_updatescreen != NULL)
   // {
   //    local_options[wxT("force_update_screen")] = wxString::Format(wxT("%i"),
   // 								   m_checkbox_updatescreen->GetValue());
   // }
	if(m_radiobutton_controlsize != NULL)
		local_options[wxT("control_size")] =
			wxString::Format(wxT("%i"), m_radiobutton_controlsize->GetValue());
	if(m_radiobutton_fullscreen != NULL)
		local_options[wxT("full_screen")] =
			wxString::Format(wxT("%i"), m_radiobutton_fullscreen->GetValue());

	if(m_radiobutton_customgeometry != NULL) {
		local_options[wxT("custom_geometry")] =
			wxString::Format(wxT("%i"), m_radiobutton_customgeometry->GetValue());
		if(m_text_width != NULL)
			local_options[wxT("width")] = m_text_width->GetValue();
		if(m_text_height != NULL)
			local_options[wxT("heigth")] = m_text_height->GetValue();
	}
	if(m_radiobutton_presetsize != NULL) {
		local_options[wxT("preset_screen_size")] =
			wxString::Format(wxT("%i"), m_radiobutton_presetsize->GetValue());

		if (m_combobox_resolution != NULL) {
			const wxSize resolution = GetResolution(m_combobox_resolution->GetCurrentSelection());
	 //wxArrayString resolution = GetResolutionsList();
			local_options[wxT("width")] = wxString::Format(wxT("%i"), resolution.x);
			local_options[wxT("heigth")] = wxString::Format(wxT("%i"), resolution.y);
		}
	}
	if (m_combobox_resolution != NULL)
		local_options[wxT("resolution")] =
			wxString::Format(wxT("%i"), m_combobox_resolution->GetCurrentSelection());

	if (m_checkbox_allow_degraded_colors != NULL)
		local_options[wxT("allow_degraded_colors")] =
			wxString::Format(wxT("%i"), m_checkbox_allow_degraded_colors->GetValue());

	if (m_checkbox_enable_oss != NULL)
		local_options[wxT("enable_oss")] =
			wxString::Format(wxT("%i"), m_checkbox_enable_oss->GetValue());
	if (checkboxEnableSessionSharingScreen != NULL)
		local_options[wxT("enable_session_sharing_screen")] =
			wxString::Format(wxT("%i"), checkboxEnableSessionSharingScreen->GetValue());
	if (m_checkbox_approximate_colors != NULL)
		local_options[wxT("approximate_colors")] =
			wxString::Format(wxT("%i"), m_checkbox_approximate_colors->GetValue());
	if (m_checkbox_resize_session != NULL)
		local_options[wxT("resize_session")] =
			wxString::Format(wxT("%i"), m_checkbox_resize_session->GetValue());
	if (m_combo_desired_color != NULL)
		local_options[wxT("desired_color")] =
			wxString::Format(wxT("%i"), m_combo_desired_color->GetCurrentSelection());
	if (m_combo_use_sdlvb != NULL)
		local_options[wxT("use_sdlvb")] =
			wxString::Format(wxT("%i"), m_combo_use_sdlvb->GetCurrentSelection());

	if (m_combo_font_smoothing_type != NULL)
		local_options[wxT("font_smoothing_type")] =
			wxString::Format(wxT("%i"), m_combo_font_smoothing_type->GetCurrentSelection());

	if (m_combo_screen_percent != NULL)
		local_options[wxT("screen_percent")] =
			wxString::Format(wxT("%i"), m_combo_screen_percent->GetValue());

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
bool ICAOptionsDialog_Display::SetOptions(const Options_HashMap *all_options) {
	bool state = false;
	Options_HashMap local_options(*all_options);

	if ((m_combo_desired_color != NULL)) {
		if ((wxAtoi(local_options[wxT("desired_color")]) > -1) &&
		    (wxAtoi(local_options[wxT("desired_color")]) < 4))
			m_combo_desired_color->SetSelection(wxAtoi(local_options[wxT("desired_color")]));
		else
			m_combo_desired_color->SetValue(_("15-bit color"));
	}
   // if((m_checkbox_updatescreen != NULL))
   // {
   //    m_checkbox_updatescreen->SetValue(wxAtoi(local_options[wxT("force_update_screen")]));
   // }
	if((m_radiobutton_controlsize != NULL))
		m_radiobutton_controlsize->SetValue(wxAtoi(local_options[wxT("control_size")]));
	if((m_radiobutton_fullscreen != NULL))
		m_radiobutton_fullscreen->SetValue(wxAtoi(local_options[wxT("full_screen")]));
	if((m_radiobutton_customgeometry != NULL))
		m_radiobutton_customgeometry->SetValue(wxAtoi(local_options[wxT("custom_geometry")]));
	if ((m_text_width != NULL))
		m_text_width->SetValue(local_options[wxT("width")]);
	if ((m_text_height != NULL))
		m_text_height->SetValue(local_options[wxT("heigth")]);
	if ((m_radiobutton_presetsize != NULL))
		m_radiobutton_presetsize->SetValue(wxAtoi(local_options[wxT("preset_screen_size")]));
	if (m_combobox_resolution != NULL)
		m_combobox_resolution->SetSelection(wxAtoi(local_options[wxT("resolution")]));
   // if ((m_checkbox_smartsizing != NULL))
   // {
   //    m_radiobutton_presetsize->SetValue(wxAtoi(local_options[wxT("smart_sizing")]));
   // }

	if (m_checkbox_allow_degraded_colors != NULL)
		m_checkbox_allow_degraded_colors->SetValue(wxAtoi(local_options[wxT("allow_degraded_colors")]));
	if (m_checkbox_enable_oss != NULL)
		m_checkbox_enable_oss->SetValue(wxAtoi(local_options[wxT("enable_oss")]));
	if (checkboxEnableSessionSharingScreen != NULL)
		checkboxEnableSessionSharingScreen->SetValue(wxAtoi(local_options[wxT("enable_session_sharing_screen")]));
	if (m_checkbox_approximate_colors != NULL)
		m_checkbox_approximate_colors->SetValue(wxAtoi(local_options[wxT("approximate_colors")]));
	if (m_checkbox_resize_session != NULL)
		m_checkbox_resize_session->SetValue(wxAtoi(local_options[wxT("resize_session")]));

	if (m_combo_desired_color != NULL)
		m_combo_desired_color->SetSelection(wxAtoi(local_options[wxT("desired_color")]));

	if (m_combo_use_sdlvb != NULL)
		m_combo_use_sdlvb->SetSelection(wxAtoi(local_options[wxT("use_sdlvb")]));
	if (m_combo_font_smoothing_type != NULL)
		m_combo_font_smoothing_type->SetSelection(wxAtoi(local_options[wxT("font_smoothing_type")]));

	if (m_combo_screen_percent != NULL)
		m_combo_screen_percent->SetValue(local_options[wxT("screen_percent")]);

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
: parent(parent_element)
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

	error = parent != NULL ? 0 : ICA_ERROR_PARENT_IS_NULL;
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
							   _("&Start following program"),
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
	 m_text_initial_program = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition,
						 wxDefaultSize);
	 m_flexgrid_sizer_1->Add(m_text_initial_program, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_long_command_line = new wxStaticText(this, wxID_ANY, _("Long Command Line"),
						       wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_1->Add(m_static_long_command_line, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	 m_text_long_command_line = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition,
						   wxDefaultSize, wxTE_MULTILINE);
	 m_flexgrid_sizer_1->Add(m_text_long_command_line, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_static_work_directory = new wxStaticText(this, wxID_ANY, _("Work Directory"),
						    wxDefaultPosition, wxDefaultSize);
	 m_flexgrid_sizer_1->Add(m_static_work_directory, 1,
				 wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_text_work_directory= new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
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
	 m_checkbox_mouse_sends_control_v = new wxCheckBox(this, wxID_ANY, _("Mouse Sends Control+V"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT("ID_CHECKBOX13"));
	 m_checkbox_mouse_sends_control_v->SetValue(false);
	 m_gridsizer_1->Add(m_checkbox_mouse_sends_control_v, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	 m_checkbox_persistent_cache_enabled = new wxCheckBox(this, wxID_ANY, _("Persistent Cache Enabled"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT("ID_CHECKBOX16"));
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
bool ICAOptionsDialog_Advanced::SetOptions(const Options_HashMap *all_options)
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
: parent(parent_element)
{
   error = parent != NULL ? 0 : ICA_ERROR_PARENT_IS_NULL;

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
	 wxString audio_bandwidth_limit[] ={_("high"),
					    _("medium"),
					    _("low")};
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

	 m_checkbox_drives = new wxCheckBox( this, wxID_ANY, _("&Redirect drives"), wxDefaultPosition,
					     wxDefaultSize );
	 m_checkbox_printers = new wxCheckBox( this, wxID_ANY, _("&Redirect printers"),
					       wxDefaultPosition, wxDefaultSize );
	 m_checkbox_comports = new wxCheckBox( this, wxID_ANY, _("&Redirect COM ports"),
					       wxDefaultPosition, wxDefaultSize );
	 m_checkbox_smartcards = new wxCheckBox( this, wxID_ANY, _("&Redirect smartcards"),
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

bool ICAOptionsDialog_Sound::SetOptions(const Options_HashMap *all_options)
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
   }

   return true;
}

ICAOptionsDialog_Security::ICAOptionsDialog_Security(ICAOptionsDialog *parent_element)
: parent(parent_element)
{
   m_boxsizer_1 = NULL;
   m_flexgrid_sizer_1 = NULL;
   m_static_encryption_level_session = NULL;
   m_combo_encryption_level_session = NULL;
   m_static_secure_channel_protocol = NULL;
   m_combo_secure_channel_protocol = NULL;

	error = parent != NULL ? 0 : ICA_ERROR_PARENT_IS_NULL;
}

ICAOptionsDialog_Security::~ICAOptionsDialog_Security()
{
	delete m_static_encryption_level_session;
	delete m_combo_encryption_level_session;
	delete m_static_secure_channel_protocol;
	delete m_combo_secure_channel_protocol;
}

int ICAOptionsDialog_Security::Build() {
	if (error != 0)
		return error;

	const bool state = Create(parent, wxID_ANY,
	                          wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	if (state == false) {
		error = ICA_ERROR_CANT_CREATE_PANEL;
		return error;
	}

	m_boxsizer_1 = new wxBoxSizer(wxVERTICAL);
	m_flexgrid_sizer_1 = new wxFlexGridSizer(0, 2, 0, 0);
	m_flexgrid_sizer_1->AddGrowableCol(1);
	m_static_encryption_level_session = new wxStaticText(this, wxID_ANY, _("EncryptionLevelSession"));
	m_flexgrid_sizer_1->Add(m_static_encryption_level_session, 0,
	                        wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	wxString encryption_level_session[] = { _("None"),
	                                        _("Basic"),
	                                        _("RC5 (128 bit 0 Login Only)"),
	                                        _("RC5 (40 bit)"),
	                                        _("RC5 (56 bit)"),
	                                        _("RC5 (128 bit)"), };
	m_combo_encryption_level_session = new wxComboBox(this,
	                                                  wxID_ANY,
	                                                  wxEmptyString,
	                                                  wxDefaultPosition,
	                                                  wxDefaultSize,
	                                                  6,
	                                                  encryption_level_session);
	m_combo_encryption_level_session->SetValue(encryption_level_session[1]);
	m_combo_encryption_level_session->SetSelection(1);

	m_flexgrid_sizer_1->Add(m_combo_encryption_level_session, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_static_secure_channel_protocol = new wxStaticText(this, wxID_ANY, _("SecureChannelProtocol"));
	 m_flexgrid_sizer_1->Add(m_static_secure_channel_protocol, 0,
				 wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 wxString secure_channel_protocol[] = {wxT("ssl"),
					       wxT("tls"),
					       _("detect"), };
	m_combo_secure_channel_protocol =
	   new wxComboBox(this,
			  wxID_ANY,
			  wxEmptyString,
			  wxDefaultPosition,
			  wxDefaultSize,
			  3,
			  secure_channel_protocol);
	 m_combo_secure_channel_protocol->SetValue(secure_channel_protocol[2]);
	 m_combo_secure_channel_protocol->SetSelection(2);

	 m_flexgrid_sizer_1->Add(m_combo_secure_channel_protocol, 1,
				 wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(m_flexgrid_sizer_1, 0,
			   wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 m_boxsizer_1->Add(-1,-1,1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	 this->SetSizer(m_boxsizer_1);

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

bool ICAOptionsDialog_Security::SetOptions(const Options_HashMap *all_options) {
	if (all_options == NULL)
		return true; // TODO: false?..

	Options_HashMap local_options(*all_options);
	if (m_combo_encryption_level_session != NULL)
		m_combo_encryption_level_session->SetSelection(
			wxAtoi(local_options[wxT("encryption_level_session")]));
	if (m_combo_secure_channel_protocol != NULL)
		m_combo_secure_channel_protocol->SetSelection(
			wxAtoi(local_options[wxT("secure_channel_protocol")]));

	return true;
}

wxString ICAOptionsDialog_Security::Get_Options_String()
{
	return wxString();
}

