@echo off
set PROJECT_PATH=%~dp0
echo mksymlink start!
pushd %PROJECT_PATH%
echo Dir is %CD%
if not exist HCppBox\CMakeLists.txt  mklink /D /J HCppBox  ..\3rdparty\HCppBox\master\
popd
echo mksymlink end!
