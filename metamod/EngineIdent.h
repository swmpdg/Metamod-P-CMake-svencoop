#ifndef METAMOD_ENGINEIDENT_H
#define METAMOD_ENGINEIDENT_H

#include "osdep.h"

/**
*	Information about the engine.
*/
class CEngineIdent
{
public:
	CEngineIdent() DLLINTERNAL = default;

	/**
	*	Identify the engine.
	*/
	bool DLLINTERNAL Identify( void* pAddress );

	DLHANDLE DLLINTERNAL GetHandle() { return m_hHandle; }

	const char* DLLINTERNAL GetName() const { return m_szName; }

	const char* DLLINTERNAL GetArchitecture() const { return m_szArch; }

	const char* DLLINTERNAL GetArchDescription() const;

private:
	DLHANDLE m_hHandle = NULL;

	char m_szName[ PATH_MAX ] = {};
	char m_szArch[ PATH_MAX ] = {};

private:
	CEngineIdent( const CEngineIdent& ) = delete;
	CEngineIdent& operator=( const CEngineIdent& ) = delete;
};

#endif //METAMOD_ENGINEIDENT_H
