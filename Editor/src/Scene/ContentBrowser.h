#pragma once

#include "Cup.h"
#include <filesystem>

namespace Cup {

	class ContentBrowser
	{
	public:
		ContentBrowser(const std::string& filepath);

		void OnImGuiRender();
	private:
		std::filesystem::path m_buildDirectory;
		std::filesystem::path m_currentDirectory;

	};

}