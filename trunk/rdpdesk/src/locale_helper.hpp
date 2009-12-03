#ifndef LOCALEHELPER
#define LOCALEHELPER

#include "tree_group.hpp"


class LocaleHelper
{
public:
	LocaleHelper();
	~LocaleHelper();

	wxArrayString GetAvailableList();
	wxArrayString GetNameList(wxArrayString short_list);

	BOOL SetLang(wxString lang_name, wxLocale * locale);
	BOOL CheckLang(wxString lang_name);
};


















#endif // LOCALEHELPER