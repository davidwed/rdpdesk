///////////////////////////////////////////////////////////////////////////////
// File name:   fastconn_dialog.hpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-11-24 17:46:21"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef __FASTCONNDIALOG__
#define __FASTCONNDIALOG__

#include <memory>

//#include "rdp_dialogs.hpp"
//#include "defineds.hpp"
//class RDPTree;

#include "tree_group.hpp"

class MainFrame;

class FastConnDialog : public wxDialog {
public:
   FastConnDialog(MainFrame   *parent,
	               int         dialogType,
	               RDPConn    *rdpc,
                 wxTextFile *frameTextFile,
                 base_conn   rdpBaseMain);

	~FastConnDialog();

	const Options_HashMap &GetOptions() const;
	void SetOptions(const Options_HashMap &options);

private:
	wxTextFile *BaseFile;
	wxArrayString servers;
	MainFrame *mainFrame;
	/* const */ base_conn rdpBase;

	void LoadFromBase();

	//RDPConn rdpconn;
	Options_HashMap options;
	//wxWindow *parent;

	std::auto_ptr<wxButton> btnCancel;
	std::auto_ptr<wxButton> btnConnect;
	std::auto_ptr<wxButton> btnDetails;

	std::auto_ptr<wxStaticText> staticTxtServer;
	std::auto_ptr<wxComboBox>   comboboxServer;

	wxBoxSizer * sizer_server;
	wxBoxSizer * sizer_btn;
	wxBoxSizer * sizer_top;

	wxStaticText * m_static_protocol;
	wxComboBox   * m_combobox_protocol;
	wxStaticLine * lineButtons;

	//RDPConn GetRDPConn() {return rdpconn;}
	//void SetRDPConn(RDPConn rdpc) {rdpconn = rdpc;}

protected:

	Options_HashMap SetDefaultRDPConn(const wxString &server);
	void OnButtonConnect(wxCommandEvent &event);
	void OnButtonCancel(wxCommandEvent &event);
	void OnButtonDetails(wxCommandEvent &event);
	void OnComboboxChange(wxCommandEvent &event);
	void OnComboboxText(wxCommandEvent &event);
	void dialog_hotkeys(wxKeyEvent &event);

	DECLARE_EVENT_TABLE()
};

inline const Options_HashMap &FastConnDialog::GetOptions() const {
	return options;
}

inline void FastConnDialog::SetOptions(const Options_HashMap &newOptions) {
	options = newOptions; // TODO: rewrite
}

enum {
	ID_FASTCONN_CANCEL   = 61,
	ID_FASTCONN_CONNECT  = 62,
	ID_FASTCONN_DETAILS  = 63,
	ID_COMBOBOX_FAST     = 64,
	ID_COMBOBOX_PROTOCOL = 65,
};

enum FASTCONN_RET_CODE {
	FASTCONN_RET_CANCEL  = 0,
	FASTCONN_RET_CONNECT = 1,
};

#endif

