
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
	* shader_s by BobaFett. My thanks!
	**************************************************/

	typedef struct shader_s
	{
	/*00*/		char	name[64];
	/*64*/
	/*68*/		
	/*72*/		char	unknown1[20]; // 20 bytes unaccounted for
	/*76*/
	/*80*/
	/*84*/		int		index;
	/*88*/		
	/*92*/		char	unknown2[16]; // 16 bytes unaccounted for
	/*96*/
	/*100*/
	/*104*/		char	defaultshader;
	/*105*/
	/*106*/
	/*107*/
	/*108*/
	/*112*/		
	/*116*/		char	unknown3[57]; // 57 bytes unaccounted for
	/*120*/
	/*124*/
	/*128*/
	/*132*/
	/*136*/
	/*140*/
	/*144*/
	/*148*/
	/*152*/
	/*156*/
	/*160*/
	/*162*/		short	 numUnfoggedPasses;
	/*164*/		void	*stages;  //shaderStage_t	*stages[MAX_SHADER_STAGES];		
	/*168*/		float	 clampTime;
	/*172*/		float	 timeOffset;		
	/*176*/		int		 unknown4;	// 4 bytes unaccounted for
	/*180*/		struct shader_s *remappedshader;
	/*184*/		struct shader_s *next;
	/*???*/		// Struct length unknown
	} shader_t;

	static long generateHashValue( const char *fname, const int size )
	{
		int		i;
		long	hash;
		char	letter;

		hash = 0;
		i = 0;

		while (fname[i] != '\0') {
			letter = tolower(fname[i]);
			if (letter =='.') break;				// don't include extension
			if (letter =='\\') letter = '/';		// damn path names
			if (letter == PATH_SEP) letter = '/';	// damn path names
			hash+=(long)(letter)*(i+119);
			i++;
		}

		hash = (hash ^ (hash >> 10) ^ (hash >> 20));
		hash &= (size-1);
		return hash;
	}

	void RemoveRemap( char *lpString )
	{
		long		 hash	= generateHashValue( lpString, 1024 );
		shader_t	*sh		= NULL;

		for ( sh = *( void ** )( 0x1072B90 + ( 0x4 * hash )); sh; sh = sh->next )
		{
			if ( sys->Q_stricmp( sh->name, lpString ) == 0 )
			{
				sh->remappedshader = NULL;
			}
		}
	}

	/**************************************************
	* remapData_s
	**************************************************/

	typedef struct remapData_s
	{

		char				*lpOldTexture;
		char				*lpNewTexture;
		int					 iLightIndex;
		struct remapData_s	*next;
		struct remapData_s	*prev;

	} remapData_t;

	/**************************************************
	* Remap data - Beginning, end, current entry, matches and such.
	**************************************************/

	remapData_t		*remapBegin	= NULL;
	remapData_t		*remapEntry	= NULL;
	remapData_t		*remapEnd	= NULL;
	qboolean		 bRemapped	= qfalse;
	int				 iMatched;
	int				*iLightIndex;

	/**************************************************
	* R_FindShader
	*
	* Detoured R_FindShader. Create a list of available
	* textures that require a change. A linked list
	* will contain all the required details!
	**************************************************/

	char *pR_FindShaderName; unsigned long pR_FindShader; __declspec( naked ) void R_FindShader()
	{
		__asm
		{
			push	eax
			mov		eax, [esp + 0x8]
			mov		pR_FindShaderName, eax
			pop		eax
			mov		iLightIndex, ecx
			pushad
		}

		if ( pR_FindShaderName && pR_FindShaderName[0] )
		{
			if ( remapEntry )
			{
				iLightIndex = &remapEntry->iLightIndex;
			}
			else
			{
				remapEntry	= remapEnd;
				iMatched	= 0;

				if( strstr( pR_FindShaderName, "textures" )
					&& !strstr( pR_FindShaderName, "skies" )
					&& !strstr( pR_FindShaderName, "sky" )
					&& !strstr( pR_FindShaderName, "glass" )
					&& !strstr( pR_FindShaderName, "fog" )
					&& !( strstr( pR_FindShaderName, "sfx" ) && strstr( pR_FindShaderName, "beam" )))
				{
					iMatched = 1;
				}
				else if( strstr( pR_FindShaderName, "skies" ) || strstr( pR_FindShaderName, "sky" ))
				{
					iMatched = 2;
				}
				
				while( remapEntry )
				{
					if ( sys->Q_stricmp( remapEntry->lpOldTexture, pR_FindShaderName ) == 0 )
					{
						iMatched = 0;
						break;
					}

					remapEntry = remapEntry->prev;
				}

				if ( iMatched )
				{
					if ( remapBegin == NULL )
					{
						remapBegin				= ( remapData_t * ) malloc( sizeof( remapData_t ));
						remapEnd				= remapBegin;
						remapBegin->next		= NULL;
						remapBegin->prev		= NULL;
					}
					else
					{
						remapEntry				= ( remapData_t * ) malloc( sizeof( remapData_t ));
						remapEnd->next			= remapEntry;
						remapEntry->next		= NULL;
						remapEntry->prev		= remapEnd;
						remapEnd				= remapEntry;
					}
					
					if ( iMatched == 1 )
					{
						remapEnd->lpNewTexture = "textures/impdetention/metal";
					}
					else
					{
						remapEnd->lpNewTexture = "textures/skies/boba_night";
					}

					remapEnd->iLightIndex	= ( int ) iLightIndex;
					remapEnd->lpOldTexture	= sys->CopyString( pR_FindShaderName );
				}

				remapEntry = NULL;
			}
		}

		__asm
		{
			popad
			mov		ecx, iLightIndex
			jmp		pR_FindShader
		}
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
			case CG_INIT:
			{
				sys->mppAttachClean(( unsigned long ) 0x4B5F00, ( unsigned long ) R_FindShader, ( unsigned long * ) &pR_FindShader, PATCH_JUMP );
				break;
			}

			case CG_DRAW_ACTIVE_FRAME:
			{
				if ( !bRemapped && !sys->inScreenshot )
				{
					remapEntry = remapBegin;

					while( remapEntry )
					{
						trap.R.RemapShader( remapEntry->lpOldTexture, remapEntry->lpNewTexture, 0 );
						remapEntry = remapEntry->next;
					}

					bRemapped = qtrue;
				}

				if ( bRemapped && sys->inScreenshot )
				{
					remapEntry = remapBegin;

					while( remapEntry )
					{
						RemoveRemap( remapEntry->lpOldTexture );
						remapEntry = remapEntry->next;
					}

					bRemapped = qfalse;
				}

				break;
			}
		}

		return sys->noBreakCode;
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
			case CG_SHUTDOWN:
			{
				sys->mppDetach(( unsigned long ) 0x4B5F00, pR_FindShader, qtrue );
				remapEntry = remapEnd;

				while( remapEntry )
				{
					if (( remapEntry = remapEntry->prev ) && remapEntry && remapEntry->next )
					{
						sys->FreeString( remapEntry->next->lpOldTexture );
						free( remapEntry->next );
					}
				}

				bRemapped	= qfalse;
				remapBegin	= NULL;
				remapEntry	= NULL;
				remapEnd	= NULL;
				break;
			}
		}

		return sys->noBreakCode;
	}