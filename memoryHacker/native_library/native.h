#pragma once

#define DEBUG

// INSTRUCTIONS
#define NOP_INSTRUCTION 0x90
#define DEC_INSTRUCTION 0x48

// ERRORS
#define NO_WINDOW 1
#define NO_OPEN_PROCESS 2
#define READ_MEMORY 3
#define WRITE_MEMORY 4

HANDLE getProcessHandle(HWND hWnd, DWORD &PID);
UINT_PTR getProcessBaseAddress(DWORD processID, HANDLE *handle);
HWND findPinballWindow();
int readMemory(HANDLE handle, UINT_PTR aimedAddress, unsigned char *buffer, uint8_t nBytesToRead);
int writeMemory(HANDLE handle, UINT_PTR aimedAddress, int data, uint8_t dataSize);
int toggleHack(bool activate);