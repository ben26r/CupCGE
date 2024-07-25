#pragma once

#include "Core/Base.h"

#include <string>

namespace Cup {
	
	struct TextureProps
	{
		uint32_t width;
		uint32_t height;

		TextureProps(uint32_t _width = 1, uint32_t _height = 1) : width(_width), height(_height) { }
	};

	class Texture
	{
	public:
		Texture() = default;
		//Texture(const TextureProps& props);
		explicit Texture(const std::string& _filepath, const TextureProps& props = TextureProps());
		~Texture() = default;

		inline uint32_t GetIndex() const { return m_rendererID; }
	private:
		uint32_t m_rendererID = 0;
		const std::string filepath;
	};

}