@echo off
if not exist build mkdir build

call build_shaders.bat

pushd build
cl /nologo ../src/main.c /DSOKOL_D3D11 /DSOKOL_IMPL /I../ext /I. /link /LTCG /LIBPATH:../lib cimgui.lib
popd