///////////////////////////////////////////////////////////////////////////////
// File name:   autoupdate.hpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-11-24 16:16:45"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef AUTOUPDATE
#define AUTOUPDATE

//#include "rdpbasedefs.hpp"
#include <wx/thread.h>
#include <wx/sstream.h>
#include "main_window.hpp"

class MainFrame;

class Autoupdate :public wxThread
{
public:
	Autoupdate(MainFrame *main);
	~Autoupdate();
	virtual void* Entry();
private:
	bool GetAutoUpdateFile();

	wxInputStream *downloader_stream;
	wxString update_file;
	MainFrame *main_frame;
};

#endif //AUTOUPDATE
