///////////////////////////////////////////////////////////////////////////////
// File name:   bencoded.hpp
// Version:     0.0
// Purpose:
// E-mail:      rdpdesk@rdpdesk.com
// Copyright:   (c) 2009-2010 RDPDesk <rdpdesk@rdpdesk.com>
// Licence:     GPL v3
///////////////////////////////////////////////////////////////////////////////

#ifndef BENCODED_HPP
#define BENCODED_HPP

#include <wx/wx.h>
#include <wx/file.h>
#include <wx/ffile.h>
#include <wx/textfile.h>
#include <wx/busyinfo.h>

#include <wx/arrimpl.cpp>

#include <openssl/aes.h>
#include <openssl/evp.h>

#include "rdpbasedefs.hpp"
//#include "main.hpp"
#include "proto.hpp"

//WX_DECLARE_OBJARRAY(options_struct, GeneralOptionsArray);
WX_DECLARE_OBJARRAY(RDPConn,base_conn);
//WX_DECLARE_OBJARRAY(connection_setting, OptionsArray);

//WX_DEFINE_OBJARRAY(GeneralOptionsArray);
class CryptSettings
{
public:
   CryptSettings();
   CryptSettings(const wxString &Path);
   void WriteKey(wxString Path);

   void create_context_simple();
   void create_context_aes();

   char * SimpleCryptData(char * data, int * size);
   char * AESEncryptData(char * data, int * size);
   char * AESDecryptData(char * data, int * size);

private:

   wxString localPath;
   wxString base_key;
   BYTE_L real_key[256];
   EVP_CIPHER_CTX en_aes;
   EVP_CIPHER_CTX de_aes;
   bool state;
};


class Benc
{
public:
   Benc(){};
   ~Benc(){};

   int generate_uniq_name(base_conn * bc);
   int generate_uniq_name();
   void Add(base_conn * bc, const RDPConn rdpconn);
   void Add(Connections_List *all_connection_records, const OptionsArray options);
   void Delete(base_conn * bc, int num);
   void Delete(base_conn * bc, RDPConn rdpc);
   int Count(base_conn * bc);
   RDPConn Get(base_conn * bc, int line_number);
//   Options_HashMap Get_Options(Options_HashMap *all_options, int line_number);
   Options_HashMap Get_Parsed_Options(wxString str, int pos);
   Options_HashMap Get_Tree_Options(wxString str);

   int Find(base_conn * bc, int uniq_name);
   bool Load(base_conn * bc);
   Connections_List Load();
   bool Save(base_conn * bc);
   bool Save(Connections_List *all_connection_records);
   BOOL_L LoadProgramSettings(programsettings * ps);
   BOOL_L SaveProgramSettings(programsettings * ps);
private:

   wxString GetString(const RDPConn rdpc);
   wxString GetString(const Options_HashMap *all_options);
   wxString GetParamStr(wxString name, wxString param);
   wxString GetParamInt(wxString name, int param);
   int NumberCount(unsigned int val);
   int ByteLen(wxString str);
   wxString ConvertToUTF8(wxString str);
   wxString ConvertFromUTF8(const char * utf8);
   int iCurrSize;

   int FileCount(wxString filename);
   BOOL_L isNormalLine(wxFile * fBase, int pos);
   wxString FileGetString(wxString filename, int num);

   RDPConn GetRDPConn(wxString str);
   BOOL_L GetOptions(wxString str, int pos, RDPConn * rdp_conn);
   BOOL_L isNormalParam(wxString str, int pos);

   wxString sFilename;


   BOOL_L GetProgramSettings(const wxString str_set, programsettings * ps);
   BOOL_L GetProgramOptions(wxString str, int pos, programsettings * ps);


};


#endif
