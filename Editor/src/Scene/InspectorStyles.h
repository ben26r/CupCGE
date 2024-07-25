#pragma once
#include "Cup.h"

#include <string>

namespace Cup {

	class InspectorStyles
	{
	public:
		template <typename T>
		static void GetStyle(T& component);

		template <typename T>
		inline static const char* GetName();
	};

}