@echo off
cd ..
set path="%cd%";%path%

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

set final_build_dir="%cd%\Build\Debug\ExampleApplication.exe"
set final_build_path="%cd%\Build\Debug"
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

set additional_includes=/I"C:\VulkanSDK\1.3.216.0\Include" /I"C:\VulkanSDK\1.3.216.0\Third-Party\Include" /I"%cd%\..\CSTD\Includes" /I"%cd%\..\Engine\Includes" /I"%cd%\Includes" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\ucrt"
set additional_lib_paths=/LIBPATH:"C:\VulkanSDK\1.3.216.0\Lib" /LIBPATH:"%cd%\Libs" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\ucrt\x64" /LIBPATH:"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.31.31103\lib\x64"
set additional_libs=kernel32.lib user32.lib libucrtd.lib libvcruntimed.lib libcmtd.lib cstdd.lib elixird.lib

set optimization_op=/Ot /Od /Oi-
set codeGen_op=/arch:AVX2 /EHa- /fp:precise /fp:except- /fpcvt:BC /GA /GL- /GR- /GS- /Gs9999999 /guard:cf- /TP
set preprocessor_op=/DDEBUG /D_UNICODE /DUNICODE /DVK_USE_PLATFORM_WIN32_KHR /DVK_NO_STDINT_H /DVK_NO_STDDEF_H
set language_op=/std:c++20 /Zc:wchar_t /Zc:rvalueCast- /openmp- /Zc:alignedNew- /Zc:char8_t- /Zc:__cplusplus /Zc:externConstexpr /Zc:inline- /Zi /Zl
set linking_op=/F4194304 %additional_includes%
set misc_op=/nologo
set diagnostics_op=/external:W0 /options:strict /sdl- /W2
set link_op=/link /OUT:%final_build_dir% %additional_libs% /DEBUG /MACHINE:X64 /NODEFAULTLIB /CGTHREADS:4 /NOLOGO /SUBSYSTEM:WINDOWS /STACK:0x400000,0x400000 /MANIFEST:NO /LTCG /CLRUNMANAGEDCODECHECK:NO /CLRSUPPORTLASTERROR:NO /PROFILE /GUARD:NO %additional_lib_paths%

"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.31.31103\bin\Hostx64\x64\cl.exe" %optimization_op% %codeGen_op% %preprocessor_op% %language_op% %linking_op% %misc_op% %diagnostics_op% %filenames% %link_op%

DEL *.obj

endlocal
