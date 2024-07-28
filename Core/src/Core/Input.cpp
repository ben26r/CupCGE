#include "Input.h"

namespace Cup {

	CupEngine* Input::m_appPtr = nullptr;

	void Input::Init(CupEngine* appPtr)
	{
		m_appPtr = appPtr;
	}

	bool Input::GetKeyPressed(olc::Key key)
	{
		return m_appPtr->GetKey(key).bPressed;
	}

	bool Input::GetKeyDown(olc::Key key)
	{
		return m_appPtr->GetKey(key).bHeld;
	}

	bool Input::GetKeyUp(olc::Key key)
	{
		return m_appPtr->GetKey(key).bReleased;
	}

	bool Input::GetMousePressed(uint32_t button)
	{
		return m_appPtr->GetMouse(button).bPressed;
	}

	bool Input::GetMouseDown(uint32_t button)
	{
		return m_appPtr->GetMouse(button).bHeld;
	}

	bool Input::GetMouseUp(uint32_t button)
	{
		return m_appPtr->GetMouse(button).bReleased;
	}

	const Vector2i& Input::GetMousePos()
	{
		return { m_appPtr->GetMouseX(), m_appPtr->GetMouseY() };
	}

	uint32_t Input::GetMouseX()
	{
		return m_appPtr->GetMouseX();
	}

	uint32_t Input::GetMouseY()
	{
		return m_appPtr->GetMouseY();
	}
}