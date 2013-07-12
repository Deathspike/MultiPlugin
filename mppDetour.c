
	/**************************************************
	* MultiPlugin++ by Deathspike
	*
	* Client-side plugin project which allows any
	* developer to use this framework as a basis for
	* their own plugin.
	**************************************************/

	#include "mppHeader.h"

	/**************************************************
	* MakeDelta/MakePtr and the warning definitions!
	**************************************************/

	unsigned long dwOldProtect;
	#pragma warning ( disable: 4005 )
	#define MakeDelta( cast, x, y )			( cast )(( unsigned long )( x ) - ( unsigned long )( y ))
	#define MakePtr( cast, x, y )			( cast )(( unsigned long )( x ) + ( unsigned long )( y ))

	/**************************************************
	* mppAttach
	* 
	* Attaches a detour on the target function and makes 
	* it jump to the new provided address, should be a
	* pointer.
	**************************************************/

	unsigned long mppAttach( unsigned long dwAddress, unsigned long dwAddressTarget, char iType )
	{
		unsigned long iLen = mppGetLen( dwAddress );
		unsigned long dwTrampoline = mppGetTramp( dwAddress, iLen );

		mppUnProtect( dwAddress, iLen );
		*(( unsigned char * )(( unsigned long ) dwAddress )) = iType;
		*(( unsigned long * )(( unsigned long ) dwAddress + 1 )) = MakeDelta( unsigned long, dwAddressTarget, dwAddress + 5 );
		mppReProtect( dwAddress, iLen );

		return dwTrampoline;
	}

	/**************************************************
	* mppAttachClean
	* 
	* A simple stubstitute function for Attach, which
	* looks slightly cleaner because the storage point
	* is passed as a pointer.
	**************************************************/

	void mppAttachClean( unsigned long dwAddress, unsigned long dwAddressTarget, unsigned long *dwAddressTrampoline, int iType )
	{
		*dwAddressTrampoline = mppAttach( dwAddress, dwAddressTarget, iType );
	}

	/**************************************************
	* mppDetach
	* 
	* Detaches a function detour. The length is calculated
	* and the original bytes are restored, tramp is then
	* released.
	**************************************************/

	unsigned long mppDetach( unsigned long pAddress, unsigned long pTramp, qboolean bFreeTrampoline )
	{
		unsigned long iLen = mppGetLen( pTramp );

		mppUnProtect( pAddress, iLen );
		memcpy(( void * ) pAddress, ( void * ) pTramp, iLen );

		mppReProtect( pAddress, iLen );
		if ( bFreeTrampoline ) free(( void * ) pTramp );
		return pAddress;
	}

	/**************************************************
	* mppGetLen
	* 
	* Retrieve the minimum ammount of opcodes to patch
	* at the given instruction. Utilizes the DisAssemble
	* function to count the opcodes.
	**************************************************/

	unsigned long mppGetLen( unsigned long pAddress )
	{
		unsigned long iLen = 0;
		unsigned long iSize = 0;

		while( iSize < 5 )
		{
			mppDisassemble(( unsigned char * ) pAddress, &iLen );
			pAddress += iLen;
			iSize += iLen;
		}

		return iSize;
	}

	/**************************************************
	* mppGetTramp
	* 
	* Creates a trampoline, allocates the minimum amount
	* of memory for the length, copies the opcodes to
	* the trampoline and creates a jump to continue
	* execution.
	**************************************************/

	unsigned long mppGetTramp( unsigned long pAddress, unsigned int iLen )
	{
		unsigned long pTramp = ( unsigned long ) malloc( iLen + 5 );

		memcpy(( void * ) pTramp, ( void * ) pAddress, iLen );
		*( unsigned char * )( pTramp + iLen ) = 0xE9;
		*( unsigned long * )( pTramp + iLen + 1 ) = ( unsigned long )(( unsigned long )( pAddress + iLen ) - ( unsigned long )( pTramp + iLen + 5 ));

		return pTramp;
	}

	/**************************************************
	* mppInlineFetch
	* 
	* Returns the absolute address of the provided
	* instruction. The length expects the total length
	* of the instruction.
	**************************************************/

	unsigned long mppInlineFetch( unsigned long dwAddress, unsigned int iLen )
	{
		unsigned long *dwAddressBase;
		unsigned long  dwAddressOffset;

		switch( iLen )
		{
			case 1:
			case 2:
			{
				dwAddressBase	=  ( unsigned long  * )( dwAddress + 2 );
				dwAddressOffset	= *( unsigned char  * )( dwAddress + 1 );
				break;
			}

			case 3:
			{
				dwAddressBase	=  ( unsigned long  * )( dwAddress + 3 );
				dwAddressOffset	= *( unsigned short * )( dwAddress + 1 );
				break;
			}

			default: /*case 5:*/
			{
				dwAddressBase	=  ( unsigned long * )( dwAddress + 5 );
				dwAddressOffset	= *( unsigned long * )( dwAddress + 1 );
				break;
			}
		}

		return MakePtr( unsigned long, dwAddressBase, dwAddressOffset );
	}

	/**************************************************
	* mppInlinePatch
	* 
	* Patches the address which is stored in anything
	* similar to a jump or call (4 bytes address after
	* a single opcode).
	**************************************************/

	unsigned long mppInlinePatch( unsigned long pAddress, unsigned long pNewAddress, unsigned int iLen )
	{
		unsigned long pReturnAddress = mppInlineFetch(( unsigned long ) pAddress, iLen );
		mppUnProtect( pAddress, iLen );

		switch( iLen )
		{
			case 2:
			{
				*( unsigned char * )( pAddress + 1 ) = MakeDelta( unsigned char, pNewAddress, pAddress + 2 );
				break;
			}

			case 3:
			{
				*( unsigned short * )( pAddress + 1 ) = MakeDelta( unsigned short, pNewAddress, pAddress + 3 );
				break;
			}

			default: /*case 5:*/
			{
				*( unsigned long * )( pAddress + 1 ) = MakeDelta( unsigned long, pNewAddress, pAddress + 5 );
				break;
			}
		}

		mppReProtect( pAddress, iLen );
		return pReturnAddress;
	}

	/**************************************************
	* mppPatch
	* 
	* Patches a single byte at the target address, 
	* usually performed to skip instructions from that 
	* given address.
	**************************************************/

	void mppPatch( unsigned long pAddress, void *bByte, int iLen )
	{
		mppUnProtect( pAddress, iLen );

		switch( iLen )
		{
			case 2:
			{
				*(( unsigned short * ) pAddress ) = ( unsigned short  ) bByte;
				break;
			}

			case 4:
			{
				*(( unsigned long *  ) pAddress ) = ( unsigned long   ) bByte;
				break;
			}

			default:
			{
				*(( unsigned char *  ) pAddress ) = ( unsigned char   ) bByte;
				break;
			}
		}

		mppReProtect( pAddress, iLen );
	}
	/**************************************************
	* mppReProtect
	* 
	* Reapplies protection to the target address, must
	* be called after any UnProtect call!
	**************************************************/

	void mppReProtect( unsigned long pAddress, unsigned int iLen )
	{

	#ifdef WIN32

		VirtualProtect(( void * ) pAddress, iLen, dwOldProtect, &dwOldProtect );

	#endif

	}

	/**************************************************
	* mppUnProtect
	* 
	* Removes protection to the target address, after
	* which it can be written upon. Do not open an entire
	* page. On linux we have to determine whether or not
	* the entire length is on the same page.
	**************************************************/

	void mppUnProtect( unsigned long pAddress, unsigned int iLen )
	{

	#ifndef WIN32

		int iPage1 = ( int ) pAddress &~ ( getpagesize() - 1 );
		int iPage2 = (( int ) pAddress + iLen ) &~ ( getpagesize() - 1 );

		if ( iPage1 == iPage2 )
		{
			mprotect(( char * ) iPage1, getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC );
		}
		else
		{
			mprotect(( char * ) iPage1, getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC );
			mprotect(( char * ) iPage2, getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC );
		}

	#else

		VirtualProtect(( void * ) pAddress, iLen, PAGE_EXECUTE_READWRITE, &dwOldProtect );

	#endif

	}

	/**************************************************
	* The following function is the disassembler library,
	* definitions and utilized functions. It is not
	* recommended to alter this.
	**************************************************/

	typedef unsigned long	DWORD;
	#define C_ERROR         0xFFFFFFFF
	#define C_PREFIX        0x00000001
	#define C_66            0x00000002
	#define C_67            0x00000004
	#define C_DATA66        0x00000008
	#define C_DATA1         0x00000010
	#define C_DATA2         0x00000020
	#define C_DATA4         0x00000040
	#define C_MEM67         0x00000080
	#define C_MEM1          0x00000100
	#define C_MEM2          0x00000200
	#define C_MEM4          0x00000400
	#define C_MODRM         0x00000800
	#define C_DATAW0        0x00001000
	#define C_FUCKINGTEST   0x00002000
	#define C_TABLE_0F      0x00004000

	int table_1[256] =
	{
	  /* 00 */   C_MODRM
	  /* 01 */,  C_MODRM
	  /* 02 */,  C_MODRM
	  /* 03 */,  C_MODRM
	  /* 04 */,  C_DATAW0
	  /* 05 */,  C_DATAW0
	  /* 06 */,  0
	  /* 07 */,  0
	  /* 08 */,  C_MODRM
	  /* 09 */,  C_MODRM
	  /* 0A */,  C_MODRM
	  /* 0B */,  C_MODRM
	  /* 0C */,  C_DATAW0
	  /* 0D */,  C_DATAW0
	  /* 0E */,  0
	  /* 0F */,  C_TABLE_0F
	  /* 10 */,  C_MODRM
	  /* 11 */,  C_MODRM
	  /* 12 */,  C_MODRM
	  /* 13 */,  C_MODRM
	  /* 14 */,  C_DATAW0
	  /* 15 */,  C_DATAW0
	  /* 16 */,  0
	  /* 17 */,  0
	  /* 18 */,  C_MODRM
	  /* 19 */,  C_MODRM
	  /* 1A */,  C_MODRM
	  /* 1B */,  C_MODRM
	  /* 1C */,  C_DATAW0
	  /* 1D */,  C_DATAW0
	  /* 1E */,  0
	  /* 1F */,  0
	  /* 20 */,  C_MODRM
	  /* 21 */,  C_MODRM
	  /* 22 */,  C_MODRM
	  /* 23 */,  C_MODRM
	  /* 24 */,  C_DATAW0
	  /* 25 */,  C_DATAW0
	  /* 26 */,  C_PREFIX
	  /* 27 */,  0
	  /* 28 */,  C_MODRM
	  /* 29 */,  C_MODRM
	  /* 2A */,  C_MODRM
	  /* 2B */,  C_MODRM
	  /* 2C */,  C_DATAW0
	  /* 2D */,  C_DATAW0
	  /* 2E */,  C_PREFIX
	  /* 2F */,  0
	  /* 30 */,  C_MODRM
	  /* 31 */,  C_MODRM
	  /* 32 */,  C_MODRM
	  /* 33 */,  C_MODRM
	  /* 34 */,  C_DATAW0
	  /* 35 */,  C_DATAW0
	  /* 36 */,  C_PREFIX
	  /* 37 */,  0
	  /* 38 */,  C_MODRM
	  /* 39 */,  C_MODRM
	  /* 3A */,  C_MODRM
	  /* 3B */,  C_MODRM
	  /* 3C */,  C_DATAW0
	  /* 3D */,  C_DATAW0
	  /* 3E */,  C_PREFIX
	  /* 3F */,  0
	  /* 40 */,  0
	  /* 41 */,  0
	  /* 42 */,  0
	  /* 43 */,  0
	  /* 44 */,  0
	  /* 45 */,  0
	  /* 46 */,  0
	  /* 47 */,  0
	  /* 48 */,  0
	  /* 49 */,  0
	  /* 4A */,  0
	  /* 4B */,  0
	  /* 4C */,  0
	  /* 4D */,  0
	  /* 4E */,  0
	  /* 4F */,  0
	  /* 50 */,  0
	  /* 51 */,  0
	  /* 52 */,  0
	  /* 53 */,  0
	  /* 54 */,  0
	  /* 55 */,  0
	  /* 56 */,  0
	  /* 57 */,  0
	  /* 58 */,  0
	  /* 59 */,  0
	  /* 5A */,  0
	  /* 5B */,  0
	  /* 5C */,  0
	  /* 5D */,  0
	  /* 5E */,  0
	  /* 5F */,  0
	  /* 60 */,  0
	  /* 61 */,  0
	  /* 62 */,  C_MODRM
	  /* 63 */,  C_MODRM
	  /* 64 */,  C_PREFIX
	  /* 65 */,  C_PREFIX
	  /* 66 */,  C_PREFIX+C_66
	  /* 67 */,  C_PREFIX+C_67
	  /* 68 */,  C_DATA66
	  /* 69 */,  C_MODRM+C_DATA66
	  /* 6A */,  C_DATA1
	  /* 6B */,  C_MODRM+C_DATA1
	  /* 6C */,  0
	  /* 6D */,  0
	  /* 6E */,  0
	  /* 6F */,  0
	  /* 70 */,  C_DATA1
	  /* 71 */,  C_DATA1
	  /* 72 */,  C_DATA1
	  /* 73 */,  C_DATA1
	  /* 74 */,  C_DATA1
	  /* 75 */,  C_DATA1
	  /* 76 */,  C_DATA1
	  /* 77 */,  C_DATA1
	  /* 78 */,  C_DATA1
	  /* 79 */,  C_DATA1
	  /* 7A */,  C_DATA1
	  /* 7B */,  C_DATA1
	  /* 7C */,  C_DATA1
	  /* 7D */,  C_DATA1
	  /* 7E */,  C_DATA1
	  /* 7F */,  C_DATA1
	  /* 80 */,  C_MODRM+C_DATA1
	  /* 81 */,  C_MODRM+C_DATA66
	  /* 82 */,  C_MODRM+C_DATA1
	  /* 83 */,  C_MODRM+C_DATA1
	  /* 84 */,  C_MODRM
	  /* 85 */,  C_MODRM
	  /* 86 */,  C_MODRM
	  /* 87 */,  C_MODRM
	  /* 88 */,  C_MODRM
	  /* 89 */,  C_MODRM
	  /* 8A */,  C_MODRM
	  /* 8B */,  C_MODRM
	  /* 8C */,  C_MODRM
	  /* 8D */,  C_MODRM
	  /* 8E */,  C_MODRM
	  /* 8F */,  C_MODRM
	  /* 90 */,  0
	  /* 91 */,  0
	  /* 92 */,  0
	  /* 93 */,  0
	  /* 94 */,  0
	  /* 95 */,  0
	  /* 96 */,  0
	  /* 97 */,  0
	  /* 98 */,  0
	  /* 99 */,  0
	  /* 9A */,  C_DATA66+C_MEM2
	  /* 9B */,  0
	  /* 9C */,  0
	  /* 9D */,  0
	  /* 9E */,  0
	  /* 9F */,  0
	  /* A0 */,  C_MEM67
	  /* A1 */,  C_MEM67
	  /* A2 */,  C_MEM67
	  /* A3 */,  C_MEM67
	  /* A4 */,  0
	  /* A5 */,  0
	  /* A6 */,  0
	  /* A7 */,  0
	  /* A8 */,  C_DATA1
	  /* A9 */,  C_DATA66
	  /* AA */,  0
	  /* AB */,  0
	  /* AC */,  0
	  /* AD */,  0
	  /* AE */,  0
	  /* AF */,  0
	  /* B0 */,  C_DATA1
	  /* B1 */,  C_DATA1
	  /* B2 */,  C_DATA1
	  /* B3 */,  C_DATA1
	  /* B4 */,  C_DATA1
	  /* B5 */,  C_DATA1
	  /* B6 */,  C_DATA1
	  /* B7 */,  C_DATA1
	  /* B8 */,  C_DATA66
	  /* B9 */,  C_DATA66
	  /* BA */,  C_DATA66
	  /* BB */,  C_DATA66
	  /* BC */,  C_DATA66
	  /* BD */,  C_DATA66
	  /* BE */,  C_DATA66
	  /* BF */,  C_DATA66
	  /* C0 */,  C_MODRM+C_DATA1
	  /* C1 */,  C_MODRM+C_DATA1
	  /* C2 */,  C_DATA2
	  /* C3 */,  0
	  /* C4 */,  C_MODRM
	  /* C5 */,  C_MODRM
	  /* C6 */,  C_MODRM+C_DATA66
	  /* C7 */,  C_MODRM+C_DATA66
	  /* C8 */,  C_DATA2+C_DATA1
	  /* C9 */,  0
	  /* CA */,  C_DATA2
	  /* CB */,  0
	  /* CC */,  0
	  /* CD */,  C_DATA1+C_DATA4
	  /* CE */,  0
	  /* CF */,  0
	  /* D0 */,  C_MODRM
	  /* D1 */,  C_MODRM
	  /* D2 */,  C_MODRM
	  /* D3 */,  C_MODRM
	  /* D4 */,  0
	  /* D5 */,  0
	  /* D6 */,  0
	  /* D7 */,  0
	  /* D8 */,  C_MODRM
	  /* D9 */,  C_MODRM
	  /* DA */,  C_MODRM
	  /* DB */,  C_MODRM
	  /* DC */,  C_MODRM
	  /* DD */,  C_MODRM
	  /* DE */,  C_MODRM
	  /* DF */,  C_MODRM
	  /* E0 */,  C_DATA1
	  /* E1 */,  C_DATA1
	  /* E2 */,  C_DATA1
	  /* E3 */,  C_DATA1
	  /* E4 */,  C_DATA1
	  /* E5 */,  C_DATA1
	  /* E6 */,  C_DATA1
	  /* E7 */,  C_DATA1
	  /* E8 */,  C_DATA66
	  /* E9 */,  C_DATA66
	  /* EA */,  C_DATA66+C_MEM2
	  /* EB */,  C_DATA1
	  /* EC */,  0
	  /* ED */,  0
	  /* EE */,  0
	  /* EF */,  0
	  /* F0 */,  C_PREFIX
	  /* F1 */,  0                       // 0xF1
	  /* F2 */,  C_PREFIX
	  /* F3 */,  C_PREFIX
	  /* F4 */,  0
	  /* F5 */,  0
	  /* F6 */,  C_FUCKINGTEST
	  /* F7 */,  C_FUCKINGTEST
	  /* F8 */,  0
	  /* F9 */,  0
	  /* FA */,  0
	  /* FB */,  0
	  /* FC */,  0
	  /* FD */,  0
	  /* FE */,  C_MODRM
	  /* FF */,  C_MODRM
	}; // table_1

	int table_0F[256] =
	{
	  /* 00 */   C_MODRM
	  /* 01 */,  C_MODRM
	  /* 02 */,  C_MODRM
	  /* 03 */,  C_MODRM
	  /* 04 */,  -1
	  /* 05 */,  -1
	  /* 06 */,  0
	  /* 07 */,  -1
	  /* 08 */,  0
	  /* 09 */,  0
	  /* 0A */,  0
	  /* 0B */,  0
	  /* 0C */,  -1
	  /* 0D */,  -1
	  /* 0E */,  -1
	  /* 0F */,  -1
	  /* 10 */,  -1
	  /* 11 */,  -1
	  /* 12 */,  -1
	  /* 13 */,  -1
	  /* 14 */,  -1
	  /* 15 */,  -1
	  /* 16 */,  -1
	  /* 17 */,  -1
	  /* 18 */,  -1
	  /* 19 */,  -1
	  /* 1A */,  -1
	  /* 1B */,  -1
	  /* 1C */,  -1
	  /* 1D */,  -1
	  /* 1E */,  -1
	  /* 1F */,  -1
	  /* 20 */,  -1
	  /* 21 */,  -1
	  /* 22 */,  -1
	  /* 23 */,  -1
	  /* 24 */,  -1
	  /* 25 */,  -1
	  /* 26 */,  -1
	  /* 27 */,  -1
	  /* 28 */,  -1
	  /* 29 */,  -1
	  /* 2A */,  -1
	  /* 2B */,  -1
	  /* 2C */,  -1
	  /* 2D */,  -1
	  /* 2E */,  -1
	  /* 2F */,  -1
	  /* 30 */,  -1
	  /* 31 */,  -1
	  /* 32 */,  -1
	  /* 33 */,  -1
	  /* 34 */,  -1
	  /* 35 */,  -1
	  /* 36 */,  -1
	  /* 37 */,  -1
	  /* 38 */,  -1
	  /* 39 */,  -1
	  /* 3A */,  -1
	  /* 3B */,  -1
	  /* 3C */,  -1
	  /* 3D */,  -1
	  /* 3E */,  -1
	  /* 3F */,  -1
	  /* 40 */,  -1
	  /* 41 */,  -1
	  /* 42 */,  -1
	  /* 43 */,  -1
	  /* 44 */,  -1
	  /* 45 */,  -1
	  /* 46 */,  -1
	  /* 47 */,  -1
	  /* 48 */,  -1
	  /* 49 */,  -1
	  /* 4A */,  -1
	  /* 4B */,  -1
	  /* 4C */,  -1
	  /* 4D */,  -1
	  /* 4E */,  -1
	  /* 4F */,  -1
	  /* 50 */,  -1
	  /* 51 */,  -1
	  /* 52 */,  -1
	  /* 53 */,  -1
	  /* 54 */,  -1
	  /* 55 */,  -1
	  /* 56 */,  -1
	  /* 57 */,  -1
	  /* 58 */,  -1
	  /* 59 */,  -1
	  /* 5A */,  -1
	  /* 5B */,  -1
	  /* 5C */,  -1
	  /* 5D */,  -1
	  /* 5E */,  -1
	  /* 5F */,  -1
	  /* 60 */,  -1
	  /* 61 */,  -1
	  /* 62 */,  -1
	  /* 63 */,  -1
	  /* 64 */,  -1
	  /* 65 */,  -1
	  /* 66 */,  -1
	  /* 67 */,  -1
	  /* 68 */,  -1
	  /* 69 */,  -1
	  /* 6A */,  -1
	  /* 6B */,  -1
	  /* 6C */,  -1
	  /* 6D */,  -1
	  /* 6E */,  -1
	  /* 6F */,  -1
	  /* 70 */,  -1
	  /* 71 */,  -1
	  /* 72 */,  -1
	  /* 73 */,  -1
	  /* 74 */,  -1
	  /* 75 */,  -1
	  /* 76 */,  -1
	  /* 77 */,  -1
	  /* 78 */,  -1
	  /* 79 */,  -1
	  /* 7A */,  -1
	  /* 7B */,  -1
	  /* 7C */,  -1
	  /* 7D */,  -1
	  /* 7E */,  -1
	  /* 7F */,  -1
	  /* 80 */,  C_DATA66
	  /* 81 */,  C_DATA66
	  /* 82 */,  C_DATA66
	  /* 83 */,  C_DATA66
	  /* 84 */,  C_DATA66
	  /* 85 */,  C_DATA66
	  /* 86 */,  C_DATA66
	  /* 87 */,  C_DATA66
	  /* 88 */,  C_DATA66
	  /* 89 */,  C_DATA66
	  /* 8A */,  C_DATA66
	  /* 8B */,  C_DATA66
	  /* 8C */,  C_DATA66
	  /* 8D */,  C_DATA66
	  /* 8E */,  C_DATA66
	  /* 8F */,  C_DATA66
	  /* 90 */,  C_MODRM
	  /* 91 */,  C_MODRM
	  /* 92 */,  C_MODRM
	  /* 93 */,  C_MODRM
	  /* 94 */,  C_MODRM
	  /* 95 */,  C_MODRM
	  /* 96 */,  C_MODRM
	  /* 97 */,  C_MODRM
	  /* 98 */,  C_MODRM
	  /* 99 */,  C_MODRM
	  /* 9A */,  C_MODRM
	  /* 9B */,  C_MODRM
	  /* 9C */,  C_MODRM
	  /* 9D */,  C_MODRM
	  /* 9E */,  C_MODRM
	  /* 9F */,  C_MODRM
	  /* A0 */,  0
	  /* A1 */,  0
	  /* A2 */,  0
	  /* A3 */,  C_MODRM
	  /* A4 */,  C_MODRM+C_DATA1
	  /* A5 */,  C_MODRM
	  /* A6 */,  -1
	  /* A7 */,  -1
	  /* A8 */,  0
	  /* A9 */,  0
	  /* AA */,  0
	  /* AB */,  C_MODRM
	  /* AC */,  C_MODRM+C_DATA1
	  /* AD */,  C_MODRM
	  /* AE */,  -1
	  /* AF */,  C_MODRM
	  /* B0 */,  C_MODRM
	  /* B1 */,  C_MODRM
	  /* B2 */,  C_MODRM
	  /* B3 */,  C_MODRM
	  /* B4 */,  C_MODRM
	  /* B5 */,  C_MODRM
	  /* B6 */,  C_MODRM
	  /* B7 */,  C_MODRM
	  /* B8 */,  -1
	  /* B9 */,  -1
	  /* BA */,  C_MODRM+C_DATA1
	  /* BB */,  C_MODRM
	  /* BC */,  C_MODRM
	  /* BD */,  C_MODRM
	  /* BE */,  C_MODRM
	  /* BF */,  C_MODRM
	  /* C0 */,  C_MODRM
	  /* C1 */,  C_MODRM
	  /* C2 */,  -1
	  /* C3 */,  -1
	  /* C4 */,  -1
	  /* C5 */,  -1
	  /* C6 */,  -1
	  /* C7 */,  -1
	  /* C8 */,  0
	  /* C9 */,  0
	  /* CA */,  0
	  /* CB */,  0
	  /* CC */,  0
	  /* CD */,  0
	  /* CE */,  0
	  /* CF */,  0
	  /* D0 */,  -1
	  /* D1 */,  -1
	  /* D2 */,  -1
	  /* D3 */,  -1
	  /* D4 */,  -1
	  /* D5 */,  -1
	  /* D6 */,  -1
	  /* D7 */,  -1
	  /* D8 */,  -1
	  /* D9 */,  -1
	  /* DA */,  -1
	  /* DB */,  -1
	  /* DC */,  -1
	  /* DD */,  -1
	  /* DE */,  -1
	  /* DF */,  -1
	  /* E0 */,  -1
	  /* E1 */,  -1
	  /* E2 */,  -1
	  /* E3 */,  -1
	  /* E4 */,  -1
	  /* E5 */,  -1
	  /* E6 */,  -1
	  /* E7 */,  -1
	  /* E8 */,  -1
	  /* E9 */,  -1
	  /* EA */,  -1
	  /* EB */,  -1
	  /* EC */,  -1
	  /* ED */,  -1
	  /* EE */,  -1
	  /* EF */,  -1
	  /* F0 */,  -1
	  /* F1 */,  -1
	  /* F2 */,  -1
	  /* F3 */,  -1
	  /* F4 */,  -1
	  /* F5 */,  -1
	  /* F6 */,  -1
	  /* F7 */,  -1
	  /* F8 */,  -1
	  /* F9 */,  -1
	  /* FA */,  -1
	  /* FB */,  -1
	  /* FC */,  -1
	  /* FD */,  -1
	  /* FE */,  -1
	  /* FF */,  -1
	};

	/**************************************************
	* mppDisassemble
	*
	* Disassembles the provided address and returns the
	* size of the opcode in the pointer. This is mainly
	* a length-disassembler and should not be utilized
	* in any other way.
	**************************************************/

	void mppDisassemble( unsigned char *iptr0, unsigned long *osizeptr )
	{
		unsigned char	*iptr = iptr0;
		unsigned char	 mod, rm, b;
		unsigned int	 f = 0;

	prefix: 

		b  = *iptr++;
		f |= table_1[b];

		if ( f & C_FUCKINGTEST )
		{
			if ((( *iptr ) &0x38 ) == 0x00 )
			{
				f = C_MODRM + C_DATAW0;
			}
			else
			{
				f = C_MODRM;
			}
		}

		if ( f &C_TABLE_0F )
		{
			b = *iptr++;
			f = table_0F[b];
		}

		if (f == C_ERROR)
		{
			*osizeptr = C_ERROR;
			return;
		}

		if ( f & C_PREFIX )
		{
			f &= ~C_PREFIX;
			goto prefix;
		}

		if ( f & C_DATAW0 )
		{
			if ( b & 0x01 )
			{
				f |= C_DATA66;
			}
			else
			{
				f |= C_DATA1;
			}
		}

		if ( f &C_MODRM )
		{
			b	=  *iptr++;
			mod	= b &0xC0;
			rm	= b &0x07;

			if ( mod != 0xC0 )
			{
				// modrm16
				if ( f &C_67 )
				{
					if (( mod == 0x00 ) && ( rm == 0x06 ))
					{
						f |= C_MEM2;
					}

					if ( mod == 0x40 )
					{
						f |= C_MEM1;
					}

					if ( mod == 0x80 )
					{
						f |= C_MEM2;
					}
				}
				// modrm32
				else
				{
					if ( mod == 0x40 )
					{
						f |= C_MEM1;
					}

					if (mod == 0x80)
					{
						f |= C_MEM4;
					}

					if (rm == 0x04)
					{
						rm = (*iptr++) &0x07;
					}

					// rm<-sib.base
					if (( rm == 0x05 ) && ( mod == 0x00 ))
					{
						f |= C_MEM4;
					}
				}
			}
		}

		if ( f & C_MEM67 )
		{
			if (f & C_67 )
			{
				f |= C_MEM2;
			}
			else
			{
				f |= C_MEM4;
			}
		}

		if ( f & C_DATA66 )
		{
			if ( f & C_66 )
			{
				f |= C_DATA2;
			}
			else
			{
				f |= C_DATA4;
			}
		}

		if ( f & C_MEM1 )
		{
			iptr++;
		}

		if ( f &C_MEM2 )
		{
			iptr += 2;
		}

		if ( f & C_MEM4 )
		{
			iptr += 4;
		}

		if ( f & C_DATA1 )
		{
			iptr++;
		}
		if ( f & C_DATA2 )
		{
			iptr += 2;
		}
		if ( f & C_DATA4 )
		{
			iptr += 4;
		}

		*osizeptr = ( unsigned int )( iptr - iptr0 );
	}