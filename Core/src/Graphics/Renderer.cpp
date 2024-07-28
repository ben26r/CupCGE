#include "Renderer.h"

namespace Cup {

    Renderer::RendererData* Renderer::m_data = nullptr;

    bool Renderer::Init(olc::PixelGameEngine* appPtr) // - temporary solution
    {
		SCOPE_FUNC();
		m_data = new RendererData();
		m_data->appPtr = appPtr;
		m_data->depthBuffer = new float[appPtr->ScreenWidth() * appPtr->ScreenHeight()];
		m_data->spriteMesh.triangles = { { Vector3f(0.0f, 0.0f, 0.0f),    Vector3f(0.0f, 1.0f, 0.0f),    Vector3f(1.0f, 1.0f, 0.0f),  Vector2f(0.0f, 1.0f),      Vector2f(0.0f, 0.0f),     Vector2f(1.0f, 0.0f) },
											   { Vector3f(0.0f, 0.0f, 0.0f),    Vector3f(1.0f, 1.0f, 0.0f),    Vector3f(1.0f, 0.0f, 0.0f),  Vector2f(0.0f, 1.0f),      Vector2f(1.0f, 0.0f),     Vector2f(1.0f, 1.0f) } };
        return true;
    }

	// Change back to props
	void Renderer::Submit(const Meshf& mesh, const Matrix4x4f& model, uint32_t texture, const Vector2f tiling, const olc::Pixel& color, bool wired)
	{
		SCOPE_FUNC();
		m_data->stats.submits++;

		for (auto triangle : mesh.triangles)
		{
			for (auto& tex : triangle.texCoords)
			{
				tex *= tiling;
			}

			triangle *= model;
			//float distance = (triangle[0] - m_rendererData->cameraPos).magnitude();
			//if (distance > m_rendererData->clipFar) return;

			Vector3f normal((triangle[1] - triangle[0]).cross((triangle[2] - triangle[0])));
			normal.normalize();

			if (normal.dot(triangle[0] - m_data->cameraPos) < 0.0f)
			{
				triangle *= m_data->viewMat;

				int nClippedTrianglesA = 0;
				Trianglef clippedA[2];
				nClippedTrianglesA = ClipAgainstPlane({ 0.0f, 0.0f, m_data->clipFar }, { 0.0f, 0.0f, -1.0f }, triangle, clippedA[0], clippedA[1]);

				for (int j = 0; j < nClippedTrianglesA; j++)
				{

					int nClippedTriangles = 0;
					Trianglef clipped[2];
					nClippedTriangles = ClipAgainstPlane({ 0.0f, 0.0f, m_data->clipNear }, { 0.0f, 0.0f, 1.0f }, clippedA[j], clipped[0], clipped[1]);

					for (int n = 0; n < nClippedTriangles; n++)
					{
						for (int i = 0; i < 3; i++)
						{
							clipped[n].vertices[i] = m_data->projectionMat * clipped[n].vertices[i];
							clipped[n].vertices[i] = clipped[n].vertices[i] / clipped[n].vertices[i].w;

							clipped[n].texCoords[i] = clipped[n].texCoords[i] / clipped[n].vertices[i].w;
							clipped[n].texCoords[i].w = 1.0f / clipped[n].vertices[i].w;

							clipped[n].vertices[i].x += 1.0f; clipped[n].vertices[i].y += 1.0f;
							clipped[n].vertices[i].x *= 0.5f * (float)m_data->appPtr->ScreenWidth(); clipped[n].vertices[i].y *= 0.5f * (float)m_data->appPtr->ScreenHeight();
						}

						m_data->sumTriangles.push_back(clipped[n]);
					}
				}
			}
		}
		
		Flush(m_data->sumTextures.GetSprite(texture), color, wired);
	}

	void Renderer::DrawSprite(uint32_t texture, const Matrix4x4f& model, const Vector2f tiling, const olc::Pixel& color, bool wired)
	{
		Submit(m_data->spriteMesh, model, texture, tiling, color, wired);
	}

	void Renderer::DrawWiredCube(const Matrix4x4f& model, const olc::Pixel& color)
	{
		Vector3f min;
		Vector3f max(1,1,1);

		// Define the 8 vertices of the box
		Vector3f vertices[8] = {
			{min.x, min.y, min.z},
			{max.x, min.y, min.z},
			{max.x, max.y, min.z},
			{min.x, max.y, min.z},
			{min.x, min.y, max.z},
			{max.x, min.y, max.z},
			{max.x, max.y, max.z},
			{min.x, max.y, max.z}
		};

		for (auto& vertex : vertices)
			vertex = model * vertex;

		// Draw the edges of the box
		Renderer::DrawLine(vertices[0], vertices[1], color);
		Renderer::DrawLine(vertices[1], vertices[2], color);
		Renderer::DrawLine(vertices[2], vertices[3], color);
		Renderer::DrawLine(vertices[3], vertices[0], color);

		Renderer::DrawLine(vertices[4], vertices[5], color);
		Renderer::DrawLine(vertices[5], vertices[6], color);
		Renderer::DrawLine(vertices[6], vertices[7], color);
		Renderer::DrawLine(vertices[7], vertices[4], color);

		Renderer::DrawLine(vertices[0], vertices[4], color);
		Renderer::DrawLine(vertices[1], vertices[5], color);
		Renderer::DrawLine(vertices[2], vertices[6], color);
		Renderer::DrawLine(vertices[3], vertices[7], color);
	}

	void Renderer::DrawLine(const Vector3f& start, const Vector3f& end, const olc::Pixel& color)
	{
		Vector3f startScreen = m_data->viewMat * start;
		Vector3f endScreen = m_data->viewMat * end;

		// Define near and far plane
		Vector3f nearPlaneNormal = { 0.0f, 0.0f, 1.0f };
		Vector3f farPlaneNormal = { 0.0f, 0.0f, -1.0f };
		Vector3f nearPlanePoint = { 0.0f, 0.0f, m_data->clipNear };
		Vector3f farPlanePoint = { 0.0f, 0.0f, m_data->clipFar };

		// Clip against near plane
		bool startInside = (nearPlaneNormal.dot(startScreen - nearPlanePoint) >= 0);
		bool endInside = (nearPlaneNormal.dot(endScreen - nearPlanePoint) >= 0);

		if (!startInside) startScreen = IntersectPlane(nearPlanePoint, nearPlaneNormal, startScreen, endScreen);
		if (!endInside) endScreen = IntersectPlane(nearPlanePoint, nearPlaneNormal, startScreen, endScreen);

		// Clip against far plane
		startInside = (farPlaneNormal.dot(startScreen - farPlanePoint) >= 0);
		endInside = (farPlaneNormal.dot(endScreen - farPlanePoint) >= 0);

		if (!startInside) startScreen = IntersectPlane(farPlanePoint, farPlaneNormal, startScreen, endScreen);
		if (!endInside) endScreen = IntersectPlane(farPlanePoint, farPlaneNormal, startScreen, endScreen);

		// Transform the start and end points from world space to screen space
		startScreen = m_data->projectionMat * startScreen;
		endScreen = m_data->projectionMat * endScreen;

		// Perform perspective divide
		startScreen /= startScreen.w;
		endScreen /= endScreen.w;

		// Convert from normalized device coordinates to screen coordinates
		startScreen.x = (startScreen.x + 1.0f) * (0.5f * m_data->appPtr->ScreenWidth());
		startScreen.y = (startScreen.y + 1.0f) * (0.5f * m_data->appPtr->ScreenHeight());
		endScreen.x = (endScreen.x + 1.0f) * (0.5f * m_data->appPtr->ScreenWidth());
		endScreen.y = (endScreen.y + 1.0f) * (0.5f * m_data->appPtr->ScreenHeight());

		// Rasterize the line
		int x0 = static_cast<int>(startScreen.x);
		int y0 = static_cast<int>(startScreen.y);
		int x1 = static_cast<int>(endScreen.x);
		int y1 = static_cast<int>(endScreen.y);

		// Use Bresenham's line algorithm
		int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
		int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
		int err = dx + dy, e2; // error value e_xy

		while (true) {
			m_data->stats.pixels++;
			m_data->appPtr->Draw(x0, y0, color); // Draw the pixel
			if (x0 == x1 && y0 == y1) break;
			e2 = 2 * err;
			if (e2 >= dy) { err += dy; x0 += sx; } // e_xy + e_x > 0
			if (e2 <= dx) { err += dx; y0 += sy; } // e_xy + e_y < 0
		}
	}

	void Renderer::Flush(const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color, bool wired)
	{
		//Sort();

		for (const Triangle<float>& triangle : m_data->sumTriangles)
		{
			Triangle<float> clipped[2];
			std::list<Triangle<float>> listTriangles;

			// Add initial triangle
			listTriangles.push_back(triangle);
			int nNewTriangles = 1;

			for (int p = 0; p < 4; p++)
			{
				int nTrisToAdd = 0;
				while (nNewTriangles > 0)
				{
					Triangle<float> test = listTriangles.front();
					listTriangles.pop_front();
					nNewTriangles--;

					switch (p)
					{
					case 0:	nTrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 1:	nTrisToAdd = ClipAgainstPlane({ 0.0f, (float)m_data->appPtr->ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 2:	nTrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 3:	nTrisToAdd = ClipAgainstPlane({ (float)m_data->appPtr->ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					}

					for (int w = 0; w < nTrisToAdd; w++)
						listTriangles.push_back(clipped[w]);
				}
				nNewTriangles = listTriangles.size();
			}

			for (auto& t : listTriangles)
			{
				DrawCupTriangle(t, sprite, color);
				if (wired)
					DrawWiredTriangle(t, sprite, color);
			}
		}

		m_data->sumTriangles.clear();
	}

	uint32_t Renderer::CreateTexture(const TextureProps& props)
	{
		return m_data->sumTextures.CreateTexture(props.width, props.height);
	}

	uint32_t Renderer::CreateTexture(const std::string& filepath, const TextureProps& props)
	{
		return m_data->sumTextures.CreateTexture(filepath);
	}

	void Renderer::Start(const std::shared_ptr<Camera>& camera, const Vector3f& position)
	{
		m_data->stats = RendererStats();

		m_data->appPtr->FillRect(0, 0, m_data->appPtr->ScreenWidth(), m_data->appPtr->ScreenHeight(), olc::Pixel(0, 0, 0));
		m_data->projectionMat = camera->GetProjection();
		m_data->viewMat = camera->GetView(position);
		m_data->cameraPos = position;

		// Shouldn't be using camera
		m_data->clipNear = std::max(camera->cnear, 0.1f);
		m_data->clipFar = std::max(camera->cfar, 0.1f);
		for (int i = 0; i < m_data->appPtr->ScreenWidth() * m_data->appPtr->ScreenHeight(); i++)
			m_data->depthBuffer[i] = 0;
	}

	void Renderer::End()
	{
		SCOPE_FUNC();
	}

	void Renderer::DrawCupTriangle(const Trianglef& triangle, const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color)
	{
		SCOPE_FUNC();
		m_data->stats.triangles++;
		FillTexturedTriangle(triangle.vertices, triangle.texCoords, sprite, color);
		//m_rendererData->appPtr->DrawTriangle(triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, olc::Pixel(255.0f, 255.0f, 255.0f, 255.0f));
	}

	void Renderer::DrawWiredTriangle(const Trianglef& triangle, const std::shared_ptr<olc::Sprite>& sprite, const olc::Pixel& color)
	{
		SCOPE_FUNC();
		m_data->stats.triangles++;
		m_data->appPtr->DrawTriangle(triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, color);
	}

	inline float fract(float x)
	{
		return x - std::floor(x);
	}

	inline Vector2f fract(const Vector2f& vec)
	{
		return Vector2f(fract(vec.x), fract(vec.y), vec.w);
	}

	void Renderer::FillTexturedTriangle(const std::array<Vector3f, 3>& vPoints, std::array<Vector2f, 3> vTex, const std::shared_ptr<olc::Sprite>& sprTex, const olc::Pixel& color)
	{
		Vector2i p1 = {(int)vPoints[0].x, (int)vPoints[0].y};
		Vector2i p2 = {(int)vPoints[1].x, (int)vPoints[1].y};
		Vector2i p3 = {(int)vPoints[2].x, (int)vPoints[2].y};

		if (p2.y < p1.y) { std::swap(p1.y, p2.y); std::swap(p1.x, p2.x); std::swap(vTex[0].x, vTex[1].x); std::swap(vTex[0].y, vTex[1].y); std::swap(vTex[0].w, vTex[1].w); }
		if (p3.y < p1.y) { std::swap(p1.y, p3.y); std::swap(p1.x, p3.x); std::swap(vTex[0].x, vTex[2].x); std::swap(vTex[0].y, vTex[2].y); std::swap(vTex[0].w, vTex[2].w); }
		if (p3.y < p2.y) { std::swap(p2.y, p3.y); std::swap(p2.x, p3.x); std::swap(vTex[1].x, vTex[2].x); std::swap(vTex[1].y, vTex[2].y); std::swap(vTex[1].w, vTex[2].w); }

		Vector2i dPos1 = p2 - p1;
		Vector2f dTex1 = vTex[1] - vTex[0];

		Vector2i dPos2 = p3 - p1;
		Vector2f dTex2 = vTex[2] - vTex[0];

		float dax_step = 0, dbx_step = 0, dcr1_step = 0, dcr2_step = 0, dcg1_step = 0, dcg2_step = 0, dcb1_step = 0, dcb2_step = 0, dca1_step = 0, dca2_step = 0;
		Vector2f vTex1Step, vTex2Step;

		if (dPos1.y)
		{
			dax_step = dPos1.x / (float)abs(dPos1.y);
			vTex1Step = dTex1 / (float)abs(dPos1.y);
		}

		if (dPos2.y)
		{
			dbx_step = dPos2.x / (float)abs(dPos2.y);
			vTex2Step = dTex2 / (float)abs(dPos2.y);
		}

		Vector2i vStart;
		Vector2i vEnd;
		int vStartIdx;

		for (int pass = 0; pass < 2; pass++)
		{
			if (pass == 0)
			{
				vStart = p1; vEnd = p2;	vStartIdx = 0;
			}
			else
			{
				dPos1 = p3 - p2;
				dTex1 = vTex[2] - vTex[1];
				dcr1_step = 0; dcg1_step = 0; dcb1_step = 0; dca1_step = 0;

				if (dPos2.y) dbx_step = dPos2.x / (float)abs(dPos2.y);
				if (dPos1.y)
				{
					dax_step = dPos1.x / (float)abs(dPos1.y);
					vTex1Step = dTex1 / (float)abs(dPos1.y);
				}

				vStart = p2; vEnd = p3; vStartIdx = 1;
			}

			if (dPos1.y)
			{
				for (int i = vStart.y; i <= vEnd.y; i++)
				{
					int ax = int(vStart.x + (float)(i - vStart.y) * dax_step);
					int bx = int(p1.x + (float)(i - p1.y) * dbx_step);

					Vector2f tex_s(vTex[vStartIdx].x + (float)(i - vStart.y) * vTex1Step.x, vTex[vStartIdx].y + (float)(i - vStart.y) * vTex1Step.y, vTex[vStartIdx].w + (float)(i - vStart.y) * vTex1Step.w);
					Vector2f tex_e(vTex[0].x + (float)(i - p1.y) * vTex2Step.x, vTex[0].y + (float)(i - p1.y) * vTex2Step.y, vTex[0].w + (float)(i - p1.y) * vTex2Step.w);

					if (ax > bx) { std::swap(ax, bx); std::swap(tex_s, tex_e); }

					float tstep = 1.0f / ((float)(bx - ax));
					float t = 0.0f;

					for (int j = ax; j < bx; j++)
					{
						olc::Pixel pixel = color;
						Vector2 samplePos = tex_s.lerp(tex_e, t);
						if (samplePos.w >= m_data->depthBuffer[j * m_data->appPtr->ScreenHeight() + i])
						{
							float sampleX = fract(samplePos.x / samplePos.w);
							float sampleY = fract(samplePos.y / samplePos.w);
							if (sprTex != nullptr) pixel *= sprTex->Sample(sampleX, sampleY);
							//uint8_t dColor = (uint8_t)(samplePos.w * 255);
							float fogFactor = (samplePos.w - m_data->fogMin) / (m_data->fogMax - m_data->fogMin);
							fogFactor = std::clamp(fogFactor, 0.0f, 1.0f);
							fogFactor = fogFactor * fogFactor * (3.0f - 2.0f * fogFactor); // Equivalent to smoothstep
							m_data->appPtr->Draw(j, i, pixel - (m_data->fogColor * (1.0f - fogFactor)));
							//m_data->appPtr->Draw(j, i, pixel * samplePos.w);
							m_data->stats.pixels++;
							m_data->depthBuffer[j * m_data->appPtr->ScreenHeight() + i] = samplePos.w;
						}
						t += tstep;
					}
				}
			}
		}
	}

	void Renderer::Sort()
	{
		SCOPE_FUNC();
		std::sort(m_data->sumTriangles.begin(), m_data->sumTriangles.end(), [](const Triangle<float>& triangle, const Triangle<float>& other) {
			float z1 = (triangle[0].z + triangle[1].z + triangle[2].z) / 3;
			float z2 = (other[0].z + other[1].z + other[2].z) / 3;
			return z1 > z2;
			});
	}
}