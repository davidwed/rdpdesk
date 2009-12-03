#ifndef RDPSETDIALOGS
#define RDPSETDIALOGS

#include "tree_group.hpp"
#include "locale_helper.hpp"

#ifdef __WXMSW__
#include "wxRDP_win.hpp"
#endif

#ifdef __WXGTK__
#include "wxRDP_nix.hpp"
#endif

class SettingsDialog : public wxDialog
{
public:
	SettingsDialog(Main_Frame * main, wxWindow* parent, int dialogType);
	~SettingsDialog();

	wxString file_path;
	wxString settings_path;

	programsettings progsettings;
	void init_programsettings(programsettings * ps);
	programsettings fill_settings();
	void save_to_file();
	
private:

	// rdp_base path
	wxStaticText * m_static_rdpbase;
	wxTextCtrl * m_text_rdpbase;
	wxButton * m_button_rdpbase;

	// rdp_key path

	wxStaticText * m_static_rdpkey;
	wxTextCtrl * m_text_rdpkey;
	wxButton * m_button_rdpkey;

	// rdesktop path (only linux)

	wxStaticText * m_static_rdesktop;
	wxTextCtrl * m_text_rdesktop;
	wxButton * m_button_rdesktop;

	// keyboardmap path (only linux)
	wxStaticText * m_static_keymappath;
	wxTextCtrl * m_text_keymappath;
	wxButton * m_button_keymappath;

	wxStaticLine * m_line_separator;
	wxStaticLine * m_line_button;

	wxCheckBox	*m_checkbox_icontray; 
	wxCheckBox	*m_checkbox_showform;  

	wxBoxSizer	*sizer_usecrypt;
	wxCheckBox	*m_checkbox_usecrypt; 
	wxComboBox	*m_combobox_usecrypt;

	wxCheckBox	*m_checkbox_focuspage; 

	wxCheckBox *m_checkbox_autosave;

	wxStaticText * m_static_fcount;
	wxTextCtrl * m_text_fcount;

	wxStaticText * m_static_fmax;
	wxTextCtrl * m_text_fmax;

	wxStaticText * m_static_lang;
	wxComboBox * m_combobox_lang;

	Main_Frame * main_frame;

	wxButton * m_button_ok;
	wxButton * m_button_cancel;
	wxButton * m_button_apply;

	wxFlexGridSizer * sizer_path;

	wxBoxSizer * sizer_fmax;
	wxBoxSizer * sizer_fcount;
	wxBoxSizer * sizer_lang;
	
	wxFlexGridSizer * sizer_main;

	wxBoxSizer * sizer_btn;

	wxBoxSizer * sizer_top;


	void display_settings();
	void apply_settings();

	void button_ok_func(wxCommandEvent &event);
	void button_cancel_func(wxCommandEvent &event);
	void button_apply_func(wxCommandEvent &event);

	void button_rdpbase_func(wxCommandEvent &event);
	void button_rdpkey_func(wxCommandEvent &event);
	void button_rdesktop_func(wxCommandEvent &event);
	void button_keymap_func(wxCommandEvent &event);

	void checkbox_icon_func(wxCommandEvent &event);
	void checkbox_crypt_func(wxCommandEvent &event);

	void dialog_hotkeys(wxKeyEvent &event);

	DECLARE_EVENT_TABLE()

};

const int ID_BUTTON_RDPBASE = 140;
const int ID_BUTTON_RDPKEY = 139;
const int ID_BUTTON_RDESKTOP = 138;
const int ID_BUTTON_KEYMAP = 141;
const int ID_CHECKBOX_ICON = 142;
const int ID_CHECKBOX_CRYPT = 143;

const int ID_BUTTON_SD_OK = 137;
const int ID_BUTTON_SD_CANCEL = 136;
const int ID_BUTTON_SD_APPLY = 135;




#endif