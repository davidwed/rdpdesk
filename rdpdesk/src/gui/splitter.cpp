///////////////////////////////////////////////////////////////////////////////
// File name:   splitter.cpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-11-24 17:54:20"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include "splitter.hpp"
//#include "BasicConnection.hpp"
#include "small_shoot.hpp"

#ifdef __WXGTK__
#include "RDPConnection_nix.hpp"
#include "ICAConnection_nix.hpp"
#include "RFBConnection_nix.hpp"
#include "FRDPConnection_nix.hpp"
#endif

#ifdef __WXMSW__
#include "RDPConnection_win.hpp"
#include "ICAConnection_win.hpp"
#include "RFBConnection_win.hpp"
#endif
//#include "main_window.hpp"

#ifdef __WXGTK__
///////////////////////////////////////////////////////////////////////////////
//! \brief Initialise main spliter
///////////////////////////////////////////////////////////////////////////////
void ConnSplitter::init()
{
   wxSize szRDP = GetClientSize();
   if (options[wxT("proto")] == wxT("rdp"))
   {
      this->conn = new RDPConnection(main_frame,options,this,wxID_ANY,wxPoint(0,0),szRDP,0,wxT("Test"));
   }
   if (options[wxT("proto")] == wxT("frdp"))
   {
      this->conn = new FRDPConnection(main_frame,options,this,wxID_ANY,wxPoint(0,0),szRDP,0,wxT("Test"));
   }
   else if (options[wxT("proto")] == wxT("ica"))
   {
      this->conn = new ICAConnection(main_frame,options,this,wxID_ANY,wxPoint(0,0),szRDP,0,wxT("Test"));
   }
   else if (options[wxT("proto")] == wxT("rfb"))
   {
      this->conn = new RFBConnection(main_frame,options,this,wxID_ANY,wxPoint(0,0),szRDP,0,wxT("Test"));
   }
   else
   {
      this->conn = new RDPConnection(main_frame,options,this,wxID_ANY,wxPoint(0,0),szRDP,0,wxT("Test"));
   }

   if (conn != NULL)
   {
      this->screenshot = new PanelBitmapBtn(this,-1,wxDefaultPosition,wxDefaultSize,0);
      screenshot->main_frame = main_frame;

      this->screenshot->Hide();
      this->Initialize(conn);
      this->SetSashGravity(0.3);
      this->SetSashSize(1);
      m_timer_screenshot.SetOwner(this,ID_TIMER_SCREENSHOT);
   }
}
#endif

#ifdef __WXGTK__
void ConnSplitter::switch_to_on()
{
   screenshot->SetPosition(wxPoint(0,0));
   screenshot->SetSize(this->GetClientSize());
   screenshot->bmpbtn->SetPosition(wxPoint(0,0));
   screenshot->bmpbtn->SetSize(screenshot->GetClientSize());

   screenshot->bmpbtn->SetBitmapFocus(bmp_screenshot);
   screenshot->bmpbtn->SetBitmapSelected(bmp_screenshot);
   screenshot->bmpbtn->SetBitmapDisabled(bmp_screenshot);
   screenshot->bmpbtn->SetBitmapHover(bmp_screenshot);
   screenshot->bmpbtn->SetBitmapLabel(bmp_screenshot);

   main_frame->nb->Refresh();
   main_frame->nb->Update();

   Initialize(screenshot);
   Unsplit(conn);
   screenshot->Show(true);
   conn->Hide();

   state = FALSE;

}

void ConnSplitter::switch_to_off()
{
   Initialize(conn);
   Unsplit(screenshot);
   screenshot->Hide();
   conn->Show(true);
   conn->SetFocus();
   state = TRUE;

}


void ConnSplitter::switch_state()
{
   if (state)
   {
      switch_to_on();
   }
   else
   {
      switch_to_off();
   }
}
#endif

ConnSplitter::ConnSplitter(MainFrame * main,Options_HashMap local_options ,wxWindow * parent,
			   wxWindowID id, const wxPoint& pos, const wxSize& size,
			   long style, const wxString& name):
   wxSplitterWindow(parent,-1,pos,size,wxSP_3D),
   conn(NULL),
   screenshot(NULL)
{
   main_frame = main;
   state = FALSE;
   options = local_options;
}

BOOL_L ConnSplitter::check_conn()
{

   if (conn == NULL)
   {
      return false;
   }
   else
   {
      return conn->bObjectOk;
   }
}

#ifdef __WXMSW__
void ConnSplitter::init()
{
   if (options[wxT("proto")] == wxT("rdp"))
   {
      this->conn = new RDPConnection(main_frame,options,this,/*__uuidof(MSTSCLib::MsRdpClient) ,*/this->GetId() ,wxPoint(0,0),GetClientSize(),WS_VISIBLE ,wxT("Test"));
   }
   else if (options[wxT("proto")] == wxT("ica"))
   {
      this->conn = new ICAConnection(main_frame,options,this,/*__uuidof(MSTSCLib::MsRdpClient) ,*/this->GetId() ,wxPoint(0,0),GetClientSize(),WS_VISIBLE ,wxT("Test"));
   }
   else if (options[wxT("proto")] == wxT("rfb"))
   {
	 this->conn = new RFBConnection(main_frame,options,this,/*__uuidof(MSTSCLib::MsRdpClient) ,*/this->GetId() ,wxPoint(0,0),GetClientSize(),WS_VISIBLE ,wxT("Test"));
   }
   else  // TEST
   {
      this->conn = new RDPConnection(main_frame,options,this,/*__uuidof(MSTSCLib::MsRdpClient) ,*/this->GetId() ,wxPoint(0,0),GetClientSize(),WS_VISIBLE ,wxT("Test"));
   }
   this->conn->conn_splitter = this;

   this->screenshot = new PanelBitmapBtn(this,-1,wxDefaultPosition,wxDefaultSize,WS_VISIBLE);
   screenshot->main_frame = main_frame;

   this->screenshot->Hide();
   this->Initialize( conn);
   this->SetSashGravity(0.3);
   this->SetSashSize(1);

   m_timer_screenshot.SetOwner(this,ID_TIMER_SCREENSHOT);
}
#endif
ConnSplitter::~ConnSplitter()
{
	if (conn)
	{
		conn->Destroy(); //delete conn;
		conn = NULL;
	}
	if (screenshot)
	{
		screenshot->Destroy(); //delete screenshot;
		screenshot = NULL;
	}
}

void ConnSplitter::on_timer_screenshot(wxTimerEvent& event)
{
	if (conn && screenshot)
	{
		if (state)
		{
			create_screenshot();
		}
	}
}

void ConnSplitter::on_enter_focus(wxFocusEvent& event)
{
	if (conn && screenshot)
	{
		if (!m_timer_screenshot.IsRunning())
		{
			m_timer_screenshot.Start(20000);
		}
		if (!state)
		{
			screenshot->SetPosition(wxPoint(0,0));
			screenshot->SetSize(GetClientSize());
			screenshot->bmpbtn->CenterOnParent();
		}
	}
	event.Skip();
}

void ConnSplitter::on_kill_focus(wxFocusEvent& event)
{
	if (screenshot)
	{
		if (m_timer_screenshot.IsRunning())
		{
			m_timer_screenshot.Stop();
		}
	}
	event.Skip();
}

void ConnSplitter::on_activate(wxActivateEvent& event)
{
	if (screenshot)
	{
		if (event.GetActive())
		{
			if (!m_timer_screenshot.IsRunning())
			{
				m_timer_screenshot.Start(20000);
			}
			if (!state)
			{
				screenshot->SetPosition(wxPoint(0,0));
				screenshot->SetSize(GetClientSize());
				screenshot->bmpbtn->CenterOnParent();
			}
		}
		else
		{
			if (m_timer_screenshot.IsRunning())
			{
				m_timer_screenshot.Stop();
			}
		}
	}
	event.Skip();
}

void ConnSplitter::create_screenshot()
{
	wxClientDC client_dc(this->conn);
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

	bmp_screenshot = bmp;
}
#ifdef __WXMSW__
void ConnSplitter::switch_state()
{
	if (conn && screenshot)
	{
		if (state)
		{
			screenshot->SetPosition(wxPoint(0,0));
			screenshot->SetSize(main_frame->nb->GetClientSize());
			screenshot->bmpbtn->SetSize(screenshot->GetClientSize());

			screenshot->bmpbtn->SetBitmapFocus(bmp_screenshot);
			screenshot->bmpbtn->SetBitmapSelected(bmp_screenshot);
			screenshot->bmpbtn->SetBitmapDisabled(bmp_screenshot);
			screenshot->bmpbtn->SetBitmapHover(bmp_screenshot);
			screenshot->bmpbtn->SetBitmapLabel(bmp_screenshot);

			main_frame->nb->Refresh();
			main_frame->nb->Update();

			Initialize(screenshot);
			Unsplit(conn);

			screenshot->CenterOnParent();
			screenshot->bmpbtn->CenterOnParent();
			screenshot->Show(true);
			conn->Hide();

			state = FALSE;
		}
		else
		{
			Initialize(conn);
			Unsplit(screenshot);
			screenshot->CenterOnParent();
			screenshot->bmpbtn->CenterOnParent();
			screenshot->Hide();
			conn->Show(true);
			if ((ConnSplitter *)main_frame->nb->GetPage(main_frame->nb->GetSelection()) == this)
			{
				conn->SetFocus();
			}
			state = TRUE;
		}
	}
}

#endif
