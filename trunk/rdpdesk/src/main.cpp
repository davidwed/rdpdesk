///////////////////////////////////////////////////////////////////////////////
// File name: main.cpp
// Date create: Thu Jul 23 18:46:31 2009
// Version: 0.0
// Time-stamp: "2009-11-10 21:31:57" 
// E-mail: pkolobaev@solovat.net
// Content-Type: text/plain; charset=utf8
// $Id$ 
// Description: 
// 
// 
// 
///////////////////////////////////////////////////////////////////////////////

#include "main.hpp"


DECLARE_APP(RDP_Core)

IMPLEMENT_APP(RDP_Core)

bool RDP_Core::OnInit()
{
	
	wxLocale locale;

    int frame_width;
	int frame_heigth;
	wxDisplaySize(&frame_width,&frame_heigth);
		
	frame_width *= 0.85;
	frame_heigth *= 0.88;
		
	programsettings ps = load_main_settings();
	LocaleHelper lh;
	lh.SetLang(ps.lang ,&locale);

    main_frame = new Main_Frame(TS_NAME ,wxPoint(50,50), wxSize(frame_width,frame_heigth), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN );
    main_frame->Show(true); 
    return true;  
}




#ifdef __WXMSW__

wchar_t * AnsiToUnicode(const char * buffer)
{
	int size = MultiByteToWideChar(CP_ACP,0,buffer,-1,0,0);
	wchar_t * wch = new wchar_t [size];
	MultiByteToWideChar(CP_ACP,0,buffer,-1,wch,size);
	return wch;
}
#endif

programsettings load_main_settings()
{
	programsettings ps;
	Benc bc;
	bc.LoadProgramSettings(&ps);
		
	return ps;
}

