#pragma once

#include "Core/Base.h"
#include "Register.h"
#include "json.hpp"

namespace Cup {

	class ComponentSerializer
	{
	public:
		template <typename T>
		static void Serialize(nlohmann::json& file, const T& component);

		template <typename T>
		static void Deserialize(const nlohmann::json& file, const T& component, Registry& registry, Entity entity);
	};

}