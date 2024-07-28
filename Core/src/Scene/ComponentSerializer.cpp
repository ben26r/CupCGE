#include "ComponentSerializer.h"

#include "Components.h"
#include "Graphics/Texture.h"
#include "Script/ScriptRegistry.h"
#include "Core/CupEngine.h"

namespace Cup {


	template <typename T>
	void ComponentSerializer::Serialize(nlohmann::json& file, const T& component)
	{
	}

	template <typename T>
	void ComponentSerializer::Deserialize(const nlohmann::json& file, const T& component, Registry& registry, Entity entity)
	{
	}

	template <>
	void ComponentSerializer::Serialize<TagComponent>(nlohmann::json& file, const TagComponent& component)
	{
		file["TagComponent"] = { {"tag", component.tag } };
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
	void ComponentSerializer::Serialize<TransformComponent>(nlohmann::json& file, const TransformComponent& component)
	{
		file["TransformComponent"] = {
			{"position", { component.position.x, component.position.y, component.position.z }},
			{"rotation", { component.rotation.x, component.rotation.y, component.rotation.z }},
			{"scale", { component.scale.x, component.scale.y, component.scale.z }}
		};
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
	void ComponentSerializer::Serialize<MeshRendererComponent>(nlohmann::json& file, const MeshRendererComponent& component)
	{
		file["MeshComponent"] = 
		{
			{ "filepath", component.mesh.modelFilepath },
			{ "texture", component.texture },
			{ "tiling", { component.tiling.x, component.tiling.y }},
			{ "color", { component.color.r, component.color.g, component.color.b, component.color.a }}
		};
	}
	template <>
	void ComponentSerializer::Deserialize<MeshRendererComponent>(const nlohmann::json& file, const MeshRendererComponent& component, Registry& registry, Entity entity)
	{
		if (file.contains("MeshComponent"))
		{
			Meshf mesh;
			std::string& filepath = file.at("MeshComponent").at("filepath").get<std::string>();
			if (!filepath.empty())
				mesh.LoadModel(filepath);
			else
				mesh.CreateCube();
			auto color = file.at("MeshComponent").at("color");
			auto tiling = file.at("MeshComponent").at("tiling");

			uint32_t textureIndex = file.at("MeshComponent").at("texture").get<uint32_t>();
			if (!Renderer::GetTextureStorage().HasTexture(textureIndex))
				textureIndex = 0;
			registry.AddComponent<MeshRendererComponent>(entity, mesh, textureIndex, 
				olc::Pixel(color[0], color[1], color[2], color[3]), Vector2f(tiling[0], tiling[1]));
		}
	}

	template <>
	void ComponentSerializer::Serialize<SpriteRendererComponent>(nlohmann::json& file, const SpriteRendererComponent& component)
	{
		file["SpriteComponent"] =
		{
			{ "texture", component.texture },
			{ "tiling", { component.tiling.x, component.tiling.y }},
			{ "color", { component.color.r, component.color.g, component.color.b, component.color.a }}
		};
	}
	template <>
	void ComponentSerializer::Deserialize<SpriteRendererComponent>(const nlohmann::json& file, const SpriteRendererComponent& component, Registry& registry, Entity entity)
	{
		if (file.contains("SpriteComponent"))
		{
			auto color = file.at("SpriteComponent").at("color");
			auto tiling = file.at("SpriteComponent").at("tiling");

			uint32_t textureIndex = file.at("SpriteComponent").at("texture").get<uint32_t>();
			if (!Renderer::GetTextureStorage().HasTexture(textureIndex))
				textureIndex = 0;
			registry.AddComponent<SpriteRendererComponent>(entity, textureIndex,
				olc::Pixel(color[0], color[1], color[2], color[3]), Vector2f(tiling[0], tiling[1]));
		}
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
			{"lookDir", { component.camera->m_lookDir.x, component.camera->m_lookDir.y, component.camera->m_lookDir.z }},
			{"yaw", component.camera->m_yaw}
		};
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
			auto lookDir = file.at("CameraComponent").at("lookDir");
			auto yaw = file.at("CameraComponent").at("yaw").get<float>();

			auto camera = std::make_shared<Camera>(aspectRatio);
			camera->cnear = cnear;
			camera->cfar = cfar;
			camera->fov = fov;
			camera->aspectRatio = aspectRatio;
			camera->m_lookDir = Vector3f(lookDir[0], lookDir[1], lookDir[2]);
			camera->m_yaw = yaw;

			registry.AddComponent<CameraComponent>(entity, camera, mainCamera);
		}
	}

	template <>
	void ComponentSerializer::Serialize<BoxColliderComponent>(nlohmann::json& file, const BoxColliderComponent& component)
	{
		file["BoxColliderComponent"] = { 
			{"scale", { component.scale.x, component.scale.y, component.scale.z } }, 
			{"autoScale", component.autoScale }
		};
	}
	template <>
	void ComponentSerializer::Deserialize<BoxColliderComponent>(const nlohmann::json& file, const BoxColliderComponent& component, Registry& registry, Entity entity)
	{
		if (file.contains("BoxColliderComponent"))
		{
			auto scale = file.at("BoxColliderComponent").at("scale");
			//bool autoScale = file.at("BoxColliderComponent").at("autoScale").get<bool>();
			registry.AddComponent<BoxColliderComponent>(entity, Vector3f(scale[0], scale[1], scale[2]));
		}
	}

	template <>
	void ComponentSerializer::Serialize<MeshColliderComponent>(nlohmann::json& file, const MeshColliderComponent& component)
	{
		file["MeshColliderComponent"] = { };
	}

	template <>
	void ComponentSerializer::Deserialize<MeshColliderComponent>(const nlohmann::json& file, const MeshColliderComponent& component, Registry& registry, Entity entity)
	{
		if (file.contains("MeshColliderComponent"))
		{
			registry.AddComponent<MeshColliderComponent>(entity);
		}
	}

	template <>
	void ComponentSerializer::Serialize<ScriptComponent>(nlohmann::json& file, const ScriptComponent& component)
	{
		file["NativeScriptComponent"] = {
			{"scriptType", component.scriptType}
		};
	}

	template <>
	void ComponentSerializer::Deserialize<ScriptComponent>(const nlohmann::json& file, const ScriptComponent& component, Registry& registry, Entity entity)
	{
		if (file.contains("NativeScriptComponent"))
		{
			std::string scriptType = file.at("NativeScriptComponent").at("scriptType").get<std::string>();
			ScriptComponent nsc;
			nsc.instance = ScriptRegistry::Instance().CreateScript(scriptType);
			
			CUP_ASSERT_FUNC(nsc.instance, return, "Failed to create script!");

			nsc.instance->Init(CupEngine::ActiveScene().get(), entity);
			nsc.scriptType = scriptType;
			registry.AddComponent<ScriptComponent>(entity, std::move(nsc));
		}
	}

	template <>
	void ComponentSerializer::Serialize<SpriteAnimatorComponent>(nlohmann::json& file, const SpriteAnimatorComponent& component)
	{
		file["SpriteAnimatorComponent"] = { };
	}

	template <>
	void ComponentSerializer::Deserialize<SpriteAnimatorComponent>(const nlohmann::json& file, const SpriteAnimatorComponent& component, Registry& registry, Entity entity)
	{
		if (file.contains("SpriteAnimatorComponent"))
		{
			registry.AddComponent<SpriteAnimatorComponent>(entity);
		}
	}
}