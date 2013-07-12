
	/**************************************************
	* MultiPlugin++ by Deathspike
	*
	* Client-side plugin project which allows any
	* developer to use this framework as a basis for
	* their own plugin.
	**************************************************/

	#include "mppHeader.h"

	/**************************************************
	* This is the Shared Buffer across all the modules,
	* and contains about any function you'd might wish.
	**************************************************/

	MultiPlugin_t	 MultiPlugin;
	MultiSystem_t	 MultiSystem;
	MultiModule_t	*MultiModule;
	MultiRender_t	 pRenders[1024];
	int				 iRenders = 0;

	/**************************************************
	* These are data segments filled in by MultiPlugin++ 
	* to avoid having to rely on unreliable data addresses,
	* which modifications are able to alter.
	**************************************************/

	clientInfo_t	clientInfo[MAX_CLIENTS];
	vec3_t			currentOrigin[MAX_GENTITIES];
	qboolean		currentValid[MAX_GENTITIES];
	refdef_t		refdef;
	snapshot_t		snap;

	/**************************************************
	* These pointers contain the location of the VM for
	* the target module (cgame) and the addresses of
	* the real functions.
	**************************************************/

	HMODULE	 hModule;
	vm_t	*cgVM;
	vm_t	*uiVM;

	/**************************************************
	* The pointers point to a trampoline memory segment,
	* containing the originally overwritten bytes followed
	* by a jump to continue the code execution.
	**************************************************/

	vm_t	*( *pDllCreate )( char *, int *, int );
	void	*( *pDllFree )( vm_t * );
	void	*( *pDllLoadDll )( unsigned long );

	/**************************************************
	* DirectSoundCreate
	*
	* Exported function. We use this as our entry point
	* into the application. Due to LoadLibrary, this
	* module gets loaded instead. This replacement
	* function will simply call the original.
	**************************************************/

	long __stdcall DirectSoundCreate( LPCGUID lpcGUID, void *ppDS, void *pUnkOuter )
	{
		long ( __stdcall *pDirectSoundCreate )( LPCGUID, void *, void * );
		char *zSoundPath = ( char * ) malloc( MAX_PATH );

		/* Hide this module, we do not want to leave any trace we were here! */
		DllHide( hModule );

		/* Get the windows directory and create the absolute path.. */
		RtlZeroMemory( zSoundPath, MAX_PATH );
		GetWindowsDirectoryA( zSoundPath, MAX_PATH );
		RtlCopyMemory( zSoundPath + strlen( zSoundPath ), "\\System32\\dsound.dll", 20 );

		/* Get the real address and free the path buffer. */
		pDirectSoundCreate = ( HRESULT ( WINAPI * )( LPCGUID, void *, void * )) GetProcAddress( LoadLibraryA( zSoundPath ), "DirectSoundCreate" );
		free( zSoundPath );

		/* Create the sound and return its results. */
		return pDirectSoundCreate( lpcGUID, ppDS, pUnkOuter );
	}

	/**************************************************
	* DllAttach
	* 
	* This gets called as soon as possible, patches
	* the entire CD-key checking algorithme and attaches
	* the required detours for 
	**************************************************/

	void DllAttach()
	{
		mppPatch(( unsigned long ) 0x45148C, ( void * ) 0x00, 1 );	// Disable the tampering flag. /*0x451486 + 6*/
		mppPatch(( unsigned long ) 0x41D9B8, ( void * ) 0xB8, 1 );	// Disable the first device call.
		mppPatch(( unsigned long ) 0x45DE61, ( void * ) 0xB8, 1 );	// Disable the second device call.
		mppPatch(( unsigned long ) 0x52C329, ( void * ) 0xB8, 1 );	// Disable the last device call.
		*( unsigned long * ) 0xB8D544 = 0;							// Disable the tamping flag if it was on!

		mppAttachClean(( unsigned long ) ADDRESS_VM_CREATE		, ( unsigned long ) &DllCreate	, ( unsigned long * ) &pDllCreate	, PATCH_JUMP	);
		mppAttachClean(( unsigned long ) ADDRESS_VM_FREE		, ( unsigned long ) &DllFree	, ( unsigned long * ) &pDllFree		, PATCH_JUMP	);
		mppAttachClean(( unsigned long ) ADDRESS_VM_LOAD_DLL	, ( unsigned long ) &DllLoadDll	, ( unsigned long * ) &pDllLoadDll	, PATCH_JUMP	);
	}

	/**************************************************
	* DllCallDll
	* 
	* When a DLL is loaded, both the UI and CG module
	* will use this function for system calls. It will
	* fix the expected argument stack and make the
	* call to the MultiPlugin function(s).
	**************************************************/

	int DllCallDll( int iCmd, ... )
	{
		static int	*arg[16];
		va_list		 arg_list;
		int			 iReturn, i;
		vm_t		*pVM = ( *( vm_t ** )( ADDRESS_ENGINE_SWITCH ));

		va_start( arg_list, iCmd );
		for ( i = 1; i <= 15; i++ ) arg[i] = ( int * ) va_arg( arg_list, int );
		va_end( arg_list );

		arg[0]	= ( int * ) iCmd;
		iReturn	= ( pVM == uiVM ) ? mppPluginUISystem(( int * ) arg ) : mppPluginCGSystem(( int * ) arg );

		for ( i = 0; i < 15; i++ ) arg[i] = 0;
		return iReturn;
	}

	/**************************************************
	* DllCreate
	* 
	* Creates any initial VM, which is a DLL in this case.
	* This struct contains pointers to the functions
	* assumed to be correct, therefore we can change these.
	**************************************************/
	
	vm_t *DllCreate( char *module, int *systemCalls, int interpret )
	{
		vm_t *pReturn = pDllCreate( module, systemCalls, interpret );

		if ( Q_stricmp( module, "cgame" ) == 0 && ( cgVM = pReturn ))
		{
			MultiPlugin.pCGSystemCall = cgVM->systemCall;
			cgVM->systemCall = &mppPluginCGSystem;

			if( cgVM->entryPoint )
			{
				MultiPlugin.pCGMainCall = ( int ( * )( int, int, int, int, int, int, int, int, int, int, int, int, int )) cgVM->entryPoint;
				cgVM->entryPoint = ( int ( * )( int, ... )) &mppPluginCGMain;
			}
			else
			{
				MultiPlugin.pCGMainCall = ( int ( * )( int, int, int, int, int, int, int, int, int, int, int, int, int )) cgVM->codeBase;
				cgVM->codeBase = ( BYTE * ) &mppPluginCGMain;
			}
		}

		if ( Q_stricmp( module, "ui" ) == 0 && ( uiVM = pReturn ))
		{
			MultiPlugin.pUISystemCall = uiVM->systemCall;
			uiVM->systemCall = &mppPluginUISystem;

			if( uiVM->entryPoint )
			{
				MultiPlugin.pUIMainCall = ( int ( * )( int, int, int, int, int, int, int, int, int, int, int, int, int )) uiVM->entryPoint;
				uiVM->entryPoint = ( int ( * )( int, ... )) &mppPluginUIMain;
			}
			else
			{
				MultiPlugin.pUIMainCall = ( int ( * )( int, int, int, int, int, int, int, int, int, int, int, int, int )) uiVM->codeBase;
				uiVM->codeBase = ( BYTE * ) &mppPluginUIMain;
			}
		}

		return pReturn;
	}

	/**************************************************
	* DllDetach
	* 
	* Detach our placed detours, to avoid detection as
	* much as possible.
	**************************************************/

	void DllDetach()
	{
		mppPatch(( unsigned long ) 0x45148C, ( void * ) 0x01, 1 );	// Enable the tampering flag. /*0x451486 + 6*/
		mppPatch(( unsigned long ) 0x41D9B8, ( void * ) 0xE8, 1 );	// Enable the first device call.
		mppPatch(( unsigned long ) 0x45DE61, ( void * ) 0xE8, 1 );	// Enable the second device call.
		mppPatch(( unsigned long ) 0x52C329, ( void * ) 0xE8, 1 );	// Enable the last device call.

		mppDetach(( unsigned long ) ADDRESS_VM_CREATE	, ( unsigned long ) pDllCreate	, qtrue	);
		mppDetach(( unsigned long ) ADDRESS_VM_FREE		, ( unsigned long ) pDllFree	, qtrue	);
		mppDetach(( unsigned long ) ADDRESS_VM_LOAD_DLL	, ( unsigned long ) pDllLoadDll	, qtrue	);
	}

	/**************************************************
	* DllFree
	* 
	* A VM is being freed, it is best for us to unload
	* our child modules as well. They should have received
	* the shutdown signal, so a complete reboot isn't all
	* that bad - depending on the type of plugin.
	**************************************************/

	void DllFree( vm_t *vm )
	{
		if ( vm == cgVM )
		{
			cgVM						= NULL;
			MultiPlugin.pCGMainCall		= NULL;
			MultiPlugin.pCGSystemCall	= NULL;
		}

		if ( vm == uiVM )
		{
			uiVM						= NULL;
			MultiPlugin.pUIMainCall		= NULL;
			MultiPlugin.pUISystemCall	= NULL;
		}

		pDllFree( vm );
	}

	/**************************************************
	* DllHide
	* 
	* Hides a module by changing the entry in the PEB,
	* all the API calls rely on this being correct so
	* we'd rather force our own to be broken, and thus
	* to be hidden. Created by 'PizzaPan'!
	* 
	* http://forum.gamedeception.net/showthread.php?t=7743
	**************************************************/

	void DllHide( HMODULE hMod )
	{
		DWORD dwPEB_LDR_DATA = 0;

		__asm
		{
			pushad
			pushfd
			mov		eax, fs:[30h]					// PEB
			mov		eax, [eax+0Ch]					// PEB->ProcessModuleInfo
			mov		dwPEB_LDR_DATA, eax				// Save ProcessModuleInfo

			mov		esi, [eax+0Ch]					// ProcessModuleInfo->InLoadOrderModuleList[FORWARD]
			mov		edx, [eax+10h]					// ProcessModuleInfo->InLoadOrderModuleList[BACKWARD]

	LoopInLoadOrderModuleList: 

			lodsd									// Load First Module
			mov		esi, eax						// ESI points to Next Module
			mov		ecx, [eax+18h]					// LDR_MODULE->BaseAddress
			cmp		ecx, hMod						// Is it Our Module ?
			jne		SkipA		    				// If Not, Next Please (@f jumps to nearest Unamed Lable @@:)
			mov		ebx, [eax]						// [FORWARD] Module 
			mov		ecx, [eax+4]    				// [BACKWARD] Module
			mov		[ecx], ebx						// Previous Module's [FORWARD] Notation, Points to us, Replace it with, Module++
			mov		[ebx+4], ecx					// Next Modules, [BACKWARD] Notation, Points to us, Replace it with, Module--
			jmp		InMemoryOrderModuleList			// Hidden, so Move onto Next Set

		SkipA:

			cmp		edx, esi						// Reached End of Modules ?
			jne		LoopInLoadOrderModuleList		// If Not, Re Loop

	InMemoryOrderModuleList:

			mov		eax, dwPEB_LDR_DATA				// PEB->ProcessModuleInfo
			mov		esi, [eax+14h]					// ProcessModuleInfo->InMemoryOrderModuleList[START]
			mov		edx, [eax+18h]					// ProcessModuleInfo->InMemoryOrderModuleList[FINISH]

		LoopInMemoryOrderModuleList: 

			lodsd
			mov		esi, eax
			mov		ecx, [eax+10h]
			cmp		ecx, hMod
			jne		SkipB
			mov		ebx, [eax] 
			mov		ecx, [eax+4]
			mov		[ecx], ebx
			mov		[ebx+4], ecx
			jmp		InInitializationOrderModuleList

		SkipB:

			cmp		edx, esi
			jne		LoopInMemoryOrderModuleList

	InInitializationOrderModuleList:

			mov		eax, dwPEB_LDR_DATA				// PEB->ProcessModuleInfo
			mov		esi, [eax+1Ch]					// ProcessModuleInfo->InInitializationOrderModuleList[START]
			mov		edx, [eax+20h]					// ProcessModuleInfo->InInitializationOrderModuleList[FINISH]

		LoopInInitializationOrderModuleList: 

			lodsd
			mov		esi, eax		
			mov		ecx, [eax+08h]
			cmp		ecx, hMod		
			jne		SkipC
			mov		ebx, [eax] 
			mov		ecx, [eax+4]
			mov		[ecx], ebx
			mov		[ebx+4], ecx
			jmp		Finished

		SkipC:

			cmp edx, esi
			jne LoopInInitializationOrderModuleList

	Finished:

			popfd
			popad
		}
	}

	/**************************************************
	* DllLoadDll
	* 
	* Second phase when creating a VM, but this will
	* only apply on a DLL. Before sending the address
	* to the module function, alter the address to
	* have it call to my own function.
	**************************************************/

	char *pModule; __declspec( naked ) void *DllLoadDll( unsigned long systemCalls )
	{
		__asm
		{
			mov		pModule, eax
		}

		if ( Q_stricmp( pModule, "cgame" ) == 0 )
		{
			__asm
			{
				lea		eax, DllCallDll
				mov		[esp + 8h], eax
			}
		}

		if ( Q_stricmp( pModule, "ui" ) == 0 )
		{
			__asm
			{
				lea		eax, DllCallDll
				mov		[esp + 8h], eax
			}
		}

		__asm
		{
			mov		eax, pModule
			jmp		pDllLoadDll
		}
	}

	/**************************************************
	* DllMain
	* 
	* Application entry-point, does all the required
	* initialization and hooking for this project.
	**************************************************/

	int __stdcall DllMain( void *hMod, unsigned long fdwReason, void *lpvReserved )
	{
		switch ( fdwReason )
		{
			case DLL_PROCESS_ATTACH:
			{
				if ( Q_stricmp(( char * ) ADDRESS_ENGINE_STRING, "Jedi Knight®: Jedi Academy (MP)" ))
				{
					CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE ) FreeLibrary, hMod, 0, NULL );
					return 0;
				}

				mppPluginInit( &MultiPlugin );	// Initialize the plugin structure.
				mppSystemInit( &MultiSystem );	// Initialize the system structure.

				DllAttach();					// Attach the detours to the game.
				DllPlugin(( HMODULE ) hMod );	// Builds the list of plugins.
				hModule = ( HMODULE ) hMod;		// Save the module handle to be hidden.
				break;
			}

			case DLL_PROCESS_DETACH:
			{
				DllDetach();
				break;
			}
		}

		return 1;
	}

	/**************************************************
	* DllPlugin
	* 
	* Builds the list of plugins for this module, does
	* not load them up though. This will be done when
	* the initialization call is coming in.
	**************************************************/

	void DllPlugin( HMODULE hMod )
	{
		WIN32_FIND_DATA sFile;
		HANDLE hFile;
		char zPath[1024];

		// FIXME: Unreliable and stupid method to get the path.

		GetModuleFileNameA( hMod, zPath, sizeof( zPath ));
		memset( zPath + strlen( zPath ) - 10, 0, 10 );

		sprintf_s( zPath, sizeof( zPath ), "%s\\MultiPlugin++\\*.dll", zPath );
		hFile = FindFirstFile( zPath, &sFile );
		memset( zPath + strlen( zPath ) - 6, 0, 4 );

		if( hFile == INVALID_HANDLE_VALUE )
		{
			return;
		}

		do
		{

			mppPluginLoad( va( "%s\\%s", zPath, sFile.cFileName ));
			
		} while( FindNextFile( hFile, &sFile ));

		FindClose( hFile );
	}

	/**************************************************
	* DllPointer
	* 
	* Retrieves a correct pointer for the data segment
	* and whatever address we might have requested. Really
	* useful to break-and-enter into structs.
	**************************************************/

	void *DllPointer( vm_t *pVM, int x )
	{
		if ( pVM->entryPoint ) 
		{
			return ( void * )( pVM->dataBase + x );
		}

		return ( pVM->dataBase + ( x & pVM->dataMask ));
	}

	/**************************************************
	* DllStealth
	* 
	* Enables or disables the stealth measures, which
	* effectively changes the VM pointers to their original
	* state, allowing us to go by undetected when checking
	* from the cgame module. Other modules (or threads)
	* can easily detect the change.
	**************************************************/

	void DllStealth( vm_t *pVM, qboolean bEnableStealth )
	{
		if ( pVM == cgVM )
		{
			pVM->entryPoint = ( int ( * )( int, ... ))(( bEnableStealth ) ? MultiPlugin.pCGMainCall : &mppPluginCGMain );
			pVM->systemCall = ( bEnableStealth ) ? MultiPlugin.pCGSystemCall : &mppPluginCGSystem;
		}

		if ( pVM == uiVM )
		{
			pVM->entryPoint = ( int ( * )( int, ... ))(( bEnableStealth ) ? MultiPlugin.pUIMainCall : &mppPluginUIMain );
			pVM->systemCall = ( bEnableStealth ) ? MultiPlugin.pUISystemCall : &mppPluginUISystem;
		}
	}