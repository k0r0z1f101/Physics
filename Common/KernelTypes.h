#pragma once

#include <cstdint>
#include <iomanip>

#define GLM_FORCE_INLINE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_CTOR_INIT

#if SK_INSTRUCTION_SET_VALUE==SK_INTRINSIC_SSE_1
#	define GLM_FORCE_SSE
#elif SK_INSTRUCTION_SET_VALUE==SK_INTRINSIC_SSE_2
#	define GLM_FORCE_SSE2
#elif SK_INSTRUCTION_SET_VALUE==SK_INTRINSIC_SSE_3
#	define GLM_FORCE_SSE3
#elif SK_INSTRUCTION_SET_VALUE==SK_INTRINSIC_SSSE_3
#	define GLM_FORCE_SSSE3
#elif SK_INSTRUCTION_SET_VALUE==SK_INTRINSIC_SSE_4_1
#	define GLM_FORCE_SSE41
#elif SK_INSTRUCTION_SET_VALUE==SK_INTRINSIC_SSE_4_2
#	define GLM_FORCE_SSE42
#elif SK_INSTRUCTION_SET_VALUE==SK_INTRINSIC_AVX
#	define GLM_FORCE_AVX
#elif SK_INSTRUCTION_SET_VALUE==SK_INTRINSIC_AVX2
#	define GLM_FORCE_AVX2
#endif

#include <glm/glm.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <glm/detail/type_quat.hpp>

namespace SK
{
	typedef	bool					Bool;

	typedef	unsigned char			UInt8;
	typedef	unsigned short			UInt16;
	typedef	unsigned int			UInt32;
	typedef	unsigned long long		UInt64;

	typedef	char					Int8;
	typedef	short					Int16;
	typedef	int						Int32;
	typedef	long long				Int64;

	typedef char					Char;
	typedef wchar_t					WChar;
	typedef char					Char;
	typedef char					Char8;
	typedef wchar_t					WChar;
	typedef char16_t				Char16;
	typedef char32_t				Char32;

	typedef	char					CharASCII;
	typedef	char					CharUTF8;
	typedef	char16_t				CharUTF16;
	typedef	char32_t				CharUTF32;
	typedef	wchar_t					CharWide;
	//	__uint128_t
	typedef	void*					Ptr;
	typedef	void const*				PtrConst;
	typedef	void const* const		PtrConst_Const;
	typedef	void* const				Ptr_Const;

	typedef	float					f32;
	typedef	double					f64;
	typedef	long double				f80;

#if SK_SCALAR_BITS == 32
	typedef	f32						Scalar;
#else
	typedef	f64						Scalar;
#endif

#if SK_INTEGER_BITS == 32
	typedef	Int32					Integer;
	typedef	UInt32					UInteger;
#else
	typedef	Int64					Integer;
	typedef	UInt64					UInteger;
#endif

#ifdef __SK_WIN32__
	typedef	UInt32					Address;
#elif defined( __SK_X64__ )
	typedef	UInt64					Address;
#endif

	typedef	decltype( nullptr )		NullPtrType;

#ifdef __SK_WIN32__
	typedef	UInt16					UPlatformHalfWord;
	typedef	Int16					PlatformHalfWord;
	typedef	UInt32					UPlatformWord;
	typedef	Int32					PlatformWord;
	typedef	UInt64					UPlatformDoubleWord;
	typedef	Int64					PlatformDoubleWord;
#elif defined( __SK_X64__ )
	typedef	UInt32					UPlatformHalfWord;
	typedef	Int32					PlatformHalfWord;
	typedef	UInt64					UPlatformWord;
	typedef	Int64					PlatformWord;
	// Add UInt128 & SInt128 As {S}PlateformDoubleWord
#endif

	typedef	SK::UInt64				MatrixIndex;

	// Vectors
	typedef	glm::tvec2< Int32 >		Vec2i;
	typedef	glm::tvec2< UInt32 >	Vec2u;
	typedef	glm::tvec2< f32 >		Vec2f;
	typedef	glm::tvec2< f64 >		Vec2d;
	typedef	glm::tvec2< Scalar >	Vec2;

	typedef	glm::tvec3< Int32 >		Vec3i;
	typedef	glm::tvec3< UInt32 >	Vec3u;
	typedef	glm::tvec3< f32 >		Vec3f;
	typedef	glm::tvec3< f64 >		Vec3d;
	typedef	glm::tvec3< Scalar >	Vec3;

	typedef	glm::tvec4< f32 >		Vec4f;
	typedef	glm::tvec4< f64 >		Vec4d;
	typedef	glm::tvec4< Scalar >	Vec4;

	typedef	glm::tvec3< float >		Vec3f;

	// Matrices
	typedef	glm::tmat2x2< Scalar >	Mat22;
	typedef	glm::tmat3x3< Scalar >	Mat33;
	typedef	glm::tmat4x4< Scalar >	Mat44;

	// Quaternion
	typedef	glm::qua< Scalar >		Quat;
}

#define	SK_DECLARE_TEMPLATE_TYPE( TemplateType, NewNameType )			\
				typedef	TemplateType< Scalar >		NewNameType;		\
				typedef	TemplateType< UInt8 >		NewNameType ## u8;	\
				typedef	TemplateType< UInt16 >		NewNameType ## u16;	\
				typedef	TemplateType< UInt32 >		NewNameType ## u32;	\
				typedef	TemplateType< UInt32 >		NewNameType ## u64;	\
				typedef	TemplateType< Int8 >		NewNameType ## i8;	\
				typedef	TemplateType< Int16 >		NewNameType ## i16;	\
				typedef	TemplateType< Int32 >		NewNameType ## i32;	\
				typedef	TemplateType< Int64 >		NewNameType ## i64;	\
				typedef	TemplateType< f32 >			NewNameType ## f32;	\
				typedef	TemplateType< f64 >			NewNameType ## f64;	\
				typedef	TemplateType< f80 >			NewNameType ## f80;	\
				typedef	TemplateType< f32 >			NewNameType ## f;	\
				typedef	TemplateType< f64 >			NewNameType ## d;	\
				typedef	TemplateType< Int32 >		NewNameType ## i;	\
				typedef	TemplateType< UInt32 >		NewNameType ## u;	\
				typedef	TemplateType< MatrixIndex >	NewNameType ## M	

#define	SK_ALIGNOF( X )	__declspec( align( ( X ) ) )
#define	SK_UNUSED( X )		( X );

#define	__SK_ASCII__	1
//#define	__SK_UNICODE__	1
//#define	__SK_UTF8__	1
//#define	__SK_UTF16__	1
//#define	__SK_UTF32__	1

#if		__SK_UTF8__
	typedef	SK::CharUTF8		skChar;
#elif	__SK_UTF16__
	typedef	SK::CharUTF16		skChar;
#elif	__SK_UTF32__
	typedef	SK::CharUTF32		skChar;
#elif	__SK_UNICODE__
	typedef	SK::CharWide		skChar;
#elif	__SK_ASCII__
	typedef	SK::CharASCII		skChar;
#endif

#define	SKASCIIStr( x )	x
#define	SKUTF8Str( x )	u8 ## x
#define	SKUTF16Str( x )	u ## x
#define	SKUTF32Str( x )	U ## x
#define	SKWideStr( x )	L ## x

#if		__SK_UTF8__
#	define	SK_T( x )	u8 ## x
#elif	__SK_UTF16__
#	define	SK_T( x )	u ## x
#elif	__SK_UTF32__
#	define	SK_T( x )	U ## x
#elif	__SK_UNICODE__
#	define	SK_T( x )	L ## x
#	define	_UNICODE				1
#	define	UNICODE					1
#elif	__SK_ASCII__
#	define	SK_T( x )	x
#endif

#define SK_kB(size) ( ( size )*1000 )
#define SK_MB(size) ( SK_kB( size )*1000 )
#define SK_GB(size) ( SK_MB( size )*1000 )
#define SK_TB(size) ( SK_GB( size )*1000 )
#define SK_PB(size) ( SK_TB( size )*1000 )

#define SK_kiB(size) ( ( size )*1024 )
#define SK_MiB(size) ( SK_kiB( size )*1024 )
#define SK_GiB(size) ( SK_MiB( size )*1024 )
#define SK_TiB(size) ( SK_GiB( size )*1024 )
#define SK_PiB(size) ( SK_TiB( size )*1024 )
