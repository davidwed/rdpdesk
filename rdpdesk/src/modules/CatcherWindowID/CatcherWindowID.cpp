///////////////////////////////////////////////////////////////////////////////
// File name:   CatcherWindowID.hpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////


#include "CatcherWindowID.hpp"

CatcherWindowID::CatcherWindowID(wxWindow *parent, int width, int height, int eventId)
   :windowId(0),
    localWidth(width),
    localHeight(height),
    localEventId(eventId),
    localParent(parent)
{
}

CatcherWindowID::~CatcherWindowID()
{
}

void* CatcherWindowID::Entry()
{
   if (localParent != NULL)
   {
      Display *display = XOpenDisplay(NULL);
      Window root = DefaultRootWindow(display);
      XSetWindowAttributes attributes;
      attributes.event_mask = SubstructureNotifyMask;
      XSelectInput(display, root, attributes.event_mask);
      XSetErrorHandler(NULL);
      XEvent xEvent;
      for (;;) {
	 XNextEvent(display, &xEvent);
	 if (xEvent.type == CreateNotify)
	 {
	    Atom actual_type;
	    int actual_format;
	    unsigned long nitems, leftover;
	    unsigned char *pid;
	    pid_t pid_return = -1;

	    if (XGetWindowProperty(display, xEvent.xcreatewindow.window,
				   XInternAtom(display, "_NET_WM_PID", False), 0,
				   1, False, XA_CARDINAL, &actual_type,
				   &actual_format, &nitems, &leftover, &pid) == Success)
	    {
	       if (pid) pid_return = *(pid_t *) pid;
	       XFree(pid);
	    }
	    if ((pid_return			 == -1) &&
		(xEvent.xcreatewindow.width	 == localWidth) &&
		(xEvent.xcreatewindow.height	 == localHeight))
	    {
	       XUnmapWindow(display, xEvent.xcreatewindow.window);
	       XSync(display,0);
	       wxCommandEvent wxLocalEvent(localEventId);
 	       wxLocalEvent.SetExtraLong(xEvent.xcreatewindow.window);
	       wxPostEvent(localParent, wxLocalEvent);
	       break;
	    }

	 }
      }
      XCloseDisplay(display);
   }
   return 0;
}
