///////////////////////////////////////////////////////////////////////////////
// File name:   network_helper.hpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-06-01 16:34:32" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#ifndef NETWORKHELPER
#define NETWORKHELPER

//#include <wx/socket.h>
#ifdef __WXMSW__
//#include <winsock2.h>
#endif
#ifdef __WXGTK__
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif
#include "rdp_dialogs.hpp"

class NetworkHelper
{
public:
	NetworkHelper();
	~NetworkHelper();

	BOOL_L Test(wxString hostname,USHORT port);
	BOOL_L Test(wxString hostname,wxString port);
};












#endif //NETWORKHELPER
