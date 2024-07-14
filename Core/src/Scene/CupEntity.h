#pragma once
#include <vector>

#include "Core/Base.h"
#include "Register.h"

namespace Cup {

	class CupEntity
	{
	public:

		CupEntity() = default;
		explicit CupEntity(Registry* _registry);

		template<typename T, typename ... Args>
		inline void AddComponent(Args&&... args)
		{
			m_registry->AddComponent<T>(m_entity, std::forward<Args>(args)...);
		}

		template<typename T>
		inline T& GetComponent()
		{
			return m_registry->GetComponent<T>(m_entity);
		}

		template<typename T>
		inline bool HasComponent()
		{
			return m_registry->HasComponent<T>(m_entity);
		}
	private:
		Registry* m_registry;
		Entity m_entity;
	};

}