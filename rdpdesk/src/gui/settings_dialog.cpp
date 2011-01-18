///////////////////////////////////////////////////////////////////////////////
// File name:   settings_dialog.cpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include "settings_dialog.hpp"
#include "main_window.hpp"

bool SettingsDialog::dialog_count = false;

///////////////////////////////////////////////////////////////////////////////
//! \brief
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
SettingsDialog::SettingsDialog(MainFrame * main, wxWindow* parent, int dialogType)
: main_frame(main)
{
    m_global_sizer = NULL;
    m_notebook = NULL;
    m_panel_1 = NULL;

    m_panel_2 = NULL;
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

    m_static_rdpbase = NULL;
    m_text_rdpbase = NULL;
    m_button_rdpbase = NULL;

    m_static_rdpkey = NULL;
    m_text_rdpkey = NULL;
    m_button_rdpkey = NULL;

    m_static_rdesktop = NULL;
    m_text_rdesktop = NULL;
    m_button_rdesktop = NULL;

    m_static_keymappath = NULL;
    m_text_keymappath = NULL;
    m_button_keymappath = NULL;

    m_checkbox_icontray = NULL;
    m_checkbox_grabkbd = NULL;

    m_static_fcount = NULL;
    m_text_fcount = NULL;

    m_static_fmax = NULL;
    m_text_fmax = NULL;

    m_checkbox_showform = NULL;

    //sizer_usecrypt = NULL;
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

   Create(parent,
         ID_SETTINGS,
         _("Program settings"),
         wxDefaultPosition,
         wxDefaultSize,
         wxDEFAULT_FRAME_STYLE | wxCLOSE_BOX | wxFRAME_SHAPED |
         wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxSTATIC_BORDER);


    m_global_sizer = new wxBoxSizer(wxVERTICAL);
    m_boxsizer_2 = new wxBoxSizer(wxVERTICAL);
    m_notebook =  new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(507,378), 0);
    m_panel_1 = new wxPanel(m_notebook, wxID_ANY, wxPoint(34,45), wxDefaultSize, wxTAB_TRAVERSAL);
    m_notebook->AddPage(m_panel_1, _("Paths"), false);
    m_global_sizer->Add(m_notebook, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    m_flexgrid_sizer_1 = new wxFlexGridSizer(4, 3, 1, 1);
    m_flexgrid_sizer_1->AddGrowableCol(1);
    m_flexgrid_sizer_1->AddGrowableRow(1);

    m_static_rdpbase = new wxStaticText(m_panel_1, wxID_ANY, _("Path for rdp base"),
					wxDefaultPosition, wxDefaultSize, 0);
    m_text_rdpbase = new wxTextCtrl(m_panel_1, wxID_ANY, wxT(""), wxDefaultPosition,
				    wxDefaultSize, wxTE_PROCESS_ENTER);
    m_button_rdpbase = new wxButton(m_panel_1,ID_BUTTON_RDPBASE, _("..."), wxDefaultPosition,
				    wxDefaultSize, 0, wxDefaultValidator );

    m_static_rdpkey = new wxStaticText(m_panel_1, wxID_ANY, _("Path for rdp key"),
				       wxDefaultPosition, wxDefaultSize);
    m_text_rdpkey = new wxTextCtrl(m_panel_1, wxID_ANY, wxT(""), wxDefaultPosition,
				   wxDefaultSize, wxTE_PROCESS_ENTER);
    m_button_rdpkey = new wxButton(m_panel_1,ID_BUTTON_RDPKEY, _("..."), wxDefaultPosition,
				   wxDefaultSize, 0 );

    m_static_rdesktop = new wxStaticText(m_panel_1, wxID_ANY, _("Path for rdesktop"),wxDefaultPosition,
					 wxDefaultSize);
    m_text_rdesktop = new wxTextCtrl( m_panel_1, wxID_ANY, wxT(""), wxDefaultPosition,
				      wxDefaultSize, wxTE_PROCESS_ENTER);
    m_button_rdesktop = new wxButton(m_panel_1,ID_BUTTON_RDESKTOP, _("..."), wxDefaultPosition,
				     wxDefaultSize );

    m_static_keymappath = new wxStaticText(m_panel_1, wxID_ANY, _("Path for key map files"),
					   wxDefaultPosition, wxDefaultSize);
    m_text_keymappath = new wxTextCtrl( m_panel_1, wxID_ANY, wxT(""), wxDefaultPosition,
					wxDefaultSize, wxTE_PROCESS_ENTER);
    m_button_keymappath = new wxButton(m_panel_1,ID_BUTTON_KEYMAP, _("..."), wxDefaultPosition,
				       wxDefaultSize );


    m_flexgrid_sizer_1->Add(m_static_rdpbase, 0,
			    wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
#ifndef __WXMSW__
    m_flexgrid_sizer_1->Add(m_text_rdpbase, 1,
			    wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
#else
	m_flexgrid_sizer_1->Add(m_text_rdpbase, 1,
			    wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);
#endif
    m_flexgrid_sizer_1->Add(m_button_rdpbase, 0,
			    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    m_flexgrid_sizer_1->Add(m_static_rdpkey, 0,
			    wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

#ifndef __WXMSW__
    m_flexgrid_sizer_1->Add(m_text_rdpkey, 1,
			    wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
#else
	m_flexgrid_sizer_1->Add(m_text_rdpkey, 1,
			    wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);
#endif
    m_flexgrid_sizer_1->Add(m_button_rdpkey, 0,
			    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    m_flexgrid_sizer_1->Add(m_static_rdesktop, 0,
			    wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    m_flexgrid_sizer_1->Add(m_text_rdesktop, 1,
			    wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_flexgrid_sizer_1->Add(m_button_rdesktop, 0,
			    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    m_flexgrid_sizer_1->Add(m_static_keymappath, 0,
			    wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    m_flexgrid_sizer_1->Add(m_text_keymappath, 1,
			    wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_flexgrid_sizer_1->Add(m_button_keymappath, 0,
			    wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);


    m_boxsizer_2->Add(m_flexgrid_sizer_1, 0,
		      wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_2->Add(-1,-1,1,
		      wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_panel_1->SetSizer(m_boxsizer_2);
    m_boxsizer_2->Fit(m_panel_1);
    m_boxsizer_2->SetSizeHints(m_panel_1);




    m_panel_2 = new wxPanel(m_notebook, wxID_ANY, wxPoint(34,45), wxDefaultSize, wxTAB_TRAVERSAL);
    m_notebook->AddPage(m_panel_2, _("General"), false);

    m_boxsizer_3 = new wxBoxSizer(wxVERTICAL);

    m_checkbox_icontray = new wxCheckBox(m_panel_2, ID_CHECKBOX_ICON, _("&Display icon in system tray"),
					 wxDefaultPosition, wxDefaultSize );
    m_checkbox_showform = new wxCheckBox(m_panel_2, -1, _("&Show frame"), wxDefaultPosition, wxDefaultSize);

    m_checkbox_focuspage = new wxCheckBox(m_panel_2, -1, _("&Set focus on new page"),
					  wxDefaultPosition, wxDefaultSize);
    m_checkbox_autosave = new wxCheckBox(m_panel_2, -1, _("&Auto saving"),
					 wxDefaultPosition, wxDefaultSize );

    m_checkbox_usecrypt = new wxCheckBox(m_panel_2, ID_CHECKBOX_CRYPT , _("&Use crypt"),
					 wxDefaultPosition, wxDefaultSize );
    wxString  crypt_methods[] = {_("Simple"), _("AES")};
    m_combobox_usecrypt = new wxComboBox(m_panel_2,wxID_ANY,wxT(""),wxDefaultPosition, wxDefaultSize ,
					 2, crypt_methods, wxCB_READONLY | wxCB_DROPDOWN);
    m_combobox_usecrypt->SetSelection(0);
    m_combobox_usecrypt->SetValue(crypt_methods[0]);

    m_checkbox_grabkbd = new wxCheckBox(m_panel_2, -1, _("&Grab keyboard keys"),
					wxDefaultPosition, wxDefaultSize );

    m_static_fcount = new wxStaticText(m_panel_2, wxID_ANY, _("Connection count"),
				       wxDefaultPosition, wxDefaultSize);
    m_text_fcount = new wxTextCtrl(m_panel_2, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(40,CONTROL_H),
				   wxTE_PROCESS_ENTER, wxTextValidator(wxFILTER_NUMERIC));

    m_static_fmax = new wxStaticText(m_panel_2, wxID_ANY, _("Maximum favorites"),wxDefaultPosition,
				     wxDefaultSize);
    m_text_fmax = new wxTextCtrl(m_panel_2, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(40,CONTROL_H), wxTE_PROCESS_ENTER , wxTextValidator(wxFILTER_NUMERIC) );

    m_static_lang = new wxStaticText(m_panel_2, wxID_ANY, _("Select language"),wxDefaultPosition,
				     wxDefaultSize);
    LocaleHelper lh;
    wxArrayString list = lh.GetNameList(lh.GetAvailableList());
    m_combobox_lang = new wxComboBox(m_panel_2,wxID_ANY,wxT(""),wxDefaultPosition, wxDefaultSize , list,
				     wxCB_READONLY);


    m_boxsizer_4 = new wxBoxSizer(wxHORIZONTAL);
    m_boxsizer_5 = new wxBoxSizer(wxHORIZONTAL);
    m_boxsizer_6 = new wxBoxSizer(wxHORIZONTAL);
    m_boxsizer_7 = new wxBoxSizer(wxHORIZONTAL);
    m_boxsizer_8 = new wxBoxSizer(wxHORIZONTAL);
    m_boxsizer_9 = new wxBoxSizer(wxHORIZONTAL);
    m_boxsizer_10 = new wxBoxSizer(wxHORIZONTAL);
    m_boxsizer_11= new wxBoxSizer(wxHORIZONTAL);

    m_boxsizer_4->Add (m_checkbox_icontray, 1,
		       wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_4->Add (m_checkbox_showform, 1,
		       wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    m_boxsizer_5->Add (m_checkbox_focuspage, 1,
		       wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_5->Add (m_checkbox_autosave, 1,
		       wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_6->Add (m_checkbox_usecrypt, 1,
		       wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_6->Add (m_combobox_usecrypt, 1,
		       wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_7->Add (m_boxsizer_6, 1,
		       wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_7->Add (m_checkbox_grabkbd, 1,
		       wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_8->Add (m_static_fcount, 1,
		       wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_8->Add (m_text_fcount, 0,
		       wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_9->Add (m_static_fmax, 1,
		       wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_9->Add (m_text_fmax, 0,
		       wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_10->Add (m_boxsizer_8, 1,
			wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_10->Add (m_boxsizer_9, 1,
			wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_11->Add (m_static_lang, 1,
			wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_11->Add (m_combobox_lang, 1,
			wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    m_boxsizer_3->Add(m_boxsizer_4, 0,
		      wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_3->Add(m_boxsizer_5, 0,
		      wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_3->Add(m_boxsizer_7, 0,
		      wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_3->Add(m_boxsizer_10, 0,
		      wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_boxsizer_3->Add(m_boxsizer_11, 0,
		      wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    m_boxsizer_3->Add(-1,-1,1,
		      wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    m_panel_2->SetSizer(m_boxsizer_3);
    m_boxsizer_3->Fit(m_panel_2);
    m_boxsizer_3->SetSizeHints(m_panel_2);

   m_button_ok = new wxButton(this,ID_BUTTON_SD_OK,_("OK"),wxDefaultPosition,wxSize(100 + 0.5*SHIFT_W,25));
   m_button_cancel = new wxButton(this,ID_BUTTON_SD_CANCEL,_("Cancel"),wxDefaultPosition,wxSize(100 + 0.5*SHIFT_W,25));
   m_button_apply = new wxButton(this,ID_BUTTON_SD_APPLY,_("Apply"),wxDefaultPosition,wxSize(100 + 0.5*SHIFT_W,25));

#ifdef __WXMSW__
    m_static_rdesktop->Show(false);
    m_text_rdesktop->Show(false);
    m_button_rdesktop->Show(false);
    m_static_keymappath->Show(false);
    m_text_keymappath->Show(false);
    m_button_keymappath->Show(false);
#endif

    //#ifndef TS_NORMAL_VERSION
    //m_checkbox_showform->Show(false);
    //#endif
      m_boxsizer_1 = new wxBoxSizer(wxHORIZONTAL);
      m_boxsizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
      m_boxsizer_1->Add(m_button_ok, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
      m_boxsizer_1->Add(m_button_apply, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
      m_boxsizer_1->Add(m_button_cancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
      m_boxsizer_1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
      m_global_sizer->Add(m_boxsizer_1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
      SetSizer(m_global_sizer);
      m_global_sizer->Fit(this);
      m_global_sizer->SetSizeHints(this);
      init_programsettings(&progsettings);
      Benc bc;
      bc.LoadProgramSettings(&progsettings);
      display_settings();
      //dialog_count = true;
      //  }


	Refresh();
	Update();

#ifdef __WXMSW__

	wxSize sz = GetClientSize();
	sz.x += 1;
	SetClientSize(sz);
	Update();
	//SendSizeEvent();

#endif


}


SettingsDialog::~SettingsDialog()
{
  // if (dialog_count == true)
  //  {
      // if (m_static_rdpbase) {delete m_static_rdpbase; m_static_rdpbase = NULL;}
      // if (m_text_rdpbase) {delete  m_text_rdpbase; m_text_rdpbase = NULL;}
      // if (m_button_rdpbase) {delete m_button_rdpbase; m_button_rdpbase = NULL;}

      // if (m_static_rdpkey) {delete m_static_rdpkey ; m_static_rdpkey = NULL;}
      // if (m_text_rdpkey) {delete  m_text_rdpkey; m_text_rdpkey = NULL;}
      // if (m_button_rdpkey) {delete  m_button_rdpkey; m_button_rdpkey = NULL;}

      // if (m_static_rdesktop) {delete  m_static_rdesktop; m_static_rdesktop = NULL;}
      // if (m_text_rdesktop) {delete  m_text_rdesktop; m_text_rdesktop = NULL;}
      // if (m_button_rdesktop) {delete m_button_rdesktop ; m_button_rdesktop = NULL;}

      // if (m_static_keymappath) {delete  m_static_keymappath; m_static_keymappath = NULL;}
      // if (m_text_keymappath) {delete  m_text_keymappath; m_text_keymappath = NULL;}
      // if (m_button_keymappath) {delete m_button_keymappath ; m_button_keymappath = NULL;}

  if(m_button_ok) {delete m_button_ok; m_button_ok = NULL;}
  if(m_button_cancel) {delete m_button_cancel; m_button_cancel = NULL;}
  if(m_button_apply) {delete m_button_apply; m_button_apply = NULL;}

      // if (m_checkbox_icontray) {delete m_checkbox_icontray; m_checkbox_icontray = NULL;}
      // if (m_checkbox_showform) {delete m_checkbox_showform; m_checkbox_showform = NULL;}
      // wxDELETE(m_combobox_usecrypt);
      // if (m_checkbox_usecrypt) {delete m_checkbox_usecrypt; m_checkbox_usecrypt = NULL;}
      // if (m_checkbox_focuspage) {delete m_checkbox_focuspage; m_checkbox_focuspage = NULL;}
      // if (m_checkbox_autosave) {delete m_checkbox_autosave; m_checkbox_autosave = NULL;}

      // if (m_static_fcount) {delete m_static_fcount; m_static_fcount = NULL;}
      // if (m_text_fcount) {delete m_text_fcount; m_text_fcount = NULL;}

      // if (m_static_fmax) {delete m_static_fmax; m_static_fmax = NULL;}
      // if (m_text_fmax) {delete m_text_fmax; m_text_fmax = NULL;}

      // if (m_line_separator) {delete m_line_separator; m_line_separator = NULL;}
      // if (m_line_button) {delete m_line_button; m_line_button = NULL;}

      // if (m_static_lang) {delete m_static_lang; m_static_lang = NULL;}
      // if (m_combobox_lang) {delete m_combobox_lang; m_combobox_lang = NULL;}
      // if (m_checkbox_grabkbd) {delete m_checkbox_grabkbd; m_checkbox_grabkbd = NULL;}

      // if (m_boxsizer_11) {delete m_boxsizer_11; m_boxsizer_11 = NULL;}
      // if (m_boxsizer_8) {delete m_boxsizer_8; m_boxsizer_8 = NULL;}
      // if (m_boxsizer_9) {delete m_boxsizer_9; m_boxsizer_9 = NULL;}
      // if (m_boxsizer_10) {delete m_boxsizer_10; m_boxsizer_10 = NULL;}
      // if (m_boxsizer_6) {delete m_boxsizer_6; m_boxsizer_6 = NULL;}
      // if (m_boxsizer_7) {delete m_boxsizer_7; m_boxsizer_7 = NULL;}
      // if (m_boxsizer_4) {delete m_boxsizer_4; m_boxsizer_4 = NULL;}
      // if (m_boxsizer_5) {delete m_boxsizer_5; m_boxsizer_5 = NULL;}
      // if (m_boxsizer_3) {delete m_boxsizer_3; m_boxsizer_3 = NULL;}
      // if (m_boxsizer_1) {delete m_boxsizer_1; m_boxsizer_1 = NULL;}
      // if (m_boxsizer_2) {delete m_boxsizer_2; m_boxsizer_2 = NULL;}



      //if (m_panel_1) {delete m_panel_1; m_panel_1 = NULL;}
      //if (m_panel_2) {delete m_panel_2; m_panel_2 = NULL;}
      delete m_notebook;
      //if (m_global_sizer) {delete m_global_sizer; m_global_sizer = NULL;}


      //      std::cout << "DEST" << std::endl;

      //dialog_count = false;
      //}

}

///////////////////////////////////////////////////////////////////////////////
//! \brief Create default options for RDPDesk
///////////////////////////////////////////////////////////////////////////////
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
	ps->lang = _("Default");
	ps->grabkbd = true;
}

///////////////////////////////////////////////////////////////////////////////
//! \brief process cancel button event
///////////////////////////////////////////////////////////////////////////////
void SettingsDialog::button_cancel_func(wxCommandEvent &event)
{
	this->EndModal(0);
}
///////////////////////////////////////////////////////////////////////////////
//! \brief process apply button event
///////////////////////////////////////////////////////////////////////////////
void SettingsDialog::button_apply_func(wxCommandEvent &event)
{
	progsettings = fill_settings();
	save_to_file();
	main_settings = progsettings;
	apply_settings();
}
///////////////////////////////////////////////////////////////////////////////
//! \brief process ok button event
///////////////////////////////////////////////////////////////////////////////
void SettingsDialog::button_ok_func(wxCommandEvent &event)
{
	progsettings = fill_settings();
	save_to_file();
	main_settings = progsettings;
	apply_settings();
	this->EndModal(1);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief process keys
///////////////////////////////////////////////////////////////////////////////
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

   TREEPANEL(main_frame->m_panel_tree)->rdptree->fcount = progsettings.favorites_count;
   TREEPANEL(main_frame->m_panel_tree)->rdptree->fmax = progsettings.favorites_max;
   TREEPANEL(main_frame->m_panel_tree)->favorites->fcount = progsettings.favorites_count;
   TREEPANEL(main_frame->m_panel_tree)->favorites->fmax = progsettings.favorites_max;

   //FIXME: need apply new lang with out restart application
   wxLocale locale;
   LocaleHelper lh;
   lh.SetLang(progsettings.lang ,&locale);
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
   wxString  crypt_methods[] = {_("Simple"), _("AES")};
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
   if (m_checkbox_grabkbd != NULL)
   {
	  // m_checkbox_grabkbd->SetValue((progsettings.grabkbd != 0));
      m_checkbox_grabkbd->SetValue(progsettings.grabkbd);
   }

   m_combobox_lang->SetValue(progsettings.lang);
}

///////////////////////////////////////////////////////////////////////////////
//! \brief process cansel button event
///////////////////////////////////////////////////////////////////////////////
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
      if (m_checkbox_icontray->GetValue())
      {
	 progsettings.bIcon = TRUE;
      }
      else
      {
	 progsettings.bIcon = FALSE;
      }
   }

   if (m_checkbox_showform != NULL)
   {
      if (m_checkbox_showform->GetValue())
      {
	 progsettings.bShowFrame = TRUE;
      }
      else
      {
	 progsettings.bShowFrame = FALSE;
      }
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

   if (m_checkbox_grabkbd != NULL)
   {
      progsettings.grabkbd = m_checkbox_grabkbd->GetValue();
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
	wxDirDialog * dd = new wxDirDialog(this,_("Choose rdesktop path"));
	int iRes = dd->ShowModal();
	if (iRes == wxID_CANCEL) {delete dd; return;}
	progsettings.rdesktoppath = dd->GetPath();
	m_text_rdesktop->SetValue(progsettings.rdesktoppath);
	delete dd;

}

void SettingsDialog::button_rdpbase_func(wxCommandEvent &event)
{
	wxDirDialog * dd = new wxDirDialog(this,_("Choose rdpbase path"));
	int iRes = dd->ShowModal();
	if (iRes == wxID_CANCEL) {delete dd; return;}
	progsettings.rdpbasepath  = dd->GetPath();
	m_text_rdpbase->SetValue(progsettings.rdpbasepath);
	delete dd;
}

void SettingsDialog::button_rdpkey_func(wxCommandEvent &event)
{
	wxDirDialog * dd = new wxDirDialog(this,_("Choose rdpkey path"));
	int iRes = dd->ShowModal();
	if (iRes == wxID_CANCEL) {delete dd; return;}
	progsettings.rdpkeypath = dd->GetPath();
	m_text_rdpkey->SetValue(progsettings.rdpkeypath);
	delete dd;
}

void SettingsDialog::button_keymap_func(wxCommandEvent &event)
{
	wxDirDialog * dd = new wxDirDialog(this,_("Choose rdesktop keyboard map path"));
	int iRes = dd->ShowModal();
	if (iRes == wxID_CANCEL) {delete dd; return;}
	progsettings.rdesktop_key_path = dd->GetPath();
	m_text_keymappath->SetValue(progsettings.rdesktop_key_path);
	delete dd;
}

