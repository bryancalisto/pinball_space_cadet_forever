#include <windows.h>
#include <iostream>
#include <psapi.h>
#include <stdio.h>
#include <string>
#include "native.h"

using namespace std;

int main()
{
  // toggleHack(false);
  // printf("IS ACTIVE: %d\n", isHackActive());
  // toggleHack(true);
  // printf("IS ACTIVE: %d\n", isHackActive());
  // getExeFilePath();
  return 0;
}

HWND findPinballWindow()
{
#ifdef DEBUG
  printf("Searching window with classname 'WaveMixSoundGuy'\n");
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

wstring getExeFilePath()
{
  DWORD exeFilePathBufferLen = MAX_PATH;
  DWORD processID = 0;
  wstring exeFilePath;

  HWND hWnd = findPinballWindow();

  if (!hWnd)
  {
    printf("No pinball window\n");
    return nullptr;
  }

  HANDLE hProc = getProcessHandle(hWnd, processID);

  if (!hProc)
  {
    printf("No open process\n");
    return nullptr;
  }

  LPWSTR exeFilePathBuffer = (LPWSTR)LocalAlloc(LPTR, exeFilePathBufferLen);
  BOOL result = QueryFullProcessImageNameW(hProc, 0, exeFilePathBuffer, &exeFilePathBufferLen);

  if (result)
  {
    exeFilePath = exeFilePathBuffer;
#ifdef DEBUG
    wcout << "File path: " << exeFilePath << endl;
#endif
  }
  else
  {
    printf("Exe file path not found\n");
  }

  LocalFree(exeFilePathBuffer);

  return exeFilePath;
}

UINT_PTR getProcessBaseAddress(DWORD processID, HANDLE *handle)
{
  UINT_PTR baseAddress = 0;

  HMODULE *moduleArray;
  LPBYTE moduleArrayBytes;
  DWORD bytesRequired;
  DWORD exeFilenameBufferLen = 256;
  LPWSTR exeFilename = (LPWSTR)LocalAlloc(LPTR, exeFilenameBufferLen);
  LPWSTR moduleFilename = (LPWSTR)LocalAlloc(LPTR, 256);
  DWORD moduleFilenameLen = 0;

  if (*handle)
  {
    BOOL result = QueryFullProcessImageNameW(*handle, 0, exeFilename, &exeFilenameBufferLen);

    if (result)
    {
#ifdef DEBUG
      printf("exe filename '%ls'\n", exeFilename);
#endif

      if (EnumProcessModulesEx(*handle, NULL, 0, &bytesRequired, 0x01))
      {
        if (bytesRequired)
        {
          moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

          if (moduleArrayBytes)
          {
            int moduleCount;

            moduleCount = bytesRequired / sizeof(HMODULE);
#ifdef DEBUG
            printf("Module count: %d\n", moduleCount);
#endif
            moduleArray = (HMODULE *)moduleArrayBytes;

            if (EnumProcessModulesEx(*handle, moduleArray, bytesRequired, &bytesRequired, 0x01))
            {
              for (int i = 0; i < moduleCount; i++)
              {
                moduleFilenameLen = GetModuleFileNameExW(*handle, moduleArray[i], moduleFilename, 256);

                if (!moduleFilenameLen)
                {
#ifdef DEBUG
                  printf("Error at GetModuleFileNameEx (0x%p): %d\n", (void *)moduleArray[i], GetLastError());
#endif
                  continue;
                }

                if (*moduleFilename == *exeFilename)
                {
#ifdef DEBUG
                  printf("Found .exe module\n");
#endif
                  baseAddress = (UINT_PTR)moduleArray[i];
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
  GetWindowThreadProcessId(hWnd, &PID);

#ifdef DEBUG
  printf("Getting process handle (PID: %d)\n", PID);
#endif

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
  printf("Writing 0x%x to [0x%p]\n", data, (void *)aimedAddress);
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
  int result = ReadProcessMemory(handle, (LPVOID)aimedAddress, buffer, nBytesToRead, NULL);

#ifdef DEBUG
  printf("Reading %d bytes from [0x%p]\n", nBytesToRead, (void *)aimedAddress);
#endif

  if (result > 0)
  {
#ifdef DEBUG
    clog << "Memory read successfully" << endl;

    printf("[ ");

    for (int i = 0; i < nBytesToRead; i++)
    {
      printf("0x%x ", *(buffer + i));
    }

    printf("]\n");
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
  DWORD PID = 0;
  int instruction = NOP_INSTRUCTION;

  if (!activate)
  {
    instruction = DEC_INSTRUCTION;
  }

  HWND hWnd = findPinballWindow();

  if (!hWnd)
  {
    return NO_WINDOW;
  }

  HANDLE hProc = getProcessHandle(hWnd, PID);

  if (!hProc)
  {
    return NO_OPEN_PROCESS;
  }

  unsigned char buffer;
  UINT_PTR baseAddress = getProcessBaseAddress(PID, &hProc);
#ifdef DEBUG
  printf("Base address: 0x%p\n", (void *)baseAddress);
#endif
  UINT_PTR aimedAddress = baseAddress + DEC_INSTRUCTION_ADDRESS;

  int readResult = readMemory(hProc, aimedAddress, &buffer, 1);

  if (!readResult)
  {
    return READ_MEMORY;
  }

  if (buffer != DEC_INSTRUCTION && buffer != NOP_INSTRUCTION)
  {
    return UNEXPECTED_BYTECODE;
  }

  if ((activate && buffer == NOP_INSTRUCTION) || (!activate && buffer == DEC_INSTRUCTION))
  {
    return NO_OPERATION;
  }

  int writeResult = writeMemory(hProc, aimedAddress, instruction, 1);

  if (!writeResult)
  {
    return WRITE_MEMORY;
  }

  CloseHandle(hProc);

  return 0;
}

int isHackActive()
{
  DWORD PID = 0;

  HWND hWnd = findPinballWindow();

  if (!hWnd)
  {
    return NO_WINDOW;
  }

  HANDLE hProc = getProcessHandle(hWnd, PID);

  if (!hProc)
  {
    return NO_OPEN_PROCESS;
  }

  unsigned char buffer;
  UINT_PTR baseAddress = getProcessBaseAddress(PID, &hProc);
#ifdef DEBUG
  printf("Base address: 0x%p\n", (void *)baseAddress);
#endif
  UINT_PTR aimedAddress = baseAddress + DEC_INSTRUCTION_ADDRESS;

  int readResult = readMemory(hProc, aimedAddress, &buffer, 1);

  if (!readResult)
  {
    return READ_MEMORY;
  }

  if (buffer != DEC_INSTRUCTION && buffer != NOP_INSTRUCTION)
  {
    return UNEXPECTED_BYTECODE;
  }

  bool result = buffer != DEC_INSTRUCTION;

  CloseHandle(hProc);

  return result;
}
