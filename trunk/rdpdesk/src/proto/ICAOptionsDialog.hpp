///////////////////////////////////////////////////////////////////////////////
// File name:   ICAOptionsDialog.hpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-17 12:23:54" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#ifndef ICA_OPTIONS_DIALOG
#define ICA_OPTIONS_DIALOG

#include "bencoded.hpp"
#include <wx/spinctrl.h>

enum ICA_ERRORS {
   ICA_ERROR_OK = 0,
   ICA_ERROR_PARENT_IS_NULL,
   ICA_ERROR_CANT_CREATE_NOTEBOOK,
   ICA_ERROR_CANT_CREATE_PANEL,
   ICA_ERROR_EMPTY_HOSTNAME
};

#ifdef __WXMSW__
#define CONTROL_H 20
#define SHIFT_H 1
#endif

#ifdef __WXGTK__
#define CONTROL_H 25
#define SHIFT_H 4
#endif

enum {
	ID_ICA_CHECKBOX_CUSTOMPORT  = 123,
	ID_ICA_TEXTCTRL_PASSWORD    = 124,
	ID_ICA_CHECKBOX_AUTOLOGON   = 125,
	ID_ICA_CHK_CONTROLSIZE      = 126,
	ID_ICA_CHECKBOX_USE_PROGRAM = 1127,
	ID_ICA_COMBOBOX_SPEED       = 128,
	ID_ICA_SSL_ENABLE           = 1129,
	ID_ICA_DESIRED_COLOR        = 1130,
};

class ICAOptions
{
public:
   ICAOptions();
   ~ICAOptions();
private:

};


class ICAOptionsDialog;


class ICAOptionsDialog_General : public wxPanel
{
public:
   ICAOptionsDialog_General(ICAOptionsDialog *parent_element);
   virtual ~ICAOptionsDialog_General();
   int Build();
   Options_HashMap Get_Options();
   bool SetOptions(const Options_HashMap *all_options);
   void SetGroupList(const wxArrayString &groupList);
private:
   ICAOptionsDialog *parent;

   int error;

   wxFlexGridSizer *m_flexgrid_sizer_1;
   wxFlexGridSizer *m_flexgrid_sizer_2;
   wxFlexGridSizer *m_flexgrid_sizer_3;
   wxBoxSizer *m_boxsizer_1;
   wxBoxSizer *m_boxsizer_2;
   wxBoxSizer *m_boxsizer_3;

   wxStaticText *staticServer;
   wxTextCtrl *textServer;

   wxStaticText *staticPort;
   wxTextCtrl *textPort;

   wxStaticText * staticUsername;
   wxTextCtrl * textUsername;

   wxStaticText * staticPasswd;
   wxTextCtrl * textPasswd;

   wxStaticText * staticDomain;
   wxTextCtrl * textDomain;

   wxCheckBox *checkboxAutologon;
   wxCheckBox *checkboxCustomPort;
//   wxCheckBox *m_checkbox_attachtoconsole;

   wxStaticText *staticGroupName;
   wxComboBox *comboboxGroupName;

   wxStaticText *staticConnectionName;
   wxTextCtrl *textConnectionName;

   wxStaticText *staticProxyAutoconfigUrl;
   wxTextCtrl *textProxyAutoconfigUrl;

   wxStaticText *staticProxyUsername;
   wxTextCtrl *textProxyUsername;

   wxStaticText *staticProxyPassword;
   wxTextCtrl *textProxyPassword;

   wxStaticText *staticProxyHost;
   wxTextCtrl *textProxyHost;

   wxStaticText *staticProxyType;
   wxComboBox *comboboxProxyType;

   wxStaticText *staticProxyBypassList;
   wxTextCtrl *textProxyBypassList;

   wxStaticText *staticProxyTimeout;
   wxSpinCtrl *spinctrlProxyTimeout;

   wxStaticText *staticSslProxyHost;
   wxTextCtrl *textSslProxyHost;

   wxCheckBox *checkboxSslEnable;
   wxCheckBox *checkboxProxyUseDefault;

   int groupcount;
   wxArrayString arrayStringGroupList;
   //base_conn *rdp_base;
   //GeneralOptionsArray options;
   //wxArrayString array_string_group_list; 


   void GetGroupList();
   void checkbox_customport_func(wxCommandEvent &event);
   void checkbox_customport();
   void text_passwordentry_func(wxCommandEvent &event);
   DECLARE_EVENT_TABLE();

};

class ICAOptionsDialog_Display: public wxPanel
{
public:
   ICAOptionsDialog_Display(ICAOptionsDialog *parent_element);
   virtual ~ICAOptionsDialog_Display();
   int Build();
   Options_HashMap Get_Options();
   bool SetOptions(const Options_HashMap *all_options);
private:
   ICAOptionsDialog *parent;
   int error;

   wxBoxSizer *m_boxsizer_1;
   wxBoxSizer *m_boxsizer_2;
   wxBoxSizer *m_boxsizer_3;
   wxBoxSizer *m_boxsizer_4;
   wxBoxSizer *m_boxsizer_5;
   wxBoxSizer *m_boxsizer_6;
   wxBoxSizer *m_boxsizer_7;
   wxBoxSizer *m_boxsizer_8;
   wxBoxSizer *m_boxsizer_9;
   wxBoxSizer *m_boxsizer_10;
   wxBoxSizer *m_boxsizer_11;
   wxBoxSizer *m_boxsizer_12;

   wxFlexGridSizer *m_flexgrid_sizer_1;
   wxRadioBox *m_radiobox_1;


   wxStaticText *m_static_width;
   wxTextCtrl *m_text_width;

   wxStaticText *m_static_height;
   wxTextCtrl *m_text_height;

//   wxStaticText *m_static_colordepth;
//   wxComboBox *m_choise_colordepth;

   wxStaticText *m_static_resolution;
   wxRadioButton *m_radiobutton_controlsize;
   wxRadioButton *m_radiobutton_fullscreen;
   wxRadioButton *m_radiobutton_customgeometry;
   wxRadioButton *m_radiobutton_presetsize;
   wxComboBox *m_combobox_resolution;
   wxCheckBox *m_checkbox_customgeometry;
   wxCheckBox *m_checkbox_controlsize;
   wxCheckBox *m_checkbox_fullscreen;
//   wxCheckBox *m_checkbox_smartsizing;
//   wxCheckBox *m_checkbox_updatescreen;
   wxCheckBox *m_checkbox_allow_degraded_colors;
   wxCheckBox *m_checkbox_enable_oss;
   wxCheckBox *checkboxEnableSessionSharingScreen;
   wxCheckBox *m_checkbox_approximate_colors;
   wxCheckBox *m_checkbox_resize_session;

   wxStaticText *m_static_desired_color;
   wxComboBox *m_combo_desired_color;

   wxStaticText *m_static_use_sdlvb;
   wxComboBox *m_combo_use_sdlvb;

   wxStaticText *m_static_font_smoothing_type;
   wxComboBox *m_combo_font_smoothing_type;

   wxStaticText *m_static_screen_percent;
   wxSpinCtrl *m_combo_screen_percent;
   
   wxArrayString GetResolutionsList();
   wxSize GetResolution(int i);


   int GetDefaultResolution();
   void checkbox_customgeometry_func(wxCommandEvent& event);
   void checkbox_fullscreen_func(wxCommandEvent &event);
   void checkbox_controlsize_func(wxCommandEvent &event);
   void change_desired_color_func(wxCommandEvent &event);
   void change_desired_color();
   void CheckICADisplay();
   DECLARE_EVENT_TABLE()
};

class ICAOptionsDialog_Sound : public wxPanel
{
public:
   ICAOptionsDialog_Sound(ICAOptionsDialog *parent_element);
   virtual ~ICAOptionsDialog_Sound();
   int Build();
   Options_HashMap Get_Options();
   bool SetOptions(const Options_HashMap *all_options);
//   wxString Get_Options_String();
private:

   int error;

   ICAOptionsDialog *parent;

   wxBoxSizer *m_boxsizer_1;
   wxBoxSizer *m_boxsizer_2;
   wxBoxSizer *m_boxsizer_3;
   wxBoxSizer *m_boxsizer_4;

   wxCheckBox *m_checkbox_client_audio;
   wxCheckBox *m_checkbox_enable_audio_input;

   wxCheckBox *m_checkbox_drives;
   wxCheckBox *m_checkbox_printers;
   wxCheckBox *m_checkbox_comports;
   wxCheckBox *m_checkbox_smartcards;


   wxStaticText *m_static_audio_bandwidth_limit;
   wxComboBox *m_combo_audio_bandwidth_limit;
};


class ICAOptionsDialog_Advanced : public wxPanel
{

public:
   ICAOptionsDialog_Advanced(ICAOptionsDialog *parent_element);
   virtual ~ICAOptionsDialog_Advanced();
   int Build();
   Options_HashMap Get_Options();
   bool SetOptions(const Options_HashMap *all_options);
   wxString Get_Options_String();

private:
   ICAOptionsDialog *parent;
   int error;

   wxBoxSizer *m_boxsizer_1;
   wxFlexGridSizer *m_flexgrid_sizer_1;
   wxGridSizer *m_gridsizer_1;

   wxCheckBox *m_checkbox_start_initial_program;

   wxStaticText *m_static_initial_program;
   wxTextCtrl *m_text_initial_program;

   wxStaticText *m_static_long_command_line;
   wxTextCtrl *m_text_long_command_line;

   wxStaticText *m_static_work_directory;
   wxTextCtrl *m_text_work_directory;

   wxCheckBox *m_checkbox_compress;
   wxCheckBox *m_checkbox_allow_duplicate_sessions;
   wxCheckBox *m_checkbox_persistent_cache_enabled;
   wxCheckBox *m_checkbox_desktop_restart_allowed;
   wxCheckBox *m_checkbox_mouse_sends_control_v;
   wxCheckBox *m_checkbox_echo_shift_keys;

   void checkbox_program_func(wxCommandEvent &event);
   void checkbox_program();

   DECLARE_EVENT_TABLE()

};

class ICAOptionsDialog_Security : public wxPanel
{

public:
   ICAOptionsDialog_Security(ICAOptionsDialog *parent_element);
   virtual ~ICAOptionsDialog_Security();
   int Build();
   Options_HashMap Get_Options();
   bool SetOptions(const Options_HashMap *all_options);
   wxString Get_Options_String();

private:
   ICAOptionsDialog *parent;
   int error;
   
   wxBoxSizer *m_boxsizer_1;
   wxFlexGridSizer *m_flexgrid_sizer_1;

   wxStaticText *m_static_encryption_level_session;
   wxComboBox *m_combo_encryption_level_session;

   wxStaticText *m_static_secure_channel_protocol;
   wxComboBox *m_combo_secure_channel_protocol;

   DECLARE_EVENT_TABLE()

};



class ICAOptionsDialog : public wxNotebook
{
public:

   ICAOptionsDialog(wxWindow *parent_element);
   ~ICAOptionsDialog();
   int Build();
   bool SetOptions(const Options_HashMap *options);
   void SetGroupList(const wxArrayString &groupList);
   Options_HashMap Get_Options();
private:
   wxWindow *parent;
   int error;
   ICAOptionsDialog_General *ica_general;
   ICAOptionsDialog_Display *ica_display;
//   ICAOptionsDialog_Share *ica_share;
   ICAOptionsDialog_Advanced *ica_advanced;
   ICAOptionsDialog_Sound *ica_sound;
   ICAOptionsDialog_Security *ica_security;

   Options_HashMap save_options;
   wxArrayString arrayStringGroupList;


};





#endif //ICAOPTIONSDIALOG
