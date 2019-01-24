#ifndef __SERIALPLUS_H__
#define __SERIALPLUS_H__

#include <stddef.h>
#include <stdbool.h>
#include "types.h"

typedef struct
{
  char *ok1;
  char *ok2;
  char *error1;
  char *error2;
  char *error3;
}
ok2_error3;

#define SERIAL_ERR_OK 0
#define SERIAL_ERR_TIMEOUT 1

unsigned char serial_error(void);

void serial_timeout(unsigned int timeout_ms);
void serial_begin(void);
void serial_enable(void);
void serial_disable(void);

unsigned char serial_get(char *b);
void serial_writeln(char *line);
void serial_write(char *line);
void serial_write_int(int i);

void serial_readln(char *line, uint16_t lineSize);
int serial_read_int(char *terminator);
void serial_read(char *buffer, uint16_t bufferSize);
void serial_read_until(char *buffer, uint16_t bufferSize, char *terminator);

bool serial_expect(char *string);
bool serial_expect_ok_error(char *ok, char *error);
//bool serial_expect_ok2_error3(char *ok1, char *ok2, char *error1, char *error2, char *error3);
bool serial_expect_ok2_error3(ok2_error3 *oe);

#endif // __SERIALPLUS_H__
