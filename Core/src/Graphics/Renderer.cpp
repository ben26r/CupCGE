#include "Renderer.h"

namespace Cup {

    Renderer::RendererData Renderer::m_rendererData = Renderer::RendererData();

    bool Renderer::Init(const float& width, const float& height) // - temporary solution
    {
        m_rendererData.screenWidth = width;
        m_rendererData.screenHeight = height;
        return true;
    }

}