#pragma once

// CONFIGURATION:BEGIN

#ifndef SK_SCALAR_BITS
#define	SK_SCALAR_BITS		32	// or 64
#endif
#define	SK_INTEGER_BITS		32	// or 64
#define SK_USE_QUIET_NAN		// or USE_SIGNAL_NAN

// CONFIGURATION:END

#if defined( _DEBUG ) || defined( NDEBUG ) || defined( _NDEBUG ) || defined( DEBUG ) || defined( D_WIN32 ) || defined( D_WIN64 )
//#ifdef DEBUG
#	undef	__SK_RELEASE__
#	define	__SK_DEBUG__	1
#else
#	undef	__SK_DEBUG__
#	define	__SK_RELEASE__	1
#endif

#ifdef __SK_DEBUG__
#	define	SK_DEBUG_ONLY( X )		X
#	define	SK_RELEASE_ONLY( X )	
#else
#	define	SK_DEBUG_ONLY( X )
#	define	SK_RELEASE_ONLY( X )	X
#endif

#ifdef	__SK_DEV_VERSION__
#	define	SK_DEV_ONLY( X )	X
#	define	SK_PROD_ONLY( X )	
#elif	defined( __SK_PROD_VERSION__ )
#	define	SK_DEV_ONLY( X )	
#	define	SK_PROD_ONLY( X )	X
#endif

#if defined( __clang__ )
#	define	SK_COMPILER_CLANG	1
#	define	SKForceInline		__forceinline
#elif defined ( __GNUC__ ) || defined( __MINGW64__ )
#	define	SK_COMPILER_GPP		1
#	define	SKForceInline		__attribute__((always_inline))
#elif defined( _MSC_BUILD ) || defined( _MSC_VER )
#	define	SK_COMPILER_VCPP	1
#	define	SKForceInline		__forceinline
#elif defined( __INTEL_COMPILER )
#	define	SK_COMPILER_ICPP	1
#	define	SKForceInline		__forceinline
#else
#	error Compiler Unknown
#endif

#if SK_COMPILER_VCPP == 1
// defined( _MSC_BUILD ) || defined( __INTEL_COMPILER )
#ifndef _CRT_SECURE_NO_WARNINGS
#	define	_CRT_SECURE_NO_WARNINGS
#endif
#	if defined( D_WIN32 ) || defined( _WIN32 ) || defined( _M_IX86 ) || defined( _M_IX86_FP )
#		undef __SK_WIN64__
#		ifndef __SK_WIN32__
#			define __SK_WIN32__	1
#			define __SK_X86__		1
#		endif
#		ifndef __SK_WIN__
#			define __SK_WIN__		1
#		endif
#	else
#		error only for Windows
#	endif

#	if defined( D_WIN64 ) || defined( _WIN64 ) || defined( _M_X64 ) || defined( _M_AMD64 ) || defined( __x86_64 ) || defined( _M_IA64 )
#		undef __SK_WIN32__
#		undef __SK_X86__
#		ifndef __SK_WIN64__
#			define __SK_WIN64__	1
#			define __SK_X64__		1
#		endif
#		ifndef __SK_WIN__
#			define __SK_WIN__		1
#		endif
#	endif
#elif SK_COMPILER_GPP == 1
//defined ( __GNUC__ ) || defined( __MINGW64__ )
#	if defined( i386 ) || defined( __i386 ) || defined( __i386__ ) || defined( _X86_ )
#		undef __SK_WIN64__
#		ifndef __SK_WIN32__
#			define __SK_WIN32__	1
#			define __SK_X86__		1
#		endif
#		ifndef __SK_WIN__
#			define __SK_WIN__		1
#		endif
#	endif

#	if defined( __x86_64 ) || defined( __x86_64__ ) || defined( __amd64 ) || defined( __amd64__ ) || defined( _M_AMD64 ) || defined( _M_X64 )
#		undef __SK_WIN32__
#		undef __SK_X86__
#		ifndef __SK_WIN64__
#			define __SK_WIN64__	1
#			define __SK_X64__		1
#		endif
#		ifndef __SK_WIN__
#			define __SK_WIN__		1
#		endif
#	endif
#elif SK_COMPILER_CLANG == 1
#	if defined( i386 ) || defined( __i386 ) || defined( __i386__ ) || defined( _M_IX86 ) || defined( _X86_ )
#		undef __SK_WIN64__
#		ifndef __SK_WIN32__
#			define __SK_WIN32__	1
#			define __SK_X86__		1
#		endif
#		ifndef __SK_WIN__
#			define __SK_WIN__		1
#		endif
#	endif

#	if defined( __x86_64 ) || defined( __x86_64__ ) || defined( __amd64 ) || defined( __amd64__ ) || defined( _M_AMD64 ) || defined( _M_X64 )
#		undef __SK_WIN32__
#		undef __SK_X86__
#		ifndef __SK_WIN64__
#			define __SK_WIN64__	1
#			define __SK_X64__		1
#		endif
#		ifndef __SK_WIN__
#			define __SK_WIN__		1
#		endif
#	endif
#else
#	error Compiler not yet tested, use VC++2019
#endif

#ifdef SK_COMPILER_VCPP
#	define SK_DISABLE_WARNING_START	__pragma( warning( push ) )
#	define SK_DISABLE_WARNING( X )	__pragma( warning( disable : X ) )
#	define SK_DISABLE_WARNING_END	__pragma( warning( pop ) )
#else
#	define SK_DISABLE_WARNING_START
#	define SK_DISABLE_WARNING( X )
#	define SK_DISABLE_WARNING_END
#endif

#define SKPrivateCopy( ClassName )							\
			private:										\
				ClassName( ClassName const& );				\
				ClassName& operator=( ClassName const& );	

#if defined SK_KERNEL_DISABLE_BREAK
#	define SK_BREAK_CPU()
#elif defined __SK_LINUX__
#	define SK_BREAK_CPU()	( *( ( char* )0 ) = 0 )
// TODO remplace with?
//#include <signal.h>
//#	define SK_BREAK_CPU()	raise(SIGTRAP) // SIGBREAK?
#elif defined __SK_WIN64__
#	define SK_BREAK_CPU()	__debugbreak()
#elif defined __SK_WIN32__
#	define SK_BREAK_CPU()	__asm { int 3 }
#else
#	define SK_BREAK_CPU()	( *( ( char* )0 ) = 0 )
#endif

#define SK_STATIC_ASSERT	static_assert

#define SK_NOT_IMPL	SK_BREAK_CPU()

//////////////////////////////////////////////////////////////////////////
// Math
#define	SK_INTRINSIC_NO		0
#define	SK_INTRINSIC_MMX		1
#define	SK_INTRINSIC_SSE_1		2
#define	SK_INTRINSIC_SSE_2		3
#define	SK_INTRINSIC_SSSE_3		4
#define	SK_INTRINSIC_SSE_3		5
#define	SK_INTRINSIC_SSE_4		6
#define	SK_INTRINSIC_SSE_4_1	7
#define	SK_INTRINSIC_SSE_4_2	8
#define	SK_INTRINSIC_AVX		9
#define	SK_INTRINSIC_AVX_2		10
#define	SK_INTRINSIC_AVX_512	11
#define	SK_INTRINSIC_MIC		12	// 512 bits Instrict
#define	SK_INTRINSIC_FMA_3		13	// 2014
#define	SK_INTRINSIC_FMA_4		14	//

//#define __SK_MMX__
//#define	__SK_SSE__
#define	__SK_SSE2__
//#define	__SK_SSE3__
//#define	__SK_SSSE3__
//#define	__SK_SSE4_1__
//#define	__SK_SSE4_2__
//#define	__SK_AVX__
//#define	__SK_AVX2__

#ifndef SK_INSTRUCTION_SET_VALUE
#	if defined ( __SK_AVX2__ )
#		define SK_INSTRUCTION_SET_VALUE	SK_INTRINSIC_AVX_2
#	elif defined ( __SK_AVX__ )
#		define SK_INSTRUCTION_SET_VALUE	SK_INTRINSIC_AVX
#	elif defined ( __SK_SSE4_2__ )
#		define SK_INSTRUCTION_SET_VALUE	SK_INTRINSIC_SSE_4_2
#	elif defined ( __SK_SSE4_1__ )
#		define SK_INSTRUCTION_SET_VALUE	SK_INTRINSIC_SSE_4_1
#	elif defined ( __SK_SSSE3__ )
#		define SK_INSTRUCTION_SET_VALUE	SK_INTRINSIC_SSSE_3
#	elif defined ( __SK_SSE3__ )
#		define SK_INSTRUCTION_SET_VALUE	SK_INTRINSIC_SSE_3
#	elif defined ( __SK_SSE2__ ) || defined ( __x86_64__ )
#		define SK_INSTRUCTION_SET_VALUE	SK_INTRINSIC_SSE_2
#	elif defined ( __SK_SSE__ )
#		define SK_INSTRUCTION_SET_VALUE	SK_INTRINSIC_SSE_1
#	elif defined ( _M_IX86_FP )	// Defined in MS compiler. 1: SSE, 2: SSE2
#		if _M_IX86_FP == 1
#			define	SK_INSTRUCTION_SET_VALUE	SK_INTRINSIC_SSE_1
#		else
#			define	SK_INSTRUCTION_SET_VALUE	SK_INTRINSIC_SSE_2
#		endif
#	else
#		define SK_INSTRUCTION_SET_VALUE	SK_INTRINSIC_NO
#	endif // instruction set defines
#endif // SK_INSTRUCTION_SET_VALUE

//#ifdef __MMX__
//// MMX Not supported on x64 use SSE
//SK_TODO("Implement Vectorialu8 With SSE")
//// MMX
////#	include <mmintrin.h>
////#	include <xmmintrin.h>
//#endif

// Include the appropriate header file for intrinsic functions
#if SK_INSTRUCTION_SET_VALUE >= SK_INTRINSIC_AVX_2	// AVX2 and later
#	ifdef __GNUC__
#		include <x86intrin.h>	// x86intrin.h includes header files for whatever instruction 
// sets are specified on the compiler command line, such as:
// xopintrin.h, fma4intrin.h
#	else
#		include <immintrin.h>	// MS version of immintrin.h covers AVX, AVX2 and FMA3
#	endif // __GNUC__
#elif SK_INSTRUCTION_SET_VALUE == SK_INTRINSIC_AVX
#	include <immintrin.h>				// AVX
#elif SK_INSTRUCTION_SET_VALUE == SK_INTRINSIC_SSE_4_2
#	include <nmmintrin.h>				// SSE4.2
#elif SK_INSTRUCTION_SET_VALUE == SK_INTRINSIC_SSE_4_1
#	include <smmintrin.h>				// SSE4.1
#elif SK_INSTRUCTION_SET_VALUE == SK_INTRINSIC_SSSE_3
#	include <tmmintrin.h>				// SSSE3
#elif SK_INSTRUCTION_SET_VALUE == SK_INTRINSIC_SSE_3
#	include <pmmintrin.h>				// SSE3
#elif SK_INSTRUCTION_SET_VALUE == SK_INTRINSIC_SSE_2
#	include <emmintrin.h>				// SSE2
#elif SK_INSTRUCTION_SET_VALUE == SK_INTRINSIC_SSE_1
#	include <xmmintrin.h>				// SSE
#endif // SK_INSTRUCTION_SET_VALUE

// AMD instruction sets
#if defined ( __XOP__ ) || defined ( __FMA4__ )
#	ifdef __GNUC__
#		include <x86intrin.h>		// AMD XOP (Gnu)
#	else
#		include <ammintrin.h>		// AMD XOP (Microsoft)
#	endif //  __GNUC__
#elif defined ( __SSE4A__ )	// AMD SSE4A
#	include <ammintrin.h>
#endif // __XOP__

// FMA3 instruction set
#ifdef __FMA__
#	include <fmaintrin.h>
#endif // __FMA__

// FMA4 instruction set
#ifdef __FMA4__
#	include <fma4intrin.h>	// must have both x86intrin.h and fma4intrin.h, don't know why
#endif // __FMA4__

#ifdef	__INTEL_COMPILER
#	define	_XM_VECTORCALL_	0
#endif

#define	SK_VECTORIAL_ALIGNMENT_SSE		16
#define	SK_VECTORIAL_ALIGNMENT_AVX		32
#define	SK_VECTORIAL_ALIGNMENT_AVX512	64

#if defined( __SK_AVX__ ) || defined( __SK_AVX2__ )
#	define	SK_VECTORIAL_ALIGNMENT			SK_VECTORIAL_ALIGNMENT_AVX
#elif defined( __SK_SSE__ ) || defined( __SK_SSE2__ ) ||defined( __SK_SSE3__ ) || defined( __SK_SSSE3__ ) ||defined( __SK_SSE4_1__ ) ||defined( __SK_SSE4_2__ )
#	define	SK_VECTORIAL_ALIGNMENT			SK_VECTORIAL_ALIGNMENT_SSE
#endif

#define	SK_GPU_ALIGNMENT				16

constexpr float g_fDrawScale = 100.0f;
