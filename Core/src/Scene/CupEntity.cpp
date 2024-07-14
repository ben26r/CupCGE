#include "CupEntity.h"

namespace Cup {

	CupEntity::CupEntity(Registry* _registry)
		: m_registry(_registry)
	{
		m_entity = _registry->CreateEntity();
	}

}