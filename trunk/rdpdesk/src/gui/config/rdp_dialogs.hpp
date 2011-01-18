///////////////////////////////////////////////////////////////////////////////
// File name:   rdp_dialogs.hpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef RDPDIALOGS
#define RDPDIALOGS


#include <wx/wx.h>
#include <wx/hyperlink.h>
#include <wx/display.h>
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




class RDPDialog : public wxDialog
{
public:
   RDPDialog(MainFrame *parent);

   wxNotebook  * notebook;
   wxImageList * m_imageList;


   wxButton * m_button_ok;
   wxButton * m_button_cancel;
   wxButton * m_button_save;

   wxStaticText * m_static_protocol;
   wxComboBox   * m_combobox_protocol;
   wxStaticLine * m_line_buttons;
   wxStaticLine * m_line_protocol;

   wxBoxSizer * sizer_protocol;

   void OnButtonCancel(wxCommandEvent &event);
   void OnButtonSave(wxCommandEvent &event);

   void dialog_hotkeys(wxKeyEvent &event);

   void OnProtoChange(wxCommandEvent &event);
   void ChangeProto();
   //RDPConn * prdpconn;
   wxTextFile * BaseFile;

   BOOL_L new_uniq_name;
   base_conn * rdp_base;

   MainFrame * main_frame;

   wxBoxSizer * sizer_main;
   wxBoxSizer * sizer_btn;

   ~RDPDialog();

   void SetOptions(const Options_HashMap *options);
   Options_HashMap GetOptions() const;
   DECLARE_EVENT_TABLE();

private:
   RDPOptionsDialog	*rdpOptionsDialog;
   ICAOptionsDialog	*icaOptionsDialog;
   RFBOptionsDialog	*rfbOptionsDialog;
#ifdef __WXGTK__
   FRDPOptionsDialog	*frdpOptionsDialog;
#endif
   Options_HashMap	 local_options;
   wxArrayString	 group_list;
};

enum {
	ID_BUTTON_OK             = 20,
	ID_BUTTON_CANCEL         = 21,
	ID_BUTTON_SAVE           = 22,
	ID_COMBOBOX_PROTO_CHANGE = 23,
	ID_CHECKBOX_CUSTOMPORT   = 24,
	ID_TEXTCTRL_PASSWORD     = 25,
	ID_CHECKBOX_AUTOLOGON    = 26,
};

enum RDPDIALOG_RET_CODE {
	RDPDLG_RET_CANCEL = 0,
	RDPDLG_RET_OK     = 1,
	RDPDLG_RET_SAVE   = 2,
};

#endif
