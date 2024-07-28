#pragma once
#include <string>
#include <functional>

#include "Olc/olcPixelGameEngine.h"
#include "Math/CupMath.h"

#include "Graphics/Camera.h"
#include "Graphics/Texture.h"

#include "ScriptableEntity.h"
#include "Scene.h"

#include "Animation/SpriteAnimator.h"

namespace Cup {

	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		explicit TagComponent(const std::string& _tag) : tag(_tag) { }

		bool operator!=(const TagComponent& other) const { return tag != other.tag; }
	};

	struct TransformComponent
	{
		Vector3f position;
		Vector3f rotation;
		Vector3f scale;

		TransformComponent(const Vector3f& _position = Vector3f(), const Vector3f _rotation = Vector3f(), const Vector3f _scale = Vector3f(1.0f, 1.0f, 1.0f))
			: position(_position), rotation(_rotation), scale(_scale) { }
		inline const Matrix4x4f GetTransform() const
		{
			// Apply scale, then rotate, then translate
			Matrix4x4f transform = Matrix4x4f::Identity();
			Vector3f pos = position;
			pos -= scale * 0.5f;
			transform = Matrix4x4f::Translation(pos) * transform;
			transform = Matrix4x4f::Rotation(Vector3f::Far(), rotation.z) * transform;
			transform = Matrix4x4f::Rotation(Vector3f::Up(), rotation.y) * transform;
			transform = Matrix4x4f::Rotation(Vector3f::Right(), rotation.x) * transform;
			transform = Matrix4x4f::Scale(scale) * transform;

			return transform;
		}

		inline const Matrix4x4f GetRotation() const
		{
			return Matrix4x4f::Rotation(Vector3f::Right(), rotation.x) * Matrix4x4f::Rotation(Vector3f::Up(), rotation.y) * Matrix4x4f::Rotation(Vector3f::Far(), rotation.z);
		}

		// unimplemented
		bool operator!=(const TransformComponent& other) const { return true; }

		TransformComponent& operator-(const TransformComponent& other) const { return TransformComponent(position - other.position, rotation - other.rotation, scale - other.scale); }
		TransformComponent& operator+=(const TransformComponent& other) { position += other.position; rotation += other.rotation; scale += other.scale; return *this; }
	};

	struct MeshRendererComponent
	{
		Meshf mesh;
		uint32_t texture = 0;
		Vector2f tiling = { 1, 1 };
		olc::Pixel color;
		bool wired = false;

		MeshRendererComponent() = default;
		explicit MeshRendererComponent(const Meshf& _mesh, uint32_t _texture = 0, olc::Pixel _color = { 255, 255, 255 }, Vector2f _tiling = {1,1})
			: mesh(_mesh), texture(_texture), color(_color), tiling(_tiling)
		{ 
		
		}

		bool operator!=(const MeshRendererComponent& other) const { return texture != other.texture && color != other.color; }
	};

	struct SpriteRendererComponent
	{
		uint32_t texture = 0;
		Vector2f tiling = { 1, 1 };
		olc::Pixel color = { 255, 255, 255 };
		bool wired = false;

		SpriteRendererComponent() = default;
		explicit SpriteRendererComponent(uint32_t _texture, olc::Pixel _color = { 255, 255, 255 }, Vector2f _tiling = { 1,1 })
			: texture(_texture), color(_color), tiling(_tiling)
		{

		}

		// doesn't account for tiling
		bool operator!=(const SpriteRendererComponent& other) const { return texture != other.texture && color != other.color; }
	};

	struct CameraComponent
	{
		std::shared_ptr<Camera> camera;
		bool mainCamera = false;

		CameraComponent() = default;
		explicit CameraComponent(const std::shared_ptr<Camera>& _camera, bool _mainCamera = false) : camera(_camera), mainCamera(_mainCamera)  { }

		// unimplemented
		bool operator!=(const CameraComponent& other) const { return true; }
	};

	struct ScriptComponent
	{
		ScriptableEntity* instance = nullptr;
		bool started = false;
		std::string scriptType;

		template<typename T>
		void Bind(std::shared_ptr<Scene>& scene, Entity entity)
		{
			instance = static_cast<ScriptableEntity*>(new T());
			instance->Init(scene, entity);
			scriptType = typeid(T).name();
		}

		void Delete()
		{
			instance->OnDestroy();
			delete instance;
			instance = nullptr;
		}

		bool operator!=(const CameraComponent& other) const { return true; }
	};


	struct BoxColliderComponent
	{
		Vector3f scale;
		Vector3f origin;
		Vector3f transformScale;
		bool autoScale = true;
		bool showOutline = false;

		BoxColliderComponent() = default;
		explicit BoxColliderComponent(const Vector3f& _scale, bool _autoScale = true) : scale(_scale), autoScale(_autoScale) {}

		bool operator!=(const BoxColliderComponent& other) const { return scale.x != other.scale.x || scale.y != other.scale.y || scale.z != other.scale.z; }
	};

	struct MeshColliderComponent
	{

		MeshColliderComponent() = default;

		bool operator!=(const MeshColliderComponent& other) const { return true; }
	};

	struct SpriteAnimatorComponent
	{
		SpriteAnimator animator;

		SpriteAnimatorComponent() = default;
	};

	using ComponentTypes = std::tuple<TagComponent, TransformComponent, CameraComponent, MeshRendererComponent, BoxColliderComponent, MeshColliderComponent, ScriptComponent, SpriteRendererComponent,
	SpriteAnimatorComponent>;
}