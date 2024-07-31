@echo off
if not exist build mkdir build

call build_shaders.bat

pushd build
cl ../src/main.c /DSOKOL_D3D11 /DSOKOL_IMPL /I..\..\sokol /I.
popd