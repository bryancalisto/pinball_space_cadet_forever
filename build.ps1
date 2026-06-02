mkdir -Force build
cl /EHsc /std:c++20 /Fe:build/pinball-hacker.exe src/main.cpp /link psapi.lib user32.lib kernel32.lib