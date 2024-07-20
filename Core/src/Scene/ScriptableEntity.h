#pragma once

#include "CupEntity.h"

namespace Cup {

	class ScriptableEntity : public CupEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		inline virtual void Init(std::shared_ptr<Scene>& scene, Entity entity) { m_scene = scene; m_entity = entity; }

		virtual void Start() {}
		virtual void Update(float deltatime) {}
		virtual void OnDestroy() {}
	};

}