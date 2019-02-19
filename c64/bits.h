#ifndef __BITS_H__
#define __BITS_H__

#define _BV(bit)   (1 << (bit))

#define outb(addr, val)  (*((char*)addr)) = (val)
#define inb(addr)        (*((char*)addr))

#define bit_is_set(sfr, bit)   (inb(sfr) & _BV(bit))
 
#define bit_is_clear(sfr, bit)   (!(inb(sfr) & _BV(bit)))
 
#define loop_until_bit_is_set(sfr, bit)   do { } while (bit_is_clear(sfr, bit))
 
#define loop_until_bit_is_clear(sfr, bit)   do { } while (bit_is_set(sfr, bit))

#endif
