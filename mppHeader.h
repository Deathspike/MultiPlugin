
	/**************************************************
	* MultiPlugin++ by Deathspike
	*
	* Client-side plugin project which allows any
	* developer to use this framework as a basis for
	* their own plugin.
	**************************************************/

	// Improve path detection algorithme, it's rather silly.

	// Detection:
	//	- VM_Create				(Not In Synch)
	//	- VM_Call				(Not In Synch)
	//	- VM_DllLoad			(Not In Synch)
	//	- NO-CD					(Not In Synch)
	//	- ScreenShot			(Always)
	//
	// Modules:
	//	- Immunize Mind			(Always, Removing Flags)
	//	- Team Colorization		(Always, Rendering Flags)
	//	- Texture Replacement	(Always, R_FindShader, Tracing Textures, Remap Data)

	#include "SDK/cg_local.h"					// SDK
	#include "mppShared.h"						// MultPlugin Shared

	#include <windows.h>
	#include <stdio.h>

	#define ADDRESS_COMMAND_ARG		0xB39808
	#define ADDRESS_COMMAND_COUNT	0xB3CC08

	#define ADDRESS_ENGINE_COMMAND	0xB3CC18
	#define ADDRESS_ENGINE_CVAR		0xB44228
	#define ADDRESS_ENGINE_STRING	0x553DC4
	#define ADDRESS_ENGINE_SWITCH	0x12A4F18

	#define ADDRESS_VIEW_ANGLES_X	0x97DF8C
	#define ADDRESS_VIEW_ANGLES_Y	0x97DF88

	#define ADDRESS_VM_CREATE		0x44A8C0
	#define ADDRESS_VM_FREE			0x44EAC0
	#define ADDRESS_VM_LOAD_DLL		0x450990

	/**************************************************
	* vm_t
	* --------------------------------------------------
	* This is the struct which holds all the pointers for
	* each VM. Do not mess with it unless you know 
	* exactly what you are doing!
	**************************************************/

	typedef struct {

		int					 programStack;
		int					( *systemCall )( int *parms );
		char				 name[64];
		void				*dllHandle;
		int					 ( __cdecl *entryPoint )( int callNum, ... );
		qboolean			 currentlyInterpreting;
		qboolean			 compiled;
		byte				*codeBase;
		int					 codeLength;
		int					*instructionPointers;
		int					 instructionPointersLength;
		byte				*dataBase;
		int					 dataMask;
		int					 stackBottom;
		int					 numSymbols;
		struct				 vmSymbol_s *symbols;
		int					 callLevel;
		int					 breakFunction;
		int					 breakCount;

	} vm_t;

	typedef struct {

		void				*hMod;				// The module handle which gets hidden almost instantly.
		char				*zPath;				// The path of the module, physical path for replacement.
		qboolean			 bNoRelease;		// True when the module should not get released).

		void				( *pEntryCall )( MultiPlugin_t * );
		int					( *pPreMainCall )( int, int, int, int, int, int, int, int, int, int, int, int, int );
		int					( *pPostMainCall )( int, int, int, int, int, int, int, int, int, int, int, int, int );
		int					( *pPreSystemCall )( int * );
		int					( *pPostSystemCall )( int * );

		void				*pNextPlugin;		// The handle of the next plugin, if any.
		void				*pPreviousPlugin;	// The handle of the previous plugin, if any.

	} MultiModule_t;

	typedef struct
	{

		qboolean	 bCheckWall;	// Set to true when a visibility check must be done!
		qboolean	 bPersistant;	// Set to true when persistant rendering is required (never cleared).
		char		*lpText;		// Allocated buffer of copied text.
		int			 iIndex;		// If set to -1, use the raw vector instead.
		int			 iLines;		// Increments the number of lines added for a specific entity.
		vec3_t		 vOrigin;		// The raw origin which is to be used when iIndex is invalid.

	} MultiRender_t;

	/**************************************************
	* This is the Shared Buffer across all the modules,
	* and contains about any function you'd might wish.
	**************************************************/

	extern MultiPlugin_t  MultiPlugin;
	extern MultiSystem_t  MultiSystem;
	extern MultiModule_t *MultiModule;
	extern MultiRender_t  pRenders[1024];
	extern int			  iRenders;

	/**************************************************
	* These are data segments filled in by MultiPlugin++ 
	* to avoid having to rely on unreliable data addresses,
	* which modifications are able to alter.
	**************************************************/

	extern clientInfo_t	clientInfo[MAX_CLIENTS];
	extern vec3_t		currentOrigin[MAX_GENTITIES];;
	extern qboolean		currentValid[MAX_GENTITIES];
	extern refdef_t		refdef;
	extern snapshot_t	snap;

	/**************************************************
	* These pointers contain the location of the VM for
	* the target module (cgame) and the addresses of
	* the real functions.
	**************************************************/

	extern HMODULE		 hModule;
	extern vm_t			*cgVM;
	extern vm_t			*uiVM;

	/**************************************************
	* mppDetour
	*
	* Provides an easy-to-use method to attach to functions,
	* patch bytes, replace inline calls and more. This
	* can be used by any plugin.
	**************************************************/

	unsigned long	 mppAttach( unsigned long dwAddress, unsigned long dwAddressTarget, char iType );
	void			 mppAttachClean( unsigned long dwAddress, unsigned long dwAddressTarget, unsigned long *dwAddressTrampoline, int iType );
	void			 mppDisassemble( unsigned char *iptr0, unsigned long *osizeptr );
	unsigned long	 mppDetach( unsigned long pAddress, unsigned long pTramp, qboolean bFreeTrampoline );
	unsigned long	 mppGetLen( unsigned long Address );
	unsigned long	 mppGetTramp( unsigned long pAddress, unsigned int iLen );
	unsigned long	 mppInlineFetch( unsigned long pAddress, unsigned int iLen );
	unsigned long	 mppInlinePatch( unsigned long pAddress, unsigned long pNewAddress, unsigned int iLen );
	void			 mppPatch( unsigned long pAddress, void *bByte, int iLen );
	void			 mppReProtect( unsigned long pAddress, unsigned int iLen );
	void			 mppUnProtect( unsigned long pAddress, unsigned int iLen );

	/**************************************************
	* mppGame
	*
	* This is the game interface, where only new game functions
	* are stored. This includes methods to retrieve any command or
	* cvar from the engine.
	**************************************************/

	void			 mppAimAtVector( vec3_t targOrigin );
	qboolean		 mppIsPlayerAlly( int iIndex );
	centity_t		*mppIsPlayerEntity( int iIndex );
	qboolean		 mppIsPlayerInDuel( int iIndex );
	cmd_t			*mppLocateCommand( char *name );
	cvar_t			*mppLocateCvar( char *name );
	void			 mppPerformScreenshot( qboolean bDoScreenshot );
	void			 mppRawTextCalculateDraw( char *lpString, float x, float y, float fScale, int iFont, int iLines );
	int				 mppRawTextCalculateHeight( int iFont, float fScale );
	int				 mppRawTextCalculateWidth( char *lpString, int iFont, float fScale );
	int				 mppRenderTextAtEntity( int iIndex, char *lpString, qboolean bCheckWall, qboolean bPersistant );
	int				 mppRenderTextAtVector( vec3_t origin, char *lpString, qboolean bCheckWall, qboolean bPersistant );
	void			 mppRenderTextClear( int iIndex );
	void			 mppRenderTextFinalize( void );
	qboolean		 mppRenderTextVisible( vec3_t origin, qboolean checkWalls );
	void			 mppScreenshotCall( void );
	void			 mppScreenshotDetour();
	qboolean		 mppWorldToScreen( vec3_t point, float *x, float *y );

	/**************************************************
	* mppImports
	*
	* Imports functions from the SDK, which are useful
	* and should be available here. Please note that
	* these are only core functions.
	**************************************************/

	int				 Cmd_Argc( void );
	char			*Cmd_Argv( int arg );
	char			*Cmd_ConcatArgs( int start );
	void			 Com_Error( int level, const char *error, ... );
	void			 Com_Printf( const char *msg, ... );
	void			 Com_Sprintf( char *dest, int size, const char *fmt, ... );
	void			 Com_StringEscape( char *in, char *out );
	char			*CopyString( const char *in );
	void			 FreeString( char *lpString );
	char			*Q_CleanStr( char *string );
	int				 Q_stricmpn( const char *s1, const char *s2, int n );
	int				 Q_stricmp( const char *s1, const char *s2 );
	char			*va( const char *format, ... );

	/**************************************************
	* mppMain
	*
	* This is the main application entry point, attaches
	* itself to the host process and detours some beloved
	* functions. Provides the way to intercept, patch for
	* a working NO-CD, and holds the metamod-like functions.
	**************************************************/

	void			 DllAttach();
	vm_t			*DllCreate( char *module, int *systemCalls, int interpret );
	void			 DllDetach();
	void			 DllFree( vm_t *vm );
	void			 DllHide( HMODULE hMod );
	void			*DllLoadDll( unsigned long systemCalls );
	void			 DllPlugin( HMODULE hMod );
	void			*DllPointer( vm_t *pVM, int x );
	void			 DllStealth( vm_t *pVM, qboolean bEnableStealth );

	/**************************************************
	* mppPlugin
	*
	* Manages the calls in and from the plugin, as well
	* as including of our own required initialization
	* calls. It also loads other DLL's, manages them
	* and so forth.
	**************************************************/

	void			 mppPluginInit( MultiPlugin_t *pPlugin );
	void			 mppPluginLoad( char *zModule );
	int				 mppPluginCGMain( int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 );
	int				 mppPluginCGSystem( int *arg );
	int				 mppPluginUIMain( int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11 );
	int				 mppPluginUISystem( int *arg );

	/**************************************************
	* mppSystem
	*
	* Imports functions from the SDK, which are useful
	* and should be available here. Please note that
	* these are only system call functions. It incorporates
	* a simple wrapper to make the actual calls.
	**************************************************/

	int				 mppSystemCall( int iCmd, ... );
	void			 mppSystemInit( MultiSystem_t *trap );