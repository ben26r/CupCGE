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
#include "Scene/Components.h"

namespace Cup {

	class Renderer
	{
	public:
		struct RendererStats
		{
			uint32_t submits = 0;
			uint32_t triangles = 0;
			uint32_t pixels = 0;
		};
	public:

		static bool Init(olc::PixelGameEngine* appPtr);

		static void Submit(const Matrix4x4f& matrix, const MeshRendererComponent& meshComponent);
		static void DrawLine(const Vector3f& start, const Vector3f& end, const olc::Pixel& color = olc::Pixel(255, 255, 255));

		static uint32_t CreateTexture(const TextureProps& props = TextureProps());
		static uint32_t CreateTexture(const std::string& filepath, const TextureProps& props = TextureProps());
		static TextureStorage& GetTextureStorage() { return m_rendererData->sumTextures; }

        static void Sort();
		static void Start(const std::shared_ptr<Camera>& camera);
		static void Flush(const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color);
        static void End();

        static void DrawCupTriangle(const Trianglef& triangle, const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color);
		static const RendererStats& GetStats() { return m_rendererData->stats; }
		static const float* GetDepthBuffer() { return m_rendererData->depthBuffer; }
		//static void Renderer::DrawString(const std::string& text, const Vector4& color);
	private:
		static void FillTexturedTriangle(const std::array<Vector3f, 3>& vPoints, std::array<Vector2f, 3> vTex, const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color);
	private:

		struct RendererData
		{
			RendererStats stats;

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

		static RendererData* m_rendererData;
	};
}