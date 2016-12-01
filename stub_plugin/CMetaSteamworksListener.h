#ifndef STUB_PLUGIN_CMETASTEAMWORKSLISTENER_H
#define STUB_PLUGIN_CMETASTEAMWORKSLISTENER_H

#include "IMetaSteamworks.h"
#include "plugin/SteamworksAPI.h"

/**
*	Default Steamworks listener. Initializes the server API when needed, shuts down the library when the game shuts down.
*/
class CMetaSteamworksListener : public IMetaSteamworksListener
{
public:
	CMetaSteamworksListener() = default;

	void OnServerStarted() override;

	void OnServerShutdown() override;

private:
	CMetaSteamworksListener( const CMetaSteamworksListener& ) = delete;
	CMetaSteamworksListener& operator=( const CMetaSteamworksListener& ) = delete;
};

/**
*	The Steamworks listener used by this plugin.
*/
extern IMetaSteamworksListener* g_pMetaSteamworksListener;

#endif //STUB_PLUGIN_CMETASTEAMWORKSLISTENER_H
