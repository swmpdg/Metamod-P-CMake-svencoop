#ifndef METAMOD_STEAMWORKSAPI_META_H
#define METAMOD_STEAMWORKSAPI_META_H

#include <vector>

#include "IMetaSteamworks.h"

class MPlugin;

/**
*	@file
*
*	Metamod routines for Steamworks API management.
*/

/**
*	Initialize Steamworks.
*	@return Whether Steamworks initialized successfully.
*/
bool Steamworks_Init();

class CMetaSteamworks : public IMetaSteamworks
{
private:
	struct Listener_t
	{
		plid_t id; //!Id of the plugin that added this listener.
		IMetaSteamworksListener* pListener;

		Listener_t( plid_t id, IMetaSteamworksListener* pListener )
			: id( id )
			, pListener( pListener )
		{
		}
	};

public:
	CMetaSteamworks() = default;

	bool IsServerActive() const override final { return m_bServerStarted; }

	bool HasListener( IMetaSteamworksListener* pListener ) const;

	void AddListener( plid_t plid, IMetaSteamworksListener* pListener ) override final;

	void RemoveListener( plid_t plid, IMetaSteamworksListener* pListener ) override final;

	void OnCreateInstancedBaselines();

	void OnGameShutdown();

	void OnPluginUnloaded( MPlugin& plugin );

	/**
	*	Notify listeners of event.
	*	@param callback Callback to invoke.
	*	@param args Arguments to pass.
	*	@see IMetaSteamworksListener
	*/
	template<typename CALLBACKFN, typename... ARGS>
	void NotifyListeners( CALLBACKFN callback, ARGS&&... args )
	{
		for( decltype( m_Listeners.size() ) index = 0; index < m_Listeners.size(); )
		{
			m_pCurrentListener = m_Listeners[ index ].pListener;

			( m_pCurrentListener->*callback )( std::move( args )... );

			//Listeners can remove themselves while being called, so make sure to account for that.
			if( !m_bRemovedCurrentListener )
			{
				++index;
			}
			else
			{
				m_bRemovedCurrentListener = false;
			}
		}

		m_pCurrentListener = nullptr;
	}

private:
	std::vector<Listener_t> m_Listeners;

	bool m_bServerStarted = false;
	bool m_bRemovedCurrentListener = false;

	IMetaSteamworksListener* m_pCurrentListener = nullptr;
};

/**
*	Singleton accessor for CMetaSteamworks.
*/
CMetaSteamworks* MetaSteamworks();

#endif //METAMOD_STEAMWORKSAPI_META_H
