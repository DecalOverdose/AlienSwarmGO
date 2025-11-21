//====== Copyright 1996-2015, Valve Corporation, All rights reserved. =======
//
// Purpose: Internal private Steamworks API declarations and definitions
//
//=============================================================================

#ifndef STEAM_API_INTERNAL_H
#define STEAM_API_INTERNAL_H

S_API HSteamUser SteamAPI_GetHSteamUser();
S_API bool S_CALLTYPE SteamInternal_Init();
S_API void* S_CALLTYPE SteamInternal_CreateInterface(const char* ver);
S_API void* S_CALLTYPE SteamGameServerInternal_CreateInterface(const char* ver);


#if !defined( STEAM_API_EXPORTS )

#if !defined( VERSION_SAFE_STEAM_API_INTERFACES )
inline CSteamAPIContext& SteamInternal_ModuleContext()
{
	// NOTE: declaring "static CSteamAPIConext" creates a large function
	// which queries the initialization status of the object. We know that
	// it is pointer-aligned and fully memset with zeros, so just alias a
	// static buffer of the appropriate size and call it a CSteamAPIContext.
	static void* ctx[sizeof(CSteamAPIContext) / sizeof(void*)];
	return *(CSteamAPIContext*)ctx;
}
#define _STEAMINTERNAL_ACCESSOR_BODY( AccessFunc )				\
		if ( !SteamAPI_GetHSteamPipe() ) return 0;				\
		CSteamAPIContext &ctx = SteamInternal_ModuleContext();	\
		if ( !ctx.AccessFunc() ) ctx.Init();					\
		return ctx.AccessFunc();
inline ISteamClient* SteamClient() { _STEAMINTERNAL_ACCESSOR_BODY(SteamClient) }
inline ISteamUser* SteamUser() { _STEAMINTERNAL_ACCESSOR_BODY(SteamUser) }
inline ISteamFriends* SteamFriends() { _STEAMINTERNAL_ACCESSOR_BODY(SteamFriends) }
inline ISteamUtils* SteamUtils() { _STEAMINTERNAL_ACCESSOR_BODY(SteamUtils) }
inline ISteamMatchmaking* SteamMatchmaking() { _STEAMINTERNAL_ACCESSOR_BODY(SteamMatchmaking) }
inline ISteamUserStats* SteamUserStats() { _STEAMINTERNAL_ACCESSOR_BODY(SteamUserStats) }
inline ISteamApps* SteamApps() { _STEAMINTERNAL_ACCESSOR_BODY(SteamApps) }
inline ISteamMatchmakingServers* SteamMatchmakingServers() { _STEAMINTERNAL_ACCESSOR_BODY(SteamMatchmakingServers) }
inline ISteamNetworking* SteamNetworking() { _STEAMINTERNAL_ACCESSOR_BODY(SteamNetworking) }
inline ISteamRemoteStorage* SteamRemoteStorage() { _STEAMINTERNAL_ACCESSOR_BODY(SteamRemoteStorage) }
inline ISteamScreenshots* SteamScreenshots() { _STEAMINTERNAL_ACCESSOR_BODY(SteamScreenshots) }
inline ISteamHTTP* SteamHTTP() { _STEAMINTERNAL_ACCESSOR_BODY(SteamHTTP) }
inline ISteamUnifiedMessages* SteamUnifiedMessages() { _STEAMINTERNAL_ACCESSOR_BODY(SteamUnifiedMessages) }
inline ISteamController* SteamController() { _STEAMINTERNAL_ACCESSOR_BODY(SteamController) }
inline ISteamUGC* SteamUGC() { _STEAMINTERNAL_ACCESSOR_BODY(SteamUGC) }
inline ISteamAppList* SteamAppList() { _STEAMINTERNAL_ACCESSOR_BODY(SteamAppList) }
inline ISteamMusic* SteamMusic() { _STEAMINTERNAL_ACCESSOR_BODY(SteamMusic) }
inline ISteamMusicRemote* SteamMusicRemote() { _STEAMINTERNAL_ACCESSOR_BODY(SteamMusicRemote) }
inline ISteamHTMLSurface* SteamHTMLSurface() { _STEAMINTERNAL_ACCESSOR_BODY(SteamHTMLSurface) }
inline ISteamInventory* SteamInventory() { _STEAMINTERNAL_ACCESSOR_BODY(SteamInventory) }
inline ISteamVideo* SteamVideo() { _STEAMINTERNAL_ACCESSOR_BODY(SteamVideo) }
#undef _STEAMINTERNAL_ACCESSOR_BODY
#endif // !defined( VERSION_SAFE_STEAM_API_INTERFACES )

#endif // !defined( STEAM_API_EXPORTS )


inline void CSteamAPIContext::Clear()
{
	m_pSteamClient = NULL;
	m_pSteamUser = NULL;
	m_pSteamFriends = NULL;
	m_pSteamUtils = NULL;
	m_pSteamMatchmaking = NULL;
	m_pSteamUserStats = NULL;
	m_pSteamApps = NULL;
	m_pSteamMatchmakingServers = NULL;
	m_pSteamNetworking = NULL;
	m_pSteamRemoteStorage = NULL;
	m_pSteamHTTP = NULL;
	m_pSteamScreenshots = NULL;
	m_pSteamMusic = NULL;
	m_pSteamUnifiedMessages = NULL;
	m_pController = NULL;
	m_pSteamUGC = NULL;
	m_pSteamAppList = NULL;
	m_pSteamMusic = NULL;
	m_pSteamMusicRemote = NULL;
	m_pSteamHTMLSurface = NULL;
	m_pSteamInventory = NULL;
}


// This function must be declared inline in the header so the module using steam_api.dll gets the version names they want.
inline bool CSteamAPIContext::Init()
{
	return true;
}


//-----------------------------------------------------------------------------
// The following macros are implementation details, not intended for public use
//-----------------------------------------------------------------------------
#define _STEAM_CALLBACK_AUTO_HOOK( thisclass, func, param )
#define _STEAM_CALLBACK_HELPER( _1, _2, SELECTED, ... )		_STEAM_CALLBACK_##SELECTED
#define _STEAM_CALLBACK_SELECT( X, Y )						_STEAM_CALLBACK_HELPER X Y
#define _STEAM_CALLBACK_3( extra_code, thisclass, func, param ) \
	struct CCallbackInternal_ ## func : private CCallbackImpl< sizeof( param ) > { \
		CCallbackInternal_ ## func () { extra_code SteamAPI_RegisterCallback( this, param::k_iCallback ); } \
		CCallbackInternal_ ## func ( const CCallbackInternal_ ## func & ) { extra_code SteamAPI_RegisterCallback( this, param::k_iCallback ); } \
		CCallbackInternal_ ## func & operator=( const CCallbackInternal_ ## func & ) { return *this; } \
		private: virtual void Run( void *pvParam ) { _STEAM_CALLBACK_AUTO_HOOK( thisclass, func, param ) \
			thisclass *pOuter = reinterpret_cast<thisclass*>( reinterpret_cast<char*>(this) - offsetof( thisclass, m_steamcallback_ ## func ) ); \
			pOuter->func( reinterpret_cast<param*>( pvParam ) ); \
		} \
	} m_steamcallback_ ## func ; void func( param *pParam )
#define _STEAM_CALLBACK_4( _, thisclass, func, param, var ) \
	CCallback< thisclass, param > var; void func( param *pParam )


//-----------------------------------------------------------------------------
// Purpose: maps a steam async call result to a class member function
//			template params: T = local class, P = parameter struct
//-----------------------------------------------------------------------------
template< class T, class P >
inline CCallResult<T, P>::CCallResult()
{
	m_hAPICall = k_uAPICallInvalid;
	m_pObj = NULL;
	m_Func = NULL;
	m_iCallback = P::k_iCallback;
}

template< class T, class P >
inline void CCallResult<T, P>::Set(SteamAPICall_t hAPICall, T* p, func_t func)
{
	if (m_hAPICall)
		SteamAPI_UnregisterCallResult(this, m_hAPICall);

	m_hAPICall = hAPICall;
	m_pObj = p;
	m_Func = func;

	if (hAPICall)
		SteamAPI_RegisterCallResult(this, hAPICall);
}

template< class T, class P >
inline bool CCallResult<T, P>::IsActive() const
{
	return (m_hAPICall != k_uAPICallInvalid);
}

template< class T, class P >
inline void CCallResult<T, P>::Cancel()
{
	if (m_hAPICall != k_uAPICallInvalid)
	{
		SteamAPI_UnregisterCallResult(this, m_hAPICall);
		m_hAPICall = k_uAPICallInvalid;
	}

}

template< class T, class P >
inline CCallResult<T, P>::~CCallResult()
{
	Cancel();
}

template< class T, class P >
inline void CCallResult<T, P>::Run(void* pvParam)
{
	m_hAPICall = k_uAPICallInvalid; // caller unregisters for us
	(m_pObj->*m_Func)((P*)pvParam, false);
}

template< class T, class P >
inline void CCallResult<T, P>::Run(void* pvParam, bool bIOFailure, SteamAPICall_t hSteamAPICall)
{
	if (hSteamAPICall == m_hAPICall)
	{
		m_hAPICall = k_uAPICallInvalid; // caller unregisters for us
		(m_pObj->*m_Func)((P*)pvParam, bIOFailure);
	}
}


//-----------------------------------------------------------------------------
// Purpose: maps a steam callback to a class member function
//			template params: T = local class, P = parameter struct,
//			bGameserver = listen for gameserver callbacks instead of client callbacks
//-----------------------------------------------------------------------------
template< class T, class P, bool bGameserver >
inline CCallback< T, P, bGameserver >::CCallback(T* pObj, func_t func)
	: m_pObj(NULL), m_Func(NULL)
{
	if (bGameserver)
	{
		this->SetGameserverFlag();
	}
	Register(pObj, func);
}

template< class T, class P, bool bGameserver >
inline void CCallback< T, P, bGameserver >::Register(T* pObj, func_t func)
{
	if (!pObj || !func)
		return;

	if (this->m_nCallbackFlags & CCallbackBase::k_ECallbackFlagsRegistered)
		Unregister();

	m_pObj = pObj;
	m_Func = func;
	// SteamAPI_RegisterCallback sets k_ECallbackFlagsRegistered
	SteamAPI_RegisterCallback(this, P::k_iCallback);
}

template< class T, class P, bool bGameserver >
inline void CCallback< T, P, bGameserver >::Unregister()
{
	// SteamAPI_UnregisterCallback removes k_ECallbackFlagsRegistered
	SteamAPI_UnregisterCallback(this);
}

template< class T, class P, bool bGameserver >
inline void CCallback< T, P, bGameserver >::Run(void* pvParam)
{
	(m_pObj->*m_Func)((P*)pvParam);
}


#if defined(USE_BREAKPAD_HANDLER) || defined(STEAM_API_EXPORTS)
// this should be called before the game initialized the steam APIs
// pchDate should be of the format "Mmm dd yyyy" (such as from the __ DATE __ macro )
// pchTime should be of the format "hh:mm:ss" (such as from the __ TIME __ macro )
// bFullMemoryDumps (Win32 only) -- writes out a uuid-full.dmp in the client/dumps folder
// pvContext-- can be NULL, will be the void * context passed into m_pfnPreMinidumpCallback
// PFNPreMinidumpCallback m_pfnPreMinidumpCallback   -- optional callback which occurs just before a .dmp file is written during a crash.  Applications can hook this to allow adding additional information into the .dmp comment stream.
S_API void S_CALLTYPE SteamAPI_UseBreakpadCrashHandler(char const* pchVersion, char const* pchDate, char const* pchTime, bool bFullMemoryDumps, void* pvContext, PFNPreMinidumpCallback m_pfnPreMinidumpCallback);
S_API void S_CALLTYPE SteamAPI_SetBreakpadAppID(uint32 unAppID);
#endif


#endif // STEAM_API_INTERNAL_H