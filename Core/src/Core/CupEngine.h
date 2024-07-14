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
        static inline Scene& InsScene() { return s_instance->GetScene(); }
	public:
        explicit CupEngine();
        ~CupEngine() = default;

		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;
        bool OnUserRender() override;

        void PushLayer(Layer* layer) { m_layerstack.PushLayer(layer); layer->OnAttach(); }

        inline Scene& GetScene() { return m_scene; }
        inline float GetAspectRatio() const
        {
            return (float)ScreenHeight() / (float)ScreenWidth();
        }

	private:

        LayerStack m_layerstack;
        Scene m_scene;
        static CupEngine* s_instance;
	};

    CupEngine* CreateApplication();
}