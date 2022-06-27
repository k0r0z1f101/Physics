
template < typename Type >
inline
Type	tFunc< Type >::Sqrt( Type const fScalar )
{
	return std::sqrt( fScalar );
}

template < typename Type >
template < typename SecondType >
inline
Type	tFunc< Type >::Pow( Type const fA, SecondType const fB )
{
	return std::pow( fA, fB );
}

template <>
inline
f32	tFunc< f32 >::Mod( f32 const fA, f32 const fB )
{
	return std::fmod( fA, fB );
}

template <>
inline
f64	tFunc< f64 >::Mod( f64 const fA, f64 const fB )
{
	return std::fmod( fA, fB );
}

template <>
inline
f80	tFunc< f80 >::Mod( f80 const fA, f80 const fB )
{
	return std::fmod( fA, fB );
}

// Integer
template < typename Type >
inline
Type	tFunc< Type >::Mod( Type const iA, Type const iB )
{
	return iA % iB;
}

// template < typename Type >
// inline
// typename eMV::Core::EnableIf< eMV::Core::IsFloatingPoint< Type >::Value, Type >::Type
// 		tFunc< Type >::Mod( Type f32ScalarA, Type f32ScalarB  )
// {
// 	return std::fmod( f32ScalarA, f32ScalarB );
// }
//
// template < typename Type >
// inline
// typename eMV::Core::EnableIf< eMV::Core::IsIntegral< Type >::Value, Type >::Type
// 		tFunc< Type >::Mod( Type f32ScalarA, Type f32ScalarB  )
// {
// 	return ( f32ScalarA%f32ScalarB );
// }

// Integer
// template < typename Type >
// inline
// Integer	tFunc< Type >::Mod( Integer iA, Integer iB )
// {
// 	return ( iA % iB );
// }

// Trigonometry
template < typename Type >
inline
Type tFunc< Type >::Cos( Type const fScalar )
{
	return std::cos( fScalar );
}

template < typename Type >
inline
Type tFunc< Type >::Sin( Type const fScalar )
{
	return std::sin( fScalar );
}

template < typename Type >
inline
Type tFunc< Type >::Tan( Type const fScalar )
{
	return std::tan( fScalar );
}

template < typename Type >
inline
Type tFunc< Type >::ArcCos( Type const fScalar )
{
	return std::acos( fScalar );
}

template < typename Type >
inline
Type tFunc< Type >::ArcSin( Type const fScalar )
{
	return std::asin( fScalar );
}

template < typename Type >
inline
Type tFunc< Type >::ArcTan( Type const fScalar )
{
	return std::atan( fScalar );
}

template < typename Type >
inline
Type tFunc< Type >::ArcTan2( Type const fScalarA, Type const fScalarB )
{
	return std::atan2( fScalarA, fScalarB );
}

// template < typename Type >
// inline
// typename eMV::Core::EnableIf< eMV::Core::IsSame< f32, Type >::Value, Type >::Type
// 		tFunc< Type >::Hypot( Type f32ScalarA, Type f32ScalarB  )
// {
// 	return std::hypotf( f32ScalarA, f32ScalarB );
// }
//
// template < typename Type >
// inline
// typename eMV::Core::EnableIf< eMV::Core::IsSame< f64, Type >::Value, Type >::Type
// 		tFunc< Type >::Hypot( Type f64ScalarA, Type f64ScalarB  )
// {
// 	return std::hypot( f64ScalarA, f64ScalarB );
// }

template <>
inline
f32		tFunc< f32 >::Hypot( f32 const fScalarA, f32 const fScalarB )
{
	return std::hypotf( fScalarA, fScalarB );
}

template <>
inline
f64		tFunc< f64 >::Hypot( f64 const fScalarA, f64 const fScalarB )
{
	return std::hypot( fScalarA, fScalarB );
}

// inline
// Type Hypot( Type fScalarA, Type fScalarB )
// {
// 	return std::hypot( fScalarA, fScalarB );
// }

// --------
template < typename Type >
inline
Type	tFunc< Type >::Round( Type const fScalar )
{
	//return static_cast< Type >( static_cast< Integer >( fScalar ) );
	return std::round( fScalar );
}

template < typename Type >
inline
Type	tFunc< Type >::Ceil( Type const fScalar )
{
	//return static_cast< Type >( static_cast< Integer >( fScalar ) );
	return std::ceil( fScalar );
}

template < typename Type >
inline
Type	tFunc< Type >::CeilAlign( Type const fScalar, Type const fAlign )
{
	SK_NOT_IMPL;
	return SKNaN< Type >;
}

#define DECL_CEIL_ALIGN( Type )													\
	template <>																	\
	__forceinline																\
	Type	tFunc< Type >::CeilAlign( Type const fScalar, Type const fAlign )	\
	{																			\
		return ( fScalar + ( fAlign - 1 ) ) & ~( fAlign - 1 );					\
	}																			

DECL_CEIL_ALIGN( UInt8 )
DECL_CEIL_ALIGN( UInt16 )
DECL_CEIL_ALIGN( UInt32 )
DECL_CEIL_ALIGN( UInt64 )
DECL_CEIL_ALIGN(  Int8 )
DECL_CEIL_ALIGN(  Int16 )
DECL_CEIL_ALIGN(  Int32 )
DECL_CEIL_ALIGN(  Int64 )

#undef DECL_CEIL_ALIGN

template < typename Type >
inline
Type	tFunc< Type >::Floor( Type const fScalar )
{
	//return static_cast< Type >( static_cast< Integer >( fScalar ) ) + tConsts< Type >::t1;
	return std::floor( fScalar );
}

template < typename Type >
inline
Type	tFunc< Type >::FloorAlign( Type const fScalar, Type const fAlign)
{
	SK_NOT_IMPL;
	return SKNaN< Type >;
}

#define DECL_FLOOR_ALIGN( Type )												\
	template <>																	\
	__forceinline																\
	Type	tFunc< Type >::FloorAlign( Type const fScalar, Type const fAlign )	\
	{																			\
		return fScalar & ~( fAlign - 1 );										\
	}																			

DECL_FLOOR_ALIGN( UInt8 )
DECL_FLOOR_ALIGN( UInt16 )
DECL_FLOOR_ALIGN( UInt32 )
DECL_FLOOR_ALIGN( UInt64 )
DECL_FLOOR_ALIGN(  Int8 )
DECL_FLOOR_ALIGN(  Int16 )
DECL_FLOOR_ALIGN(  Int32 )
DECL_FLOOR_ALIGN(  Int64 )

#undef DECL_FLOOR_ALIGN

template < typename Type >
inline
Type	tFunc< Type >::Exp( Type const fScalar )
{
	return std::exp( fScalar );
}

template < typename Type >
inline
Type	tFunc< Type >::Exp2( Type const fScalar )
{
#ifdef SK_COMPILER_ICPP
	return std::exp2( fScalar );
#else
	return tFunc< Type >::Pow( tConsts< Type >::t2, fScalar );
#endif
}

template < typename Type >
inline
Type	tFunc< Type >::Ln( Type const fScalar )
{
	return std::log( fScalar );
}

template < typename Type >
inline
Type	tFunc< Type >::Log( Type const fScalar )
{
	return std::log10( fScalar );
}

template < typename Type >
inline
Type	tFunc< Type >::Log2( Type const fScalar )
{
	//return std::log( fScalar )*tConsts< Type >::t1_Log2;
	//return std::log( fScalar )*tConsts< Type >::t1_Log2
	//return std::log( fScalar )/std::log( static_cast< Type >( 2. ) );
#if SK_COMPILER_ICPP
	return std::log2( fScalar );
#else
	return std::log( fScalar )*tConsts< Type >::t1_Log2;
#endif
	//std::log2( fScalar );
}

template < typename Type >
inline
Type	tFunc< Type >::LogN( Type const fScalar, Type const fBase )
{
	return std::log( fScalar )/std::log( fBase );
}

template < typename Type >
inline
Bool	tFunc< Type >::IsNaN( Type const fScalar )
{
	return fScalar != fScalar;
}

template < typename Type >
inline
Bool	tFunc< Type >::IsNumber( Type const fScalar )
{
	return fScalar == fScalar;
}

template < typename Type >
inline
Bool	tFunc< Type >::IsPowerOfTwo( Type const fScalar )
{
	SK_NOT_IMPL;

	return false;
}

template <>
inline
Bool	tFunc< UInt8 >::IsPowerOfTwo( UInt8 const x )
{
	return x > 0 && !( x & ( x - 1 ) );
}

template <>
inline
Bool	tFunc< UInt16 >::IsPowerOfTwo( UInt16 const x )
{
	return x > 0 && !( x & ( x - 1 ) );
}

template <>
inline
Bool	tFunc< UInt32 >::IsPowerOfTwo( UInt32 const x )
{
	return x > 0 && !( x & ( x - 1 ) );
}

template <>
inline
Bool	tFunc< UInt64 >::IsPowerOfTwo( UInt64 const x )
{
	return x > 0 && !( x & ( x - 1 ) );
}

template <>
inline
Bool	tFunc< Int8 >::IsPowerOfTwo( Int8 const x )
{
	return x > 0 && !( x & ( x - 1 ) );
}

template <>
inline
Bool	tFunc< Int16 >::IsPowerOfTwo( Int16 const x )
{
	return x > 0 && !( x & ( x - 1 ) );
}

template <>
inline
Bool	tFunc< Int32 >::IsPowerOfTwo( Int32 const x )
{
	return x > 0 && !( x & ( x - 1 ) );
}

template <>
inline
Bool	tFunc< Int64 >::IsPowerOfTwo( Int64 const x )
{
	return x > 0 && !( x & ( x - 1 ) );
}

template <>
inline
Bool	tFunc< f32 >::IsPowerOfTwo( f32 const /*uScalar*/ )
{
	return false;
}

template <>
inline
Bool	tFunc< f64 >::IsPowerOfTwo( f64 const /*uScalar*/ )
{
	return false;
}

template <>
inline
Bool	tFunc< f80 >::IsPowerOfTwo( f80 const /*uScalar*/ )
{
	return false;
}

template < typename Type >
inline
Type	tFunc< Type >::Abs( Type const a )
{
	//return std::abs( a );
	//https://twitter.com/zeuxcg/status/1044800466847068160
	return a < tConsts< Type >::t0 ? -a : a;
}

// Implement for f32 & f64? Test Perfs
template <>
inline
Int32	tFunc< Int32 >::Abs( Int32 const a )
{
	return a & 0x7FFFFFFF;
}

template <>
inline
Int64	tFunc< Int64 >::Abs( Int64 const a )
{
	return a & 0x7FFFFFFFFFFFFFFF;
}

template < typename Type >
inline
Type	tFunc< Type >::Min( Type const a, Type const b )
{
	return ( a < b ? a : b );
}

template < typename Type >
inline
Type	tFunc< Type >::Max( Type const a, Type const b )
{
	return ( a > b ? a : b );
}

template < typename Type >
inline
Type	tFunc< Type >::Lerp( Type const a, Type const b, Type const t )
{
	// Implementation for numerical stability
	// Not: a + ( b - a )*t
	return a*( tConsts< Type >::t1 - t ) + b*t;
}

template < typename Type >
inline
Type	tFunc< Type >::Clamp( Type const x, Type const min, Type const max )
{
	return ( x < min ? min : ( x > max ? max : x ) );
}

template < typename Type >
inline
Type	tFunc< Type >::Saturate( Type const x )
{
	return Clamp( x, tConsts< Type >::t0, tConsts< Type >::t1 );
}

template < typename Type >
inline
Type	tFunc< Type >::Normalize( Type const x, Type const min, Type const max )
{
	return ( x - min )/( max - min );
}

template < typename Type >
inline
Type	tFunc< Type >::ReScale(	Type const x,	Type const minBound,	Type const maxBound,
												Type const minNewBound,	Type const maxNewBound )
{
	return tFunc< Type >::Normalize( x, minBound, maxBound )*( maxNewBound - minNewBound ) + minNewBound;
}

template < typename Type >
inline
Type	tFunc< Type >::ReScaleSafe(	Type const x,	Type const minBound,	Type const maxBound,
													Type const minNewBound,	Type const maxNewBound )
{
	return tFunc< Type >::Normalize( tFunc< Type >::Clamp( x, minBound, maxBound ), minBound, maxBound )*( maxNewBound - minNewBound ) + minNewBound;
}


template < typename Type >
inline
Type		tFunc< Type >::Linear2sRGB( Type const x )
{
	if (x <= static_cast< Type >(0.00313066844250063))
	{
		return x*static_cast<Type>(12.92);
	}
	else
	{
		return tFunc<Type>::Pow(static_cast<Type>(1.055)*x, static_cast<Type>(1.0/2.4)) - static_cast<Type>(0.055);
	}
}

template < typename Type >
inline
Type		tFunc< Type >::sRGB2Linear( Type const x )
{
	if (x <= static_cast<Type>(0.0404482362771082))
	{
		return x / static_cast<Type>(12.92);
	}
	else
	{
		return tFunc<Type>::Pow((x + static_cast<Type>(1.055)) / static_cast<Type>(1.055), static_cast<Type>(2.4));
	}
}

template < typename Type >
inline
Type	tFunc< Type >::Polar( Type const x, Type const y )
{
	Type tTheta = tConsts< Type >::t0;

	if ( x >= tConsts< Type >::t0 )	// Quadrant I or IV
	{
		// If x = 0, then tFunc< Type >::ATanf(y/x) = +pi/2 if y > 0
		//                tFunc< Type >::ATanf(y/x) = -pi/2 if y < 0
		tTheta = tFunc< Type >::ArcTan( y/x ); // in [-pi/2, +pi/2]

		if ( tTheta < tConsts< Type >::t0 )
			tTheta += tConsts< Type >::tTau; // in [0, 2*pi[.
	}
	else
		tTheta = tFunc< Type >::ArcTan( y/x ) + tConsts< Type >::tPi; // in [0, 2*pi[.

	return tTheta;
}

//

template < typename Type >
inline
Type		tFunc< Type >::Sign( Type const x )
{
	return x >= tConsts< Type >::t0 ? tConsts< Type >::t1 : tConsts< Type >::t_1;
}

template < typename Type >
inline
Type		tFunc< Type >::HeavisideTheta( Type const x )
{
	return tFunc< Type >::Saturate( tFunc< Type >::Sign( x ) );
}

template < typename Type >
inline
Type		tFunc< Type >::OnlyPositive( Type const x )
{
	return x*tFunc< Type >::HeavisideTheta( x );
}

template < typename Type >
inline
Type		tFunc< Type >::OnlyNegative( Type const x )
{
	return x*tFunc< Type >::HeavisideTheta( -x );
}

template < typename Type >
inline
Type		tFunc< Type >::OnlyLower( Type const x, Type const bound )
{
	return x*tFunc< Type >::HeavisideTheta( x - bound );
}

template < typename Type >
inline
Type		tFunc< Type >::OnlyHigher( Type const x, Type const bound )
{
	return x*tFunc< Type >::HeavisideTheta( bound - x );
}

template < typename Type >
inline
Type		tFunc< Type >::OnlyBounded( Type const x, Type const min, Type const max )
{
	return HeavisideTheta( x - min )*HeavisideTheta( max - x )*x;
}
