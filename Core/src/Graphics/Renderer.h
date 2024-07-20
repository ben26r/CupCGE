#pragma once

#include <algorithm>
#include <list>
#include <functional>

#include "Core/Base.h"
#include "Camera.h"
#include "Math/CupMath.h"
#include "Olc/olcPixelGameEngine.h"
#include "TextureStorage.h"
#include <array>

namespace Cup {

	class Renderer
	{
	public:

		static bool Init(olc::PixelGameEngine* appPtr);

		static void Submit(const Matrix4x4f& matrix, const Meshf& mesh, uint32_t texture = 0, olc::Pixel color = olc::Pixel(255, 255, 255));
		static uint32_t CreateTexture(const std::string& filepath, const TextureProps& props = TextureProps());
		static const TextureStorage& GetTextureStorage() { return m_rendererData.sumTextures; }

        static void Sort();
		static void Start(const std::shared_ptr<Camera>& camera);
		static void Flush(const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color);
        static void End();

        static void DrawCupTriangle(const Trianglef& triangle, const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color);
		//static void Renderer::DrawString(const std::string& text, const Vector4& color);
	private:
		static void FillTexturedTriangle(const std::array<Vector3f, 3>& vPoints, std::array<Vector2f, 3> vTex, const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color);
	private:
		struct RendererData
		{
			std::vector<Trianglef> sumTriangles;
			TextureStorage sumTextures;

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