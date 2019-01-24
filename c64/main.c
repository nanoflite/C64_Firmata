#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <serial.h>
#include <serial+.h>

int main (void)
{
  char c = '?';

  clrscr();
  puts("C64 serial");

  serial_begin();
  puts("write 'X'");
  ser_put('X'); 

  puts("receive...");
  serial_get(&c);
  printf("-> %c\n", c);
 
  return EXIT_SUCCESS;
}
