/**
* This is the main Blank Simple C++ Project program file.
*/

#include <stdio.h>
#include <propeller.h>
#include "BusMaster.h"
#include "coglib.h"

#define FREQ 80000000

// potrebujeme praci s frontou

void uart_init(uart_conf* conf, int ticks_per_baud, int rx_pin, int tx_pin, int rts_pin)
{
	conf->ticks_per_baud = ticks_per_baud;
	conf->rx_pin = rx_pin;
	conf->tx_pin = tx_pin;
	conf->rts_pin = rts_pin;
	
	conf->rx_data = -1;
	conf->tx_data = -1;
}

void send(uart_conf* conf, int data)
{
	while (conf->tx_data >= 0);
	conf->tx_data = data;	
}

void send(uart_conf* conf, const char* str)
{
	while (*str != 0)
	{
		send(conf, (int) *str);
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

int main()
{
	uart_frame frame;

	uart_conf* conf = &frame.conf;

	uart_init(conf, FREQ / (4 * 115200), 0, 2, 16);

	bit_set(DIRA, 23);
	bit_set(OUTA, 23);
	waitcnt(CNT + FREQ / 2);
	bit_clear(OUTA, 23);

	//send(conf, "INIT\r\n");
	send(conf, '!');

	extern unsigned int _load_start_uart_rx_cog[];
	cognew(_load_start_uart_rx_cog, conf);

	extern unsigned int _load_start_uart_tx_cog[];
	cognew(_load_start_uart_tx_cog, conf);

	while (true)
	{
		int data = receive(conf);

		if (data == '\r' || data == '\n')
		{
			send(conf, '\r');
			send(conf, '\n');

		}
		else
		{
			send(conf, data);
		}
	}

	return 0;
}
