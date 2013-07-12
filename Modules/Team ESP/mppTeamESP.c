
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
	* InFront
	*
	* Determines wether or not the provided spot is in
	* front of the given origin with the angles. It allows
	* a treshhold to determine an arc.
	**************************************************/

	qboolean InFront( vec3_t spot, vec3_t from, vec3_t fromAngles, float threshHold )
	{
		vec3_t	dir, forward, angles;
		float	dot;

		VectorSubtract( spot, from, dir );
		dir[2] = 0;
		sys->VectorNormalize( dir );

		VectorCopy( fromAngles, angles );
		angles[0] = 0;
		sys->AngleVectors( angles, forward, NULL, NULL );

		dot = DotProduct( dir, forward );
		return ( qboolean )( dot > threshHold );
	}

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
	* mppPostMain
	*
	* Plugin exported function. This function gets
	* called after entering the main function in the
	* loaded game module.
	**************************************************/

	__declspec( dllexport ) int mppPostMain( int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 )
	{
		switch( cmd )
		{
			case CG_CONSOLE_COMMAND:
			{
				if ( sys->Q_stricmp( sys->Cmd_Argv( 0 ), "addText" ) == 0 )
				{
					if ( sys->Cmd_Argc() <= 1 )
					{
						sys->Com_Printf( "Sorry, this command more parameters.\n" );
					}
					else
					{
						sys->mppRenderTextAtVector( sys->snap->ps.origin, sys->Cmd_ConcatArgs( 1 ), qfalse, qtrue );
					}

					return qtrue;
				}

				if ( sys->Q_stricmp( sys->Cmd_Argv( 0 ), "addTextWall" ) == 0 )
				{
					if ( sys->Cmd_Argc() <= 1 )
					{
						sys->Com_Printf( "Sorry, this command requires more parameters.\n" );
					}
					else
					{
						sys->mppRenderTextAtVector( sys->snap->ps.origin, sys->Cmd_ConcatArgs( 1 ), qtrue, qtrue );
					}

					return qtrue;
				}

				break;
			}

			case CG_DRAW_ACTIVE_FRAME:
			{
				int i;

				for ( i = 0; i < MAX_CLIENTS; i++ )
				{
					centity_t *cent = sys->cg_entities[i];

					if ( i == sys->snap->ps.clientNum || !sys->currentValid[i] || cent->currentState.eFlags & EF_DEAD )
					{
						continue;
					}

					if ( cent->currentState.weapon == WP_SABER						// * Must be wielding a saber.
						&& cent->currentState.saberEntityNum						// * Must be holding his saber.
						&& !cent->currentState.saberInFlight						// * Must be holding his saber.
						&& cent->currentState.modelindex2 != WEAPON_RAISING			// * Must have saber in ready stance.
						&& cent->currentState.saberHolstered != 2					// * Must have saber activated.
						&& !cent->currentState.activeForcePass						// * Must not be using any flashy force.
						&& cent->currentState.saberMove >= LS_INVALID				// * Must not be in any kind of attack.
						&& cent->currentState.saberMove <= LS_READY					// * Must not be in any kind of attack.
						&& InFront( sys->snap->ps.origin, sys->currentOrigin[i], sys->cg_entities[i]->currentState.apos.trBase, 0.3f ))
					{
						sys->clientInfo[i].facial_aux = 1;
					}
					else
					{
						sys->clientInfo[i].facial_aux = 0;
					}

					sys->mppRenderTextAtEntity( i, sys->clientInfo[i].name, qfalse, qfalse );

					if ( sys->clientInfo[i].facial_aux )
					{
						sys->mppRenderTextAtEntity( i, "^1Invulnerable", qfalse, qfalse );
					}
				}

				break;
			}	
		}

		return sys->noBreakCode;
	}