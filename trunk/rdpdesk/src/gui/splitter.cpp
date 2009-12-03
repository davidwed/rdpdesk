///////////////////////////////////////////////////////////////////////////////
// File name: splitter.cpp
// Date create: Fri Sep 25 13:39:02 2009
// Version: 0.0
// Time-stamp: "2009-11-13 17:43:53" 
// E-mail: 
// Content-Type: text/plain; charset=utf8
// $Id$ 
// Description: 
// 
// 
// 
///////////////////////////////////////////////////////////////////////////////

#include "splitter.hpp"
#include "main_window.hpp"

#ifdef __WXGTK__
void wxSplitterRDP::init()
{
	//bool local_status = false;
	wxSize szRDP = GetClientSize();
	this->rdp = new wxRDP(main_frame,rdpc,this,wxID_ANY,wxPoint(0,0),szRDP,0,wxT("Test"));
	if (rdp != NULL)
	{
		this->screenshot = new PanelBitmapBtn(this,-1,wxDefaultPosition,wxDefaultSize,0);
		screenshot->main_frame = main_frame;
	
		this->screenshot->Hide();
		this->Initialize(rdp);
		this->SetSashGravity(0.3);
		this->SetSashSize(1);
		m_timer_screenshot.SetOwner(this,ID_TIMER_SCREENSHOT);
	}
	
}
#endif

#ifdef __WXGTK__
void wxSplitterRDP::switch_to_on()
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
	Unsplit(rdp);
	screenshot->Show(true);
	rdp->Hide();

	state = FALSE;

}

void wxSplitterRDP::switch_to_off()
{
	Initialize(rdp);
	Unsplit(screenshot);
	screenshot->Hide();
	rdp->Show(true);
	rdp->SetFocus();
	state = TRUE;

}


void wxSplitterRDP::switch_state()
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




wxSplitterRDP::wxSplitterRDP(Main_Frame * main,RDPConn rdpsettings ,wxWindow * parent, wxWindowID id,
        const wxPoint& pos, const wxSize& size,long style ,	const wxString& name):
wxSplitterWindow(parent,-1,pos,size,wxSP_3D),
rdp(NULL),
screenshot(NULL)
{
	main_frame = main;
	state = FALSE;
	rdpc = rdpsettings;
}

#ifdef __WXMSW__
void wxSplitterRDP::init()
{
	
	this->rdp = new wxRDP(main_frame,rdpc,this,__uuidof(MSTSCLib::MsRdpClient) ,this->GetId() ,wxPoint(0,0),GetClientSize(),WS_VISIBLE ,wxT("Test"));
	this->rdp->splitter_rdp = this;
	
	this->screenshot = new PanelBitmapBtn(this,-1,wxDefaultPosition,wxDefaultSize,WS_VISIBLE);
	screenshot->main_frame = main_frame;
	
	this->screenshot->Hide(); 
	this->Initialize( rdp);
	this->SetSashGravity(0.3);
	this->SetSashSize(1);

	m_timer_screenshot.SetOwner(this,ID_TIMER_SCREENSHOT);
}
#endif
wxSplitterRDP::~wxSplitterRDP()
{
	if (rdp) 
	{
		rdp->Destroy(); //delete rdp; 
		rdp = NULL;
	}
	if (screenshot) 
	{
		screenshot->Destroy(); //delete screenshot; 
		screenshot = NULL;
	}
}

void wxSplitterRDP::on_timer_screenshot(wxTimerEvent& event)
{
	if (rdp && screenshot)
	{
		if (state)
		{
			create_screenshot();
		}
	}
}

void wxSplitterRDP::on_enter_focus(wxFocusEvent& event)
{
	if (rdp && screenshot)
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

void wxSplitterRDP::on_kill_focus(wxFocusEvent& event)
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

void wxSplitterRDP::on_activate(wxActivateEvent& event)
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

void wxSplitterRDP::create_screenshot()
{
	wxClientDC client_dc(this->rdp);
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
void wxSplitterRDP::switch_state()
{
	if (rdp && screenshot)
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
			Unsplit(rdp);
	
			screenshot->CenterOnParent();
			screenshot->bmpbtn->CenterOnParent();
			screenshot->Show(true);
			rdp->Hide();

			state = FALSE;
		}
		else
		{
			Initialize(rdp);
			Unsplit(screenshot);
			screenshot->CenterOnParent();
			screenshot->bmpbtn->CenterOnParent();
			screenshot->Hide();
			rdp->Show(true);
			if ((wxSplitterRDP *)main_frame->nb->GetPage(main_frame->nb->GetSelection()) == this)
			{
				rdp->SetFocus();
			}
			state = TRUE;
		}
	}
}

#endif
