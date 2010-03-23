///////////////////////////////////////////////////////////////////////////////
// File name:   rdpbasedefs.hpp
// Version:     0.0
// Purpose: 
// Time-stamp:  "2010-03-03 18:19:49" 
// E-mail:      rdpdesk@rdpdesk.com
// $Id$ 
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#ifndef RDPBASEDEFS
#define RDPBASEDEFS

#include "defineds.hpp"
#include <wx/wx.h>


class Main_Frame;



#define TS_PERF_DISABLE_WALLPAPER 0x00000001
#define TS_PERF_ENABLE_ENHANCED_GRAPHICS 0x00000010
#define TS_PERF_DISABLE_FULLWINDOWDRAG 0x00000002
#define TS_PERF_DISABLE_THEMING 0x00000008

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
	BOOL attach_to_console;
				
	USHORT width;
	USHORT heigth;
	BYTE color_depth;
		
	BOOL bSmartSizing;
	BOOL bControlSize;
	BOOL bFullScreen;
		
	BOOL force_update_screen; 

	wxString shell;
	wxString directory;
	BOOL bUseProgram;
	BOOL bProgramMaximized;
					
	wxString keyboard_map; 
	BYTE keyboard; 

	BOOL bShareDrives;
	BOOL bSharePrinters;
 	BOOL bShareComPorts;
 	BOOL bShareSmartCards;
 	BYTE SoundType;
	wxString redirect_devices_nix;
	
	BOOL bEnableBitmapCaching;
	
	BOOL bEnableWallpaper;
	BOOL bEnableFullWindowDrag;
 	BOOL bEnableAnimation;
 	BOOL bEnableThemes;
 	
	BYTE bandwidth; 

	BOOL backing_store; 
	BOOL encription_enable_french;
	BOOL encription_enable_new; 
	BYTE use_rdp_version; 
	BOOL send_mouse_event;  
	BOOL private_color_map; 
	
	BYTE single_mode; 
	BOOL numlock_sync; 
	BOOL enable_compres; 
	
	DWORD dwConnectionCount;
	int uniq_name;
	wxString group_name;
	wxString connection_name;
	BOOL state;


#ifdef __WXGTK__
	int pid;
	wxProcess *rdp_process;
#endif

	UsedConnectionType conn_type;

	// ICA Citrix
	wxString server_ini;
	wxString client_ini;
	wxString ica_file;
	BOOL bUseApplication;
	wxString IcaApplication;
	//ICACitrixEncryptionType ica_encryption;
	int ica_encryption;
	BOOL bIcaEncryption;
	BOOL bIcaSound;
	BYTE bIcaSoundType;

	// Proxy Settings (for ICA Citrix)
	BYTE bProxyType; // 0 - no proxy; 1-socks; 2- SSL
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
  wxString rdesktop_key_path;
  BOOL bIcon;
  BOOL bShowFrame;
  BOOL bUseCrypt;
  int iTypeCrypt;
  BOOL bFocusPage;
  BOOL bAutosave;
  int favorites_count;
  int favorites_max;
  wxString lang;
  int grabkbd;
};

static programsettings main_settings;

programsettings load_main_settings();


#ifdef __WXMSW__
#define GETBASEPATH()							\
   wxString BASEPATH = wxString::Format(wxT("%s\\%s"),load_main_settings().rdpbasepath,STANDARD_RDPBASE_NAME); 
	


#define GETKEYPATH()							\
   wxString KEYPATH = wxString::Format(wxT("%s\\%s"),load_main_settings().rdpkeypath, STANDARD_RDPKEY_NAME); 

#define GETRDESKTOPPATH()						\
   wxString RDESKTOPPATH = wxString::Format(wxT("%s\\rdesktop.exe"),load_main_settings().rdesktoppath); 

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




#endif




#endif
