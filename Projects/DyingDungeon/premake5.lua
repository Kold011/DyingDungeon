project "Dying Dungeon"
    language "C++"
    cppdialect "C++17"
    kind "ConsoleApp"
    architecture "x86_64"
    staticruntime "Off"
    dependson "Odyssey"

    flags { "NoPCH", "MultiProcessorCompile" }
    
    files {
        "src/**.h",
        "src/**.inl",
        "src/**.cpp",
        "src/**.hpp",
        "src/**.hlsl",
    }

    includedirs {
        "src",
        "src/**",
    }

    externalincludedirs {
        "%{wks.location}/Projects/Odyssey/inc",
        "%{wks.location}/Projects/Odyssey/inc/**",
    }
    
    links {
        "Odyssey",
    }

    filter "action:vs*"
        linkoptions { "/ignore:4098", "/ignore:4099" } -- Disable no PDB found warning
        disablewarnings { "4068" } -- Disable "Unknown #pragma mark warning"
        
    filter { "configurations:Debug" }
        runtime "Debug"
		symbols "On"
        ProcessDependencies("Debug")
        
    filter { "configurations:Release" }
        runtime "Release"
        symbols "On"
        ProcessDependencies("Release")