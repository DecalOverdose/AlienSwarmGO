//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Expose things from GameInterface.cpp. Mostly the engine interfaces.
//
// $NoKeywords: $
//=============================================================================//

#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#ifdef _WIN32
#pragma once
#endif

#include "mapentities.h"

#ifndef NO_STEAM
#include "steam/steam_gameserver.h"
#endif

extern INetworkStringTable *g_pStringTableInfoPanel;

#ifndef OVERRIDE
#define OVERRIDE override
#endif

// Player / Client related functions
// Most of this is implemented in gameinterface.cpp, but some of it is per-mod in files like cs_gameinterface.cpp, etc.
class CServerGameClients : public IServerGameClients
{
public:
	virtual bool			ClientConnect(edict_t* pEntity, char const* pszName, char const* pszAddress, char* reject, int maxrejectlen);
	virtual void			ClientActive(edict_t* pEntity, bool bLoadGame);
	virtual void			ClientFullyConnect(edict_t* pEntity);
	virtual void			ClientDisconnect(edict_t* pEntity);
	virtual void			ClientPutInServer(edict_t* pEntity, const char* playername);
	virtual void			ClientCommand(edict_t* pEntity, const CCommand& args);
	virtual void			ClientSettingsChanged(edict_t* pEntity);
	virtual void			ClientSetupVisibility(edict_t* pViewEntity, edict_t* pClient, unsigned char* pvs, int pvssize);
	virtual float			ProcessUsercmds(edict_t* player, bf_read* buf, int numcmds, int totalcmds,
		int dropped_packets, bool ignore, bool paused);
	// Player is running a command
	virtual void			PostClientMessagesSent(void);
	virtual void			SetCommandClient(int index);
	virtual CPlayerState* GetPlayerState(edict_t* player);
	virtual void			ClientEarPosition(edict_t* pEntity, Vector* pEarOrigin);

	virtual bool			ClientReplayEvent(edict_t* pEdict, const ClientReplayEventParams_t& params) OVERRIDE;

	virtual void			GetPlayerLimits(int& minplayers, int& maxplayers, int& defaultMaxPlayers) const;

	// returns number of delay ticks if player is in Replay mode (0 = no delay)
	virtual int				GetReplayDelay(edict_t* player, int& entity);
	// Anything this game .dll wants to add to the bug reporter text (e.g., the entity/model under the picker crosshair)
	//  can be added here
	virtual void			GetBugReportInfo(char* buf, int buflen);

	// A player sent a voice packet
	virtual void			ClientVoice(edict_t* pEdict);

	virtual void			NetworkIDValidated(const char* pszUserName, const char* pszNetworkID, CSteamID steamID) OVERRIDE;

	virtual void			NetworkIDValidated(const char* pszUserName, const char* pszNetworkID);
	virtual int				GetMaxSplitscreenPlayers();
	virtual int				GetMaxHumanPlayers();

	virtual const char* ClientNameHandler(uint64 xuid, const char* pchName) OVERRIDE;

	// The client has submitted a keyvalues command
	virtual void			ClientCommandKeyValues(edict_t* pEntity, KeyValues* pKeyValues);
};

class CServerGameDLL : public IServerGameDLL
{
public:
	virtual bool			DLLInit(CreateInterfaceFn engineFactory, CreateInterfaceFn physicsFactory,
		CreateInterfaceFn fileSystemFactory, CGlobalVars* pGlobals);
	virtual void			DLLShutdown(void);
	virtual float			GetTickInterval(void) const;
	virtual bool			GameInit(void);
	virtual void			GameShutdown(void);
	virtual bool			LevelInit(const char* pMapName, char const* pMapEntities, char const* pOldLevel, char const* pLandmarkName, bool loadGame, bool background);
	virtual void			ServerActivate(edict_t* pEdictList, int edictCount, int clientMax);
	virtual void			LevelShutdown(void);
	virtual void			GameFrame(bool simulating); // could be called multiple times before sending data to clients
	virtual void			PreClientUpdate(bool simulating); // called after all GameFrame() calls, before sending data to clients

	virtual ServerClass* GetAllServerClasses(void);
	virtual const char* GetGameDescription(void);
	virtual void			CreateNetworkStringTables(void);

	virtual CSaveRestoreData* SaveInit(int size);
	virtual void			SaveWriteFields(CSaveRestoreData*, char const*, void*, datamap_t*, typedescription_t*, int);
	virtual void			SaveReadFields(CSaveRestoreData*, char const*, void*, datamap_t*, typedescription_t*, int);
	virtual void			SaveGlobalState(CSaveRestoreData*);
	virtual void			RestoreGlobalState(CSaveRestoreData*);
	virtual int				CreateEntityTransitionList(CSaveRestoreData*, int);
	virtual void			BuildAdjacentMapList(void);

	virtual void			PreSave(CSaveRestoreData*);
	virtual void			Save(CSaveRestoreData*);
	virtual void			GetSaveComment(char* comment, int maxlength, float flMinutes, float flSeconds, bool bNoTime = false);

	virtual void			WriteSaveHeaders(CSaveRestoreData*);

	virtual void			ReadRestoreHeaders(CSaveRestoreData*);
	virtual void			Restore(CSaveRestoreData*, bool);
	virtual bool			IsRestoring();
	virtual bool			SupportsSaveRestore();

	virtual bool ShouldHoldGameServerReservation(float flTimeElapsedWithoutClients);

	// Retrieve info needed for parsing the specified user message
	virtual bool			GetUserMessageInfo(int msg_type, char* name, int maxnamelength, int& size);

	virtual CStandardSendProxies* GetStandardSendProxies();

	virtual void			PostInit();
	virtual void			PostToolsInit();
	virtual void			Think(bool finalTick);

	virtual void			OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* pCvarName, const char* pCvarValue);

	virtual void			PreSaveGameLoaded(char const* pSaveName, bool bInGame);

	virtual bool			ShouldHideServer(void);

	virtual void			InvalidateMdlCache();

	// Called to apply lobby settings to a dedicated server
	virtual void			ApplyGameSettings(KeyValues* pKV);

	virtual void			GetMatchmakingTags(char* buf, size_t bufSize);

	virtual void			ServerHibernationUpdate(bool bHibernating);

	virtual bool			ShouldPreferSteamAuth();

	virtual bool			SupportsRandomMaps();

	virtual bool			ShouldTimeoutClient( int nUserID, float flTimeSinceLastReceived );

	virtual bool			ShouldAllowDirectConnect(void);
	virtual bool			FriendsReqdForDirectConnect(void);
	virtual bool			IsLoadTestServer(void);

	virtual bool			IsValveDS(void);

	// Matchmaking game data buffer to set into SteamGameServer()->SetGameData
	virtual void			GetMatchmakingGameData(char* buf, size_t bufSize);

	float	m_fAutoSaveDangerousTime;
	float	m_fAutoSaveDangerousMinHealthToCommit;
	bool	m_bIsHibernating;

	// Called after the steam API has been activated post-level startup
	virtual void			GameServerSteamAPIActivated(void);

	// validate if player is a caster and add them to the active caster list
	virtual bool ValidateAndAddActiveCaster(const CSteamID& steamID);

	// Network channel notification from engine to game server code
	virtual void OnEngineClientNetworkEvent(edict_t* edictClient, uint64 ullSteamID, int nEventType, void* pvParam);

	virtual bool LogForHTTPListeners(const char* szLogLine);

protected:
	KeyValues* FindLaunchOptionByValue(KeyValues* pLaunchOptions, char const* szLaunchOption);

private:

	// This can just be a wrapper on MapEntity_ParseAllEntities, but CS does some tricks in here
	// with the entity list.
	void LevelInit_ParseAllEntities(const char* pMapEntities);
	void LoadMessageOfTheDay();
};


// Normally, when the engine calls ClientPutInServer, it calls a global function in the game DLL
// by the same name. Use this to override the function that it calls. This is used for bots.
typedef CBasePlayer* (*ClientPutInServerOverrideFn)( edict_t *pEdict, const char *playername );

void ClientPutInServerOverride( ClientPutInServerOverrideFn fn );

// -------------------------------------------------------------------------------------------- //
// Entity list management stuff.
// -------------------------------------------------------------------------------------------- //
// These are created for map entities in order as the map entities are spawned.
class CMapEntityRef
{
public:
	int		m_iEdict;			// Which edict slot this entity got. -1 if CreateEntityByName failed.
	int		m_iSerialNumber;	// The edict serial number. TODO used anywhere ?
};

extern CUtlLinkedList<CMapEntityRef, unsigned short> g_MapEntityRefs;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CMapLoadEntityFilter : public IMapEntityFilter
{
public:
	virtual bool ShouldCreateEntity( const char *pClassname )
	{
		// During map load, create all the entities.
		return true;
	}

	virtual CBaseEntity* CreateNextEntity( const char *pClassname )
	{
		CBaseEntity *pRet = CreateEntityByName( pClassname );

		CMapEntityRef ref;
		ref.m_iEdict = -1;
		ref.m_iSerialNumber = -1;

		if ( pRet )
		{
			ref.m_iEdict = pRet->entindex();
			if ( pRet->edict() )
				ref.m_iSerialNumber = pRet->edict()->m_NetworkSerialNumber;
		}

		g_MapEntityRefs.AddToTail( ref );
		return pRet;
	}
};

bool IsEngineThreaded();

class CServerGameTags : public IServerGameTags
{
public:
	virtual void GetTaggedConVarList( KeyValues *pCvarTagList );

};
EXPOSE_SINGLE_INTERFACE( CServerGameTags, IServerGameTags, INTERFACEVERSION_SERVERGAMETAGS );

#ifndef NO_STEAM
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CSteam3Server : public CSteamGameServerAPIContext
{
public:
	CSteam3Server();

	void Shutdown( void )
	{
		Clear();
		m_bInitialized = false;
	}

	bool CheckInitialized( void )
	{
		if ( !m_bInitialized )
		{
			Init();
			m_bInitialized = true;
			return true;
		}

		return false;
	}

private:
	bool	m_bInitialized;
};
CSteam3Server &Steam3Server();
#endif

#endif // GAMEINTERFACE_H

