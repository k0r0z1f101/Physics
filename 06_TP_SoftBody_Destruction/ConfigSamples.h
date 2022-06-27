#pragma once

#include "Common/Kernel.h"

struct Config
{
	f32 fDeltaTime = 0.0025f;
	f32 fTotalMass = 10.0f;
	f32 fGravity = -1.0f;

	f32 fInitAngle = 45.0f;
	f32 fInitSpinSpeed = 1.0f;

	f32 fPointScale = 1.0f;
	f32 fWidth = 1.0f;
	f32 fHeight = 4.0f;
	Int32 iCountWidth = 8;
	Int32 iCountHeight = 32;

	f32 fSpringStrength = 10.0f;
	f32 fStartHeight = 2.0f;
	f32 fSpringDepth = 2.83f; // 2*sqrt(2)
	f32 fMainSpringDepth = 0.4f;

	f32 fGroundFriction = 10.0f;
	f32 fSpringDamping = 0.5f;

	f32 fCollisionElasticity = Consts::t1;

	Vec2 vColliderPosition = Vec2(Consts::t0, Consts::t2);
	Vec2 vColliderPreviousPosition = vColliderPosition;
	Vec2 vColliderExternalForce = Vec2(Consts::t0, Consts::t0);
	f32 fColliderRadius = 0.32f;
	f32 fColliderMass = 1.0f;

	f32 fSpringPlastic = 0.01f;
	//f32 fSpringFracture = 0.125f;
	f32 fSpringFracture = 0.05f;
	Bool bSpinAtStart = false;
	Bool bColliderPlay = false;
	Bool bShowPlasticity = true;
	Bool bShowMainSpringOnly = true;

	SpringType eSpringType = SpringType::Stretch;
};

enum class SelectedSamples : UInt8
{
	Default		= 0,
	BrokenBar = 1,
	CompressBar,
	CompressStretchBar,
	SpinningBar,
	Tearing,
	SpinningCube,
	BouncyJelly,
	COUNT
};

SelectedSamples eSelectedSample;
Config oCurrent;

static std::vector<Config> gs_aConfigs;

void SetupConfigurations()
{
	gs_aConfigs.clear();
	// Default
	{
		Config oDefault;
		gs_aConfigs.push_back(oDefault);
	}
	// BrokenBar
	{
		Config oDefault;
		oDefault.fInitAngle = 90.0f;
		oDefault.fSpringFracture = 0.125f;
		gs_aConfigs.push_back(oDefault);
	}
	// CompressionBar
	{
		Config oDefault;
		oDefault.fInitAngle = 90.0f;
		oDefault.iCountWidth = 9;
		oDefault.iCountHeight = 33;
		oDefault.fSpringStrength = 30.0f;
		oDefault.fSpringDepth = 3.097f;
		oDefault.fSpringFracture = 0.125f;
		oDefault.fSpringPlastic = 0.05f;
		oDefault.fSpringFracture = 0.125f;
		oDefault.eSpringType = SpringType::Compressible;
		gs_aConfigs.push_back(oDefault);
	}
	// CompressStretchBar
	{
		Config oDefault;
		oDefault.fInitAngle = 90.0f;
		oDefault.iCountWidth = 9;
		oDefault.iCountHeight = 33;
		oDefault.fSpringStrength = 15.0f;
		oDefault.fSpringDepth = 3.097f;
		oDefault.fSpringFracture = 0.125f;
		oDefault.fSpringPlastic = 0.025f;
		oDefault.fSpringFracture = 0.1f;
		oDefault.eSpringType = SpringType::Both;
		gs_aConfigs.push_back(oDefault);
	}
	// SpinningBar
	{
		Config oDefault;
		oDefault.fSpringStrength = 20.0f;
		oDefault.fSpringDamping = 0.25f;
		oDefault.fInitAngle = 90.0f;
		oDefault.fInitSpinSpeed = 3.5f;
		oDefault.iCountWidth = 16;
		oDefault.iCountHeight = 64;
		oDefault.bSpinAtStart = true;
		oDefault.fSpringFracture = 0.125f;
		gs_aConfigs.push_back(oDefault);
	}
	// Tearing
	{
		Config oDefault;
		oDefault.fSpringStrength = 2.5f;
		oDefault.fSpringDamping = 0.25f;
		oDefault.fInitAngle = 90.0f;
		oDefault.fWidth = 2.0f;
		oDefault.fHeight = 8.0f;
		oDefault.iCountWidth = 16;
		oDefault.iCountHeight = 64;
		oDefault.fColliderRadius = 1.0f;
		oDefault.bSpinAtStart = false;
		oDefault.fSpringPlastic= 0.125f;
		oDefault.fSpringFracture = 0.25f;
		gs_aConfigs.push_back(oDefault);
	}
	// SpinningCube
	{
		Config oDefault;
		oDefault.fSpringStrength = 10.0f;
		oDefault.fSpringDamping = 0.25f;
		oDefault.fStartHeight = 3.00f;
		oDefault.fInitAngle = 0.0f;
		oDefault.fWidth = 2.0f;
		oDefault.fHeight = 2.0f;
		oDefault.iCountWidth = 33;
		oDefault.iCountHeight = 33;
		oDefault.bSpinAtStart = true;
		oDefault.fInitSpinSpeed = 10.0f;
		oDefault.fSpringPlastic = 0.01f;
		oDefault.fSpringFracture = 0.25f;
		oDefault.vColliderPosition = Vec2(-3.0f, Consts::t6);
		oDefault.fColliderRadius = 0.25f;
		gs_aConfigs.push_back(oDefault);
	}
	// BouncyJelly
	{
		Config oDefault;
		oDefault.fSpringStrength = 4.0f;
		oDefault.fWidth = 4.0f;
		oDefault.fHeight = 2.0f;
		oDefault.fStartHeight = 0.0f;
		oDefault.fInitAngle = 0.0f;
		oDefault.iCountWidth = 33;
		oDefault.iCountHeight = 17;
		oDefault.vColliderPosition = Vec2(Consts::t0, Consts::t4);
		oDefault.fColliderMass = 2.5f;
		oDefault.fSpringFracture = 2.0f;
		oDefault.bColliderPlay = true;
		gs_aConfigs.push_back(oDefault);
	}
}
