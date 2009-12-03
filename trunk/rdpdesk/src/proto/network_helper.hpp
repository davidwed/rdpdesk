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

	BOOL Test(wxString hostname,USHORT port);
	BOOL Test(wxString hostname,wxString port);
};












#endif //NETWORKHELPER