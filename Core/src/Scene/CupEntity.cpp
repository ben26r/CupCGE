#include "CupEntity.h"

#include "Scene.h"

namespace Cup {

	CupEntity::CupEntity(Entity entity, Scene* _scene)
		: m_entity(entity), m_scene(_scene)
	{
	}

}