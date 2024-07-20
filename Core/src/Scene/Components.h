#pragma once
#include <string>

#include "Math/CupMath.h"
#include "Graphics/Camera.h"
#include "Olc/olcPixelGameEngine.h"
#include "Graphics/Texture.h"
#include "ScriptableEntity.h"
#include "Scene.h"
#include <functional>

namespace Cup {

	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		explicit TagComponent(const std::string& _tag) : tag(_tag) { }
	};

	struct TransformComponent
	{
		Vector3f position;
		Vector3f rotation;
		Vector3f scale;

		TransformComponent() = default;
		explicit TransformComponent(const Vector3f& _position, const Vector3f _rotation = Vector3f(), const Vector3f _scale = Vector3f(1.0f, 1.0f, 1.0f))
			: position(_position), rotation(_rotation), scale(_scale) { }

		inline const Matrix4x4f GetTransform() const 
		{ 
			Matrix4x4f r = Matrix4x4f::Rotation(Vector3f::Right(), rotation.x) * Matrix4x4f::Rotation(Vector3f::Up(), rotation.y) * Matrix4x4f::Rotation(Vector3f::Far(), rotation.z);
			return Matrix4x4f::Scale(scale) * r * Matrix4x4f::Translation(position);
		}
	};

	struct MeshRendererComponent
	{
		Meshf mesh;
		uint32_t texture = 0;
		olc::Pixel color = { 255, 255, 255 };

		MeshRendererComponent() = default;
		explicit MeshRendererComponent(const Meshf& _mesh)
			: mesh(_mesh)
		{ 
		
		}
	};

	struct CameraComponent
	{
		std::shared_ptr<Camera> camera;
		bool mainCamera = false;

		CameraComponent() = default;
		explicit CameraComponent(const std::shared_ptr<Camera>& _camera, bool _mainCamera = false) : camera(_camera), mainCamera(_mainCamera)  { }
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;
		bool started = false;

		template<typename T>
		void Bind(std::shared_ptr<Scene>& scene, Entity entity)
		{
			instance = static_cast<ScriptableEntity*>(new T());
			instance->Init(scene, entity);
		}

		void Delete()
		{
			delete instance;
			instance = nullptr;
		}
	};

	using ComponentTypes = std::tuple<TagComponent, TransformComponent, CameraComponent, MeshRendererComponent>;
}