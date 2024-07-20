#include "EditorLayer.h"

#define ENGINE Cup::CupEngine::Instance()

namespace Cup {

    EditorLayer::EditorLayer(CupEngine* instance)
        : m_sceneHierarchy(Cup::CupEngine::MainScene()), m_instance(instance)
    {
       //instance->SetLayerCustomRenderFunction(0, std::bind(&EditorLayer::RenderViewport, this));
    }

	void EditorLayer::OnAttach()
	{
        m_mainScene = Cup::CupEngine::MainScene();
        //m_mainScene->Deserialize("assets/scene.json");

		m_meshEntity = Cup::CupEntity(m_mainScene);
		m_cameraEntity = Cup::CupEntity(m_mainScene, "Camera");

        auto& tex = Texture("assets/Dirt.png");
        auto& tex2 = Texture("assets/Dirt2.png");

        for (int i = 0; i < 500; i++)
        {
            Meshf cube;
            cube.CreateCube();

            auto entity = Cup::CupEntity(m_mainScene);
            entity.AddComponent<Cup::MeshRendererComponent>(cube).texture = tex.GetIndex();
            entity.AddComponent<Cup::TransformComponent>(Cup::Vector3f(i % 50, 0.0f, 5.0f + (i / 50)), Cup::Vector3f(), Cup::Vector3f(1.0f, 1.0f, 1.0f));
        }

        for (int i = 500; i < 1000; i++)
        {
            Meshf cube;
            cube.CreateCube();

            auto entity = Cup::CupEntity(m_mainScene);
            entity.AddComponent<Cup::MeshRendererComponent>(cube).texture = tex2.GetIndex();
            entity.AddComponent<Cup::TransformComponent>(Cup::Vector3f(i % 50, 0.0f, 5.0f + (i / 50)), Cup::Vector3f(), Cup::Vector3f(1.0f, 1.0f, 1.0f));
        }
        //auto& meshComp = m_meshEntity.GetComponent<Cup::MeshComponent>();
        //meshComp.material.sprite.LoadFromFile("assets/Dirt.png");

		m_camera = std::make_shared<Cup::Camera>(0.8f);
		m_cameraEntity.AddComponent<Cup::CameraComponent>(m_camera, true);
        m_cameraEntity.AddComponent<Cup::TransformComponent>(Cup::Vector3f(0.0f, 0.0f, 10.0f), Cup::Vector3f(), Cup::Vector3f(1.0f, 1.0f, 1.0f));

        class CameraController : public ScriptableEntity
        {
        public:

            void Update(float deltatime) override
            {
                auto& m_camera = GetComponent<CameraComponent>().camera;

                if (ENGINE.GetKey(olc::Key::LEFT).bHeld)  m_camera->m_position.x -= 10.0f * deltatime;
                if (ENGINE.GetKey(olc::Key::RIGHT).bHeld) m_camera->m_position.x += 10.0f * deltatime;
                if (ENGINE.GetKey(olc::Key::UP).bHeld)    m_camera->m_position.y += 10.0f * deltatime;
                if (ENGINE.GetKey(olc::Key::DOWN).bHeld)  m_camera->m_position.y -= 10.0f * deltatime;


                Cup::Vector3<float> foward = m_camera->m_lookDir * 10 * deltatime;

                if (ENGINE.GetKey(olc::Key::S).bHeld)
                    m_camera->m_position -= foward;

                if (ENGINE.GetKey(olc::Key::W).bHeld)
                    m_camera->m_position += foward;

                if (ENGINE.GetKey(olc::Key::A).bHeld)
                    m_camera->m_yaw -= 2.0f * deltatime;

                if (ENGINE.GetKey(olc::Key::D).bHeld)
                    m_camera->m_yaw += 2.0f * deltatime;
            }
        };

        m_cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>(m_mainScene, m_cameraEntity.GetIndex());
	}

    void EditorLayer::OnDetach()
    {
        Cup::CupEngine::MainScene()->Serialize("assets/scene.json");
    }

	void EditorLayer::OnUpdate(float deltatime)
	{
        if (m_isViewportFocus)
        {
        }
		m_camera->RecalulateView();
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

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
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
        m_camera->Resize(viewportPanelSize.x, viewportPanelSize.y);
        m_isViewportFocus   = ImGui::IsWindowFocused();
        m_isViewportHovered = ImGui::IsWindowHovered();

        ImGui::Image((void*)2, ImVec2(viewportPanelSize.x, viewportPanelSize.y));

        ImGui::End();
    }

    
}