///////////////////////////////////////////////////////////////////////////////
// File name:   RDPOptionsDialog.hpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-07 13:09:11" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#ifndef RDP_OPTIONS_DIALOG
#define RDP_OPTIONS_DIALOG

#include "bencoded.hpp"


enum ERRORS
{
   ERROR_OK = 0,
   ERROR_PARENT_IS_NULL,
   ERROR_CANT_CREATE_NOTEBOOK,
   ERROR_CANT_CREATE_PANEL,
   ERROR_EMPTY_HOSTNAME
};

#ifdef __WXMSW__
#define CONTROL_H 20
#define SHIFT_H 1
#endif

#ifdef __WXGTK__
#define CONTROL_H 25
#define SHIFT_H 4
#endif

const int ID_RDP_CHECKBOX_CUSTOMPORT = 123;
const int ID_RDP_TEXTCTRL_PASSWORD = 124;
const int ID_RDP_CHECKBOX_AUTOLOGON = 125;
const int ID_RDP_CHK_CONTROLSIZE = 126;
const int ID_RDP_CHECKBOX_USE_PROGRAM = 127;
const int ID_RDP_COMBOBOX_SPEED = 128;




class RDPOptions
{
public:
   RDPOptions();
   ~RDPOptions();
private:

};


class RDPOptionsDialog;


class RDPOptionsDialog_General : public wxPanel
{
public:
   RDPOptionsDialog_General(RDPOptionsDialog *parent_element);
   virtual ~RDPOptionsDialog_General();
   int Build();
   Options_HashMap Get_Options();
   bool Set_Options(const Options_HashMap *all_options);
   void Set_Group_List(wxArrayString group_list);
private:
   RDPOptionsDialog *parent;

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

class RDPOptionsDialog_Display: public wxPanel
{
public:
   RDPOptionsDialog_Display(RDPOptionsDialog *parent_element);
   virtual ~RDPOptionsDialog_Display();
   int Build();
   Options_HashMap Get_Options();
   bool Set_Options(const Options_HashMap *all_options);
private:
   RDPOptionsDialog *parent;
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
   wxCheckBox *m_checkbox_updatescreen;

   wxArrayString GetResolutionsList();
   wxSize GetResolution(int i);


   int GetDefaultResolution();
   void checkbox_customgeometry_func(wxCommandEvent& event);
   void checkbox_fullscreen_func(wxCommandEvent &event);
   void checkbox_controlsize_func(wxCommandEvent &event);
   void CheckRDPDisplay();
   DECLARE_EVENT_TABLE()
};

class RDPOptionsDialog_Share : public wxPanel
{
public:
   RDPOptionsDialog_Share(RDPOptionsDialog *parent_element);
   virtual ~RDPOptionsDialog_Share();
   int Build();
   Options_HashMap Get_Options();
   bool Set_Options(const Options_HashMap *all_options);
   wxString Get_Options_String();

private:
   RDPOptionsDialog *parent;
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

class RDPOptionsDialog_Program : public wxPanel
{

public:
   RDPOptionsDialog_Program(RDPOptionsDialog *parent_element);
   virtual ~RDPOptionsDialog_Program();
   int Build();
   Options_HashMap Get_Options();
   bool Set_Options(const Options_HashMap *all_options);
private:
   RDPOptionsDialog *parent;
   int error;
   wxBoxSizer *m_boxsizer_1;
   wxStaticText * m_static_program;
   wxTextCtrl * m_text_program;

   wxStaticText * m_static_workdir;
   wxTextCtrl * m_text_workdir;

   wxCheckBox	*m_checkbox_program;
   wxCheckBox	*m_checkbox_maximized;

   void checkbox_program_func(wxCommandEvent &event);
   void checkbox_program();

   DECLARE_EVENT_TABLE()

};

class RDPOptionsDialog_Performance : public wxPanel
{
public:
   RDPOptionsDialog_Performance(RDPOptionsDialog *parent_element);
   virtual ~RDPOptionsDialog_Performance();
   int Build();
   Options_HashMap Get_Options();
   bool Set_Options(const Options_HashMap *all_options);
private:
   RDPOptionsDialog *parent;
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

   wxCheckBox	*m_checkbox_enablewallpaper;
   wxCheckBox	*m_checkbox_fullwindowdrag;
   wxCheckBox	*m_checkbox_animation;
   wxCheckBox	*m_checkbox_themes;
   wxCheckBox	*m_checkbox_bitmapcaching;

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

class RDPOptionsDialog_Advanced : public wxPanel
{

public:
   RDPOptionsDialog_Advanced(RDPOptionsDialog *parent_element);
   virtual ~RDPOptionsDialog_Advanced();
   int Build();
   Options_HashMap Get_Options();
   bool Set_Options(const Options_HashMap *all_options);
   wxString Get_Options_String();
   
   
private:
   RDPOptionsDialog *parent;
   int error;

   wxBoxSizer *m_boxsizer_1;
   wxBoxSizer *m_boxsizer_2;
   wxBoxSizer *m_boxsizer_3;
   wxBoxSizer *m_boxsizer_4;
   wxGridSizer *m_gridsizer_1;

   wxStaticText *m_static_rdpversion;
   wxComboBox	*m_choise_rdpversion;
   wxCheckBox	*m_checkbox_french;
   wxCheckBox	*m_checkbox_encryption_enable;
   wxCheckBox	*m_checkbox_backingstore;
   wxCheckBox	*m_checkbox_usemouse;
   wxCheckBox	*m_checkbox_privatecolormap;
   wxCheckBox	*m_checkbox_numlock;
   wxCheckBox	*m_checkbox_enablecompress;

   wxStaticText *m_static_keyboard_map;
   wxComboBox *m_combobox_keyboard_map;

};



class RDPOptionsDialog : public wxNotebook
{
public:

   RDPOptionsDialog(wxWindow *parent_element);
   ~RDPOptionsDialog();
   int Build();
   bool Set_Options(const Options_HashMap *options);
   void Set_Group_List(wxArrayString group_list);
   Options_HashMap Get_Options();
private:
   wxWindow *parent;
   int error;
   RDPOptionsDialog_General *rdp_general;
   RDPOptionsDialog_Display *rdp_display;
   RDPOptionsDialog_Share *rdp_share;
   RDPOptionsDialog_Program *rdp_program;
   RDPOptionsDialog_Performance *rdp_perfomance;
   RDPOptionsDialog_Advanced *rdp_advanced;
   Options_HashMap save_options;
   wxArrayString array_string_group_list;
   

};





#endif //RDPOPTIONSDIALOG
