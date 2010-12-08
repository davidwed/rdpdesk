///////////////////////////////////////////////////////////////////////////////
// File name:   locale_helper.hpp
// Version:     0.0
// Purpose:
// Time-stamp:  "2010-12-02 19:16:50"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef LOCALEHELPER
#define LOCALEHELPER

#include "tree_group.hpp"


class LocaleHelper
{
public:
	LocaleHelper();
	~LocaleHelper();

	wxArrayString GetAvailableList();
	wxArrayString GetNameList(const wxArrayString short_list);

	BOOL_L SetLang(wxString lang_name, wxLocale * locale);
	BOOL_L CheckLang(wxString lang_name);
};


















#endif // LOCALEHELPER
