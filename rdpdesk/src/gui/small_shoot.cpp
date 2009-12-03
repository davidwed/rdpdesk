///////////////////////////////////////////////////////////////////////////////
// File name: small_shoot.cpp
// Date create: Fri Sep 25 13:36:57 2009
// Version: 0.0
// Time-stamp: "2009-10-09 13:12:25" 
// E-mail: 
// Content-Type: text/plain; charset=utf8
// $Id$ 
// Description: 
// 
// 
// 
///////////////////////////////////////////////////////////////////////////////

#include "small_shoot.hpp"
#ifdef __WXMSW__
#include "main_window.hpp"
#endif

PanelBitmapBtn::PanelBitmapBtn(wxWindow * parent, wxWindowID id,const wxPoint& point,const wxSize& size, long style):
wxPanel(parent,id,point,size,style)
{
	wxBitmap bmp;
	bmpbtn = new wxBitmapButton(this,ID_CONNECT_FROM_SCREENSHOT,bmp,wxPoint(0,0),this->GetSize(),wxNO_BORDER );
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
