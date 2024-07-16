#pragma once

#include "Core/Base.h"
#include "Olc/olcPixelGameEngine.h"
#include "ImGui/imgui_impl_pge.h"

namespace Cup {

	class ImGuiCommand
	{
	public:
		static bool Init(olc::PixelGameEngine* appPtr);
		static void Shutdown();

		static void Begin();
		static void End();
	};

}

