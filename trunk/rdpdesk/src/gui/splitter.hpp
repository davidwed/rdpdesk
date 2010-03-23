///////////////////////////////////////////////////////////////////////////////
// File name:   splitter.hpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-21 21:13:11" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#ifndef SPLITTER_HPP
#define SPLITTER_HPP

#include <wx/wx.h>
#include <wx/splitter.h>

#include "main_window.hpp"

//#include "small_shoot.hpp"
//#include "rdpbasedefs.hpp"



class BasicConnection;


//#ifdef __WXGTK__
class PanelBitmapBtn;
//#endif


class ConnSplitter : public wxSplitterWindow
{
public:
   ConnSplitter(Main_Frame * main,Options_HashMap local_options ,wxWindow * parent, wxWindowID id = -1,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxPanelNameStr);

   ~ConnSplitter();

   //wxRDP * rdp;
   BasicConnection * conn;
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
   BOOL check_conn();
   //RDPConn rdpc;
   Options_HashMap options;
   
   DECLARE_EVENT_TABLE();
};

const int ID_TIMER_SCREENSHOT = 1000;





#endif //SPLITTER_HPP

