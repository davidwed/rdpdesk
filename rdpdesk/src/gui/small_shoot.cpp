///////////////////////////////////////////////////////////////////////////////
// File name:   small_shoot.cpp
// Version:     0.0
// Purpose: 
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#include "small_shoot.hpp"
//#ifdef __WXMSW__
#include "main_window.hpp"
//#endif

PanelBitmapBtn::PanelBitmapBtn(wxWindow * parent, wxWindowID id,const wxPoint& point,const wxSize& size, long style):
wxPanel(parent,id,point,size,style)
{
	wxBitmap bmp;
	bmpbtn = new wxBitmapButton(this,ID_CONNECT_FROM_SCREENSHOT,bmp,wxPoint(0,0),this->GetSize(),
								wxNO_BORDER|wxBU_AUTODRAW );
}

void PanelBitmapBtn::bmpbtn_func(wxCommandEvent &event)
{
	wxCommandEvent evt;
	evt.SetId(1);
	main_frame->on_connect(evt); 
}

PanelBitmapBtn::~PanelBitmapBtn()
{
	if (bmpbtn) 
	{
		bmpbtn->Destroy(); //delete bmpbtn;
		bmpbtn = NULL;
	}
}










