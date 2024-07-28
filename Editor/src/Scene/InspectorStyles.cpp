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
			ImGui::Checkbox("Main Camera", &component.mainCamera);
	}


	static std::string GetFilePathFromExplorer() {
		return std::string();
	}

	template <>
	void InspectorStyles::GetStyle<MeshRendererComponent>(MeshRendererComponent& component)
	{
			CupEngine::Instance().DrawPartialSprite({ 0,0 }, Renderer::GetTextureStorage().GetSprite(component.texture).get(), { 0,0 }, { 32, 32 });
			int textureIndex = (int)component.texture;

			ImGui::Button("Model");
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path modelPath(path);
					component.mesh.LoadModel(modelPath.string());
				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::CollapsingHeader("Texture"))
			{
				ImGui::Button("Texture");
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath(path);
						component.texture = Texture(texturePath.string()).GetIndex();
					}
					ImGui::EndDragDropTarget();
				}

				if (ImGui::InputInt("Texture Index", &textureIndex))
					component.texture = (uint32_t)textureIndex;

				ImGui::DragFloat2("Tiling", &component.tiling.x);
			}
			Vector3i color = { component.color.r, component.color.g, component.color.b };
			if (ImGui::DragInt3("Color", &color.x))
				component.color.r = color.x; component.color.g = color.y; component.color.b = color.z;
	}

	template <>
	void InspectorStyles::GetStyle<SpriteRendererComponent>(SpriteRendererComponent& component)
	{
		CupEngine::Instance().DrawPartialSprite({ 0,0 }, Renderer::GetTextureStorage().GetSprite(component.texture).get(), { 0,0 }, { 32, 32 });
		int textureIndex = (int)component.texture;

		ImGui::Button("Texture");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path texturePath(path);
				component.texture = Texture(texturePath.string()).GetIndex();
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::InputInt("Texture Index", &textureIndex))
			component.texture = (uint32_t)textureIndex;

		ImGui::DragFloat2("Tiling", &component.tiling.x);
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
	void InspectorStyles::GetStyle<MeshColliderComponent>(MeshColliderComponent& component)
	{
	}

	template <>
	void InspectorStyles::GetStyle<ScriptComponent>(ScriptComponent& component)
	{
	}

	template <>
	void InspectorStyles::GetStyle<SpriteAnimatorComponent>(SpriteAnimatorComponent& component)
	{
		if (ImGui::Button("Add Animation Clip"))
			component.animator.AddClip(AnimationClip());

		for (auto& clip : component.animator.GetClips())
		{
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), clip.second.name.c_str());
			if (ImGui::InputText("Clip Name", buffer, sizeof(buffer)))
				clip.second.name = std::string(buffer);
			if (ImGui::CollapsingHeader(clip.second.name.c_str()))
			{
				if (ImGui::Button("Play"))
				{
					component.animator.Play(clip.second.name);
				}

				ImGui::DragFloat("Duration", &clip.second.duration);

				ImGui::Button("Add Frame");
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath(path);
						clip.second.AddFrame(Texture(texturePath.string()).GetIndex());
					}
					ImGui::EndDragDropTarget();
				}

				for (int i = 0; i < clip.second.GetFrameAmount(); i++)
				{
					if (ImGui::Button("Remove Frame"))
						clip.second.RemoveFrame(i);
				}
			}
		}
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
	const char* InspectorStyles::GetName<SpriteRendererComponent>()
	{
		return "Sprite Renderer";
	}

	template <>
	const char* InspectorStyles::GetName<SpriteAnimatorComponent>()
	{
		return "Sprite Animator";
	}

	template <>
	const char* InspectorStyles::GetName<BoxColliderComponent>()
	{
		return "Box Collider";
	}

	template <>
	const char* InspectorStyles::GetName<MeshColliderComponent>()
	{
		return "Mesh Collider";
	}

	template <>
	const char* InspectorStyles::GetName<ScriptComponent>()
	{
		return "Script";
	}
}