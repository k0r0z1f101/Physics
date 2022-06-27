//////////////////////////////////////////////////////////////////////////
// Nom:		Lacasse
// Prenom:	Alexis
//////////////////////////////////////////////////////////////////////////

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

ImU32	ColorScale(Scalar const t)
{
	Scalar const fHue = Func::Lerp(Consts::t0, Consts::t1_3, t);
	Vec3 const vHSVColor = Vec3(fHue, Consts::t1, Consts::t1);
	Vec3 const vRGB = glm::clamp(HSV2RGB(vHSVColor), Consts::t0, Consts::t1);

	return IM_COL32(static_cast<UInt32>(Func::Round(vRGB.x * 255.0f)),
					static_cast<UInt32>(Func::Round(vRGB.y * 255.0f)),
					static_cast<UInt32>(Func::Round(vRGB.z * 255.0f)),
					255);
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

	Vec2	GetSpeed(Scalar const fDt) const
	{
		return (vCurrentPos - vPreviousPos) / fDt;
	}

	Vec2	GetAcceleration(Scalar const fDt) const
	{
		Vec2 const vPreviousSpeed	= (vCurrentPos - vPreviousPos) / fDt;
		Vec2 const vCurrentSpeed	= (vComputedPos - vCurrentPos) / fDt;

		return (vCurrentSpeed - vPreviousSpeed) / fDt;
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

	Vec2	VirtualStep(Scalar const fDt) const
	{
		return Integrate(vCurrentPos, vPreviousPos, fDt, fMass, !bStatic ? vExternalForce : Vec2(Consts::t0, Consts::t0));
	}
};

struct Spring
{
	Particle*	pA;
	Particle*	pB;
	Scalar		fSpringStrength;
	Scalar		fStartLength;
	Scalar		fMaxLength;
	Scalar		fRestLength;
	Bool		bIsAlive;
	Bool		bIsMainSpring;

	Scalar	GetCurrentElongation() const
	{
		return glm::length(pA->vCurrentPos - pB->vCurrentPos);
	}
};

enum class SpringType : UInt8
{
	Compressible = 0,
	Stretch,
	Both,
	COUNT
};

//////////////////////////////////////////////////////////////////////////
/// Fonction retournant un nombre entre 0.0f et 1.0f, montrant l'etat du ressort
///
/// /!\ Important
///		Prendre en compte le mode si l'on est SpringType::Compressible, SpringType::Stretch ou SpringType::Both
///
///		Si 'bPlasticity' => Return 0.0f si le 'oSpring' est toujours elastique
///							return ]0.0f, 1.0f[ en fonction de sa longueur au repos par rapport a sa longueur de fracture
///							et 1.0f si 'oSrping' est fracture
///
///		Si '!bPlasticity' => Return 0.0f si 'oSpring' est de la longueur a sa creation
///							return ]0.0f, 1.0f[ en fonction de son elongation courante par rapport a sa longueur de fracture
///							et 1.0f si 'oSrping' est fracture
//////////////////////////////////////////////////////////////////////////
Scalar	GetCritical(Spring const& oSpring, SpringType const eSpringType, Bool const bPlasticity, Scalar const fracture, Scalar const plastic)
{
	//3. Implementer la fonction GetCritical

	Scalar colorCoef = 1.0f;
	if (bPlasticity)
	{
		switch (eSpringType)
		{
		case SpringType::Stretch:
			if (oSpring.fRestLength == oSpring.fStartLength)
			{
				colorCoef = 0.0f;
			}
			else if (oSpring.bIsAlive)
			{
				colorCoef = (oSpring.fRestLength - oSpring.fStartLength) / ((oSpring.fStartLength * fracture) - (oSpring.fStartLength * plastic));
				Scalar test = colorCoef;
			}
			break;
		case SpringType::Compressible:
			if (oSpring.fRestLength == oSpring.fStartLength)
			{
				colorCoef = 0.0f;
			}
			else if (oSpring.bIsAlive)
			{
				colorCoef = abs((oSpring.fStartLength - oSpring.fRestLength) / ((oSpring.fStartLength * fracture) - (oSpring.fStartLength * plastic)));
			}
			break;
		case SpringType::Both:
			if (oSpring.fRestLength == oSpring.fStartLength)
			{
				colorCoef = 0.0f;
			}
			else if (oSpring.bIsAlive && oSpring.fRestLength > oSpring.fStartLength)
			{
				colorCoef = (oSpring.fRestLength - oSpring.fStartLength) / ((oSpring.fStartLength * fracture) - (oSpring.fStartLength * plastic));
			}
			else if (oSpring.bIsAlive&& oSpring.fRestLength < oSpring.fStartLength)
			{
				colorCoef = abs((oSpring.fStartLength - oSpring.fRestLength) / ((oSpring.fStartLength * fracture) - (oSpring.fStartLength * plastic)));
			}
			break;
		}
	}
	else
	{
		switch (eSpringType)
		{
		case SpringType::Stretch:
			if (oSpring.fRestLength == oSpring.fStartLength)
			{
				colorCoef = 0.0f;
			}
			else if (oSpring.bIsAlive)
			{
				colorCoef = (oSpring.fRestLength - oSpring.fStartLength) / (oSpring.fStartLength * fracture);
			}
			break;
		case SpringType::Compressible:
			if (oSpring.fRestLength == oSpring.fStartLength)
			{
				colorCoef = 0.0f;
			}
			else if (oSpring.bIsAlive)
			{
				colorCoef = abs((oSpring.fStartLength - oSpring.fRestLength) / (oSpring.fStartLength * fracture));
			}
			break;
		case SpringType::Both:
			if (oSpring.fRestLength == oSpring.fStartLength)
			{
				colorCoef = 0.0f;
			}
			else if (oSpring.bIsAlive && oSpring.fRestLength > oSpring.fStartLength)
			{
				colorCoef = (oSpring.fRestLength - oSpring.fStartLength) / (oSpring.fStartLength * fracture);
			}
			else if (oSpring.bIsAlive&& oSpring.fRestLength < oSpring.fStartLength)
			{
				colorCoef = abs((oSpring.fStartLength - oSpring.fRestLength) / (oSpring.fStartLength * fracture));
			}
			break;
		}
	}

	return colorCoef > 1.0f ? 1.0f : colorCoef;
}

#include "ConfigSamples.h"

Bool bPlay = true;

std::vector<Particle> aParticles;
std::vector<Spring> aSprings;

void	CreateBox(	std::vector<Particle>& aParticles, std::vector<Spring>& aSprings,
					Scalar const fTotalMass, Scalar const fSpringStrength,
					Scalar const fStartHeight,
					Scalar const fWidth, Scalar const fHeight,
					UInt32 const uWidthCount, UInt32 const uHeightCount,
					Scalar const fDeltaTime, Scalar const fSpringDepth, Scalar const fMainSpringDepth,
					Scalar const fSpringPlastic, Scalar const fSpringFracture)
{
	UInt32 const uParticlesCount = uWidthCount * uHeightCount;

	Scalar const dx			= fWidth / static_cast<Scalar>(uWidthCount - 1);
	Scalar const dy			= fHeight / static_cast<Scalar>(uHeightCount - 1);
	Scalar const dd			= fSpringDepth * fSpringDepth * (dx * dx + dy * dy);
	Scalar const ddMain		= fMainSpringDepth * fSpringDepth * (dx * dx + dy * dy);
	Scalar const xOffset	= (fWidth - dx)* Consts::t1_2;
	Scalar const fMass		= fTotalMass / static_cast<Scalar>(uParticlesCount);

	Scalar const fAngle = oCurrent.fInitAngle * Consts::tDegToRad;
	Scalar const fCos0 = Func::Cos(fAngle);
	Scalar const fSin0 = Func::Sin(fAngle);
	Mat22 rot = {	fCos0, -fSin0,
					fSin0,  fCos0 };

	Vec2 const vCenter = Vec2(Consts::t0, fStartHeight + fHeight * Consts::t1_2);

	for (UInt32 j = 0; j < uHeightCount; ++j)
	{
		Scalar const y = static_cast<Scalar>(j) * dy + fStartHeight;
		for (UInt32 i = 0; i < uWidthCount; ++i)
		{
			Scalar const x = static_cast<Scalar>(i) * dx - xOffset;

			Particle p;
			Vec2 const vRelative = Vec2(x, y) - vCenter;
			p.vCurrentPos = rot * vRelative + vCenter;
			if (oCurrent.bSpinAtStart)
			{
				f32    const fAlpha = Func::ArcTan2(vRelative.y, vRelative.y);
				Scalar const fCosA = Func::Cos(fAlpha);
				Scalar const fSinA = Func::Sin(fAlpha);
				p.vPreviousPos = p.vCurrentPos - Vec2(-fSinA, fCosA) * fDeltaTime * oCurrent.fInitSpinSpeed * glm::length(p.vCurrentPos - vCenter);
			}
			else
			{
				p.vPreviousPos = p.vCurrentPos;
			}
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

		Scalar mainLength = {};
		for (auto oItB = oItA + 1; oItB != oEndIt; ++oItB)
		{
			Particle& oB = *oItB;

			Scalar const fSqDistance = LengthSqr(oA.vCurrentPos - oB.vCurrentPos);

			if (fSqDistance < dd)
			{
				Spring s;

				s.pA				= &oA;
				s.pB				= &oB;
				s.fSpringStrength	= fSpringStrength;

				s.fRestLength		= Func::Sqrt( fSqDistance );

				s.fStartLength = s.fRestLength;
				s.fMaxLength = s.fRestLength + s.fRestLength * oCurrent.fSpringFracture;

				s.bIsAlive = true;
				s.bIsMainSpring = fSqDistance < ddMain;

				//1. Modifier la force du ressort 'fSpringStrength' en fonction de la longueur a la création.
				Scalar sqrtdd = Func::Sqrt(dd);
				s.fSpringStrength *= sqrtdd / s.fRestLength;

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

void ResetSimulation()
{
	aParticles	.clear();
	aSprings	.clear();

#if 1
	CreateBox(	aParticles, aSprings,
				oCurrent.fTotalMass, oCurrent.fSpringStrength,
				oCurrent.fStartHeight,
				oCurrent.fWidth, oCurrent.fHeight,
				oCurrent.iCountWidth, oCurrent.iCountHeight,
				oCurrent.fDeltaTime, oCurrent.fSpringDepth, oCurrent.fMainSpringDepth,
				oCurrent.fSpringPlastic, oCurrent.fSpringFracture);
#elif 0
#endif
}

void Initialize()
{
	SetupConfigurations();
}

void DrawUI()
{
	ImGui::Text("Global");

	ImGui::DragFloat("Point Scale", &oCurrent.fPointScale, 0.01f, 1e-3f, 5.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat(u8"\u0394t", &oCurrent.fDeltaTime, 0.01f, 1e-3f, 2.0f, "%.5f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Gravity", &oCurrent.fGravity, 0.01f, -20.0f, 20.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Ground Friction", &oCurrent.fGroundFriction, 0.01f, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::NewLine();
	ImGui::Text("Object - Visual/Transform");
	ImGui::DragFloat("Width", &oCurrent.fWidth, 0.01f, 1.0f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Height", &oCurrent.fHeight, 0.01f, 1.0f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragInt("Width Particles", &oCurrent.iCountWidth, 3, 16, 1 << 16, "%d", ImGuiSliderFlags_Logarithmic);
	ImGui::DragInt("Height Particles", &oCurrent.iCountHeight, 3, 16, 1 << 16, "%d", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Start Height", &oCurrent.fStartHeight, 0.01f, 1.0f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Init Angle", &oCurrent.fInitAngle, 0.01f, -180.0f, 180.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Collision Elasticity", &oCurrent.fCollisionElasticity, 0.01f, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::NewLine();
	ImGui::Text("Object - Physics");

	ImGui::DragFloat("Total Mass", &oCurrent.fTotalMass, 0.01f, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::Checkbox("Spin At Start", &oCurrent.bSpinAtStart);
	ImGui::Indent();
	if (oCurrent.bSpinAtStart)
		ImGui::DragFloat("Angular Speed", &oCurrent.fInitSpinSpeed, 0.01f, -5.0f, 5.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::Unindent();

	ImGui::NewLine();
	ImGui::Text("Springs Infos");

	ImGui::DragFloat("Spring Strength", &oCurrent.fSpringStrength, 0.01f, 1e-3f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Spring Damping", &oCurrent.fSpringDamping, 0.01f, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::DragFloat("Spring Depth", &oCurrent.fSpringDepth, 0.01f, 0.05f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Main Spring Depth", &oCurrent.fMainSpringDepth, 0.01f, 0.05f, oCurrent.fSpringDepth - 1e-6f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::NewLine();
	ImGui::Text("Springs Deformation/Fracture");

	ImGui::DragFloat("Spring Plastic", &oCurrent.fSpringPlastic, 0.01f, 0.0f, oCurrent.fSpringFracture + 1e-3f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Spring Fracture", &oCurrent.fSpringFracture, 0.01f, oCurrent.fSpringPlastic + 1e-3f, 2.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::NewLine();
	ImGui::Text("Collider");
	Vec2 vLocalColliderPos(oCurrent.vColliderPosition.x, oCurrent.vColliderPosition.y);
	if (ImGui::DragFloat2("Collider Position", &vLocalColliderPos.x, 0.125f, -32.0f, 32.0f, "%.3f", ImGuiSliderFlags_Logarithmic))
	{
		oCurrent.vColliderPosition = vLocalColliderPos;
		oCurrent.vColliderPreviousPosition = oCurrent.vColliderPosition;
	}
	ImGui::DragFloat("Collider Radius", &oCurrent.fColliderRadius, 0.125f, -32.0f, 32.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Collider Mass", &oCurrent.fColliderMass, 0.125f, 0.1f, 32.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::Separator();

	ImGui::TextUnformatted("Physics Params");
	ImGui::Indent();
	ImGui::Separator();
	{
		char const* pItems[] = { "Compressible", "Stretch", "Both" };
		char const* pCurrentItem = pItems[static_cast<UInt8>(oCurrent.eSpringType)];

		if (ImGui::BeginCombo("##combo", pCurrentItem))
		{
			for (Int32 n = 0; n < IM_ARRAYSIZE(pItems); n++)
			{
				Bool bIsSelected = (pCurrentItem == pItems[n]);
				if (ImGui::Selectable(pItems[n], bIsSelected))
				{
					pCurrentItem = pItems[n];
					oCurrent.eSpringType = static_cast<SpringType>(n);
					if (bIsSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::Unindent();

	if (ImGui::Button("Reset"))
	{
		ResetSimulation();
		bPlay = false;
	}
	if (ImGui::Button("Play/Pause"))
	{
		bPlay = !bPlay;
	}
	ImGui::SameLine();
	ImGui::Text(bPlay ? " : Played." : " : Stopped.");

	if (ImGui::Button("Collision Play"))
	{
		oCurrent.bColliderPlay = !oCurrent.bColliderPlay;
	}
	ImGui::SameLine();
	ImGui::Text(oCurrent.bColliderPlay ? " : Played." : " : Stopped.");

	ImGui::Checkbox("Show Only Main Spring", &oCurrent.bShowMainSpringOnly);

	ImGui::Separator();
	{
		char const* pItems[] = { "Default", "BrokenBar", "CompressBar", "CompressStretchBar", "SpinningBar", "Tearing", "SpinningCube", "BouncyJelly"};
		char const* pCurrentItem = pItems[static_cast<UInt8>(eSelectedSample)];

		if (ImGui::BeginCombo("##comboSamples", pCurrentItem))
		{
			for (Int32 n = 0; n < IM_ARRAYSIZE(pItems); n++)
			{
				Bool bIsSelected = (pCurrentItem == pItems[n]);
				if (ImGui::Selectable(pItems[n], bIsSelected))
				{
					pCurrentItem = pItems[n];
					eSelectedSample = static_cast<SelectedSamples>(n);
					if (bIsSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Set"))
		{
			oCurrent = gs_aConfigs[static_cast<UInt32>(eSelectedSample)];
			ResetSimulation();
			bPlay = false;
		}
	}
	ImGui::Separator();

	ImGui::Separator();
	ImGui::Text("Bonus");

	ImGui::Checkbox("Color Plasticity", &oCurrent.bShowPlasticity);
}

void DrawScene(ImDrawList* pDrawList, f32 const dt)
{
	if (!aParticles.empty())
	{
		for (Particle const& oParticle : aParticles)
			pDrawList->AddCircleFilled(VecToImDraw(oParticle.vCurrentPos), oCurrent.fPointScale * g_fDrawScale * 0.0125f, IM_COL32(255, 128, 0, 255), 8);
		for (Spring const& oSpring : aSprings)
		{
			if (oSpring.bIsAlive)
			{
				ImU32 const uCol = ColorScale(Consts::t1 - GetCritical(oSpring, oCurrent.eSpringType, oCurrent.bShowPlasticity, oCurrent.fSpringFracture, oCurrent.fSpringPlastic));
				if (!oCurrent.bShowMainSpringOnly || (oCurrent.bShowMainSpringOnly && oSpring.bIsMainSpring))
				{
					pDrawList->AddLine(VecToImDraw(oSpring.pA->vCurrentPos),
						VecToImDraw(oSpring.pB->vCurrentPos), uCol, oCurrent.fPointScale * g_fDrawScale * 0.00125f);
				}
			}
		}
	}

	pDrawList->AddCircleFilled(VecToImDraw(oCurrent.vColliderPosition), oCurrent.fColliderRadius * g_fDrawScale, IM_COL32(255, 64, 64, 255));
}

Vec2	IntersectSegmentToGround( Vec2 const vA, Vec2 const vB )
{
#if 0
	Scalar const fDeltaX = Func::Abs( vA.x - vB.x );
	Scalar const fDeltaY = Func::Abs( vA.y - vB.y );
	Scalar const a = fDeltaY / fDeltaX;
	Scalar const b = vA.y - a * vA.x;

	Scalar const fHitX = ( a * vA.x - vA.y ) / a;

	return Vec2( fHitX, Consts::t0 );
#else
	Vec2 const vDelta = vA - vB;

	Scalar const fH = Func::Abs( vA.y );
	Scalar const fL = glm::length( vDelta );
	Scalar const fQ = fL * fH / Func::Abs( vDelta.y );
	Scalar const fW = Func::Sqrt( fQ * fQ - fH * fH );

	return Vec2( vA.x - fW, 0.0f );
#endif
}

void Update(f32 const dt)
{
	// Simulation Code Here
	if (bPlay)
	{
		Int32 const iSize = static_cast<Int32>(aParticles.size());
		Int32 const iSizeS = static_cast<Int32>(aSprings.size());
		// Add Gravity for Collider
		oCurrent.vColliderExternalForce = Vec2(Consts::t0, oCurrent.fGravity * oCurrent.fColliderMass);
		// Update elongation
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSizeS; ++iIdx)
		{
			Spring& oSpring = aSprings[iIdx];
			if (!oSpring.bIsAlive)
			{
				continue;
			}

			Particle& oA = *oSpring.pA;
			Particle& oB = *oSpring.pB;
			Vec2 const vDelta = oB.vCurrentPos - oA.vCurrentPos;
			Scalar const fSpringElongation = glm::length(vDelta);

			//2. Trouver un moyen d'implementer la destruction du Soft-Body.

			Scalar stretchPlasticLength = oSpring.fStartLength + (oSpring.fStartLength * oCurrent.fSpringPlastic);
			Scalar compressiblePlasticLength = oSpring.fStartLength - (oSpring.fStartLength * oCurrent.fSpringPlastic);
			Scalar stretchFractureLength = oSpring.fStartLength + (oSpring.fStartLength * oCurrent.fSpringFracture);
			Scalar compressibleFractureLength = oSpring.fStartLength - (oSpring.fStartLength * oCurrent.fSpringFracture);

			switch (oCurrent.eSpringType)
			{
			case SpringType::Stretch:
				if (fSpringElongation > stretchPlasticLength && fSpringElongation > oSpring.fRestLength)
				{
					if (fSpringElongation > oSpring.fRestLength + oSpring.fRestLength * oCurrent.fSpringPlastic)
					{
						oSpring.fRestLength += fSpringElongation - (oSpring.fRestLength + oSpring.fRestLength * oCurrent.fSpringPlastic) - (oSpring.fRestLength - oSpring.fStartLength);
						oSpring.fMaxLength -= fSpringElongation - (oSpring.fRestLength + oSpring.fRestLength * oCurrent.fSpringPlastic) - (oSpring.fRestLength - oSpring.fStartLength);
					}
					oSpring.bIsAlive = fSpringElongation > oSpring.fMaxLength ? false : oSpring.bIsAlive;
				}
				break;
			case SpringType::Compressible:
				if (fSpringElongation < compressiblePlasticLength && fSpringElongation < oSpring.fRestLength)
				{
					if (fSpringElongation < compressiblePlasticLength)
					{
						oSpring.fRestLength -= fSpringElongation - (compressiblePlasticLength - (oSpring.fRestLength - oSpring.fStartLength));
					}
					oSpring.bIsAlive = fSpringElongation < compressibleFractureLength ? false : oSpring.bIsAlive;
				}
				break;
			case SpringType::Both:
				if (fSpringElongation > stretchPlasticLength && fSpringElongation > oSpring.fRestLength)
				{
					if (fSpringElongation > oSpring.fRestLength + oSpring.fRestLength * oCurrent.fSpringPlastic)
					{
						oSpring.fRestLength += fSpringElongation - (oSpring.fRestLength + oSpring.fRestLength * oCurrent.fSpringPlastic) - (oSpring.fRestLength - oSpring.fStartLength);
						oSpring.fMaxLength -= fSpringElongation - (oSpring.fRestLength + oSpring.fRestLength * oCurrent.fSpringPlastic) - (oSpring.fRestLength - oSpring.fStartLength);
					}
					oSpring.bIsAlive = fSpringElongation > oSpring.fMaxLength ? false : oSpring.bIsAlive;
				}
				if (fSpringElongation < compressiblePlasticLength && fSpringElongation < oSpring.fRestLength)
				{
					if (fSpringElongation < compressiblePlasticLength)
					{
						oSpring.fRestLength -= fSpringElongation - (compressiblePlasticLength - (oSpring.fRestLength - oSpring.fStartLength));
					}
					oSpring.bIsAlive = fSpringElongation < compressibleFractureLength ? false : oSpring.bIsAlive;
				}
				break;
			}
		}
		// Spring Force
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			Particle& oParticleCur = aParticles[iIdx];
			auto const& aLinkedSprings = oParticleCur.aLinkedSprings;
			Vec2 vSpeed = oParticleCur.bStatic ? Vec2(Consts::t0, Consts::t0) : oParticleCur.GetSpeed(oCurrent.fDeltaTime);
			for ( Spring const* pSpring : aLinkedSprings )
			{
				Spring const& oSpring = *pSpring;
				if (!oSpring.bIsAlive)
				{
					continue;
				}
				Particle& oA = *oSpring.pA;
				Particle& oB = *oSpring.pB;
				Vec2 vDelta = oB.vCurrentPos - oA.vCurrentPos;

				Vec2 const vDelta0 = oB.vCurrentPos - oA.vCurrentPos;
				Vec2 const vDelta1 = oB.vPreviousPos - oA.vPreviousPos;

				Scalar const fLengthSpeed	= ( glm::length( vDelta0 ) - glm::length( vDelta1 ) ) / oCurrent.fDeltaTime;
				Scalar const fElongation	= glm::length( vDelta );
				Scalar const fDelta			= fElongation - oSpring.fRestLength;

				vDelta /= fElongation;
				Vec2 vForce = vDelta * (Consts::t1_2 * fDelta * oSpring.fSpringStrength);

				Particle* pMe		= oSpring.pA;
				Particle* pOther	= oSpring.pB;
				if (pMe != &oParticleCur)
				{
					std::swap(pMe, pOther);
				}

				if ( pOther->bStatic )
					vForce *= Consts::t2;

				Scalar const fSign = (&oParticleCur == oSpring.pA) ? Consts::t1 : Consts::t_1;
				oParticleCur.vExternalForce += fSign * vForce;
			}
		}
		// Add Gravity
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			Particle& oParticleCur = aParticles[iIdx];
			oParticleCur.vExternalForce += Vec2(Consts::t0, oCurrent.fGravity * oParticleCur.fMass);
		}
		// Spring Damping
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			Particle& oParticleCur = aParticles[iIdx];
			auto const& aLinkedSprings = oParticleCur.aLinkedSprings;
			Vec2 vSpeed = oParticleCur.bStatic ? Vec2(Consts::t0, Consts::t0) : oParticleCur.GetSpeed(oCurrent.fDeltaTime);
			for (Spring const* pSpring : aLinkedSprings)
			{
				Spring const& oSpring = *pSpring;
				if (!oSpring.bIsAlive)
				{
					continue;
				}
				Particle& oA = *oSpring.pA;
				Particle& oB = *oSpring.pB;
				Vec2 vDelta = oB.vCurrentPos - oA.vCurrentPos;

				Vec2 const vDelta0 = oB.vCurrentPos - oA.vCurrentPos;
				Vec2 const vDelta1 = oB.vPreviousPos - oA.vPreviousPos;

				Scalar const fLengthSpeed	= ( glm::length( vDelta0 ) - glm::length( vDelta1 ) ) / oCurrent.fDeltaTime;
				Scalar const fElongation	= glm::length( vDelta );
				vDelta /= fElongation;

				Scalar const fDamping = Consts::t1_2 * fLengthSpeed * oCurrent.fSpringDamping;
				Vec2 const vDamping = vDelta * fDamping;

				Scalar const fSign = (&oParticleCur == oSpring.pA) ? Consts::t1 : Consts::t_1;

				Particle* pMe		= oSpring.pA;
				Particle* pOther	= oSpring.pB;
				if (pMe != &oParticleCur)
				{
					std::swap(pMe, pOther);
				}

				oParticleCur.vExternalForce += fSign * vDamping;

			}
		}
		// Sphere Collision
		Vec2 vAccumulatedForce = Vec2(Consts::t0, Consts::t0);
		Vec2 vAccumulatedDelta = Vec2(Consts::t0, Consts::t0);
		#pragma omp parallel for shared(vAccumulatedForce)
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			Particle& oParticleCur = aParticles[iIdx];
			Vec2 const vSphereToPoint = oParticleCur.vComputedPos - oCurrent.vColliderPosition;
			Scalar const fLengthSqr = glm::dot(vSphereToPoint, vSphereToPoint);
			Scalar const fRadiusSqr = oCurrent.fColliderRadius * oCurrent.fColliderRadius;
			if (fLengthSqr < fRadiusSqr)
			{
				Scalar const fDist = Func::Sqrt(fLengthSqr);
				Vec2 const vNormal = vSphereToPoint / fDist;
				Vec2 const vHit = oCurrent.vColliderPosition + vNormal * oCurrent.fColliderRadius;
				Vec2 const vSpeed = oParticleCur.vComputedPos - oParticleCur.vCurrentPos;
				Vec2 const vNewSpeed = glm::reflect(vSpeed, vNormal);

				oParticleCur.vCurrentPos = vHit;
				oParticleCur.vComputedPos = vHit + vNewSpeed * oCurrent.fCollisionElasticity * Consts::t1_2;

				oCurrent.vColliderPreviousPosition = oCurrent.vColliderPosition;
				oCurrent.vColliderPosition = oCurrent.vColliderPosition - vNewSpeed * oCurrent.fCollisionElasticity * Consts::t1_2;

				// Collider
				Vec2 const vSphereToPoint = oParticleCur.vComputedPos - oCurrent.vColliderPosition;
				Scalar const fLengthSqr = glm::dot(vSphereToPoint, vSphereToPoint);
				Scalar const fRadiusSqr = oCurrent.fColliderRadius * oCurrent.fColliderRadius;
				if (fLengthSqr < fRadiusSqr)
				{
					Scalar const fDist		= Func::Sqrt(fLengthSqr);
					Vec2 const vNormal		= vSphereToPoint / fDist;
					Vec2 const vHit			= oCurrent.vColliderPosition + vNormal * oCurrent.fColliderRadius;
					Vec2 const vSpeed		= oParticleCur.vComputedPos - oParticleCur.vCurrentPos;
					Vec2 const vNewSpeed	= glm::reflect(vSpeed, vNormal);

					Vec2 const vParticleForce	= oParticleCur.vExternalForce;
					if (oCurrent.bColliderPlay)
					{
						// Bonus
					}
					else
					{
						oParticleCur.vCurrentPos	= vHit;
						oParticleCur.vComputedPos	= vHit + vNewSpeed * oCurrent.fCollisionElasticity * Consts::t1_2;
					}
				}
			}
		}
		// Bonus
		if (oCurrent.bColliderPlay)
		{
			oCurrent.vColliderExternalForce	+= vAccumulatedForce;
			oCurrent.vColliderPosition		+= vAccumulatedDelta;
		}

		// Ground Friction
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			Particle& oParticleCur = aParticles[iIdx];
			Vec2 const vVirtualNextPos = oParticleCur.VirtualStep(oCurrent.fDeltaTime);
			if (vVirtualNextPos.y <= Consts::t0)
			{
				Vec2 const vHit = IntersectSegmentToGround(vVirtualNextPos, oParticleCur.vCurrentPos);
				Vec2 const vDelta = vVirtualNextPos - vHit;

				if (glm::dot(vDelta, vDelta) > 1e-6f * 1e-6f)
					oParticleCur.vExternalForce += Vec2(-oParticleCur.GetSpeed(oCurrent.fDeltaTime).x * oCurrent.fGroundFriction, Consts::t0);
			}
		}
		// Integration & Collision
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			// Collision with ground at y == 0.0f
			Particle& oParticleCur = aParticles[iIdx];
			oParticleCur.Step(oCurrent.fDeltaTime);
			Vec2 const vCurPos = oParticleCur.vCurrentPos;
			Vec2 const vNewPos = oParticleCur.vComputedPos;
			// Ground Collision
			if (vNewPos.y < 0.0f)
			{
				Vec2 vHit;
				if (LineLineIntersect(vHit, vNewPos, vCurPos, Vec2(-Consts::t2048, Consts::t0), Vec2(Consts::t2048, Consts::t0)))
				{
					Vec2 const vSpeed		= vNewPos - vCurPos;
					Vec2 const vNewSpeed	= glm::reflect(vSpeed, Vec2(Consts::t0, Consts::t1));

					oParticleCur.vCurrentPos	=	vHit;
					oParticleCur.vComputedPos	=	vHit + vNewSpeed * oCurrent.fCollisionElasticity;
				}
			}
		}
		if (oCurrent.bColliderPlay)
		{
			// Collider Integratino
			Vec2 vTmp = oCurrent.vColliderPosition;
			oCurrent.vColliderPosition = Integrate(oCurrent.vColliderPosition, oCurrent.vColliderPreviousPosition, oCurrent.fDeltaTime, oCurrent.fColliderMass, oCurrent.vColliderExternalForce);
			oCurrent.vColliderPreviousPosition = vTmp;
			oCurrent.vColliderExternalForce = Vec2(Consts::t0, Consts::t0);
			// Collider collision with Ground
			if (oCurrent.vColliderPosition.y - oCurrent.fColliderRadius < 0.0f)
			{
				Vec2 const vDelta = oCurrent.vColliderPosition - oCurrent.vColliderPreviousPosition;
				Vec2 const vHeight = Vec2(Consts::t0, oCurrent.fColliderRadius);
				Vec2 const vHit = IntersectSegmentToGround(oCurrent.vColliderPosition - vHeight, oCurrent.vColliderPreviousPosition - vHeight) + vHeight;
				Vec2 const vNewSpeed = glm::reflect(vDelta, Vec2(Consts::t0, Consts::t1));
				oCurrent.vColliderPreviousPosition	= vHit;
				oCurrent.vColliderPosition			= vHit + vNewSpeed * oCurrent.fCollisionElasticity;
			}
		}
		#pragma omp parallel for
		for (Int32 iIdx = 0; iIdx < iSize; ++iIdx)
		{
			Particle& oParticleCur = aParticles[iIdx];
			oParticleCur.Swap();
		}
	}
	else
	{
		oCurrent.vColliderPreviousPosition	= oCurrent.vColliderPosition;
		oCurrent.vColliderExternalForce		= Vec2();
	}
}
