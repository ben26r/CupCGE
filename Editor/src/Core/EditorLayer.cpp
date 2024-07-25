#include "EditorLayer.h"

#include "../Scripts/CameraController.h"
#include "../Scripts/EnemyController.h"

#define ENGINE Cup::CupEngine::Instance()

namespace Cup {

    EditorLayer::EditorLayer(CupEngine* instance)
        : m_sceneHierarchy(Cup::CupEngine::ActiveScene()), m_instance(instance)
    {
       //instance->SetLayerCustomRenderFunction(0, std::bind(&EditorLayer::RenderViewport, this));
    }

	void EditorLayer::OnAttach()
	{
        m_mainScene = Cup::CupEngine::ActiveScene();
        m_mainScene->Deserialize("assets/scene.json");
        Renderer::GetTextureStorage().Deserialize("assets/resourcepack.json");

        //Renderer::GetTextureStorage().SetTextureProps(1, olc::Sprite::PERIODIC);
	}

    void EditorLayer::OnDetach()
    {
        Cup::CupEngine::ActiveScene()->Serialize("assets/scene.json");
        Renderer::GetTextureStorage().Serialize("assets/resourcepack.json");
    }

	void EditorLayer::OnUpdate(float deltatime)
	{
        if (m_isViewportFocus)
        {
        }
        UpdateColliders();
	}

	void EditorLayer::OnImGuiRender()
	{
        static bool p_open = true;
        static bool opt_fullscreen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::Begin("DockSpace Demo", &p_open, window_flags);
        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                ImGui::Separator();

                //if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                    //*p_open = false;
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();

        ImGui::Begin("Renderer Stats");

        auto& stats = Renderer::GetStats();
        ImGui::Text("Submits: %d", stats.submits);
        ImGui::Text("Triangles: %d", stats.triangles);
        ImGui::Text("Pixels: %d", stats.pixels);

        ImGui::End();

        RenderViewport();

        m_sceneHierarchy.OnImGuiRender();

        ImGui::ShowDemoWindow();
	}

    void EditorLayer::RenderViewport()
    {
        ImGui::Begin("Viewport");

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        //const auto& target = m_instance->GetDrawTarget();

        // TODO
        // - Either use a vector2 to cache the viewport size or figure out how opge handels textures
        //if (target->width != viewportPanelSize.x || target->height != viewportPanelSize.y)
        //m_camera->Resize(viewportPanelSize.x, viewportPanelSize.y);
        m_mainScene->GetRegistry().ForEachComponent<CameraComponent>([&](Entity entity, CameraComponent& component) {
            component.camera->Resize(viewportPanelSize.x, viewportPanelSize.y);
            });
        m_isViewportFocus   = ImGui::IsWindowFocused();
        m_isViewportHovered = ImGui::IsWindowHovered();

        ImGui::Image((void*)2, ImVec2(viewportPanelSize.x, viewportPanelSize.y));

        ImGui::End();
    }

    void EditorLayer::UpdateColliders()
    {
        m_mainScene->GetRegistry().ForEachComponent<BoxColliderComponent>([&](Entity entity, BoxColliderComponent& component)
            {
                TransformComponent transform = m_mainScene->GetRegistry().GetComponent<TransformComponent>(entity);

                if (component.showOutline)
                {
                    Vector3f min = transform.position;
                    Vector3f max = component.transformScale;

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

                    // Draw the edges of the box
                    Renderer::DrawLine(vertices[0], vertices[1], olc::RED);
                    Renderer::DrawLine(vertices[1], vertices[2], olc::RED);
                    Renderer::DrawLine(vertices[2], vertices[3], olc::RED);
                    Renderer::DrawLine(vertices[3], vertices[0], olc::RED);

                    Renderer::DrawLine(vertices[4], vertices[5], olc::RED);
                    Renderer::DrawLine(vertices[5], vertices[6], olc::RED);
                    Renderer::DrawLine(vertices[6], vertices[7], olc::RED);
                    Renderer::DrawLine(vertices[7], vertices[4], olc::RED);

                    Renderer::DrawLine(vertices[0], vertices[4], olc::RED);
                    Renderer::DrawLine(vertices[1], vertices[5], olc::RED);
                    Renderer::DrawLine(vertices[2], vertices[6], olc::RED);
                    Renderer::DrawLine(vertices[3], vertices[7], olc::RED);
                }
            });
    }
}