#include <iostream>
#include <windows.h>
// #include "assembly.h"
#pragma comment(lib, "user32")

using namespace std;

/**
 * NOTES:
 * 1. Can find the program with 'WaveMixSoundGuy' class.
 */
HWND findPinballWindow()
{
  char *names[] = {"WavMix32", "3DPB_SPLASH_CLASS", "WaveMixSoundGuy"};
  int nNames = 3;
  HWND hWnd = nullptr;

  for (int i = 0; i < nNames; i++)
  {
    printf("Searching %s\n", names[i]);
    hWnd = FindWindowA(names[i], nullptr);

    if (hWnd)
    {
      return hWnd;
    }
  }

  return nullptr;
}

int main(void)
{

  unsigned char buffer[10];

  HWND hWnd = findPinballWindow();

  int newballCount = 4;

  // int eip = get_eip();
  // printf("EIP => 0x%x\n", eip);

  if (hWnd == nullptr)
  {
    cerr << "Could not find window." << endl;
  }
  else
  {
    DWORD PID;
    GetWindowThreadProcessId(hWnd, &PID);
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, PID);

    if (!hProc)
    {
      cerr << "Cannot open process." << endl;
    }
    else
    {
      int uiBallCounter = 0x02F9BF5C;
      int score = 0x02f9c13c;
      int stat = WriteProcessMemory(hProc, (LPVOID)score, &newballCount, (DWORD)sizeof(newballCount), NULL);
      // int stat = ReadProcessMemory(hProc, (LPVOID)0x00011CF2, &buffer, (DWORD)sizeof(buffer), NULL);

      if (stat > 0)
      {
        clog << "Memory written to process." << endl;
        // cout << "BUFF: " << buffer << endl;
      }
      else
      {
        cerr << "Memory couldn't be written to process." << endl;
      }

      CloseHandle(hProc);

      // std::cin.get();
    }
  }

  return 0;
}