#pragma once

#include "Core/Base.h"

#include "CupEngine.h"

namespace Cup {

	class Input
	{
	public:
		static void Init(CupEngine* appPtr);

		static bool GetKeyPressed(olc::Key key);
		static bool GetKeyDown(olc::Key key);
		static bool GetKeyUp(olc::Key key);

		static bool GetMousePressed(uint32_t button);
		static bool GetMouseDown(uint32_t button);
		static bool GetMouseUp(uint32_t button);

		static const Vector2i& GetMousePos();
		static uint32_t GetMouseX();
		static uint32_t GetMouseY();

	private:
		static CupEngine* m_appPtr;
	};

}