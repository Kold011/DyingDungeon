include "Dependencies.lua"

workspace "Dying Dungeon"
    configurations { "Debug", "Release" }
	
    targetdir "%{wks.location}/Build/%{cfg.buildcfg}"
	objdir "%{wks.location}/Intermediates/%{cfg.buildcfg}"
    
    startproject "Dying Dungeon"
    
	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "language:C++ or language:C"
		architecture "x86_64"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}"

group "Dying Dungeon"
include "Projects/DyingDungeon"
include "Projects/Odyssey"
group ""