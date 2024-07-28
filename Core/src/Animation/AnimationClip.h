#pragma once

#include <string>
#include <vector>
#include "Core/Base.h"


namespace Cup {

	// Change order of frames

	class AnimationClip
	{
	public:
		AnimationClip(const std::string& _name = "default", float _duration = 1.0f, bool _loop = true)
			: name(_name), duration(_duration), loop(_loop) {}

		inline void AddFrame(uint32_t texture) { m_textureFrames.push_back(texture); }
		inline void RemoveFrame(uint32_t frame) { CUP_ASSERT_FUNC(frame < m_textureFrames.size(), return, "Invaild frame index!!"); m_textureFrames.erase(m_textureFrames.begin() + frame); }

		inline uint32_t GetFrameAmount() const { return m_textureFrames.size(); }
		inline uint32_t GetFrame(uint32_t index) const { return m_textureFrames[index]; }
		inline const std::vector<uint32_t>& GetTextures() const { return m_textureFrames; }
	public:
		float duration;
		bool loop;
		std::string name;
	private:
		std::vector<uint32_t> m_textureFrames;
	};

}