
	/**************************************************
	* MultiPlugin++ by Deathspike
	*
	* Client-side plugin project which allows any
	* developer to use this framework as a basis for
	* their own plugin.
	**************************************************/

	#include "mppHeader.h"
	#pragma warning( disable: 4996 )

	/**************************************************
	* The null origin, the usual base of anything.
	**************************************************/

	vec3_t vec3_origin = { 0, 0, 0};

	/**************************************************
	* AngleNormalize360
	*
	* Returns angle normalized to the range [0 <= angle < 360]
	**************************************************/

	float AngleNormalize360( float angle )
	{
		return ( 360.0 / 65536 ) * (( int )( angle * ( 65536 / 360.0 )) & 65535 );
	}

	/**************************************************
	* AngleNormalize180
	*
	* Returns angle normalized to the range [-180 < angle <= 180]
	**************************************************/

	float AngleNormalize180( float angle )
	{
		angle = AngleNormalize360( angle );

		if ( angle > 180.0 )
		{
			angle -= 360.0;
		}

		return angle;
	}

	/**************************************************
	* AngleDelta
	*
	* Returns the normalized delta from angle1 to angle2
	**************************************************/

	float AngleDelta( float angle1, float angle2 )
	{
		return AngleNormalize180( angle1 - angle2 );
	}

	/**************************************************
	* AnglesToAxis
	*
	* Returns the axis based on the angles
	**************************************************/

	void AnglesToAxis( const vec3_t angles, vec3_t axis[3] )
	{
		vec3_t	right;
		AngleVectors( angles, axis[0], right, axis[2] );
		VectorSubtract( vec3_origin, right, axis[1] );
	}

	/**************************************************
	* AnglesToAxis
	*
	* Returns the forward, right and up vectors from the angles.
	**************************************************/

	void AngleVectors( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up )
	{
		float			angle;
		static float	sr, sp, sy, cr, cp, cy;

		angle = angles[YAW] * ( M_PI*2 / 360 );
		sy = sin( angle );
		cy = cos( angle );

		angle = angles[PITCH] * (M_PI * 2 / 360 );
		sp = sin( angle );
		cp = cos( angle );

		angle = angles[ROLL] * ( M_PI * 2 / 360 );
		sr = sin( angle );
		cr = cos( angle );

		if ( forward )
		{
			forward[0] = cp * cy;
			forward[1] = cp * sy;
			forward[2] = -sp;
		}

		if ( right )
		{
			right[0] = ( -1 * sr * sp * cy + -1 * cr * -sy );
			right[1] = ( -1 * sr * sp * sy + -1 * cr * cy );
			right[2] = -1 * sr * cp;
		}

		if ( up )
		{
			up[0] = ( cr * sp * cy + -sr * -sy );
			up[1] = ( cr * sp * sy + -sr *cy );
			up[2] = cr * cp;
		}
	}

	/**************************************************
	* Cmd_Argc
	*
	* Returns the number of command arguments that are available.
	**************************************************/

	int Cmd_Argc( void )
	{
		return *( int * ) ADDRESS_COMMAND_COUNT;
	}

	/**************************************************
	* Cmd_Argv
	* 
	* Returns the command argument which is specified.
	**************************************************/

	char *Cmd_Argv( int arg )
	{
		if (( unsigned int ) arg >= Cmd_Argc())
		{
			return "";
		}

		return *( char ** )( ADDRESS_COMMAND_ARG + ( arg * 0x4 ));
	}

	/**************************************************
	* Com_ConcatArgs
	* 
	* Connects all the input variables inside a long
	* but simple to use string. This is often used to
	* catch data such as a text message.
	**************************************************/

	char *Cmd_ConcatArgs( int start )
	{
		int			i, c, tlen;
		static char	line[MAX_STRING_CHARS];
		int			len;
		char		*arg;

		len = 0;
		c = Cmd_Argc();

		for ( i = start ; i < c ; i++ )
		{
			arg = Cmd_Argv( i );
			tlen = strlen( arg );

			if ( len + tlen >= MAX_STRING_CHARS - 1 )
			{
				break;
			}

			memcpy( line + len, arg, tlen );
			len += tlen;

			if ( i != c - 1 )
			{
				line[len] = ' ';
				len++;
			}
		}

		line[len] = 0;
		return line;
	}

	/**************************************************
	* Com_Error
	* 
	* Instantly drop the client with the provided 
	* string, the menu re-appears and the client module
	* is unloaded.
	**************************************************/

	__declspec( naked ) void Com_Error( int level, const char *error, ... )
	{
		__asm
		{
			mov		eax, 0x437290
			jmp		eax
		}
	}

	/**************************************************
	* Com_Printf
	* 
	* Prints a simple message to the console, accepts
	* optional arguments for formatting etc.
	**************************************************/

	__declspec( naked ) void Com_Printf( const char *msg, ... )
	{
		__asm
		{
			mov		eax, 0x437080
			jmp		eax
		}
	}

	/**************************************************
	* Com_Sprintf
	* 
	* Safely copy over bytes to the destination buffer,
	* with the actual size and the format to be placed.
	**************************************************/

	void Com_Sprintf( char *dest, int size, const char *fmt, ... )
	{
		int			len;
		va_list		argptr;
		char		bigbuffer[32000];

		va_start( argptr, fmt );
		len = vsprintf_s( bigbuffer, sizeof( bigbuffer ), fmt, argptr);
		va_end (argptr);

		if ( len >= sizeof( bigbuffer ))
		{
			return;
		}

		if ( len >= size )
		{
			return;
		}

		Q_strncpyz( dest, bigbuffer, size );
	}

	/**************************************************
	* Com_StringEscape
	* 
	* Escapes a string from the in buffer to the out
	* buffer, altering newlines to become actual
	* new lines.
	**************************************************/

	void Com_StringEscape( char *in, char *out )
	{
		char	ch, ch1	= 0;
		int		len		= 0;
		int		Size	= 1023;

		while ( 1 )
		{
			ch = *in++;
			ch1 = *in;

			if ( ch == '\\' && ch1 == 'n' )
			{
				in++;
				*out++ = '\n';
			}
			else
			{
				*out++ = ch;
			}

			if( len > Size - 1 ) 
			{
				break;
			}

			len++;
		}

		return;
	}

	/**************************************************
	* CopyString
	*
	* Copies the provided string into an allocated buffer.
	**************************************************/

	char *CopyString( const char *in )
	{
		int iLen = strlen( in );
		char *lpString = ( char * ) malloc( iLen + 1 );
		strcpy_s( lpString, iLen + 1, in );
		return lpString;
	}

	/**************************************************
	* FreeString
	*
	* Free's an earlier copied string, must be done here.
	**************************************************/

	void FreeString( char *lpString )
	{
		free( lpString );
	}

	/**************************************************
	* Info_RemoveKey
	*
	* Removes the provided key from the buffer.
	**************************************************/

	void Info_RemoveKey( char *s, const char *key ) 
	{
		char	*start;
		char	pkey[MAX_INFO_KEY];
		char	value[MAX_INFO_VALUE];
		char	*o;

		if ( strlen( s ) >= MAX_INFO_STRING )
		{
			return;
		}

		if ( strchr ( key, '\\' ))
		{
			return;
		}

		while( 1 )
		{
			start = s;

			if ( *s == '\\')
			{
				s++;
			}

			o = pkey;

			while ( *s != '\\' )
			{
				if (!*s)
				{
					return;
				}

				*o++ = *s++;
			}

			*o = 0;
			s++;
			o = value;

			while ( *s != '\\' && *s )
			{
				if ( !*s )
				{
					return;
				}

				*o++ = *s++;
			}

			*o = 0;

			if ( !strcmp( key, pkey ))
			{
				strcpy( start, s );
				return;
			}

			if ( !*s )
			{
				return;
			}
		}
	}

	/**************************************************
	* Info_SetValueForKey
	*
	* Sets the provided key in the buffer with the new value.
	**************************************************/

	void Info_SetValueForKey( char *s, const char *key, const char *value )
	{
		char newi[MAX_INFO_STRING];

		if ( strlen( s ) >= MAX_INFO_STRING )
		{
			return;
		}

		Info_RemoveKey (s, key);

		if ( !value || !strlen( value ))
		{
			return;
		}

		Com_Sprintf( newi, sizeof( newi ), "\\%s\\%s", key, value );

		if ( strlen(newi) + strlen( s ) > MAX_INFO_STRING)
		{
			return;
		}

		strcat( newi, s );
		strcpy( s, newi );
	}

	/**************************************************
	* Info_ValueForKey
	*
	* Gets the provided key from the buffer.
	**************************************************/

	char *Info_ValueForKey( const char *s, const char *key ) 
	{
		char	pkey[BIG_INFO_KEY];
		static	char value[2][BIG_INFO_VALUE];

		static	int	valueindex = 0;
		char	*o;
		
		if ( !s || !key ) 
		{
			return "";
		}

		if ( strlen( s ) >= BIG_INFO_STRING ) 
		{
			return "";
		}

		valueindex ^= 1;

		if ( *s == '\\' )
		{
			s++;
		}

		while ( 1 )
		{
			o = pkey;

			while ( *s != '\\' )
			{
				if ( !*s )
				{
					return "";
				}

				*o++ = *s++;
			}

			*o = 0;
			s++;

			o = value[valueindex];

			while (*s != '\\' && *s)
			{
				*o++ = *s++;
			}

			*o = 0;

			if (!Q_stricmp (key, pkey) )
			{
				return value[valueindex];
			}

			if (!*s)
			{
				break;
			}

			s++;
		}

		return "";
	}

	/**************************************************
	* Q_CleanStr
	* 
	* Removes color codes and such nonsense.
	**************************************************/

	char *Q_CleanStr( char *string )
	{
		char*	d;
		char*	s;
		int		c;

		s = string;
		d = string;

		while (( c = *s ) != 0 )
		{
			if ( Q_IsColorString( s ))
			{
				s++;
			}		
			else if ( c >= 0x20 && c <= 0x7E )
			{
				*d++ = c;
			}

			s++;
		}

		*d = '\0';

		return string;
	}

	/**************************************************
	* Q_stricmpn
	* 
	* OS-independant function to compare strings to
	* each other. You can also compare the beginning of
	* any string with this function.
	**************************************************/

	int Q_stricmpn( const char *s1, const char *s2, int n )
	{
		int		c1, c2;

		if ( s1 == NULL ) 
		{
			if ( s2 == NULL )
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else if ( s2 == NULL )
		{
			return 1;
		}
		
		do 
		{
			c1 = *s1++;
			c2 = *s2++;

			if ( !n-- )
			{
				return 0;
			}
			
			if ( c1 != c2 )
			{
				if ( c1 >= 'a' && c1 <= 'z' ) 
				{
					c1 -= ( 'a' - 'A' );
				}

				if ( c2 >= 'a' && c2 <= 'z' ) 
				{
					c2 -= ('a' - 'A' );
				}

				if ( c1 != c2 ) 
				{
					return c1 < c2 ? -1 : 1;
				}
			}

		} while ( c1 );
		
		return 0;
	}

	/**************************************************
	* Q_stricmpn
	* 
	* A simple wrapper for the Q_stricmpn function which
	* will basically compare everything inside the
	* passed strings rather then the number of it.
	**************************************************/

	int Q_stricmp( const char *s1, const char *s2 )
	{
		return ( s1 && s2 ) ? Q_stricmpn ( s1, s2, 99999 ) : -1;
	}

	/**************************************************
	* Q_strncpyz
	* 
	* Safely copy over bytes from one buffer to another,
	* could still very well overflow if the destination
	* size is too small!
	**************************************************/

	void Q_strncpyz( char *dest, const char *src, int destsize ) 
	{
		if ( !dest ) 
		{
			return;
		}

		if ( !src ) 
		{
			return;
		}

		if ( destsize < 1 ) 
		{
			return;
		}

		strncpy_s( dest, destsize, src, destsize-1 );
		dest[destsize - 1] = 0;
	}

	/**************************************************
	* va
	* 
	* A very useful function which holds a rather large
	* buffer string and accepts the format string with
	* any number of parameters. When done, returns the
	* formatted string.
	**************************************************/

	char *va( const char *format, ... )
	{
		va_list			 argptr;
		static char		 string[2][32000];
		static int		 index = 0;
		char			*buf;

		buf = string[index & 1];
		index++;

		va_start ( argptr, format );
		vsprintf_s ( buf, sizeof( string[0] ), format, argptr );
		va_end ( argptr );

		return buf;
	}

	/**************************************************
	* vectoangles
	* 
	* Converts the provided vector into the angles
	* derived from the vector. The first vector is
	* assumed to the the angle vector.
	**************************************************/

	void vectoangles( const vec3_t value1, vec3_t angles )
	{
		float	forward;
		float	yaw, pitch;
		
		if ( value1[1] == 0 && value1[0] == 0 )
		{
			yaw = 0;

			if ( value1[2] > 0 )
			{
				pitch = 90;
			}
			else
			{
				pitch = 270;
			}
		}
		else
		{
			if ( value1[0] )
			{
				yaw = ( atan2 ( value1[1], value1[0] ) * 180 / M_PI );
			}
			else if ( value1[1] > 0 )
			{
				yaw = 90;
			}
			else
			{
				yaw = 270;
			}

			if ( yaw < 0 )
			{
				yaw += 360;
			}

			forward = sqrt( value1[0]*value1[0] + value1[1]*value1[1] );
			pitch	= ( atan2(value1[2], forward) * 180 / M_PI );

			if ( pitch < 0 )
			{
				pitch += 360;
			}
		}

		angles[PITCH]	= -pitch;
		angles[YAW]		= yaw;
		angles[ROLL]	= 0;
	}

	/**************************************************
	* VectorNormalize
	*
	* Normalizes the provided vector.
	**************************************************/

	vec_t VectorNormalize( vec3_t v ) 
	{
		float	length, ilength;

		length = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
		length = sqrt (length);

		if ( length ) 
		{
			ilength	 = 1 / length;
			v[0]	*= ilength;
			v[1]	*= ilength;
			v[2]	*= ilength;
		}
			
		return length;
	}