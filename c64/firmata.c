#include "firmata.h"
#include <serial.h>
#include <serial+.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits.h>

char _version[6];
unsigned char _ports[2];
unsigned int _analog[6];

void _delay(unsigned char counter)
{
  while(--counter)
  {}
}

void firmata_begin(void)
{
  _version[0] = '\0';
  serial_begin();
  _delay(255);
  serial_put(REPORT_VERSION);  
  while(1){
    firmata_update();
    if(_version[0] != '\0') break;
  }
}

char *firmata_version(void) {
  return _version;
}

void firmata_update(void)
{
  char data, command, channel, msb, lsb;
  unsigned int value;

  if(!serial_get(&data)) {
  
    // printf("%d, ", data);
    command = data & 0xF0;
    channel = data & 0x0F;

    if(data == REPORT_VERSION) {
      serial_get(&lsb);
      serial_get(&msb);
      snprintf(_version, 6, "%d.%d", lsb, msb);
    } else 

    if (command == DIGITAL_MESSAGE) {
      serial_get(&lsb);
      serial_get(&msb);
      _ports[channel] = (msb << 7) + lsb;
      // printf("digital(%d) = %d\n", channel, _ports[channel]);
    } else

    if (command == ANALOG_MESSAGE) {
      serial_get(&lsb);
      serial_get(&msb);
      value = (msb << 7) + lsb;
      // printf("analog(%d) = %d\n", channel, value);
      if(value  < 1024) {
      _analog[channel] = value;
      }
    }

  }  
}

void firmata_pin_mode(unsigned char pin, unsigned char mode)
{
    serial_put(SET_PIN_MODE);
    serial_put(pin);
    serial_put(mode);
    if(mode==INPUT){
      unsigned char port = (pin >> 3) & 0x0F;
      serial_put(REPORT_DIGITAL | port);
      serial_put(ENABLE);
    } else 
    if(mode==ANALOG){  
      serial_put(REPORT_ANALOG | pin);
      serial_put(ENABLE); 
    }

}

void firmata_pin_write_digital(unsigned char pin, unsigned char value)
{
  serial_put(SET_DIGITAL_PIN_VALUE);
  serial_put(pin);
  serial_put(value==0?0:1);
}

unsigned char firmata_pin_read_digital(unsigned char pin)
{
  unsigned char port = (pin >> 3) & 0x0F;
  unsigned char _pin = pin & 0x07;
  return (_ports[port] & (1<<_pin))?HIGH:LOW;
}

void firmata_pin_write_analog(unsigned char pin, unsigned int value)
{
  serial_put(ANALOG_MESSAGE | pin);
  serial_put(value & 0x7F);
  serial_put((value >> 7) & 0x7F);
}

unsigned int firmata_pin_read_analog(unsigned char pin)
{
  return _analog[pin];
}
