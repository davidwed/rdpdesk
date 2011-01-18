///////////////////////////////////////////////////////////////////////////////
// File name:   locale_helper.cpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include "locale_helper.hpp"
#include <wx/stdpaths.h>

LocaleHelper::LocaleHelper()
{
}

LocaleHelper::~LocaleHelper()
{
}

wxArrayString LocaleHelper::GetAvailableList()
{
   wxArrayString list;
   list.Clear();

   wxLocale locale;
   wxString localename;

#ifdef __WXMSW__
   wxStandardPaths standartPaths;
   wxString path = wxString::Format(wxT("%s\\lang"),standartPaths.GetResourcesDir() + wxT("\\"));
   //wxMessageBox(standartPaths.GetResourcesDir());
#endif

#ifdef __WXGTK__
   wxString path = wxString::Format(wxT("/usr/share/locale"));
#endif

   wxDir dir(path);
   wxDir subdir;
   if (!dir.IsOpened() || !wxDirExists(path.data()))
   {
      return list;
   }


   bool cont = dir.GetFirst(&localename, wxEmptyString, wxDIR_DIRS);
   while (cont)
   {
      const wxLanguageInfo * lang_info = locale.FindLanguageInfo(localename);
      if (lang_info)
      {
	 wxString subdirpath = wxString::Format(wxT("%s/%s"),path.data(),localename.data());
 	 if (subdir.Open(subdirpath))
 	 {

// #ifdef __WXGTK__
// 	 wxString subdirpath = wxString::Format(wxT("%s/%s"),path.data(),localename.data());
// 	 if (subdir.Open(subdirpath))
// 	 {
// #endif
// #ifdef __WXMSW__
// 	 if (subdir.Open(localename))
// 	 {
// #endif
#ifdef __WXMSW__
	    wxString mo_file = wxString::Format(wxT("%s\\%s\\%s.mo"),
						path,
						localename,
						APPLICATION_LANG_NAME);
#endif
#ifdef __WXGTK__
	    wxString mo_file = wxString::Format(wxT("%s/%s/LC_MESSAGES/%s.mo"),
						path.data(),
						localename.data(),
						APPLICATION_LANG_NAME);
#endif
	    if (wxFileExists(mo_file))
	    {
	       list.Add(localename);
	    }
	 }
      }
      cont = dir.GetNext(&localename);
   }

   list.Sort();
   list.Shrink();
   list.Insert(_("Default"),0);
//   list.Insert(_("English (U.S.)"),1);
   return list;
}


wxArrayString LocaleHelper::GetNameList(const wxArrayString short_list)
{
   wxLocale locale;
   wxArrayString list;
   list.Clear();

   for (int i = 1; i < (int)short_list.Count(); i++)
   {
      const wxLanguageInfo * lang_info = locale.FindLanguageInfo(short_list.Item(i));
      if (lang_info)
      {
	 list.Add(lang_info->Description);
      }
   }

   list.Sort();
   list.Shrink();
   list.Insert(_("Default"),0);
   list.Insert(_("English (U.S.)"),1);
   return list;
}


BOOL_L LocaleHelper::CheckLang(wxString lang_name)
{

#ifdef __WXMSW__
   wxStandardPaths standartPaths;
   wxString path = wxString::Format(wxT("%s\\lang"),standartPaths.GetResourcesDir() + wxT("\\"));
   wxString mo_file = wxString::Format(wxT("%s\\%s\\%s.mo"),path,lang_name,APPLICATION_LANG_NAME);
#endif
#ifdef __WXGTK__
   wxString path = wxString::Format(wxT("/usr/share/locale"));
   wxString mo_file = wxString::Format(wxT("%s/%s/LC_MESSAGES/%s.mo"),path.data(),lang_name.data(),APPLICATION_LANG_NAME);
#endif

   if (wxFileExists(mo_file))
   {
	  return TRUE;
   }
	return FALSE;
}

BOOL_L LocaleHelper::SetLang(wxString lang_name, wxLocale * locale)
{

   if (lang_name == wxT("Default"))
   {
      lang_name = locale->GetLanguageName(locale->GetSystemLanguage());
   }
   if (lang_name == wxT("English (U.S.)"))
   {
	   return TRUE;
   }

   const wxLanguageInfo * lang_info = locale->FindLanguageInfo(lang_name);
   if (!lang_info)
   {
      return FALSE;
   }
#ifdef __WXMSW__
   if (!CheckLang(lang_info->CanonicalName))
   {

      return FALSE;
   }
#endif
   const int lang_id = lang_info->Language;
   if (!wxLocale::IsAvailable(lang_id))
   {
      return FALSE;
   }



   if(locale->Init(lang_id, wxLOCALE_CONV_ENCODING))
   {

#ifdef __WXMSW__
	  wxStandardPaths standartPaths;
	  wxString path = wxString::Format(wxT("%s\\lang"),standartPaths.GetResourcesDir() + wxT("\\"));
#endif

#ifdef __WXGTK__
      //wxString path =
      //wxString::Format(wxT("/usr/share/locale/ru/LC_MESSAGES"));
      wxString path = wxString::Format(wxT("/usr/share/locale"));
      locale->AddCatalogLookupPathPrefix(wxT("/usr"));
      locale->AddCatalogLookupPathPrefix(wxT("/usr/local"));
      wxStandardPaths* paths = (wxStandardPaths*) &wxStandardPaths::Get();
      wxString prefix = paths->GetInstallPrefix();
      locale->AddCatalogLookupPathPrefix( prefix );
#endif
      locale->AddCatalogLookupPathPrefix(path);
      locale->AddCatalog(APPLICATION_LANG_NAME);

      if (locale->IsOk())
      {
	 return TRUE;
      }
      else
      {
	 return FALSE;
      }
   }
   return FALSE;
}
