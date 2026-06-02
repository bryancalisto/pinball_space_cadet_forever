mkdir -Force build
cl /EHsc /Fe:build/pinball-hacker.exe src/main.cpp /link psapi.lib user32.lib kernel32.lib