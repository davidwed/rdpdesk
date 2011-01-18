///////////////////////////////////////////////////////////////////////////////
// File name:   small_shoot.hpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef SMALL_SHOOT_HPP
#define SMALL_SHOOT_HPP

#include <wx/wx.h>
#include "rdpbasedefs.hpp"
//#ifdef __WXGTK__
//#include "main_window.hpp"
//#endif

const int ID_CONNECT_FROM_SCREENSHOT = 142;

class PanelBitmapBtn : public wxPanel
{
public:
	PanelBitmapBtn(wxWindow * parent, wxWindowID id,const wxPoint& point,const wxSize& size,
				   long style = 0);
	~PanelBitmapBtn();
	MainFrame * main_frame;
	wxBitmapButton * bmpbtn;
	void bmpbtn_func(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif //SMALL_SHOOT_HPP
