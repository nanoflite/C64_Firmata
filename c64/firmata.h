#ifndef __FIRMATA_H__
#define __FIRMATA_H__

// Message command bytes (0x80(128) to 0xFF(255)) - straight from Firmata.h
#define DIGITAL_MESSAGE         0x90  // send data for a digital pin
#define ANALOG_MESSAGE          0xE0  // send data for an analog pin (or PWM)
#define DIGITAL_PULSE           0x91  // SysEx command to send a digital pulse

// PULSE_MESSAGE                  0xA0  // proposed pulseIn/Out msg (SysEx)
// SHIFTOUT_MESSAGE               0xB0  // proposed shiftOut msg (SysEx)
#define REPORT_ANALOG           0xC0  // enable analog input by pin //
#define REPORT_DIGITAL          0xD0  // enable digital input by port pair
#define START_SYSEX             0xF0  // start a MIDI SysEx msg
#define SET_PIN_MODE            0xF4  // set a pin to INPUT/OUTPUT/PWM/etc
#define SET_DIGITAL_PIN_VALUE   0xF5  // set the value of pin
#define END_SYSEX               0xF7  // end a MIDI SysEx msg
#define REPORT_VERSION          0xF9  // report firmware version
#define SYSTEM_RESET            0xFF  // reset from MIDI
#define QUERY_FIRMWARE          0x79  // query the firmware name

// extended command set using sysex (0-127/0x00-0x7F)
// 0x00-0x0F reserved for user-defined commands */

#define EXTENDED_ANALOG         0x6F  // analog write (PWM, Servo, etc) to any pin
#define PIN_STATE_QUERY         0x6D  // ask for a pin's current mode and value
#define PIN_STATE_RESPONSE      0x6E  // reply with pin's current mode and value
#define CAPABILITY_QUERY        0x6B  // ask for supported modes and resolution of all pins
#define CAPABILITY_RESPONSE     0x6C  // reply with supported modes and resolution
#define ANALOG_MAPPING_QUERY    0x69  // ask for mapping of analog to pin numbers
#define ANALOG_MAPPING_RESPONSE 0x6A  // reply with mapping info

#define SERVO_CONFIG            0x70  // set max angle, minPulse, maxPulse, freq
#define STRING_DATA             0x71  // a string message with 14-bits per char
#define SHIFT_DATA              0x75  // a bitstream to/from a shift register
#define I2C_REQUEST             0x76  // send an I2C read/write request
#define I2C_REPLY               0x77  // a reply to an I2C read request
#define I2C_CONFIG              0x78  // config I2C settings such as delay times and power pins
#define REPORT_FIRMWARE         0x79  // report name and version of the firmware
#define SAMPLING_INTERVAL       0x7A  // set the poll rate of the main loop
#define SYSEX_NON_REALTIME      0x7E  // MIDI Reserved for non-realtime messages
#define SYSEX_REALTIME          0x7F  // MIDI Reserved for realtime messages


// Pin modes.
// except from UNAVAILABLE taken from Firmata.h
#define UNAVAILABLE   -1
#define INPUT          0              // as defined in wiring.h
#define OUTPUT         1              // as defined in wiring.h
#define ANALOG         2              // analog pin in analogInput mode
#define PWM            3              // digital pin in PWM output mode
#define SERVO          4              // digital pin in SERVO mode

// some other defines
#define LOW  0
#define HIGH 1

#define DISABLE 0
#define ENABLE  1


// function prototypes

void firmata_begin(void);
void firmata_update(void);

char *firmata_version(void);

// firmata_set_pin_mode()
void firmata_pin_mode(unsigned char pin, unsigned char mode);

// firmata_write_digital()
void firmata_pin_write_digital(unsigned char pin, unsigned char value);

// firmata_read_digital()
unsigned char firmata_pin_read_digital(unsigned char pin);

// firmata_write_analog()
void firmata_pin_write_analog(unsigned char pin, unsigned int value);

// firmata_read_analog()
unsigned int firmata_pin_read_analog(unsigned char pin);

void firmata_servo_config(unsigned char pin, unsigned int minPulse, unsigned int maxPulse );
void firmata_pin_write_servo(unsigned char pin, unsigned int value);
int map(int x, int in_min, int in_max, int out_min, int out_max);
// TODO
// - register a callback when input changes for analog and digital
// - simpler function names
// - servo 
// - shift
// - i2c
// - onewrire
// - stepper
// - distance sensor (ping)
// - encoder
// - lcd ?
// - BASIC extension?


#endif // __FIRMATA_H__
