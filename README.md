# pinball_space_cadet_forever
A tool to get infinite balls in Pinball Space Cadet (the classic Windows game).

It overwrites the ball decreasing instruction with a NOP so that the ball counter is not decreased and also overwrites the UI's 'Demo' option functionality with code to trigger a game end manually and thus save your score.


The program has two versions:
- One that patches the static game `.exe` file.
- One that patches the binary at runtime from a native Windows executable.


## Static .exe patch usage
### To ****enable**** infinite balls
```python
python3 patchPinball.py pathToYourPinballProgram.exe patch
```


### To **disable** infinite balls
```python
python3 patchPinball.py pathToYourPinballProgram.exe unpatch
```


## **Finishing the game and saving your score**
- Just click the game UI's **Demo** option menu.

## Runtime patch workflow
The runtime patcher is now a native Windows executable built from `memoryHacker/native_library`.

### Prerequisites
- MinGW-w64 must be installed and `g++` must be available in `PATH`.

### Build and run
From the repository root, run:
```powershell
.\run-hot-patcher.ps1
```

That script:
- Builds `memoryHacker/native_library/native.exe` with MinGW using `memoryHacker/native_library/build.ps1`.
- Runs `native.exe enable` from the same directory.

### Manual native build
If you want to build the executable directly, run the MinGW command from `memoryHacker/native_library`:
```powershell
g++ -o native.exe native.cpp -lpsapi
```

### Manual runtime commands
From `memoryHacker/native_library`, you can run:
```powershell
.\native.exe enable
.\native.exe disable
.\native.exe status
```
