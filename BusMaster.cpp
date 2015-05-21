/**
* This is the main Blank Simple C++ Project program file.
*/

#include <stdio.h>
#include <propeller.h>
#include "BusMaster.h"
#include "coglib.h"

#define FREQ 80000000

void uart_init(uart_conf* conf, int ticks_per_baud, int rx_pin, int tx_pin, int rts_pin)
{
	conf->ticks_per_baud = ticks_per_baud;
	conf->rx_pin = rx_pin;
	conf->tx_pin = tx_pin;
	conf->rts_pin = rts_pin;
	
	conf->rx_data = -1;
	conf->tx_data = -1;
}

void queue_init(uart_queue& queue)
{
	queue.head = 0;
	queue.length = 0;
}

int queue_get(uart_queue& queue)
{
	if (queue.length == 0)
	{
		return -1;
	}
	int data = queue.buffer[queue.head];
	queue.head = (queue.head + 1) & QUEUE_MASK;
	queue.length--;
	return data;
}

int queue_put(uart_queue& queue, int data)
{
	if (queue.length == QUEUE_LENGTH)
	{
		return false;
	}	
	queue.buffer[(queue.head + queue.length) & QUEUE_MASK] = data;
	queue.length++;
	return true;
}

void send(uart_queue& queue, const char* str)
{
	while (*str != 0)
	{
		queue_put(queue, *str);
		str++;
	}
}

int receive(uart_conf* conf)
{
	while (conf->rx_data < 0);
	int data = conf->rx_data;
	conf->rx_data = -1;
	return data;
}

void leds_init()
{
	for (int led = 16; led <= 23; led++)
	{
		bit_change(DIRA, led, OUT_DIR);
		bit_clr(OUTA, led);
	}
}

int main()
{	
	uart_frame frame;
	uart_conf* conf = &frame.conf;
	uart_queue queue;

	leds_init();
	uart_init(conf, FREQ / (1 * 115200), 0, 2, 16);
	queue_init(queue);

	bit_set(DIRA, 23);
	bit_set(OUTA, 23);
	waitcnt(CNT + FREQ / 2);
	bit_clear(OUTA, 23);

	extern unsigned int _load_start_uart_rx_cog[];
	cognew(_load_start_uart_rx_cog, conf);

	extern unsigned int _load_start_uart_tx_cog[];
	cognew(_load_start_uart_tx_cog, conf);

	send(queue, "READY\r\n");

	int data;
	while (true)
	{		
		data = conf->rx_data;
		if (data >= 0)
		{
			conf->rx_data = -1;
			if (data == '\r' || data == '\n')
			{
				queue_put(queue, '\r');
				queue_put(queue, '\n');
			}
			else
			{
				queue_put(queue, data);
			}
		}

		if (conf->tx_data < 0)
		{
			data = queue_get(queue);
			if (data >= 0)
			{
				conf->tx_data = data;
			}
		}
	}

	return 0;
}
