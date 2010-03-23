///////////////////////////////////////////////////////////////////////////////
// File name:   tree_group.hpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-07 23:31:12" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#ifndef TREEGROUP
#define TREEGROUP

//#include "fastconn_dialog.hpp"
/*
#ifdef __WXMSW__
#include "wxRDP_win.hpp"
#endif

#ifdef __WXGTK__
#include "wxRDP_nix.hpp"
#endif
*/

#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/imaglist.h>

#include "main_window.hpp"

const int ID_RDPTREE = 170;
const int ID_FAVORITESTREE = 170;

const int ID_EVENT_FROM_WXRDP = 180;
const int ID_TREE_SETCREDS = 181;

const int ID_GROUP_ENUM = 171;
const int ID_GROUP_BY_HOST = 172;
const int ID_GROUP_BY_CRED = 173;

const int ID_TREE_CONNECT = 174;
const int ID_TREE_CHANGE = 175;
const int ID_TREE_DELETE = 176;
const int ID_TREE_ADDTONEWGROUP = 177;
const int ID_TREE_CLONE = 178;
const int ID_TREE_NULLCOUNT = 179;



const int ID_TREE_ADDTOEXISTSGROUP = 7000;

const int TREEDATA_INC_OBJCOUNT = 10000;
const int TREEDATA_DEC_OBJCOUNT = 10001;
const int TREEDATA_INC_CONNCOUNT = 10002;
const int TREEDATA_DEC_CONNCOUNT = 10003;


class RDPTreeData : public wxTreeItemData
{
public:
    RDPTreeData(DWORD objcount, DWORD conncount, int un) : dwObjCount(objcount),dwConnCount(conncount),uniq_name(un) { }
	RDPTreeData()
	{
		dwObjCount = 0;
		dwConnCount = 0;
		uniq_name = 0;
	}

	DWORD dwObjCount;
	DWORD dwConnCount;
	int uniq_name;
private:
    
};


class RDPTree : public wxTreeCtrl
{
public:

   RDPTree(Main_Frame * main, wxWindow *parent, const wxWindowID id,
	   const wxPoint& pos, const wxSize& size,
	   long style);

   virtual ~RDPTree(){};
		
   wxArrayTreeItemIds items;
   wxTreeItemId root;
   
   wxTreeItemId base;
   void LoadSettings();
   void ReloadSettings();

   Main_Frame * main_frame;
   wxString * string_server_list;
   int lineCount;
   wxString currstring;
   int curr_uniq_name;

   BOOL IsFavorites;

   int fcount;
   int fmax;

   void on_dleft_click(wxMouseEvent& event);
   void on_right_click(wxMouseEvent& event);


   wxTreeItemId curr_drag;
   void on_begin_drag(wxTreeEvent& event);
   void on_end_drag(wxTreeEvent& event);

   void createmenu(wxTreeEvent& event);
	
   void on_event_from_wxrdp(wxCommandEvent& event);
   void from_wxrdp(wxString info, int type);
   void from_wxrdp(int info_uniq_name, int type);
	
   void on_deleting_item(wxTreeEvent& event);
   void on_activated(wxTreeEvent& event);
   void on_key_down(wxTreeEvent& event);
   void on_tree_connect(wxCommandEvent& event);
   void on_tree_delete(wxCommandEvent& event);
   void on_tree_change(wxCommandEvent& event);
   void on_tree_clone(wxCommandEvent& event);
   void on_add_group(wxCommandEvent& event);
   void on_tree_delete_from_favorites(wxCommandEvent& event);
   void on_tree_set_credentials(wxCommandEvent& event);
   void on_kill_focus(wxFocusEvent& event);
private:
   int find_min_favorites(int * un);
   BOOL IsFavoritesFull();

   void sort_group(wxTreeItemId group);
   BOOL IsGroup(wxTreeItemId item);
   BOOL IsRDPConn(wxTreeItemId item);
   wxTreeItemId create_new_group();
   void delete_item(wxTreeItemId id);
   void delete_this_item(wxString item_text);
   wxTreeItemId find_this_item(wxString item_text);
   wxTreeItemId find_item_by_uniq_name(int uniq_name);
   wxTreeItemId find_this_group(wxString group_name);

   void redraw(wxTreeItemId item);
   void move_item(wxTreeItemId item, wxTreeItemId group);

   void get_rdpbase_item(wxTreeItemId item, RDPConn * rdpconn);
   void refresh_rdpbase_item(RDPConn * rdpconn);
	
   BOOL item_already_exists(wxTreeItemId id);
   BOOL group_already_exists(wxString group_name);

   void add_to_favorites(wxTreeItemId id,  Options_HashMap local_options);

   DECLARE_EVENT_TABLE()

};


class TreePanel : public wxPanel
{
public:
    TreePanel(Main_Frame * main, wxSplitterWindow * parent, wxWindowID id,const wxPoint& pt,const wxSize& sz, long style);
		
	Main_Frame * main_frame;
	RDPTree * rdptree;

	RDPTree * favorites;
	
	wxToolBarBase * toolBar;
	wxAuiToolBar * auitoolbar;	
	wxPanel * m_panel_toolbar;
	wxNotebook * tree_notebook;

private:

	void on_panel_closebutton(wxAuiToolBarEvent& event);
	void on_panel_creategroup(wxAuiToolBarEvent& event);
	void on_panel_deletegroup(wxAuiToolBarEvent& event);

	void on_size_change(wxSizeEvent& event);


	void OnToolLeftClick(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
  
};

#define TREEPANEL(ptr) ((TreePanel *)ptr)

const int ID_CREDSDIALOG_OK = 185;
const int ID_CREDSDIALOG_CANCEL = 186;

class CredsDialog : public wxDialog
{
public:
	CredsDialog(wxWindow* parent, int dialogType,const wxString& groupname);
	~CredsDialog();

	wxString group_username;
	wxString group_password;
	wxString group_domain;

private:
	wxStaticText * m_static_username;
	wxTextCtrl * m_text_username;

	wxStaticText * m_static_password;
	wxTextCtrl * m_text_password;

	wxStaticText * m_static_domain;
	wxTextCtrl * m_text_domain;

	wxButton * m_button_ok;
	wxButton * m_button_cancel;

	wxBoxSizer * sizer_btn;
	wxFlexGridSizer * sizer_creds;
	wxBoxSizer * sizer_top;
	
	void button_ok_func(wxCommandEvent &event);
	void button_cancel_func(wxCommandEvent &event);

	void dialog_hotkeys(wxKeyEvent &event);

	DECLARE_EVENT_TABLE()

};


const int ID_PANEL_CLOSEBUTTON = 169;
const int ID_PANEL_CREATEGROUP = 168;
const int ID_PANEL_DELETEGROUP = 167;

const int ID_MENU_ADDTOGROUP = 166;





#endif
