#include "InspectorStyles.h"
#include <filesystem>
#include <windows.h>
#include <iostream>

namespace Cup {

	template <typename T>
	void InspectorStyles::GetStyle(T& component)
	{
	}

	template <>
	void InspectorStyles::GetStyle<TagComponent>(TagComponent& component)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), component.tag.c_str());
		if (ImGui::InputText("Name", buffer, sizeof(buffer)))
			component.tag = std::string(buffer);
	}

	template <>
	void InspectorStyles::GetStyle<TransformComponent>(TransformComponent& component)
	{
			ImGui::Text("Position");
			ImGui::SameLine();
			ImGui::DragFloat3("##position", &component.position.x, 0.1f);

			ImGui::Text("Rotation");
			ImGui::SameLine();
			ImGui::DragFloat3("##rotation", &component.rotation.x, 0.1f);

			ImGui::Text("Scale   ");
			ImGui::SameLine();
			ImGui::DragFloat3("##scale", &component.scale.x, 0.1f);
	}

	template <>
	void InspectorStyles::GetStyle<CameraComponent>(CameraComponent& component)
	{
			ImGui::DragFloat("Near", &component.camera->cnear, 0.1f);
			ImGui::DragFloat("Yaw", &component.camera->m_yaw, 0.1f);
			ImGui::DragFloat("Far", &component.camera->cfar, 0.1f);
			ImGui::DragFloat("Aspect Ratio", &component.camera->aspectRatio, 0.1f);
			ImGui::DragFloat("Fov", &component.camera->fov, 0.1f);
			ImGui::DragFloat3("Position", &component.camera->m_position.x, 0.1f);
			ImGui::Checkbox("Main Camera", &component.mainCamera);
	}


	static std::string GetFilePathFromExplorer() {
		wchar_t szFile[260] = {0}; // Buffer for file path

		OPENFILENAME ofn; // Common dialog box structure
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL; // Handle to the parent window
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
		ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0"; // Filter for file types
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL; // Initial directory
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // Flags for dialog box

		// Display the file dialog box
		if (GetOpenFileName(&ofn) == TRUE) {
			// The user selected a file
			// Convert wide string to narrow string
			int sizeRequired = WideCharToMultiByte(CP_ACP, 0, szFile, -1, NULL, 0, NULL, NULL);
			std::string filePath(sizeRequired - 1, 0);
			WideCharToMultiByte(CP_ACP, 0, szFile, -1, &filePath[0], sizeRequired, NULL, NULL);
			return filePath;
		}
		return ""; // Return an empty string if no file was selected
	}

	template <>
	void InspectorStyles::GetStyle<MeshRendererComponent>(MeshRendererComponent& component)
	{
			CupEngine::Instance().DrawPartialSprite({ 0,0 }, Renderer::GetTextureStorage().GetSprite(component.texture).get(), { 0,0 }, { 32, 32 });
			int textureIndex = (int)component.texture;
			if (ImGui::CollapsingHeader("Texture"))
			{
				if (ImGui::InputInt("Texture Index", &textureIndex))
					component.texture = (uint32_t)textureIndex;

				ImGui::DragFloat2("Tiling", &component.tiling.x);

				if (ImGui::Button("Change Texture"))
				{
					std::string filepath = GetFilePathFromExplorer();
					if (!filepath.empty())
					{
						Texture("D:\\ezram\\Projects\\CupCGE\\Editor\\assets\\example.png");
					}
				}
			}
			Vector3i color = { component.color.r, component.color.g, component.color.b };
			if (ImGui::DragInt3("Color", &color.x))
				component.color.r = color.x; component.color.g = color.y; component.color.b = color.z;
	}

	template <>
	void InspectorStyles::GetStyle<BoxColliderComponent>(BoxColliderComponent& component)
	{
		ImGui::DragFloat3("Scale", &component.scale.x);
		ImGui::Checkbox("Auto Scale", &component.autoScale);
		ImGui::Checkbox("Show Outline", &component.showOutline);
	}

	template <>
	void InspectorStyles::GetStyle<ScriptComponent>(ScriptComponent& component)
	{
	}

	template <typename T>
	const char* InspectorStyles::GetName()
	{
		return "Component";
	}

	template <>
	const char* InspectorStyles::GetName<TagComponent>()
	{
		return "Name";
	}

	template <>
	const char* InspectorStyles::GetName<TransformComponent>()
	{
		return "Transform";
	}

	template <>
	const char* InspectorStyles::GetName<CameraComponent>()
	{
		return "Camera";
	}

	template <>
	const char* InspectorStyles::GetName<MeshRendererComponent>()
	{
		return "Mesh Renderer";
	}

	template <>
	const char* InspectorStyles::GetName<BoxColliderComponent>()
	{
		return "Box Collider";
	}

	template <>
	const char* InspectorStyles::GetName<ScriptComponent>()
	{
		return "Script";
	}
}