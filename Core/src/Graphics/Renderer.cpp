#include "Renderer.h"

namespace Cup {

    Renderer::RendererData Renderer::m_rendererData = Renderer::RendererData();

    bool Renderer::Init(olc::PixelGameEngine* appPtr) // - temporary solution
    {
		SCOPE_FUNC();
		m_rendererData.appPtr = appPtr;
		m_rendererData.depthBuffer = new float[appPtr->ScreenWidth() * appPtr->ScreenHeight()];
        return true;
    }

	void Renderer::Submit(const Matrix4x4f& matrix, const Meshf& mesh, olc::Sprite* sprite)
	{
		SCOPE_FUNC();

		for (auto triangle : mesh.triangles)
		{
			triangle *= matrix;

			Vector3f normal((triangle[1] - triangle[0]).cross((triangle[2] - triangle[0])));
			normal.normalize();

			if (normal.dot(triangle[0] - m_rendererData.cameraPos) < 0.0f)
			{
				triangle *= m_rendererData.viewMat;

				int nClippedTriangles = 0;
				Trianglef clipped[2];
				nClippedTriangles = ClipAgainstPlane({ 0.0f, 0.0f, m_rendererData.clipNear }, { 0.0f, 0.0f, 1.0f }, triangle, clipped[0], clipped[1]);

				for (int n = 0; n < nClippedTriangles; n++)
				{
					for (int i = 0; i < 3; i++)
					{
						clipped[n].vertices[i] = m_rendererData.projectionMat * clipped[n].vertices[i];
						clipped[n].vertices[i] = clipped[n].vertices[i] / clipped[n].vertices[i].w;

						clipped[n].texCoords[i] = clipped[n].texCoords[i] / clipped[n].vertices[i].w;
						clipped[n].texCoords[i].w = 1.0f / clipped[n].vertices[i].w;

						clipped[n].vertices[i].x += 1.0f; clipped[n].vertices[i].y += 1.0f;
						clipped[n].vertices[i].x *= 0.5f * (float)m_rendererData.appPtr->ScreenWidth(); clipped[n].vertices[i].y *= 0.5f * (float)m_rendererData.appPtr->ScreenHeight();
					}

					m_rendererData.sumTriangles.push_back(clipped[n]);
				}
			}
		}
		
		Flush(sprite);
	}

	void Renderer::Flush(olc::Sprite* sprite)
	{
		//Sort();

		for (const Triangle<float>& triangle : m_rendererData.sumTriangles)
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
					case 1:	nTrisToAdd = ClipAgainstPlane({ 0.0f, (float)m_rendererData.appPtr->ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 2:	nTrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 3:	nTrisToAdd = ClipAgainstPlane({ (float)m_rendererData.appPtr->ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					}

					for (int w = 0; w < nTrisToAdd; w++)
						listTriangles.push_back(clipped[w]);
				}
				nNewTriangles = listTriangles.size();
			}

			for (auto& t : listTriangles)
			{
				DrawCupTriangle(t, sprite);
			}
		}

		m_rendererData.sumTriangles.clear();
	}

	uint32_t Renderer::CreateTexture(const std::string& filepath, const TextureProps& props)
	{
		//auto it = m_rendererData.sumTextures.emplace(m_rendererData.sumTextures.end(), filepath);
		return 0;
	}

	void Renderer::Start(const std::shared_ptr<Camera>& camera)
	{
		m_rendererData.appPtr->FillRect(0, 0, m_rendererData.appPtr->ScreenWidth(), m_rendererData.appPtr->ScreenHeight(), olc::Pixel(0, 0, 0));
		m_rendererData.projectionMat = camera->GetProjection();
		m_rendererData.viewMat = camera->GetView();
		m_rendererData.cameraPos = camera->GetPosition();
		m_rendererData.clipNear = std::max(camera->cnear, 0.1f);
		for (int i = 0; i < m_rendererData.appPtr->ScreenWidth() * m_rendererData.appPtr->ScreenHeight(); i++)
			m_rendererData.depthBuffer[i] = 0;
	}

	void Renderer::End()
	{
		SCOPE_FUNC();
	}

	void Renderer::DrawCupTriangle(const Trianglef& triangle, olc::Sprite* sprite)
	{
		SCOPE_FUNC();
		FillTexturedTriangle(triangle.vertices, triangle.texCoords, sprite, triangle.colors);
		//m_rendererData.appPtr->DrawTriangle(triangle[0].x, triangle[0].y, triangle[1].x, triangle[1].y, triangle[2].x, triangle[2].y, olc::Pixel(255.0f, 255.0f, 255.0f, 255.0f));
	}

	void Renderer::FillTexturedTriangle(const std::array<Vector3f, 3>& vPoints, std::array<Vector2f, 3> vTex, olc::Sprite* sprTex, std::array<olc::Pixel, 3> vColour)
	{
		Vector2i p1 = {(int)vPoints[0].x, (int)vPoints[0].y};
		Vector2i p2 = {(int)vPoints[1].x, (int)vPoints[1].y};
		Vector2i p3 = {(int)vPoints[2].x, (int)vPoints[2].y};

		if (p2.y < p1.y) { std::swap(p1.y, p2.y); std::swap(p1.x, p2.x); std::swap(vTex[0].x, vTex[1].x); std::swap(vTex[0].y, vTex[1].y); std::swap(vTex[0].w, vTex[1].w); std::swap(vColour[0], vColour[1]); }
		if (p3.y < p1.y) { std::swap(p1.y, p3.y); std::swap(p1.x, p3.x); std::swap(vTex[0].x, vTex[2].x); std::swap(vTex[0].y, vTex[2].y); std::swap(vTex[0].w, vTex[2].w); std::swap(vColour[0], vColour[2]); }
		if (p3.y < p2.y) { std::swap(p2.y, p3.y); std::swap(p2.x, p3.x); std::swap(vTex[1].x, vTex[2].x); std::swap(vTex[1].y, vTex[2].y); std::swap(vTex[1].w, vTex[2].w); std::swap(vColour[1], vColour[2]); }

		Vector2i dPos1 = p2 - p1;
		Vector2f dTex1 = vTex[1] - vTex[0];
		int dcr1 = vColour[1].r - vColour[0].r;
		int dcg1 = vColour[1].g - vColour[0].g;
		int dcb1 = vColour[1].b - vColour[0].b;
		int dca1 = vColour[1].a - vColour[0].a;

		Vector2i dPos2 = p3 - p1;
		Vector2f dTex2 = vTex[2] - vTex[0];
		int dcr2 = vColour[2].r - vColour[0].r;
		int dcg2 = vColour[2].g - vColour[0].g;
		int dcb2 = vColour[2].b - vColour[0].b;
		int dca2 = vColour[2].a - vColour[0].a;

		float dax_step = 0, dbx_step = 0, dcr1_step = 0, dcr2_step = 0, dcg1_step = 0, dcg2_step = 0, dcb1_step = 0, dcb2_step = 0, dca1_step = 0, dca2_step = 0;
		Vector2f vTex1Step, vTex2Step;

		if (dPos1.y)
		{
			dax_step = dPos1.x / (float)abs(dPos1.y);
			vTex1Step = dTex1 / (float)abs(dPos1.y);
			dcr1_step = dcr1 / (float)abs(dPos1.y);
			dcg1_step = dcg1 / (float)abs(dPos1.y);
			dcb1_step = dcb1 / (float)abs(dPos1.y);
			dca1_step = dca1 / (float)abs(dPos1.y);
		}

		if (dPos2.y)
		{
			dbx_step = dPos2.x / (float)abs(dPos2.y);
			vTex2Step = dTex2 / (float)abs(dPos2.y);
			dcr2_step = dcr2 / (float)abs(dPos2.y);
			dcg2_step = dcg2 / (float)abs(dPos2.y);
			dcb2_step = dcb2 / (float)abs(dPos2.y);
			dca2_step = dca2 / (float)abs(dPos2.y);
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
				dcr1 = vColour[2].r - vColour[1].r;
				dcg1 = vColour[2].g - vColour[1].g;
				dcb1 = vColour[2].b - vColour[1].b;
				dca1 = vColour[2].a - vColour[1].a;
				dcr1_step = 0; dcg1_step = 0; dcb1_step = 0; dca1_step = 0;

				if (dPos2.y) dbx_step = dPos2.x / (float)abs(dPos2.y);
				if (dPos1.y)
				{
					dax_step = dPos1.x / (float)abs(dPos1.y);
					vTex1Step = dTex1 / (float)abs(dPos1.y);
					dcr1_step = dcr1 / (float)abs(dPos1.y);
					dcg1_step = dcg1 / (float)abs(dPos1.y);
					dcb1_step = dcb1 / (float)abs(dPos1.y);
					dca1_step = dca1 / (float)abs(dPos1.y);
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

					olc::Pixel col_s(vColour[vStartIdx].r + uint8_t((float)(i - vStart.y) * dcr1_step), vColour[vStartIdx].g + uint8_t((float)(i - vStart.y) * dcg1_step),
						vColour[vStartIdx].b + uint8_t((float)(i - vStart.y) * dcb1_step), vColour[vStartIdx].a + uint8_t((float)(i - vStart.y) * dca1_step));

					olc::Pixel col_e(vColour[0].r + uint8_t((float)(i - p1.y) * dcr2_step), vColour[0].g + uint8_t((float)(i - p1.y) * dcg2_step),
						vColour[0].b + uint8_t((float)(i - p1.y) * dcb2_step), vColour[0].a + uint8_t((float)(i - p1.y) * dca2_step));

					if (ax > bx) { std::swap(ax, bx); std::swap(tex_s, tex_e); std::swap(col_s, col_e); }

					float tstep = 1.0f / ((float)(bx - ax));
					float t = 0.0f;

					for (int j = ax; j < bx; j++)
					{
						olc::Pixel pixel = PixelLerp(col_s, col_e, t);
						Vector2 samplePos = tex_s.lerp(tex_e, t);
						if (samplePos.w >= m_rendererData.depthBuffer[j * m_rendererData.appPtr->ScreenHeight() + i])
						{
							if (sprTex != nullptr) pixel *= sprTex->Sample(samplePos.x / samplePos.w, samplePos.y / samplePos.w);
							uint8_t dColor = (uint8_t)(samplePos.w * 255);
							m_rendererData.appPtr->Draw(j, i, pixel * samplePos.w * 2);
							m_rendererData.depthBuffer[j * m_rendererData.appPtr->ScreenHeight() + i] = samplePos.w;
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
		std::sort(m_rendererData.sumTriangles.begin(), m_rendererData.sumTriangles.end(), [](const Triangle<float>& triangle, const Triangle<float>& other) {
			float z1 = (triangle[0].z + triangle[1].z + triangle[2].z) / 3;
			float z2 = (other[0].z + other[1].z + other[2].z) / 3;
			return z1 > z2;
			});
	}
}