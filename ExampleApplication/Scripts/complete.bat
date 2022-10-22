cd "..\..\Engine\Scripts"
cmd /c complete.bat
cd "..\..\ExampleApplication\Scripts"
cmd /c grabLibs.bat
cmd /c buildDebug.bat
cmd /c buildRelease.bat