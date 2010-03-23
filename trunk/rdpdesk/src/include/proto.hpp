///////////////////////////////////////////////////////////////////////////////
// File name:   proto.hpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-03-21 21:39:17"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef PROTO
#define PROTO

#include <wx/wx.h>
#include <wx/notebook.h>
//#include <wx/dynarray.h>
#include <wx/arrimpl.cpp>


struct options_struct
{
  wxString type;
  wxString name;
  wxString value;
};

//WX_DECLARE_HASH_MAP( wxString name, wxString value, wxString type, int, MyHash1 );
//WX_DECLARE_STRING_HASH_MAP( int, MyHash5 );

WX_DECLARE_STRING_HASH_MAP( wxString, Options_HashMap );

WX_DECLARE_OBJARRAY(options_struct, GeneralOptionsArray);

WX_DECLARE_OBJARRAY(GeneralOptionsArray, OptionsArray);

WX_DECLARE_OBJARRAY(Options_HashMap, Connections_List);

#endif //PROTO

