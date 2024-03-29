/*
	q_stdinc.h
	includes the minimum necessary stdc headers,
	defines common and / or missing types.
	NOTE:	for net stuff use net_sys.h,
		for byte order use q_endian.h,
		for math stuff use mathlib.h.

	Copyright (C) 1996-1997  Id Software, Inc.
	Copyright (C) 2007-2011  O.Sezer <sezero@users.sourceforge.net>
	Copyright (C) 2009-2014 Baker and others

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:

		Free Software Foundation, Inc.
		51 Franklin St, Fifth Floor,
		Boston, MA  02110-1301  USA
*/
// q_stdinc.h

#ifndef __Q_STDINC_H__
#define __Q_STDINC_H__

#include <sys/types.h>
#include <stddef.h>
#include <limits.h>

/* NOTES on TYPE SIZES:
   Quake/Hexen II engine relied on 32 bit int type size
   with ILP32 (not LP32) model in mind.  We now support
   LP64 and LLP64, too. We expect:
   sizeof (char)	== 1
   sizeof (short)	== 2
   sizeof (int)		== 4
   sizeof (float)	== 4
   sizeof (long)	== 4 / 8
   sizeof (pointer *)	== 4 / 8
   For this, we need stdint.h (or inttypes.h)
   FIXME: On some platforms, only inttypes.h is available.
   FIXME: Properly replace certain short and int usage
	  with int16_t and int32_t.
 */
#if defined(_MSC_VER) && (_MSC_VER < 1600)
/* MS Visual Studio provides stdint.h only starting with
 * version 2010.  Even in VS2010, there is no inttypes.h.. */
#include <msinttypes/stdint.h>
#else
#include <stdint.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#if !defined(_WIN32)
#include <strings.h>	/* strcasecmp and strncasecmp	*/
#endif	/* ! PLATFORM_WINDOWS */

/*==========================================================================*/

#ifndef NULL
#if defined(__cplusplus)
#define	NULL		0
#else
#define	NULL		((void *)0)
#endif
#endif

#define	Q_MAXCHAR	((char)0x7f)
#define	Q_MAXSHORT	((short)0x7fff)
#define	Q_MAXINT	((int)0x7fffffff)
#define	Q_MAXLONG	((int)0x7fffffff)
#define	Q_MAXFLOAT	((int)0x7fffffff)

#define	Q_MINCHAR	((char)0x80)
#define	Q_MINSHORT	((short)0x8000)
#define	Q_MININT	((int)0x80000000)
#define	Q_MINLONG	((int)0x80000000)
#define	Q_MINFLOAT	((int)0x7fffffff)

// Baker: Moved to assertions.h

///* Make sure the types really have the right
// * sizes: These macros are from SDL headers.
// */
//#define	COMPILE_TIME_ASSERT(name, x)	\
//	typedef int dummy_ ## name[(x) * 2 - 1]
//
//COMPILE_TIME_ASSERT(char, sizeof(char) == 1);
//COMPILE_TIME_ASSERT(float, sizeof(float) == 4);
//COMPILE_TIME_ASSERT(long, sizeof(long) >= 4);
//COMPILE_TIME_ASSERT(int, sizeof(int) == 4);
//COMPILE_TIME_ASSERT(short, sizeof(short) == 2);
//
///* make sure enums are the size of ints for structure packing */
//typedef enum {
//	THE_DUMMY_VALUE
//} THE_DUMMY_ENUM;
//COMPILE_TIME_ASSERT(enum, sizeof(THE_DUMMY_ENUM) == sizeof(int));


/* Provide a substitute for offsetof() if we don't have one.
 * This variant works on most (but not *all*) systems...
 */
#ifndef offsetof
#define offsetof(t,m) ((size_t)&(((t *)0)->m))
#endif


/*==========================================================================*/

#ifndef __CORE_INCLUDED__
typedef unsigned char byte;
#endif // ! __CORE_INCLUDED__

#if defined(__cplusplus)
	/* some structures have qboolean members and the x86 asm code expect
	 * those members to be 4 bytes long. therefore, qboolean must be 32
	 * bits and it can NOT be binary compatible with the 8 bit C++ bool.  */
	typedef int	qasmbool_e;
	COMPILE_TIME_ASSERT(falsehood, (0 == false));
	COMPILE_TIME_ASSERT(truth, (1  == true));
#else
	#ifndef __CORE_INCLUDED__

		#undef true
		#undef false

		typedef enum { ENUM_FORCE_INT_GCC_ (qasmbool)
			false = 0,
			true  = 1
		} qasmbool_e;
	#else
		typedef cbool qasmbool_e;
	#endif // __CORE_INCLUDED__

	COMPILE_TIME_ASSERT (falsehood, ((1 != 1) == false ));
	COMPILE_TIME_ASSERT (truth,		((1 == 1) == true  ));
#endif
COMPILE_TIME_ASSERT (qasmbool_e, sizeof(qasmbool_e) == 4);

/*==========================================================================*/

/* math */
#ifndef __CORE_INCLUDED__
typedef float	vec_t;
typedef vec_t	vec3_t[3];
#endif // ! __CORE_INCLUDED__


typedef vec_t	rgba4_t[4]; // Baker: RGBA color
typedef vec_t	vec5_t[5];
typedef int	fixed4_t;
typedef int	fixed8_t;
typedef int	fixed16_t;
typedef struct
{
	union
	{
		vec3_t vec3;
		struct
		{
			float x;
			float y;
			float z;
		};
	};
} Point3D;


/*==========================================================================*/

#if defined(_MSC_VER)
#if defined(_WIN64)
#define ssize_t	SSIZE_T
#else
typedef int	ssize_t;
#endif // _WIN64
#endif // _MSC_VER

/* compatibility with M$ types */
#if !defined(_WIN32)
#define	PASCAL
#define	FAR
#define	APIENTRY
#endif	/* ! WINDOWS */

#if !defined(__GNUC__)
#define	__core_attribute__(x)
#endif // ! __GNUC__

/* argument format attributes for function
 * pointers are supported for gcc >= 3.1
 */

#if defined(_MSC_VER) && !defined(__cplusplus)
#define inline __inline
#endif // _MSC_VER

/*==========================================================================*/


#endif // ! __Q_STDINC_H__

