#include "ContentBrowser.h"

namespace Cup {


	ContentBrowser::ContentBrowser(const std::string& filepath)
		: m_buildDirectory(filepath), m_currentDirectory(filepath)
	{

	}

	void ContentBrowser::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		// Taken from the hazel game engine https://github.com/TheCherno/Hazel
		if (m_currentDirectory != std::filesystem::path(m_buildDirectory))
		{
			if (ImGui::Button("<-"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
			}
		}

		static float padding = 0.0f;
		static float thumbnailSize = 160.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			ImVec4 buttonColor = directoryEntry.is_directory() ? ImVec4(0.2f, 0.21f, 0.35f, 255) : ImVec4(0.2f, 0.28f, 0.45f, 255);
			ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
			ImGui::Button(filenameString.c_str(), {thumbnailSize, 50});

			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(path);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_currentDirectory /= path.filename();

			}
			//ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}

}