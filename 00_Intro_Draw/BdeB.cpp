#include "BdeB.h"

#include <vector>

#include <cmath>

#include <Common/Kernel.h>
#include <Common/Consts.h>
#include <Common/Func.h>
#include <Common/Helpers.h>
#include "imgui_internal.h"

using namespace SK;

f32		fCircleScale	= 1.0f;
Int32	iCircleStep		= 12;
Bool	bShowCircle		= false;

ImVec2 vPoint;
ImVec2 vPointA;
ImVec2 vPointB;

void Initialize()
{}

void DrawUI()
{
	ImGui::Separator();

	ImGui::Checkbox("Show Circle", &bShowCircle);
	if (bShowCircle)
	{
		ImGui::SliderFloat("Circle Scale", &fCircleScale, 0.0f, 10.0f);
		ImGui::SliderInt("Circle Segments", &iCircleStep, 3, 64);
	}

	ImGui::DragFloat2("Point", &vPoint.x, 0.1f, -16.0f, 16.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat2("Point A", &vPointA.x, 0.1f, -16.0f, 16.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
	ImGui::DragFloat2("Point B", &vPointB.x, 0.1f, -16.0f, 16.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

	ImGui::Separator();
}

void DrawScene(ImDrawList* pDrawList, f32 const dt)
{
	pDrawList->AddCircleFilled(ImToDraw(vPoint), 5.0f, IM_COL32(255, 128, 0, 255));
	pDrawList->AddLine(ImToDraw(vPointA), ImToDraw(vPointB), IM_COL32(255, 128, 0, 255), 5.0f);

	// TODO: Draw disk at vPoint position of a radius of 5.0f
	// TODO: Draw between vPointA & vPointB of a thickness of 5.0f

	// Helper:
	//		- ImToDraw
	//		- VecToImDraw

	if (bShowCircle)
	{
		f32 const fDeltaTheta = Constsf::tTau / static_cast<f32>(iCircleStep);

		Vec2f vPrevious = { vPoint.x + fCircleScale, vPoint.y };

		for (f32 fTheta = fDeltaTheta; fTheta <= Constsf::tTau + 1e-5f; fTheta += fDeltaTheta)
		{
			//pDrawList->AddLine(VecToImDraw(0.0f, 0.0f), VecToImDraw(0.0f, 0.0f), IM_COL32(255, 128, 0, 255), 5.0f);
			// TODO	Afficher un cercle de centre vPoint de rayon fCircleScale composé de iCircleStep segment

			Vec2f vCurrent = { fCircleScale * Funcf::Cos(fTheta), fCircleScale * Funcf::Sin(fTheta) };
			vCurrent.x += vPoint.x;
			vCurrent.y += vPoint.y;
			pDrawList->AddLine(VecToImDraw(vPrevious), VecToImDraw(vCurrent), IM_COL32(255, 128, 0, 255), 5.0f);
			vPrevious = vCurrent;
			
		}
	}
}

void Update(f32 const dt)
{}
