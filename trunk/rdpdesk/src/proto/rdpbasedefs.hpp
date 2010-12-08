///////////////////////////////////////////////////////////////////////////////
// File name:   rdpbasedefs.hpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-12-08 13:13:43"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef RDPBASEDEFS
#define RDPBASEDEFS

#include "defineds.hpp"
#include <wx/wx.h>


class MainFrame; // TODO: remove it

#define TS_PERF_DISABLE_WALLPAPER        0x00000001
#define TS_PERF_ENABLE_ENHANCED_GRAPHICS 0x00000010
#define TS_PERF_DISABLE_FULLWINDOWDRAG   0x00000002
#define TS_PERF_DISABLE_THEMING          0x00000008

#ifdef __WXMSW__
#define CONTROL_H 20
#define SHIFT_H 1
#endif

#ifdef __WXGTK__
#define CONTROL_H 25
#define SHIFT_H 4
#endif

typedef enum __UsedConnectionType
{
	ConnectionType_Unknown = 0,
	ConnectionType_RDP,
	ConnectionType_ICA
} UsedConnectionType;
/*
typedef enum __ICACitrixEncryptionType
{
	Basic = 0,
	RC5Logon,
	RC5_40,
	RC5_56,
	RC5_128
}ICACitrixEncryptionType;
*/

typedef struct __RDPConn
{
	wxString hostname;
	wxString username;
	wxString password;
	wxString domain;
	wxString port;
	BOOL_L attach_to_console;

	USHORT width;
	USHORT heigth;
	BYTE_L color_depth;

	BOOL_L bSmartSizing;
	BOOL_L bControlSize;
	BOOL_L bFullScreen;

	BOOL_L force_update_screen;

	wxString shell;
	wxString directory;
	BOOL_L bUseProgram;
	BOOL_L bProgramMaximized;

	wxString keyboard_map;
	BYTE_L keyboard;

	BOOL_L bShareDrives;
	BOOL_L bSharePrinters;
 	BOOL_L bShareComPorts;
 	BOOL_L bShareSmartCards;
 	BYTE_L SoundType;
	wxString redirect_devices_nix;

	BOOL_L bEnableBitmapCaching;

	BOOL_L bEnableWallpaper;
	BOOL_L bEnableFullWindowDrag;
 	BOOL_L bEnableAnimation;
 	BOOL_L bEnableThemes;

	BYTE_L bandwidth;

	BOOL_L backing_store;
	BOOL_L encription_enable_french;
	BOOL_L encription_enable_new;
	BYTE_L use_rdp_version;
	BOOL_L send_mouse_event;
	BOOL_L private_color_map;

	BYTE_L single_mode;
	BOOL_L numlock_sync;
	BOOL_L enable_compres;

	DWORD dwConnectionCount;
	int uniq_name;
	wxString group_name;
	wxString connection_name;
	BOOL_L state;


#ifdef __WXGTK__
	int pid;
	wxProcess *rdp_process;
#endif

	UsedConnectionType conn_type;

	// ICA Citrix
	wxString server_ini;
	wxString client_ini;
	wxString ica_file;
	BOOL_L bUseApplication;
	wxString IcaApplication;
	//ICACitrixEncryptionType ica_encryption;
	int ica_encryption;
	BOOL_L bIcaEncryption;
	BOOL_L bIcaSound;
	BYTE_L bIcaSoundType;

	// Proxy Settings (for ICA Citrix)
	BYTE_L bProxyType; // 0 - no proxy; 1-socks; 2- SSL
	wxString ProxyAddr;
	wxString ProxyPort;
	//wxString ProxyUsername;
	wxString ProxyUserId;
	wxString ProxyPassword;
	//wxString ProxyCertificate;

}RDPConn;




struct programsettings
{
   wxString rdpbasepath;
   wxString rdpkeypath;
   wxString rdesktoppath;
   wxString freerdppath;
   wxString rdesktop_key_path;
   BOOL_L bIcon;
   BOOL_L bShowFrame;
   BOOL_L bUseCrypt;
   int iTypeCrypt;
   BOOL_L bFocusPage;
   BOOL_L bAutosave;
   int favorites_count;
   int favorites_max;
   wxString lang;
   int grabkbd;
};

static programsettings main_settings;

programsettings load_main_settings();


#ifdef __WXMSW__
#define GETBASEPATH()							\
   wxString BASEPATH = wxString::Format(wxT("%s\\%s"),load_main_settings().rdpbasepath.fn_str(),STANDARD_RDPBASE_NAME);



#define GETKEYPATH()							\
   wxString KEYPATH = wxString::Format(wxT("%s\\%s"),load_main_settings().rdpkeypath.fn_str(), STANDARD_RDPKEY_NAME);

#define GETRDESKTOPPATH()						\
   wxString RDESKTOPPATH = wxString::Format(wxT("%s\\rdesktop.exe"),load_main_settings().rdesktoppath.fn_str());

#endif
#ifdef __WXGTK__
/*
  #define GETBASEPATH()													\
  wxString BASEPATH = load_main_settings().rdpbasepath + wxT("/RDPBase.txt");

  #define GETKEYPATH()													\
  wxString KEYPATH = load_main_settings().rdpkeypath + wxT("/rdpkey.txt");

  #define GETRDESKTOPPATH()												\
  wxString RDESKTOPPATH;												\
  RDESKTOPPATH.Clear();												\
  if (load_main_settings().rdesktoppath.IsEmpty() == false)			\
  {																	\
  RDESKTOPPATH = load_main_settings().rdesktoppath + wxT("/rdesktop"); \
  }
  \

  #define GETRDESKTOPMAPPATH()											\
  wxString RDESKTOPMAPPATH;										\
  RDESKTOPMAPPATH.Clear();										\
  if (load_main_settings().rdesktop_key_path.IsEmpty() == false)	\
  {																\
  RDESKTOPMAPPATH = load_main_settings().rdesktop_key_path;		\
  }																\

*/

#define GETBASEPATH()							\
   wxString BASEPATH = load_main_settings().rdpbasepath + wxT("/") + STANDARD_RDPBASE_NAME;

#define GETKEYPATH()							\
   wxString KEYPATH = load_main_settings().rdpkeypath + wxT("/") + STANDARD_RDPKEY_NAME;

#define GETRDESKTOPPATH()						\
   wxString RDESKTOPPATH;						\
   RDESKTOPPATH.Clear();						\
   if (load_main_settings().rdesktoppath.IsEmpty() == false)		\
   {									\
      RDESKTOPPATH = load_main_settings().rdesktoppath + wxT("/rdesktop"); \
   }
\

#define GETRDESKTOPMAPPATH()						\
   wxString RDESKTOPMAPPATH;						\
   RDESKTOPMAPPATH.Clear();						\
   if (load_main_settings().rdesktop_key_path.IsEmpty() == false)	\
   {									\
      RDESKTOPMAPPATH = load_main_settings().rdesktop_key_path;		\
   }									\

#define GETFREERDPPATH()						\
   wxString FREERDPPATH;						\
   FREERDPPATH.Clear();							\
   if (load_main_settings().rdesktoppath.IsEmpty() == false)		\
   {									\
      FREERDPPATH = load_main_settings().rdesktoppath + wxT("/xfreerdp"); \
   }


#endif




#endif
