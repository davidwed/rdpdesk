///////////////////////////////////////////////////////////////////////////////
// File name:   FRDPKeyboard.hpp
// Version:     0.0
// Time-stamp:  "2010-11-30 20:52:04"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef __FRDPKEYBOARD__
#define __FRDPKEYBOARD__

#include <wx/wx.h>
#include <wx/arrimpl.cpp>
#include "defineds.hpp"

WX_DECLARE_STRING_HASH_MAP(wxString, KeyboardList);


class FRDPKeyboard
{
public:
   FRDPKeyboard();
   ~FRDPKeyboard();
   int GetCount();
   bool Init();
   wxString GetKeyNameByIndex(const int index);
   wxString GetKeyNameByValue(const wxString value);
   wxString GetValueByIndex(const int index);
   wxString GetValueByKey(const wxString key);
private:
   KeyboardList kbdList;

};

#endif // __FRDPKKEYBOARD__
