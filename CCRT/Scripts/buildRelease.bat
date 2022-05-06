cd ..
set path="%cd%";%path%
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64

set final_build_dir="%cd%\Build\Release\ccrt.lib"
set final_build_path="%cd%\Build\Release"
set filenames=
setlocal enabledelayedexpansion
for /r "%cd%\Source\" %%a in (.) do (
    pushd %%a
    for %%f in (*.c) do (
    	if "%%~xf"==".c" set filenames=!filenames! "%%~dpnxa\%%f"
    )
    popd
)

set optimization_op=/O2
set codeGen_op=/arch:AVX2 /EHa- /fp:precise /fp:except- /fpcvt:BC /GA /Gd /GL /GR- /GS- /Gs9999999 /guard:cf- /Gw /TC
set preprocessor_op=/D_UNICODE /DUNICODE
set language_op=/std:c++20 /Zc:wchar_t- /Zc:rvalueCast- /openmp- /Zc:alignedNew- /Zc:char8_t- /Zc:__cplusplus- /Zc:externConstexpr /Zc:inline /Zl
set linking_op=/F4194304
set misc_op=/nologo /c
set diagnostics_op=/external:W0 /options:strict /sdl- /Wall

"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.31.31103\bin\Hostx64\x64\cl.exe" %optimization_op% %codeGen_op% %preprocessor_op% %language_op% %linking_op% %misc_op% %diagnostics_op% %filenames%

set objectnames=
for %%f in (*.obj) do (
    if "%%~xf"==".obj" set objectnames=!objectnames! %%f
)

set link_op=/OUT:%final_build_dir% /MACHINE:X64 /NODEFAULTLIB /NOLOGO /SUBSYSTEM:WINDOWS /LTCG /LIBPATH:"C:"

"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.31.31103\bin\Hostx64\x64\lib.exe" %link_op% %objectnames%
endlocal