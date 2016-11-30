#ifndef PLUGIN_STEAMWORKSAPI_H
#define PLUGIN_STEAMWORKSAPI_H

#include "steam_api.h"

class IMetaSteamworks;

/**
*	Metamod Steamworks API.
*/
extern IMetaSteamworks* g_pMetaSteamworks;

/**
*	Steamworks API interface.
*/
extern CSteamAPIContext* g_pSteamAPIContext;

/**
*	Initializes the Steamworks interface for this library.
*	Requires the interface factories to have been initialized.
*	@return Whether Steamworks was initialized successfully.
*/
bool Steamworks_InitLibrary();

/**
*	Shuts down the Steamworks interface for this library.
*/
void Steamworks_ShutdownLibrary();

#endif //PLUGIN_STEAMWORKSAPI_H
