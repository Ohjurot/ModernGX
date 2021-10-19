[void](New-Item -ItemType Directory -Force -Path "./publish/")
[void](New-Item -ItemType Directory -Force -Path "./publish/include/")
[void](New-Item -ItemType Directory -Force -Path "./publish/release/")
[void](New-Item -ItemType Directory -Force -Path "./publish/debug/")
Get-ChildItem "./ModernGXLib/src" | Copy-Item -Destination "./publish/include" -Recurse -Force -filter "*.h"
Get-ChildItem "./publish/include" -recurse -include *.cpp | remove-item
Copy-Item "./LICENSE" -Destination "./publish" -Force
Copy-Item "./bin/Release/ModernGXLib.lib" -Destination "./publish/release" -Force
Copy-Item "./bin/Debug/ModernGXLib.lib" -Destination "./publish/debug" -Force
