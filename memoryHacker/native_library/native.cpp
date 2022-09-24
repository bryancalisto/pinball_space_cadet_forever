#include <iostream>
#include <Psapi.h>
#include <stdio.h>
#include <windows.h>
#include "native.h"
// #pragma comment(lib, "user32")

using namespace std;
// CONNECT C - Dart Guide: https://dart.dev/guides/libraries/c-interop

int main()
{
  toggleHack();
  return 0;
}

int toggleHack()
{
  printf("Toggle\n");
  return 0;
}

HWND findPinballWindow()
{
#ifdef DEBUG
  printf("Searching window with classname '%s'\n");
#endif

  HWND hWnd = FindWindowA("WaveMixSoundGuy", nullptr);

#ifdef DEBUG
  if (!hWnd)
  {
    cerr << "Couldn't find 'WaveMixSoundGuy' window" << endl;
  }
#endif

  return hWnd;
}

UINT_PTR getProcessBaseAddress(DWORD processID, HANDLE *handle)
{
  DWORD_PTR baseAddress = 0;

  HMODULE *moduleArray;
  LPBYTE moduleArrayBytes;
  DWORD bytesRequired;
  LPWSTR exeFilename = (LPWSTR)LocalAlloc(LPTR, 256);
  DWORD exeFilenameLen = 0;
  LPWSTR moduleFilename = (LPWSTR)LocalAlloc(LPTR, 1024);
  DWORD moduleFilenameLen = 0;
  wstring exeName = L"C:\\Users\\itsme007\\Documents\\Apps\\pinball\\pinball_original.exe";

  if (*handle)
  {
    exeFilenameLen = GetProcessImageFileNameW(*handle, exeFilename, 256);

    if (exeFilenameLen)
    {
#ifdef DEBUG
      printf("exe filename len: %d, %ls\n", exeFilenameLen, exeFilename);
#endif

      if (EnumProcessModulesEx(*handle, NULL, 0, &bytesRequired, 0x02))
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

            if (EnumProcessModulesEx(*handle, moduleArray, bytesRequired, &bytesRequired, 0x02))
            {
              for (int i = 0; i < moduleCount; i++)
              {
                moduleFilenameLen = GetModuleFileNameExW(*handle, moduleArray[i], moduleFilename, 1024);

                if (!moduleFilenameLen)
                {
                  printf("Error at GetModuleFileNameEx (0x%x): %d\n", (DWORD_PTR)moduleArray[i] + 0x00f6b7ec, GetLastError());
                  continue;
                }

                printf("module_exe: %ls\n", exeName);
                printf("module: %ls\n", moduleFilename);

                if (moduleFilename == exeName)
                {
                  // baseAddress = GetModuleInformation(*handle, moduleArray[i],)
                  printf("FOUND MODULE: %d\n", i);
                  baseAddress = (DWORD_PTR)moduleArray[i];
                  break;
                }
              }
            }

            LocalFree(moduleArrayBytes);
          }
        }
      }
    }
  }

  LocalFree(exeFilename);
  LocalFree(moduleFilename);

  return baseAddress;
}

HANDLE getProcessHandle(HWND hWnd, DWORD &PID)
{
#ifdef DEBUG
  printf("Getting process handle (PID: %d)\n", PID);
#endif

  GetWindowThreadProcessId(hWnd, &PID);
  HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, PID);

#ifdef DEBUG
  if (!handle)
  {
    cerr << "Couldn't get process handle" << endl;
  }
#endif

  return handle;
}

int writeMemory(HANDLE handle, UINT_PTR aimedAddress, int data, uint8_t dataSize)
{
  int result = WriteProcessMemory(handle, (LPVOID)aimedAddress, &data, dataSize, NULL);

#ifdef DEBUG
  printf("Writing 0x%x to [0x%x]\n", data, aimedAddress);
#endif

  if (result > 0)
  {
#ifdef DEBUG
    clog << "Memory written successfully" << endl;
#endif
  }
  else
  {
    printf("Memory couldn't be written: %d\n", GetLastError());
  }

  return result;
}

int readMemory(HANDLE handle, UINT_PTR aimedAddress, unsigned char *buffer, uint8_t nBytesToRead)
{
  int result = ReadProcessMemory(handle, (LPVOID)aimedAddress, &buffer, nBytesToRead, NULL);

#ifdef DEBUG
  printf("Reading %d bytes from [0x%x]\n", nBytesToRead, aimedAddress);
#endif

  if (result > 0)
  {
#ifdef DEBUG
    clog << "Memory read successfully" << endl;

    printf("[ ");

    while (buffer)
    {
      printf("%d ", *buffer);
    }

    printf("]");
#endif
  }
  else
  {
    printf("Memory couldn't be read: %d\n", GetLastError());
  }

  return result;
}

int toggleHack(bool activate)
{
  DWORD PID;
  int instruction = NOP_INSTRUCTION;

  if (!activate)
  {
    instruction = DEC_INSTRUCTION;
  }

  HWND hWnd;
  hWnd = findPinballWindow();

  if (!hWnd)
  {
    return NO_WINDOW;
  }

  HANDLE hProc = getProcessHandle(hWnd, PID);

  if (!hProc)
  {
    return NO_OPEN_PROCESS;
  }
  else
  {
    unsigned char buffer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT_PTR baseAddress = getProcessBaseAddress(PID, &hProc);
    UINT_PTR aimedAddressStatic = 0x000128F2;
    UINT_PTR aimedAddress = baseAddress + aimedAddressStatic;

    int readResult = readMemory(hProc, aimedAddress, buffer, 1);

    if (!readResult)
    {
      return READ_MEMORY;
    }

    int writeResult = writeMemory(hProc, aimedAddress, instruction, 1);

    if (!writeResult)
    {
      return WRITE_MEMORY;
    }

    CloseHandle(hProc);
  }

  return 0;
}
