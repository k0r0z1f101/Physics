#include "BdeB.h"

#include <vector>

#include <cmath>

#include <Common/Kernel.h>
#include <Common/Consts.h>
#include <Common/Func.h>
#include <Common/Helpers.h>
#include "imgui_internal.h"

using namespace SK;

f32 fCircleScale = 1.0f;
Int32 iCircleStep = 12;

void Initialize()
{}

void DrawUI()
{
	ImGui::SliderFloat("Circle Scale", &fCircleScale, 0.0f, 10.0f);
	ImGui::SliderInt("Circle Segments", &iCircleStep, 3, 128);
}

void DrawScene(ImDrawList* pDrawList, f32 const dt)
{
	f32 const fScale = fCircleScale;

	f32 fX_ = Funcf::Cos(0.0f);
	f32 fY_ = Funcf::Sin(0.0f);
	f32 const fDeltaTheta = Constsf::tTau / static_cast<f32>(iCircleStep);
	for (f32 fTheta = fDeltaTheta; fTheta <= Constsf::tTau + 1e-5f; fTheta += fDeltaTheta)
	{
		f32 const fX = Funcf::Cos(fTheta);
		f32 const fY = Funcf::Sin(fTheta);

		pDrawList->AddLine(
			VecToImDraw(fScale * fX_, fScale * fY_),
			VecToImDraw(fScale * fX, fScale * fY),
			IM_COL32(255, 128, 0, 255),
			5.0f * g_fDrawScale * 0.01f);

		fX_ = fX;
		fY_ = fY;
	}
}

void Update(f32 const dt)
{}
