#include "firmata.h"
#include <serial.h>
#include <serial+.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits.h>

#define MIDI_MSB(value) ((unsigned char)((value>>7) & 0x7f))
#define MIDI_LSB(value) ((unsigned char)(value & 0x7f))

char _version[6];
unsigned char _ports[2];
unsigned int _analog[6];
char _read_buffer[3];
#define _read_buffer_length 3

void _delay(unsigned char counter)
{
  while(--counter)
  {}
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
  long dx = (long)(x - in_min);
  long dout = (long)(out_max - out_min);
  long din = (long)(in_max - in_min);
  long r1 = dx * dout;
  int r2 =  (int)(r1 / din);
  int value = r2 + out_min; 
  return value;
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

char firmata_read_serial (void)
{
  char b, i, result;

  result = 0;
  for(i=0;i<_read_buffer_length;i++) {
    if(serial_get(&b)) {
      result = 1;
      break;
    }  
    _read_buffer[i] = b;
  }

  return result;
}

void firmata_update(void)
{
  char data, command, channel, msb, lsb;
  unsigned int value;

  if(!firmata_read_serial()) {
    data = _read_buffer[0];
    lsb = _read_buffer[1];
    msb = _read_buffer[2];
  
    command = data & 0xF0;
    channel = data & 0x0F;

    if(data == REPORT_VERSION) {
      snprintf(_version, 6, "v%d.%d", lsb, msb);
    } else 

    if (command == DIGITAL_MESSAGE) {
      _ports[channel] = (msb << 7) + lsb;
    } else

    if (command == ANALOG_MESSAGE) {
      value = (msb << 7) + lsb;
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
  serial_put(MIDI_LSB(value));
  serial_put(MIDI_MSB(value));
}

unsigned int firmata_pin_read_analog(unsigned char pin)
{
  return _analog[pin];
}

void firmata_servo_config(unsigned char pin, unsigned int minPulse, unsigned int maxPulse ) {
  serial_put(START_SYSEX);
  serial_put(SERVO_CONFIG);
  serial_put(pin);
  serial_put(MIDI_LSB(minPulse));
  serial_put(MIDI_MSB(minPulse));
  serial_put(MIDI_LSB(maxPulse));
  serial_put(MIDI_MSB(maxPulse));
  serial_put(END_SYSEX);
}

void firmata_pin_write_servo(unsigned char pin, unsigned int value) {
  firmata_pin_write_analog(pin, value);
}

