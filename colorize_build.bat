@ECHO OFF

echo "Building coloryze.exe..."

RMDIR /S /Q build >NUL
MKDIR build >NUL

g++ -O3 -static-libgcc -static-libstdc++ -o build\tmp.exe colorize.cpp
if NOT ["%errorlevel%"]==["0"] RMDIR /S /Q build >NUL && pause

strip colorize\tmp.exe
if NOT ["%errorlevel%"]==["0"] RMDIR /S /Q build >NUL && pause

upx -9 -obuild\colorize.exe build\tmp.exe
if NOT ["%errorlevel%"]==["0"] RMDIR /S /Q colorize >NUL && pause

del build\tmp.exe > nul
