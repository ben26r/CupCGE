#include "ImGuiCommand.h"

#include "gl/GL.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_opengl3.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Cup {

	static olc::PixelGameEngine* s_appPtr = nullptr;
	static olc::vi2d s_windowSize = { 0, 0 };

	bool ImGuiCommand::Init(olc::PixelGameEngine* appPtr)
	{
		s_appPtr = appPtr;

		// Setup Dear ImGui context
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Set up key mapping
		io.KeyMap[ImGuiKey_Tab] = olc::Key::TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = olc::Key::LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = olc::Key::RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = olc::Key::UP;
		io.KeyMap[ImGuiKey_DownArrow] = olc::Key::DOWN;
		io.KeyMap[ImGuiKey_PageUp] = olc::Key::PGUP;
		io.KeyMap[ImGuiKey_PageDown] = olc::Key::PGDN;
		io.KeyMap[ImGuiKey_Home] = olc::Key::HOME;
		io.KeyMap[ImGuiKey_End] = olc::Key::END;
		io.KeyMap[ImGuiKey_Insert] = olc::Key::INS;
		io.KeyMap[ImGuiKey_Delete] = olc::Key::DEL;
		io.KeyMap[ImGuiKey_Backspace] = olc::Key::BACK;
		io.KeyMap[ImGuiKey_Space] = olc::Key::SPACE;
		io.KeyMap[ImGuiKey_Enter] = olc::Key::ENTER;
		io.KeyMap[ImGuiKey_Escape] = olc::Key::ESCAPE;
		io.KeyMap[ImGuiKey_A] = olc::Key::A;
		io.KeyMap[ImGuiKey_C] = olc::Key::C;
		io.KeyMap[ImGuiKey_V] = olc::Key::V;
		io.KeyMap[ImGuiKey_X] = olc::Key::X;
		io.KeyMap[ImGuiKey_Y] = olc::Key::Y;
		io.KeyMap[ImGuiKey_Z] = olc::Key::Z;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer bindings
		ImGui_ImplOpenGL3_Init("#version 410");

		return true;
	}

	void ImGuiCommand::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();

		// Cleanup OpenGL resources
		HGLRC hglrc = wglGetCurrentContext();
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hglrc);
	}

	void ImGuiCommand::Begin(const olc::vi2d& windowSize)
	{
		s_windowSize = windowSize;
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2((float)windowSize.x, (float)windowSize.y);

		// Setup time step
		io.DeltaTime = s_appPtr->GetElapsedTime();

		auto pos = s_appPtr->GetWindowMouse();
		// Setup inputs
		io.MousePos = ImVec2((float)pos.x, (float)pos.y);
		io.MouseDown[0] = s_appPtr->GetMouse(0).bHeld;
		io.MouseDown[1] = s_appPtr->GetMouse(1).bHeld;
		io.MouseWheel = (float)s_appPtr->GetMouseWheel();

		for (int i = 0; i < 512; i++) {
			io.KeysDown[i] = s_appPtr->GetKey((olc::Key)i).bHeld;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiCommand::End()
	{
		//s_appPtr->SetPixelMode(olc::Pixel::ALPHA);
		ImGui::Render();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)s_windowSize.x, (float)s_windowSize.y);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}