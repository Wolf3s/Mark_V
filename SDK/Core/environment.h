/*
Copyright (C) 2011-2014 Baker

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// environment.h -- platform environment


#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

/*
** Special basic datatypes SHALL be prefixed with 'c'.
** Special basic functions/function-wrapping macros SHALL be prefixed with 'c_'
**
** datatype examples: cbool, crect_t, etc.
** function examples: c_strlcpy, c_snprint2, c_rint, c_min, c_max, etc.
*/

///////////////////////////////////////////////////////////////////////////////
// Platform identification: Create our own define for Mac OS X, etc.
///////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
	#include "TargetConditionals.h"
//	#define PLATFORM_FAMILY_APPLE // Nope.  We are going to be more explicit
	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
		// iOS device
		// iOS Simulator
//		#pragma message ("IPHONE DETECTED")
		# define PLATFORM_IOS
		# define PLATFORM_MOBILE			1
		# define PLATFORM_NAME				"Apple iOS"

		# define PLATFORM_SHORTNAME			"iOS"
		# define PLATFORM_ENUM				os_enum_iphone
		# define PLATFORM_GUI_IOS
		# define DISPATCH_BANDAGES_IOS
		# define PLATFORM_SCREEN_FLIPPED_Y	1				// Screen 0,0 is *bottom* left
		# define PLATFORM_SCREEN_PORTRAIT	1				// Mobile screen, iPhone, Android, etc., Android, IOS
		# define PLATFORM_IOS_BPP_32		32				// Not best place for this
	#elif TARGET_OS_MAC
		# define PLATFORM_OSX
		# define PLATFORM_MOBILE			0
		# define PLATFORM_NAME				"Mac OS X"

		# define PLATFORM_SHORTNAME			"Mac"
		# define PLATFORM_ENUM				os_enum_mac
		# define PLATFORM_GUI_OSX
		# define DISPATCH_BANDAGES_OSX
		# define PLATFORM_SCREEN_FLIPPED_Y	1				// Screen 0,0 is *bottom* left
		# define PLATFORM_SCREEN_PORTRAIT	0				// Not mobile screen
	#else
		// Unsupported platform
		#pragma message ("UNKNOWN APPLE PLATFORM")
	#endif

//	#define CORE_TIMESHARE // May
//#endif // __APPLE__

#elif defined (__linux__) || defined (__linux)
	#ifdef __ANDROID__
		# define PLATFORM_ANDROID
		# define PLATFORM_MOBILE			1
		# define PLATFORM_NAME				"Android"

		# define PLATFORM_SHORTNAME			"Android"
		# define PLATFORM_ENUM				os_enum_android
		# define PLATFORM_SCREEN_FLIPPED_Y	0				// Screen 0,0 is top left
		# define PLATFORM_SCREEN_PORTRAIT	3				// Mobile screen, Android, IOS - Portrait is 3 (270 instead of 90)		
	#else
		# define PLATFORM_LINUX
		# define PLATFORM_MOBILE				0
		# define PLATFORM_NAME				"Linux"

		# define PLATFORM_SHORTNAME			"Linux"
		# define PLATFORM_ENUM				os_enum_linux
		# define PLATFORM_SCREEN_FLIPPED_Y	0				// Screen 0,0 is top left
		# define PLATFORM_SCREEN_PORTRAIT	0				// Not mobile screen
	//	# pragma message ("Linux detected")
	#endif

#elif defined(_WIN32)
 	# define PLATFORM_WINDOWS
	# define PLATFORM_MOBILE				0
	# define PLATFORM_NAME					"Windows"
	
	# define PLATFORM_SHORTNAME				"Windows"
	# define PLATFORM_ENUM					os_enum_windows
	# define PLATFORM_GUI_WINDOWS
	# define DISPATCH_BANDAGES_WINDOWS
	# define PLATFORM_SCREEN_FLIPPED_Y		0				// Screen 0,0 is top left
	# define PLATFORM_SCREEN_PORTRAIT		0				// Not mobile screen

	# define FILESYSTEM_WINDOWS				// UNUSED?
#else
	#pragma error ("Platform unknown");
#endif // _WIN32

#ifndef PLATFORM_NAME
    #error Unknown platform
#endif

#if defined(CORE_SDL) || defined(_CONSOLE)
	// This overrides the graphics interface
	#undef PLATFORM_GUI_WINDOWS
		#undef DISPATCH_BANDAGES_WINDOWS
	#undef PLATFORM_GUI_OSX
		#undef DISPATCH_BANDAGES_OSX
	#undef PLATFORM_GUI_IOS
		#undef DISPATCH_BANDAGES_IOS



	#undef DISPATCH_BANDAGES_WIN32

	// Problem --- this lets an allocated console in Windows do those things.
//	#define CORE_NO_SHELL_GUI // No show file in explorer, no file dialogs, still can have clipboard
	#ifdef CORE_SDL
		#define PLATFORM_GUI_SDL		// Sort of redundant?
		#define DISPATCH_BANDAGES_SDL	// Marking my touchups that SDL2 doesn't deal with.  SDL2 is "SDL" at this point as far as I'm concerned.
	#endif				// 2000s SDL is obsolete and since Linux and other platforms are the focus, old SDL is useless and any apps using it are probably broken now.

	#ifdef _CONSOLE
		#define PLATFORM_GUI_CONSOLE_NONE
	#endif
#endif



#ifdef PLATFORM_IOS
	#define CORE_NO_SHELL_GUI // No show file in explorer, no file dialogs, still can have clipboard
#endif

#define CORE_NEEDS_STRRSTR
#define CORE_NEEDS_VSNPRINTF_SNPRINTF

#define c_alloca alloca

#if defined(DEBUG) && !defined(_DEBUG)
#define _DEBUG
#endif // DEBUG Consistency

#if defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif // DEBUG Consistency

#ifdef DEBUG
	#define IS_DEBUG_BUILD 1
#else
	#define IS_DEBUG_BUILD 0
#endif

#ifdef DIRECT3D8_WRAPPER // dx8 - Define DIRECT3D_WRAPPER_VERSION 8
	#define DIRECT3D_WRAPPER_VERSION 8
#endif

#ifdef DIRECT3D9_WRAPPER // dx9 - Define DIRECT3D_WRAPPER_VERSION
	#define DIRECT3D_WRAPPER_VERSION 9
#endif

#ifndef DIRECT3D_WRAPPER_VERSION
	#define DIRECT3D_WRAPPER_VERSION 0
#endif

///////////////////////////////////////////////////////////////////////////////
//  GCC compile attributes, not needed for CLANG and not supported in MSVC6
///////////////////////////////////////////////////////////////////////////////

//#if !defined(__GNUC__)
//	#define	__core_attribute__(x)
//#endif	/* __GNUC__ */

/* argument format attributes for function
 * pointers are supported for gcc >= 3.1
 */
//#pragma message ("Attribute check")
#if defined(__GNUC__) //&& (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 0))
	#define	__core_attribute__	__attribute__
	//#pragma message ("Attribute define")
#else
	#define	__core_attribute__(x)
#endif


#if (defined(PLATFORM_WINDOWS) || defined(PLATFORM_LINUX)) && defined (__GNUC__)
// Linux does not have BSD strlcpy which is not standard C
	#define CORE_NEEDS_STRLCPY_STRLCAT // Really?
	#define CORE_NEEDS_STRCASESTR
#endif

#ifdef __GNUC__
	#ifndef PLATFORM_LINUX
		#define CORE_NEEDS_STRNDUP // Mingw is pissing me off ..
	#endif // !PLATFORM_LINUX
	#define CORE_NEEDS_STRNLEN // Mingw is pissing me off ..
#endif // __GNUC__

#ifdef __GNUC__
	#define ENUM_FORCE_INT_GCC_(_shortname) __ ## _shortname ## _invalid = -1, // look like __borderstyle_invalid = -1,
#else
	#define ENUM_FORCE_INT_GCC_(_shortname) // Nada
#endif // __GNUC__


///////////////////////////////////////////////////////////////////////////////
//  Microsoft Visual Studio
///////////////////////////////////////////////////////////////////////////////


#ifdef PLATFORM_WINDOWS
    #define CORE_NEEDS_MEMMEM // Everyone?   mingw/gcc seems to need on Windows too.
#endif

#ifndef _MSC_VER
	#define CORE_NEEDS_MEMICMP	// Non-Microsoft compilers don't have this function
	#define c_atoi64 atoll // string to long long (int64_t).  Microsoft uses a different name, for our purposes we will use this name
	#include <stdint.h>
#endif // ! _MSC_VER


#ifdef _MSC_VER
	typedef __int64 int64_t;
	#define va_copy(dest, src) (dest = src) // Visual Studio doesn't need/use va_copy

	#define PRINTF_INT64 "%I64d" // This is stupid int64_t print .. how to do on non-Microsoft?


	#define c_atoi64 _atoi64

	#if defined(_WIN64)
		#define ssize_t	SSIZE_T
		#define PRINTF_INTPTR "%I64d" // This is stupid int64_t print .. how to do on non-Microsoft?
		#define PRINTF_UINTPTR "%I64u" // This is stupid int64_t print .. how to do on non-Microsoft?
	#else
		typedef int	ssize_t;
		#define PRINTF_INTPTR "%d" // This is stupid int64_t print .. how to do on non-Microsoft?
		#define PRINTF_UINTPTR "%u" // This is stupid int64_t print .. how to do on non-Microsoft?
	#endif // _WIN64

	#ifdef _WIN64 //
	   typedef signed __int64    intptr_t;
	   typedef unsigned __int64  uintptr_t;
		//#define CORE_64_BIT 1
		#define CORE_64_BIT
	#else // _WIN64
	   typedef signed int   intptr_t;
	   typedef unsigned int uintptr_t;
	   //#define CORE_64_BIT 0
	#endif // _WIN64


	#undef c_alloca
	#define c_alloca _alloca // Because we have a non-standard name here.


	#define CORE_NEEDS_MEMMEM

//	#define CORE_NEEDS_VSNPRINTF_SNPRINTF   // All need
//	#define CORE_NEEDS_STRRSTR				// All need
	#define CORE_NEEDS_STRLCPY_STRLCAT
	#define CORE_NEEDS_STRCASESTR
	#define CORE_NEEDS_STRPTIME
    #define CORE_NEEDS_STRNDUP

	#if _MSC_VER <= 1200
		#define __VISUAL_STUDIO_6__
		#define CORE_NEEDS_STRNLEN
    #else
        #define __VISUAL_STUDIO_MODERN__
	#endif // Visual Studio 6

	#if _MSC_VER < 1400
		#define INVALID_FILE_ATTRIBUTES ((DWORD)-1) // GetFileAttributes MSVC6
	#endif // _MSC_VER < 1400

	#if !defined(__cplusplus)
		#define inline __inline
	#endif	// not cpp

	// Visual Studio has different names for these functions

	#ifdef __VISUAL_STUDIO_6__
		//#define __func__ va(__FILE__ " %d", __LINE__)  // No Pretty function support
		#define __func__ "not available"

		#define strcasecmp stricmp
		#define strncasecmp strnicmp
	#else
		#define __func__ __FUNCTION__  // Pretty function support

		#ifndef strcasecmp
			#define strcasecmp _stricmp		// Largely for HTTPClientWrapper.h
		#endif
		#ifndef strncasecmp
			#define strncasecmp _strnicmp	// Largely for HTTPClientWrapper.h
		#endif
	#endif // ! __VISUAL_STUDIO_6__

	#ifndef __VISUAL_STUDIO_6__
		#define _CRT_SECURE_NO_WARNINGS // Get rid of error messages about secure functions
		#define POINTER_64 __ptr64 // VS2008+ include order involving DirectX SDK can cause this not to get defined
	#endif
	#pragma warning(disable :4244)     // MIPS
	#pragma warning(disable:4244) // 'argument'	 : conversion from 'type1' to 'type2', possible loss of data
	#pragma warning(disable:4305) // 'identifier': truncation from 'type1' to 'type2', in our case, truncation from 'double' to 'float' */
	#pragma warning(disable:4996) // VS2008+ do not like fopen, but fopen_s is not standard C so unusable here

//	#pragma warning(disable:4761) // Baker: vc6 integral size mismatch in argument; conversion supplied
//	#pragma warning(disable:4267) // conversion from 'size_t' to 'type', possible loss of data (/Wp64 warning)

#else
	// Detect 64-bit on other platforms here.
	// https://stackoverflow.com/questions/5272825/detecting-64bit-compile-in-c
	// Commented out until I can verify.

	//#include <stdint.h>
	//#if UINTPTR_MAX == 0xffffffff
	///* 32-bit */
	//#elif UINTPTR_MAX == 0xffffffffffffffff
	///* 64-bit */
	//#else
	///* wtf */
	//#endif

#endif // _MSC_VER

// -save-temps option in gcc is pre-processed file.  Didn't work as far as I could tell -E is what worked.
//#define __FUNC_COLON_SPACE__ __core_function__ ": " // You have to treat it as a variable for gcc :(  Doesn't behave like string literal.


#ifdef __clang__
	#pragma clang diagnostic ignored "-Wshorten-64-to-32" // size_t 64 bits to a 32 bit int (because we don't use these for offsets and we aren't handling giga-files or giga-memory)
//	#pragma clang diagnostic ignored "-Wincompatible-pointer-types" // This is generally not advised!!

//	#pragma clang diagnostic ignored "-Wsign-conversion" // unsigned char vs. char  Not needed, I use compiler option now.
	#pragma clang diagnostic ignored "-Wmissing-prototypes" // Yes, this is killing me.
	#pragma clang diagnostic ignored "-Winvalid-noreturn" // Core_printf.c returns with attribute noreturn
	//	#pragma clang diagnostic ignored "-Wambiguous-macro" // M_PI  No I had it defined twice
//	#pragma clang diagnostic ignored "-Wunused-const-variable" // Not looking for waldos right now
//	#pragma clang diagnostic ignored "-Wno-unused-parameter"
//	#pragma clang diagnostic ignored "-Wsign-compare"
//	#pragma clang diagnostic ignored "-Wno-pointer-sign" // Don't work, must command line ... Grrr.
//	#pragma clang diagnostic ignored "-Wunused-variable"
	// -fsigned-char command line option
	//Project->Build Settings->Language->Char is unsigned
//	#pragma clang diagnostic ignored "-Wconversion"
#endif // __clang__

#if defined(__clang__) && defined(_DEBUG)
	#pragma clang diagnostic ignored "-Wformat-nonliteral" // Fruitzy stuff.  I don't roll that way.
	#pragma clang diagnostic ignored "-Wunused-variable" // Don't shove these in my face except for release build
	#pragma clang diagnostic ignored "-Wincompatible-pointer-types" // I pound this to the ground :(
	#pragma clang diagnostic ignored "-Wunused-function"
	#pragma clang diagnostic ignored "-Wswitch"
	#pragma clang diagnostic ignored "-Wswitch-enum"
	#pragma clang diagnostic ignored "-Wint-to-pointer-cast"
	#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
#endif

#ifdef __GNUC__
	#pragma GCC diagnostic ignored "-Wmissing-field-initializers" // Not a fan of disabling this but messes with struct x mystruct = {0};
#endif // __GNUC__

#ifdef __OBJC__ // Apple
	#define CSTRING(_x) [NSString stringWithUTF8String:_x]
	#define TO_CSTRING(_x) [_x cStringUsingEncoding:NSASCIIStringEncoding]
#endif

///////////////////////////////////////////////////////////////////////////////
//  Data types and defines
///////////////////////////////////////////////////////////////////////////////

#if __STDC_VERSION__ >= 201112L
	/* C11 support */
#endif

#undef true
#undef false


typedef enum { ENUM_FORCE_INT_GCC_ (cbool) // To throw into enums to force fucking int like the C standards say enumerations are supposed to be :(
	false = 0,
	true = 1,
} cbool;
#define CBOOL_DEFINED

typedef unsigned char byte;

#ifndef PLATFORM_IOS
#ifndef NULL
#if defined(__cplusplus)
#define	NULL		0
#else
#define	NULL		((void *)0)
#endif
#endif
#endif // ! PLATFORM_IOS


///////////////////////////////////////////////////////////////////////////////
//  Simple/Base Math and Logic
///////////////////////////////////////////////////////////////////////////////

#include <math.h> // Ulp!

#ifndef M_PI
	#define M_PI 3.14159265358979323846	// matches value in gcc v2 math.h
#endif
#define IDX_NOT_FOUND_NEG1 -1

#define in_range( _lo, _v, _hi )					( (_lo) <= (_v) && (_v) <= (_hi) )
#define out_of_bounds( _lo, _v, _hi )				( (_v) < (_lo) || (_hi) < (_v) ) // It's back!  And bigger than ever!
#define range_length(_start,_end)					((_end) - (_start) + 1)
#define range_end(_start,_length)					((_start) + (_length) - 1)
#define in_range_beyond(_start, _v, _beyond )		( (_start) <= (_v) && (_v) < (_beyond) )

#define	CLAMP(_minval, x, _maxval)		\
	((x) < (_minval) ? (_minval) :		\
	 (x) > (_maxval) ? (_maxval) : (x))

// Returns if true if would clamp
#define WOULDCLAMP(_minval, x, _maxval)	\
	((x) < (_minval) ? true :			\
	 (x) > (_maxval) ? true : false)

#define SQUARED(x)	((x)*(x))

#undef min
#undef max

#define	c_min(a, b)	(((a) < (b)) ? (a) : (b))
#define	c_max(a, b)	(((a) > (b)) ? (a) : (b))
#define c_rint(x)	((x) > 0 ? (int)((x) + 0.5) : (int)((x) - 0.5))
#define c_sgn(x)	((x > 0) - (x < 0))

typedef void  (*voidfunc_t)			(void);
typedef cbool (*progress_fn_t )		(void *id, int oldamt, int newamt); // Allows boundary checks easier
typedef cbool (*boolfunc_t )		(void);

extern const char * const empty_string;  // Why?  A decent compiler would combine them anyway, but may allow us to specifically identify an unchanged string (?)

///////////////////////////////////////////////////////////////////////////////
//  Constants
///////////////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_WINDOWS
	#define SYSTEM_BINARY_EXTENSION ".exe"
	#define MAX_OSPATH 256   // Technically 260 +/-
#else // Non-Windows ..
	#define SYSTEM_BINARY_EXTENSION ""
	#define MAX_OSPATH 1024 // OS X can have some long pathnames
	// Linux is harder to predict and not really a compile-time constant
	// because multiple file systems are available.
	// If forced to pick one ... well ... Hmmmm.
	// "Linux has a maximum filename length of 255 characters for most filesystems
	// "(including EXT4), and a maximum path of 4096 characters."
	// Not really sure best platform neutral way of handling at moment.
#endif // !PLATFORM_WINDOWS

#define SYSTEM_STRING_SIZE_1024 1024
typedef void *sys_handle_t;								// Used often.

///////////////////////////////////////////////////////////////////////////////
//  Command Support
///////////////////////////////////////////////////////////////////////////////

// lparse_t - Line_Parse_Alloc and Line_Parse_Free
// MAX_CMD_256 is max string.  We could dynamically allocate but
// then we run into the harder limit of 80 args and how to handle that.
// And nothing needs 80 args.  For a general splitter, we should make a new
// function.

#define MAX_CMD_256 256
#define MAX_ARGS_80	80

// Lets see who complains ...

typedef enum { ENUM_FORCE_INT_GCC_ (VRESULT)
	VR_RT_CANT_RUN_EMPTY_BYTECODE_NEG4 = -4,		// Differs from uncompiled as we are trying to run it.  We shouldn't, would be VB6 coding mistake.
	VR_RT_STOPPED_BY_USER_NEG3 = -3,				// Possibly ... I may change my mind
	VR_RT_BREAK_NEG2 = -2,							// Possibly ... I may change my mind
	VR_CT_UNCOMPILED_NEG1 = -1,						// FOR EXTERN USE ONLY
	VR_OK_0 = 0,
	VR_ERROR_ANY_1 = 1,
	VR_VARTYPE_OVERFLOW = 2,							// Divide by zero?
	_VRESULT_MAXNUM
} VRESULT;

///////////////////////////////////////////////////////////////////////////////
//  Macros
///////////////////////////////////////////////////////////////////////////////

#define case_break						break; { } case							// MVP.  Used tons.  default: much come first in switch.
#define STRINGIFY(x)					#x										// MVP.  Used tons.

#define ARRAY_COUNT(_array)				(sizeof(_array) / sizeof(_array[0]))	// Used tons.
#define ARRAY_STRLEN(s)					(ARRAY_COUNT(s) - sizeof(s[0]))			// Indispensible.  Tags: STRING_LENGTH STRING_LEN STRLEN  (size - 1)
#define ARRAY_BOUNDS_FAIL(_n, _count)	((_n) < 0 || (_count) <= (_n))			// Not used.

#define OFFSET_ZERO_0 0															// Happens to be not used?

///////////////////////////////////////////////////////////////////////////////
//  Reply Simplification/Varadic Simplification
///////////////////////////////////////////////////////////////////////////////

#define VA_ARGSCOPY_START(_text)				\
	va_list		__arglist_source, argscopy;		\
	va_start (__arglist_source, _text);			\
	va_copy	 (argscopy, __arglist_source)

#define VA_ARGSCOPY_END							\
	va_end (argscopy);							\
	va_end (__arglist_source);

#define NOT_MISSING_ASSIGN(pvar,val)		if (pvar) (*(pvar)) = val		// Used much.  Simplifies a reply return
#define REQUIRED_ASSIGN(pvar,val)			(*(pvar)) = val					// Used much.  Simplifies a reply return
#define REQUIRED_INCREMENT(PVAR,AMOUNT)		(*(PVAR))+= AMOUNT				// Unused at this time.  Would simplify an accumulator.
#define REQUIRED_DECREMENT(PVAR,AMOUNT)		(*(PVAR))-= AMOUNT				// Unused at this time.  Would simplify an accumulator.


///////////////////////////////////////////////////////////////////////////////
//  Endangered Species Section - Some are slight used.
///////////////////////////////////////////////////////////////////////////////

typedef void  (*MShutdown)			(void *);

#define __OBJ_REQUIRED__			\
	const char * _cname;			\
	struct cobj_s * _parent;		\
	struct cobj_s * _child;			\
	MShutdown * Shutdown;


// We put this at the end of object initialization that way if it failed to initialize we null it
// We can also auto-register it
#define OBJ_REQUIRED_HOOKUP(_x)		\
	if (_x)							\
	{								\
		_x->_cname		= _tag;		\
		_x->Shutdown	= (MShutdown *)Shutdown; \
	}

#define TRUISM(_x) ((_x) || 1) // Result of expression is always true.  Unused.  Endangered.  Used for enhanced for-loop/if logic
#define SWITCH_CASE(_val, _cond) (_cond) ? (_val) :		// EVIL!  And so awesome.  Endangered because unclear to the casual read.
#define SWITCH_DEFAULT(_val) (_val)						// Endangered because unclear to the casual read.
#define block_start__ {									// Endangered.
#define __block_end }
#define XORY(x, y) ((x) ? (x) : (y))					// Endangered.  Unused.
#define BATCHSIZE_8 8									// Endangered.  Unused.
#define BYTE_POSITION(_dest, _n)			(((byte *)(_dest))[_n])			// Used just once.  Rename/simplify or clarify.




#endif // ! __ENVIRONMENT_H__




