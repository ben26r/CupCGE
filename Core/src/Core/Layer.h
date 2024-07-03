#pragma once

#include <string>


namespace Cup {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer") : m_debugName(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltatime) {}
		//virtual void OnImGuiRender() {}
		//virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_debugName; }
	protected:
		std::string m_debugName;
	};

}