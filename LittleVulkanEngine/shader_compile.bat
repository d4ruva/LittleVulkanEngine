@echo off

call %VULKAN_SDK%\\Bin\\glslc.exe shaders\simple_shader.vert -o shaders\simple_shader.vert.spv
call %VULKAN_SDK%\\Bin\\glslc.exe shaders\simple_shader.frag -o shaders\simple_shader.frag.spv

pause