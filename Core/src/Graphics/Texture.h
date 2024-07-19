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
		//static Texture& Create(const TextureProps& props = TextureProps());
		static Texture& Create(const std::string& filepath, const TextureProps& props = TextureProps());
		//void Bind();
	private:
		uint32_t m_rendererID;
	};

}