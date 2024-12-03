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
        "shaders/**.hlsl",
        "shaders/**.hlsli",
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
        
    filter { "files:**.hlsl" }
        flags "ExcludeFromBuild"
        shadermodel "5.0"

    filter { "files:**.hlsli" }
        flags "ExcludeFromBuild"
        shadermodel "5.0"

    filter { "files:**.frag.hlsl" }
        removeflags "ExcludeFromBuild"
        shadertype "Pixel"
        shaderentry "main"

    filter { "files:**.vert.hlsl" }
        removeflags "ExcludeFromBuild"
        shadertype "Vertex"
        shaderentry "main"

    filter { "files:**.comp.hlsl" }
        removeflags "ExcludeFromBuild"
        shadertype "Compute"
        shaderentry "main"

    filter { "files:**.geo.hlsl" }
        removeflags "ExcludeFromBuild"
        shadertype "Geometry"
        shaderentry "main"

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        runtime "Release"
        symbols "On"
        optimize "On"