@echo off
cd ..
set path="%cd%";%path%

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

set final_build_dir="%cd%\Build\Release\elixir.lib"
set final_build_path="%cd%\Build\Release"
set filenames=
setlocal enabledelayedexpansion
for /r "%cd%\Source\" %%a in (.) do (
    pushd %%a
    for %%f in (*.cpp) do (
        if "%%~xf"==".cpp" set filenames=!filenames! "%%~dpnxa\%%f"
    )

    for %%f in (*.c) do (
        if "%%~xf"==".c" set filenames=!filenames! "%%~dpnxa\%%f"
    )
    popd
)

set additional_includes=/I"C:\VulkanSDK\1.3.216.0\Include" /I"C:\VulkanSDK\1.3.216.0\Third-Party\Include" /I"%cd%\..\CSTD\Includes" /I"%cd%\Includes" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\ucrt"
set optimization_op=/O2 /Oi-
set codeGen_op=/arch:AVX2 /EHa- /fp:precise /fp:except- /fpcvt:BC /GA /GL- /GR- /GS- /Gs9999999 /guard:cf- /TP
set preprocessor_op=/D_UNICODE /DUNICODE /DVK_USE_PLATFORM_WIN32_KHR /DVK_NO_STDINT_H /DVK_NO_STDDEF_H
set language_op=/std:c++20 /Zc:wchar_t /Zc:rvalueCast- /openmp- /Zc:alignedNew- /Zc:char8_t- /Zc:__cplusplus /Zc:externConstexpr /Zc:inline- /Zi /Zl
set linking_op=/F4194304 %additional_includes%
set misc_op=/nologo /c
set diagnostics_op=/external:W0 /options:strict /sdl- /W2

"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.31.31103\bin\Hostx64\x64\cl.exe" %optimization_op% %codeGen_op% %preprocessor_op% %language_op% %linking_op% %misc_op% %diagnostics_op% %filenames%

set objectnames=
for %%f in (*.obj) do (
    if "%%~xf"==".obj" set objectnames=!objectnames! %%f
)

set link_op=/OUT:%final_build_dir% /MACHINE:X64 /NODEFAULTLIB /NOLOGO /SUBSYSTEM:CONSOLE /LTCG

"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.31.31103\bin\Hostx64\x64\lib.exe" %link_op% %objectnames%

DEL *.obj

endlocal
