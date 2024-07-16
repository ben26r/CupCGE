#pragma once
#include "Cup.h"

namespace Cup {

	class InspectorStyles
	{
	public:
		template <typename T>
		static void GetStyle(T& component);
	};

}