#include "Texture.h"

#include "Renderer.h"

namespace Cup {

	//Texture::Texture(const TextureProps& props)
	//	: m_rendererID(Renderer::CreateTexture(props)), filepath("")
	//{

	//}

	Texture::Texture(const std::string& _filepath, const TextureProps& props)
		: m_rendererID(Renderer::CreateTexture(_filepath)), filepath(_filepath)
	{

	}

}