cd ..
set path="%cd%";%path%

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64

set final_build_dir="%cd%\Build\Debug\Elixir.exe"
set filenames=
cd Source
setlocal enabledelayedexpansion
for %%f in (*.cpp) do (
    if "%%~xf"==".cpp" set filenames=!filenames! %%f
)

for %%f in (*.c) do (
    if "%%~xf"==".c" set filenames=!filenames! %%f
)
set filenames=!filenames:~1!
set additional_includes=/I"C:\VulkanSDK\1.3.211.0\Include" /I"C:\VulkanSDK\1.3.211.0\Third-Party\Include" /I"%cd%\CCRT\Includes"
set additional_lib_paths=/LIBPATH:"C:\VulkanSDK\1.3.211.0\Lib" /LIBPATH:"%cd%\..\Libs"
set additional_libs=kernel32.lib user32.lib ccrtd.lib

set optimization_op=/Ot /Od /Oi
set codeGen_op=/arch:AVX2 /EHa- /fp:precise /fp:except- /fpcvt:BC /GA /Gd /GL- /GR- /GS- /Gs9999999 /guard:cf- /Gw- /TP
set preprocessor_op=/DDEBUG /D_UNICODE /DUNICODE /DVK_USE_PLATFORM_WIN32_KHR
set language_op=/std:c++20 /Zc:wchar_t- /Zc:rvalueCast- /openmp- /Zc:alignedNew- /Zc:char8_t- /Zc:__cplusplus /Zc:externConstexpr /Zc:inline- /Zi
set linking_op=/F4194304 %additional_includes%
set misc_op=/nologo
set diagnostics_op=/external:W0 /options:strict /sdl- /W2
set link_op=/link /OUT:%final_build_dir% %additional_libs% /DEBUG /MACHINE:X64 /ENTRY:wWinMainCRTStartup /NODEFAULTLIB /CGTHREADS:4 /NOLOGO /SUBSYSTEM:WINDOWS /STACK:0x400000,0x400000 /MANIFEST:NO /LTCG /PROFILE /GUARD:NO %additional_lib_paths%

"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.31.31103\bin\Hostx64\x64\cl.exe" %optimization_op% %codeGen_op% %preprocessor_op% %language_op% %linking_op% %misc_op% %diagnostics_op% %filenames% %link_op%
endlocal
