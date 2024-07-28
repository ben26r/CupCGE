#include "SpriteAnimator.h"

namespace Cup {

	void SpriteAnimator::Tick(float deltatime)
	{
		if (m_clips.empty())
			return;

		if (!playing)
			return;

		if (m_currentDuration < m_currentClip->duration)
		{
			uint32_t currentFrame = m_currentDuration / (m_currentClip->duration / m_currentClip->GetFrameAmount());
			m_currentTexture = m_currentClip->GetFrame(currentFrame);
			m_currentDuration += deltatime;
			return;
		}
		
		if (m_currentClip->loop)
			m_currentDuration = 0;
		else
			playing = false;
	}

}