workspace "Renderer"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    project "Renderer"
        location "renderer"
        kind "ConsoleApp"
        language "C++"
        characterset ("MBCS")

        targetdir("build/" ..outputdir .. "/%{prj.name}")
        objdir("build-int/" ..outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.name}/includes/**.h",
			"%{prj.name}/src/**.c"
        }

        includedirs
        {
            "%{prj.name}/includes",
			"vendor/sdl2/include"
        }
		
		libdirs 
		{ 
			"vendor/sdl2/lib"
		}

        links
        {
			"SDL2.lib",
			"SDL2main.lib"
        }
		
		postbuildcommands 
		{
			'{COPY} "../vendor/sdl2/lib/SDL2.dll" "%{cfg.targetdir}"'
		}
	

        filter "system:windows"
        cppdialect "C++17"
        staticruntime "on"
        systemversion "latest"

        defines
        {
        }

        filter "configurations:Debug"
        defines {"_DEBUG", "_CRT_SECURE_NO_WARNINGS"}
        symbols "on"

        filter "configurations:Release"
        defines {"_RELEASE", "_CRT_SECURE_NO_WARNINGS"}
        optimize "on"
