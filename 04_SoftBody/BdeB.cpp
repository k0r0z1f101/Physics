#include "BdeB.h"

#include <vector>

#include <cmath>

#include <Common/Kernel.h>
#include <Common/Consts.h>
#include <Common/Func.h>
#include <Common/Helpers.h>

using namespace SK;

Vec2	Integrate(Vec2 const yt, Vec2 const yt_dt, Scalar const dt, Scalar const mass, Vec2 const vExternalForce)
{
	return Consts::t2 * yt - yt_dt + (dt * dt / mass) * vExternalForce;
}

struct Spring;

struct Particle
{
	Vec2	vCurrentPos;
	Vec2	vPreviousPos;

	Vec2	vComputedPos;
	Vec2	vExternalForce;

	Scalar	fMass;

	Bool	bStatic;

	std::vector<Spring const*>	aLinkedSprings = {};

	Vec2	GetSpeed(Scalar const fDt)
	{
		return (vCurrentPos - vPreviousPos) / fDt;
	}

	void	Swap()
	{
		vPreviousPos	= vCurrentPos;
		vCurrentPos		= vComputedPos;
	}

	void	Step(Scalar const fDt)
	{
		vComputedPos = Integrate(vCurrentPos, vPreviousPos, fDt, fMass, !bStatic ? vExternalForce : Vec2(Consts::t0, Consts::t0));
		vExternalForce = Vec2(Consts::t0, Consts::t0);
	}
};

struct Spring
{
	Particle*	pA;
	Particle*	pB;
	Scalar		fRestLength;
	Scalar		fSpringStrength;
};

f32 fDeltaTime = 0.0025f;
f32 fTotalMass = 10.0f;
f32 fGravity = -1.0f;

f32 fPointScale = 1.0f;
f32 fWidth = 1.0f;
f32 fHeight = 4.0f;
Int32 iCountWidth = 8;
Int32 iCountHeight = 32;

f32 fSpringStrength = 10.0f;
f32 fStartHeight = 2.0f;
f32 fSpringDepth = 2.83f; // 2*sqrt(2)

f32 fGroundFriction = 1.01f;
f32 fSpringDamping = 0.5f;

Bool bPlay = true;

std::vector<Particle> aParticles;
std::vector<Spring> aSprings;

void	CreateBox(	std::vector<Particle>& aParticles, std::vector<Spring>& aSprings,
					Scalar const fTotalMass, Scalar const fSpringStrength,
					Scalar const fStartHeight,
					Scalar const fWidth, Scalar const fHeight,
					UInt32 const uWidthCount, UInt32 const uHeightCount,
					Scalar const fDeltaTime, Scalar const fSpringDepth)
{
	aParticles	.clear();
	aSprings	.clear();

	UInt32 const uParticlesCount = uWidthCount * uHeightCount;

	Scalar const dx			= fWidth / static_cast<Scalar>(uWidthCount);
	Scalar const dy			= fHeight / static_cast<Scalar>(uHeightCount);
	Scalar const dd			= fSpringDepth * fSpringDepth * (dx * dx + dy * dy);
	Scalar const xOffset	= (fWidth - dx)* Consts::t1_2;
	Scalar const fMass		= fTotalMass / static_cast<Scalar>(uParticlesCount);

	for (UInt32 j = 0; j < uHeightCount; ++j)
	{
		Scalar const y = static_cast<Scalar>(j) * dy + fStartHeight;
		for (UInt32 i = 0; i < uWidthCount; ++i)
		{
			Scalar const x = static_cast<Scalar>(i) * dx - xOffset;

			Particle p;
			p.vCurrentPos		= Vec2(x, y);
			p.vPreviousPos		= p.vCurrentPos;
			p.vComputedPos		= Vec2(Consts::t0, Consts::t0);
			p.vExternalForce	= Vec2(Consts::t0, Consts::t0);
			p.fMass				= fMass;
			p.bStatic			= false;

			aParticles.push_back(p);
		}
	}

	auto oItA = aParticles.begin();
	auto oEndIt = aParticles.end();
	for (auto oItA = aParticles.begin(); oItA != oEndIt; ++oItA)
	{
		Particle& oA = *oItA;
		for (auto oItB = oItA + 1; oItB != oEndIt; ++oItB)
		{
			Particle& oB = *oItB;

			Scalar const fSqDistance = LengthSqr(oA.vCurrentPos - oB.vCurrentPos);

			if (fSqDistance < dd)
			{
				Spring s;

				s.pA				= &oA;
				s.pB				= &oB;
				s.fRestLength		= Func::Sqrt(fSqDistance);
				s.fSpringStrength	= fSpringStrength;

				aSprings.push_back(s);
			}
		}
	}

	for (Spring const& s : aSprings)
	{
		s.pA->aLinkedSprings.push_back(&s);
		s.pB->aLinkedSprings.push_back(&s);
	}
}

void ResetSimulation(	std::vector<Particle>& aParticles, std::vector<Spring>& aSprings,
						Scalar const fTotalMass, Scalar const fSpringStrength,
						Scalar const fStartHeight,
						Scalar const fWidth, Scalar const fHeight,
						UInt32 const uWidthCount, UInt32 const uHeightCount,
						Scalar const fDeltaTime, Scalar const fSpringDepth)
{
#if 1
	CreateBox(	aParticles, aSprings,
				fTotalMass, fSpringStrength,
				fStartHeight,
				fWidth, fHeight,
				uWidthCount, uHeightCount,
				fDeltaTime, fSpringDepth);
#elif 0
#endif
}

void Initialize()
{}

void DrawUI()
{
	ImGui::DragFloat(u8"\u0394t", &fDeltaTime, 0.01f, 1e-3f, 2.0f, "%.5f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Gravity", &fGravity, 0.01f, -20.0f, 20.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::DragFloat("Total Mass", &fTotalMass, 0.01f, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Spring Strength", &fSpringStrength, 0.01f, 1e-3f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::DragFloat("Ground Friction", &fGroundFriction, 0.01f, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Spring Damping", &fSpringDamping, 0.01f, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::DragFloat("Start Height", &fStartHeight, 0.01f, 1.0f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Spring Depth", &fSpringDepth, 0.01f, 0.05f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::DragFloat("Width", &fWidth, 0.01f, 1.0f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Height", &fHeight, 0.01f, 1.0f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::DragInt("Width Particles", &iCountWidth, 3, 16, 1 << 16, "%d", ImGuiSliderFlags_Logarithmic);
	ImGui::DragInt("Height Particles", &iCountHeight, 3, 16, 1 << 16, "%d", ImGuiSliderFlags_Logarithmic);

	ImGui::DragFloat("Point Scale", &fPointScale, 0.01f, 1e-3f, 2.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	if (ImGui::Button("Reset"))
	{
		ResetSimulation(aParticles, aSprings,
			fTotalMass, fSpringStrength,
			fStartHeight,
			fWidth, fHeight,
			iCountWidth, iCountHeight,
			fDeltaTime,
			fSpringDepth);
		bPlay = true;
	}
	if (ImGui::Button("Play/Pause"))
	{
		bPlay = !bPlay;
	}
}

void DrawScene(ImDrawList* pDrawList, f32 const dt)
{
	if (!aParticles.empty())
	{
		for (Particle const& oParticle : aParticles)
			pDrawList->AddCircleFilled(VecToImDraw(oParticle.vCurrentPos), fPointScale * g_fDrawScale * 0.0125f, IM_COL32(255, 128, 0, 255), 8);
		for (Spring const& oSpring : aSprings)
			pDrawList->AddLine(VecToImDraw(oSpring.pA->vCurrentPos),
				VecToImDraw(oSpring.pB->vCurrentPos), IM_COL32(0, 255, 0, 255), fPointScale * g_fDrawScale * 0.00125f);
	}
}

void Update(f32 const dt)
{
	// Simulation Code Here
	if (bPlay)
	{
		Int32 const iSize = static_cast<Int32>(aParticles.size());
		Int32 const iSizeS = static_cast<Int32>(aSprings.size());
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			Particle& oParticleCur = aParticles[iIdx];
			auto const& aLinkedSprings = oParticleCur.aLinkedSprings;
			Vec2 vSpeed = oParticleCur.GetSpeed(fDeltaTime);
			for (Spring const* pSpring : aLinkedSprings)
			{
				Spring const& oSpring = *pSpring;
				Particle& oA = *oSpring.pA;
				Particle& oB = *oSpring.pB;
				Vec2 const vDelta = oB.vCurrentPos - oA.vCurrentPos;
				Scalar const fDelta = glm::length(vDelta) - oSpring.fRestLength;

				// F = -k * Dl

				pSpring->fSpringStrength;

				if (Func::Abs(fDelta) > Consts::t0)
				{
					//oParticleCur.vExternalForce += ;
				}
			}
		}
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			// Collision with ground at y == 0.0f
			Particle& oParticleCur = aParticles[iIdx];
			oParticleCur.vExternalForce += Vec2(Consts::t0, fGravity * oParticleCur.fMass);
			oParticleCur.Step(fDeltaTime);
			Vec2 vCurPos = oParticleCur.vCurrentPos;
			Vec2 vNewPos = oParticleCur.vComputedPos;
			if (vNewPos.y < 0.0f)
			{
#if 1
				Vec2 vHit;
				if (LineLineIntersect(vHit, vNewPos, vCurPos, Vec2(-Consts::t2048, Consts::t0), Vec2(Consts::t2048, Consts::t0)))
				{
					Vec2 const vSpeed		= vNewPos - vCurPos;
					Vec2 const vNewSpeed	= glm::reflect(vSpeed, Vec2(Consts::t0, Consts::t1));

					oParticleCur.vCurrentPos	=	vHit;
					oParticleCur.vComputedPos	=	vHit + vNewSpeed;
				}
#elif 0
				Vec2 const vDelta = vNewPos - vCurPos;

				Scalar const fH = Func::Abs(vNewPos.y);
				Scalar const fL = glm::length(vDelta);
				Scalar const fQ = fL * fH / Func::Abs(vDelta.y);
				Scalar const fW = Func::Sqrt(fQ * fQ - fH * fH);

				Vec2 const vHit(vNewPos.x - fW, 0.0f);

				oParticleCur.vCurrentPos	= vHit;
				oParticleCur.vComputedPos	= vHit + glm::normalize(Vec2(fW, fH)) * fL;
#else
				oParticleCur.vExternalForce.y = Consts::t0;
				oParticleCur.vCurrentPos.y = Consts::t0;
#endif
			}
		}
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			Particle& oParticleCur = aParticles[iIdx];
			oParticleCur.Swap();
		}
	}
}
