#include "SceneHierarchy.h"
#include "InspectorStyles.h"

#include <algorithm>

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

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
            m_selectedEntity.clear();

        if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
                CupEntity(m_context, "Example");

            if (ImGui::MenuItem("Create Cube"))
            {
                auto& entity = CupEntity(m_context, "Cube");
                Meshf mesh;
                mesh.CreateCube();
                entity.AddComponent<TransformComponent>();
                entity.AddComponent<MeshRendererComponent>(mesh);
            }

            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Inspector");

        if (!m_selectedEntity.empty())
        {
            DrawComponents();

            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("Add Component");
            if (ImGui::BeginPopup("Add Component"))
            {
                m_context->m_registry.ForEachType([&](auto& component) {
                    using ComponentType = std::decay_t<decltype(component)>;
                    if (family::type<ComponentType>() != family::type<CameraComponent>() &&
                        family::type<ComponentType>() != family::type<ScriptComponent>())
                    {
                        if (ImGui::MenuItem(InspectorStyles::GetName<ComponentType>()))
                        {
                            for (size_t i = 0; i < m_selectedEntity.size(); ++i)
                            {
                                m_context->m_registry.AddComponent<ComponentType>(m_selectedEntity[i]);
                            }
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    });

                if (ImGui::MenuItem(InspectorStyles::GetName<CameraComponent>()))
                {
                    for (size_t i = 0; i < m_selectedEntity.size(); ++i)
                    {
                        std::shared_ptr<Camera> camera = std::make_shared<Camera>();
                        m_context->m_registry.AddComponent<CameraComponent>(m_selectedEntity[i], camera);
                    }
                    ImGui::CloseCurrentPopup();
                }


                auto& scriptRegistry = ScriptRegistry::Instance().GetRegistry();

                for (const auto& script : scriptRegistry)
                {
                    if (ImGui::MenuItem(script.first.c_str()))
                    {
                        // foreach
                        for (size_t i = 0; i < m_selectedEntity.size(); ++i)
                        {
                            ScriptComponent nsc;
                            nsc.instance = script.second();
                            if (nsc.instance)
                            {
                                nsc.instance->Init(m_context, m_selectedEntity[i]);
                                nsc.scriptType = script.first;
                                m_context->m_registry.AddComponent<ScriptComponent>(m_selectedEntity[i], std::move(nsc));
                            }
                        }
                    }
                }

                ImGui::EndPopup();
            }
        }
        ImGui::End();
	}

    void SceneHierarchy::DrawComponents()
    {
        if (m_context->m_registry.HasComponent<TagComponent>(m_selectedEntity[0]))
        {
            TagComponent& target = m_context->m_registry.GetComponent<TagComponent>(m_selectedEntity[0]);
            TagComponent ogComponent = target;
            ImGui::SameLine();
            InspectorStyles::GetStyle(target);

            if (ogComponent != target)
            {
                for (size_t i = 1; i < m_selectedEntity.size(); ++i) {
                    if (m_context->m_registry.HasComponent<TagComponent>(m_selectedEntity[i]))
                        m_context->m_registry.GetComponent<TagComponent>(m_selectedEntity[i]) = target;
                }
            }
        }

        // Trasform selection parenting
        if (m_context->m_registry.HasComponent<TransformComponent>(m_selectedEntity[0]))
        {
            TransformComponent& target = m_context->m_registry.GetComponent<TransformComponent>(m_selectedEntity[0]);
            TransformComponent ogTransform = target;
            if (ImGui::Button("X"))
            {
                // Delete Components
                for (size_t i = 0; i < m_selectedEntity.size(); ++i) {
                    if (m_context->m_registry.HasComponent<TransformComponent>(m_selectedEntity[i]))
                        m_context->m_registry.RemoveComponent<TransformComponent>(m_selectedEntity[i]);
                }
            }
            else
            {
                // Draw Component Styles
                ImGui::SameLine();
                if (ImGui::CollapsingHeader(InspectorStyles::GetName<TransformComponent>()))
                {
                    InspectorStyles::GetStyle(target);
                }
                ogTransform = target - ogTransform;
                for (size_t i = 0; i < m_selectedEntity.size(); ++i) {
                    if (m_context->m_registry.HasComponent<TransformComponent>(m_selectedEntity[i]))
                    {
                        TransformComponent& transform = m_context->m_registry.GetComponent<TransformComponent>(m_selectedEntity[i]);
                        if (i > 0)
                            transform += ogTransform;

                        Vector3f& min = transform.position;
                        Vector3f& max = transform.position + transform.scale;

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
                        Renderer::DrawLine(vertices[0], vertices[1]);
                        Renderer::DrawLine(vertices[1], vertices[2]);
                        Renderer::DrawLine(vertices[2], vertices[3]);
                        Renderer::DrawLine(vertices[3], vertices[0]);

                        Renderer::DrawLine(vertices[4], vertices[5]);
                        Renderer::DrawLine(vertices[5], vertices[6]);
                        Renderer::DrawLine(vertices[6], vertices[7]);
                        Renderer::DrawLine(vertices[7], vertices[4]);

                        Renderer::DrawLine(vertices[0], vertices[4]);
                        Renderer::DrawLine(vertices[1], vertices[5]);
                        Renderer::DrawLine(vertices[2], vertices[6]);
                        Renderer::DrawLine(vertices[3], vertices[7]);
                    }
                }
            }
        }

        // Other components are copied directly
        m_context->m_registry.ForEachEntity(m_selectedEntity[0], [&](auto& component) {
            using ComponentType = std::decay_t<decltype(component)>;
            if (family::type<ComponentType>() != family::type<TransformComponent>() && family::type<ComponentType>() != family::type<TagComponent>()
                && family::type<ComponentType>() != family::type<ScriptComponent>())
            {
                ComponentType ogComponent = component;
                ImGui::Button("X");
                if (ImGui::IsItemClicked())
                {
                    // Delete Components
                    for (size_t i = 0; i < m_selectedEntity.size(); ++i) {
                        if (m_context->m_registry.HasComponent<ComponentType>(m_selectedEntity[i]))
                            m_context->m_registry.RemoveComponent<ComponentType>(m_selectedEntity[i]);
                    }
                }
                else
                {
                    // Draw Component Styles
                    ImGui::SameLine();
                    if (ImGui::CollapsingHeader(InspectorStyles::GetName<ComponentType>()))
                    {
                        InspectorStyles::GetStyle(component);
                    }

                    // Copies directly within considering changes
                    for (size_t i = 1; i < m_selectedEntity.size(); ++i) {
                        if (m_context->m_registry.HasComponent<ComponentType>(m_selectedEntity[i]))
                            m_context->m_registry.GetComponent<ComponentType>(m_selectedEntity[i]) = component;
                    }
                }
            }
            });
    
        if (m_context->m_registry.HasComponent<ScriptComponent>(m_selectedEntity[0]))
        {
            ImGui::Button("X");
            if (ImGui::IsItemClicked())
            {
                // Delete Components
                auto target = m_context->m_registry.GetComponent<ScriptComponent>(m_selectedEntity[0]).scriptType;
                for (size_t i = 0; i < m_selectedEntity.size(); ++i) {
                    if (m_context->m_registry.HasComponent<ScriptComponent>(m_selectedEntity[i]))
                    {
                        ScriptComponent& script = m_context->m_registry.GetComponent<ScriptComponent>(m_selectedEntity[i]);
                        if (script.scriptType == target)
                            m_context->m_registry.RemoveComponent<ScriptComponent>(m_selectedEntity[i]);
                    }
                }
            }
            else 
            {
                ScriptComponent& target = m_context->m_registry.GetComponent<ScriptComponent>(m_selectedEntity[0]);
                ImGui::SameLine();
                if (ImGui::CollapsingHeader(target.scriptType.c_str()))
                {
                    InspectorStyles::GetStyle(target);
                }
            }
        }
    }

    void SceneHierarchy::DrawEntityNode(Entity entity, const std::string& name) 
    { 
        bool selected = std::find(m_selectedEntity.begin(), m_selectedEntity.end(), entity) != m_selectedEntity.end();
        ImGuiTreeNodeFlags flags = (selected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_Leaf
            | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;;
        ImGui::TreeNodeEx((void*)entity, flags, name.c_str());

        if (ImGui::IsItemClicked())
        {
            // Select specific entity's
            if (CupEngine::Instance().GetKey(olc::Key::SHIFT).bHeld &&
                CupEngine::Instance().GetKey(olc::Key::CTRL).bHeld)
            {
                if (std::find(m_selectedEntity.begin(), m_selectedEntity.end(), entity) == m_selectedEntity.end())
                    m_selectedEntity.push_back(entity);
            }
            // Select a group of entity's
            else if (CupEngine::Instance().GetKey(olc::Key::SHIFT).bHeld)
            {
                if (std::find(m_selectedEntity.begin(), m_selectedEntity.end(), entity) == m_selectedEntity.end())
                    m_selectedEntity.push_back(entity);

                std::sort(m_selectedEntity.begin(), m_selectedEntity.end());
                uint32_t start = m_selectedEntity[0]; uint32_t end = m_selectedEntity.back();
                for (uint32_t i = start; i <= end; i ++)
                {
                    if (std::find(m_selectedEntity.begin(), m_selectedEntity.end(), i) == m_selectedEntity.end())
                        m_selectedEntity.push_back(i);
                }
            }
            // Select a single entity
            else
            {
                m_selectedEntity.clear();
                m_selectedEntity.push_back(entity);
            }
        }

        // Delete entity
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Duplicate Entity"))
            {
                if (std::find(m_selectedEntity.begin(), m_selectedEntity.end(), entity) == m_selectedEntity.end())
                    m_selectedEntity.push_back(entity);
                std::vector<int> newEntities;
                for (auto e : m_selectedEntity)
                {
                    newEntities.push_back(m_context->DuplicateEntity(e));
                }
                m_selectedEntity = newEntities;
            }

            if (ImGui::MenuItem("Delete Entity"))
            {
                if (std::find(m_selectedEntity.begin(), m_selectedEntity.end(), entity) == m_selectedEntity.end())
                    m_selectedEntity.push_back(entity);

                for (auto e : m_selectedEntity)
                {
                    m_context->m_registry.ForEachEntity(e, [&](auto& component) {
                        using ComponentType = std::decay_t<decltype(component)>;
                        m_context->m_registry.RemoveComponent<ComponentType>(e);
                        });
                }
                m_selectedEntity.clear();
            }
            ImGui::EndPopup();
        }
    }
}