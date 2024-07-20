#pragma once

#include "Core/Base.h"

#include <string>

namespace Cup {
	
	struct TextureProps
	{
		uint32_t width = 1;
		uint32_t height = 1;

	};

	class Texture
	{
	public:
		Texture() = default;
		explicit Texture(const std::string& _filepath, const TextureProps& props = TextureProps());
		~Texture() = default;

		inline uint32_t GetIndex() const { return m_rendererID; }
	private:
		uint32_t m_rendererID = 0;
		const std::string filepath;
	};

}