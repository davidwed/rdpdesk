///////////////////////////////////////////////////////////////////////////////
// File name:   network_helper.cpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include "network_helper.hpp"

NetworkHelper::NetworkHelper()
{
}

NetworkHelper::~NetworkHelper()
{
}

BOOL_L NetworkHelper::Test(wxString hostname,wxString port)
{
	unsigned long iPort;
	port.ToULong(&iPort);
	return Test(hostname,iPort);
}

BOOL_L NetworkHelper::Test(wxString hostname,USHORT port)
{
	BOOL_L bRes = FALSE;
#ifdef __WXMSW__
	WSAData wsa;
	WSAStartup(MAKEWORD(1,1),&wsa);
	SOCKET sock;
#endif
#ifdef __WXGTK__
	int sock;
#endif
	struct sockaddr_in sin;
	struct hostent *hp;
	DWORD lAddr;
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

#ifdef __WXGTK__
	if (sock == 0)
	{
#endif
#ifdef __WXMSW__
	if(sock == INVALID_SOCKET)
	{
		WSACleanup();
#endif

		goto end;
	}

	if ((hp = gethostbyname(hostname.To8BitData())) == NULL)
	{
		lAddr = inet_addr(hostname.To8BitData());
		if (lAddr == 0L)
		{
#ifdef __WXMSW__
			closesocket(sock);
			WSACleanup();
#endif
#ifdef __WXGTK__
			close(sock);
#endif

			goto end;
		}
		else
		{
			memcpy(&sin.sin_addr, &lAddr, 4);
		}
	}
	else
	{
		memcpy(&sin.sin_addr, hp->h_addr , 4);
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons((u_short)port);
#ifdef __WXGTK__
	if (connect(sock, (struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		close(sock);
	}
#endif
#ifdef __WXMSW__
	if(connect(sock, (struct sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		closesocket(sock);
		WSACleanup();
		goto end;
	}
#endif


	shutdown(sock,2);
#ifdef __WXMSW__
	closesocket(sock);
	WSACleanup();
#endif
#ifdef __WXGTK__
	close(sock);
#endif

	bRes = TRUE;
end:
	return bRes;


}
