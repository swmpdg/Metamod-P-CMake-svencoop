#include <string>
#include <vector>

#include <extdll.h>
#include <meta_api.h>

#include "EngineIdent.h"

std::string GetLibraryName( DLHANDLE handle, bool* pSuccess )
{
	if( pSuccess )
		*pSuccess = false;

	std::vector<char> vecFileName;

	vecFileName.resize( PATH_MAX );

#ifdef WIN32
	size_t uiCount = 0;

	//Double the buffer size up to 10 times before quitting.
	do
	{
		const DWORD uiLength = GetModuleFileNameA( handle, vecFileName.data(), vecFileName.size() );

		//Buffer too small.
		if( uiLength < vecFileName.size() )
		{
			if( pSuccess )
				*pSuccess = true;

			return std::string( vecFileName.begin(), vecFileName.end() );
		}

		const auto newSize = vecFileName.size() * 2;

		//Account for overflows.
		if( newSize < vecFileName.size() )
			return "";

		vecFileName.resize( newSize );
	}
	while( uiCount++ < 10 );

#else
	//TODO
#endif

	return "";
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
	std::string szFilename;

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

		szFilename = GetLibraryName( handle, &bSuccess );

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

		szFilename = info.dli_fname;
	}
#endif

	//Strip the path.
	auto it = szFilename.rbegin();
	auto end = szFilename.rend();

	for( ; it != end; ++it )
	{
		if( *it == '/' || *it == '\\' )
			break;
	}

	szFilename.erase( szFilename.begin(), it.base() );

	//Strip the extension.
	auto extPos = szFilename.find( '.' );

	if( extPos != std::string::npos )
		szFilename.resize( extPos );

	//Strip the arch (Linux).
	auto archPos = szFilename.rfind( '_' );

	std::string szArch;

	if( archPos != std::string::npos )
	{
		szArch = szFilename.substr( archPos + 1 );
		szFilename.resize( archPos );
	}

	if( szFilename.empty() )
	{
		META_WARNING( "CEngineIdent::Identify: Couldn't identify the name of the engine library" );
		return false;
	}

	//Name too large (should never happen).
	if( szFilename.size() >= sizeof( m_szName ) )
	{
		META_WARNING( "CEngineIdent::Identify: Engine library name is too large" );
		return false;
	}

	if( szArch.size() >= sizeof( m_szArch ) )
	{
		META_WARNING( "CEngineIdent::Identify: Engine library architecture name is too large" );
		return false;
	}

	m_hHandle = handle;
	strncpy( m_szName, szFilename.c_str(), sizeof( m_szName ) );
	m_szName[ sizeof( m_szName ) - 1 ] = '\0';

	strncpy( m_szArch, szArch.c_str(), sizeof( m_szArch ) );
	m_szArch[ sizeof( m_szArch ) - 1 ] = '\0';

	return true;
}

const char* CEngineIdent::GetArchDescription() const
{
	//On Windows, there is no architecture specific engine version.
#ifdef WIN32
	return "Windows";
#else
	return *m_szArch ? m_szArch : "Unknown";
#endif
}
