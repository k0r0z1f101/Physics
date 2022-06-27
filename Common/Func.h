#pragma once

#include <Common/Kernel.h>
#include <Common/Consts.h>

#include <cmath>

namespace SK
{
	template < typename Type >
	class tFunc
	{
	public:
		static	Type		Sqrt		( Type const fScalar );
		template < typename SecondType >
		static	Type		Pow			( Type const fA, SecondType const fB );
		static	Type		Mod			( Type const fA, Type const fB );

		// Trigonometry
		static	Type		Cos			( Type const fScalar );
		static	Type		Sin			( Type const fScalar );
		static	Type		Tan			( Type const fScalar );
		static	Type		ArcCos		( Type const fScalar );
		static	Type		ArcSin		( Type const fScalar );
		static	Type		ArcTan		( Type const fScalar );
		static	Type		ArcTan2		( Type const fScalarA, Type const fScalarB );
		static	Type		Hypot		( Type const fScalarA, Type const fScalarB );

		// Others
		static	Type		Round		( Type const fScalar );
		static	Type		Ceil		( Type const fScalar );
		static	Type		CeilAlign	( Type const fScalar, Type const fAlign ); // Only for Intergers & Align are PowerOf2
		static	Type		Floor		( Type const fScalar );
		static	Type		FloorAlign	( Type const fScalar, Type const fAlign ); // Only for Intergers & Align are PowerOf2
		static	Type		Exp			( Type const fScalar );
		static	Type		Exp2		( Type const fScalar );
		static	Type		Ln			( Type const fScalar );
		static	Type		Log			( Type const fScalar );
		static	Type		Log2		( Type const fScalar );
		static	Type		LogN		( Type const fScalar, Type const fBase );

		// Test
		static	Bool		IsNaN		( Type const fScalar );
		static	Bool		IsNumber	( Type const fScalar );
		static	Bool		IsPowerOfTwo( Type const fScalar );

		// Useful
		static	Type		Abs			( Type const a );
		static	Type		Min			( Type const a, Type const b );
		static	Type		Max			( Type const a, Type const b );
		static	Type		Lerp		( Type const a, Type const b, Type const t );
		static	Type		Clamp		( Type const x, Type const min, Type const max );
		static	Type		Saturate	( Type const x );
		static	Type		Normalize	( Type const x, Type const min, Type const max );
		static	Type		ReScale		( Type const x, Type const minBound, Type const maxBound, Type const minNewBound, Type const maxNewBound );
		static	Type		ReScaleSafe	( Type const x, Type const minBound, Type const maxBound, Type const minNewBound, Type const maxNewBound );

		static	Type		Linear2sRGB	(Type const x);
		static	Type		sRGB2Linear	(Type const x);

		static	Type		Polar		( Type const x, Type const y );

		//
		static	Type		Sign			( Type const x );
		static	Type		HeavisideTheta	( Type const x );
		static	Type		OnlyPositive	( Type const x );
		static	Type		OnlyNegative	( Type const x );
		static	Type		OnlyLower		( Type const x, Type const bound );
		static	Type		OnlyHigher		( Type const x, Type const bound );
		static	Type		OnlyBounded		( Type const x, Type const min, Type const max );
	};

	SK_DECLARE_TEMPLATE_TYPE( tFunc, Func );

	#include <Common/Func.hpp>
}
