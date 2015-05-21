#ifndef BusMaster_h
#define BusMaster_h

#include <stdint.h>

#define QUEUE_LENGTH 8
#define QUEUE_MASK 7

typedef struct
{
	uint8_t buffer[QUEUE_LENGTH];
	uint8_t head;
	uint8_t length;
}
uart_queue;

typedef struct
{
	uint32_t ticks_per_baud;
	int rts_pin;
	int rx_pin;
	int tx_pin;
	volatile int rx_data;
	volatile int tx_data;
}
uart_conf;

typedef struct
{
	uint32_t stack[10];
	uart_conf conf;
}
uart_frame;

#endif


