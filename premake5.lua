workspace "CupCGE"
    architecture "x64"
    startproject "Editor"
    configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["CupECS"] = "Core/vendor/CupECS/include"
IncludeDir["ImGui"] = "Core/vendor/ImGui"
IncludeDir["Json"] = "Core/vendor/nlohmannJson/single_include/nlohmann"
IncludeDir["Glm"] = "Core/vendor/glm"

include "Core/vendor/ImGui"

project "Core"
    location "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/vendor/CupECS/include/**.h",
        "%{prj.name}/vendor/glm/glm/**.hpp"
    }
    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.Json}",
        "%{IncludeDir.CupECS}",
        "%{IncludeDir.Glm}"
    }
    links
    {
        "ImGui"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "CUP_PLATFORM_WINDOWS",
            "CUP_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
    
    filter "configurations:Debug"
        defines "CUP_DEBUG"
        symbols "On"
        defines
        {
            "ENABLE_CUP_ASSERTS"
        }

    filter "configurations:Release"
        defines "CUP_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "CUP_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }
    includedirs
    {
        "Core/src",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.CupECS}",
        "%{IncludeDir.Glm}"
    }
    links 
    { 
        "Core"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "CUP_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "CUP_DEBUG"
        symbols "On"
        defines
        {
            "ENABLE_CUP_ASSERTS"
        }

    filter "configurations:Release"
        defines "CUP_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "CUP_DIST"
        optimize "On"

project "Editor"
    location "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }
    includedirs
    {
        "Core/src",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.CupECS}",
        "%{IncludeDir.Glm}"
    }
    links 
    { 
        "Core"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "CUP_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "CUP_DEBUG"
        symbols "On"
        defines
        {
            "ENABLE_CUP_ASSERTS"
        }

    filter "configurations:Release"
        defines "CUP_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "CUP_DIST"
        optimize "On"