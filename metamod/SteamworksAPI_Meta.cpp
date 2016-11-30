#include "IMetaSteamworks.h"
#include "SteamworksAPI_Meta.h"

bool Steamworks_Init()
{
	//SteamAPI_Init is called by the engine, but it seems that calling it again is safe.
	//Because we use version safe APIs we need InitSafe, which seems to trigger some internal changes.
	//The engine stills works after this call. - Solokiller
	return SteamAPI_InitSafe();
}

class CMetaSteamworks : public IMetaSteamworks
{
public:
	ISteamClient* GetSteamClient() override final
	{
		return SteamClient();
	}

	HSteamPipe GetHSteamPipe() override final
	{
		return SteamAPI_GetHSteamPipe();
	}

	HSteamUser GetHSteamUser() override final
	{
		return SteamAPI_GetHSteamUser();
	}
};

EXPOSE_SINGLE_INTERFACE( CMetaSteamworks, IMetaSteamworks, IMETASTEAMWORKS_NAME );
