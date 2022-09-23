cc=cl
link=link
all: "memoryHacker.exe"

.cpp.obj:
  $(cc) $(cdebug) $(cflags) $(cvars) $*.cpp

memoryHacker.exe: main.obj assembly.obj
  $(link) $(ldebug) $(conflags) -out:"memoryHacker.exe" $**