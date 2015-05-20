/**
* This is the main Blank Simple C++ Project program file.
*/

#include <stdio.h>
#include <propeller.h>
#include "BusMaster.h"
#include "coglib.h"

#define FREQ 80000000

int main()
{
	uart_frame frame;

	uart_conf* conf = &frame.conf;

	conf->ticks_per_baud = FREQ / 115200;	
	conf->rx_pin = 0;
	conf->tx_pin = 2;
	conf->rts_pin = 16;
	
	conf->rx_data = -1;
	conf->tx_data = -1;

	extern unsigned int _load_start_uart_rx_cog[];
	cognew(_load_start_uart_rx_cog, conf);

	extern unsigned int _load_start_uart_tx_cog[];
	cognew(_load_start_uart_tx_cog, conf);

	while (false)
	{
		conf->tx_data = 'a';
		while (conf->tx_data >= 0);
		waitcnt(CNT + 80000000);
	}     

	while (true)
	{
		while (conf->rx_data < 0);
		int data = conf->rx_data;
		conf->rx_data = -1;
		while (conf->tx_data >= 0);
		conf->tx_data = data;
		// a moh by bliknout
	}

	return 0;
}
