

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Fri May 28 16:26:34 2010
 */
/* Compiler settings for .\UltraVncAx.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IUltraVncAxObj,0x8002D0F3,0x4218,0x44A7,0x9A,0x23,0xFB,0xA2,0xD1,0x76,0xF4,0x32);


MIDL_DEFINE_GUID(IID, LIBID_ULTRAVNCAXLib,0x987AD05D,0x338A,0x4E92,0x99,0x1B,0xF3,0x0C,0x31,0x82,0x49,0x68);


MIDL_DEFINE_GUID(IID, DIID__IUltraVncAxObjEvents,0x27E8B694,0x5F19,0x446B,0x8A,0x4A,0x6A,0x5E,0x08,0x6D,0x82,0x54);


MIDL_DEFINE_GUID(CLSID, CLSID_UltraVncAxObj,0x36D64AE5,0x6626,0x4DDE,0xA9,0x58,0x2F,0xF1,0xD4,0x6D,0x44,0x24);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



