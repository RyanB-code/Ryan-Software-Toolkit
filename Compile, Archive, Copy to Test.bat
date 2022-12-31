@echo off
cd "C:\dev\VS Code Projects\RST-Static-Lib\Object Files"
g++ -c -std=c++20 "C:\dev\VS Code Projects\RST-Static-Lib/src/*.cpp"
cd "C:\dev\VS Code Projects\RST-Static-Lib"
ar rcs "C:\dev\VS Code Projects\RST-Static-Lib\libRST.a" "C:\dev\VS Code Projects\RST-Static-Lib\Object Files\*.o"
copy "C:\dev\VS Code Projects\RST-Static-Lib\libRST.a" "C:\dev\VS Code Projects\RST-Static-Lib\Test Code\lib\"
copy "C:\dev\VS Code Projects\RST-Static-Lib\src\RST.h" "C:\dev\VS Code Projects\RST-Static-Lib\Test Code\include\"