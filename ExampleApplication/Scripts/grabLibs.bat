@echo off
set grab_path_sd="%cd%\..\..\CSTD\Build\Debug\cstdd.lib"
set grab_path_sr="%cd%\..\..\CSTD\Build\Release\cstd.lib"
set grab_path_er="%cd%\..\..\Engine\Build\Release\elixir.lib"
set grab_path_ed="%cd%\..\..\Engine\Build\Debug\elixird.lib"
set final_path="%cd%\..\Libs"

xcopy /s /Y %grab_path_sd% %final_path%
xcopy /s /Y %grab_path_sr% %final_path%
xcopy /s /Y %grab_path_ed% %final_path%
xcopy /s /Y %grab_path_er% %final_path%
