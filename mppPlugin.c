	
	/**************************************************
	* MultiPlugin++ by Deathspike
	*
	* Client-side plugin project which allows any
	* developer to use this framework as a basis for
	* their own plugin.
	**************************************************/

	#include "mppHeader.h"

	/**************************************************
	* mppPluginInit
	*
	* Initializes the plugin structure for any new plugin,
	* this is done so we can seperate the given memory
	* for each plugin. One plugin cannot screw up another,
	* in this case.
	**************************************************/

	void mppPluginInit( MultiPlugin_t *pPlugin )
	{
		int	i								= 0;
		pPlugin->noBreakCode				= 1001;

		/**************************************************
		* Core Function Pointers
		*
		* These are function pointers provided by MultiPlugin
		* itself, and not fitting in any other section.
		**************************************************/

		pPlugin->pCGMainCall				= MultiPlugin.pCGMainCall;
		pPlugin->pCGSystemCall				= MultiPlugin.pCGSystemCall;
		pPlugin->pUIMainCall				= MultiPlugin.pUIMainCall;
		pPlugin->pUISystemCall				= MultiPlugin.pUISystemCall;

		/**************************************************
		* Detour Function Pointers
		* 
		* Collection of functions written to assist in detouring
		* functions and patching bytes. It mainly allocates
		* trampolines to execute the original bytes and uses
		* jumps to go to the new function.
		**************************************************/

		pPlugin->mppAttach					= &mppAttach;
		pPlugin->mppAttachClean				= &mppAttachClean;
		pPlugin->mppDisassemble				= &mppDisassemble;
		pPlugin->mppDetach					= &mppDetach;
		pPlugin->mppGetLen					= &mppGetLen;
		pPlugin->mppGetTramp				= &mppGetTramp;
		pPlugin->mppInlineFetch				= &mppInlineFetch;
		pPlugin->mppInlinePatch				= &mppInlinePatch;
		pPlugin->mppPatch					= &mppPatch;
		pPlugin->mppReProtect				= &mppReProtect;
		pPlugin->mppUnProtect				= &mppUnProtect;

		/**************************************************
		* Game Function Pointers
		*
		* These function pointers show custom functions able
		* to perform a useful task easily and swiftly. For example,
		* find a cvar or command in the engine.
		**************************************************/

		pPlugin->mppAimAtVector				= &mppAimAtVector;
		pPlugin->mppIsPlayerAlly			= &mppIsPlayerAlly;
		pPlugin->mppIsPlayerEntity			= &mppIsPlayerEntity;
		pPlugin->mppIsPlayerInDuel			= &mppIsPlayerInDuel;
		pPlugin->mppLocateCommand			= &mppLocateCommand;
		pPlugin->mppLocateCommand			= &mppLocateCommand;
		pPlugin->mppRawTextCalculateDraw	= &mppRawTextCalculateDraw;
		pPlugin->mppRawTextCalculateHeight	= &mppRawTextCalculateHeight;
		pPlugin->mppRawTextCalculateWidth	= &mppRawTextCalculateWidth;
		pPlugin->mppRenderTextAtEntity		= &mppRenderTextAtEntity;
		pPlugin->mppRenderTextAtVector		= &mppRenderTextAtVector;
		pPlugin->mppRenderTextClear			= &mppRenderTextClear;
		pPlugin->mppRenderTextFinalize		= &mppRenderTextFinalize;
		pPlugin->mppRenderTextVisible		= &mppRenderTextVisible;
		pPlugin->mppWorldToScreen			= &mppWorldToScreen;
		pPlugin->mppSystemCall				= &mppSystemCall;

		/**************************************************
		* Game Pointers
		*
		* These are data segments filled in by MultiPlugin++ 
		* to avoid having to rely on unreliable data addresses,
		* which modifications are able to alter.
		**************************************************/

		pPlugin->currentOrigin				= &currentOrigin[0];
		pPlugin->currentValid				= &currentValid[0];
		pPlugin->clientInfo					= &clientInfo[0];
		pPlugin->refdef						= &refdef;
		pPlugin->snap						= &snap;

		/**************************************************
		* Entity Pointers
		* 
		* These are the entities which are being registered
		* for reading. Modifications can alter almost all
		* of the addresses in here, be cautious and avoid
		* using them if you plan to use modifications.
		**************************************************/

		for ( i = 0; i < MAX_GENTITIES; i++ )
		{
			pPlugin->cg						= *( cg_t **  )		&MultiPlugin.cg;
			pPlugin->cgs					= *( cgs_t ** )		&MultiPlugin.cgs;
			pPlugin->cg_entities[i]			= *( centity_t ** ) &MultiPlugin.cg_entities[i];
		}

		/**************************************************
		* SDK Function Pointers
		*
		* Imports functions from the SDK, which are useful
		* and should be available here. Please note that
		* these are only core functions.
		**************************************************/

		pPlugin->AngleNormalize360			= &AngleNormalize360;
		pPlugin->AngleNormalize180			= &AngleNormalize180;
		pPlugin->AngleDelta					= &AngleDelta;
		pPlugin->AnglesToAxis				= &AnglesToAxis;
		pPlugin->AngleVectors				= &AngleVectors;
		pPlugin->Cmd_Argc					= &Cmd_Argc;
		pPlugin->Cmd_Argv					= &Cmd_Argv;
		pPlugin->Cmd_ConcatArgs				= &Cmd_ConcatArgs;
		pPlugin->Com_Error					= &Com_Error;
		pPlugin->Com_Printf					= &Com_Printf;
		pPlugin->Com_Sprintf				= &Com_Sprintf;
		pPlugin->Com_StringEscape			= &Com_StringEscape;
		pPlugin->CopyString					= &CopyString;
		pPlugin->FreeString					= &FreeString;
		pPlugin->Info_RemoveKey				= &Info_RemoveKey;
		pPlugin->Info_SetValueForKey		= &Info_SetValueForKey;
		pPlugin->Info_ValueForKey			= &Info_ValueForKey;
		pPlugin->Q_CleanStr					= &Q_CleanStr;
		pPlugin->Q_stricmpn					= &Q_stricmpn;
		pPlugin->Q_stricmp					= &Q_stricmp;
		pPlugin->Q_strncpyz					= &Q_strncpyz;
		pPlugin->va							= &va;
		pPlugin->vectoangles				= &vectoangles;
		pPlugin->VectorNormalize			= &VectorNormalize;
	}

	/**************************************************
	* mppPluginCGMain
	* 
	* This is where all the calls from the engine into
	* the module go, so we are free to manipulate it
	* in any way we see fit. We, however, choose to forward
	* it to our plugins.
	**************************************************/

	int mppPluginCGMain( int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 )
	{
		MultiModule_t	*pMultiModule	= MultiModule;
		int				 retCall		= 0;
		int				 retValue		= MultiPlugin.noBreakCode;
		int				 retFlag		= 0;

		/**************************************************
		* Run internal functionality in pre-execution mode.
		**************************************************/

		switch( cmd )
		{
			case CG_INIT:
			{
				DllDetach();
				mppScreenshotDetour();
				break;
			}

			case CG_CONSOLE_COMMAND:
			{
				char *cmd = Cmd_Argv( 0 );

				if ( Q_stricmp( cmd, "toggleVisual" ) == 0 )
				{
					if ( MultiPlugin.inScreenshot )
					{
						MultiPlugin.inScreenshot = 0;
					}
					else
					{
						MultiPlugin.inScreenshot = -1;
					}

					return qtrue;
				}

				break;
			}

			case CG_DRAW_ACTIVE_FRAME:
			{

				if ( MultiPlugin.inScreenshot && MultiPlugin.inScreenshot != -1 )
				{
					mppScreenshotCall();
				}

				break;
			}
		}

		/**************************************************
		* Run module functionality in pre-execution mode.
		**************************************************/

		while( pMultiModule )
		{
			if ( pMultiModule->pPreMainCall )
			{
				retCall = pMultiModule->pPreMainCall( cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11 );
				retValue = ( retCall != MultiPlugin.noBreakCode ) ? retCall : retValue;
			}

			pMultiModule = ( MultiModule_t * ) pMultiModule->pPreviousPlugin;
		}

		/**************************************************
		* Run game module functionality which is basically
		* the core of the actual game. We're simply wrapping
		* around it.
		**************************************************/

		if ( retValue == MultiPlugin.noBreakCode )
		{
			DllStealth( cgVM, qtrue );
			retFlag = MultiPlugin.pCGMainCall( cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11 );
			DllStealth( cgVM, qfalse );
		}

		/**************************************************
		* Run internal functionality in post-execution mode.
		**************************************************/

		switch( cmd )
		{
			case CG_DRAW_ACTIVE_FRAME:
			{
				mppRenderTextFinalize();
				mppRawTextCalculateDraw( "^1Multi^7Plugin++ by ^1Death^7spike", 320, 470, 0.5, trap_R_RegisterFont( "ergoec" ), 0 );
				break;
			}

			case CG_SHUTDOWN:
			{
				DllAttach();
				break;
			}
		}

		/**************************************************
		* Rewind the module access list so post-execution works.
		**************************************************/

		pMultiModule = MultiModule;

		/**************************************************
		* Run module functionality in post-execution mode.
		**************************************************/

		while( pMultiModule )
		{
			if ( pMultiModule->pPostMainCall )
			{
				retCall = pMultiModule->pPostMainCall( cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11 );
				retValue = ( retCall != MultiPlugin.noBreakCode ) ? retCall : retValue;
			}

			pMultiModule = ( MultiModule_t * ) pMultiModule->pPreviousPlugin;
		}

		return ( retValue != MultiPlugin.noBreakCode ) ? retValue : retFlag;
	}

	/**************************************************
	* mppPluginCGSystem
	*
	* This is where a call is being made from the module
	* to the engine, and we can alter its response in
	* any way we see fit. Even prevent it, whatever.
	**************************************************/

	int mppPluginCGSystem( int *arg )
	{
		MultiModule_t	*pMultiModule	= MultiModule;
		int				 retCall		= 0;
		int				 retValue		= MultiPlugin.noBreakCode;
		int				 retFlag		= 0;

		/**************************************************
		* Run internal functionality in pre-execution mode.
		**************************************************/

		switch( arg[0] )
		{
			case CG_GETCURRENTSNAPSHOTNUMBER:
			{
				MultiPlugin.cg = ( cg_t * ) (( int ) DllPointer( cgVM, arg[2] ) - 32 );
				break;
			}

			case CG_GETGAMESTATE:
			{
				MultiPlugin.cgs = ( cgs_t * ) DllPointer( cgVM, arg[1] );
				break;
			}

			case CG_GETDEFAULTSTATE:
			{
				MultiPlugin.cg_entities[arg[1]] = ( centity_t * ) DllPointer( cgVM, arg[2] );
				break;
			}

			case CG_R_RENDERSCENE:
			{
				refdef_t *fd = ( refdef_t * ) arg[1];
				
				if ( fd->rdflags & RDF_DRAWSKYBOX )
				{
					memcpy( &refdef, fd, sizeof( refdef_t ));
				}

				break;
			}

			case CG_S_UPDATEENTITYPOSITION:
			{
				if ( arg[1] >= 0 && arg[1] < MAX_GENTITIES )
				{
					MultiPlugin.gameEntity = ( int ) arg[1];
					VectorCopy( *( vec3_t * ) arg[2], currentOrigin[MultiPlugin.gameEntity] );
				}

				break;
			}
		}

		/**************************************************
		* Rewind the module access list so post-execution works.
		**************************************************/

		pMultiModule = MultiModule;

		/**************************************************
		* Run module functionality in pre-execution mode.
		**************************************************/

		while( pMultiModule )
		{
			if ( pMultiModule->pPreSystemCall )
			{
				retCall	= pMultiModule->pPreSystemCall( arg );
				retValue = ( retCall != MultiPlugin.noBreakCode ) ? retCall : retValue;
			}

			pMultiModule = ( MultiModule_t * ) pMultiModule->pPreviousPlugin;
		}

		/**************************************************
		* Run game module functionality which is basically
		* the core of the actual game. We're simply wrapping
		* around it.
		**************************************************/

		if ( retValue == MultiPlugin.noBreakCode )
		{
			DllStealth( cgVM, qtrue );
			retFlag = MultiPlugin.pCGSystemCall( arg );
			DllStealth( cgVM, qfalse );
		}

		/**************************************************
		* Run internal functionality in post-execution mode.
		**************************************************/

		switch( arg[0] )
		{
			case CG_GETGAMESTATE:
			{
				int i, iNum = atoi( Cmd_Argv( 1 ));
				gameState_t *gameState = ( gameState_t * ) arg[1];

				if ( iNum < CS_PLAYERS && iNum >= ( CS_PLAYERS + MAX_CLIENTS ))
				{
					break;
				}

				for ( i = 0; i < MAX_CLIENTS; i++ )
				{
					const char *configstring = configstring = ( const char * )( gameState->stringData + gameState->stringOffsets[CS_PLAYERS + i] );
					clientInfo_t *ci = &clientInfo[i];

					if ( !configstring[0] || Info_ValueForKey( configstring, "n" ) == NULL )
					{
						memset( ci, 0, sizeof( clientInfo_t ));
						continue;
					}

					Q_strncpyz( ci->name, Info_ValueForKey( configstring, "n" ), sizeof( ci->name ));
					Q_strncpyz( ci->modelName, Info_ValueForKey( configstring, "model" ), sizeof( ci->modelName ));
					Q_strncpyz( ci->forcePowers, Info_ValueForKey( configstring, "forcepowers" ), sizeof( ci->forcePowers ));
					Q_strncpyz( ci->saberName, Info_ValueForKey( configstring, "st" ), sizeof( ci->saberName ));
					Q_strncpyz( ci->saber2Name, Info_ValueForKey( configstring, "st2" ), sizeof( ci->saber2Name ));

					ci->infoValid			= qtrue;
					ci->icolor1				= atoi( Info_ValueForKey( configstring, "c1" ));
					ci->icolor2				= atoi( Info_ValueForKey( configstring, "c2" ));
					ci->botSkill			= atoi( Info_ValueForKey( configstring, "skill" ));
					ci->handicap			= atoi( Info_ValueForKey( configstring, "hc" ));
					ci->wins				= atoi( Info_ValueForKey( configstring, "w" ));
					ci->losses				= atoi( Info_ValueForKey( configstring, "l" ));
					ci->teamTask			= atoi( Info_ValueForKey( configstring, "tt" ));
					ci->teamLeader			= ( qboolean ) atoi( Info_ValueForKey( configstring, "tl" ));
					ci->siegeDesiredTeam	= atoi( Info_ValueForKey( configstring, "sdt" ));
					ci->duelTeam			= atoi( Info_ValueForKey( configstring, "dt" ));

					if ( strlen( Info_ValueForKey( configstring, "MBClass" )))
					{
						if ( atoi( Info_ValueForKey( configstring, "t" )) == TEAM_RED || atoi( Info_ValueForKey( configstring, "t" )) == TEAM_BLUE )
						{
							ci->team = atoi( Info_ValueForKey( configstring, "t" ));
						}

						ci->siegeDesiredTeam = atoi( Info_ValueForKey( configstring, "MBClass" ));
					}
					else
					{
						ci->team = atoi( Info_ValueForKey( configstring, "t" ));
					}
				}

				break;
			}

			case CG_GETSNAPSHOT:
			{
				if ( snap.serverCommandSequence < arg[1] )
				{
					int i;

					memset( &currentValid, 0, sizeof( qboolean ) * MAX_CLIENTS );
					memset( &snap, 0, sizeof( snapshot_t ));
					memcpy( &snap, ( snapshot_t * ) arg[2], sizeof( snapshot_t ));

					for ( i = 0; i < MAX_GENTITIES; i++ )
					{
						currentValid[i] = qfalse;
					}

					for ( i = 0; i < snap.numEntities; i++ )
					{
						currentValid[snap.entities[i].number] = qtrue;
					}
				}

				break;
			}
		}

		/**************************************************
		* Run module functionality in post-execution mode.
		**************************************************/

		while( pMultiModule )
		{
			if ( pMultiModule->pPostSystemCall )
			{
				retCall	= pMultiModule->pPostSystemCall( arg );
				retValue = ( retCall != MultiPlugin.noBreakCode ) ? retCall : retValue;
			}

			pMultiModule = ( MultiModule_t * ) pMultiModule->pPreviousPlugin;
		}

		return ( retValue != MultiPlugin.noBreakCode ) ? retValue : retFlag;
	}

	/**************************************************
	* mppPluginUIMain
	* 
	* This is where all the calls from the engine into
	* the module go, so we are free to manipulate it
	* in any way we see fit. We, however, choose to forward
	* it to our plugins.
	**************************************************/

	int mppPluginUIMain( int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 )
	{
		int	retFlag;
		DllStealth( uiVM, qtrue );
		retFlag = MultiPlugin.pUIMainCall( cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11 );
		DllStealth( uiVM, qtrue );
		return retFlag;
	}

	/**************************************************
	* mppPluginUISystem
	*
	* This is where a call is being made from the module
	* to the engine, and we can alter its response in
	* any way we see fit. Even prevent it, whatever.
	**************************************************/

	int mppPluginUISystem( int *arg )
	{
		int retFlag;
		DllStealth( uiVM, qtrue );
		retFlag = MultiPlugin.pUISystemCall( arg );
		DllStealth( uiVM, qfalse );
		return retFlag;
	}

	/**************************************************
	* mppPluginLoad
	* 
	* This loads all the modules into memory, writes
	* down the path and name, retrieves a handle, allocates
	* the possible functions and hides the module.
	**************************************************/

	void mppPluginLoad( char *zModule )
	{
		int iLen = 0;

		/**************************************************
		* Store the previous loaded module and allocate the
		* space for a new entry. Clear this newly allocated
		* space for proper writing.
		**************************************************/
		
		MultiModule_t *pMultiModule = MultiModule;								// Retrieve the latest added module.
		MultiModule = ( MultiModule_t * ) malloc( sizeof( MultiModule_t ));		// Allocate some space to add a new module.
		memset( MultiModule, 0, sizeof( MultiModule_t ));						// Clean up the new space and zero everything.

		/**************************************************
		* Determine the path of this module and load it up,
		* so we can use it for future reference.
		**************************************************/

		iLen = strlen( zModule );
		MultiModule->zPath = malloc( iLen + 1 );
		memset( MultiModule->zPath, 0, iLen + 1 );
		memcpy( MultiModule->zPath, zModule, iLen );

		/**************************************************
		* Fill in the pointers for both the previous and
		* this module, so they point to each other. Also store
		* the handle for this module.
		**************************************************/

		if ( pMultiModule ) pMultiModule->pNextPlugin = ( void * ) MultiModule;	// Fill in the 'next' label of the latest added module.
		MultiModule->pPreviousPlugin = ( void * ) pMultiModule;					// Fill in the 'previous' label of the newly added module.
		MultiModule->hMod = LoadLibrary( zModule );								// Store the handle of this module for future reference.

		/**************************************************
		* See if we can find an entry point to give the
		* MultiPlugin structure and system call pointers.
		**************************************************/

		if (( MultiModule->pEntryCall = ( void ( * )( MultiPlugin_t * )) GetProcAddress(( HMODULE ) MultiModule->hMod, "mpp" )) != NULL )
		{
			// Allocate some memory to inform this module of functions.
			MultiSystem_t pSystem;

			// Generate the plugin definitions and system definitions.
			mppSystemInit( &pSystem );

			// Link the system pointer to this system struct.
			MultiPlugin.System = ( void * ) &MultiSystem;

			// Perform the entry call. When we're done, everything is free.
			MultiModule->pEntryCall( &MultiPlugin );
		}

		/**************************************************
		* Determine the function pointers and finally
		* hide this module from the PEB. We're done now!
		**************************************************/

		MultiModule->pPreMainCall		= ( int ( * )( int, int, int, int, int, int, int, int, int, int, int, int, int )) GetProcAddress( MultiModule->hMod, "mppPreMain" );
		MultiModule->pPostMainCall		= ( int ( * )( int, int, int, int, int, int, int, int, int, int, int, int, int )) GetProcAddress( MultiModule->hMod, "mppPostMain" );

		MultiModule->pPreSystemCall		= ( int ( * )( int * )) GetProcAddress(( HMODULE ) MultiModule->hMod, "mppPreSystem" );
		MultiModule->pPostSystemCall	= ( int ( * )( int * )) GetProcAddress(( HMODULE ) MultiModule->hMod, "mppPostSystem" );

		DllHide(( HMODULE ) MultiModule->hMod );
	}