
	/**************************************************
	* MultiPlugin++ by Deathspike
	*
	* Client-side plugin project which allows any
	* developer to use this framework as a basis for
	* their own plugin.
	**************************************************/

	#define PATCH_CALL	0xE8
	#define PATCH_JUMP	0xE9

	/**************************************************
	* cmd_t
	*
	* The internal structure used for commands in the
	* engine. Each key binding such as "+attack" or commands
	* such as "screenshot" can be located in this struct,
	* and of course detoured.
	**************************************************/

	typedef struct cmd_function_s
	{

		struct cmd_function_s	*next;
		char					*name;
		void					( *function )( void );

	} cmd_t;

	/**************************************************
	* MultiPlugin_t
	* 
	* The main plugin structure, holds all the important
	* variables, pointers and functions. It also includes
	* the MultiSystem structure, passed down to this
	* module.
	**************************************************/

	typedef struct {

		int				 noBreakCode;
		int				 gameEntity;
		int				 inScreenshot;

		/**************************************************
		* Core Function Pointers
		*
		* These are function pointers provided by MultiPlugin
		* itself, and not fitting in any other section.
		**************************************************/

		int				 ( *pCGMainCall )( int, int, int, int, int, int, int, int, int, int, int, int, int );
		int				 ( *pCGSystemCall )( int * );

		int				 ( *pUIMainCall )( int, int, int, int, int, int, int, int, int, int, int, int, int );
		int				 ( *pUISystemCall )( int * );

		/**************************************************
		* Detour Function Pointers
		* 
		* Collection of functions written to assist in detouring
		* functions and patching bytes. It mainly allocates
		* trampolines to execute the original bytes and uses
		* jumps to go to the new function.
		**************************************************/

		unsigned long	 ( *mppAttach )( unsigned long dwAddress, unsigned long dwAddressTarget, char iType );
		void			 ( *mppAttachClean )( unsigned long dwAddress, unsigned long dwAddressTarget, unsigned long *dwAddressTrampoline, int iType );
		void			 ( *mppDisassemble )( unsigned char *iptr0, unsigned long *osizeptr );
		unsigned long	 ( *mppDetach )( unsigned long pAddress, unsigned long pTramp, qboolean bFreeTrampoline );
		unsigned long	 ( *mppGetLen )( unsigned long Address );
		unsigned long	 ( *mppGetTramp )( unsigned long pAddress, unsigned int iLen );
		unsigned long	 ( *mppInlineFetch )( unsigned long pAddress, unsigned int iLen );
		unsigned long	 ( *mppInlinePatch )( unsigned long pAddress, unsigned long pNewAddress, unsigned int iLen );
		void			 ( *mppPatch )( unsigned long pAddress, void *bByte, int iLen );
		void			 ( *mppReProtect )( unsigned long pAddress, unsigned int iLen );
		void			 ( *mppUnProtect )( unsigned long pAddress, unsigned int iLen );

		/**************************************************
		* Game Function Pointers
		*
		* These function pointers show custom functions able
		* to perform a useful task easily and swiftly. For example,
		* find a cvar or command in the engine.
		**************************************************/

		void			 ( *mppAimAtVector )( vec3_t targOrigin );
		qboolean		 ( *mppIsPlayerAlly )( int iIndex );
		qboolean		 ( *mppIsPlayerInDuel )( int iIndex );
		centity_t		*( *mppIsPlayerEntity )( int iIndex );
		cmd_t			*( *mppLocateCommand )( char *zName );
		cvar_t			*( *mppLocateCvar )( char *zName );
		void			 ( *mppRawTextCalculateDraw )( char *lpString, float x, float y, float fScale, int iFont, int iLines );
		int				 ( *mppRawTextCalculateHeight )( int iFont, float fScale );
		int				 ( *mppRawTextCalculateWidth )( char *lpString, int iFont, float fScale );
		int				 ( *mppRenderTextAtEntity )( int iIndex, char *lpString, qboolean bCheckWall, qboolean bPersistant );
		int				 ( *mppRenderTextAtVector )( vec3_t origin, char *lpString, qboolean bCheckWall, qboolean bPersistant );
		void			 ( *mppRenderTextClear )( int iIndex );
		void			 ( *mppRenderTextFinalize )( void );
		qboolean		 ( *mppRenderTextVisible )( vec3_t origin, qboolean checkWalls );
		qboolean		 ( *mppWorldToScreen )( vec3_t point, float *x, float *y );
		int				 ( *mppSystemCall )( int iCmd, ... );

		/**************************************************
		* Game Pointers
		*
		* These are data segments filled in by MultiPlugin++ 
		* to avoid having to rely on unreliable data addresses,
		* which modifications are able to alter.
		**************************************************/

		clientInfo_t	*clientInfo;
		vec3_t			*currentOrigin;
		qboolean		*currentValid;
		refdef_t		*refdef;
		snapshot_t		*snap;

		/**************************************************
		* Entity Pointers
		* 
		* These are the entities which are being registered
		* for reading. Modifications can alter almost all
		* of the addresses in here, be cautious and avoid
		* using them if you plan to use modifications.
		**************************************************/

		cg_t			*cg;							/* Pseudo-access - Unreliable for some modifications */
		cgs_t			*cgs;							/* Pseudo-access - Unreliable for some modifications */
		centity_t		*cg_entities[MAX_GENTITIES];	/* Peeuso-access - currentState is reliable, rest might not */

		/**************************************************
		* SDK Function Pointers
		*
		* Imports functions from the SDK, which are useful
		* and should be available here. Please note that
		* these are only core functions.
		**************************************************/

		float			 ( *AngleNormalize360 )( float angle );
		float			 ( *AngleNormalize180 )( float angle );
		float			 ( *AngleDelta )( float angle1, float angle2 );
		void			 ( *AnglesToAxis )( const vec3_t angles, vec3_t axis[3] );
		void			 ( *AngleVectors )( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up );
		int				 ( *Cmd_Argc )( void );
		const char		*( *Cmd_Argv )( int arg );
		char			*( *Cmd_ConcatArgs )( int start );
		void			 ( *Com_Error )( int level, const char *error, ... );
		void			 ( *Com_Printf )( const char *msg, ... );
		void			 ( *Com_Sprintf )( char *dest, int size, const char *fmt, ... );
		void			 ( *Com_StringEscape )( char *in, char *out );
		char			*( *CopyString )( const char *in );
		void			 ( *FreeString )( char *lpString );
		void			 ( *Info_RemoveKey )( char *s, const char *key );
		void			 ( *Info_SetValueForKey )( char *s, const char *key, const char *value );
		char			*( *Info_ValueForKey )( const char *s, const char *key );
		char			*( *Q_CleanStr )( char *string );
		int				 ( *Q_stricmpn )( const char *s1, const char *s2, int n );
		int				 ( *Q_stricmp )( const char *s1, const char *s2 );
		void			 ( *Q_strncpyz )( char *dest, const char *src, int destsize );
		char			*( *va )( const char *format, ... );
		void			 ( *vectoangles )( const vec3_t value1, vec3_t angles );
		vec_t			 ( *VectorNormalize )( vec3_t v );

		/**************************************************
		* System Call Function Pointers
		*
		* All of the system calls have been indexed and
		* sorted for your ease of use. The syntax is slightly
		* altered so remove all ugly ones.
		**************************************************/

		void			*System;

	} MultiPlugin_t;

	/**************************************************
	* MultiSystem_t
	**************************************************/

	typedef struct {

		void 			( *AddCommand )( const char *cmdName );
		void			( *AddWeatherZone )( const vec3_t mins, const vec3_t maxs );
		int 			( *Argc )( void );
		void 			( *Args )( char *buffer, int bufferLength );
		void 			( *Argv )( int n, char *buffer, int bufferLength );
		void 			( *Error )( const char *fmt );
		int 			( *GetCurrentCmdNumber )( void );
		void 			( *GetCurrentSnapshotNumber )( int *snapshotNumber, int *serverTime );
		qboolean 		( *GetDefaultState )( int entityIndex, entityState_t *state );
		qboolean		( *GetEntityToken )( char *buffer, int bufferSize );
		void 			( *GetGameState )( gameState_t *gamestate );
		void 			( *GetGlconfig )( glconfig_t *glconfig );
		qboolean 		( *GetServerCommand )( int serverCommandNumber );
		qboolean 		( *GetSnapshot )( int snapshotNumber, snapshot_t *snapshot );
		int				( *GetStringTextString )( const char *text, char *buffer, int bufferLength );
		qboolean 		( *GetUserCmd )( int cmdNumber, usercmd_t *ucmd );
		void			( *Init )( int terrainID, const char *terrainInfo );
		void			( *InitRendererTerrain )( const char *info );
		int				( *MemoryRemaining )( void );
		int 			( *Milliseconds )( void );
		void			( *OpenUIMenu )( int menuID );
		void 			( *Print )( const char *fmt );
		int				( *RealTime )( qtime_t *qtime );
		void			( *RegisterSharedMemory )( char *memory );
		void 			( *RemoveCommand )( const char *cmdName );
		void 			( *SendConsoleCommand )( const char *text );
		void			( *SendClientCommand )( const char *s );
		void			( *SetClientForceAngle )( int time, vec3_t angle );
		void			( *SetClientTurnExtent )( float turnAdd, float turnSub, int turnTime );
		void			( *SetUserCmdValue )( int stateValue, float sensitivityScale, float mPitchOverride, float mYawOverride, float mSensitivityOverride, int fpSel, int invenSel, qboolean fighterControls );
		void			( *SnapVector )( float *v );
		void			( *TrueMalloc )( void **ptr, int size );
		void			( *TrueFree )( void **ptr );
		void			( *UpdateScreen )( void );

		struct {

			void			( *AddPrecacheEntry )( const char *name );
			sfxHandle_t		( *GetBModelSound )( const char *name, int stage );
			void			( *ParseSets )( void );

		} AS;

		struct {

			void			( *DrawCinematic )( int handle );
			int				( *PlayCinematic )( const char *arg0, int xpos, int ypos, int width, int height, int bits );
			e_status		( *RunCinematic )( int handle );
			void			( *SetExtents )( int handle, int x, int y, int w, int h );
			e_status		( *StopCinematic )( int handle );

		} CIN;
		
		struct {

			void 			( *BoxTrace )( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask );
			void 			( *CapsuleTrace )( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask );
			clipHandle_t	( *InlineModel )( int index );
			void			( *LoadMap )( const char *mapname, qboolean SubBSP );
			int				( *NumInlineModels )( void );
			int 			( *MarkFragments )( int numPoints, const vec3_t *points, const vec3_t projection, int maxPoints, vec3_t pointBuffer, int maxFragments, markFragment_t *fragmentBuffer );
			int 			( *PointContents )( const vec3_t p, clipHandle_t model );
			int				( *RegisterTerrain )( const char *config );
			clipHandle_t	( *TempBoxModel )( const vec3_t mins, const vec3_t maxs );
			clipHandle_t	( *TempCapsuleModel )( const vec3_t mins, const vec3_t maxs );	
			void 			( *TransformedBoxTrace )( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask, const vec3_t origin, const vec3_t angles );
			void 			( *TransformedCapsuleTrace )( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask, const vec3_t origin, const vec3_t angles );
			int				( *TransformedPointContents )( const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles );

		} CM;

		struct {

			int 			( *GetHiddenVarValue )( const char *name );
			void 			( *Register )( vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags );
			void 			( *Set )( const char *var_name, const char *value );
			void 			( *Update )( vmCvar_t *vmCvar );
			void 			( *VariableStringBuffer )( const char *var_name, char *buffer, int bufsize );

		} Cvar;

		struct {

			void 			( *FCloseFile )( fileHandle_t f );
			int  			( *FOpenFile )( const char *qpath, fileHandle_t *f, fsMode_t mode );
			int 			( *GetFileList )(  const char *path, const char *extension, char *listbuf, int bufsize );
			void 			( *Read )( void *buffer, int len, fileHandle_t f );
			void 			( *Write )( const void *buffer, int len, fileHandle_t f );

		} FS;

		struct {

			void 			( *AddBezier )( addbezierArgStruct_t *p );
			void 			( *AddElectricity )( addElectricityArgStruct_t *p );
			void 			( *AddLine )( const vec3_t start, const vec3_t end, float size1, float size2, float sizeParm, float alpha1, float alpha2, float alphaParm, const vec3_t sRGB, const vec3_t eRGB, float rgbParm, int killTime, qhandle_t shader, int flags );
			void 			( *AddPoly )( addpolyArgStruct_t *p );
			void 			( *AddPrimitive )( effectTrailArgStruct_t *p );
			void 			( *AddScheduledEffects )( qboolean skyPortal );
			void 			( *AddSprite )( addspriteArgStruct_t *p );
			void 			( *AdjustTime )( int time );
			void 			( *Draw2DEffects  )( float screenXScale, float screenYScale );
			qboolean		( *FreeSystem )( void );
			int 			( *InitSystem )( refdef_t* refdef );
			int				( *RegisterEffect )( const char *file );
			void 			( *PlayBoltedEffectID )( int id, vec3_t org, void *ghoul2, const int boltNum, const int entNum, const int modelNum, int iLooptime, qboolean isRelative );
			void			( *PlayEffect )( const char *file, vec3_t org, vec3_t fwd, int vol, int rad );
			void 			( *PlayEntityEffectID )( int id, vec3_t org, vec3_t axis[3], const int boltInfo, const int entNum, int vol, int rad );
			void 			( *PlayEffectID )( int id, vec3_t org, vec3_t fwd, int vol, int rad );
			void			( *PlayEntityEffect )( const char *file, vec3_t org, vec3_t axis[3], const int boltInfo, const int entNum, int vol, int rad );
			void 			( *PlayPortalEffectID )( int id, vec3_t org, vec3_t fwd, int vol, int rad );
			void 			( *Reset )( void );
			void 			( *SetRefDef )( refdef_t* refdef );	

		} FX;

		struct {

			qboolean 		( *HaveWeGhoul2Models )( void *ghoul2 );
			void			( *ListModelSurfaces )( void *ghlInfo );
			void 			( *ListModelBones )( void *ghlInfo, int frame );
			void 			( *SetGhoul2ModelIndexes )( void *ghoul2, qhandle_t *modelList, qhandle_t *skinList );

		} G2;

		struct {

			void			( *AbsurdSmoothing )( void *ghoul2, qboolean status );
			int				( *AddBolt )( void *ghoul2, int modelIndex, const char *boneName );
			void			( *AddSkinGore )( void *ghlInfo,SSkinGoreData *gore );
			void			( *AnimateG2Models )( void *ghoul2, int time, sharedRagDollUpdateParams_t *params );
			qboolean		( *AttachEnt )( int *boltInfo, void *ghlInfoTo, int toBoltIndex, int entNum, int toModelNum );
			void			( *AttachInstanceToEntNum )( void *ghoul2, int entityNum, qboolean server );
			void			( *ClearAttachedInstance )( int entityNum );
			void			( *CleanEntAttachments )( void );
			void			( *CleanGhoul2Models )( void **ghoul2Ptr );
			void			( *ClearSkinGore  )( void* ghlInfo );
			void			( *CollisionDetect )( CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, const vec3_t rayStart, const vec3_t rayEnd, const vec3_t scale, int traceFlags, int useLod, float fRadius );
			void 			( *CollisionDetectCache )( CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, const vec3_t rayStart, const vec3_t rayEnd, const vec3_t scale, int traceFlags, int useLod, float fRadius );
			int 			( *CopyGhoul2Instance )( void *g2From, void *g2To, int modelIndex );
			void 			( *CopySpecificGhoul2Model )( void *g2From, int modelFrom, void *g2To, int modelTo );
			qboolean		( *DoesBoneExist )( void *ghoul2, int modelIndex, const char *boneName );
			void 			( *DuplicateGhoul2Instance )( void *g2From, void **g2To );
			qboolean 		( *GetBoltMatrix )( void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale );
			qboolean 		( *GetBoltMatrix_NoReconstruct )( void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale );
			qboolean 		( *GetBoltMatrix_NoRecNoRot )( void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale );
			qboolean 		( *GetBoneAnim )( void *ghoul2, const char *boneName, const int currentTime, float *currentFrame, int *startFrame, int *endFrame, int *flags, float *animSpeed, int *modelList, const int modelIndex );
			qboolean 		( *GetBoneFrame )( void *ghoul2, const char *boneName, const int currentTime, float *currentFrame, int *modelList, const int modelIndex );
			void 			( *GetGLAName )( void *ghoul2, int modelIndex, char *fillBuf );
			qboolean		( *GetRagBonePos )( void *ghoul2, const char *boneName, vec3_t pos, vec3_t entAngles, vec3_t entPos, vec3_t entScale );
			int				( *GetNumGoreMarks )( void *ghlInfo, int modelIndex );
			int				( *GetTime )( void );
			void			( *GetSurfaceName )( void *ghoul2, int surfNumber, int modelIndex, char *fillBuf );
			int				( *GetSurfaceRenderStatus )( void *ghoul2, const int modelIndex, const char *surfaceName );
			int				( *Ghoul2Size  )( void* ghlInfo );
			qboolean		( *HasGhoul2ModelOnIndex )( void *ghlInfo, int modelIndex );
			qboolean		( *IKMove )( void *ghoul2, int time, sharedIKMoveParams_t *params );
			int 			( *InitGhoul2Model )( void **ghoul2Ptr, const char *fileName, int modelIndex, qhandle_t customSkin, qhandle_t customShader, int modelFlags, int lodBias );
			qboolean		( *OverrideServer )( void *serverInstance );
			qboolean		( *RagEffectorGoal )( void *ghoul2, const char *boneName, vec3_t pos );
			qboolean		( *RagEffectorKick )( void *ghoul2, const char *boneName, vec3_t velocity );
			qboolean		( *RagForceSolve )( void *ghoul2, qboolean force );
			qboolean		( *RagPCJConstraint )( void *ghoul2, const char *boneName, vec3_t min, vec3_t max );
			qboolean		( *RagPCJGradientSpeed )( void *ghoul2, const char *boneName, const float speed );
			qboolean		( *RemoveBone )( void *ghoul2, const char *boneName, int modelIndex );
			qboolean		( *RemoveGhoul2Model )( void *ghlInfo, int modelIndex );
			void			( *SetBoltInfo )( void *ghoul2, int modelIndex, int boltInfo );
			qboolean 		( *SetBoneAnim )( void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame , const int blendTime );
			qboolean 		( *SetBoneAngles )( void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags, const int up, const int right, const int forward, qhandle_t *modelList, int blendTime , int currentTime );
			qboolean		( *SetBoneIKState )( void *ghoul2, int time, const char *boneName, int ikState, sharedSetBoneIKStateParams_t *params );
			qboolean		( *SetRootSurface )( void *ghoul2, const int modelIndex, const char *surfaceName );
			qboolean		( *SetNewOrigin )( void *ghoul2, const int boltIndex );
			void			( *SetRagDoll )( void *ghoul2, sharedRagDollParams_t *params );
			qboolean 		( *SetSkin )( void *ghoul2, int modelIndex, qhandle_t customSkin, qhandle_t renderSkin );
			qboolean		( *SetSurfaceOnOff )( void *ghoul2, const char *surfaceName, const int flags );
			void			( *SetTime )( int time, int clock );
			qboolean		( *SkinlessModel )( void *ghlInfo, int modelIndex );
			
		} G2API;

		struct {

			int				( *GetCatcher )( void );
			int				( *GetKey )( const char *binding );
			qboolean		( *IsDown )( int keynum );
			void			( *SetCatcher )( int catcher );

		} Key;

		struct {

			qboolean		( *IsAsian )( void );
			unsigned int	( *ReadCharFromString )( const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation/* = NULL*/ );
			qboolean		( *UsesSpaces )( void );

		} Language;

		struct {

			int 			( *AddGlobalDefine )( char *define );
			int 			( *FreeSource )( int handle );
			int 			( *LoadGlobalDefines )( const char* filename );
			int 			( *LoadSource )( const char *filename );
			int 			( *ReadToken )( int handle, pc_token_t *pc_token );
			void 			( *RemoveAllGlobalDefines )( void );
			int 			( *SourceFileAndLine )( int handle, char *filename, int *line );

		} PC;

		struct {

			int 			( *End )( void *theTimer );
			void 			( *Start )( void **theNewTimer );

		} PrecisionTimer;

		struct {

			void 			( *AutomapElevAdj )( float newHeight );
			void 			( *AddAdditiveLightToScene )( const vec3_t org, float intensity, float r, float g, float b );
			void 			( *AddDecalToScene  )( qhandle_t shader, const vec3_t origin, const vec3_t dir, float orientation, float r, float g, float b, float a, qboolean alphaFade, float radius, qboolean temporary );
			void 			( *AddLightToScene )( const vec3_t org, float intensity, float r, float g, float b );
			void 			( *AddPolyToScene )( qhandle_t hShader , int numVerts, const polyVert_t *verts );
			void 			( *AddPolysToScene )( qhandle_t hShader , int numVerts, const polyVert_t *verts, int num );
			void 			( *AddRefEntityToScene )( const refEntity_t *re );
			void 			( *ClearDecals )( void );
			void 			( *ClearScene )( void );
			void 			( *DrawRotatePic )( float x, float y, float w, float h, float s1, float t1, float s2, float t2,float a, qhandle_t hShader );
			void 			( *DrawRotatePic2 )( float x, float y, float w, float h, float s1, float t1, float s2, float t2,float a, qhandle_t hShader );
			void 			( *DrawStretchPic )( float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader );

			struct {

				void		( *DrawString )( int ox, int oy, const char *text, const float *rgba, const int setIndex, int iCharLimit, const float scale );
				int			( *HeightPixels )( const int iFontIndex, const float scale );
				int			( *StrLenChars )( const char *text );
				int			( *StrLenPixels )( const char *text, const int iFontIndex, const float scale );

			} Font;

			void 			( *GetBModelVerts )( int bmodelIndex, vec3_t *verts, vec3_t normal );
			void 			( *GetDistanceCull )( float *f );
			void 			( *GetLightStyle )( int style, color4ub_t color );
			void 			( *GetRealRes )( int *w, int *h );
			qboolean 		( *InitWireframeAutomap )( void );
			qboolean		( *InPVS )( const vec3_t p1, const vec3_t p2, byte *mask );
			int 			( *LerpTag )( orientation_t *tag, clipHandle_t mod, int startFrame, int endFrame, float frac, const char *tagName );
			int 			( *LightForPoint )( vec3_t point, vec3_t ambientLight, vec3_t directedLight, vec3_t lightDir );
			void			( *LoadWorldMap )( const char *mapname );
			void 			( *ModelBounds )( clipHandle_t model, vec3_t mins, vec3_t maxs );
			void 			( *RenderScene )( const refdef_t *fd );
			void 			( *RemapShader )( const char *oldShader, const char *newShader, const char *timeOffset );
			qhandle_t		( *RegisterFont )( const char *fontName );
			qhandle_t		( *RegisterModel )( const char *name );
			qhandle_t		( *RegisterShader )( const char *name );
			qhandle_t		( *RegisterShaderNoMip )( const char *name );
			qhandle_t		( *RegisterSkin )( const char *name );		
			void 			( *SetColor )( const float *rgba );
			void 			( *SetLightStyle )( int style, int color );
			void 			( *SetRangeFog )( float range );
			void 			( *SetRefractProp )( float alpha, float stretch, qboolean prepost, qboolean negate );
			void			( *WeatherContentsOverride )( int contents );
			void			( *WorldEffectCommand )( const char *cmd );

		} R;

		struct {

			int				( *Cache )( char *file );
			qboolean		( *Clean )( void );
			qboolean		( *Play )( int entID, int roffID, qboolean doTranslation );
			qboolean		( *PurgeEnt )( int entID );
			void			( *UpdateEntities )( void );

		} ROFF;

		struct {

			int				( *AddLocalSet )( const char *name, vec3_t listener_origin, vec3_t origin, int entID, int time );
			void 			( *AddLoopingSound )( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx );
			void 			( *AddRealLoopingSound )( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx );
			void 			( *ClearLoopingSounds )( void );
			int 			( *GetVoiceVolume )( int entityNum );
			void 			( *MuteSound )( int entityNum, int entchannel );
			sfxHandle_t		( *RegisterSound )( const char *sample );
			void 			( *Respatialize )( int entityNum, const vec3_t origin, vec3_t axis[3], int inwater );
			void 			( *ShutUp )( qboolean shutUpFactor );
			void 			( *StartBackgroundTrack )( const char *intro, const char *loop, qboolean bReturnWithoutStarting );
			void 			( *StartLocalSound )( sfxHandle_t sfx, int channelNum );
			void			( *StartSound )( vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx );
			void			( *StopBackgroundTrack )( void );
			void 			( *StopLoopingSound )( int entityNum );
			void 			( *UpdateAmbientSet )( const char *name, vec3_t origin );
			void 			( *UpdateEntityPosition )( int entityNum, const vec3_t origin );

		} S;

	} MultiSystem_t;

	/**************************************************
	* Allocate your own jump table for two purposes,
	* no other module can alter your calls (which they
	* can for the rest), and have a nice syntax!
	**************************************************/

	extern	MultiSystem_t trap;