@echo off
if not exist build mkdir build
call .\tools\sokol-shdc.exe -i src/shaders.glsl -o build/shaders.glsl.h -l hlsl5