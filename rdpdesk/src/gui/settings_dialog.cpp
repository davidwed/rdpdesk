#include "settings_dialog.hpp"
#include "main_window.hpp"

SettingsDialog::SettingsDialog(Main_Frame * main, wxWindow* parent, int dialogType):
wxDialog(parent, wxID_ANY, _T("Program settings"),
                 wxDefaultPosition, wxSize(500 + 2*SHIFT_W, 350),dialogType)
{
	main_frame = main;

	m_static_rdpbase = NULL;
	m_text_rdpbase = NULL;
	m_button_rdpbase = NULL;

	m_static_rdpkey = NULL;
	m_text_rdpkey = NULL;
	m_button_rdpkey = NULL;

	m_checkbox_icontray = NULL;

	m_static_rdesktop = NULL;
	m_text_rdesktop = NULL;
	m_button_rdesktop = NULL;

	m_static_fcount = NULL;
	m_text_fcount = NULL;

	m_static_fmax = NULL;
	m_text_fmax = NULL;

	m_static_keymappath = NULL;
	m_text_keymappath = NULL;
	m_button_keymappath = NULL;
	
	m_checkbox_showform = NULL;

	sizer_usecrypt = NULL;
	m_checkbox_usecrypt = NULL;
	m_combobox_usecrypt = NULL; 

	m_checkbox_focuspage = NULL;  

	m_checkbox_autosave = NULL;

	m_line_separator = NULL;
	m_line_button = NULL;

	m_static_lang = NULL;
	m_combobox_lang = NULL;
	

	m_button_ok = NULL;
	m_button_cancel = NULL;
	m_button_apply = NULL;

	sizer_top = new wxBoxSizer(wxVERTICAL);

	sizer_path = new wxFlexGridSizer(3,10,10);

	sizer_main = new wxFlexGridSizer(2,10,10);

	m_static_rdpbase = new wxStaticText(this, wxID_ANY, wxT("Path for rdp base"),wxDefaultPosition, wxDefaultSize);
	m_text_rdpbase = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,CONTROL_H), wxTE_PROCESS_ENTER);
	m_button_rdpbase = new wxButton(this,ID_BUTTON_RDPBASE, wxT(""), wxDefaultPosition, wxSize(CONTROL_H,CONTROL_H) );

	sizer_path->Add(m_static_rdpbase,0,wxALL, 5);
	sizer_path->Add(m_text_rdpbase,0,wxALL | wxALIGN_RIGHT  , 2);
	sizer_path->Add(m_button_rdpbase,0,wxALL | wxALIGN_RIGHT, 2);

	m_static_rdpkey = new wxStaticText(this, wxID_ANY, wxT("Path for rdp key"),wxDefaultPosition, wxDefaultSize);
	m_text_rdpkey = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,CONTROL_H), wxTE_PROCESS_ENTER);
	m_button_rdpkey = new wxButton(this,ID_BUTTON_RDPKEY, wxT(""), wxDefaultPosition, wxSize(CONTROL_H,CONTROL_H) );

	sizer_path->Add(m_static_rdpkey,0,wxALL, 5);
	sizer_path->Add(m_text_rdpkey,0,wxALL | wxALIGN_RIGHT , 2);
	sizer_path->Add(m_button_rdpkey,0,wxALL  | wxALIGN_RIGHT, 2);

	m_checkbox_icontray = new wxCheckBox( this, ID_CHECKBOX_ICON, wxT("&Display icon in system tray"), wxDefaultPosition, wxDefaultSize );
	m_checkbox_showform = new wxCheckBox( this, -1, wxT("&Show frame"), wxDefaultPosition, wxDefaultSize);

	sizer_usecrypt = new wxBoxSizer(wxHORIZONTAL);
	m_checkbox_usecrypt = new wxCheckBox( this, ID_CHECKBOX_CRYPT , wxT("&Use crypt"), wxDefaultPosition, wxDefaultSize );
	
	wxString  crypt_methods[] = {wxT("Simple"), wxT("AES")};
	m_combobox_usecrypt = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition, wxDefaultSize , 2, crypt_methods, wxCB_READONLY | wxCB_DROPDOWN);	
	m_combobox_usecrypt->SetSelection(0);
	m_combobox_usecrypt->SetValue(crypt_methods[0]);
	sizer_usecrypt->Add(m_checkbox_usecrypt, 0, wxALIGN_LEFT | wxALIGN_CENTER | wxTOP | wxBOTTOM ,5);
	sizer_usecrypt->Add(m_combobox_usecrypt, 0, wxALIGN_LEFT | wxALIGN_CENTER | wxALL /*| wxEXPAND*/ ,5);
	

	m_checkbox_focuspage = new wxCheckBox( this, -1, wxT("&Set focus on new page"), wxDefaultPosition, wxDefaultSize);
	m_checkbox_autosave = new wxCheckBox( this, -1, wxT("&Auto saving"), wxDefaultPosition, wxDefaultSize );

	sizer_main->Add(m_checkbox_icontray, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_main->Add(m_checkbox_showform, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_main->Add(sizer_usecrypt, 0, wxALL | /*wxALIGN_LEFT | wxALIGN_CENTER*/ wxALIGN_CENTRE_VERTICAL ,5);
	sizer_main->Add(m_checkbox_focuspage, 0, wxALL | /* wxALIGN_LEFT | wxALIGN_CENTER */wxALIGN_CENTRE_VERTICAL  ,5);
	sizer_main->Add(m_checkbox_autosave, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_main->AddSpacer(1);
	
	sizer_main->Layout();
	m_static_rdesktop = new wxStaticText(this, wxID_ANY, wxT("Path for rdesktop"),wxDefaultPosition, wxDefaultSize);
	m_text_rdesktop = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,CONTROL_H), wxTE_PROCESS_ENTER);
	m_button_rdesktop = new wxButton(this,ID_BUTTON_RDESKTOP, wxT(""), wxDefaultPosition, wxSize(CONTROL_H,CONTROL_H) );

	sizer_path->Add(m_static_rdesktop,0,wxALL, 5);
	sizer_path->Add(m_text_rdesktop,0,wxALL | wxALIGN_RIGHT , 2);
	sizer_path->Add(m_button_rdesktop,0,wxALL| wxALIGN_RIGHT, 2);
	
	m_static_keymappath = new wxStaticText(this, wxID_ANY, wxT("Path for key map files"),wxDefaultPosition, wxDefaultSize);
	m_text_keymappath = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(300,CONTROL_H), wxTE_PROCESS_ENTER);
	m_button_keymappath = new wxButton(this,ID_BUTTON_KEYMAP, wxT(""), wxDefaultPosition, wxSize(CONTROL_H,CONTROL_H) );
	
	sizer_path->Add(m_static_keymappath,0,wxALL, 5);
	sizer_path->Add(m_text_keymappath,0,wxALL | wxALIGN_RIGHT , 2);
	sizer_path->Add(m_button_keymappath,0,wxALL | wxALIGN_RIGHT, 2);


	sizer_fmax = new wxBoxSizer(wxHORIZONTAL);
	sizer_fcount = new wxBoxSizer(wxHORIZONTAL);

	m_static_fcount = new wxStaticText(this, wxID_ANY, wxT("Connection count"),wxDefaultPosition, wxDefaultSize);
	m_text_fcount = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(40,CONTROL_H), wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC));
	sizer_fcount->Add(m_static_fcount,0,wxALL, 5);
	sizer_fcount->Add(m_text_fcount,0,wxALL | wxALIGN_CENTER , 2);

	m_static_fmax = new wxStaticText(this, wxID_ANY, wxT("Maximum favorites"),wxDefaultPosition, wxDefaultSize);
	m_text_fmax = new wxTextCtrl( this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(40,CONTROL_H), wxTE_PROCESS_ENTER , wxTextValidator(wxFILTER_NUMERIC) );
	sizer_fmax->Add(m_static_fmax,0,wxALL, 5);
	sizer_fmax->Add(m_text_fmax,0,wxALL | wxALIGN_CENTER , 2);

	sizer_lang = new wxBoxSizer(wxHORIZONTAL);
	m_static_lang = new wxStaticText(this, wxID_ANY, wxT("Select language"),wxDefaultPosition, wxDefaultSize);
	LocaleHelper lh;
	wxArrayString list = lh.GetNameList(lh.GetAvailableList());  
	m_combobox_lang = new wxComboBox(this,wxID_ANY,wxT(""),wxDefaultPosition, wxDefaultSize ,list ,wxCB_READONLY);
	sizer_lang->Add(m_static_lang,0,wxALL, 5);
	sizer_lang->Add(m_combobox_lang,0,wxALL | wxALIGN_CENTER , 2);

	sizer_main->Add(sizer_fcount, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_main->Add(sizer_fmax, 0, wxALL | wxALIGN_LEFT ,5);
	sizer_main->Add(sizer_lang, 0, wxALL | wxALIGN_LEFT ,5);

	m_line_separator = new wxStaticLine(this,wxID_ANY,wxDefaultPosition, wxSize(-1,2) ,wxLI_HORIZONTAL);

	m_line_button = new wxStaticLine(this,wxID_ANY,wxDefaultPosition, wxSize(-1,2) ,wxLI_HORIZONTAL);

	sizer_btn = new wxBoxSizer(wxHORIZONTAL);
	m_button_ok = new wxButton(this,ID_BUTTON_SD_OK,wxT("OK"),wxDefaultPosition,wxSize(100 + 0.5*SHIFT_W,25));
	m_button_cancel = new wxButton(this,ID_BUTTON_SD_CANCEL,wxT("Cancel"),wxDefaultPosition,wxSize(100 + 0.5*SHIFT_W,25));
	m_button_apply = new wxButton(this,ID_BUTTON_SD_APPLY,wxT("Apply"),wxDefaultPosition,wxSize(100 + 0.5*SHIFT_W,25));

	
	sizer_btn->Add(m_button_cancel,0,wxALL,5);
	sizer_btn->Add(m_button_apply,0,wxALL,5);
	sizer_btn->Add(m_button_ok,0,wxALL,5);

	sizer_top->Add(sizer_path,0,wxALL | wxALIGN_CENTER | wxEXPAND ,5);
	sizer_top->Add(m_line_separator,0,wxALL | wxALIGN_CENTER | wxEXPAND ,5);
	sizer_top->Add(sizer_main,0,wxALL | wxALIGN_LEFT ,5);
	sizer_top->Add(m_line_button,0,wxALL | wxALIGN_CENTER | wxEXPAND ,5);
	sizer_top->Add(sizer_btn,0,wxALL | wxALIGN_CENTER ,5);


	SetSizer(sizer_top);
	sizer_top->SetSizeHints(this);
#ifdef __WXMSW__
	m_static_rdesktop->Show(false);
	m_text_rdesktop->Show(false);
	m_button_rdesktop->Show(false);

	m_static_keymappath->Show(false);
	m_text_keymappath->Show(false);
	m_button_keymappath->Show(false);
#endif
	sizer_top->Fit(this); 

#ifndef TS_NORMAL_VERSION
	m_checkbox_showform->Show(false);
#endif

	
	init_programsettings(&progsettings);
	Benc bc;
	bc.LoadProgramSettings(&progsettings);
	display_settings();
}


SettingsDialog::~SettingsDialog()
{
	if (m_static_rdpbase) {delete m_static_rdpbase; m_static_rdpbase = NULL;}
	if (m_text_rdpbase) {delete  m_text_rdpbase; m_text_rdpbase = NULL;}
	if (m_button_rdpbase) {delete m_button_rdpbase; m_button_rdpbase = NULL;}

	if (m_static_rdpkey) {delete m_static_rdpkey ; m_static_rdpkey = NULL;}
	if (m_text_rdpkey) {delete  m_text_rdpkey; m_text_rdpkey = NULL;}
	if (m_button_rdpkey) {delete  m_button_rdpkey; m_button_rdpkey = NULL;}

	if (m_static_rdesktop) {delete  m_static_rdesktop; m_static_rdesktop = NULL;}
	if (m_text_rdesktop) {delete  m_text_rdesktop; m_text_rdesktop = NULL;}
	if (m_button_rdesktop) {delete m_button_rdesktop ; m_button_rdesktop = NULL;}

	if (m_static_keymappath) {delete  m_static_keymappath; m_static_keymappath = NULL;}
	if (m_text_keymappath) {delete  m_text_keymappath; m_text_keymappath = NULL;}
	if (m_button_keymappath) {delete m_button_keymappath ; m_button_keymappath = NULL;}

	if(m_button_ok) {delete m_button_ok; m_button_ok = NULL;}
	if(m_button_cancel) {delete m_button_cancel; m_button_cancel = NULL;}
	if(m_button_apply) {delete m_button_apply; m_button_apply = NULL;}

	if (m_checkbox_icontray) {delete m_checkbox_icontray; m_checkbox_icontray = NULL;}
	if (m_checkbox_showform) {delete m_checkbox_showform; m_checkbox_showform = NULL;}
	wxDELETE(m_combobox_usecrypt);
	if (m_checkbox_usecrypt) {delete m_checkbox_usecrypt; m_checkbox_usecrypt = NULL;}
	if (m_checkbox_focuspage) {delete m_checkbox_focuspage; m_checkbox_focuspage = NULL;}
	if (m_checkbox_autosave) {delete m_checkbox_autosave; m_checkbox_autosave = NULL;}

	if (m_static_fcount) {delete m_static_fcount; m_static_fcount = NULL;}
	if (m_text_fcount) {delete m_text_fcount; m_text_fcount = NULL;}

	if (m_static_fmax) {delete m_static_fmax; m_static_fmax = NULL;}
	if (m_text_fmax) {delete m_text_fmax; m_text_fmax = NULL;}

	if (m_line_separator) {delete m_line_separator; m_line_separator = NULL;}
	if (m_line_button) {delete m_line_button; m_line_button = NULL;}

	if (m_static_lang) {delete m_static_lang; m_static_lang = NULL;}
	if (m_combobox_lang) {delete m_combobox_lang; m_combobox_lang = NULL;}

}

void SettingsDialog::init_programsettings(programsettings * ps)
{
	ps->bIcon = TRUE;
	ps->bShowFrame = FALSE;
	ps->bUseCrypt = TRUE;
	ps->bFocusPage = TRUE;
	ps->iTypeCrypt = 1; 
	ps->bAutosave = TRUE;
	ps->favorites_count = STANDARD_FAVORITES_COUNT; 
	ps->favorites_max = STANDARD_FAVORITES_MAX;
	ps->rdpbasepath = settings_path;
	ps->rdpkeypath = settings_path;
	ps->rdesktoppath = STANDARD_RDESKTOP_PATH;
	ps->rdesktop_key_path = STANDARD_RDESKTOP_KEY_PATH;
	ps->lang = wxT("Default");
}

void SettingsDialog::button_cancel_func(wxCommandEvent &event)
{
	this->EndModal(0); 
}

void SettingsDialog::button_apply_func(wxCommandEvent &event) 
{
	progsettings = fill_settings();
	save_to_file();
	main_settings = progsettings;
	apply_settings();
}

void SettingsDialog::button_ok_func(wxCommandEvent &event) 
{
	progsettings = fill_settings();
	save_to_file();
	main_settings = progsettings;
	apply_settings();
	this->EndModal(1);
}

void SettingsDialog::dialog_hotkeys(wxKeyEvent &event)
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
	default:
		break;

	}

	event.Skip();
}

void SettingsDialog::apply_settings()
{
	main_frame->bUseHideFrame = progsettings.bIcon;
	if (progsettings.bIcon && main_frame->m_hideframe )
	{
		main_frame->bShowFrame = progsettings.bShowFrame;
	}
	else
	{
		main_frame->bShowFrame = TRUE;
	}
	
	main_frame->bFocusNewPage = progsettings.bFocusPage;
	main_frame->bUseHideFrame = progsettings.bIcon;
	main_frame->bUseCrypt = progsettings.bUseCrypt; 

	main_frame->m_panel_tree->rdptree->fcount = progsettings.favorites_count;
	main_frame->m_panel_tree->rdptree->fmax = progsettings.favorites_max;
	main_frame->m_panel_tree->favorites->fcount = progsettings.favorites_count;
	main_frame->m_panel_tree->favorites->fmax = progsettings.favorites_max;

}


void SettingsDialog::display_settings() 
{
	m_text_rdpbase->SetValue(progsettings.rdpbasepath); 
	m_text_rdpkey->SetValue(progsettings.rdpkeypath);
	m_text_rdesktop->SetValue(progsettings.rdesktoppath);
	m_text_keymappath->SetValue(progsettings.rdesktop_key_path);
	
	wxString temp;
	temp = wxString::Format(wxT("%d"),progsettings.favorites_count); 
	m_text_fcount->SetValue(temp);

	temp = wxString::Format(wxT("%d"),progsettings.favorites_max);
	m_text_fmax->SetValue(temp);

	progsettings.bIcon ? m_checkbox_icontray->SetValue(true) : m_checkbox_icontray->SetValue(false);
	if (progsettings.bIcon)
	{
		progsettings.bShowFrame ? m_checkbox_showform->SetValue(true) : m_checkbox_showform->SetValue(false);
	}
	else
	{
		m_checkbox_showform->SetValue(true);
	}
	
	progsettings.bUseCrypt ? m_checkbox_usecrypt->SetValue(true) : m_checkbox_usecrypt->SetValue(false);
	progsettings.bFocusPage ? m_checkbox_focuspage->SetValue(true) : m_checkbox_focuspage->SetValue(false);
	progsettings.bAutosave ? m_checkbox_autosave->SetValue(true) : m_checkbox_autosave->SetValue(false);

	if (m_checkbox_icontray->IsChecked())
	{
		m_checkbox_showform->Enable(true);
	}
	else
	{
		m_checkbox_showform->Enable(false);
	}


	wxString  crypt_methods[] = {wxT("Simple"), wxT("AES")};
	m_combobox_usecrypt->SetSelection(progsettings.iTypeCrypt);
	m_combobox_usecrypt->SetValue(crypt_methods[progsettings.iTypeCrypt]);
	if (m_checkbox_usecrypt->IsChecked())
	{
		m_combobox_usecrypt->Enable(true);
	}
	else
	{
		m_combobox_usecrypt->Enable(false);
	}
	
	m_combobox_lang->SetValue(progsettings.lang);
}

void SettingsDialog::checkbox_icon_func(wxCommandEvent &event)
{
	if (m_checkbox_icontray->IsChecked())
	{
		m_checkbox_showform->Enable(true);
	}
	else
	{
		m_checkbox_showform->SetValue(false);
		m_checkbox_showform->Enable(false);
	}
}

void SettingsDialog::checkbox_crypt_func(wxCommandEvent &event)
{
	if (m_checkbox_usecrypt->IsChecked())
	{
		m_combobox_usecrypt->Enable(true);
	}
	else
	{
		m_combobox_usecrypt->Enable(false);
	}
	event.StopPropagation();
}

programsettings SettingsDialog::fill_settings() 
{
	progsettings.rdpbasepath = m_text_rdpbase->GetValue();
	progsettings.rdpkeypath = m_text_rdpkey->GetValue();
	progsettings.rdesktoppath = m_text_rdesktop->GetValue();
	progsettings.rdesktop_key_path = m_text_keymappath->GetValue();
	progsettings.lang = m_combobox_lang->GetValue();

	progsettings.iTypeCrypt = m_combobox_usecrypt->GetSelection();

	progsettings.favorites_count = wxAtoi(m_text_fcount->GetValue());
	progsettings.favorites_max = wxAtoi(m_text_fmax->GetValue());

	if (m_checkbox_icontray != NULL)
	{
		if (m_checkbox_icontray->IsChecked())
			progsettings.bIcon = TRUE;
		else
			progsettings.bIcon = FALSE;
	}

	if (m_checkbox_showform != NULL)
	{
		if (m_checkbox_showform->IsChecked())
			progsettings.bShowFrame = TRUE;
		else
			progsettings.bShowFrame = FALSE;
	}

	if (m_checkbox_usecrypt != NULL)
	{
		if (m_checkbox_usecrypt->IsChecked())
			progsettings.bUseCrypt = TRUE;
		else
			progsettings.bUseCrypt = FALSE;
	}

	if (m_checkbox_focuspage != NULL)
	{
		if (m_checkbox_focuspage->IsChecked())
			progsettings.bFocusPage = TRUE;
		else
			progsettings.bFocusPage = FALSE;
	}

	if (m_checkbox_autosave != NULL)
	{
		if (m_checkbox_autosave->IsChecked())
			progsettings.bAutosave = TRUE;
		else
			progsettings.bAutosave = FALSE;
	}

	return progsettings;
}

void SettingsDialog::save_to_file() 
{
	Benc bc;
	bc.SaveProgramSettings(&progsettings); 
	return;
}


void SettingsDialog::button_rdesktop_func(wxCommandEvent &event) 
{
	wxDirDialog * dd = new wxDirDialog(this,wxT("Choose rdesktop path"));
	int iRes = dd->ShowModal(); 
	if (iRes == wxID_CANCEL) {delete dd; return;}
	progsettings.rdesktoppath = dd->GetPath();
	m_text_rdesktop->SetValue(progsettings.rdesktoppath);
	delete dd;

}

void SettingsDialog::button_rdpbase_func(wxCommandEvent &event) 
{
	wxDirDialog * dd = new wxDirDialog(this,wxT("Choose rdpbase path"));
	int iRes = dd->ShowModal(); 
	if (iRes == wxID_CANCEL) {delete dd; return;}
	progsettings.rdpbasepath  = dd->GetPath();
	m_text_rdpbase->SetValue(progsettings.rdpbasepath);
	delete dd;
}

void SettingsDialog::button_rdpkey_func(wxCommandEvent &event) 
{
	wxDirDialog * dd = new wxDirDialog(this,wxT("Choose rdpkey path"));
	int iRes = dd->ShowModal(); 
	if (iRes == wxID_CANCEL) {delete dd; return;}
	progsettings.rdpkeypath = dd->GetPath();
	m_text_rdpkey->SetValue(progsettings.rdpkeypath);
	delete dd;
}

void SettingsDialog::button_keymap_func(wxCommandEvent &event) 
{
	wxDirDialog * dd = new wxDirDialog(this,wxT("Choose rdesktop keyboard map path"));
	int iRes = dd->ShowModal(); 
	if (iRes == wxID_CANCEL) {delete dd; return;}
	progsettings.rdesktop_key_path = dd->GetPath();
	m_text_keymappath->SetValue(progsettings.rdesktop_key_path);
	delete dd;
}

