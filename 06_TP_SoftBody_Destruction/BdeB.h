#pragma once

#include "imgui.h"
#include "Common/Kernel.h"

void Initialize();
void Update(f32 const dt);

void DrawUI();
void DrawScene(ImDrawList* pDrawList, f32 const dt);
