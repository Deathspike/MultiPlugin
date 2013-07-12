
	/**************************************************
	* MultiPlugin++ Module by Deathspike
	*
	* This is a MultiPlugin++ module. It contains the
	* MultiPlugin++ interface and executeable code.
	**************************************************/

	#include "../../SDK/cg_local.h"
	#include "../../mppShared.h"

	/**************************************************
	* MultiPlugin++ Shared Structures
	**************************************************/

	MultiPlugin_t	*sys;
	MultiSystem_t	 trap;

	/**************************************************
	* mpp
	*
	* Plugin exported function. This function gets called
	* the moment the module is loaded and provides a 
	* pointer to a shared structure. Store the pointer
	* and copy the System pointer safely.
	**************************************************/

	__declspec( dllexport ) void mpp( MultiPlugin_t *pPlugin )
	{
		memcpy( &trap, pPlugin->System, sizeof( MultiSystem_t ));
		sys = pPlugin;
	}

	/**************************************************
	* mppPreSystem
	*
	* Plugin exported function. This function gets called
	* from the game module to perform an action in the
	* engine, before going into the engine.
	**************************************************/

	__declspec( dllexport ) int mppPreSystem( int *arg )
	{
		switch( arg[0] )
		{
			case CG_R_ADDREFENTITYTOSCENE:
			{
				refEntity_t *refEntity = ( refEntity_t * ) arg[1];
				centity_t *cent = sys->mppIsPlayerEntity( sys->gameEntity );

				if ( !cent
					|| sys->inScreenshot
					|| cent->currentState.clientNum == sys->snap->ps.clientNum
					|| cent->currentState.eType == ET_BODY
					|| cent->currentState.eFlags & EF_DEAD
					|| refEntity->customShader 
					|| sys->mppIsPlayerInDuel( cent->currentState.clientNum ))
				{
					break;
				}

				if ( cent->currentState.clientNum != sys->gameEntity )
				{
					trap.R.AddRefEntityToScene( refEntity );
				}

				switch(( sys->clientInfo[cent->currentState.clientNum].superSmoothTime ) ? TEAM_BLUE : (( sys->mppIsPlayerAlly( cent->currentState.clientNum )) ? TEAM_FREE : TEAM_RED ))
				{
					case TEAM_RED:
					{
						refEntity->shaderRGBA[0] = 255;
						refEntity->shaderRGBA[1] = 0;
						refEntity->shaderRGBA[2] = 0;
						break;
					}

					case TEAM_BLUE:
					{
						refEntity->shaderRGBA[0] = 0;
						refEntity->shaderRGBA[1] = 0;
						refEntity->shaderRGBA[2] = 255;
						break;
					}

					default:
					{
						refEntity->shaderRGBA[0] = 0;
						refEntity->shaderRGBA[1] = 255;
						refEntity->shaderRGBA[2] = 0;
						break;
					}
				}

				refEntity->customShader = trap.R.RegisterShader( "gfx/misc/forceprotect" );
				refEntity->renderfx	|= RF_RGB_TINT;

				if ( cent->currentState.clientNum == sys->gameEntity )
				{
					refEntity->renderfx |= RF_FORCE_ENT_ALPHA;
				}
			}

			break;
		}

		return sys->noBreakCode;
	}