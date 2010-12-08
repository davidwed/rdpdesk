#ifndef _RFB_CONNECTION_H
#define _RFB_CONNECTION_H
#include "BasicConnection.hpp"
#include "UltraVncAx.h"

wchar_t * AnsiToUnicode(const char * buffer);

#define RFB_EVENT_CONNECTED 1
#define RFB_EVENT_DISCONNECTED 2


class RFBFullScreen;

class RFBConnection : public BasicConnection
{
public:
	RFBConnection(MainFrame *main,Options_HashMap options ,wxWindow * parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxPanelNameStr);
	~RFBConnection();

	virtual bool DoConnection();
	virtual bool Connect();
	virtual void Disconnect();
	virtual void FullScreen(BOOL bRestore);
	virtual void SendKey(BOOL cad);
	virtual void GrabAll(BOOL state);
	virtual void CentreConnection();
	virtual bool DisconnectClose();
	void DispatcherActiveX(wxActiveXEvent& event);
	void on_enter_focus(wxFocusEvent& event);
	RFBFullScreen * fullscreen_frame;
	
private:
	bool inner_status;
	//static DWORD WINAPI TestHost(RFBConnection* rfb);

	DECLARE_EVENT_TABLE()
private:
	Options_HashMap local_options;
	IUltraVncAxObj *vncax;
};

class RFBFullScreen : public wxTopLevelWindow
{
public:
	RFBFullScreen(wxWindow *cnt, Options_HashMap options);
	~RFBFullScreen();
private:
	RFBConnection *rfb;
	Options_HashMap local_options;
	wxWindow *cnt_old;
	wxWindow *old_window;
	wxScrolledWindow * win;
	ToolbarCnt * win2;
	FullScreenToolBar * tb;

	void on_enter_focus(wxFocusEvent& event);
	void on_toolbar(wxCommandEvent& event);
	void Leave();
	DECLARE_EVENT_TABLE();
};

#endif