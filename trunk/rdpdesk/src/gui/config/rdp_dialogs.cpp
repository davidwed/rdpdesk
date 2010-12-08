///////////////////////////////////////////////////////////////////////////////
// File name:   rdp_dialogs.cpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-12-08 13:24:19"
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
EVT_BUTTON    (ID_BUTTON_CANCEL,         RDPDialog::OnButtonCancel)
EVT_BUTTON    (ID_BUTTON_SAVE,           RDPDialog::OnButtonSave)
EVT_COMBOBOX  (ID_COMBOBOX_PROTO_CHANGE, RDPDialog::OnProtoChange)
//EVT_CHAR_HOOK(RDPDialog::dialog_hotkeys)
END_EVENT_TABLE()



///////////////////////////////////////////////////////////////////////////////
//! \brief Constructor for Dialog connection configurations
//! \param wxWindow* parent - parant element
///////////////////////////////////////////////////////////////////////////////
RDPDialog::RDPDialog(MainFrame *parent) :
   wxDialog(parent, wxID_ANY, _("Connection settings"),
            wxDefaultPosition, wxSize(750, 572), wxCAPTION),
   main_frame(parent),
   m_button_ok(NULL),
   m_button_cancel(NULL),
   m_button_save(NULL),

   sizer_btn(NULL),

   m_static_protocol(NULL),
   m_combobox_protocol(NULL),
   m_line_buttons(NULL),
   m_line_protocol(NULL),

   sizer_protocol(new wxBoxSizer(wxHORIZONTAL)),

   rdpOptionsDialog(new RDPOptionsDialog(this)),
   icaOptionsDialog(new ICAOptionsDialog(this)),
   rfbOptionsDialog(new RFBOptionsDialog(this)),
#ifdef __WXGTK__
   frdpOptionsDialog(new FRDPOptionsDialog(this)),
#endif
	sizer_main(new wxBoxSizer(wxVERTICAL))
{
   Options_HashMap local_options;
   bool found_duplicate;
   m_static_protocol = new wxStaticText(this, wxID_ANY, _("Protocol     "));
   const wxString arrProto[] = {_("RDP Protocol"), // TODO: code duplication
                                _("ICA Protocol"), // see fastcon_dialog.cpp
                                _("VNC Protocol")
#ifdef __WXGTK__
				,_("RDP Protocol (freerdp)")
#endif
   };
   m_combobox_protocol = new wxComboBox(this,
                                        ID_COMBOBOX_PROTO_CHANGE,
                                        wxT(""),
                                        wxDefaultPosition,
                                        wxDefaultSize,
                                        WXSIZEOF(arrProto),
                                        arrProto,
                                        wxCB_READONLY);
   m_combobox_protocol->SetSelection(0);
   m_combobox_protocol->SetValue(arrProto[0]);

   sizer_protocol->Add(m_static_protocol,0,wxALL,7);
   sizer_protocol->Add(m_combobox_protocol,0,wxALL | wxEXPAND,4);

   m_line_protocol = new wxStaticLine(this,
                                      wxID_ANY,
                                      wxDefaultPosition,
                                      wxSize(-1, 2),
                                      wxLI_HORIZONTAL);

   for (int i = 0; i < (int)main_frame->all_connection_records.Count(); i++) {
      local_options = main_frame->all_connection_records.Item(i);
      for (int f = 0 ; f < (int)group_list.Count() ; f++)
         if ( group_list.Item(f) == local_options[wxT("group_name")])
            found_duplicate = true;
      if (found_duplicate == false)
         group_list.Add(local_options[wxT("group_name")]);
      found_duplicate = false;
   }
   group_list.Sort();
   group_list.Shrink();

   rdpOptionsDialog->Show(false);
   rdpOptionsDialog->SetGroupList(group_list);
   rdpOptionsDialog->Build();

   icaOptionsDialog->Show(false);
   icaOptionsDialog->SetGroupList(group_list);
   icaOptionsDialog->Build();

   rfbOptionsDialog->Show(false);
   rfbOptionsDialog->SetGroupList(group_list);
   rfbOptionsDialog->Build();
#ifdef __WXGTK__
   frdpOptionsDialog->Show(false);
   frdpOptionsDialog->SetGroupList(group_list);
   frdpOptionsDialog->Build();
#endif
	sizer_main = new wxBoxSizer(wxVERTICAL);

	sizer_main->Add(sizer_protocol,0, wxALL | wxEXPAND,0 );
	sizer_main->Add(m_line_protocol,0,wxALL | wxALIGN_CENTER | wxEXPAND,5);
	sizer_main->Add(icaOptionsDialog,1,wxALIGN_CENTER | wxALL |wxEXPAND, 5 );
	sizer_main->Add(rdpOptionsDialog,1,wxALIGN_CENTER | wxALL | wxEXPAND, 5 );
	sizer_main->Add(rfbOptionsDialog,1,wxALIGN_CENTER | wxALL | wxEXPAND, 5 );
#ifdef __WXGTK__
	sizer_main->Add(frdpOptionsDialog,1,wxALIGN_CENTER | wxALL | wxEXPAND, 5 );
#endif
	sizer_btn = new wxBoxSizer(wxHORIZONTAL);

	m_line_buttons = new wxStaticLine(this,wxID_ANY,wxDefaultPosition, wxSize(-1,2) ,wxLI_HORIZONTAL);

	if (new_uniq_name)
	{
		m_button_ok = new wxButton(this,ID_BUTTON_OK, _("Connect"), wxDefaultPosition, wxSize(100,-1) );
		sizer_btn->Add(m_button_ok,0,wxALL,5);
	}
	m_button_cancel = new wxButton(this,ID_BUTTON_CANCEL, _("Cancel"), wxDefaultPosition, wxSize(100,-1) );
	m_button_save = new wxButton(this,ID_BUTTON_SAVE, _("Save"), wxDefaultPosition, wxSize(100,-1));
	sizer_btn->Add(m_button_cancel, 0, wxALL, 5);
	sizer_btn->Add(m_button_save, 0, wxALL, 5);

	sizer_main->Add(m_line_buttons, 0, wxALL | wxALIGN_CENTER | wxEXPAND, 5);
	sizer_main->Add(sizer_btn, 0, wxALIGN_CENTER | wxALL|wxBOTTOM /*| wxEXPAND*/, 5);
	SetSizer(sizer_main);
	ChangeProto();
	//SetSize(GetEffectiveMinSize());
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Setup options
//! \param const Options_HashMap *options - pointer to options array
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPDialog::SetOptions(const Options_HashMap *options) {
	//Options_HashMap local_options;
	if (options != NULL) {
		local_options = *options;
		//m_combobox_protocol->SetSelection(wxAtoi(local_options[wxT("proto")]));
		if (local_options[wxT("proto")] == wxT("rdp")) {
			m_combobox_protocol->SetSelection(0);
		}
		else if (local_options[wxT("proto")] == wxT("ica")) {
			m_combobox_protocol->SetSelection(1);
		}
		else if (local_options[wxT("proto")] == wxT("rfb")) {
			m_combobox_protocol->SetSelection(2);
		}

		else if (local_options[wxT("proto")] == wxT("frdp")) {
			m_combobox_protocol->SetSelection(3);
		}
		else {
			m_combobox_protocol->SetSelection(0);
		}
		ChangeProto();

		for (int j = 0; j < (int)group_list.Count(); ++j)
			if ((group_list.Item(j).Len() == 0) || (group_list.Item(j) == wxT("Main")))
				group_list.RemoveAt(j);
      //std::cout << group_list.Count() << std::endl;

		if ((rdpOptionsDialog != NULL) && (rdpOptionsDialog->IsShown())) {
			rdpOptionsDialog->SetOptions(options);
			rdpOptionsDialog->SetGroupList(group_list);
		}

		if ((icaOptionsDialog != NULL) && (icaOptionsDialog->IsShown())) {
			icaOptionsDialog->SetOptions(options);
			icaOptionsDialog->SetGroupList(group_list);
		}

		if((rfbOptionsDialog != NULL) && (rfbOptionsDialog->IsShown())) {
			rfbOptionsDialog->SetOptions(options);
			rfbOptionsDialog->SetGroupList(group_list);
		}
#ifdef __WXGTK__
		if((frdpOptionsDialog != NULL) && (frdpOptionsDialog->IsShown())) {
			frdpOptionsDialog->SetOptions(options);
			frdpOptionsDialog->SetGroupList(group_list);
		}
#endif
	}
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Return Options array
///////////////////////////////////////////////////////////////////////////////
Options_HashMap RDPDialog::GetOptions() const {
   Options_HashMap local_options;

   //FIXME: Add checking for proto record
   local_options.clear();
   if ((rdpOptionsDialog != NULL) && (rdpOptionsDialog->IsShown()))
      local_options = rdpOptionsDialog->Get_Options();

   if ((icaOptionsDialog != NULL) && (icaOptionsDialog->IsShown()))
      local_options = icaOptionsDialog->Get_Options();

   if((rfbOptionsDialog != NULL) && (rfbOptionsDialog->IsShown()))
      local_options = rfbOptionsDialog->GetOptions();
#ifdef __WXGTK__
   if((frdpOptionsDialog != NULL) && (frdpOptionsDialog->IsShown()))
      local_options = frdpOptionsDialog->GetOptions();
#endif
   const int selection = m_combobox_protocol->GetSelection();
   switch (selection) {
      case 0: local_options[wxT("proto")] = wxT("rdp");
              break;
      case 1: local_options[wxT("proto")] = wxT("ica");
              break;
      case 2: local_options[wxT("proto")] = wxT("rfb");
              break;
#ifdef __WXGTK__
	  case 3: local_options[wxT("proto")] = wxT("frdp");
              break;
#endif
      default: local_options[wxT("proto")] = wxT("rdp");
	}

	//std::cout << local_options.size() << std::endl;
	//wxMessageBox( local_options[wxT("hostname")]);

	return local_options;
}


///////////////////////////////////////////////////////////////////////////////
//! \brief Destructor
///////////////////////////////////////////////////////////////////////////////
RDPDialog::~RDPDialog() {
   // if(notebook != NULL)
   // {
   // 	end = notebook->GetPageCount();
   // 	notebook->DeleteAllPages();
   // 	delete notebook;
   // 	notebook = NULL;
   // }
   if (rdpOptionsDialog != NULL)
   {
      delete rdpOptionsDialog;
      rdpOptionsDialog = NULL;
   }
   if (icaOptionsDialog != NULL)
   {
      delete icaOptionsDialog;
      icaOptionsDialog = NULL;
   }

   if(rfbOptionsDialog != NULL) {
      delete rfbOptionsDialog;
      rfbOptionsDialog = NULL;
   }

#ifdef __WXGTK__
   if(frdpOptionsDialog != NULL)
   {
	delete frdpOptionsDialog;
	frdpOptionsDialog = NULL;
   }
#endif
   if(m_button_ok != NULL) {delete m_button_ok; m_button_ok = NULL;}
   if(m_button_cancel != NULL) {delete m_button_cancel; m_button_cancel = NULL;}
   if(m_button_save != NULL) {delete m_button_save; m_button_save = NULL;}

   sizer_protocol->Clear();
   wxDELETE(m_line_buttons);
   wxDELETE(m_line_protocol);

}
///////////////////////////////////////////////////////////////////////////////
//! \brief Processe chnage proto combobox
//! \sa ChangeProto
///////////////////////////////////////////////////////////////////////////////
void RDPDialog::OnProtoChange(wxCommandEvent &event) {
	ChangeProto();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Processe chnage proto combobox
///////////////////////////////////////////////////////////////////////////////
void RDPDialog::ChangeProto() {
	// TODO: rewrite it.. remove magic numbers, write in OOP-style

   const int selection = m_combobox_protocol->GetSelection();
   rdpOptionsDialog->Hide();
   icaOptionsDialog->Hide();
   rfbOptionsDialog->Hide();
#ifdef __WXGTK__
   frdpOptionsDialog->Hide();
#endif
   switch (selection) {
   case 0: // RDP
           rdpOptionsDialog->Show();
           break;
   case 1: // ICA
           icaOptionsDialog->Show();
           break;
   case 2: // RFB
           rfbOptionsDialog->Show();
           break;
#ifdef __WXGTK__
   case 3: // FreeRDP
           frdpOptionsDialog->Show();
           break;
#endif
   }
   sizer_main->Layout();
}

///////////////////////////////////////////////////////////////////////////////
//! \brief Processed Cansel button
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPDialog::OnButtonCancel(wxCommandEvent &event)
{
   if (rdpOptionsDialog != NULL) {
      delete rdpOptionsDialog;
      rdpOptionsDialog = NULL;
   }
   if (icaOptionsDialog != NULL) {
      delete icaOptionsDialog;
      icaOptionsDialog = NULL;
   }
   if (rfbOptionsDialog != NULL) {
      delete rfbOptionsDialog;
      rfbOptionsDialog = NULL;
   }
#ifdef __WXGTK__
   if (frdpOptionsDialog != NULL)
   {
      delete frdpOptionsDialog;
      frdpOptionsDialog = NULL;
   }
#endif
   this->EndModal(RDPDLG_RET_CANCEL);
}
///////////////////////////////////////////////////////////////////////////////
//! \brief Process Save button
//! \param
//! \return
//! \sa
///////////////////////////////////////////////////////////////////////////////
void RDPDialog::OnButtonSave(wxCommandEvent &event) {
   //Benc bc;
   Options_HashMap local_options = GetOptions();
   if (local_options.size() > 0 ) {
      //wxMessageBox(local_options[wxT("uniq_name")]);
      main_frame->AddConnectionRecord(&local_options);
      //bc.Save(&main_frame->all_connection_records);
      if (local_options[wxT("hostname")].IsEmpty()) {
	 wxMessageBox(_("The \"Server\" cannot be empty"));
      } else {
	 EndModal(RDPDLG_RET_SAVE);
      }
   }
}
