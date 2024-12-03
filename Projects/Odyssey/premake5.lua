project "Odyssey"
    language "C++"
    cppdialect "C++17"
    kind "StaticLib"
    architecture "x86_64"
    staticruntime "Off"
    
    flags { "NoPCH", "MultiProcessorCompile" }
    
    files {
        "inc/**.h",
        "inc/**.inl",
        "inc/**.cpp",
        "inc/**.hpp",
        "inc/**.hlsl",
        "src/**.h",
        "src/**.inl",
        "src/**.cpp",
        "src/**.hpp",
        "src/**.hlsl",
        "src/**.c",
    }

    includedirs {
        "inc",
        "inc/**",
        "src",
        "src/**",
        "shaders",
        "shaders/**",
    }

    IncludeDependencies()
    
    filter "action:vs*"
        linkoptions { "/ignore:4099", "/ignore:4006" } -- Disable no PDB found warning
        disablewarnings { "4068" } -- Disable "Unknown #pragma mark warning"
        
    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        runtime "Release"
        symbols "Off"
        optimize "On"