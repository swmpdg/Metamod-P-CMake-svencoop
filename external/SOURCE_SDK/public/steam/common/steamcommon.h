//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#ifndef INCLUDED_STEAM_COMMON_STEAMCOMMON_H
#define INCLUDED_STEAM_COMMON_STEAMCOMMON_H

#include "steamtypes.h"

/**
*	@file
*
*	Provides Steam2 compatibility support.
*/

/**
*	Steam2 Instance ID.
*/
typedef uint16 SteamInstanceID_t;

/**
*	Steam2 Local User ID.
*/
struct ISteamSplitLocalUserID
{
	uint32 Low32bits;
	uint32 High32bits;
};

/**
*	Steam2 ID.
*	Internal representation of a Steam2 ID.
*/
struct TSteamGlobalUserID
{
	SteamInstanceID_t m_SteamInstanceID;

	union
	{
		uint64 As64bits;
		ISteamSplitLocalUserID Split;
	}
	m_SteamLocalUserID;
};

#endif //INCLUDED_STEAM_COMMON_STEAMCOMMON_H
