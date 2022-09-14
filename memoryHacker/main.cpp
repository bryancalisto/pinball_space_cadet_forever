#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <Psapi.h>
// #include "assembly.h"
#pragma comment(lib, "user32")

using namespace std;

/**
 * NOTES:
 * 1. Can find the program with 'WaveMixSoundGuy' class.
 * 2. Get image base address https://stackoverflow.com/questions/42015819/reading-an-address-relative-to-the-base-address-from-a-process-with-readprocessm ,
 * https://stackoverflow.com/questions/9545732/what-is-hmodule
 * https://stackoverflow.com/questions/14467229/get-base-address-of-process
 */
HWND findPinballWindow()
{
  char *names[] = {"WaveMixSoundGuy"};
  int nNames = 1;
  HWND hWnd = nullptr;

  for (int i = 0; i < nNames; i++)
  {
    printf("Searching window with classname '%s'\n", names[i]);
    hWnd = FindWindowA(names[i], nullptr);

    if (hWnd)
    {
      return hWnd;
    }
  }

  return nullptr;
}

UINT_PTR GetProcessBaseAddress(DWORD processID, HANDLE *processHandle)
{
  DWORD_PTR baseAddress = 0;

  HMODULE *moduleArray;
  LPBYTE moduleArrayBytes;
  DWORD bytesRequired;

  if (*processHandle)
  {
    if (EnumProcessModulesEx(*processHandle, NULL, 0, &bytesRequired, 0x02))
    {
      if (bytesRequired)
      {
        moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

        if (moduleArrayBytes)
        {
          unsigned int moduleCount;

          moduleCount = bytesRequired / sizeof(HMODULE);
          printf("Module count: %d\n", moduleCount);
          moduleArray = (HMODULE *)moduleArrayBytes;

          if (EnumProcessModulesEx(*processHandle, moduleArray, bytesRequired, &bytesRequired, 0x02))
          {
            baseAddress = (DWORD_PTR)moduleArray[0];
          }

          LocalFree(moduleArrayBytes);
        }
      }
    }
  }

  return baseAddress;
}

int main(void)
{

  unsigned char buffer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  HWND hWnd = findPinballWindow();

  int newballCount = 8;

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
      exit(1);
    }
    else
    {
      UINT_PTR baseAddress = GetProcessBaseAddress(PID, &hProc);
      UINT32 decAddressStatic = 0x00011CF2;
      // UINT32 decAddressStatic = 0x0001479B;
      int decAddress = baseAddress + decAddressStatic;
      printf("Base address 0x%x\n", decAddress);
      int uiBallCounter = 0x0498bba4;
      int score = 0x02f9c13c;
      int stat = WriteProcessMemory(hProc, (LPVOID)decAddress, &newballCount, (DWORD)sizeof(newballCount), NULL);
      printf("Reading 0x%x\n", decAddress);
      // int stat = ReadProcessMemory(hProc, (LPVOID)decAddress, &buffer, 10, NULL);

      if (stat > 0)
      {
        clog << "Memory processed." << endl;
        printf("BUFF: 0x%x\n", *buffer);
      }
      else
      {
        cerr << "Memory couldn't be processed: " << GetLastError() << endl;
      }

      CloseHandle(hProc);

      // std::cin.get();
    }
  }

  return 0;
}