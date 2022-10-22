@echo off
cd ..
set path="%cd%";%path%

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

set final_build_dir="%cd%\Build\Release\cstd.lib"
set final_build_path="%cd%\Build\Release"
set final_src_path=%cd%
set filenames=
set asm_filenames=
setlocal enabledelayedexpansion
for /r "%cd%\Source\" %%a in (.) do (
    pushd %%a
    for %%f in (*.cpp) do (
    	if "%%~xf"==".cpp" set filenames=!filenames! "%%~dpnxa\%%f"
    )
    for %%f in (*.asm) do (
        if "%%~xf"==".asm" set asm_filenames=!asm_filenames! "%%~dpnxa\%%f" & nasm -f win64 "%%~dpnxa\%%f" -o "%final_src_path%\%%~nf.obj"
    )
    popd
)

set additional_includes=/I"%cd%\Includes" /I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\ucrt"
set optimization_op=/O2 /Oi-
set codeGen_op=/arch:AVX2 /EHa- /fp:precise /fp:except- /fpcvt:BC /GA /GL- /GR- /GS- /Gs9999999 /guard:cf- /TP
set preprocessor_op=/D_UNICODE /DUNICODE
set language_op=/std:c++20 /Zc:wchar_t /Zc:rvalueCast- /openmp- /Zc:alignedNew- /Zc:char8_t- /Zc:__cplusplus /Zc:externConstexpr /Zc:inline- /Zi /Zl
set linking_op=/F4194304 %additional_includes%
set misc_op=/nologo /c
set diagnostics_op=/external:W0 /options:strict /sdl- /W1

"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.31.31103\bin\Hostx64\x64\cl.exe" %optimization_op% %codeGen_op% %preprocessor_op% %language_op% %linking_op% %misc_op% %diagnostics_op% %filenames%

set objectnames=
for %%f in (*.obj) do (
    if "%%~xf"==".obj" set objectnames=!objectnames! %%f
)

set link_op=/OUT:%final_build_dir% %additional_libs% /MACHINE:X64 /NODEFAULTLIB /NOLOGO /SUBSYSTEM:CONSOLE /LTCG

"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.31.31103\bin\Hostx64\x64\lib.exe" %link_op% %objectnames%

DEL *.obj

endlocal