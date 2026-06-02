#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <iostream>
#include <psapi.h>
#include <cstdio>
#include <string>
#include <cstdint>

#pragma comment(lib, "psapi.lib")

#define DEBUG

#define DEC_INSTRUCTION_ADDRESS 0x000128F2 // Where the instruction that decreases balls is

#define TITLE_STR_ADDRESS 0x00048a98   // Where the "3D Pinball for Windows - Space Cadet" string is
#define PLAYER1_STR_ADDRESS 0x00049d96 // Where the "Player 1" string is

// INSTRUCTIONS
#define NOP_INSTRUCTION 0x90
#define DEC_INSTRUCTION 0x48

// ERRORS
#define NO_WINDOW_ERR -1           // Couldn't get the window handle
#define NO_OPEN_PROCESS_ERR -2     // Couldn't open the process based on the window handle
#define READ_MEMORY_ERR -3         // Couldn't read the process memory
#define WRITE_MEMORY_ERR -4        // Couldn't write the process memory
#define NO_OPERATION_ERR -5        // Based on the read byte from the process memory, it was concluded that the operation was already made so no need to execute it again
#define UNEXPECTED_BYTECODE_ERR -6 // Detects an unexpected instruction in the byte that is used for the hack
#define NO_PROCESS_HANDLE_ERR -7
#define VIRTUAL_PROTECT_ERR -8

LPWSTR getExeFilePath();
HANDLE getProcessHandle(HWND hWnd, DWORD &PID);
UINT_PTR getProcessBaseAddress(DWORD processID, HANDLE *handle);
HWND findPinballWindow();
int readMemory(HANDLE handle, UINT_PTR aimedAddress, unsigned char *buffer, uint8_t nBytesToRead);
int writeMemory(const HANDLE handle, UINT_PTR aimedAddress, uint8_t data[], uint8_t dataSize);
int toggleHack(bool activate);
int isHackActive();

using namespace std;

template <typename... Args>
void logDebug(const char *format, Args... args)
{
#ifdef DEBUG
  printf(format, args...);
#endif
}

template <typename... Args>
void logDebugW(Args... args)
{
#ifdef DEBUG
  (wcout << ... << args) << endl;
#endif
}

static void printUsage(const char *programName)
{
  cout << "Usage: " << programName << " <on|off|status>" << endl;
}

int main(int argc, char **argv)
{
  cout << endl;

  if (argc < 2)
  {
    printUsage(argv[0]);
    return 1;
  }

  string command = argv[1];

  logDebug("Running in DEBUG mode\n\n");

  if (command == "on")
  {
    return toggleHack(true);
  }

  if (command == "off")
  {
    return toggleHack(false);
  }

  if (command == "status")
  {
    int result = isHackActive();

    if (result >= 0)
    {
      printf("Hack is %s\n", result ? "on" : "off");
    }

    return result;
  }

  printUsage(argv[0]);
  return 1;
}

/**
 * NOTES:
 * 1. Can find the program with 'WaveMixSoundGuy' class.
 * 2. Get image base address https://stackoverflow.com/questions/42015819/reading-an-address-relative-to-the-base-address-from-a-process-with-readprocessm ,
 * https://stackoverflow.com/questions/9545732/what-is-hmodule
 * https://stackoverflow.com/questions/14467229/get-base-address-of-process
 */
HWND findPinballWindow()
{
  logDebug("Searching window with classname 'WaveMixSoundGuy'");

  HWND hWnd = FindWindowA("WaveMixSoundGuy", nullptr);

  if (!hWnd)
  {
    cerr << "Couldn't find Pinball ('WaveMixSoundGuy') window" << endl;
    exit(NO_WINDOW_ERR);
  }

  return hWnd;
}

LPWSTR getExeFilePath()
{
  DWORD exeFilePathBufferLen = MAX_PATH;
  DWORD processID = 0;

  HWND hWnd = findPinballWindow();

  HANDLE hProc = getProcessHandle(hWnd, processID);

  if (!hProc)
  {
    printf("No open process\n");
    exit(NO_OPEN_PROCESS_ERR);
  }

  LPWSTR exeFilePathBuffer = (LPWSTR)LocalAlloc(LPTR, exeFilePathBufferLen);
  BOOL result = QueryFullProcessImageNameW(hProc, 0, exeFilePathBuffer, &exeFilePathBufferLen);

  if (result)
  {
    logDebugW("File path:", exeFilePathBuffer);
    return exeFilePathBuffer;
  }
  else
  {
    printf("Exe file path not found\n");
    return nullptr;
  }
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
      logDebug("exe filename '%ls'\n", exeFilename);

      if (EnumProcessModulesEx(*handle, NULL, 0, &bytesRequired, 0x01))
      {
        if (bytesRequired)
        {
          moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

          if (moduleArrayBytes)
          {
            int moduleCount;

            moduleCount = bytesRequired / sizeof(HMODULE);
            logDebug("Module count: %d\n", moduleCount);
            moduleArray = (HMODULE *)moduleArrayBytes;

            if (EnumProcessModulesEx(*handle, moduleArray, bytesRequired, &bytesRequired, 0x01))
            {
              for (int i = 0; i < moduleCount; i++)
              {
                moduleFilenameLen = GetModuleFileNameExW(*handle, moduleArray[i], moduleFilename, 256);

                if (!moduleFilenameLen)
                {
                  logDebug("Error at GetModuleFileNameEx (0x%p): %d\n", (void *)moduleArray[i], GetLastError());
                  continue;
                }

                if (*moduleFilename == *exeFilename)
                {
                  logDebug("Found .exe module\n");
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

  logDebug("Getting process handle (PID: %d)\n", PID);

  HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, PID);

  if (!handle)
  {
    cerr << "Couldn't get process handle" << endl;
    exit(NO_PROCESS_HANDLE_ERR);
  }

  return handle;
}

int writeMemory(const HANDLE hProc, UINT_PTR aimedAddress, uint8_t data[], uint8_t dataSize)
{
  DWORD oldProtection;

  if (!VirtualProtectEx(hProc, (PVOID)aimedAddress, dataSize, PAGE_EXECUTE_READWRITE, &oldProtection))
  {
    cerr << "Failed to set memory protection to allow writing. Error: " << GetLastError() << endl;
    CloseHandle(hProc);
    return VIRTUAL_PROTECT_ERR;
  }

  int result = WriteProcessMemory(hProc, (LPVOID)aimedAddress, data, dataSize, NULL);

  logDebug("Writing data to [0x%p]\n", data, (void *)aimedAddress);

  logDebug("Data: ");
  for (int i = 0; i < dataSize; i++)
  {
    logDebug("%02X ", data[i]);
  }
  logDebug("\n");

  if (result > 0)
  {
    logDebug("Memory written successfully\n");
    if (!VirtualProtectEx(hProc, (PVOID)aimedAddress, dataSize, oldProtection, &oldProtection))
    {
      cerr << "Failed to restore memory protection. Error: " << GetLastError() << endl;
      CloseHandle(hProc);
      return VIRTUAL_PROTECT_ERR;
    }
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

  logDebug("Reading %d bytes from [0x%p]\n", nBytesToRead, (void *)aimedAddress);

  if (result > 0)
  {
    logDebug("Memory read successfully\n");
    logDebug("[ ");

    for (int i = 0; i < nBytesToRead; i++)
    {
      logDebug("0x%x ", *(buffer + i));
    }

    logDebug("]\n");
  }
  else
  {
    logDebug("Memory couldn't be read: %d\n", GetLastError());
  }

  return result;
}

int toggleHack(bool activate)
{
  DWORD PID = 0;

  HWND hWnd = findPinballWindow();

  if (!hWnd)
  {
    return NO_WINDOW_ERR;
  }

  HANDLE hProc = getProcessHandle(hWnd, PID);

  if (!hProc)
  {
    return NO_OPEN_PROCESS_ERR;
  }

  uint8_t instructionValue;
  UINT_PTR baseAddress = getProcessBaseAddress(PID, &hProc);
  logDebug("Base address: 0x%p\n", (void *)baseAddress);
  UINT_PTR decInstructionAddress = baseAddress + DEC_INSTRUCTION_ADDRESS;
  UINT_PTR titleStrAddress = baseAddress + TITLE_STR_ADDRESS;
  UINT_PTR player1StrAddress = baseAddress + PLAYER1_STR_ADDRESS;

  if (!readMemory(hProc, decInstructionAddress, &instructionValue, 1))
  {
    return READ_MEMORY_ERR;
  }

  if (instructionValue != DEC_INSTRUCTION && instructionValue != NOP_INSTRUCTION)
  {
    return UNEXPECTED_BYTECODE_ERR;
  }

  uint8_t instructionNewValue = NOP_INSTRUCTION;

  if (!activate)
  {
    instructionNewValue = DEC_INSTRUCTION;
  }

  if ((activate && instructionValue == NOP_INSTRUCTION) || (!activate && instructionValue == DEC_INSTRUCTION))
  {
    printf("Already %s\n", activate ? "on" : "off");
    return NO_OPERATION_ERR;
  }

  logDebug("\nToggling the decrease ball instruction\n");
  uint8_t instructionPayload[1] = {instructionNewValue};
  if (!writeMemory(hProc, decInstructionAddress, instructionPayload, 1))
  {
    return WRITE_MEMORY_ERR;
  }

  logDebug("\nToggling the 'player 1' text\n");
  wstring newStrValue = activate ? L"0Hacker 1\0" : L"0Player 1\0"; // Adding the '0' padding (can be any char) to keep the first char from being clipped. I'm didn't look deeper into the issue. Just know that this fix works
  size_t newStrValueSize = (newStrValue.length() + 1) * sizeof(wchar_t);

  if (!writeMemory(hProc, player1StrAddress, (uint8_t *)newStrValue.data(), newStrValueSize))
  {
    return WRITE_MEMORY_ERR;
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
    return NO_WINDOW_ERR;
  }

  HANDLE hProc = getProcessHandle(hWnd, PID);

  if (!hProc)
  {
    return NO_OPEN_PROCESS_ERR;
  }

  unsigned char buffer;
  UINT_PTR baseAddress = getProcessBaseAddress(PID, &hProc);
  logDebug("Base address: 0x%p\n", (void *)baseAddress);
  UINT_PTR aimedAddress = baseAddress + DEC_INSTRUCTION_ADDRESS;

  int readResult = readMemory(hProc, aimedAddress, &buffer, 1);

  if (!readResult)
  {
    return READ_MEMORY_ERR;
  }

  if (buffer != DEC_INSTRUCTION && buffer != NOP_INSTRUCTION)
  {
    return UNEXPECTED_BYTECODE_ERR;
  }

  bool result = buffer != DEC_INSTRUCTION;

  CloseHandle(hProc);

  return result;
}
