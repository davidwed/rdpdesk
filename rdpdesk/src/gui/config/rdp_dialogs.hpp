///////////////////////////////////////////////////////////////////////////////
// File name:   rdp_dialogs.hpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-21 20:41:20" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#ifndef RDPDIALOGS
#define RDPDIALOGS


#include <wx/wx.h>
#include <wx/hyperlink.h>
#include <wx/display.h>
/*
#include "wx/bookctrl.h"
#include "wx/propdlg.h"
#include "wx/progdlg.h"
#include "wx/datetime.h"
#include "wx/image.h"
#include "wx/bookctrl.h"
#include "wx/artprov.h"
#include "wx/imaglist.h"
#include "wx/sysopt.h"
#include "wx/tooltip.h"
#include "wx/imaglist.h"
#include "wx/artprov.h"


#include "wx/colordlg.h"
#include "wx/choicdlg.h"
#include "wx/tipdlg.h"
#include "wx/datetime.h"     
#include "wx/aboutdlg.h"
#include "wx/statline.h"
#include "wx/generic/aboutdlgg.h"
#include "wx/busyinfo.h"
#include "wx/numdlg.h"

#include "wx/filedlg.h"
#include "wx/combobox.h"

#include "wx/textfile.h"

#include "wx/splitter.h"

#include "wx/dirdlg.h"

#include "wx/spinbutt.h"
#include "wx/tglbtn.h"


#include "wx/fontdlg.h"
#include "wx/fdrepdlg.h"
#include "wx/spinctrl.h"
#include "wx/propdlg.h"
#include "wx/generic/colrdlgg.h"
#include "wx/generic/dirdlgg.h"
#include "wx/generic/filedlgg.h"
#include "wx/generic/fontdlgg.h"
#include "wx/choice.h"
#include "wx/dir.h"
#include <wx/display.h>
#include <wx/hyperlink.h>
*/
//#include "rdpbasedefs.hpp"
//#include "defineds.hpp"
#include "fastconn_dialog.hpp"
#include "bencoded.hpp"

#ifdef __WXMSW__
#include <cryptuiapi.h>
// Undocumented CryptUI functions
typedef struct {
	DWORD dwSize;
	HWND hwndParent;
	DWORD dwFlags;
	LPCSTR pszTitle;
	DWORD dwDontUseColumn;
	LPCSTR pszDisplayString;
	LPVOID pFilterCallback;
	LPVOID pDisplayCallback;
	LPVOID pvCallbackData;
	UINT cDisplayStores;
	HCERTSTORE *rghDisplayStores;
	UINT cStores;
	HCERTSTORE *rghStores;
	UINT cPropSheetPages;
	LPVOID *rghPropSheetPages;
	HCERTSTORE hSelectedCertStore;
} CRYPTUI_SELECTCERTIFICATE_STRUCTA, *LPCRYPTUI_SELECTCERTIFICATE_STRUCTA;

typedef CRYPTUI_VIEWCERTIFICATE_STRUCTA *LPCRYPTUI_VIEWCERTIFICATE_STRUCTA;
typedef PCCERT_CONTEXT (WINAPI *LPCryptUIDlgSelectCertificateA)(LPCRYPTUI_SELECTCERTIFICATE_STRUCTA);
typedef BOOL (WINAPI *LPCryptUIDlgViewCertificateA)(LPCRYPTUI_VIEWCERTIFICATE_STRUCTA, LPBOOL);

PCCERT_CONTEXT WINAPI CryptUIDlgSelectCertificateA(LPCRYPTUI_SELECTCERTIFICATE_STRUCTA);

#endif


// class RDPGeneral : public wxPanel
// {
// 	friend class RDPDialog;
// public:
// 	RDPGeneral(wxNotebook *frame, const wxPoint& point, const wxSize& size, base_conn * rdp_base_main);
//     virtual ~RDPGeneral();

// private:

// 	wxStaticText * m_static_server;
// 	wxTextCtrl * m_text_server;

// 	wxStaticText * m_static_port;
// 	wxTextCtrl * m_text_port;

// 	wxStaticText * m_static_username;
// 	wxTextCtrl * m_text_username;

// 	wxStaticText * m_static_password;
// 	wxTextCtrl * m_text_password;

// 	wxStaticText * m_static_domain;
// 	wxTextCtrl * m_text_domain;

// 	wxCheckBox	*m_checkbox_autologon;
// 	wxCheckBox	*m_checkbox_customport;
// 	wxCheckBox	*m_checkbox_attachtoconsole;


// 	wxStaticText * m_static_groupname;
// 	wxComboBox * m_combobox_groupname;

// 	wxStaticText * m_static_connectionname;
// 	wxTextCtrl * m_text_connectionname;

// 	int groupcount;
// 	wxString * string_group_list;
// 	wxArrayString array_string_group_list;



// 	void get_group_list();
// 	base_conn * rdp_base;

// 	void checkbox_customport_func(wxCommandEvent &event);
// 	void text_passwordentry_func(wxCommandEvent &event);

// 	DECLARE_EVENT_TABLE()
// };




// class RDPDisplay : public wxPanel
// {
// 	friend class RDPDialog;
// public:
//    RDPDisplay(wxNotebook *frame, const wxPoint& point, const wxSize& size);
//    virtual ~RDPDisplay();
//  private:  

// 	wxStaticText * m_static_width;
// 	wxTextCtrl * m_text_width;

// 	wxStaticText * m_static_height;
// 	wxTextCtrl * m_text_height;

// 	wxStaticText * m_static_colordepth;
// 	wxComboBox * m_choise_colordepth;

// 	wxStaticText * m_static_resolution;
// 	wxComboBox * m_combobox_resolution;
// 	wxCheckBox	*m_checkbox_customgeometry;

// 	wxCheckBox	*m_checkbox_fullscreen;
// 	wxCheckBox	*m_checkbox_smartsizing;
// 	wxCheckBox	*m_checkbox_controlsize;

// 	wxCheckBox	*m_checkbox_updatescreen;

// 	wxFlexGridSizer * sizer_main;
// 	wxBoxSizer * s1;
// 	wxBoxSizer * s2;
// 	wxBoxSizer * sizer_top;

// 	wxArrayString GetResolutionsList();
// 	wxSize GetResolution(int i);
// 	int GetDefaultResolution();
// 	void checkbox_customgeometry_func(wxCommandEvent& event);
	

// 	void checkbox_fullscreen_func(wxCommandEvent &event);
// 	void checkbox_controlsize_func(wxCommandEvent &event);

// 	void CheckRDPDisplay();

// 	DECLARE_EVENT_TABLE()
// };

// const int ID_CHK_FULLSCREEN = 30;
// const int ID_CHK_CONTROLSIZE = 31;
// const int ID_CHK_CUSTOMGEOMETRY = 50;


// class RDPShare : public wxPanel
// {
// 	friend class RDPDialog;
// public:
// 	RDPShare(wxNotebook *frame, const wxPoint& point, const wxSize& size);
//     virtual ~RDPShare();
	
// private:
	
// 	wxStaticText * m_static_sound;
// 	wxComboBox * m_choise_sound;

// 	wxStaticText * m_static_keyboard;
// 	wxComboBox * m_choise_keyboard;

// 	wxCheckBox	*m_checkbox_drives;
// 	wxCheckBox	*m_checkbox_printers;
// 	wxCheckBox	*m_checkbox_comports;
// 	wxCheckBox	*m_checkbox_smartcards;

// 	wxFlexGridSizer * sizer_main;
// 	wxBoxSizer * sizer_top;
// 	wxBoxSizer * sizer_devices;

// 	wxStaticText * m_static_linux_devices;
// 	wxTextCtrl * m_text_linux_devices;
// };

// class RDPProgram : public wxPanel
// {
// 	friend class RDPDialog;
// public:
// 	RDPProgram(wxNotebook *frame, const wxPoint& point, const wxSize& size);
//     virtual ~RDPProgram();
    
// private:

// 	wxStaticText * m_static_program;
// 	wxTextCtrl * m_text_program;

// 	wxStaticText * m_static_workdir;
// 	wxTextCtrl * m_text_workdir;

// 	wxCheckBox	*m_checkbox_program;
// 	wxCheckBox	*m_checkbox_maximized;

// 	wxBoxSizer * sizer_main;
// 	wxBoxSizer * sizer_top;

// 	void checkbox_program_func(wxCommandEvent &event);

// 	DECLARE_EVENT_TABLE()

// };

// const int ID_CHECKBOX_USE_PROGRAM = 22;



// class RDPPerformance : public wxPanel
// {
// 	friend class RDPDialog;
// public:
// 	RDPPerformance(wxNotebook *frame, const wxPoint& point, const wxSize& size);
//     virtual ~RDPPerformance();

// private:
    	
// 	wxStaticText * m_static_speed;
// 	wxComboBox * m_choise_speed;

// 	wxStaticText * m_static_performance;

// 	wxCheckBox	*m_checkbox_enablewallpaper;
// 	wxCheckBox	*m_checkbox_fullwindowdrag;
// 	wxCheckBox	*m_checkbox_animation;
// 	wxCheckBox	*m_checkbox_themes;
// 	wxCheckBox	*m_checkbox_bitmapcaching;

// 	wxString ChoisesSpeed[5];

// 	wxBoxSizer * sizer_speed;
// 	wxBoxSizer * sizer_static;
// 	wxFlexGridSizer * sizer_perf;
// 	wxBoxSizer * sizer_window;
// 	wxBoxSizer * sizer_top;

// 	void choise_speed_func(wxCommandEvent &event);

// 	DECLARE_EVENT_TABLE()

// };

// const int ID_COMBOBOX_SPEED = 24;





// class RDPAdvanced : public wxPanel
// {
// 	friend class RDPDialog;
// public:
// 	RDPAdvanced(wxNotebook *frame, const wxPoint& point, const wxSize& size);
//     virtual ~RDPAdvanced();

// private:
	

// 	wxStaticText * m_static_rdpversion;
// 	wxComboBox	*m_choise_rdpversion; 
// 	wxCheckBox	*m_checkbox_french;  
// 	wxCheckBox	*m_checkbox_encryption_enable;	
// 	wxCheckBox	*m_checkbox_backingstore;
// 	wxCheckBox	*m_checkbox_usemouse;
// 	wxCheckBox	*m_checkbox_privatecolormap; 
// 	wxCheckBox	*m_checkbox_numlock;
// 	wxCheckBox	*m_checkbox_enablecompress;
	
// 	wxStaticText *m_static_keyboard_map;
// 	wxComboBox *m_combobox_keyboard_map;

// 	wxBoxSizer * sizer_version;
// 	wxBoxSizer * sizer_crypt;
// 	wxFlexGridSizer * sizer_settings;
// 	wxBoxSizer * sizer_keymap;
// 	wxBoxSizer * sizer_top;
	
	
// };

// class ICAAdvanced : public wxPanel
// {
// 	enum
// 	{
// 		ID_CHECKBOX_ICA_ENCRYPT = 2010,
// 		ID_CHECKBOX_ICA_SOUND = 2011,
// 		ID_BUTTON_ICA_FILE = 2012,
// 		ID_BUTTON_INI_CLIENT = 2013,
// 		ID_BUTTON_INI_SERVER = 2014,
// 		ID_CHECKBOX_ICA_APPLICATION = 2015
// 		//ID_BUTTON_ICA_CERT = 2013,
// 		//ID_RADIOBOX_PROXY = 2014
// 	};
// 	friend class RDPDialog;
// public:
// 	ICAAdvanced(wxNotebook *frame, const wxPoint& point, const wxSize& size);
//     virtual ~ICAAdvanced();

// private:
	
// 	wxCheckBox * m_checkbox_ica_application;
// 	wxTextCtrl * m_text_ica_application;

// 	wxCheckBox * m_checkbox_ica_encryption;
// 	wxStaticText * m_static_ica_encryption;
// 	wxComboBox * m_combobox_ica_encryption;
	
// 	wxStaticText * m_static_client_ini;
// 	wxTextCtrl * m_text_client_ini;
// 	wxButton * m_button_client_ini;
	
// 	wxStaticText * m_static_server_ini;
// 	wxTextCtrl * m_text_server_ini;
// 	wxButton * m_button_server_ini;

// 	wxStaticText * m_static_file_ica;
// 	wxTextCtrl * m_text_file_ica;
// 	wxButton * m_button_file_ica;
// //	wxBoxSizer * sizer_file_ica;
// /*
// 	wxBoxSizer * sizer_radio_addr;
// 	wxBoxSizer * sizer_note_cert;
// 	wxBoxSizer * sizer_radio_addr2;

// 	wxStaticBox * m_staticbox_proxy;
// 	wxRadioBox * m_radiobox_proxy;
// 	wxStaticText * m_static_proxyhost;
// 	wxTextCtrl * m_text_proxyhost;
// 	wxStaticText * m_static_proxyport;
// 	wxTextCtrl * m_text_proxyport;
// 	wxBoxSizer * sizer_proxyaddr;

// 	wxStaticText * m_static_note;
// 	wxStaticText * m_static_username;
// 	wxTextCtrl * m_text_username;
// 	wxButton * m_button_cert;
// 	wxStaticText * m_static_password;
// 	wxTextCtrl * m_text_password;

// 	wxFlexGridSizer * sizer_creds;

// 	wxStaticBoxSizer * sizer_proxy;

// 	wxStaticBitmap * m_bitmap_proxy;
// 	wxBoxSizer * sizer_low;
// */
// 	wxCheckBox * m_checkbox_ica_sound;
// 	wxComboBox * m_combobox_ica_sound;
// 	//wxBoxSizer * sizer_client_ini;
// 	//wxBoxSizer * sizer_server_ini;
// 	wxFlexGridSizer * sizer_ini_files;
// 	wxBoxSizer * sizer_ica_encryption;
// 	wxBoxSizer * sizer_ica_sound;
// 	wxBoxSizer * sizer_ica_application;
// 	wxBoxSizer * sizer_main;
	
	
// 	void on_checkbox_ica_encryption(wxCommandEvent & event);
// 	void on_checkbox_ica_sound(wxCommandEvent & event);
// 	void on_checkbox_ica_application(wxCommandEvent & event);
// 	void on_button_ica_file(wxCommandEvent & event);
// 	void on_button_ini_server(wxCommandEvent & event);
// 	void on_button_ini_client(wxCommandEvent & event);
// //	void on_button_ica_cert(wxCommandEvent & event);
// //	void on_radiobox_proxy(wxCommandEvent & event);
// //	void CheckProxySettings();


// 	DECLARE_EVENT_TABLE()
// };

// class ProxySettings : public wxPanel
// {
// 	enum
// 	{
// 		ID_RADIOBUTTON_NOPROXY = 2010,
// 		ID_RADIOBUTTON_SOCKS = 2011,
// 		ID_RADIOBUTTON_SSL = 2012,
// 		ID_HYPERLINK_CERT = 2013
// 	};
// 	friend class RDPDialog;

// public:
// 	ProxySettings(wxNotebook *frame, const wxPoint& point, const wxSize& size);
//     virtual ~ProxySettings();

// private:
// 	wxStaticText * m_static_proxytype;
// 	wxRadioButton * m_radio_noproxy;
// 	wxRadioButton * m_radio_socks;
// 	wxRadioButton * m_radio_ssl;
// 	//wxButton * m_button_cert;
// 	wxHyperlinkCtrl * m_hyperlink_cert;
// 	wxBoxSizer * sizer_proxy;

// 	wxStaticText * m_static_proxyhost;
// 	wxTextCtrl * m_text_proxyhost;
// 	wxStaticText * m_static_proxyport;
// 	wxTextCtrl * m_text_proxyport;
// 	wxBoxSizer * sizer_proxyaddr;



// 	wxStaticText * m_static_username;
// 	wxTextCtrl * m_text_username;
// 	wxStaticText * m_static_password;
// 	wxTextCtrl * m_text_password;
// 	wxFlexGridSizer * sizer_creds;
	
// 	wxStaticLine * m_line_options;
// 	wxBoxSizer * sizer_options;
// 	wxBoxSizer * sizer_main;

// 	void on_radiobutton_noproxy(wxCommandEvent& event);
// 	void on_radiobutton_socks(wxCommandEvent& event);
// 	void on_radiobutton_ssl(wxCommandEvent& event);
// 	void on_hyperlink_cert(wxHyperlinkEvent& event);
// 	void CheckProxySettings(bool set_ports = true);

// 	DECLARE_EVENT_TABLE()
// };



class RDPDialog : public wxDialog
{
public:
   // RDPDialog(wxWindow* parent, int dialogType, RDPConn * rdpc, wxTextFile * frameTextFile,
   // 	     base_conn  * rdp_base_main , BOOL not_change = TRUE);

   RDPDialog(wxWindow *parent);

   wxNotebook * notebook;
   wxImageList* m_imageList;

   // RDPGeneral * m_rdpgeneral;
   // RDPDisplay * m_rdpdisplay;
   // RDPShare * m_rdpshare;
   // RDPProgram * m_rdpprogram;
   // RDPPerformance * m_rdpperformance;

   // RDPAdvanced * m_rdpadvanced;
   // ICAAdvanced * m_icaadvanced;
   // ProxySettings * m_proxysettings;

   wxButton * m_button_ok;
   wxButton * m_button_cancel;
   wxButton * m_button_save;

   wxStaticText * m_static_protocol;
   wxComboBox * m_combobox_protocol;
   wxStaticLine * m_line_buttons;
   wxStaticLine * m_line_protocol;

   wxBoxSizer * sizer_protocol;

   void button_ok_func(wxCommandEvent &event);
   void button_cancel_func(wxCommandEvent &event);
   void button_save_func(wxCommandEvent &event);

   void dialog_hotkeys(wxKeyEvent &event);

   void change_proto_func(wxCommandEvent &event);
   void change_proto();
   //RDPConn * prdpconn;
   wxTextFile * BaseFile;

   BOOL new_uniq_name;
   base_conn * rdp_base;

   Main_Frame * main_frame;
	
   wxBoxSizer * sizer_main;
   wxBoxSizer * sizer_btn;

   ~RDPDialog();

   //void LoadRDPConn();
   void Set_Options(const Options_HashMap *options);
   //void Set_Options(const Options_HashMap *options, const RDPOptionsDialog *options_dialog);
   //void Set_Options(const Options_HashMap *options, const ICAOptionsDialog *options_dialog );
   Options_HashMap Get_Options();
   //BOOL FillRDPConn();
   DECLARE_EVENT_TABLE();
   
private:
   RDPOptionsDialog *rdp_options_dialog;
   ICAOptionsDialog *ica_options_dialog;
   Options_HashMap local_options;
   wxArrayString group_list;
};


const int ID_BUTTON_OK = 20;
const int ID_BUTTON_CANCEL = 21;
const int ID_BUTTON_SAVE = 22;
const int ID_COMBOBOX_PROTO_CHANGE = 23;
const int ID_CHECKBOX_CUSTOMPORT = 24;
const int ID_TEXTCTRL_PASSWORD = 25;
const int ID_CHECKBOX_AUTOLOGON = 26;

#endif
