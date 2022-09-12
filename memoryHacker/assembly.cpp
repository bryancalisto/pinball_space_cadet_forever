#include <stdio.h>
#include <inttypes.h>

uint64_t get_eip()
{
  // mov eax, [esp]
  __asm
  {
      mov ebx, 7
      mov eax, ebx
  }
}