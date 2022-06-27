#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <random>

#include "imgui_internal.h"
#include "imgui_canvas.h"

#include <Common/Kernel.h>
#include <Common/Consts.h>
#include <Common/Func.h>

#include <vector>
#include <tuple>

#include "BdeB.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void DrawScale(const ImVec2& from, const ImVec2& to, float majorUnit, float minorUnit, float labelAlignment, float sign = 1.0f)
{
	auto drawList = ImGui::GetWindowDrawList();
	auto direction = (to - from) * ImInvLength(to - from, 0.0f);
	auto normal = ImVec2(-direction.y, direction.x);
	auto distance = sqrtf(ImLengthSqr(to - from));

	if (ImDot(direction, direction) < FLT_EPSILON)
		return;

	auto minorSize = 5.0f;
	auto majorSize = 10.0f;
	auto labelDistance = 8.0f;

	drawList->AddLine(from, to, IM_COL32(255, 255, 255, 255));

	auto p = from;
	for (auto d = 0.0f; d <= distance; d += minorUnit, p += direction * minorUnit)
		drawList->AddLine(p - normal * minorSize, p + normal * minorSize, IM_COL32(255, 255, 255, 255));

	for (auto d = 0.0f; d <= distance + majorUnit; d += majorUnit)
	{
		p = from + direction * d;

		drawList->AddLine(p - normal * majorSize, p + normal * majorSize, IM_COL32(255, 255, 255, 255));

		if (d == 0.0f)
			continue;

		char label[16];
		snprintf(label, 15, "%g", d * sign / 100.0f);
		auto labelSize = ImGui::CalcTextSize(label);

		auto labelPosition = p + ImVec2(fabsf(normal.x), fabsf(normal.y)) * labelDistance;
		auto labelAlignedSize = ImDot(labelSize, direction);
		labelPosition += direction * (-labelAlignedSize + labelAlignment * labelAlignedSize * 2.0f);
		labelPosition = ImFloor(labelPosition + ImVec2(0.5f, 0.5f));

		drawList->AddText(labelPosition, IM_COL32(255, 255, 255, 255), label);
	}
}

Vec2	IntersectSegmentToGround(Vec2 const vA, Vec2 const vB)
{
	Vec2 const vDelta = vA - vB;

	Scalar const fH = Func::Abs(vA.y);
	Scalar const fL = glm::length(vDelta);
	Scalar const fQ = fL * fH / Func::Abs(vDelta.y);
	Scalar const fW = Func::Sqrt(fQ * fQ - fH * fH);

	return Vec2(vA.x - fW, 0.0f);
}

Vec2	d2Ydt2(Vec2 const force, Scalar const mass)
{
	return (force / mass);
}

Vec2	dYdt(Vec2 const yt, Vec2 const yt_dt, Scalar const dt)
{
	return (yt - yt_dt) / dt;
}

//std::pair<Vec2, Vec2>	Integrate(Vec2 const vPos, Vec2 const vSpeed, Vec2 const vAcc, Scalar const dt)

static std::default_random_engine g_oGen;
static std::uniform_real_distribution<Scalar> g_oDist(Consts::t_1, Consts::t1);

Scalar	Random(Scalar const fMin, Scalar const fMax)
{
	return Func::ReScale(g_oDist(g_oGen), Consts::t_1, Consts::t1, fMin, fMax);
}

//void ResetSimulation(std::vector<Body>& aBodies, Scalar const fMass, Scalar const fDeltaTime, UInt32 const uCount)
//{
//}

int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Physique", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // 1 to Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	// Our state
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);

	ImGuiEx::Canvas oCanvas;
	ImGuiWindowSettings* pWinSettings = ImGui::FindWindowSettings(ImHashStr("Canvas"));
	if (pWinSettings)
	{
		oCanvas.SetView(oCanvas.ViewOrigin() + ImVec2(static_cast<f32>(pWinSettings->Size.x) * 0.5f, static_cast<f32>(pWinSettings->Size.y) * 0.5f), oCanvas.ViewScale());
	}
	else
	{
		oCanvas.SetView(oCanvas.ViewOrigin() + ImVec2(300.0f, 300.0f), oCanvas.ViewScale());
	}

	ImVec2 vDrawStartPoint;
	Bool bIsDragging = false;
	ImRect oPanelRect;
	f32 fViewScale = oCanvas.ViewScale();

	Initialize();

	constexpr f32 fScale = 300.0f;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (ImGui::Begin(u8"Mes paramet\u00e8res"))
		{
			ImGui::Separator();

			ImGui::TextUnformatted("Scale:");
			ImGui::Indent();
			ImGui::PushItemWidth(-ImGui::GetStyle().IndentSpacing);
			if (ImGui::DragFloat("##scale", &fViewScale, 0.01f, 0.01f, 1500.0f))
				oCanvas.SetView(oCanvas.ViewOrigin(), fViewScale);
			ImGui::PopItemWidth();
			ImGui::Unindent();

			ImGui::Separator();
			DrawUI();
			ImGui::Separator();

			ImGui::End();
		}
		Update(ImGui::GetIO().DeltaTime);
		if (ImGui::Begin("Canvas"))
		{
			ImVec2 const vWinSize = ImGui::GetContentRegionAvail();
			if (oCanvas.Begin("##RenderCanvas", ImVec2(vWinSize.x, 0.0f)))
			{
				ImDrawList* pDrawList = ImGui::GetWindowDrawList();

				ImRect oCanvasRect = oCanvas.Rect();
				ImRect oViewRect = oCanvas.ViewRect();
				ImVec2 vViewOrigin = oCanvas.ViewOrigin();
				ImVec2 const vStartPos = ImGui::GetCursorScreenPos();
				f32 const fMinSize = ImMin(vWinSize.x, vWinSize.y);

				DrawScene(pDrawList, ImGui::GetIO().DeltaTime);

				if ((bIsDragging || ImGui::IsItemHovered()) && ImGui::IsMouseDragging(1, 0.0f))
				{
					if (!bIsDragging)
					{
						bIsDragging = true;
						vDrawStartPoint = vViewOrigin;
					}

					oCanvas.SetView(vDrawStartPoint + ImGui::GetMouseDragDelta(1, 0.0f) * fViewScale, fViewScale);
				}
				else if (bIsDragging)
				{
					bIsDragging = false;
				}

				oViewRect = oCanvas.ViewRect();

				if (oViewRect.Max.x > 0.0f)
					DrawScale(ImVec2(0.0f, 0.0f), ImVec2(oViewRect.Max.x, 0.0f), 100.0f, 10.0f, 0.6f);
				if (oViewRect.Min.x < 0.0f)
					DrawScale(ImVec2(0.0f, 0.0f), ImVec2(oViewRect.Min.x, 0.0f), 100.0f, 10.0f, 0.6f, -1.0f);
				if (oViewRect.Max.y > 0.0f)
					DrawScale(ImVec2(0.0f, 0.0f), ImVec2(0.0f, oViewRect.Max.y), 100.0f, 10.0f, 0.6f, -1.0f);
				if (oViewRect.Min.y < 0.0f)
					DrawScale(ImVec2(0.0f, 0.0f), ImVec2(0.0f, oViewRect.Min.y), 100.0f, 10.0f, 0.6f);

				oPanelRect.Min = ImGui::GetItemRectMin();
				oPanelRect.Max = ImGui::GetItemRectMax();

				oCanvas.End();
			}
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
