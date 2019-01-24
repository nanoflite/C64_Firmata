#ifndef __PETASCII_H
#define __PETASCII_H

#include "types.h"

extern void petToAsc(const char *pet, uint16_t len, char *asc);
extern void ascToPet(const char *asc, uint16_t len, char *pet);

extern char ac2pc(char a);

#endif
