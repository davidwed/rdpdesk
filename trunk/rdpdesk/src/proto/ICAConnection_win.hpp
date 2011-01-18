///////////////////////////////////////////////////////////////////////////////
// File name:   ICAConnection_win.hpp
// Version:     0.0
// Purpose: 
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com> 
// Licence:     GPL v3 
///////////////////////////////////////////////////////////////////////////////

#ifndef __ICACONNECTION__HPP__
#define __ICACONNECTION__HPP__


#include "BasicConnection.hpp"
#include "wfica.tlh"

wchar_t * AnsiToUnicode(const char * buffer);
/*
#define RDP_EVENT_CONNECTED 2
#define RDP_EVENT_DISCONNECTED 4
#define RDP_EVENT_ENTERFULLSCREEN 5
#define RDP_EVENT_LEAVEFULLSCREEN 6
*/

#define ICA_EVENT_CONNECTED 3
#define ICA_EVENT_DISCONNECTED 7
#define ICA_EVENT_ENTERFULLSCREEN 0x1b
#define ICA_EVENT_LEAVEFULLSCREEN 0x16

class ICAConnection;
/*
class CustomFrame : public wxFrame
{
public:
	CustomFrame(wxRDP * rdp_main, wxWindow * parent,wxWindowID id,const wxString& title, const wxPoint& pt = wxDefaultPosition , const wxSize& size = wxDefaultSize);
	
	wxRDP * rdp;
	void on_close(wxCloseEvent& event);

	DECLARE_EVENT_TABLE()
};
*/
class ICAConnection : public BasicConnection
{
public:
	 ICAConnection(MainFrame * main,Options_HashMap options ,wxWindow * parent , wxWindowID id = -1,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxPanelNameStr);

	~ICAConnection();

	//wxActiveXContainer * cnt;
	//const MSTSCLib::IMsRdpClientPtr pRDP; 
	//const WFICALib::IICAClientPtr pICA;
	//RDPConn rdpconn; 
	//Main_Frame * main_frame;

	virtual bool DoConnection();
	virtual bool Connect();
	virtual void Disconnect();
	virtual void FullScreen(BOOL bRestore);

	virtual void SendKey(BOOL cad);
	virtual void GrabAll(BOOL state);
	virtual void CentreConnection();
	virtual bool DisconnectClose();
	//void SendKey(BOOL cad = TRUE);
	//void photo();

	//void EjectCnt();
	//void InjectCnt();
	//void ChangeViewMode();
	//BOOL bInjected;

	//BOOL bConnected;
	//BOOL bFullScreen;
	//BOOL bWaitFlag;
	//BOOL bNeedReconnect;

	void on_enter_focus(wxFocusEvent& event);

//	CustomFrame * frame_rdp;
	//wxString Info;
	//int info_uniq_name;
	
	//static DWORD WINAPI TestHost(wxRDP * rdp);
	//HANDLE hTestHostThread;

	//wxSplitterRDP * splitter_rdp;
	//wxBitmap m_screenshot;
	//void screenshot(wxRDP * rdp);
	

	void DispatcherActiveX(wxActiveXEvent& event);

	
	static DWORD WINAPI TestHost(ICAConnection * ica);
	HANDLE hTestHostThread;

private:
	Options_HashMap local_options;
	//LPTSTR wxStringToLPTSTR(wxString str);
	DECLARE_EVENT_TABLE()

};


#endif //__ICACONNECTION__HPP__
