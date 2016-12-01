#include "CMetaSteamworksListener.h"

void CMetaSteamworksListener::OnServerStarted()
{
	Steamworks_InitLibraryServerAPI();
}

void CMetaSteamworksListener::OnServerShutdown()
{
	Steamworks_ShutdownLibrary();
}
