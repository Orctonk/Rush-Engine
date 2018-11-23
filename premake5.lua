workspace "Rush Engine"
	configurations {"Debug", "Release"}
	platforms {"Windows", "Linux", "Mac"}

	location "build"
	architecture "x64"

	builddir = "%{cfg.platform}-%{prj.architecture}-%{cfg.buildcfg}"

	project "Rush-Engine"
		kind "SharedLib"
		language "C++"
		cppdialect "C++17"

		files {"%{prj.name}/src/**.cpp", "%{prj.name}/src/**.h"}

		targetdir("build/bin/" .. builddir .. "/%{prj.name}")
		objdir("build/objs/" .. builddir .. "/%{prj.name}")

		defines {"RUSH_BUILD_SHARED"}

		filter "configurations:Debug"
			symbols "on"
			defines {"RUSH_DEBUG"}

		filter "configurations:Release"
			optimize "on"
			defines {"RUSH_RELEASE"}

		filter "platforms:Windows"
			system "windows"
			defines {"RUSH_WINDOWS"}

		filter "platforms:Linux"
			system "linux"
			defines {"RUSH_LINUX", "RUSH_BUILD_SHARED"}

		filter "platforms:Mac"
			system "macosx"
			defines {"RUSH_MAC"}

	project "Sandbox"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"

		links {"Rush-Engine"}

		files {"%{prj.name}/src/**.cpp", "%{prj.name}/src/**.h"}

		targetdir("build/bin/" .. builddir .. "/%{prj.name}")
		objdir("build/objs/" .. builddir .. "/%{prj.name}")

		filter "configurations:Debug"
			symbols "on"
			defines {"RUSH_DEBUG"}

		filter "configurations:Release"
			optimize "on"
			defines {"RUSH_RELEASE"}

		filter "platforms:Windows"
			system "windows"
			defines {"RUSH_WINDOWS"}

		filter "platforms:Linux"
			system "linux"
			defines {"RUSH_LINUX"}

		filter "platforms:Mac"
			system "macosx"
			defines {"RUSH_MAC"}
