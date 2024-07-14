#pragma once

#include "Olc/olcPixelGameEngine.h"

namespace Cup {

	class ImGuiCommand
	{
	public:
		static bool Init(olc::PixelGameEngine* appPtr);
		static void Shutdown();

		static void Begin(const olc::vi2d& windowSize);
		static void End();
	};

}

