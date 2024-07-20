#include "ComponentSerializer.h"

#include "Components.h"

namespace Cup {


	template <typename T>
	void ComponentSerializer::Serialize(nlohmann::json& file, const T& component)
	{
	}

	template <>
	void ComponentSerializer::Serialize<TagComponent>(nlohmann::json& file, const TagComponent& component)
	{
		file["TagComponent"] = { {"tag", component.tag } };
	}

	template <>
	void ComponentSerializer::Serialize<TransformComponent>(nlohmann::json& file, const TransformComponent& component)
	{
		file["TransformComponent"] = {
			{"position", { component.position.x, component.position.y, component.position.z }},
			{"rotation", { component.rotation.x, component.rotation.y, component.rotation.z }},
			{"scale", { component.scale.x, component.scale.y, component.scale.z }}
		};
	}

	template <>
	void ComponentSerializer::Serialize<MeshRendererComponent>(nlohmann::json& file, const MeshRendererComponent& component)
	{
		file["MeshComponent"] = {};
	}

	template <>
	void ComponentSerializer::Serialize<CameraComponent>(nlohmann::json& file, const CameraComponent& component)
	{
		file["CameraComponent"] = {
			{"mainCamera", component.mainCamera},
			{"cnear", component.camera->cnear},
			{"cfar", component.camera->cfar},
			{"fov", component.camera->fov},
			{"aspectRatio", component.camera->aspectRatio},
			{"position", { component.camera->m_position.x, component.camera->m_position.y, component.camera->m_position.z }},
			{"lookDir", { component.camera->m_lookDir.x, component.camera->m_lookDir.y, component.camera->m_lookDir.z }},
			{"yaw", component.camera->m_yaw}
		};
	}

	template <typename T>
	void ComponentSerializer::Deserialize(const nlohmann::json& file, const T& component, Registry& registry, Entity entity)
	{
	}

	template <>
	void ComponentSerializer::Deserialize<TagComponent>(const nlohmann::json& file, const TagComponent& component, Registry& registry, Entity entity)
	{
		if (file.contains("TagComponent"))
		{
			auto tag = file["TagComponent"]["tag"].get<std::string>();
			registry.AddComponent<TagComponent>(entity, tag);
		}
	}

	template <>
	void ComponentSerializer::Deserialize<TransformComponent>(const nlohmann::json& file, const TransformComponent& component, Registry& registry, Entity entity)
	{
		if (file.contains("TransformComponent"))
		{
			auto pos = file.at("TransformComponent").at("position");
			auto rot = file.at("TransformComponent").at("rotation");
			auto scale = file.at("TransformComponent").at("scale");
			registry.AddComponent<TransformComponent>(entity,
				Vector3f(pos[0], pos[1], pos[2]),
				Vector3f(rot[0], rot[1], rot[2]),
				Vector3f(scale[0], scale[1], scale[2])
			);
		}
	}

	template <>
	void ComponentSerializer::Deserialize<MeshRendererComponent>(const nlohmann::json& file, const MeshRendererComponent& component, Registry& registry, Entity entity)
	{
		if (file.contains("MeshComponent"))
		{
			Meshf mesh;
			mesh.CreateCube();
			registry.AddComponent<MeshRendererComponent>(entity, mesh);
		}
	}

	template <>
	void ComponentSerializer::Deserialize<CameraComponent>(const nlohmann::json& file, const CameraComponent& component, Registry& registry, Entity entity)
	{
		if (file.contains("CameraComponent")) {
			bool mainCamera = file.at("CameraComponent").at("mainCamera").get<bool>();
			auto cnear = file.at("CameraComponent").at("cnear").get<float>();
			auto cfar = file.at("CameraComponent").at("cfar").get<float>();
			auto fov = file.at("CameraComponent").at("fov").get<float>();
			auto aspectRatio = file.at("CameraComponent").at("aspectRatio").get<float>();
			auto pos = file.at("CameraComponent").at("position");
			auto lookDir = file.at("CameraComponent").at("lookDir");
			auto yaw = file.at("CameraComponent").at("yaw").get<float>();

			auto camera = std::make_shared<Camera>(aspectRatio);
			camera->cnear = cnear;
			camera->cfar = cfar;
			camera->fov = fov;
			camera->aspectRatio = aspectRatio;
			camera->SetPosition(Vector3f(pos[0], pos[1], pos[2]));
			camera->m_lookDir = Vector3f(lookDir[0], lookDir[1], lookDir[2]);
			camera->m_yaw = yaw;
			camera->RecalulateView(); // Ensure the view matrix is recalculated after setting all properties

			registry.AddComponent<CameraComponent>(entity, camera, mainCamera);
		}
	}
}