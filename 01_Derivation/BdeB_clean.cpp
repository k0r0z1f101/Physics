#include "BdeB.h"

#include <vector>

#include <cmath>

#include <Common/Kernel.h>
#include <Common/Consts.h>
#include <Common/Func.h>
#include <Common/Helpers.h>
#include "imgui_internal.h"

using namespace SK;

enum NumericalDifference
{
	Centered = 0,
	Forward,
	Backward,
	CenteredOrder6
};

f32 fDeltaTime = 0.0125f;

constexpr f32 fScale = 300.0f;
constexpr f32 fUpperBound = 3.0f * Constsf::tTau;

Bool bShowIntegration = false;
Bool bShowLocalDiff = false;
Bool bShowAllLocalDiff = false;

f32 fPosition = 0.0f;
f32 fLocalDelta = 0.0125f;

NumericalDifference eCurNumericalDiff = Centered;

f32 Function(f32 const x)
{
	return Funcf::Cos(x);
	//return Funcf::Cos(x * x);
	//return x * x;
}
f32 DFunction(f32 const x)
{
	return -Funcf::Sin(x);
	//return 2.0f * x;
	//return -2.0f * x * Funcf::Sin(x * x);
}

void Initialize()
{}

void DrawUI()
{
	ImGui::DragFloat(u8"\u0394t", &fDeltaTime, 0.01f, 1e-3f, 2.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::Separator();

	ImGui::DragFloat("Position", &fPosition, 0.01f, 0.0f, fUpperBound, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat(u8"Local \u0394t", &fLocalDelta, 0.01f, 1e-3f, 2.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::Checkbox("Show Integration", &bShowIntegration);
	ImGui::Checkbox("Show Local Differentiation", &bShowLocalDiff);
	ImGui::Checkbox("Show All Local Diff", &bShowAllLocalDiff);

	char const* pItems[] = { "Centered", "Forward", "Backward", "CenteredOrder6" };
	static char const* pCurrentItem = pItems[eCurNumericalDiff];

	if (ImGui::BeginCombo("##combo", pCurrentItem))
	{
		for (Int32 n = 0; n < IM_ARRAYSIZE(pItems); n++)
		{
			Bool bIsSelected = (pCurrentItem == pItems[n]);
			if (ImGui::Selectable(pItems[n], bIsSelected))
			{
				pCurrentItem = pItems[n];
				eCurNumericalDiff = static_cast<NumericalDifference>(n);
				if (bIsSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}
}

void DrawScene(ImDrawList* pDrawList, f32 const dt)
{
	f32 const fDeltaTheta = Constsf::tTau / static_cast<f32>(128);

	// Ground truth
	f32 fY_ = Function(-Constsf::tTau_2);
	for (f32 fTheta = -Constsf::tTau_2; fTheta <= fUpperBound; fTheta += fDeltaTheta)
	{
		f32 const fY = Function(fTheta);

		pDrawList->AddLine(VecToImDraw(fTheta - fDeltaTheta, fY_), VecToImDraw(fTheta, fY), IM_COL32(255, 255, 255, 255), 2.5f);

		fY_ = fY;
	}

	// Measurement Point
	pDrawList->AddCircleFilled(VecToImDraw(fPosition, Function(fPosition)), 5.0f, IM_COL32(255, 128, 0, 255));
	pDrawList->AddCircleFilled(VecToImDraw(fPosition - fLocalDelta, Function(fPosition - fLocalDelta)), 5.0f, IM_COL32(0, 255, 128, 255));
	pDrawList->AddCircleFilled(VecToImDraw(fPosition + fLocalDelta, Function(fPosition + fLocalDelta)), 5.0f, IM_COL32(0, 255, 128, 255));

	if (bShowLocalDiff)
	{
		constexpr f32 fLineS = 10.0f;
		// Centered
		f32 const fDYC = ((- 0.5f * Function(fPosition - fLocalDelta)) + (0.5f * Function(fPosition + fLocalDelta))) / fLocalDelta;
		// Forward
		f32 const fDYF = (-Function(fPosition) + Function(fPosition + fLocalDelta)) / fLocalDelta; // TODO
		// Backward
		f32 const fDYB = (Function(fPosition) - Function(fPosition - fLocalDelta)) / fLocalDelta; // TODO
		// Sixth order accuracy - 1st derivative
		f32 const fDY6 = ((-1.0f / 60.0f) * Function(fPosition - 3.0f * fLocalDelta)
			+ (3.0f / 20.0f) * Function(fPosition - 2.0f * fLocalDelta)
			- (3.0f / 4.0f) * Function(fPosition - fLocalDelta)
			+ (3.0f / 4.0f) * Function(fPosition + fLocalDelta)
			- (3.0f / 20.0f) * Function(fPosition + 2.0f * fLocalDelta)
			+ (1.0f / 60.0f) * Function(fPosition + 3.0f * fLocalDelta)) / fLocalDelta;
			
			
			
			
			
			
			/*((-1 / 60) * Function(fPosition - 3.0f * fLocalDelta)
						+ (3 / 20) * Function(fPosition - 2.0f * fLocalDelta)
						- (3 / 4) * Function(fPosition - fLocalDelta)
						+ (3 / 4) * Function(fPosition + fLocalDelta)
						- (3 / 20) * Function(fPosition + 2.0f * fLocalDelta)
						+ (1 / 60) * Function(fPosition + 3.0f * fLocalDelta))
							/ fLocalDelta;*/ // TODO

		pDrawList->AddLine(VecToImDraw(fPosition - fLineS, Function(fPosition) - DFunction(fPosition) * fLineS), VecToImDraw(fPosition + fLineS, Function(fPosition) + DFunction(fPosition) * fLineS), IM_COL32(0, 255, 0, 255), 2.0f);
		if (bShowAllLocalDiff || eCurNumericalDiff == Centered)
			pDrawList->AddLine(VecToImDraw(fPosition - fLineS, Function(fPosition) - fDYC * fLineS), VecToImDraw(fPosition + fLineS, Function(fPosition) + fDYC * fLineS), IM_COL32(255, 128, 0, 255), 2.0f);
		if (bShowAllLocalDiff || eCurNumericalDiff == Forward)
			pDrawList->AddLine(VecToImDraw(fPosition - fLineS, Function(fPosition) - fDYF * fLineS), VecToImDraw(fPosition + fLineS, Function(fPosition) + fDYF * fLineS), IM_COL32(0, 128, 255, 255), 2.0f);
		if (bShowAllLocalDiff || eCurNumericalDiff == Backward)
			pDrawList->AddLine(VecToImDraw(fPosition - fLineS, Function(fPosition) - fDYB * fLineS), VecToImDraw(fPosition + fLineS, Function(fPosition) + fDYB * fLineS), IM_COL32(128, 0, 255, 255), 2.0f);
		if (bShowAllLocalDiff || eCurNumericalDiff == CenteredOrder6)
			pDrawList->AddLine(VecToImDraw(fPosition - fLineS, Function(fPosition) - fDY6 * fLineS), VecToImDraw(fPosition + fLineS, Function(fPosition) + fDY6 * fLineS), IM_COL32(128, 128, 128, 255), 2.0f);
	}

	if (bShowIntegration)
	{
		f32 fComputedY = Function(fPosition);
		f32 fDY_ = DFunction(fPosition);
		UInt32 const uIterCount = static_cast<UInt32>(Funcf::Round(fUpperBound / static_cast<f32>(fDeltaTime)));
		f32 fTheta = fPosition;
		for (UInt32 uIdx = 0; uIdx <= uIterCount; ++uIdx)
		{
			f32 const fDY = DFunction(fTheta);
			f32 const fNewY = 2.0f * Function(fTheta) - Function(fTheta - fDeltaTime) + fDeltaTime * fDeltaTime; // TODO

			pDrawList->AddLine(VecToImDraw(fTheta, fComputedY), VecToImDraw(fTheta + fDeltaTime, fNewY), IM_COL32(128, 255, 128, 255), 2.5f);

			fComputedY = fNewY;

			fTheta += fDeltaTime;
		}
		fTheta = fPosition;
		fComputedY = Function(fPosition);
		for (UInt32 uIdx = 0; uIdx <= uIterCount; ++uIdx)
		{
			// Sources:
			//		https://en.wikipedia.org/wiki/Finite_difference_coefficient
			f32 fDY;
			switch (eCurNumericalDiff)
			{
			case Centered:
				fDY = ((-0.5f * Function(fTheta - fDeltaTime)) + (0.5f * Function(fTheta + fDeltaTime))) / fDeltaTime; // TODO
				break;
			case Forward:
				fDY = (-Function(fTheta) + Function(fTheta + fDeltaTime)) / fDeltaTime; // TODO
				break;
			case Backward:
				fDY = (Function(fTheta) - Function(fTheta - fDeltaTime)) / fDeltaTime; // TODO
				break;
			case CenteredOrder6:
			default:
				// Sixth order accuracy - 1st derivative
				fDY = ((-1.0f / 60.0f) * Function(fTheta - 3.0f * fDeltaTime)
					+ (3.0f / 20.0f) * Function(fTheta - 2.0f * fDeltaTime)
					- (3.0f / 4.0f) * Function(fTheta - fDeltaTime)
					+ (3.0f / 4.0f) * Function(fTheta + fDeltaTime)
					- (3.0f / 20.0f) * Function(fTheta + 2.0f * fDeltaTime)
					+ (1.0f / 60.0f) * Function(fTheta + 3.0f * fDeltaTime)) / fDeltaTime;
					
					
					
					
					/*((-1 / 60) * Function(fTheta - 3.0f * fDeltaTime)
					+ (3 / 20) * Function(fTheta - 2.0f * fDeltaTime)
					- (3 / 4) * Function(fTheta - fDeltaTime)
					+ (3 / 4) * Function(fTheta + fDeltaTime)
					- (3 / 20) * Function(fTheta + 2.0f * fDeltaTime)
					+ (1 / 60) * Function(fTheta + 3.0f * fDeltaTime))
					/ fDeltaTime;*/ // TODO
				break;
			}
			f32 const fNewY = fComputedY + fDeltaTime * fDY;

			pDrawList->AddLine(VecToImDraw(fTheta, fComputedY), VecToImDraw(fTheta + fDeltaTime, fNewY), IM_COL32(255, 0, 128, 255), 2.5f * g_fDrawScale * 0.01f);

			fComputedY = fNewY;

			fTheta += fDeltaTime;
		}
	}
}

void Update(f32 const dt)
{}
