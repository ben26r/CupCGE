#pragma once
#include <string>

#include "Math/CupMath.h"
#include "Graphics/Camera.h"
#include "Olc/olcPixelGameEngine.h"

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

	struct Material
	{
		//Color
		olc::Sprite sprite;
	};

	struct MeshComponent
	{
		Meshf mesh;
		Material material;

		MeshComponent() = default;
		explicit MeshComponent(const Meshf& _mesh)
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

	using ComponentTypes = std::tuple<TagComponent, TransformComponent, CameraComponent>;
}