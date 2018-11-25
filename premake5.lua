builddir = "%{cfg.platform}-%{prj.architecture}-%{cfg.buildcfg}"
glfwbuilt = false

function use_GLFW()
	includedirs "Libraries/glfw-3.2.1/include"

	if not glfwbuilt then
		build_GLFW()
	end

	filter "platforms:Windows"
		libdirs {"build/libs/glfw-3.2.1/src/%{cfg.buildcfg}"}
		links {"OpenGL32"}

	filter "platforms:not Windows"
		libdirs{"build/libs/glfw-3.2.1/src"}
		links{"GL"}
		
	filter {}

	links {"glfw3"}

end

function build_GLFW()
	prebuildcommands{
		"{MKDIR} libs/glfw-3.2.1",
		"{CHDIR} libs/glfw-3.2.1"
	}

	filter "platforms:Windows"
		prebuildcommands {"cmake -G \"Visual Studio 15 2017 Win64\" ../../../Libraries/glfw-3.2.1"}

	filter "platforms:not Windows"
		prebuildcommands {"cmake ../../../Libraries/glfw-3.2.1"}

	filter {}

	prebuildcommands {"cmake --build ."}

	glfwbuilt = true
end

function use_rush()
	links {"Rush-Engine"}

	includedirs "Rush-Engine/include"

	filter "platforms:Windows"
		postbuildcommands {
			"copy /Y bin\\" .. builddir .. "\\Rush-Engine\\Rush.dll bin\\" ..builddir .. "\\%{prj.name}\\Rush.dll" ,
		}

	filter "platforms:not Windows"
		postbuildcommands {
			"{COPY} bin/" .. builddir .. "/Rush-Engine/Rush.dll bin/" ..builddir .. "/%{prj.name}/Rush.dll" ,
		}

	filter {}
end

workspace "Rush Engine"
	configurations {"Debug", "Release"}
	platforms {"Windows", "Linux", "Mac"}

	location "build"
	architecture "x64"
	systemversion("latest")

	project "Rush-Engine"
		targetname("Rush")
		kind "SharedLib"
		language "C++"
		cppdialect "C++17"

		files {"%{prj.name}/src/**.cpp", "%{prj.name}/src/**.h"}
		use_GLFW()

		targetdir("build/bin/" .. builddir .. "/%{prj.name}")
		objdir("build/objs/" .. builddir .. "/%{prj.name}")

		defines {"RUSH_BUILD_SHARED", "RUSH_OPENGL"} -- TODO: Add other graphics APIs

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

		use_rush()

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
