#include <windows.h>
#include <stdio.h>
#include "native.h"

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