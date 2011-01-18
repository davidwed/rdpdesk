///////////////////////////////////////////////////////////////////////////////
// File name:   BasicConnection.cpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////


#define BACKAND
#include "BasicConnection.hpp"

#include "main_window.hpp"
#include "tree_group.hpp"

BasicConnection::BasicConnection(MainFrame * main,
                                 const Options_HashMap &all_options,
                                 wxWindow * parent,
                                 wxWindowID id,
                                 const wxPoint& pos,
                                 const wxSize& size,
                                 long style,
                                 const wxString& name):
	WindowConnectionClass(parent, id, pos, size, style, name),
	cnt(NULL),
	main_frame(main),
	bFullScreen(FALSE),
	bWaitFlag(FALSE),
	bObjectOk(FALSE),
	Info(wxEmptyString),
	info_uniq_name(0),
	uniq_conn(0),
	options(all_options)
{
	SetUniqConn();
}

BasicConnection::~BasicConnection() {
	if (cnt) {
		cnt->Destroy();
		cnt = NULL;
	}
}
void BasicConnection::photo() {
	wxClientDC client_dc(this);
	wxBitmap bmp(client_dc.GetSize().GetWidth(), client_dc.GetSize().GetHeight());
	wxMemoryDC mdc(bmp);
	mdc.Blit(0, 0, bmp.GetWidth(), bmp.GetHeight(), &client_dc, 0, 0);
	mdc.SelectObject(wxNullBitmap);

	std::auto_ptr<wxFileDialog> savefiledialog(
		new wxFileDialog(this, _("Save screenshot"), wxT(""), wxT(""),
		                 wxT("*.bmp"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT));
	const int iRes = savefiledialog->ShowModal();
	if (iRes == wxID_CANCEL)
		return;
	const wxString filepath(savefiledialog->GetPath());
	if (!filepath.IsEmpty())
		bmp.SaveFile(filepath, wxBITMAP_TYPE_BMP);
}

void BasicConnection::SetUniqConn() {
	if (!main_frame) return;
	Benc bc;
	bool ok = false;
	while (!ok) {
		ok = true;
		int temp = bc.generate_uniq_name(&main_frame->base);
		const size_t conn_num = main_frame->nb->GetPageCount();
		for (size_t i = 0; i < conn_num; ++i) {
			ConnSplitter * splitter = (ConnSplitter *)main_frame->nb->GetPage(i);
			if (!splitter) continue;
			BasicConnection * basic_conn = (BasicConnection *)splitter->conn;
			if (!basic_conn) continue;
			if (temp == basic_conn->uniq_conn) ok = false;
		}
		if (ok) {
			uniq_conn  = temp;
			break;
		}
	}
}


void BasicConnection::screenshot(wxWindow * win)
{
}

void BasicConnection::SendConnectEvent() {
	if (info_uniq_name != 0) {
	   TREEPANEL(main_frame->m_panel_tree)->rdptree->from_wxrdp(info_uniq_name,TREEDATA_INC_CONNCOUNT);
	}
}

void BasicConnection::SendDisconnectEvent() {
//	wxMessageBox(wxT("SendDisconnectEvent"));
	if (info_uniq_name != 0) {
		TREEPANEL(main_frame->m_panel_tree)->rdptree->from_wxrdp(info_uniq_name,TREEDATA_DEC_CONNCOUNT);
		wxCommandEvent eventCustom(ID_FULLSCREEN);
		wxPostEvent(main_frame, eventCustom);
	}
}

void BasicConnection::SendEnterFullscreenEvent() {
	CheckOptions();
}

void BasicConnection::SendLeaveFullscreenEvent() {
	CheckOptions();
}

void BasicConnection::CheckOptions() {
	if ((ConnSplitter *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->conn_splitter)
		this->main_frame->CheckCurrentConnectionMenu();
}

Options_HashMap BasicConnection::Get_Options() {
	return options;
}

FullScreenToolBar::FullScreenToolBar(MainFrame * main,
                                     const wxString &caption,
                                     wxWindow * parent,
                                     wxWindowID id,
                                     const wxPoint& point,
                                     const wxSize& size):
	wxAuiToolBar(parent,id,point,size, wxAUI_TB_DEFAULT_STYLE),
	main_frame(main)
{
	init(caption, parent, id, point, size);
}

FullScreenToolBar::FullScreenToolBar(const wxString &caption,
                                     wxWindow * parent,
                                     wxWindowID id,
                                     const wxPoint& point,
                                     const wxSize& size):
	wxAuiToolBar(parent,id,point,size, wxAUI_TB_DEFAULT_STYLE),
	main_frame(NULL)
{
	init (caption, parent, id, point, size);
}

void FullScreenToolBar::init(const wxString &caption,
                             wxWindow * parent,
                             wxWindowID id,
                             const wxPoint& point,
                             const wxSize& size)
{
	SetToolBitmapSize(wxSize(8, 8));
	m_caption = new wxStaticText(this, wxID_ANY, caption,
	                             wxDefaultPosition, wxDefaultSize,
	                             wxALIGN_CENTRE);

	AddTool(ID_FULLSCREEN_AUTOHIDE,
			wxGetBitmapFromMemory(eject_16x16_png),
			NULL,
			true,
			NULL,
			_("Auto hide"));
	AddControl(m_caption,wxT("asdasd"));
	AddSeparator();
	AddTool(ID_FULLSCREEN_PHOTO,
			_("Make screenshot"),
			wxGetBitmapFromMemory(make_bmp_screenshot_icon_16x16_png),
			_("Make screnshot"));
	AddTool(ID_FULLSCREEN_GRABINPUT,
			wxGetBitmapFromMemory(keyboard_input_icon_16x16_png),
			NULL,
			true,
			NULL,
			_("Grab Input"));
	AddTool(ID_FULLSCREEN_SENDCAD,
			_("Send Ctrl+Alt+Del"),
			wxGetBitmapFromMemory(control_alt_del_icon_16x16_png),
			_("Send Ctrl+Alt+Del"));
	AddTool(ID_FULLSCREEN_LEAVE,
			_("Leave full screen"),
			wxGetBitmapFromMemory(window_nofullscreen_icon_16x16_png),
			_("Leave full screen"));
	AddTool(ID_FULLSCREEN_CLOSE,
			_("Disconnect"),
			wxGetBitmapFromMemory(close_session_icon_16x16_png),
			_("Disconnect"));
	SetPosition(wxPoint(0,0));
	CenterOnParent(wxHORIZONTAL);

	Realize();
	ToggleTool(ID_FULLSCREEN_AUTOHIDE, true);
	EstablishedConnectionsMenu = new wxMenu;
	EstablishedConnectionsMenu->Append(wxID_ANY, caption);

	ConnectionsMenu = new wxMenu;

	if (main_frame != NULL) {
		RDPTree * rdptree = TREEPANEL(main_frame->m_panel_tree)->rdptree;
		wxTreeItemIdValue cookie;
		wxTreeItemId curr = rdptree->GetFirstChild(rdptree->root,cookie);
		int count = 0;
		while (curr.IsOk()) {
			wxMenu * groupmenu = new wxMenu();
			wxTreeItemIdValue cookie_child;
			wxTreeItemId curr_child = rdptree->GetFirstChild(curr,cookie_child);
			while (curr_child.IsOk()) {
				groupmenu->Append(ID_TREE_ADDTOEXISTSGROUP + count, rdptree->GetItemText(curr_child));
				++count;
				curr_child = rdptree->GetNextChild(curr,cookie_child);
			}
			ConnectionsMenu->AppendSubMenu(groupmenu,rdptree->GetItemText(curr));
			curr = rdptree->GetNextChild(rdptree->root,cookie);
		}
	}
}
FullScreenToolBar::~FullScreenToolBar() {
	delete m_caption;
	delete EstablishedConnectionsMenu;
	delete ConnectionsMenu;

	EstablishedConnectionsMenu = NULL;
	ConnectionsMenu = NULL;
}

void FullScreenToolBar::ProcessEvent (wxCommandEvent& event) {
	switch(event.GetId())
	{
		case ID_FULLSCREEN_ADD_TO_MENU:
			std::cout << "EVENT ADD TO MENU" << std::endl;
		case ID_FULLSCREEN_DEL_TO_MENU:
			std::cout << "EVENT ADD TO MENU" << std::endl;
		default:
			//std::cout << "-----------------NULL" << std::endl;
			event.Skip();
			break;
	}
}


void ToolbarCnt::on_enter(wxMouseEvent& event) {
	SetPosition(wxPoint(0,0));
	CenterOnParent(wxHORIZONTAL);
}

void ToolbarCnt::on_leave_func(wxMouseEvent& event) {
	on_leave();
}

void ToolbarCnt::on_leave() {
	if (fs->GetToolToggled(ID_FULLSCREEN_AUTOHIDE)) {
		const wxSize size(GetClientSize());
		SetPosition(wxPoint(0, (-1) * size.y + 2));
		CenterOnParent(wxHORIZONTAL);
	}
}

ToolbarCnt::ToolbarCnt(wxWindow * parent):
	wxWindow(parent,-1,wxPoint(0,0),wxSize(200,22))
{
}
