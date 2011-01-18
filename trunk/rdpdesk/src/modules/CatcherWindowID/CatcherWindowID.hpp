///////////////////////////////////////////////////////////////////////////////
// File name:   CatcherWindowID.hpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef __CATCHERWINDOWID__
#define __CATCHERWINDOWID__

#include <wx/wx.h>
#include <wx/process.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>


class CatcherWindowID :public wxThread
{
public:
   CatcherWindowID (wxWindow *parent, int width, int height, int eventId);
   ~CatcherWindowID();
   virtual void* Entry();
private:
   int windowId;
   int localWidth;
   int localHeight;
   int localEventId;
   wxWindow *localParent;
};


#endif // __CATCHERWINDOWID__
