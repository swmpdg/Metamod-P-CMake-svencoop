#ifndef IMETASTEAMWORKS_H
#define IMETASTEAMWORKS_H

#include "interface.h"
#include "steam_api.h"

#include <extdll.h>
#include <meta_api.h>

/**
*	Listener that lets plugins be notified of global Steamworks events.
*	By default, listeners are removed when a plugin is unloaded or when the server shuts down.
*	You can manually remove listeners if you want, and you can remove them from inside a callback.
*/
class IMetaSteamworksListener : public IBaseInterface
{
public:

	/**
	*	Called when the Steam game server has started up.
	*	This callback is the first moment that the Steam game server context can be initialized.
	*/
	virtual void OnServerStarted() {}

	/**
	*	Called when the server is shutting down. Equivalent to NEW_DLL_FUNCTIONS::pfnGameShutdown.
	*	Can be called without OnServerStarted having been called if the server started with no map and then shut down.
	*	@see IMetaSteamworks::IsServerActive
	*/
	virtual void OnServerShutdown() {}
};

/**
*	Metamod interface for Steamworks. Plugins link with Steamworks directly, this provides access to Metamod's global Steamworks support.
*/
class IMetaSteamworks : public IBaseInterface
{
public:

	/**
	*	@return Whether the server is active.
	*/
	virtual bool IsServerActive() const = 0;

	/**
	*	Add a listener.
	*/
	virtual void AddListener( plid_t plid, IMetaSteamworksListener* pListener ) = 0;

	/**
	*	Remove a listener.
	*/
	virtual void RemoveListener( plid_t plid, IMetaSteamworksListener* pListener ) = 0;
};

/**
*	Interface name.
*/
#define IMETASTEAMWORKS_NAME "MetaSteamworksV001"

#endif //IMETASTEAMWORKS_H
