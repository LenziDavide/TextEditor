workspace "TextEditor"
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release"
    }

TargetDir = "bin/%{cfg.buildcfg}-%{cfg.architecture}"
ObjDir = "bin-int/%{cfg.buildcfg}-%{cfg.architecture}-%{prj.name}"

project "Program"
    location "Program"
    targetname "Text-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    characterset "MBCS"
    warnings "Off"

    targetdir (TargetDir)
    objdir (ObjDir)

    files 
    {
        "%{prj.name}/vendor/imgui/**.cpp",
        "%{prj.name}/vendor/imgui/**.h",
        
        "%{prj.name}/vendor/glfw/**.cpp",
        "%{prj.name}/vendor/glfw/**.h",

        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h",
        
        "%{prj.name}/icon1.ico",
        "%{prj.name}/Program.rc",
        "%{prj.name}/resource.h"
    }

    links 
    {
        "opengl32.lib",
        "glfw3.lib"
    }

    includedirs
    {
        "%{prj.name}/vendor/glfw/include/x64",
        "%{prj.name}/vendor/imgui/",

        "%{prj.name}/src/",
        "%{prj.name}/"
    }

    libdirs
    {
        "%{prj.name}/vendor/glfw/lib"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"