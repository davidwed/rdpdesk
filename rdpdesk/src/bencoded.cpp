///////////////////////////////////////////////////////////////////////////////
// File name: bencoded.cpp
// Date create: Mon Sep 14 17:59:16 2009
// Version: 0.0
// Time-stamp: "2009-11-05 12:30:23" 
// E-mail: 
// Content-Type: text/plain; charset=utf8
// $Id$ 
// Description: 
// 
// 
// 
///////////////////////////////////////////////////////////////////////////////

#include "bencoded.hpp"

WX_DEFINE_OBJARRAY(base_conn);
////////////////////////////////////////////////////// 

CryptSettings::CryptSettings()
{
	long lt = wxGetLocalTime();
	srand((unsigned)time(NULL));
	
	char random;
	for (int i = 0; i < 256; i ++)
	{	
		random = (char)(rand() % 256);
		random ^= (lt*(i+1) % 128);
		base_key.Append(random);
	}
	state = false;
}

CryptSettings::CryptSettings(wxString Path)
{
	wxFile key_file;
	key_file.Open(Path);
	int filelenght = (int)key_file.SeekEnd();
	key_file.Seek(0);
	char * key_buff = new char[filelenght];
	key_file.Read(key_buff, filelenght);
	base_key = wxString::From8BitData(key_buff,filelenght); 
	delete key_buff;
	this->Path = Path;
	key_file.Close(); 
	key_file.Detach(); 
	state = false;
}

void CryptSettings::WriteKey(wxString Path)
{
	wxFile key_file;
	key_file.Open(Path,wxFile::write);
	key_file.Write(base_key.To8BitData(),base_key.Length());
	key_file.Close(); 
	this->Path = Path;
}
	
void CryptSettings::create_context_simple()
{
	BYTE * key = (BYTE *)base_key.GetData(); 
	int keylen = 256;
	BYTE Sbox[256];
	BYTE temp;
	for (int i = 0; i < 256; i ++)
	{
		Sbox[i] = i;
	}
	for (int i = 0, j = 0; i < 256; i ++)
	{
		j = (j + Sbox[i] + key[i % keylen]) & 255;
		temp = Sbox[i];
		Sbox[i] = Sbox[j];
		Sbox[j] = temp;
	}

	int i = 0;
	int j = 0;
	for (int k = 0; k < 256; k ++)
	{
		i = (i + 1) & 255;
		j = (j + Sbox[i]) & 255;
		temp = Sbox[i];
		Sbox[i] = Sbox[j];
		Sbox[j] = Sbox[i];
		int pos = (Sbox[i] + Sbox[j])&255;
		real_key[i] = Sbox[pos];
	}
	state = true;

}
void CryptSettings::create_context_aes()
{
	unsigned int keygen[] = {20000, 30000}; //{12345, 54321};
	
	int i, nrounds = 5;
	unsigned char key[32], iv[32];
  
	i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), (unsigned char *)&keygen, (unsigned char *)base_key.GetData(), 256, nrounds, key, iv);
	if (i != 32)
	{
		state = false;
		return;
	}

	EVP_CIPHER_CTX_init(&en_aes);
	EVP_EncryptInit_ex(&en_aes, EVP_aes_256_cbc(), NULL, key, iv);
	EVP_CIPHER_CTX_init(&de_aes);
	EVP_DecryptInit_ex(&de_aes, EVP_aes_256_cbc(), NULL, key, iv);

	state = true;
}

char * CryptSettings::SimpleCryptData(char * data, int * len)
{
	if (!len || data == NULL || !state) return NULL;
	int _len = *len;
	unsigned char * returndata = (unsigned char *)malloc(_len);
	for (int i = 0; i < _len; i ++)
	{
		unsigned char tmp = (BYTE)data[i];
		tmp ^= real_key[i % 256];
		returndata[i] = tmp;
	}
	return (char *)returndata;

}
char * CryptSettings::AESEncryptData(char * data, int * len)
{
	if (!len || data == NULL || !state) return NULL;
	
	int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
	unsigned char *returndata = (unsigned char *)malloc(c_len);
	EVP_EncryptInit_ex(&en_aes, NULL, NULL, NULL, NULL);
	EVP_EncryptUpdate(&en_aes, returndata, &c_len, (unsigned char *)data, *len);
	EVP_EncryptFinal_ex(&en_aes, returndata+c_len, &f_len);

	*len = c_len + f_len;
	return (char *)returndata;
}

char * CryptSettings::AESDecryptData(char * data, int * len)
{
	if (!len || data == NULL || !state) return NULL;

	int p_len = *len, f_len = 0;
	unsigned char *returndata = (unsigned char *)malloc(p_len);
	EVP_DecryptInit_ex(&de_aes, NULL, NULL, NULL, NULL);
	EVP_DecryptUpdate(&de_aes, returndata, &p_len, (unsigned char *)data, *len);
	EVP_DecryptFinal_ex(&de_aes, returndata+p_len, &f_len);
	*len = p_len + f_len;
	return (char *)returndata;
}


int Benc::generate_uniq_name(base_conn * bc)
{
	if (!bc)
	{
		wxDateTime dt = wxDateTime::UNow() ;
		srand((unsigned)time(0));
		return dt.GetTicks() + (rand()) ;
	}
	else
	{
		BOOL ok = FALSE;
		while(!ok)
		{
			ok = TRUE;
			wxDateTime dt = wxDateTime::UNow() ;
			srand((unsigned)time(0));
			int temp = dt.GetTicks() + (rand());
			
			int lc = Count(bc);
			for (int i = 0; i < lc ; i++)
			{
				RDPConn rdpc = Get(bc,i);
				if (rdpc.uniq_name == temp) 
				{
					ok = FALSE;
				}
			}
			if (ok)
			{
				return temp;
			}

		}
		return -1;
	}
}

void Benc::Add(base_conn * bc, const RDPConn rdpconn)
{
	int lc = (int)bc->Count();
	BOOL find = FALSE;
	for (int i = 0; i < lc; i++)
	{
		if (rdpconn.uniq_name == bc->Item(i).uniq_name)
		{
			bc->RemoveAt(i);
			bc->Insert(rdpconn,i);
			find = TRUE;
			break;
		}
	}
	if (!find)
	{
		bc->Add(rdpconn); 
	}
}

void Benc::Delete(base_conn * bc, int num)
{
	bc->RemoveAt(num); 
}

void Benc::Delete(base_conn * bc, RDPConn rdpc)
{
	
}

int Benc::Count(base_conn * bc)
{
	return (int)bc->Count(); 
}

RDPConn Benc::Get(base_conn * bc, int line_number)
{
	RDPConn lrdpconn = bc->Item(line_number);
	return lrdpconn;
}

int Benc::Find(base_conn * bc, int uniq_name)
{
	int lc = (int)bc->Count();
	for (int i = 0; i < lc; i++)
	{
		if (bc->Item(i).uniq_name == uniq_name)
		{
			return i;
		}
	}
	return -1;
}

bool Benc::Save(base_conn * bc)
{
	if (!bc) return FALSE;

	programsettings ps = load_main_settings();
	if (!ps.bAutosave)
	{
		wxMessageDialog dialog(NULL, _T("Rewrite base?"),_T("Program closing..."), wxNO_DEFAULT|wxYES_NO|wxICON_INFORMATION);
		if (dialog.ShowModal() != wxID_YES) return false; 
	}

	wxBusyInfo wait(wxT("Please wait"));
	GETBASEPATH();
	wxString filename = BASEPATH;
	
	wxFile * ftemp = new wxFile();
	ftemp->Create(filename,true);
	ftemp->Close();
	ftemp->Detach();
	delete ftemp;
	
	int lc = (int)bc->Count();

	for (int i = 0; i < lc; i++)
	{
		wxString str = GetString(bc->Item(i));
	}
	return true;
}



bool Benc::Load(base_conn * bc)
{
	wxBusyInfo wait(wxT("Please wait"));


	programsettings ps = load_main_settings();
	if (ps.bUseCrypt) 
	{
		GETKEYPATH()
		if (!wxFileExists(KEYPATH))
		{
			CryptSettings * cr = new CryptSettings();
			cr->WriteKey(KEYPATH);
			delete cr;
		}
	}

	GETBASEPATH();
	wxString filename = BASEPATH;

	if (!wxFileExists(filename))
	{
		return true;
	}
	
	int lc = FileCount(filename);

	for (int i = 0; i < lc ; i++)
	{
		wxString temp = FileGetString(filename,i);
		RDPConn rdpc = GetRDPConn(temp);
		Add(bc,rdpc);
	}
	return true;
}

wxString Benc::FileGetString(wxString filename, int num)
{
	wxString result;
	result.Empty();

	wxFile fBase;
	fBase.Open(filename);
	fBase.SeekEnd();
	DWORD dwFileSize = fBase.Tell();
	if ((dwFileSize == 0) || (dwFileSize == wxInvalidOffset))
	{
		fBase.Close();
		fBase.Detach(); 
		return wxEmptyString;
	}
	
	fBase.Seek(0);
	char buff[8];
		
	int CurrentLine = 0;
	for (int i = 0; i < (int)dwFileSize; i++)
	{
		fBase.Seek(i);
		fBase.Read(buff,1);
		if (buff[0] == 'k')
		{
			if (isNormalLine(&fBase,i))
			{
				if (num == CurrentLine)
				{
					wxString strSize;
					strSize.Empty();
					int iCurrPos = i + 1;
					wxString temp;
			
					memset(buff,0,8);
					BOOL read = TRUE;
					while(read)
					{
						fBase.Seek(iCurrPos);
						fBase.Read(buff,1);
						temp = wxString::Format(wxT("%s"),buff);
						
						if (temp.IsNumber())
						{
							strSize += temp;
							iCurrPos ++;
						}
						else
						{
							read = FALSE;
						}
					}
								
					int iSize = wxAtoi(strSize);
					
					iCurrPos = i + NumberCount(iSize) + 2;
					
					char * tempbuffer = new char[iSize + 1];
					memset(tempbuffer,0,(iSize + 1));
					fBase.Seek(iCurrPos);
					fBase.Read(tempbuffer,iSize);
					
					char * decrypt_data = NULL;
					int crypt_len = iSize;

					CryptSettings * cr = NULL;
					programsettings ps = load_main_settings();
					if (ps.bUseCrypt) 
					{
						GETKEYPATH()
						if (!wxFileExists(KEYPATH))
						{
							cr = new CryptSettings();
							cr->WriteKey(KEYPATH);
							delete cr;
						}
						else
						{
							cr = new CryptSettings(KEYPATH);
							if (ps.iTypeCrypt == 0)
							{
								cr->create_context_simple();
								decrypt_data = cr->SimpleCryptData((char *)tempbuffer,&crypt_len);
							}
							else if (ps.iTypeCrypt == 1)
							{
								cr->create_context_aes();
								decrypt_data = cr->AESDecryptData((char *)tempbuffer,&crypt_len);
							}
							delete cr;
						}
					}
					result.Empty();

					if (decrypt_data != NULL)
					{
						for (int i = 0; i < crypt_len; i++)
						{
							result += decrypt_data[i];
						}
					}
					else
					{
						for (int i = 0; i < iSize; i++)
						{
							result += tempbuffer[i];
						}
					}
					delete tempbuffer;
					fBase.Close();
					fBase.Detach();
					if (decrypt_data != NULL)
					{
						free(decrypt_data);
					}
					return result;
				}
				else
				{
					CurrentLine ++;
				}
			}
			else
			{
				continue;
			}
		}
	}
	if (fBase.IsOpened())
	{
		fBase.Close();
	}
	fBase.Detach();
	return wxT("");
	
}

int Benc::FileCount(wxString filename)
{
	int iResult = 0;

	wxFile fBase;
	fBase.Open(filename);
	fBase.SeekEnd();
	DWORD dwFileSize = fBase.Tell();
	if ((dwFileSize == 0) || (dwFileSize == wxInvalidOffset))
	{
		return -1;
	}
	
	fBase.Seek(0);
	char buff[8];
	
	for (int i = 0; i < (int)dwFileSize; i++)
	{
		fBase.Seek(i);
		fBase.Read(buff,1);
		if (buff[0] == 'k')
		{
			if (isNormalLine(&fBase,i))
			{
				iResult ++;
			}
			else
			{
				continue;
			}
		}
	}

	if (fBase.IsOpened())
	{
		fBase.Close();
		 
	}
	fBase.Detach();
	return iResult;
}

BOOL Benc::isNormalLine(wxFile * fBase, int pos)
{
	wxString strSize;
	strSize.Empty();
	int iCurrPos = pos + 1;
	wxString temp;
	
	char buff[8];
	memset(buff,0,8);
	BOOL read = TRUE;
	while(read)
	{
		fBase->Seek(iCurrPos);
		fBase->Read(buff,1);
		temp = wxString::Format(wxT("%s"),buff); 
		if (temp.IsNumber())
		{
			strSize += temp;
			iCurrPos ++;
		}
		else
		{
			read = FALSE;
		}
	}
	if (strSize.IsEmpty()) 
	{
		return FALSE;
	}
	
	int iSize = wxAtoi(strSize);
	int iEndPos = pos + 2 + NumberCount(iSize) + iSize;
			
	fBase->Seek(iEndPos);
	fBase->Read(buff,2);
	wxString str_end;
	str_end = wxString::Format(wxT("%s"),buff);
	if (!strcmp(buff,"zz"))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

wxString Benc::GetString(const RDPConn rdpc)
{
	RDPConn lrdpconn = rdpc;
	wxString result;
	wxString temp;
	
	iCurrSize = 0;
	result.Empty();
	result += GetParamInt (wxT("uniq_name"), (int)(rdpc.uniq_name));
	result += GetParamStr (wxT("hostname"), (rdpc.hostname));
	result += GetParamStr (wxT("username"), (rdpc.username));
	result += GetParamStr (wxT("password"), (rdpc.password));
	result += GetParamStr (wxT("domain"), (rdpc.domain));
	result += GetParamStr (wxT("port"), (rdpc.port));
	result += GetParamInt (wxT("attach_to_console"), (rdpc.attach_to_console));
	result += GetParamInt (wxT("width"), (int)(rdpc.width));
	result += GetParamInt (wxT("heigth"), (int)(rdpc.heigth));
	result += GetParamInt (wxT("color_depth"), (int)(rdpc.color_depth));
	result += GetParamInt (wxT("bSmartSizing"), (rdpc.bSmartSizing));
	result += GetParamInt (wxT("bControlSize"), (rdpc.bControlSize));
	result += GetParamInt (wxT("bFullScreen"), (rdpc.bFullScreen));
	result += GetParamInt (wxT("force_update_screen"), (rdpc.force_update_screen));
	result += GetParamStr (wxT("shell"), (rdpc.shell));
	result += GetParamStr (wxT("directory"), (rdpc.directory));
	result += GetParamInt (wxT("bUseProgram"), (rdpc.bUseProgram));
	result += GetParamInt (wxT("bProgramMaximized"), (rdpc.bProgramMaximized));
	result += GetParamStr (wxT("keyboard_map"), (rdpc.keyboard_map)); 
	result += GetParamInt (wxT("keyboard"), (rdpc.keyboard));
	result += GetParamInt (wxT("bEnableBitmapCaching"), (rdpc.bEnableBitmapCaching));
	result += GetParamInt (wxT("bEnableWallpaper"), (rdpc.bEnableWallpaper));
	result += GetParamInt (wxT("bEnableFullWindowDrag"), (rdpc.bEnableFullWindowDrag));
	result += GetParamInt (wxT("bEnableAnimation"), (rdpc.bEnableAnimation));
	result += GetParamInt (wxT("bEnableThemes"), (rdpc.bEnableThemes));
	result += GetParamInt (wxT("bandwidth"),(int) (rdpc.bandwidth)); 
	result += GetParamInt (wxT("backing_store"), (rdpc.backing_store));
	result += GetParamInt (wxT("encription_enable_french"), (rdpc.encription_enable_french)); 
	result += GetParamInt (wxT("encription_enable_new"), (rdpc.encription_enable_new));
	result += GetParamInt (wxT("use_rdp_version"), (int)(rdpc.use_rdp_version)); 
	result += GetParamInt (wxT("send_mouse_event"), (rdpc.send_mouse_event)); 
	result += GetParamInt (wxT("private_color_map"), (rdpc.private_color_map)); 
	result += GetParamInt (wxT("single_mode"), (int)(rdpc.single_mode)); 
	result += GetParamInt (wxT("numlock_sync"), (rdpc.numlock_sync)); 
	result += GetParamInt (wxT("enable_compres"), (rdpc.enable_compres));
	result += GetParamStr (wxT("group_name"), (rdpc.group_name));
	result += GetParamStr (wxT("connection_name"), (rdpc.connection_name));

	result += GetParamInt (wxT("connection_count"), (int)(rdpc.dwConnectionCount));

	result += GetParamInt (wxT("bShareDrives"), (int)(rdpc.bShareDrives));
	result += GetParamInt (wxT("bSharePrinters"), (int)(rdpc.bSharePrinters));
	result += GetParamInt (wxT("bShareComPorts"), (int)(rdpc.bShareComPorts));
	result += GetParamInt (wxT("bShareSmartCards"), (int)(rdpc.bShareSmartCards));
	result += GetParamInt (wxT("SoundType"), (int)(rdpc.SoundType));
	result += GetParamStr (wxT("linux_devices"), (rdpc.redirect_devices_nix));

	char * buff_for_write = new char [iCurrSize];
	strncpy(buff_for_write,result.To8BitData(),iCurrSize);

	char * encrypt_data = NULL;
	int crypt_len = iCurrSize;

	CryptSettings * cr = NULL;
	programsettings ps = load_main_settings();
	if (ps.bUseCrypt)
	{
		GETKEYPATH()
		if (!wxFileExists(KEYPATH))
		{
			cr = new CryptSettings();
			cr->WriteKey(KEYPATH);
			delete cr;
		}
		else
		{
			cr = new CryptSettings(KEYPATH);
			if (ps.iTypeCrypt == 0)
			{
				cr->create_context_simple();
				encrypt_data = cr->SimpleCryptData((char *)buff_for_write,&crypt_len);
			}
			else if (ps.iTypeCrypt == 1)
			{
				cr->create_context_aes();
				encrypt_data = cr->AESEncryptData((char *)buff_for_write,&crypt_len);
			}
			delete cr;
		}
	}

	GETBASEPATH()
	wxFile fBase(BASEPATH);
	fBase.Open(BASEPATH,wxFile::write_append);	

	char str_temp[128];
	memset(str_temp,0,128);
	if (encrypt_data != NULL)
	{
		sprintf(str_temp,"k%d:",crypt_len);
		fBase.Write(str_temp,NumberCount(crypt_len) + 2);
		fBase.Write(encrypt_data,crypt_len);
	}
	else
	{
		sprintf(str_temp,"k%d:",iCurrSize);
		fBase.Write(str_temp,NumberCount(iCurrSize) + 2);
		fBase.Write(buff_for_write,iCurrSize);
	}

	fBase.Write("zz",2);
	fBase.Flush();
	fBase.Close();
	fBase.Detach();
	
	delete buff_for_write;
	if (encrypt_data != NULL)
	{
		free(encrypt_data);
	}
	wxString main_result = wxT("");
	return main_result;
}

int Benc::ByteLen(wxString str)
{
#ifdef __WXGTK__
	int result = 0;
	int lc = str.Length();
	for (int i = 0; i < lc; i ++)
	{
		if (str.GetChar(i) < 256 )
		{
			result++;
		} 
		else
		{
			result += 2;
		}
		
	}
	
	return result;
#endif
#ifdef __WXMSW__
	return (int)strlen(str.To8BitData()); 
#endif
}



wxString Benc::ConvertToUTF8(wxString str)
{
#ifdef __WXMSW__

	return wxString((wxConvUTF8.cWC2MB(str.wc_str(*wxConvCurrent))).data());

#endif

#ifdef __WXGTK__

	wxString result;
	result.Empty();
	
	int len = ByteLen(str);
	char * temp = new char[len];

	strncpy(temp,str.utf8_str().data(),len);
	for (int i = 0; i < len; i ++)
	{
		result += temp[i];
	}
	delete temp;
	return result;

#endif

}


wxString Benc::ConvertFromUTF8(const char * utf8)
{
	return wxString(wxConvUTF8.cMB2WC(utf8),*wxConvCurrent);
}

int Benc::NumberCount(unsigned int val)
{
	int count = 1;
	int div = 1;
	while(1)
	{
		if (val/div < 10) return count;
		else
		{
			div *= 10;
			count++;
		}
	}
}

wxString Benc::GetParamStr(wxString name, wxString param)
{
	wxString result;
	result.Empty();
	wxString temp_name = wxString::Format(wxT("%s"), (ConvertToUTF8(name)).data()); 
	int iNameSize = ByteLen(temp_name);
	wxString temp_param = wxString::Format(wxT("%s"), (ConvertToUTF8(param)).data());
	int iParamSize = ByteLen(temp_param);
	iCurrSize += (NumberCount(iNameSize) + iNameSize + NumberCount(iParamSize) + iParamSize + 6);
	result = wxString::Format(wxT("d%i:%ss%i:%see"),iNameSize,temp_name.data(),iParamSize,temp_param.data()); 
	return result;
	
}

wxString Benc::GetParamInt(wxString name, int param)
{
	wxString result;
	result.Empty();
	wxString temp;
	temp = wxString::Format(wxT("%i"),param);
	wxString temp_name = wxString::Format(wxT("%s"), (ConvertToUTF8(name)).data()); 
	int iNameSize = ByteLen(temp_name);
	wxString temp_param = wxString::Format(wxT("%s"), (ConvertToUTF8(temp)).data());
	int iParamSize = ByteLen(temp_param);
	
	iCurrSize += (NumberCount(iNameSize) + iNameSize + NumberCount(iParamSize) + iParamSize + 6);
	result = wxString::Format(wxT("d%i:%ss%i:%see"),iNameSize,temp_name.data(),iParamSize,temp_param.data()); 
	return result;
}

BOOL Benc::GetOptions(wxString str, int pos, RDPConn * rdp_conn)
{
	wxString strSize;
	strSize.Empty();
	int iCurrPos = pos + 1;
	wxChar tmp;
	wxString temp;
	temp.Empty();
	char buff[8];
	memset(buff,0,8);
	BOOL read = TRUE;
	while(read)
	{
		temp.Empty(); 
		tmp = str.GetChar(iCurrPos); 
		temp += tmp;  
		if (temp.IsNumber())
		{
			strSize += temp;
			iCurrPos ++;
		}
		else
		{
			read = FALSE;
		}
	}
	if (strSize.IsEmpty()) 
	{
		return FALSE;
	}
			
	int iSize = wxAtoi(strSize);
	int iSize_ = iSize;	

	wxString param_name = str.Mid(pos + 2 + NumberCount(iSize),iSize); 
	
	iSize_ = iSize;		
	int iEndPos = pos + 2 + NumberCount(iSize) + iSize;
	strSize.Empty(); 
	iCurrPos = iEndPos + 1;
	read = TRUE;
	while(read)
	{
		temp = str.GetChar(iCurrPos); 
		if (temp.IsNumber())
		{
			strSize += temp;
			iCurrPos ++;
		}
		else
		{
			read = FALSE;
		}
	}
	if (strSize.IsEmpty()) 
	{
		return FALSE;
	}

	iSize = wxAtoi(strSize);

	iCurrPos = iEndPos + 2 + NumberCount(iSize);
	wxString param_value = str.Mid(iCurrPos,iSize);

	bool status = false;
		
	if (param_name == wxT("hostname"))
	{
		rdp_conn->hostname = ConvertFromUTF8(param_value.To8BitData());
		status = true;
	}
	else if (param_name == wxT("username")) 
	{
		rdp_conn->username = ConvertFromUTF8(param_value.To8BitData());
		status = true;
	} 
	else if (param_name == wxT("password"))	
	{
		rdp_conn->password = ConvertFromUTF8(param_value.To8BitData());
		status = true;
	}
	else if (param_name == wxT("domain"))
	{
		rdp_conn->domain = ConvertFromUTF8(param_value.To8BitData());
		status = true;
	}
	else if (param_name == wxT("port")) 
	{
		rdp_conn->port = ConvertFromUTF8(param_value.To8BitData());
		status = true;
	} 
	else if (param_name == wxT("attach_to_console")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->attach_to_console = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->attach_to_console = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("width")) 
	{
		rdp_conn->width = wxAtoi(param_value);
		if (rdp_conn->width <= 0 )
		{
			rdp_conn->width = 800;
		}
		status = true;
	}
	else if (param_name == wxT("heigth")) 
	{
		rdp_conn->heigth = wxAtoi(param_value);
		if (rdp_conn->heigth <= 0 )
		{
			rdp_conn->heigth = 600;
		}
		status = true;
	}
	else if (param_name == wxT("color_depth")) 
	{
		rdp_conn->color_depth = wxAtoi(param_value);
		if ((rdp_conn->color_depth != 8 ) && (rdp_conn->color_depth != 15 ) && (rdp_conn->color_depth != 16 ) &&
				(rdp_conn->color_depth != 24 ))
		{
			rdp_conn->color_depth = 16;
		}
	status = true;
	}
	else if (param_name == wxT("bSmartSizing")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->bSmartSizing = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->bSmartSizing = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("bControlSize"))
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->bControlSize = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->bControlSize = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("bFullScreen")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->bFullScreen = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->bFullScreen = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("force_update_screen")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->force_update_screen = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->force_update_screen = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("shell")) 
	{
		rdp_conn->shell = ConvertFromUTF8(param_value.To8BitData());
		status = true;
	}
	else if (param_name == wxT("directory")) 
	{
		rdp_conn->directory = ConvertFromUTF8(param_value.To8BitData());
		status = true;
	}
	else if (param_name == wxT("bUseProgram")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->bUseProgram = FALSE;
			status = true;
		} 
		else if (param_value == wxT("1"))
		{
			rdp_conn->bUseProgram = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("bProgramMaximized")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->bProgramMaximized = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->bProgramMaximized = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("keyboard_map")) 
	{
		rdp_conn->keyboard_map = ConvertFromUTF8(param_value.To8BitData());
		status = true;
	}
	else if (param_name == wxT("keyboard")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->keyboard = 0;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->keyboard = 1;
			status = true;
		}
		else if (param_value == 2)
		{
			rdp_conn->keyboard = 2;
			status = true;
		}
	}
	else if (param_name == wxT("bEnableBitmapCaching")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->bEnableBitmapCaching = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->bEnableBitmapCaching = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("bEnableWallpaper")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->bEnableWallpaper = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->bEnableWallpaper = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("bEnableFullWindowDrag")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->bEnableFullWindowDrag = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->bEnableFullWindowDrag = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("bEnableAnimation")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->bEnableAnimation = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->bEnableAnimation = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("bEnableThemes")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->bEnableThemes = FALSE;
			status = true;
		} 
		else if (param_value == wxT("1"))
		{
			rdp_conn->bEnableThemes = TRUE;
			status = true;
		}
	} 
	else if (param_name == wxT("bShareDrives"))
	{
		
			if (param_value == wxT("0"))
			{
				rdp_conn->bShareDrives = FALSE;
				status = true;
			} 
			else if (param_value == wxT("1"))
			{
				rdp_conn->bShareDrives = TRUE;
				status = true;
			}
		
	} 
	else if (param_name == wxT("bSharePrinters"))
	{
		
			if (param_value == wxT("0"))
			{
				rdp_conn->bSharePrinters = FALSE;
				status = true;
			} 
			else if (param_value == wxT("1"))
			{
				rdp_conn->bSharePrinters = TRUE;
				status = true;
			}
		
	} 
	else if (param_name == wxT("bShareComPorts")) 
	{
		
			if (param_value == wxT("0"))
			{
				rdp_conn->bShareComPorts = FALSE;
				status = true;
			}
			else if (param_value == wxT("1"))
			{
				rdp_conn->bShareComPorts = TRUE;
				status = true;
			}
		
	}
	else if (param_name == wxT("bShareSmartCards")) 
	{
		
			if (param_value == wxT("0"))
			{
				rdp_conn->bShareSmartCards = FALSE;
				status = true;
			}
			else if (param_value == wxT("1"))
			{
				rdp_conn->bShareSmartCards = TRUE;
				status = true;
			}
		
	}
	else if (param_name == wxT("SoundType")) 
	{
		
			if (param_value == wxT("0"))
			{
				rdp_conn->SoundType = 0;
				status = true;
			}
			else if (param_value == wxT("1"))
			{
				rdp_conn->SoundType = 1;
				status = true;
			}
			else if (param_value == wxT("2"))
			{
				rdp_conn->SoundType = 2;
				status = true;
			}
			else 
			{
				status = false;
			}
		
	}
	else if (param_name == wxT("bandwidth")) 
	{
		rdp_conn->bandwidth = wxAtoi(param_value);
		if (rdp_conn->bandwidth <= 0 )
		{
			rdp_conn->bandwidth = 1;
		}
		status = true;
	}
	else if (param_name == wxT("backing_store")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->backing_store = FALSE;
			status = true;
		} 
		else if (param_value == wxT("1"))
		{
			rdp_conn->backing_store = TRUE;
			status = true;
		}
	} 
	else if (param_name == wxT("encription_enable_french"))
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->encription_enable_french = FALSE;
			status = true;
		} 
		else if (param_value == wxT("1"))
		{
			rdp_conn->encription_enable_french = TRUE;
			status = true;
		}
	} 
	else if (param_name == wxT("encription_enable_new"))
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->encription_enable_new = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->encription_enable_new = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("use_rdp_version")) 
	{
		rdp_conn->use_rdp_version = wxAtoi(param_value);
		if ((rdp_conn->use_rdp_version != 0 ) && (rdp_conn->use_rdp_version != 1 ) && (rdp_conn->use_rdp_version != 2 ))
		{
			rdp_conn->use_rdp_version = 1;
			status = true;
		}
	}
	else if (param_name == wxT("send_mouse_event")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->send_mouse_event = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->send_mouse_event = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("private_color_map")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->private_color_map = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->private_color_map = TRUE;
			status = true;
		}

	}
	else if (param_name == wxT("single_mode")) 
	{
		rdp_conn->single_mode = wxAtoi(param_value);
		if ((rdp_conn->single_mode <= 0 ) && (rdp_conn->single_mode >= 255 ))
		{
			rdp_conn->single_mode = 16;
			status = true;
		}
	}
	else if (param_name == wxT("numlock_sync")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->numlock_sync = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			rdp_conn->numlock_sync = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("enable_compres")) 
	{
		if (param_value == wxT("0"))
		{
			rdp_conn->enable_compres = FALSE;
			status = true;
		} 
		else if (param_value == wxT("1"))
		{
			rdp_conn->enable_compres = TRUE;
			status = true;
		}
	} 
	else if (param_name == wxT("uniq_name"))
	{
		rdp_conn->uniq_name = wxAtoi(param_value);
		if (rdp_conn->uniq_name > 0)
		{
			status = true;
		} 
		else 
		{
			status = false;
		}
	} 
	else if (param_name == wxT("group_name"))
	{
		rdp_conn->group_name = ConvertFromUTF8(param_value.To8BitData());
		status = true;
	}
	else if (param_name == wxT("connection_name"))
	{
		rdp_conn->connection_name = ConvertFromUTF8(param_value.To8BitData());
		status = true;
	} 
	else if (param_name == wxT("connection_count"))
	{
		rdp_conn->dwConnectionCount = wxAtoi(param_value);
		status = true;
	}	
	else if (param_name == wxT("linux_devices")) 
	{
		rdp_conn->redirect_devices_nix = ConvertFromUTF8(param_value.To8BitData());
		status = true;
	}
	else
	{
		status = false;
	}
	
	return TRUE;
}

BOOL Benc::isNormalParam(wxString str, int pos)
{
	wxString strSize;
	strSize.Empty();
	int iCurrPos = pos + 1;
	wxChar tmp;
	wxString temp;
	
	char buff[8];
	memset(buff,0,8);
	BOOL read = TRUE;
	while(read)
	{
		temp.Empty(); 
		tmp = str.GetChar(iCurrPos); 
		temp += tmp;  
		if (temp.IsNumber())
		{
			strSize += temp;
			iCurrPos ++;
		}
		else
		{
			read = FALSE;
		}
	}
	if (strSize.IsEmpty()) 
	{
		return FALSE;
	}
	
	int iSize = wxAtoi(strSize);
	int iSize_ = iSize;		
	int iEndPos = pos + 2 + NumberCount(iSize) + iSize;

	strSize.Empty(); 
	iCurrPos = iEndPos + 1;
	read = TRUE;
	while(read)
	{
		temp = str.GetChar(iCurrPos); 
		if (temp.IsNumber())
		{
			strSize += temp;
			iCurrPos ++;
		}
		else
		{
			read = FALSE;
		}
	}
	if (strSize.IsEmpty()) 
	{
		return FALSE;
	}

	iSize = wxAtoi(strSize);
	iEndPos = pos + NumberCount(iSize_) + iSize_ + NumberCount(iSize) + iSize + 4;
	
	wxString str_end = str.Mid(iEndPos,2); 
	if (str_end == wxT("ee"))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

RDPConn Benc::GetRDPConn(wxString str)
{	
	RDPConn rdpc;
	int strSize = ByteLen(str);
		
	for (int i = 0; i < strSize; i ++)
	{
		wxChar curr = str.GetChar(i);
		if (curr == 'd')
		{
			if (isNormalParam(str,i))
			{
				GetOptions(str,i,&rdpc);
			}
		}
	}
	return rdpc;
}

BOOL Benc::LoadProgramSettings(programsettings * ps)
{
	if (!ps) return FALSE;

	wxString settings_path;
#ifdef __WXMSW__
	wxGetEnv(wxT("APPDATA"),&settings_path);
#endif

#ifdef __WXGTK__
	wxGetEnv(STANDARD_LINUX_ENV,&settings_path);
#endif

	if (settings_path.IsEmpty())
	{
		ps->rdpbasepath = settings_path;
		ps->rdpkeypath = settings_path;
		ps->rdesktoppath = STANDARD_RDESKTOP_PATH;
		ps->rdesktop_key_path = STANDARD_RDESKTOP_KEY_PATH;
		ps->bIcon = TRUE;
		ps->favorites_count = STANDARD_FAVORITES_COUNT;
		ps->favorites_max = STANDARD_FAVORITES_MAX;
		ps->bShowFrame = FALSE;
		ps->bUseCrypt = TRUE;
		ps->iTypeCrypt = 1;
		ps->bFocusPage = TRUE;
		ps->bAutosave = TRUE;
		ps->lang = wxT("Default");
		return FALSE;
	}
	
#ifdef __WXMSW__
	settings_path += wxT("\\");
#endif	
#ifdef __WXGTK__
	settings_path += wxT("/.");
#endif
	settings_path += STANDARD_RDP_PATH;
	if (!wxDirExists(settings_path.data()))
	{
		wxMkdir(settings_path);
	}
	wxString file_path;
	file_path.Empty(); 
	file_path = settings_path;
#ifdef __WXMSW__
	file_path += wxT("\\");
#endif
#ifdef __WXGTK__
	file_path += wxT("/");
#endif
	file_path += STANDARD_RDPCONF_NAME;
	wxFile fset;
	if (!wxFileExists(file_path.data()))
	{
		ps->rdpbasepath = settings_path;
		ps->rdpkeypath = settings_path;
		ps->rdesktoppath = STANDARD_RDESKTOP_PATH;
		ps->rdesktop_key_path = STANDARD_RDESKTOP_KEY_PATH;
		ps->bIcon = TRUE;
		ps->favorites_count = STANDARD_FAVORITES_COUNT;
		ps->favorites_max = STANDARD_FAVORITES_MAX;
		ps->bShowFrame = FALSE;
		ps->bUseCrypt = TRUE;
		ps->iTypeCrypt = 1;
		ps->bFocusPage = TRUE;
		ps->bAutosave = TRUE;
		ps->lang = wxT("Default");
		
		SaveProgramSettings(ps);
		return TRUE;
	}

	fset.Open(file_path);
	if (!fset.IsOpened()) return FALSE;
	
	fset.SeekEnd();
	DWORD dwFileSize = fset.Tell();
	fset.Seek(0);
	if ((dwFileSize == wxInvalidOffset) || (dwFileSize == 0)) 
	{
		fset.Detach();
		ps->rdpbasepath = settings_path;
		ps->rdpkeypath = settings_path;
		ps->rdesktoppath = STANDARD_RDESKTOP_PATH;
		ps->rdesktop_key_path = STANDARD_RDESKTOP_KEY_PATH;
		ps->bIcon = TRUE;
		ps->favorites_count = STANDARD_FAVORITES_COUNT;
		ps->favorites_max = STANDARD_FAVORITES_MAX;
		ps->bShowFrame = FALSE;
		ps->bUseCrypt = TRUE;
		ps->iTypeCrypt = 1;
		ps->bFocusPage = TRUE;
		ps->bAutosave = TRUE;
		ps->lang = wxT("Default");
		
		SaveProgramSettings(ps);
		return TRUE;
	}

	char * tempbuff = new char [dwFileSize + 1];
	memset(tempbuff,0,(dwFileSize + 1));
	fset.Read(tempbuff,dwFileSize);
	wxString str_set;
	str_set.Empty();
	
	for (int i = 0; i < (int)dwFileSize; i ++)
	{
		str_set += tempbuff[i];
		
	}
	
	BOOL read = GetProgramSettings(str_set, ps);
	if(!read)
	{
		ps->rdpbasepath = settings_path;
		ps->rdpkeypath = settings_path;
		ps->rdesktoppath = STANDARD_RDESKTOP_PATH;
		ps->rdesktop_key_path = STANDARD_RDESKTOP_KEY_PATH;
		ps->bIcon = TRUE;
		ps->favorites_count = STANDARD_FAVORITES_COUNT;
		ps->favorites_max = STANDARD_FAVORITES_MAX;
		ps->bShowFrame = FALSE;
		ps->bUseCrypt = TRUE;
		ps->iTypeCrypt = 1;
		ps->bFocusPage = TRUE;
		ps->bAutosave = TRUE;
		ps->lang = wxT("Default");
		
		SaveProgramSettings(ps);
		return TRUE;
	}

	if (fset.IsOpened())
	{
		fset.Close();
	}
	fset.Detach();

	return TRUE;
}

BOOL Benc::GetProgramSettings(const wxString str_set, programsettings * ps)
{

	wxChar tmp = str_set.GetChar(0);
	
	if (tmp != wxT('k')) return FALSE;
	BOOL read = TRUE;
	int iCurrPos = 1;
	wxString temp;
	wxString strSize;
	strSize.Empty(); 
	
	while(read)
	{
		wxChar t;
		wxString str_t;
		str_t.Empty(); 
		t = str_set.GetChar(iCurrPos); 	
		str_t.assign(1,t); 
		
		if (str_t.IsNumber())
		{
			strSize += str_t;
			iCurrPos ++;
		}
		else
		{
			read = FALSE;
		}
	}
	if (strSize.IsEmpty()) return FALSE; 
	
	int lc = wxAtoi(strSize);
	iCurrPos = lc + 2 + NumberCount(lc);

	wxString str_zz = str_set.Mid(iCurrPos,2);
	if (str_zz != wxT("zz")) return FALSE;

	wxString res = str_set.Mid(2 + NumberCount(lc),lc);
	
	for (int i = 0; i < lc; i ++)
	{
		wxChar t = res.GetChar(i);
		if (t == wxT('d'))
		{
			if (isNormalParam(res,i))
			{
				if (!GetProgramOptions(res,i,ps)) return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL Benc::GetProgramOptions(wxString str, int pos, programsettings * ps)
{
	wxString strSize;
	strSize.Empty();
	int iCurrPos = pos + 1;
	wxChar tmp;
	wxString temp;
	temp.Empty();
	char buff[8];
	memset(buff,0,8);
	BOOL read = TRUE;
	while(read)
	{
		temp.Empty(); 
		tmp = str.GetChar(iCurrPos); 
		temp += tmp;  
		if (temp.IsNumber())
		{
			strSize += temp;
			iCurrPos ++;
		}
		else
		{
			read = FALSE;
		}
	}
	if (strSize.IsEmpty()) 
	{
		return FALSE;
	}
	
	int iSize = wxAtoi(strSize);
	int iSize_ = iSize;	
	wxString param_name = str.Mid(pos + 2 + NumberCount(iSize),iSize); 

	iSize_ = iSize;		
	int iEndPos = pos + 2 + NumberCount(iSize) + iSize;
	strSize.Empty(); 
	iCurrPos = iEndPos + 1;
	read = TRUE;
	while(read)
	{
		temp = str.GetChar(iCurrPos); 
		if (temp.IsNumber())
		{
			strSize += temp;
			iCurrPos ++;
		}
		else
		{
			read = FALSE;
		}
	}
	if (strSize.IsEmpty()) 
	{
		return FALSE;
	}

	iSize = wxAtoi(strSize);
	iCurrPos = iEndPos + 2 + NumberCount(iSize);
	wxString param_value = str.Mid(iCurrPos,iSize);
	
	bool status = false;
	if (param_name == wxT("rdpbase"))
	{
		ps->rdpbasepath = ConvertFromUTF8(param_value.To8BitData()) ;
		status = true;
	}
	else if (param_name == wxT("rdpkey")) 
	{
		ps->rdpkeypath =  ConvertFromUTF8(param_value.To8BitData());
		status = true;
	} 
	else if (param_name == wxT("rdesktop"))	
	{
		ps->rdesktoppath =  ConvertFromUTF8(param_value.To8BitData());
		status = true;
	}
	else if (param_name == wxT("rdesktop_keymap"))	
	{
		ps->rdesktop_key_path =  ConvertFromUTF8(param_value.To8BitData());
		status = true;
	}
	else if (param_name == wxT("language"))	
	{
		ps->lang =  ConvertFromUTF8(param_value.To8BitData());
		status = true;
	}
	else if (param_name == wxT("showicon")) 
	{
		if (param_value == wxT("0"))
		{
			ps->bIcon = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			ps->bIcon = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("showframe")) 
	{
		if (param_value == wxT("0"))
		{
			ps->bShowFrame = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			ps->bShowFrame = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("usecrypt")) 
	{
		if (param_value == wxT("0"))
		{
			ps->bUseCrypt = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			ps->bUseCrypt = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("focuspage")) 
	{
		if (param_value == wxT("0"))
		{
			ps->bFocusPage = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			ps->bFocusPage = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("autosave")) 
	{
		if (param_value == wxT("0"))
		{
			ps->bAutosave = FALSE;
			status = true;
		}
		else if (param_value == wxT("1"))
		{
			ps->bAutosave = TRUE;
			status = true;
		}
	}
	else if (param_name == wxT("fcount")) 
	{
		ps->favorites_count = wxAtoi(ConvertFromUTF8(param_value.To8BitData()));
		status = true;
	}
	else if (param_name == wxT("fmax")) 
	{
		ps->favorites_max = wxAtoi(ConvertFromUTF8(param_value.To8BitData()));
		status = true;
	}
	else if (param_name == wxT("typecrypt")) 
	{
		ps->iTypeCrypt = wxAtoi(ConvertFromUTF8(param_value.To8BitData()));
		status = true;
	}
	else
	{
		status = false;
	}

	return status;
}

BOOL Benc::SaveProgramSettings(programsettings * ps)
{
	if (!ps) return FALSE;
	wxString str_set;
	iCurrSize = 0;

	str_set = GetParamStr (wxT("rdpbase"), (ps->rdpbasepath));
	str_set += GetParamStr (wxT("rdpkey"), (ps->rdpkeypath));
	str_set += GetParamStr (wxT("rdesktop"), (ps->rdesktoppath));
	str_set += GetParamStr (wxT("rdesktop_keymap"), (ps->rdesktop_key_path));
	str_set += GetParamInt (wxT("showicon"), (int)(ps->bIcon));
	str_set += GetParamInt (wxT("showframe"), (int)(ps->bShowFrame));
	str_set += GetParamInt (wxT("usecrypt"), (int)(ps->bUseCrypt));
	str_set += GetParamInt (wxT("typecrypt"), (int)(ps->iTypeCrypt));
	str_set += GetParamInt (wxT("focuspage"), (int)(ps->bFocusPage));
	str_set += GetParamInt (wxT("autosave"), (int)(ps->bAutosave));
	str_set += GetParamInt (wxT("fcount"), (int)(ps->favorites_count));
	str_set += GetParamInt (wxT("fmax"), (int)(ps->favorites_max));
	str_set += GetParamStr (wxT("language"), (ps->lang));
	
	wxString main_result;
	main_result = wxString::Format(wxT("k%i:%szz"),iCurrSize,str_set.data()); 
	iCurrSize += NumberCount(iCurrSize) + 4;

	wxString settings_path;
#ifdef __WXMSW__
	wxGetEnv(wxT("APPDATA"),&settings_path);
#endif

#ifdef __WXGTK__
	wxGetEnv(STANDARD_LINUX_ENV,&settings_path);
#endif

	if (settings_path.IsEmpty())
	{
		return FALSE; 
	}

#ifdef __WXMSW__
	settings_path += wxT("\\");
#endif
#ifdef __WXGTK__
	settings_path += wxT("/.");
#endif
	settings_path += STANDARD_RDP_PATH;
	if (!wxDirExists(settings_path.data()))
	{
		wxMkdir(settings_path);
	}
	wxString file_path;
	file_path = settings_path;
#ifdef __WXMSW__
	file_path += wxT("\\");
#endif

#ifdef __WXGTK__
	file_path += wxT("/");
#endif
	file_path += STANDARD_RDPCONF_NAME;
	wxFile fset;
	if (!fset.Create(file_path,true)) return FALSE;

	fset.Write(main_result.To8BitData(),iCurrSize);
	fset.Close();
	fset.Detach();
	return TRUE;	
	
}



