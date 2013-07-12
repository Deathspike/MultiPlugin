	
	/**************************************************
	* MultiPlugin++ by Deathspike
	*
	* Client-side plugin project which allows any
	* developer to use this framework as a basis for
	* their own plugin.
	**************************************************/

	#include "mppHeader.h"

	/**************************************************
	* Imported functions from the software development kit.
	**************************************************/

	int PASSFLOAT( float x )
	{
		float floatTemp;
		floatTemp = x;
		return *( int * ) &floatTemp;
	}

	void trap_Print( const char *fmt )
	{
 		mppSystemCall( CG_PRINT, fmt );
	}

	void trap_Error( const char *fmt )
	{
 		mppSystemCall( CG_ERROR, fmt );
	}

	int trap_Milliseconds( void )
	{
 		return mppSystemCall( CG_MILLISECONDS ); 
	}

	void trap_PrecisionTimer_Start( void **theNewTimer )
	{
 		mppSystemCall( CG_PRECISIONTIMER_START, theNewTimer );
	}

	int trap_PrecisionTimer_End( void *theTimer )
	{
 		return mppSystemCall( CG_PRECISIONTIMER_END, theTimer );
	}

	void trap_Cvar_Register( vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags ) 
	{
 		mppSystemCall( CG_CVAR_REGISTER, vmCvar, varName, defaultValue, flags );
	}

	void trap_Cvar_Update( vmCvar_t *vmCvar ) 
	{
 		mppSystemCall( CG_CVAR_UPDATE, vmCvar );
	}

	void trap_Cvar_Set( const char *var_name, const char *value )
	{
 		mppSystemCall( CG_CVAR_SET, var_name, value );
	}

	void trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize )
	{
 		mppSystemCall( CG_CVAR_VARIABLESTRINGBUFFER, var_name, buffer, bufsize );
	}

	int trap_Cvar_GetHiddenVarValue( const char *name )
	{
 		return mppSystemCall( CG_CVAR_GETHIDDENVALUE, name );
	}

	int trap_Argc( void ) 
	{
 		return mppSystemCall( CG_ARGC );
	}

	void trap_Argv( int n, char *buffer, int bufferLength ) 
	{
 		mppSystemCall( CG_ARGV, n, buffer, bufferLength );
	}

	void trap_Args( char *buffer, int bufferLength ) 
	{
 		mppSystemCall( CG_ARGS, buffer, bufferLength );
	}

	int  trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode ) 
	{
 		return mppSystemCall( CG_FS_FOPENFILE, qpath, f, mode );
	}

	void trap_FS_Read( void *buffer, int len, fileHandle_t f ) 
	{
 		mppSystemCall( CG_FS_READ, buffer, len, f );
	}

	void trap_FS_Write( const void *buffer, int len, fileHandle_t f ) 
	{
 		mppSystemCall( CG_FS_WRITE, buffer, len, f );
	}

	void trap_FS_FCloseFile( fileHandle_t f ) 
	{
 		mppSystemCall( CG_FS_FCLOSEFILE, f );
	}

	int trap_FS_GetFileList(  const char *path, const char *extension, char *listbuf, int bufsize ) 
	{
 		return mppSystemCall( CG_FS_GETFILELIST, path, extension, listbuf, bufsize );
	}

	void trap_SendConsoleCommand( const char *text ) 
	{
 		mppSystemCall( CG_SENDCONSOLECOMMAND, text );
	}

	void trap_AddCommand( const char *cmdName ) 
	{
 		mppSystemCall( CG_ADDCOMMAND, cmdName );
	}

	void trap_RemoveCommand( const char *cmdName ) 
	{
 		mppSystemCall( CG_REMOVECOMMAND, cmdName );
	}

	void trap_SendClientCommand( const char *s ) 
	{
 		mppSystemCall( CG_SENDCLIENTCOMMAND, s );
	}

	void trap_UpdateScreen( void ) 
	{
 		mppSystemCall( CG_UPDATESCREEN );
	}

	void trap_CM_LoadMap( const char *mapname, qboolean SubBSP ) 
	{
 		mppSystemCall( CG_CM_LOADMAP, mapname, SubBSP );
	}

	int  trap_CM_NumInlineModels( void ) 
	{
 		return mppSystemCall( CG_CM_NUMINLINEMODELS );
	}

	clipHandle_t trap_CM_InlineModel( int index ) 
	{
 		return mppSystemCall( CG_CM_INLINEMODEL, index );
	}

	clipHandle_t trap_CM_TempBoxModel( const vec3_t mins, const vec3_t maxs ) 
	{
 		return mppSystemCall( CG_CM_TEMPBOXMODEL, mins, maxs );
	}

	clipHandle_t trap_CM_TempCapsuleModel( const vec3_t mins, const vec3_t maxs ) 
	{
 		return mppSystemCall( CG_CM_TEMPCAPSULEMODEL, mins, maxs );
	}

	int trap_CM_PointContents( const vec3_t p, clipHandle_t model )
	{
 		return mppSystemCall( CG_CM_POINTCONTENTS, p, model );
	}

	int trap_CM_TransformedPointContents( const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles )
	{
 		return mppSystemCall( CG_CM_TRANSFORMEDPOINTCONTENTS, p, model, origin, angles );
	}

	void trap_CM_BoxTrace( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask ) 
	{
 		mppSystemCall( CG_CM_BOXTRACE, results, start, end, mins, maxs, model, brushmask );
	}

	void trap_CM_CapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask ) 
	{
 		mppSystemCall( CG_CM_CAPSULETRACE, results, start, end, mins, maxs, model, brushmask );
	}

	void trap_CM_TransformedBoxTrace( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask, const vec3_t origin, const vec3_t angles )
	{
		mppSystemCall( CG_CM_TRANSFORMEDBOXTRACE, results, start, end, mins, maxs, model, brushmask, origin, angles );
	}

	void trap_CM_TransformedCapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask, const vec3_t origin, const vec3_t angles )
	{
		mppSystemCall( CG_CM_TRANSFORMEDCAPSULETRACE, results, start, end, mins, maxs, model, brushmask, origin, angles );
	}

	int trap_CM_MarkFragments( int numPoints, const vec3_t *points, const vec3_t projection, int maxPoints, vec3_t pointBuffer, int maxFragments, markFragment_t *fragmentBuffer ) 
	{
 		return mppSystemCall( CG_CM_MARKFRAGMENTS, numPoints, points, projection, maxPoints, pointBuffer, maxFragments, fragmentBuffer );
	}

	int trap_S_GetVoiceVolume( int entityNum ) 
	{
 		return mppSystemCall( CG_S_GETVOICEVOLUME, entityNum );
	}

	void trap_S_MuteSound( int entityNum, int entchannel )
	{
 		mppSystemCall( CG_S_MUTESOUND, entityNum, entchannel );
	}

	void trap_S_StartSound( vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx ) 
	{
 		mppSystemCall( CG_S_STARTSOUND, origin, entityNum, entchannel, sfx );
	}

	void trap_S_StartLocalSound( sfxHandle_t sfx, int channelNum ) 
	{
 		mppSystemCall( CG_S_STARTLOCALSOUND, sfx, channelNum );
	}

	void trap_S_ClearLoopingSounds( void )
	{
 		mppSystemCall( CG_S_CLEARLOOPINGSOUNDS );
	}

	void trap_S_AddLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx ) 
	{
 		mppSystemCall( CG_S_ADDLOOPINGSOUND, entityNum, origin, velocity, sfx );
	}

	void trap_S_UpdateEntityPosition( int entityNum, const vec3_t origin ) 
	{
 		mppSystemCall( CG_S_UPDATEENTITYPOSITION, entityNum, origin );
	}

	void trap_S_AddRealLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx ) 
	{
 		mppSystemCall( CG_S_ADDREALLOOPINGSOUND, entityNum, origin, velocity, sfx );
	}

	void trap_S_StopLoopingSound( int entityNum ) 
	{
 		mppSystemCall( CG_S_STOPLOOPINGSOUND, entityNum );
	}

	void trap_S_Respatialize( int entityNum, const vec3_t origin, vec3_t axis[3], int inwater ) 
	{
 		mppSystemCall( CG_S_RESPATIALIZE, entityNum, origin, axis, inwater );
	}

	void trap_S_ShutUp( qboolean shutUpFactor )
	{
 		mppSystemCall( CG_S_SHUTUP, shutUpFactor );
	}

	sfxHandle_t trap_S_RegisterSound( const char *sample )
	{
 		return mppSystemCall( CG_S_REGISTERSOUND, sample );
	}

	void trap_S_StartBackgroundTrack( const char *intro, const char *loop, qboolean bReturnWithoutStarting )
	{
 		mppSystemCall( CG_S_STARTBACKGROUNDTRACK, intro, loop, bReturnWithoutStarting  );
	}

	void trap_S_UpdateAmbientSet( const char *name, vec3_t origin )
	{
 		mppSystemCall(CG_S_UPDATEAMBIENTSET, name, origin);
	}

	void trap_AS_ParseSets( void )
	{
 		mppSystemCall(CG_AS_PARSESETS);
	}

	void trap_AS_AddPrecacheEntry( const char *name )
	{
 		mppSystemCall( CG_AS_ADDPRECACHEENTRY, name );
	}

	int trap_S_AddLocalSet( const char *name, vec3_t listener_origin, vec3_t origin, int entID, int time )
	{
 		return mppSystemCall( CG_S_ADDLOCALSET, name, listener_origin, origin, entID, time );
	}

	sfxHandle_t trap_AS_GetBModelSound( const char *name, int stage )
	{
 		return mppSystemCall( CG_AS_GETBMODELSOUND, name, stage );
	}

	void trap_R_LoadWorldMap( const char *mapname ) 
	{
 		mppSystemCall( CG_R_LOADWORLDMAP, mapname );
	}

	qhandle_t trap_R_RegisterModel( const char *name ) 
	{
 		return mppSystemCall( CG_R_REGISTERMODEL, name );
	}

	qhandle_t trap_R_RegisterSkin( const char *name ) 
	{
 		return mppSystemCall( CG_R_REGISTERSKIN, name );
	}

	qhandle_t trap_R_RegisterShader( const char *name ) 
	{
 		return mppSystemCall( CG_R_REGISTERSHADER, name );
	}

	qhandle_t trap_R_RegisterShaderNoMip( const char *name ) 
	{
 		return mppSystemCall( CG_R_REGISTERSHADERNOMIP, name );
	}

	qhandle_t trap_R_RegisterFont( const char *fontName )
	{
 		return mppSystemCall( CG_R_REGISTERFONT, fontName);
	}

	int trap_R_Font_StrLenPixels( const char *text, const int iFontIndex, const float scale )
	{
 		return mppSystemCall( CG_R_FONT_STRLENPIXELS, text, iFontIndex, PASSFLOAT( scale ));
	}

	int trap_R_Font_StrLenChars( const char *text )
	{
 		return mppSystemCall( CG_R_FONT_STRLENCHARS, text );
	}

	int trap_R_Font_HeightPixels( const int iFontIndex, const float scale )
	{
 		return mppSystemCall( CG_R_FONT_STRHEIGHTPIXELS, iFontIndex, PASSFLOAT( scale ));
	}

	void trap_R_Font_DrawString( int ox, int oy, const char *text, const float *rgba, const int setIndex, int iCharLimit, const float scale )
	{
 		mppSystemCall( CG_R_FONT_DRAWSTRING, ox, oy, text, rgba, setIndex, iCharLimit, PASSFLOAT( scale ));
	}

	qboolean trap_Language_IsAsian( void )
	{
 		return mppSystemCall( CG_LANGUAGE_ISASIAN );
	}

	qboolean trap_Language_UsesSpaces( void )
	{
 		return mppSystemCall( CG_LANGUAGE_USESSPACES );
	}

	unsigned int trap_AnyLanguage_ReadCharFromString( const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation/* = NULL*/ )
	{
 		return mppSystemCall( CG_ANYLANGUAGE_READCHARFROMSTRING, psText, piAdvanceCount, pbIsTrailingPunctuation );
	}

	void trap_R_ClearScene( void ) 
	{
 		mppSystemCall( CG_R_CLEARSCENE );
	}

	void trap_R_ClearDecals( void )
	{
 		mppSystemCall ( CG_R_CLEARDECALS );
	}

	void trap_R_AddRefEntityToScene( const refEntity_t *re ) 
	{
 		mppSystemCall( CG_R_ADDREFENTITYTOSCENE, re );
	}

	void trap_R_AddPolyToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts ) 
	{
 		mppSystemCall( CG_R_ADDPOLYTOSCENE, hShader, numVerts, verts );
	}

	void trap_R_AddPolysToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts, int num ) 
	{
 		mppSystemCall( CG_R_ADDPOLYSTOSCENE, hShader, numVerts, verts, num );
	}

	void trap_R_AddDecalToScene ( qhandle_t shader, const vec3_t origin, const vec3_t dir, float orientation, float r, float g, float b, float a, qboolean alphaFade, float radius, qboolean temporary )
	{
 		mppSystemCall( CG_R_ADDDECALTOSCENE, shader, origin, dir, PASSFLOAT( orientation ), PASSFLOAT( r ), PASSFLOAT( g ), PASSFLOAT( b ), PASSFLOAT( a ), alphaFade, PASSFLOAT( radius ), temporary );
	}

	int trap_R_LightForPoint( vec3_t point, vec3_t ambientLight, vec3_t directedLight, vec3_t lightDir ) 
	{
 		return mppSystemCall( CG_R_LIGHTFORPOINT, point, ambientLight, directedLight, lightDir );
	}

	void trap_R_AddLightToScene( const vec3_t org, float intensity, float r, float g, float b )
	{
 		mppSystemCall( CG_R_ADDLIGHTTOSCENE, org, PASSFLOAT( intensity ), PASSFLOAT( r ), PASSFLOAT( g ), PASSFLOAT( b ));
	}

	void trap_R_AddAdditiveLightToScene( const vec3_t org, float intensity, float r, float g, float b )
	{
 		mppSystemCall( CG_R_ADDADDITIVELIGHTTOSCENE, org, PASSFLOAT( intensity ), PASSFLOAT( r ), PASSFLOAT( g ), PASSFLOAT( b ));
	}

	void trap_R_RenderScene( const refdef_t *fd )
	{
 		mppSystemCall( CG_R_RENDERSCENE, fd );
	}

	void trap_R_SetColor( const float *rgba )
	{
 		mppSystemCall( CG_R_SETCOLOR, rgba );
	}

	void trap_R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader )
	{
 		mppSystemCall( CG_R_DRAWSTRETCHPIC, PASSFLOAT( x ), PASSFLOAT( y ), PASSFLOAT( w ), PASSFLOAT( h ), PASSFLOAT( s1 ), PASSFLOAT( t1 ), PASSFLOAT( s2 ), PASSFLOAT( t2 ), hShader );
	}

	void trap_R_ModelBounds( clipHandle_t model, vec3_t mins, vec3_t maxs )
	{
 		mppSystemCall( CG_R_MODELBOUNDS, model, mins, maxs );
	}

	int trap_R_LerpTag( orientation_t *tag, clipHandle_t mod, int startFrame, int endFrame, float frac, const char *tagName )
	{
 		return mppSystemCall( CG_R_LERPTAG, tag, mod, startFrame, endFrame, PASSFLOAT( frac ), tagName );
	}

	void trap_R_DrawRotatePic( float x, float y, float w, float h, float s1, float t1, float s2, float t2,float a, qhandle_t hShader )
	{
 		mppSystemCall( CG_R_DRAWROTATEPIC, PASSFLOAT( x ), PASSFLOAT( y ), PASSFLOAT( w ), PASSFLOAT( h ), PASSFLOAT( s1 ), PASSFLOAT( t1 ), PASSFLOAT( s2 ), PASSFLOAT( t2 ), PASSFLOAT( a ), hShader );
	}

	void trap_R_DrawRotatePic2( float x, float y, float w, float h, float s1, float t1, float s2, float t2,float a, qhandle_t hShader )
	{
 		mppSystemCall( CG_R_DRAWROTATEPIC2, PASSFLOAT( x ), PASSFLOAT( y ), PASSFLOAT( w ), PASSFLOAT( h ), PASSFLOAT( s1 ), PASSFLOAT( t1 ), PASSFLOAT( s2 ), PASSFLOAT( t2 ), PASSFLOAT( a ), hShader );
	}

	void trap_R_SetRangeFog( float range )
	{
 		mppSystemCall( CG_R_SETRANGEFOG, PASSFLOAT( range ));
	}

	void trap_R_SetRefractProp( float alpha, float stretch, qboolean prepost, qboolean negate )
	{
 		mppSystemCall( CG_R_SETREFRACTIONPROP, PASSFLOAT( alpha ), PASSFLOAT( stretch ), prepost, negate );
	}

	void trap_R_RemapShader( const char *oldShader, const char *newShader, const char *timeOffset ) 
	{
 		mppSystemCall( CG_R_REMAP_SHADER, oldShader, newShader, timeOffset );
	}

	void trap_R_GetLightStyle( int style, color4ub_t color )
	{
 		mppSystemCall( CG_R_GET_LIGHT_STYLE, style, color );
	}

	void trap_R_SetLightStyle( int style, int color )
	{
 		mppSystemCall( CG_R_SET_LIGHT_STYLE, style, color );
	}

	void trap_R_GetBModelVerts( int bmodelIndex, vec3_t *verts, vec3_t normal )
	{
 		mppSystemCall( CG_R_GET_BMODEL_VERTS, bmodelIndex, verts, normal );
	}

	void trap_R_GetDistanceCull( float *f )
	{
 		mppSystemCall( CG_R_GETDISTANCECULL, f );
	}

	void trap_R_GetRealRes( int *w, int *h )
	{
 		mppSystemCall( CG_R_GETREALRES, w, h );
	}

	void trap_R_AutomapElevAdj( float newHeight )
	{
 		mppSystemCall( CG_R_AUTOMAPELEVADJ, PASSFLOAT( newHeight ));
	}

	qboolean trap_R_InitWireframeAutomap( void )
	{
 		return mppSystemCall( CG_R_INITWIREFRAMEAUTO );
	}

	void trap_FX_AddLine( const vec3_t start, const vec3_t end, float size1, float size2, float sizeParm, float alpha1, float alpha2, float alphaParm, const vec3_t sRGB, const vec3_t eRGB, float rgbParm, int killTime, qhandle_t shader, int flags )
	{
		mppSystemCall( CG_FX_ADDLINE, start, end, PASSFLOAT( size1 ), PASSFLOAT( size2 ), PASSFLOAT( sizeParm ), PASSFLOAT( alpha1 ), PASSFLOAT( alpha2 ), PASSFLOAT( alphaParm ), sRGB, eRGB, PASSFLOAT( rgbParm ), killTime, shader, flags );
	}

	void trap_GetGlconfig( glconfig_t *glconfig )
	{
 		mppSystemCall( CG_GETGLCONFIG, glconfig );
	}

	void trap_GetGameState( gameState_t *gamestate )
	{
 		mppSystemCall( CG_GETGAMESTATE, gamestate );
	}

	void trap_GetCurrentSnapshotNumber( int *snapshotNumber, int *serverTime )
	{
 		mppSystemCall( CG_GETCURRENTSNAPSHOTNUMBER, snapshotNumber, serverTime );
	}

	qboolean trap_GetSnapshot( int snapshotNumber, snapshot_t *snapshot )
	{
 		return mppSystemCall( CG_GETSNAPSHOT, snapshotNumber, snapshot );
	}

	qboolean trap_GetDefaultState( int entityIndex, entityState_t *state )
	{
 		return mppSystemCall( CG_GETDEFAULTSTATE, entityIndex, state );
	}

	qboolean trap_GetServerCommand( int serverCommandNumber )
	{
 		return mppSystemCall( CG_GETSERVERCOMMAND, serverCommandNumber );
	}

	int trap_GetCurrentCmdNumber( void )
	{
 		return mppSystemCall( CG_GETCURRENTCMDNUMBER );
	}

	qboolean trap_GetUserCmd( int cmdNumber, usercmd_t *ucmd )
	{
 		return mppSystemCall( CG_GETUSERCMD, cmdNumber, ucmd );
	}

	void trap_SetUserCmdValue( int stateValue, float sensitivityScale, float mPitchOverride, float mYawOverride, float mSensitivityOverride, int fpSel, int invenSel, qboolean fighterControls )
	{
 		mppSystemCall( CG_SETUSERCMDVALUE, stateValue, PASSFLOAT( sensitivityScale ), PASSFLOAT( mPitchOverride ), PASSFLOAT( mYawOverride ), PASSFLOAT( mSensitivityOverride ), fpSel, invenSel, fighterControls );
	}

	void trap_SetClientForceAngle( int time, vec3_t angle )
	{
 		mppSystemCall( CG_SETCLIENTFORCEANGLE, time, angle );
	}

	void trap_SetClientTurnExtent( float turnAdd, float turnSub, int turnTime )
	{
 		mppSystemCall( CG_SETCLIENTTURNEXTENT, PASSFLOAT( turnAdd ), PASSFLOAT( turnSub ), turnTime );
	}

	void trap_OpenUIMenu( int menuID )
	{
 		mppSystemCall( CG_OPENUIMENU, menuID );
	}

	void testPrintInt( char *string, int i )
	{
 		mppSystemCall( CG_TESTPRINTINT, string, i );
	}

	void testPrintFloat( char *string, float f )
	{
 		mppSystemCall( CG_TESTPRINTFLOAT, string, PASSFLOAT( f ));
	}

	int trap_MemoryRemaining( void )
	{
 		return mppSystemCall( CG_MEMORY_REMAINING );
	}

	qboolean trap_Key_IsDown( int keynum )
	{
 		return mppSystemCall( CG_KEY_ISDOWN, keynum );
	}

	int trap_Key_GetCatcher( void )
	{
 		return mppSystemCall( CG_KEY_GETCATCHER );
	}

	void trap_Key_SetCatcher( int catcher )
	{
 		mppSystemCall( CG_KEY_SETCATCHER, catcher );
	}

	int trap_Key_GetKey( const char *binding )
	{
 		return mppSystemCall( CG_KEY_GETKEY, binding );
	}

	int trap_PC_AddGlobalDefine( char *define )
	{
 		return mppSystemCall( CG_PC_ADD_GLOBAL_DEFINE, define );
	}

	int trap_PC_LoadSource( const char *filename )
	{
 		return mppSystemCall( CG_PC_LOAD_SOURCE, filename );
	}

	int trap_PC_FreeSource( int handle )
	{
 		return mppSystemCall( CG_PC_FREE_SOURCE, handle );
	}

	int trap_PC_ReadToken( int handle, pc_token_t *pc_token )
	{
 		return mppSystemCall( CG_PC_READ_TOKEN, handle, pc_token );
	}

	int trap_PC_SourceFileAndLine( int handle, char *filename, int *line )
	{
 		return mppSystemCall( CG_PC_SOURCE_FILE_AND_LINE, handle, filename, line );
	}

	int trap_PC_LoadGlobalDefines( const char* filename )
	{
 		return mppSystemCall ( CG_PC_LOAD_GLOBAL_DEFINES, filename );
	}

	void trap_PC_RemoveAllGlobalDefines( void )
	{
 		mppSystemCall ( CG_PC_REMOVE_ALL_GLOBAL_DEFINES );
	}

	void trap_S_StopBackgroundTrack( void ) 
	{
 		mppSystemCall( CG_S_STOPBACKGROUNDTRACK );
	}

	int trap_RealTime( qtime_t *qtime )
	{
 		return mppSystemCall( CG_REAL_TIME, qtime );
	}

	void trap_SnapVector( float *v ) 
	{
 		mppSystemCall( CG_SNAPVECTOR, v );
	}

	int trap_CIN_PlayCinematic( const char *arg0, int xpos, int ypos, int width, int height, int bits )
	{
  		return mppSystemCall( CG_CIN_PLAYCINEMATIC, arg0, xpos, ypos, width, height, bits );
	}
	 
	e_status trap_CIN_StopCinematic( int handle )
	{
  		return mppSystemCall( CG_CIN_STOPCINEMATIC, handle );
	}

	e_status trap_CIN_RunCinematic( int handle )
	{
  		return mppSystemCall( CG_CIN_RUNCINEMATIC, handle );
	}

	void trap_CIN_DrawCinematic( int handle )
	{
		mppSystemCall( CG_CIN_DRAWCINEMATIC, handle );
	}

	void trap_CIN_SetExtents( int handle, int x, int y, int w, int h )
	{
  		mppSystemCall( CG_CIN_SETEXTENTS, handle, x, y, w, h );
	}

	qboolean trap_GetEntityToken( char *buffer, int bufferSize )
	{
 		return mppSystemCall( CG_GET_ENTITY_TOKEN, buffer, bufferSize );
	}

	qboolean trap_R_inPVS( const vec3_t p1, const vec3_t p2, byte *mask )
	{
 		return mppSystemCall( CG_R_INPVS, p1, p2, mask );
	}

	int trap_FX_RegisterEffect( const char *file )
	{
 		return mppSystemCall( CG_FX_REGISTER_EFFECT, file);
	}

	void trap_FX_PlayEffect( const char *file, vec3_t org, vec3_t fwd, int vol, int rad )
	{
 		mppSystemCall( CG_FX_PLAY_EFFECT, file, org, fwd, vol, rad );
	}

	void trap_FX_PlayEntityEffect( const char *file, vec3_t org, vec3_t axis[3], const int boltInfo, const int entNum, int vol, int rad )
	{
 		mppSystemCall( CG_FX_PLAY_ENTITY_EFFECT, file, org, axis, boltInfo, entNum, vol, rad );
	}

	void trap_FX_PlayEffectID( int id, vec3_t org, vec3_t fwd, int vol, int rad )
	{
 		mppSystemCall( CG_FX_PLAY_EFFECT_ID, id, org, fwd, vol, rad );
	}

	void trap_FX_PlayPortalEffectID( int id, vec3_t org, vec3_t fwd, int vol, int rad )
	{
 		mppSystemCall( CG_FX_PLAY_PORTAL_EFFECT_ID, id, org, fwd );
	}

	void trap_FX_PlayEntityEffectID( int id, vec3_t org, vec3_t axis[3], const int boltInfo, const int entNum, int vol, int rad )
	{
		mppSystemCall( CG_FX_PLAY_ENTITY_EFFECT_ID, id, org, axis, boltInfo, entNum, vol, rad );
	}

	void trap_FX_PlayBoltedEffectID( int id, vec3_t org, void *ghoul2, const int boltNum, const int entNum, const int modelNum, int iLooptime, qboolean isRelative )
	{
		mppSystemCall( CG_FX_PLAY_BOLTED_EFFECT_ID, id, org, ghoul2, boltNum, entNum, modelNum, iLooptime, isRelative );
	}

	void trap_FX_AddScheduledEffects( qboolean skyPortal )
	{
 		mppSystemCall( CG_FX_ADD_SCHEDULED_EFFECTS, skyPortal );
	}

	void trap_FX_Draw2DEffects ( float screenXScale, float screenYScale )
	{
 		mppSystemCall( CG_FX_DRAW_2D_EFFECTS, PASSFLOAT( screenXScale ), PASSFLOAT( screenYScale ));
	} 

	int trap_FX_InitSystem( refdef_t* refdef )
	{
 		return mppSystemCall( CG_FX_INIT_SYSTEM, refdef );
	}

	void trap_FX_SetRefDef( refdef_t* refdef )
	{
 		mppSystemCall( CG_FX_SET_REFDEF, refdef );
	}

	qboolean trap_FX_FreeSystem( void )
	{
 		return mppSystemCall( CG_FX_FREE_SYSTEM );
	}

	void trap_FX_Reset( void )
	{
 		mppSystemCall ( CG_FX_RESET );
	}

	void trap_FX_AdjustTime( int time )
	{
 		mppSystemCall( CG_FX_ADJUST_TIME, time );
	}

	void trap_FX_AddPoly( addpolyArgStruct_t *p )
	{
 		mppSystemCall( CG_FX_ADDPOLY, p );
	}

	void trap_FX_AddBezier( addbezierArgStruct_t *p )
	{
 		mppSystemCall( CG_FX_ADDBEZIER, p );
	}

	void trap_FX_AddPrimitive( effectTrailArgStruct_t *p )
	{
 		mppSystemCall( CG_FX_ADDPRIMITIVE, p );
	}

	void trap_FX_AddSprite( addspriteArgStruct_t *p )
	{
 		mppSystemCall( CG_FX_ADDSPRITE, p );
	}

	void trap_FX_AddElectricity( addElectricityArgStruct_t *p )
	{
 		mppSystemCall( CG_FX_ADDELECTRICITY, p );
	}

	int trap_SP_GetStringTextString( const char *text, char *buffer, int bufferLength )
	{
 		return mppSystemCall( CG_SP_GETSTRINGTEXTSTRING, text, buffer, bufferLength );
	}

	qboolean trap_ROFF_Clean( void ) 
	{
 		return mppSystemCall( CG_ROFF_CLEAN );
	}

	void trap_ROFF_UpdateEntities( void ) 
	{
 		mppSystemCall( CG_ROFF_UPDATE_ENTITIES );
	}

	int trap_ROFF_Cache( char *file )
	{
 		return mppSystemCall( CG_ROFF_CACHE, file );
	}

	qboolean trap_ROFF_Play( int entID, int roffID, qboolean doTranslation )
	{
 		return mppSystemCall( CG_ROFF_PLAY, entID, roffID, doTranslation );
	}

	qboolean trap_ROFF_Purge_Ent( int entID )
	{
 		return mppSystemCall( CG_ROFF_PURGE_ENT, entID );
	}

	void trap_TrueMalloc( void **ptr, int size )
	{
 		mppSystemCall( CG_TRUEMALLOC, ptr, size );
	}

	void trap_TrueFree( void **ptr )
	{
 		mppSystemCall( CG_TRUEFREE, ptr );
	}

	void trap_G2_ListModelSurfaces( void *ghlInfo )
	{
 		mppSystemCall( CG_G2_LISTSURFACES, ghlInfo );
	}

	void trap_G2_ListModelBones( void *ghlInfo, int frame )
	{
 		mppSystemCall( CG_G2_LISTBONES, ghlInfo, frame );
	}

	void trap_G2_SetGhoul2ModelIndexes( void *ghoul2, qhandle_t *modelList, qhandle_t *skinList )
	{
 		mppSystemCall( CG_G2_SETMODELS, ghoul2, modelList, skinList );
	}

	qboolean trap_G2_HaveWeGhoul2Models( void *ghoul2 )
	{
		return ( qboolean ) mppSystemCall( CG_G2_HAVEWEGHOULMODELS, ghoul2 );
	}

	qboolean trap_G2API_GetBoltMatrix( void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale )
	{
		return ( qboolean ) mppSystemCall( CG_G2_GETBOLT, ghoul2, modelIndex, boltIndex, matrix, angles, position, frameNum, modelList, scale );
	}

	qboolean trap_G2API_GetBoltMatrix_NoReconstruct( void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale )
	{
		return ( qboolean ) mppSystemCall( CG_G2_GETBOLT_NOREC, ghoul2, modelIndex, boltIndex, matrix, angles, position, frameNum, modelList, scale );
	}

	qboolean trap_G2API_GetBoltMatrix_NoRecNoRot( void *ghoul2, const int modelIndex, const int boltIndex, mdxaBone_t *matrix, const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t *modelList, vec3_t scale )
	{
		return ( qboolean ) mppSystemCall( CG_G2_GETBOLT_NOREC_NOROT, ghoul2, modelIndex, boltIndex, matrix, angles, position, frameNum, modelList, scale );
	}

	int trap_G2API_InitGhoul2Model( void **ghoul2Ptr, const char *fileName, int modelIndex, qhandle_t customSkin, qhandle_t customShader, int modelFlags, int lodBias )
	{
 		return mppSystemCall( CG_G2_INITGHOUL2MODEL, ghoul2Ptr, fileName, modelIndex, customSkin, customShader, modelFlags, lodBias );
	}

	qboolean trap_G2API_SetSkin( void *ghoul2, int modelIndex, qhandle_t customSkin, qhandle_t renderSkin )
	{
 		return mppSystemCall( CG_G2_SETSKIN, ghoul2, modelIndex, customSkin, renderSkin );
	}

	void trap_G2API_CollisionDetect( CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, const vec3_t rayStart, const vec3_t rayEnd, const vec3_t scale, int traceFlags, int useLod, float fRadius )
	{
		mppSystemCall ( CG_G2_COLLISIONDETECT, collRecMap, ghoul2, angles, position, frameNumber, entNum, rayStart, rayEnd, scale, traceFlags, useLod, PASSFLOAT( fRadius ));
	}

	void trap_G2API_CollisionDetectCache( CollisionRecord_t *collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position, int frameNumber, int entNum, const vec3_t rayStart, const vec3_t rayEnd, const vec3_t scale, int traceFlags, int useLod, float fRadius )
	{
		mppSystemCall ( CG_G2_COLLISIONDETECTCACHE, collRecMap, ghoul2, angles, position, frameNumber, entNum, rayStart, rayEnd, scale, traceFlags, useLod, PASSFLOAT( fRadius ));
	}

	void trap_G2API_CleanGhoul2Models( void **ghoul2Ptr )
	{
		mppSystemCall( CG_G2_CLEANMODELS, ghoul2Ptr );
	}

	qboolean trap_G2API_SetBoneAngles( void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags, const int up, const int right, const int forward, qhandle_t *modelList, int blendTime , int currentTime )
	{
		return mppSystemCall( CG_G2_ANGLEOVERRIDE, ghoul2, modelIndex, boneName, angles, flags, up, right, forward, modelList, blendTime, currentTime );
	}

	qboolean trap_G2API_SetBoneAnim( void *ghoul2, const int modelIndex, const char *boneName, const int startFrame, const int endFrame, const int flags, const float animSpeed, const int currentTime, const float setFrame , const int blendTime )
	{
		return mppSystemCall( CG_G2_PLAYANIM, ghoul2, modelIndex, boneName, startFrame, endFrame, flags, PASSFLOAT(animSpeed), currentTime, PASSFLOAT( setFrame ), blendTime );
	}

	qboolean trap_G2API_GetBoneAnim( void *ghoul2, const char *boneName, const int currentTime, float *currentFrame, int *startFrame, int *endFrame, int *flags, float *animSpeed, int *modelList, const int modelIndex )
	{
 		return mppSystemCall(CG_G2_GETBONEANIM, ghoul2, boneName, currentTime, currentFrame, startFrame, endFrame, flags, animSpeed, modelList, modelIndex);
	}

	qboolean trap_G2API_GetBoneFrame( void *ghoul2, const char *boneName, const int currentTime, float *currentFrame, int *modelList, const int modelIndex )
	{
 		return mppSystemCall(CG_G2_GETBONEFRAME, ghoul2, boneName, currentTime, currentFrame, modelList, modelIndex);
	}

	void trap_G2API_GetGLAName( void *ghoul2, int modelIndex, char *fillBuf )
	{
 		mppSystemCall( CG_G2_GETGLANAME, ghoul2, modelIndex, fillBuf );
	}

	int trap_G2API_CopyGhoul2Instance( void *g2From, void *g2To, int modelIndex )
	{
 		return mppSystemCall( CG_G2_COPYGHOUL2INSTANCE, g2From, g2To, modelIndex );
	}

	void trap_G2API_CopySpecificGhoul2Model( void *g2From, int modelFrom, void *g2To, int modelTo )
	{
 		mppSystemCall( CG_G2_COPYSPECIFICGHOUL2MODEL, g2From, modelFrom, g2To, modelTo );
	}

	void trap_G2API_DuplicateGhoul2Instance( void *g2From, void **g2To )
	{
 		mppSystemCall( CG_G2_DUPLICATEGHOUL2INSTANCE, g2From, g2To );
	}

	qboolean trap_G2API_HasGhoul2ModelOnIndex( void *ghlInfo, int modelIndex )
	{
 		return mppSystemCall( CG_G2_HASGHOUL2MODELONINDEX, ghlInfo, modelIndex );
	}

	qboolean trap_G2API_RemoveGhoul2Model( void *ghlInfo, int modelIndex )
	{
 		return mppSystemCall( CG_G2_REMOVEGHOUL2MODEL, ghlInfo, modelIndex );
	}

	qboolean trap_G2API_SkinlessModel( void *ghlInfo, int modelIndex )
	{
 		return mppSystemCall( CG_G2_SKINLESSMODEL, ghlInfo, modelIndex );
	}

	int trap_G2API_GetNumGoreMarks( void *ghlInfo, int modelIndex )
	{
 		return mppSystemCall( CG_G2_GETNUMGOREMARKS, ghlInfo, modelIndex );
	}

	void trap_G2API_AddSkinGore( void *ghlInfo,SSkinGoreData *gore )
	{
 		mppSystemCall( CG_G2_ADDSKINGORE, ghlInfo, gore );
	}

	void trap_G2API_ClearSkinGore ( void* ghlInfo )
	{
 		mppSystemCall( CG_G2_CLEARSKINGORE, ghlInfo );
	}

	int trap_G2API_Ghoul2Size ( void* ghlInfo )
	{
 		return mppSystemCall( CG_G2_SIZE, ghlInfo );
	}

	int trap_G2API_AddBolt( void *ghoul2, int modelIndex, const char *boneName )
	{
 		return mppSystemCall( CG_G2_ADDBOLT, ghoul2, modelIndex, boneName );
	}

	qboolean trap_G2API_AttachEnt( int *boltInfo, void *ghlInfoTo, int toBoltIndex, int entNum, int toModelNum )
	{
 		return mppSystemCall( CG_G2_ATTACHENT, boltInfo, ghlInfoTo, toBoltIndex, entNum, toModelNum );
	}

	void trap_G2API_SetBoltInfo( void *ghoul2, int modelIndex, int boltInfo )
	{
 		mppSystemCall( CG_G2_SETBOLTON, ghoul2, modelIndex, boltInfo );
	}

	qboolean trap_G2API_SetRootSurface( void *ghoul2, const int modelIndex, const char *surfaceName )
	{
 		return mppSystemCall( CG_G2_SETROOTSURFACE, ghoul2, modelIndex, surfaceName );
	}

	qboolean trap_G2API_SetSurfaceOnOff( void *ghoul2, const char *surfaceName, const int flags )
	{
 		return mppSystemCall( CG_G2_SETSURFACEONOFF, ghoul2, surfaceName, flags );
	}

	qboolean trap_G2API_SetNewOrigin( void *ghoul2, const int boltIndex )
	{
 		return mppSystemCall( CG_G2_SETNEWORIGIN, ghoul2, boltIndex );
	}

	qboolean trap_G2API_DoesBoneExist( void *ghoul2, int modelIndex, const char *boneName )
	{
 		return mppSystemCall( CG_G2_DOESBONEEXIST, ghoul2, modelIndex, boneName );
	}

	int trap_G2API_GetSurfaceRenderStatus( void *ghoul2, const int modelIndex, const char *surfaceName )
	{
 		return mppSystemCall( CG_G2_GETSURFACERENDERSTATUS, ghoul2, modelIndex, surfaceName );
	}

	int trap_G2API_GetTime( void )
	{
 		return mppSystemCall( CG_G2_GETTIME );
	}

	void trap_G2API_SetTime( int time, int clock )
	{
 		mppSystemCall( CG_G2_SETTIME, time, clock );
	}

	void trap_G2API_AbsurdSmoothing( void *ghoul2, qboolean status )
	{
 		mppSystemCall( CG_G2_ABSURDSMOOTHING, ghoul2, status );
	}

	void trap_G2API_SetRagDoll( void *ghoul2, sharedRagDollParams_t *params )
	{
 		mppSystemCall( CG_G2_SETRAGDOLL, ghoul2, params );
	}

	void trap_G2API_AnimateG2Models( void *ghoul2, int time, sharedRagDollUpdateParams_t *params )
	{
 		mppSystemCall( CG_G2_ANIMATEG2MODELS, ghoul2, time, params );
	}

	qboolean trap_G2API_RagPCJConstraint( void *ghoul2, const char *boneName, vec3_t min, vec3_t max )
	{
 		return mppSystemCall( CG_G2_RAGPCJCONSTRAINT, ghoul2, boneName, min, max );
	}

	qboolean trap_G2API_RagPCJGradientSpeed( void *ghoul2, const char *boneName, const float speed )
	{
 		return mppSystemCall( CG_G2_RAGPCJGRADIENTSPEED, ghoul2, boneName, PASSFLOAT( speed ));
	}

	qboolean trap_G2API_RagEffectorGoal( void *ghoul2, const char *boneName, vec3_t pos )
	{
 		return mppSystemCall( CG_G2_RAGEFFECTORGOAL, ghoul2, boneName, pos );
	}

	qboolean trap_G2API_GetRagBonePos( void *ghoul2, const char *boneName, vec3_t pos, vec3_t entAngles, vec3_t entPos, vec3_t entScale )
	{
 		return mppSystemCall( CG_G2_GETRAGBONEPOS, ghoul2, boneName, pos, entAngles, entPos, entScale );
	}

	qboolean trap_G2API_RagEffectorKick( void *ghoul2, const char *boneName, vec3_t velocity )
	{
 		return mppSystemCall( CG_G2_RAGEFFECTORKICK, ghoul2, boneName, velocity );
	}

	qboolean trap_G2API_RagForceSolve( void *ghoul2, qboolean force )
	{
 		return mppSystemCall( CG_G2_RAGFORCESOLVE, ghoul2, force );
	}

	qboolean trap_G2API_SetBoneIKState( void *ghoul2, int time, const char *boneName, int ikState, sharedSetBoneIKStateParams_t *params )
	{
 		return mppSystemCall( CG_G2_SETBONEIKSTATE, ghoul2, time, boneName, ikState, params );
	}

	qboolean trap_G2API_IKMove( void *ghoul2, int time, sharedIKMoveParams_t *params )
	{
 		return mppSystemCall( CG_G2_IKMOVE, ghoul2, time, params );
	}

	qboolean trap_G2API_RemoveBone( void *ghoul2, const char *boneName, int modelIndex )
	{
 		return mppSystemCall( CG_G2_REMOVEBONE, ghoul2, boneName, modelIndex );
	}

	void trap_G2API_AttachInstanceToEntNum( void *ghoul2, int entityNum, qboolean server )
	{
 		mppSystemCall( CG_G2_ATTACHINSTANCETOENTNUM, ghoul2, entityNum, server );
	}

	void trap_G2API_ClearAttachedInstance( int entityNum )
	{
 		mppSystemCall( CG_G2_CLEARATTACHEDINSTANCE, entityNum );
	}

	void trap_G2API_CleanEntAttachments( void )
	{
 		mppSystemCall( CG_G2_CLEANENTATTACHMENTS );
	}

	qboolean trap_G2API_OverrideServer( void *serverInstance )
	{
 		return mppSystemCall( CG_G2_OVERRIDESERVER, serverInstance );
	}

	void trap_G2API_GetSurfaceName( void *ghoul2, int surfNumber, int modelIndex, char *fillBuf )
	{
 		mppSystemCall( CG_G2_GETSURFACENAME, ghoul2, surfNumber, modelIndex, fillBuf );
	}

	void trap_CG_RegisterSharedMemory( char *memory )
	{
 		mppSystemCall( CG_SET_SHARED_BUFFER, memory );
	}

	int trap_CM_RegisterTerrain( const char *config )
	{
 		return mppSystemCall( CG_CM_REGISTER_TERRAIN, config );
	}

	void trap_RMG_Init( int terrainID, const char *terrainInfo )
	{
 		mppSystemCall( CG_RMG_INIT, terrainID, terrainInfo );
	}

	void trap_RE_InitRendererTerrain( const char *info )
	{
 		mppSystemCall( CG_RE_INIT_RENDERER_TERRAIN, info );
	}

	void trap_R_WeatherContentsOverride( int contents )
	{
 		mppSystemCall( CG_R_WEATHER_CONTENTS_OVERRIDE, contents );
	}

	void trap_R_WorldEffectCommand( const char *cmd )
	{
 		mppSystemCall( CG_R_WORLDEFFECTCOMMAND, cmd );
	}

	void trap_WE_AddWeatherZone( const vec3_t mins, const vec3_t maxs )
	{
 		mppSystemCall( CG_WE_ADDWEATHERZONE, mins, maxs );
	}

	/**************************************************
	* mppSystemCall
	*
	* Converts an incoming call with any number of arguments
	* to fill up the internal static stack of addresses.
	* When done, copies the iCmd and returns the executed
	* function result.
	**************************************************/

	int mppSystemCall( int iCmd, ... )
	{
		static int	aArguments[12];
		va_list		lArguments;
		int			i, j;

		va_start ( lArguments, iCmd );
		aArguments[0] = iCmd;

		for ( i = 10, j = 1; i > 0; i--, j++ )
		{
			aArguments[j] = va_arg( lArguments, int );
		}

		va_end ( lArguments );
		return MultiPlugin.pCGSystemCall(( int * ) &aArguments );
	}

	/**************************************************
	* mppSystemInit
	*
	* Load all the call-able functions into the
	* MultiSystem_t structure, send as a pointer. This
	* way we can separate any plugin calls.
	**************************************************/

	void mppSystemInit( MultiSystem_t *trap )
	{
		trap->AddCommand = &trap_AddCommand;
		trap->AddWeatherZone = &trap_WE_AddWeatherZone;
		trap->Argc = &trap_Argc;
		trap->Args = &trap_Args;
		trap->Argv = &trap_Argv;
		trap->Error = &trap_Error;
		trap->GetCurrentCmdNumber = &trap_GetCurrentCmdNumber;
		trap->GetCurrentSnapshotNumber = &trap_GetCurrentSnapshotNumber;
		trap->GetDefaultState = &trap_GetDefaultState;
		trap->GetEntityToken = &trap_GetEntityToken;
		trap->GetGameState = &trap_GetGameState;
		trap->GetGlconfig = &trap_GetGlconfig;
		trap->GetServerCommand = &trap_GetServerCommand;
		trap->GetSnapshot = &trap_GetSnapshot;
		trap->GetStringTextString = &trap_SP_GetStringTextString;
		trap->GetUserCmd = &trap_GetUserCmd;
		trap->Init = &trap_RMG_Init;
		trap->InitRendererTerrain = &trap_RE_InitRendererTerrain;
		trap->MemoryRemaining = &trap_MemoryRemaining;
		trap->Milliseconds = &trap_Milliseconds;
		trap->OpenUIMenu = &trap_OpenUIMenu;
		trap->Print = &trap_Print;
		trap->RealTime = &trap_RealTime;
		trap->RegisterSharedMemory = &trap_CG_RegisterSharedMemory;
		trap->RemoveCommand = &trap_RemoveCommand;
		trap->SendConsoleCommand = &trap_SendConsoleCommand;
		trap->SendClientCommand = &trap_SendClientCommand;
		trap->SetClientForceAngle = &trap_SetClientForceAngle;
		trap->SetClientTurnExtent = &trap_SetClientTurnExtent;
		trap->SetUserCmdValue = &trap_SetUserCmdValue;
		trap->SnapVector = &trap_SnapVector;
		trap->TrueMalloc = &trap_TrueMalloc;
		trap->TrueFree = &trap_TrueFree;
		trap->UpdateScreen = &trap_UpdateScreen;

		trap->AS.AddPrecacheEntry = &trap_AS_AddPrecacheEntry;
		trap->AS.GetBModelSound = &trap_AS_GetBModelSound;
		trap->AS.ParseSets = &trap_AS_ParseSets;

		trap->CIN.DrawCinematic = &trap_CIN_DrawCinematic;
		trap->CIN.PlayCinematic = &trap_CIN_PlayCinematic;
		trap->CIN.RunCinematic = &trap_CIN_RunCinematic;
		trap->CIN.SetExtents = &trap_CIN_SetExtents;
		trap->CIN.StopCinematic = &trap_CIN_StopCinematic;
		
		trap->CM.BoxTrace = &trap_CM_BoxTrace;
		trap->CM.CapsuleTrace = &trap_CM_CapsuleTrace;
		trap->CM.InlineModel = &trap_CM_InlineModel;
		trap->CM.LoadMap = &trap_CM_LoadMap;
		trap->CM.NumInlineModels = &trap_CM_NumInlineModels;
		trap->CM.MarkFragments = &trap_CM_MarkFragments;
		trap->CM.PointContents = &trap_CM_PointContents;
		trap->CM.RegisterTerrain = &trap_CM_RegisterTerrain;
		trap->CM.TempBoxModel = &trap_CM_TempBoxModel;
		trap->CM.TempCapsuleModel = &trap_CM_TempCapsuleModel;
		trap->CM.TransformedBoxTrace = &trap_CM_TransformedBoxTrace;
		trap->CM.TransformedCapsuleTrace = &trap_CM_TransformedCapsuleTrace;
		trap->CM.TransformedPointContents = &trap_CM_TransformedPointContents;

		trap->Cvar.GetHiddenVarValue = &trap_Cvar_GetHiddenVarValue;
		trap->Cvar.Register = &trap_Cvar_Register;
		trap->Cvar.Set = &trap_Cvar_Set;
		trap->Cvar.Update = &trap_Cvar_Update;
		trap->Cvar.VariableStringBuffer = &trap_Cvar_VariableStringBuffer;

		trap->FS.FCloseFile = &trap_FS_FCloseFile;
		trap->FS.FOpenFile = &trap_FS_FOpenFile;
		trap->FS.GetFileList = &trap_FS_GetFileList;
		trap->FS.Read = &trap_FS_Read;
		trap->FS.Write = &trap_FS_Write;

		trap->FX.AddBezier = &trap_FX_AddBezier;
		trap->FX.AddElectricity = &trap_FX_AddElectricity;
		trap->FX.AddLine = &trap_FX_AddLine;
		trap->FX.AddPoly = &trap_FX_AddPoly;
		trap->FX.AddPrimitive = &trap_FX_AddPrimitive;
		trap->FX.AddScheduledEffects = &trap_FX_AddScheduledEffects;
		trap->FX.AddSprite = &trap_FX_AddSprite;
		trap->FX.AdjustTime = &trap_FX_AdjustTime;
		trap->FX.Draw2DEffects = &trap_FX_Draw2DEffects;
		trap->FX.FreeSystem = &trap_FX_FreeSystem;
		trap->FX.InitSystem = &trap_FX_InitSystem;
		trap->FX.RegisterEffect = &trap_FX_RegisterEffect;
		trap->FX.PlayBoltedEffectID = &trap_FX_PlayBoltedEffectID;
		trap->FX.PlayEffect = &trap_FX_PlayEffect;
		trap->FX.PlayEntityEffectID = &trap_FX_PlayEntityEffectID;
		trap->FX.PlayEffectID = &trap_FX_PlayEffectID;
		trap->FX.PlayEntityEffect = &trap_FX_PlayEntityEffect;
		trap->FX.PlayPortalEffectID = &trap_FX_PlayPortalEffectID;
		trap->FX.Reset = &trap_FX_Reset;
		trap->FX.SetRefDef = &trap_FX_SetRefDef;

		trap->G2.HaveWeGhoul2Models = &trap_G2_HaveWeGhoul2Models;
		trap->G2.ListModelSurfaces = &trap_G2_ListModelSurfaces;
		trap->G2.ListModelBones = &trap_G2_ListModelBones;
		trap->G2.SetGhoul2ModelIndexes = &trap_G2_SetGhoul2ModelIndexes;

		trap->G2API.AbsurdSmoothing = &trap_G2API_AbsurdSmoothing;
		trap->G2API.AddBolt = &trap_G2API_AddBolt;
		trap->G2API.AddSkinGore = &trap_G2API_AddSkinGore;
		trap->G2API.AnimateG2Models = &trap_G2API_AnimateG2Models;
		trap->G2API.AttachEnt = &trap_G2API_AttachEnt;
		trap->G2API.AttachInstanceToEntNum = &trap_G2API_AttachInstanceToEntNum;
		trap->G2API.ClearAttachedInstance = &trap_G2API_ClearAttachedInstance;
		trap->G2API.CleanEntAttachments = &trap_G2API_CleanEntAttachments;
		trap->G2API.CleanGhoul2Models = &trap_G2API_CleanGhoul2Models;
		trap->G2API.ClearSkinGore = &trap_G2API_ClearSkinGore;
		trap->G2API.CollisionDetect = &trap_G2API_CollisionDetect;
		trap->G2API.CollisionDetectCache = &trap_G2API_CollisionDetectCache;
		trap->G2API.CopyGhoul2Instance = &trap_G2API_CopyGhoul2Instance;
		trap->G2API.CopySpecificGhoul2Model = &trap_G2API_CopySpecificGhoul2Model;
		trap->G2API.DoesBoneExist = &trap_G2API_DoesBoneExist;
		trap->G2API.DuplicateGhoul2Instance = &trap_G2API_DuplicateGhoul2Instance;
		trap->G2API.GetBoltMatrix = &trap_G2API_GetBoltMatrix;
		trap->G2API.GetBoltMatrix_NoReconstruct = &trap_G2API_GetBoltMatrix_NoReconstruct;
		trap->G2API.GetBoltMatrix_NoRecNoRot = &trap_G2API_GetBoltMatrix_NoRecNoRot;
		trap->G2API.GetBoneAnim = &trap_G2API_GetBoneAnim;
		trap->G2API.GetBoneFrame = &trap_G2API_GetBoneFrame;
		trap->G2API.GetGLAName = &trap_G2API_GetGLAName;
		trap->G2API.GetRagBonePos = &trap_G2API_GetRagBonePos;
		trap->G2API.GetNumGoreMarks = &trap_G2API_GetNumGoreMarks;
		trap->G2API.GetTime = &trap_G2API_GetTime;
		trap->G2API.GetSurfaceName = &trap_G2API_GetSurfaceName;
		trap->G2API.GetSurfaceRenderStatus = &trap_G2API_GetSurfaceRenderStatus;
		trap->G2API.Ghoul2Size = &trap_G2API_Ghoul2Size;
		trap->G2API.HasGhoul2ModelOnIndex = &trap_G2API_HasGhoul2ModelOnIndex;
		trap->G2API.IKMove = &trap_G2API_IKMove;
		trap->G2API.InitGhoul2Model = &trap_G2API_InitGhoul2Model;
		trap->G2API.OverrideServer = &trap_G2API_OverrideServer;
		trap->G2API.RagEffectorGoal = &trap_G2API_RagEffectorGoal;
		trap->G2API.RagEffectorKick = &trap_G2API_RagEffectorKick;
		trap->G2API.RagForceSolve = &trap_G2API_RagForceSolve;
		trap->G2API.RagPCJConstraint = &trap_G2API_RagPCJConstraint;
		trap->G2API.RagPCJGradientSpeed = &trap_G2API_RagPCJGradientSpeed;
		trap->G2API.RemoveBone = &trap_G2API_RemoveBone;
		trap->G2API.RemoveGhoul2Model = &trap_G2API_RemoveGhoul2Model;
		trap->G2API.SetBoltInfo = &trap_G2API_SetBoltInfo;
		trap->G2API.SetBoneAnim = &trap_G2API_SetBoneAnim;
		trap->G2API.SetBoneAngles = &trap_G2API_SetBoneAngles;
		trap->G2API.SetBoneIKState = &trap_G2API_SetBoneIKState;
		trap->G2API.SetRootSurface = &trap_G2API_SetRootSurface;
		trap->G2API.SetNewOrigin = &trap_G2API_SetNewOrigin;
		trap->G2API.SetRagDoll = &trap_G2API_SetRagDoll;
		trap->G2API.SetSkin = &trap_G2API_SetSkin;
		trap->G2API.SetSurfaceOnOff = &trap_G2API_SetSurfaceOnOff;
		trap->G2API.SetTime = &trap_G2API_SetTime;
		trap->G2API.SkinlessModel = &trap_G2API_SkinlessModel;

		trap->Key.GetCatcher = &trap_Key_GetCatcher;
		trap->Key.GetKey = &trap_Key_GetKey;
		trap->Key.IsDown = &trap_Key_IsDown;
		trap->Key.SetCatcher = &trap_Key_SetCatcher;

		trap->Language.IsAsian = &trap_Language_IsAsian;
		trap->Language.ReadCharFromString = &trap_AnyLanguage_ReadCharFromString;
		trap->Language.UsesSpaces = &trap_Language_UsesSpaces;

		trap->PC.AddGlobalDefine = &trap_PC_AddGlobalDefine;
		trap->PC.FreeSource = &trap_PC_FreeSource;
		trap->PC.LoadGlobalDefines = &trap_PC_LoadGlobalDefines;
		trap->PC.LoadSource = &trap_PC_LoadSource;
		trap->PC.ReadToken = &trap_PC_ReadToken;
		trap->PC.RemoveAllGlobalDefines = &trap_PC_RemoveAllGlobalDefines;
		trap->PC.SourceFileAndLine = &trap_PC_SourceFileAndLine;

		trap->PrecisionTimer.End = &trap_PrecisionTimer_End;
		trap->PrecisionTimer.Start = &trap_PrecisionTimer_Start;

		trap->R.AutomapElevAdj = &trap_R_AutomapElevAdj;
		trap->R.AddAdditiveLightToScene = &trap_R_AddAdditiveLightToScene;
		trap->R.AddDecalToScene = &trap_R_AddDecalToScene;
		trap->R.AddLightToScene = &trap_R_AddLightToScene;
		trap->R.AddPolyToScene = &trap_R_AddPolyToScene;
		trap->R.AddPolysToScene = &trap_R_AddPolysToScene;
		trap->R.AddRefEntityToScene = &trap_R_AddRefEntityToScene;
		trap->R.ClearDecals = &trap_R_ClearDecals;
		trap->R.ClearScene = &trap_R_ClearScene;
		trap->R.DrawRotatePic = &trap_R_DrawRotatePic;
		trap->R.DrawRotatePic2 = &trap_R_DrawRotatePic2;
		trap->R.DrawStretchPic = &trap_R_DrawStretchPic;
		trap->R.Font.DrawString = &trap_R_Font_DrawString;
		trap->R.Font.HeightPixels = &trap_R_Font_HeightPixels;
		trap->R.Font.StrLenChars = &trap_R_Font_StrLenChars;
		trap->R.Font.StrLenPixels = &trap_R_Font_StrLenPixels;
		trap->R.GetBModelVerts  = &trap_R_GetBModelVerts;
		trap->R.GetDistanceCull = &trap_R_GetDistanceCull;
		trap->R.GetLightStyle = &trap_R_GetLightStyle;
		trap->R.GetRealRes = &trap_R_GetRealRes;
		trap->R.InitWireframeAutomap = &trap_R_InitWireframeAutomap;
		trap->R.InPVS = &trap_R_inPVS;
		trap->R.LerpTag = &trap_R_LerpTag;
		trap->R.LightForPoint = &trap_R_LightForPoint;
		trap->R.LoadWorldMap = &trap_R_LoadWorldMap;
		trap->R.ModelBounds = &trap_R_ModelBounds;
		trap->R.RenderScene = &trap_R_RenderScene;
		trap->R.RemapShader = &trap_R_RemapShader;
		trap->R.RegisterFont = &trap_R_RegisterFont;
		trap->R.RegisterModel = &trap_R_RegisterModel;
		trap->R.RegisterShader = &trap_R_RegisterShader;
		trap->R.RegisterShaderNoMip = &trap_R_RegisterShaderNoMip;
		trap->R.RegisterSkin = &trap_R_RegisterSkin;
		trap->R.SetColor = &trap_R_SetColor;
		trap->R.SetLightStyle = &trap_R_SetLightStyle;
		trap->R.SetRangeFog = &trap_R_SetRangeFog;
		trap->R.SetRefractProp = &trap_R_SetRefractProp;
		trap->R.WeatherContentsOverride = &trap_R_WeatherContentsOverride;
		trap->R.WorldEffectCommand = &trap_R_WorldEffectCommand;

		trap->ROFF.Cache = &trap_ROFF_Cache;
		trap->ROFF.Clean = &trap_ROFF_Clean;
		trap->ROFF.Play = &trap_ROFF_Play;
		trap->ROFF.PurgeEnt = &trap_ROFF_Purge_Ent;
		trap->ROFF.UpdateEntities = &trap_ROFF_UpdateEntities;

		trap->S.AddLocalSet = &trap_S_AddLocalSet;
		trap->S.AddLoopingSound = &trap_S_AddLoopingSound;
		trap->S.AddRealLoopingSound = &trap_S_AddRealLoopingSound;
		trap->S.ClearLoopingSounds = &trap_S_ClearLoopingSounds;
		trap->S.GetVoiceVolume = &trap_S_GetVoiceVolume;
		trap->S.MuteSound = &trap_S_MuteSound;
		trap->S.RegisterSound = &trap_S_RegisterSound;
		trap->S.Respatialize = &trap_S_Respatialize;
		trap->S.ShutUp = &trap_S_ShutUp;
		trap->S.StartBackgroundTrack = &trap_S_StartBackgroundTrack;
		trap->S.StartLocalSound = &trap_S_StartLocalSound;
		trap->S.StartSound = &trap_S_StartSound;
		trap->S.StopBackgroundTrack = &trap_S_StopBackgroundTrack;
		trap->S.StopLoopingSound = &trap_S_StopLoopingSound;
		trap->S.UpdateAmbientSet = &trap_S_UpdateAmbientSet;
		trap->S.UpdateEntityPosition = &trap_S_UpdateEntityPosition;
	}