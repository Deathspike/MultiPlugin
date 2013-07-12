
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
	* CG_IsMindTricked
	*
	* Determines wether or not the client has been mindtricked.
	**************************************************/

	int CG_IsMindTricked( int trickIndex1, int trickIndex2, int trickIndex3, int trickIndex4, int client )
	{
		int checkIn;
		int sub = 0;

		if ( client > 47 )
		{
			checkIn = trickIndex4;
			sub = 48;
		}
		else if ( client > 31 )
		{
			checkIn = trickIndex3;
			sub = 32;
		}
		else if ( client > 15 )
		{
			checkIn = trickIndex2;
			sub = 16;
		}
		else
		{
			checkIn = trickIndex1;
		}

		if ( checkIn & ( 1 << ( client - sub )))
		{
			return 1;
		}
		
		return 0;
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
	* mppPreMain
	*
	* Plugin exported function. This function gets
	* called before entering the main function in the
	* loaded game module.
	**************************************************/

	__declspec( dllexport ) int mppPreMain( int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 )
	{
		switch( cmd )
		{
			case CG_DRAW_ACTIVE_FRAME:
			{
				int i;

				if ( sys->inScreenshot )
				{
					break;
				}

				for ( i = 0; i < MAX_CLIENTS; i++ )
				{
					centity_t *cent = sys->cg_entities[i];

					if ( !sys->currentValid[i] )
					{
						continue;
					}

					if ( CG_IsMindTricked( cent->currentState.trickedentindex, cent->currentState.trickedentindex2, cent->currentState.trickedentindex3, cent->currentState.trickedentindex4, sys->snap->ps.clientNum ))
					{
						if ( sys->snap->ps.clientNum > 47 )
						{
							cent->currentState.trickedentindex4	&= ~( 1 << ( sys->snap->ps.clientNum - 48 ));
						}
						else if ( sys->snap->ps.clientNum > 31 )
						{
							cent->currentState.trickedentindex3	&= ~( 1 << ( sys->snap->ps.clientNum - 32 ));
						}
						else if ( sys->snap->ps.clientNum > 15 )
						{
							cent->currentState.trickedentindex2	&= ~( 1 << ( sys->snap->ps.clientNum - 16 ));
						}
						else
						{
							cent->currentState.trickedentindex	&= ~( 1 << sys->snap->ps.clientNum );
						}

						sys->clientInfo[i].superSmoothTime = sys->snap->serverTime;
					}
					else if ( sys->clientInfo[i].superSmoothTime && sys->snap->serverTime != sys->clientInfo[i].superSmoothTime )
					{
						sys->clientInfo[i].superSmoothTime = 0;
					}
				}

				break;
			}
		}

		return sys->noBreakCode;
	}