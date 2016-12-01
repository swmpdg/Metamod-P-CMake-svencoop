#ifndef PLUGIN_STEAMWORKSAPI_H
#define PLUGIN_STEAMWORKSAPI_H

#include "steam_api.h"
#include "steam_gameserver.h"

#include "IMetaSteamworks.h"

/**
*	Metamod Steamworks API.
*/
extern IMetaSteamworks* g_pMetaSteamworks;

/**
*	Steamworks API interface.
*/
extern CSteamAPIContext* g_pSteamAPI;

/**
*	Steamworks game server API interface.
*	Note: game server pointers are null until the server has started. This doesn't happen until a map has loaded.
*	Attempting to call any methods before that time will crash the game.
*	Use IMetaSteamworksListener::OnServerStarted to initialize the API at the right time.
*/
extern CSteamGameServerAPIContext* g_pSteamServerAPI;

/**
*	Initializes the Steamworks interface for this library.
*	Requires the interface factories to have been initialized.
*	@return Whether Steamworks was initialized successfully.
*/
bool Steamworks_InitLibrary();

/**
*	Initializes the Steamworks Server API for this library.
*	Requires the interface factories to have been initialized.
*	@return Whether Steamworks was initialized successfully.
*/
bool Steamworks_InitLibraryServerAPI();

/**
*	Shuts down the Steamworks interface for this library.
*/
void Steamworks_ShutdownLibrary();

#endif //PLUGIN_STEAMWORKSAPI_H
