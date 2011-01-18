///////////////////////////////////////////////////////////////////////////////
// File name:   main.cpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include "autoupdate.hpp"
#include "defineds.hpp"

#include <wx/stdpaths.h>
#include <wx/protocol/http.h>
#include <wx/wfstream.h>

Autoupdate::Autoupdate(MainFrame *main)
{
	downloader_stream = NULL;
	main_frame = main;
}

Autoupdate::~Autoupdate()
{
}

bool Autoupdate::GetAutoUpdateFile()
{
	wxHTTP downloader;
	wxStandardPaths sp;
	wxFile file;
	update_file = wxString::Format(wxT("%s/rdcm_update.xml"), sp.GetTempDir().data());
	downloader.SetHeader(wxT("Content-type"), wxT("text/html; charset=utf-8"));
	downloader.SetTimeout(100);

	if (downloader.Connect(UPDATE_URL))
	{
	  downloader_stream = downloader.GetInputStream(UPDATE_FILE);
		if (downloader.GetError() == wxPROTO_NOERR)
		{
		   wxString res;
			wxStringOutputStream out_stream(&res);
			if (downloader_stream->CanRead())
			{
				downloader_stream->Read(out_stream);
				file.Open(update_file, wxFile::write);
				file.Write(res);
				file.Close();
				if (main_frame != NULL)
				{
				   wxCommandEvent eventCustom(ID_UPFATE_FILE);
				   wxPostEvent(main_frame, eventCustom);
				}
			}
		}
	}

	if (downloader.GetError() == wxPROTO_NOERR)
	{
		downloader.Close();
		return true;
	} else {
		downloader.Close();
		return false;
	}
}

void* Autoupdate::Entry()
{
  wxSleep(UPDATE_FIRST_SLEEP);
  while (true) {
    GetAutoUpdateFile();
    wxSleep(UPDATE_SLEEP);
  }
  return NULL;
}
