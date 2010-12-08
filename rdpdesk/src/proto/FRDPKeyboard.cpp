///////////////////////////////////////////////////////////////////////////////
// File name:   FRDPKeyboard.cpp
// Version:     0.0
// Time-stamp:  "2010-11-30 21:58:50"
// E-mail:      rdpdesk@rdpdesk.com
// $Id$
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#include "FRDPKeyboard.hpp"

FRDPKeyboard::FRDPKeyboard()
{

}
FRDPKeyboard::~FRDPKeyboard()
{

}

bool FRDPKeyboard::Init()
{
   kbdList[wxT("0x00000401")] = wxT("Arabic (101)");
   kbdList[wxT("0x00000402")] = wxT("Bulgarian");
   kbdList[wxT("0x00000404")] = wxT("Chinese (Traditional) - US Keyboard");
   kbdList[wxT("0x00000405")] = wxT("Czech");
   kbdList[wxT("0x00000406")] = wxT("Danish");
   kbdList[wxT("0x00000407")] = wxT("German");
   kbdList[wxT("0x00000408")] = wxT("Greek");
   kbdList[wxT("0x00000409")] = wxT("US");
   kbdList[wxT("0x0000040A")] = wxT("Spanish");
   kbdList[wxT("0x0000040B")] = wxT("Finnish");
   kbdList[wxT("0x0000040C")] = wxT("French");
   kbdList[wxT("0x0000040D")] = wxT("Hebrew");
   kbdList[wxT("0x0000040E")] = wxT("Hungarian");
   kbdList[wxT("0x0000040F")] = wxT("Icelandic");
   kbdList[wxT("0x00000410")] = wxT("Italian");
   kbdList[wxT("0x00000411")] = wxT("Japanese");
   kbdList[wxT("0x00000412")] = wxT("Korean");
   kbdList[wxT("0x00000413")] = wxT("Dutch");
   kbdList[wxT("0x00000414")] = wxT("Norwegian");
   kbdList[wxT("0x00000415")] = wxT("Polish (Programmers)");
   kbdList[wxT("0x00000416")] = wxT("Portuguese (Brazilian ABNT)");
   kbdList[wxT("0x00000418")] = wxT("Romanian");
   kbdList[wxT("0x00000419")] = wxT("Russian");
   kbdList[wxT("0x0000041A")] = wxT("Croatian");
   kbdList[wxT("0x0000041B")] = wxT("Slovak");
   kbdList[wxT("0x0000041C")] = wxT("Albanian");
   kbdList[wxT("0x0000041D")] = wxT("Swedish");
   kbdList[wxT("0x0000041E")] = wxT("Thai Kedmanee");
   kbdList[wxT("0x0000041F")] = wxT("Turkish Q");
   kbdList[wxT("0x00000420")] = wxT("Urdu");
   kbdList[wxT("0x00000422")] = wxT("Ukrainian");
   kbdList[wxT("0x00000423")] = wxT("Belarusian");
   kbdList[wxT("0x00000424")] = wxT("Slovenian");
   kbdList[wxT("0x00000425")] = wxT("Estonian");
   kbdList[wxT("0x00000426")] = wxT("Latvian");
   kbdList[wxT("0x00000427")] = wxT("Lithuanian IBM");
   kbdList[wxT("0x00000429")] = wxT("Farsi");
   kbdList[wxT("0x0000042A")] = wxT("Vietnamese");
   kbdList[wxT("0x0000042B")] = wxT("Armenian Eastern");
   kbdList[wxT("0x0000042C")] = wxT("Azeri Latin");
   kbdList[wxT("0x0000042F")] = wxT("FYRO Macedonian");
   kbdList[wxT("0x00000437")] = wxT("Georgian");
   kbdList[wxT("0x00000438")] = wxT("Faeroese");
   kbdList[wxT("0x00000439")] = wxT("Devanagari - INSCRIPT");
   kbdList[wxT("0x0000043A")] = wxT("Maltese 47-key");
   kbdList[wxT("0x0000043B")] = wxT("Norwegian with Sami");
   kbdList[wxT("0x0000043F")] = wxT("Kazakh");
   kbdList[wxT("0x00000440")] = wxT("Kyrgyz Cyrillic");
   kbdList[wxT("0x00000444")] = wxT("Tatar");
   kbdList[wxT("0x00000445")] = wxT("Bengali");
   kbdList[wxT("0x00000446")] = wxT("Punjabi");
   kbdList[wxT("0x00000447")] = wxT("Gujarati");
   kbdList[wxT("0x00000449")] = wxT("Tamil");
   kbdList[wxT("0x0000044A")] = wxT("Telugu");
   kbdList[wxT("0x0000044B")] = wxT("Kannada");
   kbdList[wxT("0x0000044C")] = wxT("Malayalam");
   kbdList[wxT("0x0000044E")] = wxT("Marathi");
   kbdList[wxT("0x00000450")] = wxT("Mongolian Cyrillic");
   kbdList[wxT("0x00000452")] = wxT("United Kingdom Extended");
   kbdList[wxT("0x0000045A")] = wxT("Syriac");
   kbdList[wxT("0x00000461")] = wxT("Nepali");
   kbdList[wxT("0x00000463")] = wxT("Pashto");
   kbdList[wxT("0x00000465")] = wxT("Divehi Phonetic");
   kbdList[wxT("0x0000046E")] = wxT("Luxembourgish");
   kbdList[wxT("0x00000481")] = wxT("Maori");
   kbdList[wxT("0x00000804")] = wxT("Chinese (Simplified) - US Keyboard");
   kbdList[wxT("0x00000807")] = wxT("Swiss German");
   kbdList[wxT("0x00000809")] = wxT("United Kingdom");
   kbdList[wxT("0x0000080A")] = wxT("Latin American");
   kbdList[wxT("0x0000080C")] = wxT("Belgian French");
   kbdList[wxT("0x00000813")] = wxT("Belgian (Period)");
   kbdList[wxT("0x00000816")] = wxT("Portuguese");
   kbdList[wxT("0x0000081A")] = wxT("Serbian (Latin)");
   kbdList[wxT("0x0000082C")] = wxT("Azeri Cyrillic");
   kbdList[wxT("0x0000083B")] = wxT("Swedish with Sami");
   kbdList[wxT("0x00000843")] = wxT("Uzbek Cyrillic");
   kbdList[wxT("0x0000085D")] = wxT("Inuktitut Latin");
   kbdList[wxT("0x00000C0C")] = wxT("Canadian French (legacy)");
   kbdList[wxT("0x00000C1A")] = wxT("Serbian (Cyrillic)");
   kbdList[wxT("0x00001009")] = wxT("Canadian French");
   kbdList[wxT("0x0000100C")] = wxT("Swiss French");
   kbdList[wxT("0x0000141A")] = wxT("Bosnian");
   kbdList[wxT("0x00001809")] = wxT("Irish");
   kbdList[wxT("0x0000201A")] = wxT("Bosnian Cyrillic");
   kbdList[wxT("0x00010401")] = wxT("Arabic (102)");
   kbdList[wxT("0x00010402")] = wxT("Bulgarian (Latin)");
   kbdList[wxT("0x00010405")] = wxT("Czech (QWERTY)");
   kbdList[wxT("0x00010407")] = wxT("German (IBM)");
   kbdList[wxT("0x00010408")] = wxT("Greek (220)");
   kbdList[wxT("0x00010409")] = wxT("United States-Dvorak");
   kbdList[wxT("0x0001040A")] = wxT("Spanish Variation");
   kbdList[wxT("0x0001040E")] = wxT("Hungarian 101-key");
   kbdList[wxT("0x00010410")] = wxT("Italian (142)");
   kbdList[wxT("0x00010415")] = wxT("Polish (214)");
   kbdList[wxT("0x00010416")] = wxT("Portuguese (Brazilian ABNT2)");
   kbdList[wxT("0x00010419")] = wxT("Russian (Typewriter)");
   kbdList[wxT("0x0001041B")] = wxT("Slovak (QWERTY)");
   kbdList[wxT("0x0001041E")] = wxT("Thai Pattachote");
   kbdList[wxT("0x0001041F")] = wxT("Turkish F");
   kbdList[wxT("0x00010426")] = wxT("Latvian (QWERTY)");
   kbdList[wxT("0x00010427")] = wxT("Lithuanian");
   kbdList[wxT("0x0001042B")] = wxT("Armenian Western");
   kbdList[wxT("0x00010439")] = wxT("Hindi Traditional");
   kbdList[wxT("0x0001043A")] = wxT("Maltese 48-key");
   kbdList[wxT("0x0001043B")] = wxT("Sami Extended Norway");
   kbdList[wxT("0x00010445")] = wxT("Bengali (Inscript)");
   kbdList[wxT("0x0001045A")] = wxT("Syriac Phonetic");
   kbdList[wxT("0x00010465")] = wxT("Divehi Typewriter");
   kbdList[wxT("0x0001080C")] = wxT("Belgian (Comma)");
   kbdList[wxT("0x0001083B")] = wxT("Finnish with Sami");
   kbdList[wxT("0x00011009")] = wxT("Canadian Multilingual Standard");
   kbdList[wxT("0x00011809")] = wxT("Gaelic");
   kbdList[wxT("0x00020401")] = wxT("Arabic (102) AZERTY");
   kbdList[wxT("0x00020405")] = wxT("Czech Programmers");
   kbdList[wxT("0x00020408")] = wxT("Greek (319)");
   kbdList[wxT("0x00020409")] = wxT("United States-International");
   kbdList[wxT("0x0002041E")] = wxT("Thai Kedmanee (non-ShiftLock)");
   kbdList[wxT("0x0002083B")] = wxT("Sami Extended Finland-Sweden");
   kbdList[wxT("0x00030408")] = wxT("Greek (220) Latin");
   kbdList[wxT("0x00030409")] = wxT("United States-Dvorak for left hand");
   kbdList[wxT("0x0003041E")] = wxT("Thai Pattachote (non-ShiftLock)");
   kbdList[wxT("0x00040408")] = wxT("Greek (319) Latin");
   kbdList[wxT("0x00040409")] = wxT("United States-Dvorak for right hand");
   kbdList[wxT("0x00050408")] = wxT("Greek Latin");
   kbdList[wxT("0x00050409")] = wxT("US English Table for IBM Arabic 238_L");
   kbdList[wxT("0x00060408")] = wxT("Greek Polytonic");
   kbdList[wxT("0xB0000407")] = wxT("German Neo");
   kbdList[wxT("0xE0010404")] = wxT("Chinese (Traditional) - Phonetic");
   kbdList[wxT("0xE0010411")] = wxT("Japanese Input System (MS-IME2002)");
   kbdList[wxT("0xE0010412")] = wxT("Korean Input System (IME 2000)");
   kbdList[wxT("0xE0010804")] = wxT("Chinese (Simplified) - QuanPin");
   kbdList[wxT("0xE0020404")] = wxT("Chinese (Traditional) - ChangJie");
   kbdList[wxT("0xE0020804")] = wxT("Chinese (Simplified) - ShuangPin");
   kbdList[wxT("0xE0030404")] = wxT("Chinese (Traditional) - Quick");
   kbdList[wxT("0xE0030804")] = wxT("Chinese (Simplified) - ZhengMa");
   kbdList[wxT("0xE0040404")] = wxT("Chinese (Traditional) - Big5 Code");
   kbdList[wxT("0xE0050404")] = wxT("Chinese (Traditional) - Array");
   kbdList[wxT("0xE0050804")] = wxT("Chinese (Simplified) - NeiMa");
   kbdList[wxT("0xE0060404")] = wxT("Chinese (Traditional) - DaYi");
   kbdList[wxT("0xE0070404")] = wxT("Chinese (Traditional) - Unicode");
   kbdList[wxT("0xE0080404")] = wxT("Chinese (Traditional) - New Phonetic");
   kbdList[wxT("0xE0090404")] = wxT("Chinese (Traditional) - New ChangJie");
   kbdList[wxT("0xE00E0804")] = wxT("Chinese (Traditional) - Microsoft Pinyin IME 3.0");
   kbdList[wxT("0xE00F0404")] = wxT("Chinese (Traditional) - Alphanumeric");
   return true;
}

int FRDPKeyboard::GetCount()
{
   return kbdList.size();
}
wxString FRDPKeyboard::GetKeyNameByIndex(const int index)
{
   KeyboardList::iterator itrt;
   wxString result;
   int localIndex = 0;
   for (itrt = kbdList.begin(); itrt != kbdList.end(); ++itrt )
   {
      if (localIndex == index)
      {
	 result = itrt->first;
	 break;
      }
      ++localIndex;
   }
   return result;
}

wxString FRDPKeyboard::GetKeyNameByValue(const wxString value)
{
   KeyboardList::iterator itrt;
   wxString result;
   int localIndex = 0;
   for (itrt = kbdList.begin(); itrt != kbdList.end(); ++itrt )
   {
      if (value == itrt->second)
      {
	 result = itrt->first;
	 break;
      }
      ++localIndex;
   }
   return result;

}
wxString FRDPKeyboard::GetValueByIndex(const int index)
{
   KeyboardList::iterator itrt;
   wxString result;
   int localIndex = 0;
   for (itrt = kbdList.begin(); itrt != kbdList.end(); ++itrt )
   {
      if (localIndex == index)
      {
	 result = itrt->second;
	 break;
      }
      ++localIndex;
   }
   return result;

}
wxString FRDPKeyboard::GetValueByKey(const wxString key)
{
   KeyboardList::iterator itrt;
   wxString result;
   int localIndex = 0;

   for (itrt = kbdList.begin(); itrt != kbdList.end(); ++itrt )
   {
      if (key == itrt->first)
      {
	 result = itrt->second;
	 break;
      }
      ++localIndex;
   }
   return result;
}
