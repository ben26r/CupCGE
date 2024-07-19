#include "InspectorStyles.h"
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
		ImGui::Text("Transform");

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
		ImGui::Text("Camera");
		ImGui::DragFloat("Near", &component.camera->cnear, 0.1f);
		ImGui::DragFloat("Far", &component.camera->cfar, 0.1f);
		ImGui::DragFloat("Aspect Ratio", &component.camera->aspectRatio, 0.1f);
		ImGui::DragFloat("Fov", &component.camera->fov, 0.1f);
		ImGui::DragFloat3("Position", &component.camera->m_position.x, 0.1f);
	}

	template <>
	void InspectorStyles::GetStyle<MeshComponent>(MeshComponent& component)
	{
	}
}