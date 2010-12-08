#ifndef _RFB_OPTIONS_DIALOG
#define _RFB_OPTIONS_DIALOG

#include "bencoded.hpp"

const int ID_RFB_CHECKBOX_CUSTOMPORT = 300;
const int ID_RFB_TEXTCTRL_PASSWORD = 301;
const int ID_RFB_CHK_CONTROLSIZE = 126;

enum RFB_ERRORS
{
   RFB_ERROR_OK = 0,
   RFB_ERROR_PARENT_IS_NULL,
   RFB_ERROR_CANT_CREATE_NOTEBOOK,
   RFB_ERROR_CANT_CREATE_PANEL,
   RFB_ERROR_EMPTY_HOSTNAME
};

class RFBOptions
{
public:
	RFBOptions();
	~RFBOptions();
};

class RFBOptionsDialog;
class RFBOptionsDialog_Display;

class RFBOptionsDialog_General : public wxPanel
{
public:
	RFBOptionsDialog_General(RFBOptionsDialog *parent_element);
	virtual ~RFBOptionsDialog_General();
	int Build();
	Options_HashMap GetOptions();
	bool SetOptions(const Options_HashMap *all_options);
	void SetGroupList(const wxArrayString &group_list);
private:
	RFBOptionsDialog *parent;
	int error;
	wxFlexGridSizer *m_flexgrid_sizer;
	wxFlexGridSizer *m_flexgrid_sizer_2;
	wxBoxSizer *m_boxsizer;
	wxBoxSizer *m_boxsizer_2;
	wxBoxSizer *m_boxsizer_3;
	wxStaticText *m_static_server;
	wxTextCtrl *m_text_server;
	wxStaticText *m_static_groupname;
	wxComboBox *m_combobox_groupname;

	wxStaticText * m_static_proxy;
	wxTextCtrl * m_text_proxy;
	
	wxStaticText *m_static_proxy_port;
	wxTextCtrl *m_text_proxy_port;

	wxCheckBox *m_checkbox_shared;

	  wxCheckBox *m_checkbox_viewonly;

	wxStaticText *m_static_connectionname;
	wxTextCtrl *m_text_connectionname;

	wxStaticText *m_static_port;
	wxTextCtrl *m_text_port;

	wxCheckBox *m_checkbox_customport;

	wxStaticText * m_static_password;
	wxTextCtrl * m_text_password;


   int groupcount;
   wxString *string_group_list;
   wxArrayString array_string_group_list;

   void get_group_list();
   void checkbox_customport_func(wxCommandEvent &event);
   void checkbox_customport();
	DECLARE_EVENT_TABLE();
};

class RFBOptionsDialog : public wxNotebook
{
public:

   RFBOptionsDialog(wxWindow *parent_element);
   ~RFBOptionsDialog();
   int Build();
   bool SetOptions(const Options_HashMap *options);
   void SetGroupList(const wxArrayString &group_list);
   Options_HashMap GetOptions();
private:
   wxWindow *parent;
   int error;
   RFBOptionsDialog_General *rfb_general;
   RFBOptionsDialog_Display *rfb_display;
   Options_HashMap save_options;
   wxArrayString array_string_group_list;
};

class RFBOptionsDialog_Display: public wxPanel
{
public:
   RFBOptionsDialog_Display(RFBOptionsDialog *parent_element);
   virtual ~RFBOptionsDialog_Display();
   int Build();
   Options_HashMap GetOptions();
   bool SetOptions(const Options_HashMap *all_options);
private:
   RFBOptionsDialog *parent;
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
   void CheckRFBDisplay();
   DECLARE_EVENT_TABLE()
};


#endif
