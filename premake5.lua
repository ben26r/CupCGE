workspace "CupCGE"
    architecture "x64"
    startproject "Core"
    configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--IncludeDir = {}
--IncludeDir["GLFW"] = "CupCore/vendor/GLFW/include"

project "Core"
    location "Core"
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
        "%{prj.name}/src"
    }
    links
    {
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
        "Core/src"
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