#include <assert.h>
#include <string.h>

#include <extdll.h>
#include <meta_api.h>

#include "EngineIdent.h"

char* GetLibraryName( DLHANDLE handle, char* pszBuffer, const size_t uiSizeInCharacters, bool* pSuccess )
{
	assert( pszBuffer );
	assert( uiSizeInCharacters > 0 );

	if( pSuccess )
		*pSuccess = false;

#ifdef WIN32
	const DWORD uiLength = GetModuleFileNameA( handle, pszBuffer, uiSizeInCharacters );

	if( uiLength < uiSizeInCharacters )
	{
		if( pSuccess )
			*pSuccess = true;

		return pszBuffer;
	}

	//Buffer too small.

#else
	//TODO
#endif

	return nullptr;
}

bool CEngineIdent::Identify( void* pAddress )
{
	if( m_hHandle )
		return true;

	if( !pAddress )
	{
		META_WARNING( "CEngineIdent::Identify: Null address!" );
		return false;
	}

	DLHANDLE handle;
	
	char szFilename[ PATH_MAX ];

#ifdef WIN32
	{
		//On windows we can get a handle to the library using an address from it.
		if( FALSE == GetModuleHandleEx(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | 
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCTSTR>( pAddress ), &handle ) )
		{
			META_WARNING( "CEngineIdent::Identify: GetModuleHandleEx failed" );
			return false;
		}

		bool bSuccess;

		GetLibraryName( handle, szFilename, sizeof( szFilename ), &bSuccess );

		if( !bSuccess )
		{
			META_WARNING( "CEngineIdent::Identify: Couldn't get engine library name" );
			return false;
		}
	}

#else
	{
		Dl_info info;

		if( dladdr( gpGlobals, &info ) == 0 )
		{
			META_WARNING( "CEngineIdent::Identify: dladdr failed" );
			return false;
		}

		handle = dlopen( info.dli_fname, RTLD_NOW | RTLD_NOLOAD );

		if( handle == NULL )
		{
			META_WARNING( "CEngineIdent::Identify: Couldn't get engine library handle" );
			return false;
		}

		//Decrement reference count; engine loaded us.
		dlclose( handle );

		const size_t uiLength = strlen( info.dli_fname );

		if( uiLength >= sizeof( szFilename ) )
		{
			META_WARNING( "CEngineIdent::Identify: Library filename too large!\n" );
			return false;
		}

		strncpy( szFilename, info.dli_fname, sizeof( szFilename ) );
		szFilename[ sizeof( szFilename ) - 1 ] = '\0';
	}
#endif

	const size_t uiLength = strlen( szFilename );

	//Strip the path.
	{
		const char* pszIt = szFilename + uiLength - 1;

		for( ; pszIt != szFilename; --pszIt )
		{
			if( *pszIt == '/' || *pszIt == '\\' )
				break;
		}

		if( pszIt != szFilename )
		{
			//Also copies the null terminator.
			memmove( szFilename, pszIt + 1, strlen( pszIt + 1 ) + 1 );
		}
	}

	//Strip the extension.
	char* pszExtPos = strchr( szFilename, '.' );

	if( pszExtPos )
		*pszExtPos = '\0';

	//Strip the arch (Linux).
	char* pszArchPos = strrchr( szFilename, '_' );

	char szArch[ PATH_MAX ] = {};

	if( pszArchPos )
	{
		strncpy( szArch, pszArchPos + 1, sizeof( szArch ) );
		szArch[ sizeof( szArch ) - 1 ] = '\0';
		*pszArchPos = '\0';
	}

	if( !( *szFilename ) )
	{
		META_WARNING( "CEngineIdent::Identify: Couldn't identify the name of the engine library" );
		return false;
	}

	m_hHandle = handle;
	strncpy( m_szName, szFilename, sizeof( m_szName ) );
	m_szName[ sizeof( m_szName ) - 1 ] = '\0';

	strncpy( m_szArch, szArch, sizeof( m_szArch ) );
	m_szArch[ sizeof( m_szArch ) - 1 ] = '\0';

	return true;
}

const char* CEngineIdent::GetArchDescription() const
{
	//On Windows, there is no architecture specific engine version.
#ifdef WIN32
	return "Windows";
#else
	if( *m_szArch )
		return m_szArch;

	//On Linux, hw is the client engine library.
	if( strcmp( m_szName, "hw" ) == 0 )
		return "Linux";

	return "Unknown";
#endif
}
