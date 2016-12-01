#ifndef CSTEAM2IDSTRING_H
#define CSTEAM2IDSTRING_H

#include <cstring>

#include "common/steamcommon.h"
#include "steam_api.h"

#define STEAM_ID_PENDING "STEAM_ID_PENDING"
#define STEAM_ID_LAN "STEAM_ID_LAN"

/**
*	Represents a Steam2 ID as a string. Also keeps track of the universe.
*	Same format as auth IDs returned by enginefuncs_t::pfnGetPlayerAuthId.
*	@see enginefuncs_t::pfnGetPlayerAuthId
*	@see CSteamID
*	@see TSteamGlobalUserID
*/
class CSteam2IDString
{
public:
	static const size_t MAX_SIZE = 64;

public:
	/**
	*	Constructs an empty auth ID string.
	*/
	CSteam2IDString() = default;
	CSteam2IDString( const CSteam2IDString& other ) = default;
	CSteam2IDString& operator=( const CSteam2IDString& other ) = default;

	/**
	*	Constructs an auth ID string from a Steam2 ID.
	*	@param userID User ID.
	*	@param universe Universe that this ID belongs in.
	*/
	CSteam2IDString( const TSteamGlobalUserID& userID, EUniverse universe = k_EUniversePublic );

	/**
	*	Constructs an auth ID string from a Steam3 ID.
	*	@param steamID Steam3 ID.
	*	@param bIsLan Whether this is server is currently running with sv_lan enabled.
	*/
	CSteam2IDString( const CSteamID& steamID, const bool bIsLan );

	/**
	*	Constructs an auth ID string from a Steam2 ID string.
	*	@param pszAuthID Auth ID string.
	*	@param universe Universe that this auth ID belongs in.
	*/
	CSteam2IDString( const char* const pszAuthID, EUniverse universe = k_EUniversePublic );

	/**
	*	@return Whether this auth ID represents a valid Steam2 ID.
	*/
	bool IsValid() const { return !!( *m_szString ); }

	/**
	*	@return Whether this Steam ID is pending.
	*/
	bool IsPending() const { return stricmp( m_szString, STEAM_ID_PENDING ) == 0; }

	/**
	*	@return Whether this Steam ID is lan.
	*/
	bool IsLan() const { return stricmp( m_szString, STEAM_ID_LAN ) == 0; }

	/**
	*	@return The auth ID string.
	*/
	const char* GetString() const { return m_szString; }

	/**
	*	@return The universe that this ID belongs to.
	*/
	EUniverse GetUniverse() const { return m_Universe; }

	/**
	*	Sets the auth ID string from a Steam2 ID.
	*	@param universe Universe that this auth ID belongs in.
	*/
	void Set( const TSteamGlobalUserID& userID, EUniverse universe = k_EUniversePublic );

	/**
	*	Sets the auth ID string from a Steam3 ID.
	*	@param steamID Steam3 ID.
	*	@param bIsLan Whether this is server is currently running with sv_lan enabled.
	*/
	void Set( const CSteamID& steamID, const bool bIsLan );

	/**
	*	Sets the auth ID string from a Steam2 ID string.
	*	@param pszAuthID Auth ID string.
	*	@param universe Universe that this auth ID belongs in.
	*/
	void Set( const char* const pszAuthID, EUniverse universe = k_EUniversePublic );

	/**
	*	Convert the auth ID to a Steam2 ID.
	*	@return Whether the conversion succeeded.
	*/
	bool ConvertToSteam2( TSteamGlobalUserID& userID );

	/**
	*	Convert the auth ID to a Steam3 ID.
	*	@param[ out ] steamID Steam ID to assign to. Will be cleared if conversion failed.
	*	@param universe Universe to assign to the Steam ID.
	*	@return Whether the conversion succeeded.
	*/
	bool ConvertToSteam3( CSteamID& steamID );

	bool operator==( const CSteam2IDString& other ) const;

	bool operator!=( const CSteam2IDString& other ) const;

private:
	char m_szString[ MAX_SIZE ] = {};
	EUniverse m_Universe = k_EUniverseInvalid;
};

inline CSteam2IDString::CSteam2IDString( const TSteamGlobalUserID& userID, EUniverse universe )
{
	Set( userID, universe );
}

inline CSteam2IDString::CSteam2IDString( const CSteamID& steamID, const bool bIsLan )
{
	Set( steamID, bIsLan );
}

inline CSteam2IDString::CSteam2IDString( const char* const pszAuthID, EUniverse universe )
{
	Set( pszAuthID, universe );
}

inline void CSteam2IDString::Set( const TSteamGlobalUserID& userID, EUniverse universe )
{
	const auto result = snprintf( m_szString, sizeof( m_szString ), "STEAM_%u:%u:%u",
								  userID.m_SteamInstanceID,
								  userID.m_SteamLocalUserID.Split.High32bits,
								  userID.m_SteamLocalUserID.Split.Low32bits
	);

	if( result < 0 || static_cast<size_t>( result ) >= sizeof( m_szString ) )
	{
		memset( m_szString, 0, sizeof( m_szString ) );
		m_Universe = k_EUniverseInvalid;
		return;
	}

	m_Universe = universe;
}

inline void CSteam2IDString::Set( const CSteamID& steamID, const bool bIsLan )
{
	if( !bIsLan )
	{
		if( steamID.ConvertToUint64() )
		{
			TSteamGlobalUserID userID;

			steamID.ConvertToSteam2( &userID );

			Set( userID, steamID.GetEUniverse() );
		}
		else
		{
			strcpy( m_szString, STEAM_ID_PENDING );
			m_Universe = k_EUniversePublic;
		}
	}
	else
	{
		strcpy( m_szString, STEAM_ID_LAN );
		m_Universe = k_EUniversePublic;
	}
}

inline void CSteam2IDString::Set( const char* const pszAuthID, EUniverse universe )
{
	bool bIsValid = false;

	if( pszAuthID && !*pszAuthID )
	{
		const auto length = strlen( pszAuthID );

		if( length < MAX_SIZE )
		{
			strcpy( m_szString, pszAuthID );
			m_Universe = universe;

			CSteamID steamID;

			bIsValid = ConvertToSteam3( steamID ) && steamID.IsValid();
		}
	}

	if( !bIsValid )
	{
		memset( m_szString, 0, sizeof( m_szString ) );
		m_Universe = k_EUniverseInvalid;
	}
}

inline bool CSteam2IDString::ConvertToSteam2( TSteamGlobalUserID& userID )
{
	//Reverse engineered from the GoldSource engine.
	userID.m_SteamInstanceID = 0;
	userID.m_SteamLocalUserID.As64bits = 0LL;

	const char* pszString = m_szString;

	if( strnicmp( m_szString, "STEAM_", 6 ) == 0 )
	{
		pszString += 6;
	}

	TSteamGlobalUserID steam2ID;
	steam2ID.m_SteamInstanceID = 0;
	steam2ID.m_SteamLocalUserID.As64bits = 0LL;
	char cExtraCharCheck = '\0';

	const auto result = sscanf( pszString, "%hu:%u:%u%c",
								&steam2ID.m_SteamInstanceID,
								&steam2ID.m_SteamLocalUserID.Split.High32bits,
								&steam2ID.m_SteamLocalUserID.Split.Low32bits,
								&cExtraCharCheck
	);

	if( result == -1 ||
		cExtraCharCheck ||
		result <= 1 ||
		( result == 2 && steam2ID.m_SteamInstanceID != 1 ) )
	{
		return false;
	}

	userID = steam2ID;

	return true;
}

inline bool CSteam2IDString::ConvertToSteam3( CSteamID& steamID )
{
	TSteamGlobalUserID userID;

	if( !ConvertToSteam2( userID ) )
	{
		steamID.Clear();
		return false;
	}

	steamID.SetFromSteam2( &userID, m_Universe );

	return true;
}

inline bool CSteam2IDString::operator==( const CSteam2IDString& other ) const
{
	return stricmp( m_szString, other.m_szString ) == 0 && m_Universe == other.m_Universe;
}

inline bool CSteam2IDString::operator!=( const CSteam2IDString& other ) const
{
	return !( *this == other );
}

#endif //CSTEAM2IDSTRING_H
