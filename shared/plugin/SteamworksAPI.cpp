#include <cassert>

#include <extdll.h>
#include <meta_api.h>

#include "IMetaSteamworks.h"
#include "SteamworksAPI.h"

IMetaSteamworks* g_pMetaSteamworks = nullptr;

static CSteamAPIContext g_SteamAPIContext;

CSteamAPIContext* g_pSteamAPIContext = &g_SteamAPIContext;

//These API functions are implemented here so the context can use them directly.
S_API ISteamClient* SteamClient()
{
	assert( g_pMetaSteamworks );

	return g_pMetaSteamworks->GetSteamClient();
}

S_API HSteamPipe SteamAPI_GetHSteamPipe()
{
	assert( g_pMetaSteamworks );

	return g_pMetaSteamworks->GetHSteamPipe();
}

S_API HSteamUser SteamAPI_GetHSteamUser()
{
	assert( g_pMetaSteamworks );

	return g_pMetaSteamworks->GetHSteamUser();
}

bool Steamworks_InitLibrary()
{
	assert( g_pFactories );
	assert( g_pFactories->MetamodOnlyFactory );

	g_pMetaSteamworks = static_cast<IMetaSteamworks*>( g_pFactories->MetamodOnlyFactory( IMETASTEAMWORKS_NAME, nullptr ) );

	if( !g_pMetaSteamworks )
	{
		LOG_ERROR( PLID, "Steamworks_InitPlugin: Couldn't get interface \"%s\" from Metamod", IMETASTEAMWORKS_NAME );
		return false;
	}

	return g_pSteamAPIContext->Init();
}

void Steamworks_ShutdownLibrary()
{
	g_pSteamAPIContext->Clear();

	g_pMetaSteamworks = nullptr;
}
