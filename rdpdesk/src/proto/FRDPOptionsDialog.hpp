///////////////////////////////////////////////////////////////////////////////
// File name:   FRDPOptionsDialog.hpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef __FRDP_OPTIONS_DIALOG__
#define __FRDP_OPTIONS_DIALOG__

#include "bencoded.hpp"


enum FRDP_ERRORS
{
   FRDP_ERROR_OK = 0,
   FRDP_ERROR_PARENT_IS_NULL,
   FRDP_ERROR_CANT_CREATE_NOTEBOOK,
   FRDP_ERROR_CANT_CREATE_PANEL,
   FRDP_ERROR_EMPTY_HOSTNAME
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
	ID_FRDP_CHECKBOX_CUSTOMPORT  = 123,
	ID_FRDP_TEXTCTRL_PASSWORD    = 124,
	ID_FRDP_CHECKBOX_AUTOLOGON   = 125,
	ID_FRDP_CHK_CONTROLSIZE      = 126,
	ID_FRDP_CHECKBOX_USE_PROGRAM = 127,
	ID_FRDP_COMBOBOX_SPEED       = 128,
};

class FRDPOptions
{
public:
   FRDPOptions();
   ~FRDPOptions();
private:

};


class FRDPOptionsDialog;


class FRDPOptionsDialog_General : public wxPanel
{
public:
   FRDPOptionsDialog_General(FRDPOptionsDialog *parent_element);
   virtual ~FRDPOptionsDialog_General();
   int Build();
   Options_HashMap Get_Options();
   bool SetOptions(const Options_HashMap *all_options);
   void SetGroupList(const wxArrayString &group_list);
private:
   FRDPOptionsDialog *parent;

   int error;

   wxFlexGridSizer *m_flexgrid_sizer_1;
   wxFlexGridSizer *m_flexgrid_sizer_2;
   wxBoxSizer *m_boxsizer_1;
   wxBoxSizer *m_boxsizer_2;


   wxStaticText *m_static_server;
   wxTextCtrl *m_text_server;

   wxStaticText *m_static_port;
   wxTextCtrl *m_text_port;

   wxStaticText * m_static_username;
   wxTextCtrl * m_text_username;

   wxStaticText * m_static_password;
   wxTextCtrl * m_text_password;

   wxStaticText * m_static_domain;
   wxTextCtrl * m_text_domain;

   wxCheckBox *m_checkbox_autologon;
   wxCheckBox *m_checkbox_customport;
   wxCheckBox *m_checkbox_attachtoconsole;


   wxStaticText *m_static_groupname;
   wxComboBox *m_combobox_groupname;

   wxStaticText *m_static_connectionname;
   wxTextCtrl *m_text_connectionname;

   int groupcount;
   wxString *string_group_list;
   wxArrayString array_string_group_list;
   //base_conn *rdp_base;
   //GeneralOptionsArray options;
   //wxArrayString array_string_group_list;


   void get_group_list();
   void checkbox_customport_func(wxCommandEvent &event);
   void checkbox_customport();
   void text_passwordentry_func(wxCommandEvent &event);
   DECLARE_EVENT_TABLE();

};

class FRDPOptionsDialog_Display: public wxPanel
{
public:
   FRDPOptionsDialog_Display(FRDPOptionsDialog *parent_element);
   virtual ~FRDPOptionsDialog_Display();
   int Build();
   Options_HashMap Get_Options();
   bool SetOptions(const Options_HashMap *all_options);
private:
   FRDPOptionsDialog *parent;
   int error;

   wxBoxSizer *m_boxsizer_1;
   wxBoxSizer *m_boxsizer_2;
   wxBoxSizer *m_boxsizer_3;
   wxBoxSizer *m_boxsizer_4;
   wxBoxSizer *m_boxsizer_5;
   wxFlexGridSizer *m_flexgrid_sizer_1;
   wxRadioBox *m_radiobox_1;


   wxStaticText *m_static_width;
   wxTextCtrl *m_text_width;

   wxStaticText *m_static_height;
   wxTextCtrl *m_text_height;

   wxStaticText *m_static_colordepth;
   wxComboBox *m_choise_colordepth;

   wxStaticText *m_static_resolution;
   wxRadioButton *m_radiobutton_controlsize;
   wxRadioButton *m_radiobutton_fullscreen;
   wxRadioButton *m_radiobutton_customgeometry;
   wxRadioButton *m_radiobutton_presetsize;
   wxComboBox *m_combobox_resolution;
   wxCheckBox *m_checkbox_customgeometry;
   wxCheckBox *m_checkbox_controlsize;
   wxCheckBox *m_checkbox_fullscreen;
   wxCheckBox *m_checkbox_smartsizing;
   //wxCheckBox *m_checkbox_updatescreen;

   wxArrayString GetResolutionsList();
   wxSize GetResolution(int i);


   int GetDefaultResolution();
   void checkbox_customgeometry_func(wxCommandEvent& event);
   void checkbox_fullscreen_func(wxCommandEvent &event);
   void checkbox_controlsize_func(wxCommandEvent &event);
   void CheckFRDPDisplay();
   DECLARE_EVENT_TABLE()
};

class FRDPOptionsDialog_Share : public wxPanel
{
public:
   FRDPOptionsDialog_Share(FRDPOptionsDialog *parent_element);
   virtual ~FRDPOptionsDialog_Share();
   int Build();
   Options_HashMap Get_Options();
   bool SetOptions(const Options_HashMap *all_options);
   wxString Get_Options_String();

private:
   FRDPOptionsDialog *parent;
   int error;

   wxBoxSizer *m_boxsizer_1;
   wxBoxSizer *m_boxsizer_2;
   wxBoxSizer *m_boxsizer_3;
   wxBoxSizer *m_boxsizer_4;
   wxBoxSizer *m_boxsizer_5;
   wxBoxSizer *m_boxsizer_6;
   wxBoxSizer *m_boxsizer_7;
   wxFlexGridSizer *m_flexgrid_sizer_1;

   wxStaticText *m_static_sound;
   wxComboBox *m_choise_sound;

   wxStaticText *m_static_keyboard;
   wxComboBox *m_choise_keyboard;

   wxCheckBox *m_checkbox_drives;
   wxCheckBox *m_checkbox_printers;
   wxCheckBox *m_checkbox_comports;
   wxCheckBox *m_checkbox_smartcards;

   wxStaticText *m_static_linux_devices;
   wxTextCtrl *m_text_linux_devices;
};

class FRDPOptionsDialog_Program : public wxPanel
{

public:
   FRDPOptionsDialog_Program(FRDPOptionsDialog *parent_element);
   virtual ~FRDPOptionsDialog_Program();
   int Build();
   Options_HashMap Get_Options();
   bool SetOptions(const Options_HashMap *all_options);
private:
   FRDPOptionsDialog *parent;
   int error;
   wxBoxSizer *m_boxsizer_1;
   wxStaticText * m_static_program;
   wxTextCtrl * m_text_program;

   wxStaticText * m_static_workdir;
   wxTextCtrl * m_text_workdir;

   wxCheckBox	*m_checkbox_program;
   //wxCheckBox	*m_checkbox_maximized;

   void checkbox_program_func(wxCommandEvent &event);
   void checkbox_program();

   DECLARE_EVENT_TABLE()

};

class FRDPOptionsDialog_Performance : public wxPanel
{
public:
   FRDPOptionsDialog_Performance(FRDPOptionsDialog *parent_element);
   virtual ~FRDPOptionsDialog_Performance();
   int Build();
   Options_HashMap Get_Options();
   bool SetOptions(const Options_HashMap *all_options);
private:
   FRDPOptionsDialog *parent;
   int error;

   wxBoxSizer *m_boxsizer_1;
   wxBoxSizer *m_boxsizer_2;
   wxBoxSizer *m_boxsizer_3;
   wxBoxSizer *m_boxsizer_4;
   wxBoxSizer *m_boxsizer_5;
   wxBoxSizer *m_boxsizer_6;

   wxStaticText *m_static_speed;
   wxComboBox *m_choise_speed;

   wxStaticText * m_static_performance;

   // wxCheckBox	*m_checkbox_enablewallpaper;
   // wxCheckBox	*m_checkbox_fullwindowdrag;
   // wxCheckBox	*m_checkbox_animation;
   // wxCheckBox	*m_checkbox_themes;
   // wxCheckBox	*m_checkbox_bitmapcaching;

   wxString ChoisesSpeed[5];

   wxBoxSizer * sizer_speed;
   wxBoxSizer * sizer_static;
   wxFlexGridSizer * sizer_perf;
   wxBoxSizer * sizer_window;
   wxBoxSizer * sizer_top;

   void choise_speed_func(wxCommandEvent &event);
   void choise_speed();


   DECLARE_EVENT_TABLE()

};

class FRDPOptionsDialog_Advanced : public wxPanel
{

public:
   FRDPOptionsDialog_Advanced(FRDPOptionsDialog *parent_element);
   virtual ~FRDPOptionsDialog_Advanced();
   int Build();
   Options_HashMap Get_Options();
   bool SetOptions(const Options_HashMap *all_options);
   wxString Get_Options_String();


private:
   FRDPOptionsDialog *parent;
   int error;

   wxBoxSizer *m_boxsizer_1;
   wxBoxSizer *m_boxsizer_2;
   wxBoxSizer *m_boxsizer_3;
   wxBoxSizer *m_boxsizer_4;
   wxGridSizer *m_gridsizer_1;

   //wxStaticText *m_static_rdpversion;
   //wxComboBox	*m_choise_rdpversion;
   //wxCheckBox	*m_checkbox_french;
   //wxCheckBox	*m_checkbox_encryption_enable;
   //wxCheckBox	*m_checkbox_backingstore;
   //wxCheckBox	*m_checkbox_usemouse;
   //wxCheckBox	*m_checkbox_privatecolormap;
   wxCheckBox	*mCheckboxClipboardSync;
   wxCheckBox	*m_checkbox_enablecompress;

   wxStaticText *m_static_keyboard_map;
   wxComboBox *m_combobox_keyboard_map;
};

class FRDPOptionsDialog : public wxNotebook
{
public:

   FRDPOptionsDialog(wxWindow *parent_element);
   ~FRDPOptionsDialog();
   int Build();
   bool SetOptions(const Options_HashMap *options);
   void SetGroupList(const wxArrayString &group_list);
   Options_HashMap GetOptions();
private:
   wxWindow *parent;
   int error;
   FRDPOptionsDialog_General *rdp_general;
   FRDPOptionsDialog_Display *rdp_display;
   FRDPOptionsDialog_Share *rdp_share;
   FRDPOptionsDialog_Program *rdp_program;
   FRDPOptionsDialog_Performance *rdp_perfomance;
   FRDPOptionsDialog_Advanced *rdp_advanced;
   Options_HashMap save_options;
   wxArrayString array_string_group_list;


};





#endif //__FRDP_OPTIONS_DIALOG__
