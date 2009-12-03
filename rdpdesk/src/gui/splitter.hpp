///////////////////////////////////////////////////////////////////////////////
// File name: splittre.hpp
// Date create: Fri Sep 25 13:37:47 2009
// Version: 0.0
// Time-stamp: "2009-11-05 16:46:57" 
// E-mail: 
// Content-Type: text/plain; charset=utf8
// $Id$ 
// Description: 
// 
// 
// 
///////////////////////////////////////////////////////////////////////////////

#ifndef SPLITTER_HPP
#define SPLITTER_HPP

#include <wx/wx.h>
#include <wx/splitter.h>


#include "small_shoot.hpp"
#include "rdpbasedefs.hpp"


class wxRDP;

#ifdef __WXGTK__
class PanelBitmapBtn;
#endif


class wxSplitterRDP : public wxSplitterWindow
{
public:
	wxSplitterRDP(Main_Frame * main,RDPConn rdpsettings ,wxWindow * parent, wxWindowID id = -1,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxPanelNameStr);

	~wxSplitterRDP();

	wxRDP * rdp;
	PanelBitmapBtn * screenshot;
private:
	wxBitmap bmp_screenshot;
	void on_timer_screenshot(wxTimerEvent& event);
	void on_enter_focus(wxFocusEvent& event);
	void on_kill_focus(wxFocusEvent& event);
	void on_activate(wxActivateEvent& event);
public:
	BOOL state;

	Main_Frame * main_frame;

	wxTimer m_timer_screenshot;
	
	void create_screenshot();
	void switch_state();

#ifdef __WXGTK__
	void switch_to_on();
	void switch_to_off();
#endif

	void init();
	RDPConn rdpc;

	DECLARE_EVENT_TABLE();
};

const int ID_TIMER_SCREENSHOT = 1000;





#endif //SPLITTER_HPP

