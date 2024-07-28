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

	struct Spotlight
	{
		Vector3f position;
		float radius = 1.0f;
		float intensity = 1.0f;
		olc::Pixel color = olc::WHITE;
	};

	class Renderer
	{
	public:
		struct RendererStats
		{
			uint32_t submits = 0;
			uint32_t triangles = 0;
			uint32_t pixels = 0;
		};

		struct RendererData
		{
			RendererStats stats;

			std::vector<Trianglef> sumTriangles;
			TextureStorage sumTextures;
			Meshf spriteMesh;

			float* depthBuffer = nullptr;

			olc::Pixel fogColor = olc::WHITE;
			float fogMin = -0.171f;
			float fogMax = 0.316f;
			float fog = 2.0f;
			Spotlight light;

			float clipNear = 0.1f;
			float clipFar = 1.0f;
			Matrix4x4f projectionMat;
			Matrix4x4f viewMat;

			Vector3f cameraPos;

			olc::PixelGameEngine* appPtr = nullptr;
		};
	public:

		static bool Init(olc::PixelGameEngine* appPtr);

		static void Submit(const Meshf& mesh, const Matrix4x4f& model, uint32_t texture = 0, const Vector2f tiling = {1,1}, const olc::Pixel& color = olc::WHITE, bool wired = false);
		static void DrawSprite(uint32_t texture, const Matrix4x4f& model, const Vector2f tiling = { 1,1 }, const olc::Pixel& color = olc::WHITE, bool wired = false);
		static void DrawWiredCube(const Matrix4x4f& model, const olc::Pixel& color = olc::WHITE);
		static void DrawLine(const Vector3f& start, const Vector3f& end, const olc::Pixel& color = olc::Pixel(255, 255, 255));

		static uint32_t CreateTexture(const TextureProps& props = TextureProps());
		static uint32_t CreateTexture(const std::string& filepath, const TextureProps& props = TextureProps());
		static TextureStorage& GetTextureStorage() { return m_data->sumTextures; }

        static void Sort();
		static void Start(const std::shared_ptr<Camera>& camera, const Vector3f& position);
		static void Flush(const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color, bool wired);
        static void End();

        static void DrawCupTriangle(const Trianglef& triangle, const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color);
		static void DrawWiredTriangle(const Trianglef& triangle, const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color);

		static RendererData* GetData() { return m_data; }
		static const RendererStats& GetStats() { return m_data->stats; }
		static const float* GetDepthBuffer() { return m_data->depthBuffer; }
		static const Meshf& GetSpriteMesh() { return m_data->spriteMesh; }
		//static void Renderer::DrawString(const std::string& text, const Vector4& color);
	private:
		static void FillTexturedTriangle(const std::array<Vector3f, 3>& vPoints, std::array<Vector2f, 3> vTex, const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color);
	private:

		static RendererData* m_data;
	};
}