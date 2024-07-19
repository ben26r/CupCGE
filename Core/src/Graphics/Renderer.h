#pragma once

#include <algorithm>
#include <list>
#include <functional>

#include "Core/Base.h"
#include "Camera.h"
#include "Math/CupMath.h"
#include "Olc/olcPixelGameEngine.h"
#include "Texture.h"
#include <array>

namespace Cup {

	class Renderer
	{
	public:

		static bool Init(olc::PixelGameEngine* appPtr);

		static void Submit(const Matrix4x4f& matrix, const Meshf& mesh, olc::Sprite* sprite);
		static uint32_t CreateTexture(const std::string& filepath, const TextureProps& props = TextureProps());

        static void Sort();
		static void Start(const std::shared_ptr<Camera>& camera);
		static void Flush(olc::Sprite* sprite);
        static void End();

        static void DrawCupTriangle(const Trianglef& triangle, olc::Sprite* sprite);
		//static void Renderer::DrawString(const std::string& text, const Vector4& color);
	private:
		static void FillTexturedTriangle(const std::array<Vector3f, 3>& vPoints, std::array<Vector2f, 3> vTex, olc::Sprite* sprTex, std::array<olc::Pixel, 3> vColour = { olc::WHITE, olc::WHITE, olc::WHITE });
	private:
		struct RendererData
		{
			std::vector<Trianglef> sumTriangles;
			std::vector<olc::Sprite> sumTextures;
			uint32_t textureIndex = 0;

			float* depthBuffer = nullptr;

			float clipNear = 0.1f;
			float clipFar = 1.0f;
			Matrix4x4f projectionMat;
			Matrix4x4f viewMat;

			Vector3f cameraPos;

            olc::PixelGameEngine* appPtr = nullptr;
		};

		static RendererData m_rendererData;
	};
}