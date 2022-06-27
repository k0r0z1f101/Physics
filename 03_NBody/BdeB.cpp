#include "BdeB.h"

#include <vector>

#include <cmath>

#include <Common/Kernel.h>
#include <Common/Consts.h>
#include <Common/Func.h>
#include <Common/Helpers.h>
#include "imgui_internal.h"

using namespace SK;

Vec2	IntegrateVerlet(Vec2 const vCurrentPos,
						Vec2 const vPreviousPos,
						Scalar const fDt,
						Scalar const fMass,
						Vec2 const vExternalForce)
{
	return Consts::t2 * vCurrentPos - vPreviousPos + (fDt * fDt) * (vExternalForce / fMass);
}

void	IntegrateSymplecticEuler(Vec2& vNewPos, Vec2& vNewSpeed, Vec2 const vPos, Vec2 const vSpeed, Vec2 const vAcc, Scalar const dt)
{
	vNewPos		= Vec2(Consts::t0, Consts::t0);
	vNewSpeed	= Vec2(Consts::t0, Consts::t0);
}

struct Body
{
	Vec2	vCurrentPos;
	Vec2	vPreviousPos;

	Vec2	vComputedPos;
	Vec2	vExternalForce;

	Scalar	fMass;

	UInt32	color;

	Bool	bStatic;

	void	Swap()
	{
		vPreviousPos	= vCurrentPos;
		vCurrentPos		= vComputedPos;
	}

	void	Step(Scalar const fDt)
	{
		vComputedPos	= IntegrateVerlet(vCurrentPos, vPreviousPos, fDt, fMass, !bStatic ? vExternalForce : Vec2(Consts::t0, Consts::t0));
		vExternalForce	= Vec2(Consts::t0, Consts::t0);
	}

	void	Collide(Scalar const fRadius, Scalar const fDt)
	{
		// Do something to modify vComputedPos

		Scalar const fD2 = glm::dot(vComputedPos, vComputedPos);
		Scalar const fR2 = fRadius * fRadius;
		if (fD2 > fR2)
		{
			Vec2 const vDelta = vComputedPos - vCurrentPos;
			Vec2 const vDir = glm::normalize(vComputedPos);
			Vec2 const vHit = vDir * fRadius;

			vCurrentPos		= vHit;
			vComputedPos	= vHit + glm::reflect(vDelta, -vDir);
		}
	}
};

f32 fDeltaTime = 0.0125f;
f32 fMass = 1.0f;
f32 fCollisionRadius = 2.0f;

f32 fPointScale = 1.0f;
Int32 iCount = 128;

std::vector<Body> aBodies;

void AddGalaxy(Vec2 const vCenter, Scalar const fGalaxyRadius, Scalar const fCentralCoef, UInt32 const uColor, Bool const bIsCenterFixed)
{
	PlatformWord const uCount = static_cast<PlatformWord>(iCount);
	Scalar const fCentralMass = static_cast<Scalar>(uCount) * fCentralCoef;

	for (PlatformWord i = 0; i < uCount; ++i)
	{
		Body newBody;

		newBody.vCurrentPos		= vCenter + RandomPointOnDisk(fGalaxyRadius * Consts::t9_10);

		Scalar const fSpeed		= Random(Consts::t0, Consts::t1_10000 * fGalaxyRadius);
		Vec2 const vDir			= fDeltaTime * fSpeed * RandomPointOnCircleRaw();
		newBody.vPreviousPos	= newBody.vCurrentPos + vDir;

		Scalar const r = glm::length(vCenter - newBody.vCurrentPos);
		Vec2 const vToCenter = glm::normalize(vCenter - newBody.vCurrentPos);
		Vec2 const vTangent = Vec2(-vToCenter.y, vToCenter.x);
		Scalar const fOrbitSpeed = 0.99f * Func::Sqrt(Consts::tGravitational * fCentralMass / r);
		//Scalar const fOrbitSpeed = Func::Sqrt(Consts::tGravitational * fCentralMass *(Consts::t2 / r - Consts::t1 / r));
		newBody.vPreviousPos = newBody.vCurrentPos - fDeltaTime * vTangent * fOrbitSpeed;
		//newBody.vPreviousPos = newBody.vCurrentPos;

		newBody.vComputedPos	= Vec2(Consts::t0, Consts::t0);
		newBody.vExternalForce	= Vec2(Consts::t0, Consts::t0);

		newBody.fMass			= Random(Consts::t0, Consts::t5);

		newBody.color			= uColor;

		newBody.bStatic			= bIsCenterFixed;

		aBodies.emplace_back(newBody);
	}

	if (true)
	{
		Body newBody;

		newBody.vCurrentPos		= vCenter;
		newBody.vPreviousPos	= newBody.vCurrentPos;
		newBody.vComputedPos	= Vec2(Consts::t0, Consts::t0);
		newBody.vExternalForce	= Vec2(Consts::t0, Consts::t0);
		newBody.fMass			= fCentralMass;
		newBody.bStatic			= bIsCenterFixed;

		aBodies.emplace_back(newBody);
	}
}

void Initialize()
{
	PlatformWord const uCount = static_cast<PlatformWord>(iCount);

	aBodies.clear();
	aBodies.reserve(uCount);

	// TODO: Create uCount Bodies
	AddGalaxy(Vec2(0.0, 0.0), fCollisionRadius, Consts::t2048, IM_COL32(255, 128, 0, 255), true);
//	AddGalaxy(Vec2(3.0, 4.0), Consts::t2 * fCollisionRadius, Consts::t1024, IM_COL32(0, 255, 0, 255), false);
//	AddGalaxy(Vec2(-5.0, 5.0), Consts::t1_2 * fCollisionRadius, Consts::t1024, IM_COL32(255, 0, 0, 255), false);
}

void DrawUI()
{
	ImGui::DragFloat(u8"\u0394t", &fDeltaTime, 0.01f, 1e-3f, 2.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Mass", &fMass, 0.01f, 0.0f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::DragInt("Bodies Count", &iCount, 1, 16, 1 << 16, "%d", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Point Scale", &fPointScale, 0.01f, 1e-3f, 2.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Collision Radius", &fCollisionRadius, 0.1f, 1.0f, 32.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	if (ImGui::Button("Start/Reset"))
	{
		Initialize();
	}
}

void DrawScene(ImDrawList* pDrawList, f32 const dt)
{
	if (!aBodies.empty())
	{
		for (Body const& oBody : aBodies)
		{
			f32 const fCoef = Func::Min(oBody.fMass, 5.0f);
			//ImU32 const uCol = IM_COL32(255, 128, 0, 255);
			ImU32 const uCol = oBody.color;
			pDrawList->AddCircleFilled(VecToImDraw(oBody.vCurrentPos), fCoef * fPointScale, uCol, 8);
		}
		pDrawList->AddCircle(ImVec2(0.0f, 0.0f), fCollisionRadius * g_fDrawScale, IM_COL32(255, 0, 0, 255));
	}
}

void Update(f32 const dt)
{
	// Simulation Code Here
	{
		Int32 const iSize = static_cast<Int32>(aBodies.size());
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			Body& oBodyCur = aBodies[iIdx];
			for (Body& oBodyOther : aBodies)
			{
				if (&oBodyCur != &oBodyOther)
				{
					// F = G*m1*m2/d²
					// Sum all external forces
					Vec2 const vDelta = oBodyOther.vCurrentPos - oBodyCur.vCurrentPos;
					Scalar const fDistance2 = glm::dot(vDelta, vDelta);
					Scalar const fDistance = Func::Sqrt(fDistance2);
					Vec2 const vDir = vDelta / fDistance;
					oBodyCur.vExternalForce += vDir * (Consts::tGravitational * oBodyCur.fMass * oBodyOther.fMass / fDistance2);
				}
			}
			oBodyCur.Step(fDeltaTime);
		}
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			Body& oBodyCur = aBodies[iIdx];
			oBodyCur.Collide(fCollisionRadius, fDeltaTime);
		}
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			Body& oBodyCur = aBodies[iIdx];
			oBodyCur.Swap();
		}
	}
}
