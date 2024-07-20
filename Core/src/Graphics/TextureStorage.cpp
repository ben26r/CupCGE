#include "TextureStorage.h"

namespace Cup {

	TextureStorage::TextureStorage()
	{
		CreateTexture(1, 1);
	}

	uint32_t TextureStorage::CreateTexture(uint32_t width, uint32_t height, olc::Pixel color)
	{
		std::shared_ptr<olc::Sprite> sprite = std::make_shared<olc::Sprite>(width, height);

        for (int x = 0; x < sprite->width; x++)
        {
            for (int y = 0; y < sprite->height; y++)
            {
                sprite->SetPixel(x, y, color);
            }
        }

		m_storage.push_back(sprite);
		return m_currentIndex++;
	}

	uint32_t TextureStorage::CreateTexture(const std::string& filepath)
	{
		m_storage.push_back(std::make_shared<olc::Sprite>(filepath));
		m_fileTextureMap.emplace(filepath, m_currentIndex);
		return m_currentIndex++;
	}

	void TextureStorage::DeleteTexture()
	{

	}

	void TextureStorage::SetTextureProps(const TextureProps& props)
	{

	}
}