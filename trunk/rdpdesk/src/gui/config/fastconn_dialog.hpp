///////////////////////////////////////////////////////////////////////////////
// File name:   fastconn_dialog.hpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-10 17:23:20" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#ifndef FASTCONNDIALOG
#define FASTCONNDIALOG


//#include "rdp_dialogs.hpp"
//#include "defineds.hpp"
//class RDPTree;

#include "tree_group.hpp"



class FastConnDialog : public wxDialog
{

   friend class RDPTree;

public:
   FastConnDialog(wxWindow* parent, int dialogType, RDPConn * rdpc, wxTextFile * frameTextFile, base_conn rdp_base_main);
   ~FastConnDialog();

   wxTextFile * BaseFile;
   wxString * string_server_list;
   Main_Frame * main_frame;
   base_conn rdp_base;

private:
   int lineCount;
   //RDPConn ParseString(int iLineNumber, RDPConn rdpconn);
   Options_HashMap ParseString(int iLineNumber, Options_HashMap options);
   
   bool LoadFromBase();

   //RDPConn rdpconn;
   Options_HashMap options;
   //wxWindow *parent;
   

   wxButton * m_button_cancel;
   wxButton * m_button_connect;
   wxButton * m_button_details;

   wxComboBox * m_combobox_server;
   wxStaticText * m_static_server;

   wxBoxSizer * sizer_server;
   wxBoxSizer * sizer_btn;
   wxBoxSizer * sizer_top;


   wxStaticText * m_static_protocol;
   wxComboBox * m_combobox_protocol;
   wxStaticLine * m_line_buttons;

public:
   Options_HashMap Get_Options() 
   {
      return options;
   }

   void Set_Options(Options_HashMap all_options) 
   {
      options = all_options;
   }
   //RDPConn GetRDPConn() {return rdpconn;} 
   //void SetRDPConn(RDPConn rdpc) {rdpconn = rdpc;}

protected:

   Options_HashMap SetDefaultRDPConn(wxString server);
   void button_connect_func(wxCommandEvent &event);
   void button_cancel_func(wxCommandEvent &event);
   void button_details_func(wxCommandEvent &event);
   void combobox_change_func(wxCommandEvent &event);
   void combobox_text_func(wxCommandEvent &event);
   void dialog_hotkeys(wxKeyEvent &event);

   int getlinecount();

   DECLARE_EVENT_TABLE()
};

const int ID_FASTCONN_CANCEL = 61;
const int ID_FASTCONN_CONNECT = 62;
const int ID_FASTCONN_DETAILS = 63;
const int ID_COMBOBOX_FAST = 64;

#endif
