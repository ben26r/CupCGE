#pragma once

#include "Core/Base.h"

#include "Texture.h"
#include "Olc/olcPixelGameEngine.h"

#include <vector>
#include <string>
#include <unordered_map>

namespace Cup {

	class TextureStorage
	{
	public:
		TextureStorage();
		~TextureStorage() = default;

		uint32_t CreateTexture(uint32_t width, uint32_t height, olc::Pixel color = olc::Pixel(255, 255, 255));
		uint32_t CreateTexture(const std::string& filepath);
		inline bool HasTexture(uint32_t index) const { return index < m_currentIndex; }
		void DeleteTexture();

		void SetTextureProps(uint32_t index, const olc::Sprite::Mode& props);

		void Serialize(const std::string& filepath) const;
		void Deserialize(const std::string& filepath);
		void Clear();

		inline const std::shared_ptr<olc::Sprite>& GetSprite(uint32_t index) const { CUP_ASSERT(index < m_currentIndex, "Invaild Texture Index!"); return m_storage[index]; }

	private:
		uint32_t m_currentIndex = 0;

		std::vector<std::shared_ptr<olc::Sprite>> m_storage;
		std::vector<std::string> m_fileTextureMap;
	};

}