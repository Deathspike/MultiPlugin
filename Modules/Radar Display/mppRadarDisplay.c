
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
				int i, iIndex;

				for ( i = 0; i < MAX_CLIENTS; i++ )
				{
					iIndex = ( sys->cg_entities[i]->m_pVehicle ) ? sys->cg_entities[i]->m_pVehicle->m_pParentEntity->s.number : sys->cg_entities[i]->currentState.number;
					sys->cg_entities[iIndex]->currentState.eFlags |= EF_RADAROBJECT;
				}

				break;
			}
		}

		return sys->noBreakCode;
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
			case CG_R_SETRANGEFOG:
			{
				int i, iIndex;

				for ( i = 0; i < MAX_CLIENTS; i++ )
				{
					iIndex = ( sys->cg_entities[i]->m_pVehicle ) ? sys->cg_entities[i]->m_pVehicle->m_pParentEntity->s.number : sys->cg_entities[i]->currentState.number;
					sys->cg_entities[iIndex]->currentState.eFlags |= EF_RADAROBJECT;
				}

				break;
			}
		}

		return sys->noBreakCode;
	}