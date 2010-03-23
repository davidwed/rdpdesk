///////////////////////////////////////////////////////////////////////////////
// File name:   defineds.hpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-03 18:25:13" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#ifndef DEFINED_HPP
#define DEFINED_HPP

#ifndef __WXMSW__
typedef int BOOL;
typedef int USHORT;	// typedef unsigned short USHORT
typedef int BYTE; // typedef unsigned char BYTE
typedef int DWORD;
typedef long LONG;
typedef char CHAR;
#endif

#ifndef __WXGTK__
typedef USHORT ushort;
#endif

//#define __DEBUG_LINUX_CLIENT

#ifdef __DEBUG_LINUX_CLIENT
 #define FUNC_AND_LINE std::cout <<__LINE__<<" "<< __func__ << std::endl;
#else
#define FUNC_AND_LINE
#endif

#ifdef __WXMSW__
#define LOAD_XPM(xpm) wxBitmap(xpm,wxBITMAP_TYPE_XPM )
#endif
#ifdef __WXGTK__
#define LOAD_XPM(xpm) wxBitmap(xpm)
#endif

#define STANDARD_RDESKTOP_PATH wxT("/usr/bin")
#define STANDARD_RDESKTOP_KEY_PATH wxT("/usr/share/rdesktop/keymaps")

#define STANDARD_FAVORITES_COUNT 2
#define STANDARD_FAVORITES_MAX 3

//#define STANDARD_RDPBASE_NAME wxT("RDPBase.txt") 
//#define STANDARD_RDPKEY_NAME  wxT("rdpkey.txt") 
//#define STANDARD_RDPCONF_NAME wxT("rdpset.conf")
//#define STANDARD_RDP_PATH wxT("TerminalClients")
//#define STANDARD_LINUX_ENV wxT("HOME")

#define STANDARD_RDPBASE_NAME wxT("RDPBase.db") 
#define STANDARD_RDPKEY_NAME  wxT("rdpkey.key") 
#define STANDARD_RDPCONF_NAME wxT("rdpset.conf")
#define STANDARD_RDP_PATH wxT("rdpdesk")
#define STANDARD_LINUX_ENV wxT("HOME")

#ifdef __WXMSW__
#define DIALOG_NEWCONN_WIDTH 380
#define SHIFT_W 0
#endif

#ifdef __WXGTK__
#define DIALOG_NEWCONN_WIDTH 450
#define SHIFT_W 45
#endif

#ifdef __WXMSW__
#define SPLITTER_SASH_SIZE 2
#endif

#ifdef __WXGTK__
#define SPLITTER_SASH_SIZE 5
#endif

#define APPLICATION_LANG_NAME wxT("terminal_clients")

/*
#define TS_VERSION wxT("2.0")
#define TS_NAME wxT("RDPDesk")
#define TS_DESCRIPTION wxT("Description terminal clients")
#define TS_COPYRIGHT wxT("Copyright terminal clients")
*/

#define TS_VERSION wxT("2.0")

#define TS_NAME wxT("RDPDesk")

#define TS_DESCRIPTION wxT("Remote Desktop Protocol Infrastructure")

#define TS_COPYRIGHT wxT("SolveITLabs, 2009-2010. All rights reserved.")

#define TS_NORMAL_VERSION

#endif
