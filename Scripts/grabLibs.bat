@set grab_path_r="%cd%\..\CCRT\Build\Release\ccrt.lib"
@set grab_path_d="%cd%\..\CCRT\Build\Debug\ccrtd.lib"
@set final_path="%cd%\..\Libs"

xcopy /s %grab_path_d% %final_path%
xcopy /s %grab_path_r% %final_path%
