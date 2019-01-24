#include "serial+.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <serial.h>
#include <ctype.h>
#include <time.h>

#include "petascii.h"

#define DRIVERNAME  "c64-up2400.ser"

static unsigned long timeout = 0;
static unsigned char serror;
static char buffer[256];
static char buffer2[64];

static const struct ser_params serialParams = {
    SER_BAUD_2400,      /* Baudrate */
    SER_BITS_8,         /* Number of data bits */
    SER_STOP_1,         /* Number of stop bits */
    SER_PAR_NONE,       /* Parity setting */
    SER_HS_NONE         /* Type of handshake to use */
};

unsigned char serial_error(void)
{
  return serror;
}

unsigned char serial_get(char *b)
{
  char error;
  clock_t timestamp = clock();

  serror = SERIAL_ERR_OK;

  while (1) {
    error = ser_get(b);
  
    if (!error) return 0;
  
    if (clock() - timestamp > timeout) {
      serror = SERIAL_ERR_TIMEOUT; 
      return error;
    }
 
  }

}

void serial_timeout(unsigned int timeout_ms)
{
  timeout = (unsigned long) timeout_ms;
}

void serial_begin(void)
{
  ser_load_driver(DRIVERNAME);
  ser_open(&serialParams);
  ser_ioctl(1, NULL);

  /*
  while(1)
  {
    ser_put(0x0d);
    ser_put(0x0a);
  }
  */
}

void serial_enable(void)
{
  // ser_ioctl(1, NULL);
}

void serial_disable(void)
{
  // ser_ioctl(0, NULL);
}

void serial_writeln(char *line)
{
  char *p;

  petToAsc(line, strlen(line), buffer);

  p = buffer;
  while(*p) {
    ser_put(*p); 
    p++;   
  }
  ser_put(0x0d);
  ser_put(0x0a);
}

void serial_write(char *line)
{
  char *p;

  petToAsc(line, strlen(line), buffer);

  p = buffer;
  while(*p) {
    ser_put(*p); 
    p++;   
  }
}

void serial_write_int(int i)
{
  char si[16];

  serial_write(itoa(i, si, 10));  
}

void serial_readln(char *line, uint16_t lineSize)
{
  uint16_t i;
  char c;

  i = 0;
  while(1) {
    if (serial_get(&c)) {
      break;
    }
    buffer[i] = c;
    if (c == 0x0a) {
      i++;
      if (i >= lineSize) {
        i = lineSize - 1;
      }
      buffer[i] = '\0'; 
      break;
    }
    i++;
    if (i >= lineSize) {
      buffer[lineSize-1] = '\0';
      break;
    }
  }

  ascToPet(buffer,strlen(buffer), line); 
}

int serial_read_int(char *terminator)
{
  char c;
  uint16_t i;
  char *p;
  char *asciiTerminator;

  asciiTerminator = buffer2;
  petToAsc(terminator, strlen(terminator), asciiTerminator);

  i = 0;
  while(1) {
    if(serial_get(&c)) {
      break;
    }
    buffer[i] = c;
    buffer[i+1] = '\0';
    p = strstr(buffer, asciiTerminator);
    if(p){
      *p = '\0';
      return atoi(buffer); 
    }    
    i++;
    if (i>=sizeof(buffer)) {
      break;
    }
  }

  return -1;
}

void serial_read(char *buffer, uint16_t bufferSize)
{
  uint16_t i;
  char c;

  for(i=0;i<bufferSize;i++) {
    if ( serial_get(&c) ) {
      break;
    }
    buffer[i] = ac2pc(c);
  }

}

bool serial_expect(char *string)
{
  char c;
  uint16_t i;
  char *asciiString;
  char *p;

  asciiString = buffer2;
  petToAsc(string, strlen(string), asciiString);

  i = 0;
  while(1) {
    if (serial_get(&c)) {
      break;
    }

    if ( ! isprint(c)) {
      continue;
    }

    buffer[i] = c;
    buffer[i+1] = '\0';

    p = strstr(buffer, asciiString);
    if(p){
      return true;
    }    
    i++;
    if (i >= (sizeof(buffer)-1))  {
      break;
    }
    
  }

  return false;
}

bool serial_expect_ok_error(char *ok, char *error)
{
  char c;
  uint16_t i;

  i = 0;
  while(1) {
    if (serial_get(&c)) {
      break;
    }

    if ( ! isprint(c)) {
      continue;
    }

    buffer[i] = c;
    buffer[i+1] = '\0';

    petToAsc(ok, strlen(ok), buffer2);
    if ( strstr(buffer, buffer2)) {
      return true;
    }

    petToAsc(error, strlen(error), buffer2);
    if ( strstr(buffer, buffer2)) {
      return false;
    }
    
    i++;
    if (i>=sizeof(buffer)-1) {
      break;
    }
  }
  return false;
}

//bool serial_expect_ok2_error3(char *ok1, char *ok2, char *error1, char *error2, char *error3)
bool serial_expect_ok2_error3(ok2_error3 *oe)
{
  char c;
  uint16_t i;

  i = 0;
  while(1) {
    if (serial_get(&c)) {
      break;
    }

    if ( ! isprint(c)) {
      continue;
    }

    buffer[i] = c;
    buffer[i+1] = '\0';

    petToAsc(oe->ok1, strlen(oe->ok1), buffer2);
    if ( strstr(buffer, buffer2)) {
      return true;
    }

    petToAsc(oe->ok2, strlen(oe->ok2), buffer2);
    if ( strstr(buffer, buffer2)) {
      return true;
    }

    petToAsc(oe->error1, strlen(oe->error1), buffer2);
    if ( strstr(buffer, buffer2)) {
      return false;
    }

    petToAsc(oe->error2, strlen(oe->error2), buffer2);
    if ( strstr(buffer, buffer2)) {
      return false;
    }
 
    petToAsc(oe->error3, strlen(oe->error3), buffer2);
    if ( strstr(buffer, buffer2)) {
      return false;
    }
    
    i++;
    if (i>=sizeof(buffer)-1) {
      break;
    }
  }

  return false;
}

void serial_read_until(char *buffer, uint16_t bufferSize, char *terminator)
{
  char c;
  uint16_t i;

  i = 0;
  while(1)
  {
    if (serial_get(&c)) {
      break;
    }

    buffer[i] = c;
    buffer[i+1] = '\0';

    petToAsc(terminator, strlen(terminator), buffer2);
    if ( strstr(buffer, buffer2))
    {
      return;
    }

    i++;
    if ( i >= bufferSize-1 )
    {
      break;
    }
  }
}
