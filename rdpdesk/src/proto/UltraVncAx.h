

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Fri May 28 16:54:21 2010
 */
/* Compiler settings for ultravncax.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __ultravncax_h__
#define __ultravncax_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___IUltraVncAxObjEvents_FWD_DEFINED__
#define ___IUltraVncAxObjEvents_FWD_DEFINED__
typedef interface _IUltraVncAxObjEvents _IUltraVncAxObjEvents;
#endif 	/* ___IUltraVncAxObjEvents_FWD_DEFINED__ */


#ifndef __IUltraVncAxObj_FWD_DEFINED__
#define __IUltraVncAxObj_FWD_DEFINED__
typedef interface IUltraVncAxObj IUltraVncAxObj;
#endif 	/* __IUltraVncAxObj_FWD_DEFINED__ */


#ifndef __UltraVncAxObj_FWD_DEFINED__
#define __UltraVncAxObj_FWD_DEFINED__

#ifdef __cplusplus
typedef class UltraVncAxObj UltraVncAxObj;
#else
typedef struct UltraVncAxObj UltraVncAxObj;
#endif /* __cplusplus */

#endif 	/* __UltraVncAxObj_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __ULTRAVNCAXLib_LIBRARY_DEFINED__
#define __ULTRAVNCAXLib_LIBRARY_DEFINED__

/* library ULTRAVNCAXLib */
/* [helpstring][version][uuid] */ 




EXTERN_C const IID LIBID_ULTRAVNCAXLib;

#ifndef ___IUltraVncAxObjEvents_DISPINTERFACE_DEFINED__
#define ___IUltraVncAxObjEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IUltraVncAxObjEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IUltraVncAxObjEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("27E8B694-5F19-446B-8A4A-6A5E086D8254")
    _IUltraVncAxObjEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IUltraVncAxObjEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IUltraVncAxObjEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IUltraVncAxObjEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IUltraVncAxObjEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IUltraVncAxObjEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IUltraVncAxObjEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IUltraVncAxObjEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IUltraVncAxObjEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IUltraVncAxObjEventsVtbl;

    interface _IUltraVncAxObjEvents
    {
        CONST_VTBL struct _IUltraVncAxObjEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IUltraVncAxObjEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IUltraVncAxObjEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IUltraVncAxObjEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IUltraVncAxObjEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IUltraVncAxObjEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IUltraVncAxObjEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IUltraVncAxObjEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IUltraVncAxObjEvents_DISPINTERFACE_DEFINED__ */


#ifndef __IUltraVncAxObj_INTERFACE_DEFINED__
#define __IUltraVncAxObj_INTERFACE_DEFINED__

/* interface IUltraVncAxObj */
/* [object][oleautomation][dual][helpstring][uuid] */ 


EXTERN_C const IID IID_IUltraVncAxObj;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8002D0F3-4218-44A7-9A23-FBA2D176F432")
    IUltraVncAxObj : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Connect( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Disconnect( void) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Connected( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Server( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Server( 
            /* [in] */ BSTR pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExecuteCommand( 
            /* [in] */ BSTR cmdText,
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_IsInitialized( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Password( 
            /* [in] */ BSTR rhs) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Scale( 
            /* [in] */ VARIANT_BOOL rhs) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_FullScreen( 
            /* [in] */ VARIANT_BOOL rhs) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Shared( 
            /* [in] */ VARIANT_BOOL rhs) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ProxyHost( 
            /* [in] */ BSTR rhs) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ProxyPort( 
            /* [in] */ unsigned long rhs) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_ViewOnly( 
            /* [in] */ VARIANT_BOOL rhs) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE sendCAD( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IUltraVncAxObjVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUltraVncAxObj * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUltraVncAxObj * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUltraVncAxObj * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUltraVncAxObj * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUltraVncAxObj * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUltraVncAxObj * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUltraVncAxObj * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            IUltraVncAxObj * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Disconnect )( 
            IUltraVncAxObj * This);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Connected )( 
            IUltraVncAxObj * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Server )( 
            IUltraVncAxObj * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Server )( 
            IUltraVncAxObj * This,
            /* [in] */ BSTR pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExecuteCommand )( 
            IUltraVncAxObj * This,
            /* [in] */ BSTR cmdText,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_IsInitialized )( 
            IUltraVncAxObj * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Password )( 
            IUltraVncAxObj * This,
            /* [in] */ BSTR rhs);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Scale )( 
            IUltraVncAxObj * This,
            /* [in] */ VARIANT_BOOL rhs);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_FullScreen )( 
            IUltraVncAxObj * This,
            /* [in] */ VARIANT_BOOL rhs);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Shared )( 
            IUltraVncAxObj * This,
            /* [in] */ VARIANT_BOOL rhs);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ProxyHost )( 
            IUltraVncAxObj * This,
            /* [in] */ BSTR rhs);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ProxyPort )( 
            IUltraVncAxObj * This,
            /* [in] */ unsigned long rhs);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_ViewOnly )( 
            IUltraVncAxObj * This,
            /* [in] */ VARIANT_BOOL rhs);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *sendCAD )( 
            IUltraVncAxObj * This);
        
        END_INTERFACE
    } IUltraVncAxObjVtbl;

    interface IUltraVncAxObj
    {
        CONST_VTBL struct IUltraVncAxObjVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUltraVncAxObj_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUltraVncAxObj_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUltraVncAxObj_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUltraVncAxObj_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUltraVncAxObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUltraVncAxObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUltraVncAxObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IUltraVncAxObj_Connect(This)	\
    ( (This)->lpVtbl -> Connect(This) ) 

#define IUltraVncAxObj_Disconnect(This)	\
    ( (This)->lpVtbl -> Disconnect(This) ) 

#define IUltraVncAxObj_get_Connected(This,pVal)	\
    ( (This)->lpVtbl -> get_Connected(This,pVal) ) 

#define IUltraVncAxObj_get_Server(This,pVal)	\
    ( (This)->lpVtbl -> get_Server(This,pVal) ) 

#define IUltraVncAxObj_put_Server(This,pVal)	\
    ( (This)->lpVtbl -> put_Server(This,pVal) ) 

#define IUltraVncAxObj_ExecuteCommand(This,cmdText,pRetVal)	\
    ( (This)->lpVtbl -> ExecuteCommand(This,cmdText,pRetVal) ) 

#define IUltraVncAxObj_get_IsInitialized(This,pVal)	\
    ( (This)->lpVtbl -> get_IsInitialized(This,pVal) ) 

#define IUltraVncAxObj_put_Password(This,rhs)	\
    ( (This)->lpVtbl -> put_Password(This,rhs) ) 

#define IUltraVncAxObj_put_Scale(This,rhs)	\
    ( (This)->lpVtbl -> put_Scale(This,rhs) ) 

#define IUltraVncAxObj_put_FullScreen(This,rhs)	\
    ( (This)->lpVtbl -> put_FullScreen(This,rhs) ) 

#define IUltraVncAxObj_put_Shared(This,rhs)	\
    ( (This)->lpVtbl -> put_Shared(This,rhs) ) 

#define IUltraVncAxObj_put_ProxyHost(This,rhs)	\
    ( (This)->lpVtbl -> put_ProxyHost(This,rhs) ) 

#define IUltraVncAxObj_put_ProxyPort(This,rhs)	\
    ( (This)->lpVtbl -> put_ProxyPort(This,rhs) ) 

#define IUltraVncAxObj_put_ViewOnly(This,rhs)	\
    ( (This)->lpVtbl -> put_ViewOnly(This,rhs) ) 

#define IUltraVncAxObj_sendCAD(This)	\
    ( (This)->lpVtbl -> sendCAD(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUltraVncAxObj_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_UltraVncAxObj;

#ifdef __cplusplus

class DECLSPEC_UUID("36D64AE5-6626-4DDE-A958-2FF1D46D4424")
UltraVncAxObj;
#endif
#endif /* __ULTRAVNCAXLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


