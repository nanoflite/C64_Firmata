#include <stdio.h>
#include <stdlib.h>
#include <firmata.h>
#include <conio.h>
#include <c64.h>
#include <string.h>

#include "screen.c"

#define SERVO1 2
#define BUTTON 3
#define RELAY 4
#define LED 6
#define POTMETER 0

int main (void)
{
  char i = 0, value_0 = 0, value_1 = 0;
  unsigned int a0 = 0, a1 = 0;
  unsigned int led0, led1;
  unsigned int servo0, servo1;

  bgcolor(frame0000[1]);
  bordercolor(frame0000[0]);

  memcpy((unsigned char*)0x0400, frame0000+2, 40*25);
  memcpy((unsigned char*)0xd800, frame0000+2+40*25, 40*25);

  textcolor(COLOR_RED);
  cputsxy(35,6, "init ");
  firmata_begin();
  textcolor(COLOR_GRAY1);
  cputsxy(35,6, firmata_version());
  
  firmata_pin_mode(RELAY, OUTPUT); 
  firmata_pin_mode(BUTTON, INPUT); 
  firmata_pin_mode(POTMETER, ANALOG);
  firmata_pin_mode(LED, PWM);
  firmata_servo_config(SERVO1, 544, 2400);
        
  textcolor(COLOR_RED);
  cputsxy(28,14," OFF");
  cputsxy(10,12," OFF");

  while(1){
    firmata_update();
   
    // Relay 
    if(kbhit()){
      cgetc();
      i ^= 1;
      if(i){
        textcolor(COLOR_GREEN);
        cputsxy(28,14,"  ON");
      } else {
        textcolor(COLOR_RED);
        cputsxy(28,14," OFF");
      }
    }
    firmata_pin_write_digital(RELAY, i);

    // Button
    value_1 = firmata_pin_read_digital(BUTTON);
    if(value_0 != value_1) {
      value_0 = value_1;
      if(value_0){
        textcolor(COLOR_GREEN);
        cputsxy(10,12,"  ON");
      } else {
        textcolor(COLOR_RED);
        cputsxy(10,12," OFF");
      }
    }

    // Potmeter
    a1 = firmata_pin_read_analog(POTMETER);
    if(a0!=a1) {
      a0=a1;
      textcolor(COLOR_GREEN);
      gotoxy(10,14);
      cprintf("%4d", a0);
    }

    // LED
    led1 = a0 / 4;
    if(led0!=led1){
      led0=led1; 
      firmata_pin_write_analog(LED, led1);
      textcolor(COLOR_GREEN);
      gotoxy(28,12);
      cprintf("%4d", led0);
    } 

    // SERVO
    servo1 = map(a0,0,1023,0,180);
    if(servo0!=servo1){
      servo0=servo1;
      firmata_pin_write_servo(SERVO1, servo0);
      textcolor(COLOR_GREEN);
      gotoxy(28,16);
      cprintf("%4d", servo0);
    }

  }
  
  return EXIT_SUCCESS;
}
