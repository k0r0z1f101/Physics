#pragma once

#include <Common/Kernel.h>
#include <Common/Consts.h>

struct World2D
{
	Scalar	fCurrentTime	= Consts::t0;	// s
	Vec2	vPosition;						// m

	Vec2	vLinearSpeed;					// m.s^-1
	Vec2	vLinearAcceleration;			// m.s^-2

	Scalar	fAngularSpeed;					// rad.s^-1
	Scalar	fAngularAcceleration;			// rad.s^-2
};

struct Planet2D : World2D
{
	Scalar	fGravity	=	static_cast< Scalar >( 9.81 ); // m.s^-2
};
