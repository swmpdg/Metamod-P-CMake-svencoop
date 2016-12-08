#include <algorithm>

#include "steam_gameserver.h"

#include <extdll.h>

#undef min
#undef max

#include "mplugin.h"

#include "SteamworksAPI_Meta.h"

bool Steamworks_Init()
{
	//Don't call SteamAPI_Init, that's intended for use with Steam client interfaces.
	//We're a server only mod that will run without an active Steam client so it'll fail to connect if we try.
	//- Solokiller
	return true;//SteamAPI_InitSafe();
}

static CMetaSteamworks g_MetaSteamworks;

CMetaSteamworks* MetaSteamworks()
{
	return &g_MetaSteamworks;
}

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CMetaSteamworks, IMetaSteamworks, IMETASTEAMWORKS_NAME, g_MetaSteamworks );

bool CMetaSteamworks::HasListener( IMetaSteamworksListener* pListener ) const
{
	if( !pListener )
		return false;

	for( const auto& listener : m_Listeners )
	{
		if( listener.pListener == pListener )
			return true;
	}

	return false;
}

void CMetaSteamworks::AddListener( plid_t plid, IMetaSteamworksListener* pListener )
{
	if( !plid )
	{
		META_WARNING( "CMetaSteamworks::AddListener: Null plugin id passed!" );
		return;
	}

	if( !pListener )
	{
		META_WARNING( "CMetaSteamworks::AddListener: [%s] Tried to add null listener!", plid->logtag );
		return;
	}

	if( HasListener( pListener ) )
	{
		META_WARNING( "CMetaSteamworks::AddListener: [%s] Tried to add listener multiple times!", plid->logtag );
		return;
	}

	m_Listeners.emplace_back( plid, pListener );
}

void CMetaSteamworks::RemoveListener( plid_t plid, IMetaSteamworksListener* pListener )
{
	if( !plid )
	{
		META_WARNING( "CMetaSteamworks::RemoveListener: Null plugin id passed!" );
		return;
	}

	if( !pListener )
	{
		META_WARNING( "CMetaSteamworks::RemoveListener: [%s] Tried to remove null listener!", plid->logtag );
		return;
	}

	auto it = std::find_if( m_Listeners.begin(), m_Listeners.end(),
		[ = ]( const auto& listener )
		{
			return listener.pListener == pListener;
		}
	);

	if( it != m_Listeners.end() )
	{
		if( it->id != plid )
		{
			META_WARNING( "CMetaSteamworks::RemoveListener: [%s] Tried to remove listener that it doesn't own!", plid->logtag );
			return;
		}

		m_Listeners.erase( it );

		if( pListener == m_pCurrentListener )
		{
			m_bRemovedCurrentListener = true;
		}
	}
	else
	{
		META_DEBUG( 5, ( "CMetaSteamworks::RemoveListener: [%s] Tried to remove listener that was not in the list!", plid->logtag ) );
	}
}

void CMetaSteamworks::OnCreateInstancedBaselines()
{
	if( !m_bServerStarted )
	{
		m_bServerStarted = true;
		NotifyListeners( &IMetaSteamworksListener::OnServerStarted );
	}
}

void CMetaSteamworks::OnGameShutdown()
{
	NotifyListeners( &IMetaSteamworksListener::OnServerShutdown );

	m_Listeners.clear();
	m_bServerStarted = false;
}

void CMetaSteamworks::OnPluginUnloaded( MPlugin& plugin )
{
	//A plugin was unloaded, remove all of its listeners if they haven't done so themselves.
	for( auto it = m_Listeners.begin(); it != m_Listeners.end(); )
	{
		if( it->id == plugin.info )
		{
			it = m_Listeners.erase( it );
		}
		else
		{
			++it;
		}
	}
}
