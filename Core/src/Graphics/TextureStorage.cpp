#include "TextureStorage.h"

#include "Core/CupEngine.h"
#include "Scene/Components.h"
#include "json.hpp"

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
		auto& it = m_fileTextureMap.find(filepath);
		if (it != m_fileTextureMap.end())
			return it->second;
		m_storage.push_back(std::make_shared<olc::Sprite>(filepath));
		m_fileTextureMap.emplace(filepath, m_currentIndex);
		return m_currentIndex++;
	}

	void TextureStorage::DeleteTexture()
	{

	}

	void TextureStorage::SetTextureProps(uint32_t index, const olc::Sprite::Mode& props)
	{
		m_storage[index]->SetSampleMode(props);
	}

	void TextureStorage::Serialize(const std::string& filepath) const
	{
		nlohmann::json texturePack;

		for (const auto& pair : m_fileTextureMap)
		{
			nlohmann::json texture;
			texture["filepath"] = pair.first;
		//D:\ezram\Projects\CupCGE\Editor\assets\example.png
			texturePack["textures"].push_back(texture);
		}
		std::ofstream file(filepath);
		if (file.is_open())
		{
			file << texturePack.dump(4);
			file.close();
		}
		else
		{
			std::cerr << "Failed to open file for writing: " << filepath << std::endl;
		}
	}

	void TextureStorage::Deserialize(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (file.is_open())
		{
			nlohmann::json texturePack;
			file >> texturePack;
			file.close();

			for (const auto& texture : texturePack["textures"])
			{
				std::string textureFilePath = texture["filepath"].get<std::string>();
				if (!textureFilePath.empty())
				{
					CreateTexture(textureFilePath);
				}
			}
		}
		else
		{
			std::cerr << "Failed to open file for reading: " << filepath << std::endl;
		}
	}
}