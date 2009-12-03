///////////////////////////////////////////////////////////////////////////////
// File name: bencoded.hpp
// Date create: Mon Sep 14 17:57:32 2009
// Version: 0.0
// Time-stamp: "2009-09-18 15:53:22" 
// E-mail: 
// Content-Type: text/plain; charset=utf8
// $Id$ 
// Description: 
// 
// 
// 
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


WX_DECLARE_OBJARRAY(RDPConn,base_conn);

class CryptSettings
{
public: 
	CryptSettings();
	CryptSettings(wxString Path);
	void WriteKey(wxString Path);
	
	void create_context_simple();
	void create_context_aes();

	char * SimpleCryptData(char * data, int * size);
	char * AESEncryptData(char * data, int * size);
	char * AESDecryptData(char * data, int * size);

private:

	wxString Path;
	wxString base_key;
	BYTE real_key[256];
	EVP_CIPHER_CTX en_aes;
	EVP_CIPHER_CTX de_aes;
	bool state;
};


class Benc
{
public:
	Benc(){}; 
	~Benc(){};

	int generate_uniq_name(base_conn * bc = NULL);

	void Add(base_conn * bc, const RDPConn rdpconn);
	void Delete(base_conn * bc, int num);
	void Delete(base_conn * bc, RDPConn rdpc);
	int Count(base_conn * bc);
	RDPConn Get(base_conn * bc, int line_number);
	int Find(base_conn * bc, int uniq_name);
	
	bool Load(base_conn * bc);
	bool Save(base_conn * bc);

	BOOL LoadProgramSettings(programsettings * ps);
	BOOL SaveProgramSettings(programsettings * ps);
private:

	wxString GetString(const RDPConn rdpc);
	wxString GetParamStr(wxString name, wxString param);  
	wxString GetParamInt(wxString name, int param);  
	int NumberCount(unsigned int val);
	int ByteLen(wxString str);
	wxString ConvertToUTF8(wxString str);
	wxString ConvertFromUTF8(const char * utf8);
	int iCurrSize;

	int FileCount(wxString filename);
	BOOL isNormalLine(wxFile * fBase, int pos);
	wxString FileGetString(wxString filename, int num);

	RDPConn GetRDPConn(wxString str);
	BOOL GetOptions(wxString str, int pos, RDPConn * rdp_conn);
	BOOL isNormalParam(wxString str, int pos);

	wxString sFilename;

	
	BOOL GetProgramSettings(const wxString str_set, programsettings * ps);
	BOOL GetProgramOptions(wxString str, int pos, programsettings * ps);
	
};


#endif
