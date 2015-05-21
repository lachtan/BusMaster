#ifndef coglib_h
#define coglib_h

#include <propeller.h>

#define IN_DIR 0
#define OUT_DIR 1

#define bit_set(port, bit) ((port) |= (1 << (bit)))
#define bit_clear(port, bit) ((port) &= ~(1 << (bit)))
#define bit_clr(port, bit) bit_clear((port), (bit))
#define bit_change(port, bit, value) ((value) ? bit_set((port), (bit)) : bit_clear((port), (bit)))

#define bit_get(port, bit) (((port) >> (bit)) & 1)

#endif

