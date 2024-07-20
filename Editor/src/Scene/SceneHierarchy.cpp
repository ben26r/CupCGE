#include "SceneHierarchy.h"
#include "InspectorStyles.h"

namespace Cup {

    SceneHierarchy::SceneHierarchy(const std::shared_ptr<Scene>& scene)
        : m_context(scene)
    {

    }

	void SceneHierarchy::OnImGuiRender()
	{
        ImGui::Begin("Scene Hierarchy");

        m_context->m_registry.ForEachComponent<TagComponent>([&](Entity entity, TagComponent& component) 
            {
                DrawEntityNode(entity, component.tag);
            });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_selectedEntity = -1;

        if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
                Cup::CupEntity(m_context);

            if (ImGui::MenuItem("Create Cube"))
            {
                auto entity = Cup::CupEntity(m_context, "Cube");
                Meshf mesh;
                mesh.CreateCube();
                entity.AddComponent<TransformComponent>();
                entity.AddComponent<MeshRendererComponent>(mesh);
            }

            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Inspector");

        if (m_selectedEntity != -1)
        {
            m_context->m_registry.ForEachEntity(m_selectedEntity, [&](auto& component) {
                InspectorStyles::GetStyle(component);
                });
        }

        ImGui::End();
	}

    void SceneHierarchy::DrawEntityNode(Entity entity, const std::string& name) 
    { 
        ImGuiTreeNodeFlags flags = ((m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf 
            | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;;
        ImGui::TreeNodeEx((void*)entity, flags, name.c_str());

        if (ImGui::IsItemClicked())
        {
            m_selectedEntity = entity;
        }
    }
}