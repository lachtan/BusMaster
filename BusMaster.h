#ifndef BusMaster_h
#define BusMaster_h

#include <stdint.h>

typedef struct
{
	uint32_t ticks_per_baud;
	int rx_data;
	int tx_data;
	int rts_pin;
	int rx_pin;
	int tx_pin;
}
volatile uart_conf;

typedef struct
{
	uint32_t stack[10];
	uart_conf conf;
}
uart_frame;

#endif


