#include "BdeB.h"

#include <vector>

#include <cmath>

#include <Common/Kernel.h>
#include <Common/Consts.h>
#include <Common/Func.h>
#include <Common/Helpers.h>
#include "imgui_internal.h"

using namespace SK;

f32 fDeltaTime = 0.0125f;
f32 fMass = 1.0f;
f32 fGravity = -9.81f;
f32 fStartAngle = Consts::tTau_8;
f32 fInitialSpeed = 1.0f;
f32 fFrictionCoef = 1.0f;
Vec2 vExternalForce = Vec2(Consts::t0, Consts::t0);

std::vector<Vec2> aPoints;
Bool bContinueUpdate = false;

void Initialize()
{
	aPoints.clear();
	aPoints.reserve(1024);
	aPoints.emplace_back(0.0f, 0.0f);
	aPoints.emplace_back(Func::Cos(fStartAngle) * (fInitialSpeed * fDeltaTime),
		Func::Sin(fStartAngle) * (fInitialSpeed * fDeltaTime));
	vExternalForce = Vec2(Consts::t0, Consts::t0);
	bContinueUpdate = true;
}

void DrawUI()
{
	if (ImGui::Button("Start/Reset"))
	{
		Initialize();
	}

	ImGui::Separator();

	ImGui::DragFloat(u8"\u0394t", &fDeltaTime, 0.01f, 1e-3f, 2.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Mass", &fMass, 0.01f, 0.0f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Gravity", &fGravity, 0.01f, -20.0f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat(u8"\u03B8", &fStartAngle, 0.01f, -Constsf::tTau, Constsf::tTau, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Initial Speed", &fInitialSpeed, 0.01f, 0.0f, 10.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat("Friction Coef", &fFrictionCoef, 0.01f, 0.0f, 10.0f, " %.3f", ImGuiSliderFlags_Logarithmic);
}

void DrawScene(ImDrawList* pDrawList, f32 const dt)
{
	pDrawList->AddLine(ImVec2(0, 0), VecToImDraw(Func::Cos(fStartAngle), Func::Sin(fStartAngle)) * fInitialSpeed, IM_COL32(0, 255, 0, 255));

	if (!aPoints.empty())
	{
		for (Vec2 v : aPoints)
			pDrawList->AddCircleFilled(VecToImDraw(v.x, v.y), 5.0f, IM_COL32(255, 128, 0, 255), 8);

		auto oIt = aPoints.begin();
		auto oPrev = oIt;
		auto oEnd = aPoints.end();
		++oIt;
		for (; oIt != oEnd; ++oPrev, ++oIt)
		{
			pDrawList->AddLine(VecToImDraw(oPrev->x, oPrev->y), VecToImDraw(oIt->x, oIt->y), IM_COL32(128, 0, 0, 255), 5.0f);
		}
		oIt = aPoints.begin();
		oEnd = aPoints.end();
		f32 fY = 0.0f;
		f32 ft = 0.0f;
		ImVec2 vY0 = ImVec2(ft, fY);
		ImVec2 vY1;
		// Ground truth
		for (; oIt != oEnd; ++oIt)
		{
			ImVec2 vY1 = ImVec2(
				fInitialSpeed * ft * Func::Cos(fStartAngle),
				Constsf::t1_2 * ft * ft * fGravity + fInitialSpeed * ft * Func::Sin(fStartAngle));

			pDrawList->AddLine(ImToDraw(vY0), ImToDraw(vY1), IM_COL32(0, 255, 0, 128), 5.0f);

			ft += fDeltaTime;
			vY0 = vY1;

			if (vY1.y < 0.0f)
				break;
		}
	}
}

Vec2	IntersectSegmentToGround(Vec2 const vA, Vec2 const vB)
{
	return vA; // TODO
}

Vec2	d2Ydt2(Vec2 const force, Scalar const mass)
{
	return force; // TOPO
}

Vec2	dYdt(Vec2 const yt, Vec2 const yt_dt, Scalar const dt)
{
	return yt; // TODO
}

Vec2	Integrate(Vec2 const yt, Vec2 const yt_dt, Scalar const dt, Scalar const g, Scalar const mass, Vec2 const vExternalForce)
{
	return yt; // TODO
}

void Update(f32 const dt)
{
	// Simulation Code Here
	if (bContinueUpdate)
	{
		// Add to list to draw and reorient frame
		Vec2 const vCurPos = aPoints[aPoints.size() - 1];
		Vec2 const vPrevPos = aPoints[aPoints.size() - 2];
		//Vec2 const vCurPos = aPoints.back();
		//Vec2 const vPrevPos = *(--(--aPoints.end()));
		vExternalForce = -fFrictionCoef * (vCurPos - vPrevPos) / fDeltaTime; // Force de friction
		Vec2 vNewPos = Integrate(vCurPos, vPrevPos, fDeltaTime, fGravity, fMass, vExternalForce);

		// TODO: use IntersectSegmentToGround

		aPoints.emplace_back(vNewPos);
	}

	if (aPoints.size() >= 4096)
	{
		bContinueUpdate = false;
	}
}
