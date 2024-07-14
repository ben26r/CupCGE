#pragma once
#include "Core/Base.h"
#include "Register.h"
#include "CupEntity.h"

namespace Cup {

	class Scene
	{
	public:
		explicit Scene();
		~Scene();

		inline CupEntity& CreateEntity() { return CupEntity(&m_registry); }
		void Update(float deltatime);

	private:
		Registry m_registry;
	};

}
