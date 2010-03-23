///////////////////////////////////////////////////////////////////////////////
// File name:   BasicConnection.cpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-19 22:24:54" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#include "BasicConnection.hpp"

#include "main_window.hpp"
#include "tree_group.hpp"



BasicConnection::BasicConnection(Main_Frame * main,Options_HashMap all_options ,wxWindow * parent,
				 wxWindowID id, const wxPoint& pos, const wxSize& size,long style ,
				 const wxString& name):
   WindowConnectionClass(parent,id,pos,size,style,name),
   bObjectOk(FALSE),
   cnt(NULL)
{
   //rdpconn = rdpsettings;
   options = all_options;
   main_frame = main;
   bFullScreen = FALSE;
   bWaitFlag = FALSE;
   Info = wxEmptyString;
   info_uniq_name = 0;
   uniq_conn = 0;
   SetUniqConn();
}


BasicConnection::~BasicConnection()
{
	if (cnt) 
	{
		cnt->Destroy(); //delete cnt; 
		cnt = NULL;
	}
}
/*
void BasicConnection::SendKey(BOOL cad)
{
	HWND hwnd = (HWND)cnt->GetHandle();
	EnableWindow(hwnd,true);
	SetActiveWindow(hwnd);

	keybd_event( VkKeyScan(VK_CONTROL),	0x1D, 0,0 );
	keybd_event( VkKeyScan(VK_MENU),0x38,0,0 );
	keybd_event( VkKeyScan(VK_DELETE),0x53, 0, 0 );
	keybd_event( VkKeyScan(VK_DELETE), 0x53, KEYEVENTF_KEYUP, 0 );
	keybd_event( VkKeyScan(VK_CONTROL), 0x1D, KEYEVENTF_KEYUP, 0 );
	keybd_event( VkKeyScan(VK_MENU), 0x38, KEYEVENTF_KEYUP, 0);

}
*/
void BasicConnection::photo()
{
	wxClientDC client_dc(this);
	wxBitmap bmp(client_dc.GetSize().GetWidth(), client_dc.GetSize().GetHeight()); 
	wxMemoryDC mdc(bmp); 
	int x_bmp = bmp.GetWidth();
	int y_bmp =  bmp.GetHeight();
	mdc.Blit(0, 0, bmp.GetWidth(), bmp.GetHeight(), &client_dc, 0, 0); 
	mdc.SelectObject(wxNullBitmap);

	wxFileDialog * savefiledialog = new wxFileDialog(this,wxT("Save screenshot"),wxT(""),wxT(""),wxT("*.bmp"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	int iRes;
	iRes = savefiledialog->ShowModal();
	if (iRes == wxID_CANCEL){ delete savefiledialog; return;}


	wxString filepath = savefiledialog->GetPath();
	if (!filepath.IsEmpty())
	{
		bmp.SaveFile(filepath,wxBITMAP_TYPE_BMP); 
	}
	delete savefiledialog;
	


}

void BasicConnection::SetUniqConn()
{
	if (!main_frame) return;
	Benc bc;
	bool ok = false;
	while(!ok)
	{
		ok = true;
		int temp = bc.generate_uniq_name(&main_frame->base);
		size_t conn_num = main_frame->nb->GetPageCount();
		for (size_t i = 0; i < conn_num; i++)
		{
			ConnSplitter * splitter = (ConnSplitter *)main_frame->nb->GetPage(i);
			if (!splitter) continue;
			BasicConnection * basic_conn = (BasicConnection *)splitter->conn;
			if (!basic_conn) continue;
			if (temp == basic_conn->uniq_conn) ok = false;
		}
		if (ok)
		{
			uniq_conn  = temp;
			break;
		}
	}
}


void BasicConnection::screenshot(wxWindow * win)
{
	/*
	wxClientDC client_dc(win);
	wxBitmap bmp(client_dc.GetSize().GetWidth(), client_dc.GetSize().GetHeight()); 
	wxMemoryDC mdc(bmp); 
	int x_bmp = bmp.GetWidth();
	int y_bmp =  bmp.GetHeight();
	mdc.Blit(0, 0, bmp.GetWidth(), bmp.GetHeight(), &client_dc, 0, 0); 
	mdc.SelectObject(wxNullBitmap); 
	

	wxImage im = bmp.ConvertToImage(); 
	bmp = (wxBitmap)im.Scale(0.5*x_bmp,0.5*y_bmp,wxIMAGE_QUALITY_HIGH); 
	im.Rescale(0.5*x_bmp,0.5*y_bmp,wxIMAGE_QUALITY_HIGH); 
	bmp = (wxBitmap)im;
	
	m_screenshot = bmp;
	
	wxPaintDC dc(splitter_rdp->screenshot);
	splitter_rdp->screenshot->PrepareDC(dc);
	dc.Clear(); 
	dc.DrawBitmap(m_screenshot,wxPoint(0,0));
	splitter_rdp->screenshot->Refresh();
	*/
}

void BasicConnection::SendConnectEvent()
{
//	wxMessageBox(wxT("SendConnectEvent")); 
	if (info_uniq_name != 0)
	{
//		wxMessageBox(wxT("send connect event"));

		
		TREEPANEL(main_frame->m_panel_tree)->rdptree->from_wxrdp(info_uniq_name,TREEDATA_INC_CONNCOUNT);  
	}

}

void BasicConnection::SendDisconnectEvent()
{
//	wxMessageBox(wxT("SendDisconnectEvent")); 
	if (info_uniq_name != 0)
	{
		TREEPANEL(main_frame->m_panel_tree)->rdptree->from_wxrdp(info_uniq_name,TREEDATA_DEC_CONNCOUNT);  
		wxCommandEvent eventCustom(ID_FULLSCREEN);                                                                                              
		wxPostEvent(main_frame, eventCustom);
	}

}

void BasicConnection::SendEnterFullscreenEvent()
{
	CheckOptions();

}

void BasicConnection::SendLeaveFullscreenEvent()
{
	CheckOptions();	
}


void BasicConnection::CheckOptions()
{
	if ((ConnSplitter *)this->main_frame->nb->GetPage(this->main_frame->nb->GetSelection()) == this->conn_splitter)
	{
		this->main_frame->CheckCurrentConnectionMenu();
	}
}

Options_HashMap BasicConnection::Get_Options()
{
   return options;
}




FullScreenToolBar::FullScreenToolBar(wxString caption, wxWindow * parent, wxWindowID id, const wxPoint& point, const wxSize& size):
	wxAuiToolBar(parent,id,point,size, wxAUI_TB_DEFAULT_STYLE )
{
	SetToolBitmapSize(wxSize(8,8));
	m_caption = new wxStaticText(this,-1,caption,wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE);

	AddTool(ID_FULLSCREEN_AUTOHIDE,LOAD_XPM(media_eject),NULL,true,NULL,wxT("Auto hide"));
	AddControl(m_caption,wxT(""));
	AddSeparator();
	AddTool(ID_FULLSCREEN_PHOTO,wxT("Make screenshot"),LOAD_XPM(camera_photo16),wxT("Make screnshot"));
	AddTool(ID_FULLSCREEN_GRABINPUT,LOAD_XPM(input_keyboard16),NULL,true,NULL,wxT("Grab Input"));
	
	AddTool(ID_FULLSCREEN_SENDCAD,wxT("Send Ctrl+Alt+Del"),LOAD_XPM(go_down16),wxT("Send Ctrl+Alt+Del"));
	AddTool(ID_FULLSCREEN_LEAVE,wxT("Leave full screen"),LOAD_XPM(preferences_system_windows),wxT("Leave full screen"));
	
	AddTool(ID_FULLSCREEN_CLOSE,wxT("Disconnect"),LOAD_XPM(emblem_unreadable),wxT("Disconnect"));
	
	SetPosition(wxPoint(0,0));
	CenterOnParent(wxHORIZONTAL);
	 
	Realize();
	ToggleTool(ID_FULLSCREEN_AUTOHIDE, true);
	//on_leave();
	
	//wxMouseEvent eventCustom(ID_FULLSCREEN_AUTOHIDE);
	//wxPostEvent(this, eventCustom);
}

FullScreenToolBar::~FullScreenToolBar()
{
	delete m_caption;
}



void ToolbarCnt::on_enter(wxMouseEvent& event)
{
	SetPosition(wxPoint(0,0));
	CenterOnParent(wxHORIZONTAL);
}

void ToolbarCnt::on_leave_func(wxMouseEvent& event)
{
   on_leave();
}

void ToolbarCnt::on_leave()
{
   if (fs->GetToolToggled(ID_FULLSCREEN_AUTOHIDE))
   {
      wxSize size = GetClientSize();
      SetPosition(wxPoint(0,(-1)*size.y + 2));
      CenterOnParent(wxHORIZONTAL);
   }
}

ToolbarCnt::ToolbarCnt(wxWindow * parent):
	wxWindow(parent,-1,wxPoint(0,0),wxSize(200,22))	
{

}
