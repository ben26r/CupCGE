#pragma once
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

#include <functional>
#include <memory>

#include "Math/CupMath.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "LayerStack.h"
#include "Scene/Scene.h"

#include "Olc/olcPixelGameEngine.h"

namespace Cup {

	class CupEngine : public olc::PixelGameEngine
	{
    public:
        static constexpr CupEngine& Instance() { return *s_instance; }
        static inline std::shared_ptr<Scene>& ActiveScene() { return s_instance->GetActiveScene(); }
	public:
        explicit CupEngine();
        ~CupEngine() = default;

		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;
        bool OnUserImGuiRender() override;
        bool OnUserDestroy() override;

        void PushLayer(Layer* layer) { m_layerstack.PushLayer(layer); layer->OnAttach(); }

        inline std::shared_ptr<Scene>& GetActiveScene() { return m_activeScene; }
        inline void SetActiveScene(const std::shared_ptr<Scene>& scene) { m_activeScene = scene; }

        inline float GetAspectRatio() const
        {
            return (float)ScreenHeight() / (float)ScreenWidth();
        }

	private:

        LayerStack m_layerstack;
        std::shared_ptr<Scene> m_activeScene;
        static CupEngine* s_instance;
	};

    CupEngine* CreateApplication();
}