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

#include "Olc/olcPixelGameEngine.h"

namespace Cup {

	class CupEngine : public olc::PixelGameEngine
	{
    public:
        static CupEngine& Instance() { return *s_instance; }
	public:
        CupEngine(); // shoud be explicit
        ~CupEngine() = default;

		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;

        void PushLayer(Layer* layer) { m_layerstack.PushLayer(layer); layer->OnAttach(); }

        inline float GetAspectRatio() const
        {
            return (float)ScreenHeight() / (float)ScreenWidth();
        }
	private:
        template <typename T>
		void DrawCupTriangle(const Triangle<T>& triangle, const Vector4& color);

        LayerStack m_layerstack;
        static CupEngine* s_instance;
	};

    template <typename T>
    void CupEngine::DrawCupTriangle(const Triangle<T>& triangle, const Vector4& color)
    {
        FillTriangle(triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, olc::Pixel(color.x, color.y, color.z, 255.0f));
        //DrawTriangle(triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, olc::Pixel(0.0f, 0.0f, 0.0f, 255.0f));
    }

    CupEngine* CreateApplication();
}