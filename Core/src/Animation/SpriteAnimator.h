#pragma once

#include <unordered_map>
#include "Core/Base.h"

#include "AnimationClip.h"

namespace Cup {


	/*
	- Basic frame by frame animator
	- Ability to change frame order and length
	- Multiple Animation States each with unique frames
	*/

	class SpriteAnimator
	{
	public:

		inline void AddClip(const AnimationClip& clip) { m_clips[clip.name] = clip; }
		inline void RemoveClip(const std::string& name) { CUP_ASSERT_FUNC(m_clips.find(name) != m_clips.end(), return, "Invaild clip name!"); m_clips.erase(name); }

		inline void Play(const std::string& name) { m_currentClip = &m_clips[name]; m_currentDuration = 0; playing = true; }
		inline void PlayAtCurrent(const std::string& name) { m_currentClip = &m_clips[name]; playing = true; }
		inline void Stop() { playing = false; }

		void Tick(float deltatime);
		inline uint32_t GetTexture() const { return m_currentTexture; }
		inline std::unordered_map<std::string, AnimationClip>& GetClips() { return m_clips; }

	private:
		//float framerate;
		std::unordered_map<std::string, AnimationClip> m_clips;

		AnimationClip* m_currentClip = nullptr;
		float m_currentDuration = 0;
		uint32_t m_currentTexture = 0;

		bool playing = false;
		bool loop = true;
	};

}