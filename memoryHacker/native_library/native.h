#pragma once

#define DEBUG

// INSTRUCTIONS
#define NOP_INSTRUCTION 0x90
#define DEC_INSTRUCTION 0x48

// ERRORS
#define NO_WINDOW 1           // Couldn't get the window handle
#define NO_OPEN_PROCESS 2     // Couldn't open the process based on the window handle
#define READ_MEMORY 3         // Couldn't read the process memory
#define WRITE_MEMORY 4        // Couldn't write the process memory
#define NO_OPERATION 5        // Based on the read byte from the process memory, it was concluded that the operation was already made so no need to execute it again
#define UNEXPECTED_BYTECODE 6 // Detects an unexpected instruction in the byte that is used for the hack

HANDLE getProcessHandle(HWND hWnd, DWORD &PID);
UINT_PTR getProcessBaseAddress(DWORD processID, HANDLE *handle);
HWND findPinballWindow();
int readMemory(HANDLE handle, UINT_PTR aimedAddress, unsigned char *buffer, uint8_t nBytesToRead);
int writeMemory(HANDLE handle, UINT_PTR aimedAddress, int data, uint8_t dataSize);
int toggleHack(bool activate);