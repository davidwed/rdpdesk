///////////////////////////////////////////////////////////////////////////////
// File name:   main.hpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-12-02 15:30:38"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef MAIN_HPP
#define MAIN_HPP

/*
#include "main_window.hpp"
#include "wx/wx.h"
*/

/*
#ifdef __WXGTK__
#include "wxRDP_nix.hpp"
#endif
*/


#include <wx/wx.h>
#include <wx/statline.h>
#include "rdpbasedefs.hpp"
#include <wx/splash.h>
#include <wx/mstream.h>
#include <autoupdate.hpp>


//#include "proto.hpp"

//WX_DECLARE_OBJARRAY(options_struct, GeneralOptionsArray);

#define wxGetBitmapFromMemory(name) _wxGetBitmapFromMemory(name, sizeof(name))

inline wxBitmap _wxGetBitmapFromMemory(const unsigned char *data, int length) {
 	wxMemoryInputStream is(data, length);
 	return wxBitmap(wxImage(is, wxBITMAP_TYPE_ANY, -1), -1);
}


class RDP_Core : public wxApp
{
public:
    virtual bool OnInit();

	virtual void OnFatalException();
	virtual void OnUnhandledException();
	virtual bool OnExceptionInMainLoop();

   MainFrame * main_frame;
private:
	wxLocale locale;
	long language;

};

DECLARE_APP(RDP_Core)

class ExceptionMessage : public wxDialog
{
public:
	ExceptionMessage(bool fatal = true);
	~ExceptionMessage();
private:
	wxStaticText * m_static_msg;
	wxStaticBitmap * m_bitmap_err;
	wxStaticLine * m_line_separator;
	wxButton * m_button_ok;
	wxSizer * sizer_err;
	wxSizer * sizer_main;
	wxString msg;
};




#endif
