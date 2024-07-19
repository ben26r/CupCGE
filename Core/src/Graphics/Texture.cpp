#include "Texture.h"

#include "Renderer.h"

namespace Cup {

	Texture& Texture::Create(const std::string& filepath, const TextureProps& props)
	{
		Texture texture;
		texture.m_rendererID = Renderer::CreateTexture(filepath);
		return texture;
	}

}