#include "ImGuiCommand.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

namespace Cup {

	struct ImGuiCommandData
	{
		olc::PixelGameEngine* appPtr;
		uint32_t layer = 0;
	};

	static ImGuiCommandData s_data;

	bool ImGuiCommand::Init(olc::PixelGameEngine* appPtr)
	{
		s_data.appPtr = appPtr;
		s_data.layer = appPtr->CreateLayer();
		appPtr->EnableLayer(s_data.layer, true);
		appPtr->SetLayerCustomRenderFunction(s_data.layer, std::bind(&olc::PixelGameEngine::OnUserImGuiRender, appPtr));

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		olc::imgui::ImGui_ImplPGE_Init(appPtr);
		return true;

	}
	void ImGuiCommand::Shutdown()
	{
		olc::imgui::ImGui_ImplPGE_Shutdown();
	}

	void ImGuiCommand::Begin()
	{
		olc::imgui::ImGui_ImplPGE_NewFrame();
		ImGui::NewFrame();
	}
	void ImGuiCommand::End()
	{
		ImGui::Render();
		olc::imgui::ImGui_ImplPGE_Render();
	}
}