#include <stdio.h>
#include <stdlib.h>
#include <firmata.h>
#include <conio.h>

#define BUTTON 3
#define RELAY 4
#define LED 6
#define POTMETER 0

int main (void)
{
  char i = 0, value_0 = 0, value_1 = 0;
  unsigned int a0 = 0, a1 = 0;

  firmata_begin();
  printf("firmata version: %s\n", firmata_version());
  
  firmata_pin_mode(RELAY, OUTPUT); 
  firmata_pin_mode(BUTTON, INPUT); 
  firmata_pin_mode(POTMETER, ANALOG);
  firmata_pin_mode(LED, PWM);
  
  while(1){
    firmata_update();
    
    if(kbhit()){
      cgetc();
      i ^= 1;
    }
    firmata_pin_write_digital(RELAY, i);

    value_1 = firmata_pin_read_digital(BUTTON);
    if(value_0 != value_1) {
      value_0 = value_1;
      printf("BUTTON: %d\n", value_0);
    }

    a1 = firmata_pin_read_analog(POTMETER);
    if(a0!=a1) {
      a0=a1;
      printf("POTMETER: %d\n", a0);
    }

   firmata_pin_write_analog(LED, a0/4); 

  }
  return EXIT_SUCCESS;
}
