#include "CupEntity.h"

#include "Scene.h"

namespace Cup {

	CupEntity::CupEntity(std::shared_ptr<Scene>& _scene, const std::string& tag)
		: m_scene(_scene)
	{
		m_entity = _scene->m_registry.CreateEntity();
		_scene->m_registry.AddComponent<TagComponent>(m_entity, tag);
	}

}