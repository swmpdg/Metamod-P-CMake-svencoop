#ifndef IMETASTEAMWORKS_H
#define IMETASTEAMWORKS_H

#include "interface.h"
#include "steam_api.h"

/**
*	Provides access to Steamworks APIs to plugins.
*	Metamod itself can safely call Steamworks API functions directly, plugins cannot because they won't link with the library.
*	This is to reduce the number of dependencies, and since Metamod is in control of the interfaces we can deal with versioning more easily.
*/
class IMetaSteamworks : public IBaseInterface
{
public:

	/**
	*	Gets the Steam client.
	*/
	virtual ISteamClient* GetSteamClient() = 0;

	/**
	*	The pipe used to communicate with.
	*/
	virtual HSteamPipe GetHSteamPipe() = 0;

	/**
	*	The user we're running as.
	*/
	virtual HSteamUser GetHSteamUser() = 0;
};

/**
*	Interface name.
*/
#define IMETASTEAMWORKS_NAME "MetaSteamworksV001"

#endif //IMETASTEAMWORKS_H
