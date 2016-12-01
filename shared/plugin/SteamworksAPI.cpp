#include <cassert>

#include <extdll.h>
#include <meta_api.h>

#include "IMetaSteamworks.h"
#include "SteamworksAPI.h"

IMetaSteamworks* g_pMetaSteamworks = nullptr;

static CSteamAPIContext g_SteamAPIContext;

static CSteamGameServerAPIContext g_SteamServerAPIContext;

CSteamAPIContext* g_pSteamAPI = &g_SteamAPIContext;

CSteamGameServerAPIContext* g_pSteamServerAPI = &g_SteamServerAPIContext;

bool Steamworks_InitLibrary()
{
	assert( g_pFactories );
	assert( g_pFactories->MetamodOnlyFactory );

	g_pMetaSteamworks = static_cast<IMetaSteamworks*>( g_pFactories->MetamodOnlyFactory( IMETASTEAMWORKS_NAME, nullptr ) );

	if( !g_pMetaSteamworks )
	{
		LOG_ERROR( PLID, "Steamworks_InitLibrary: Couldn't get interface \"%s\" from Metamod", IMETASTEAMWORKS_NAME );
		return false;
	}

	return g_pSteamAPI->Init();
}

bool Steamworks_InitLibraryServerAPI()
{
	//This assert will hit if you try to initialize the API before the server is active.
	assert( g_pSteamClientGameServer );

	if( !g_pSteamServerAPI->Init() )
	{
		LOG_ERROR( PLID, "Steamworks_InitLibraryServerAPI: Failed to initialize Server API" );
		return false;
	}

	LOG_MESSAGE( PLID, "Steamworks_InitLibraryServerAPI: Server API initialized" );

	return true;
}

void Steamworks_ShutdownLibrary()
{
	g_pSteamServerAPI->Clear();
	g_pSteamAPI->Clear();

	g_pMetaSteamworks = nullptr;
}
