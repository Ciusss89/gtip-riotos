/**
 * Author:	Giuseppe Tipaldi
 * Created:	2020
 *
 * Based on tests/driver_nrf24l01p_lowlevel/main.c
 **/


#ifndef SPI_PORT
#error "SPI_PORT not defined"
#endif
#ifndef CE_PIN
#error "CE_PIN not defined"
#endif
#ifndef CS_PIN
#error "CS_PIN not defined"
#endif
#ifndef IRQ_PIN
#error "IRQ_PIN not defined"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <board.h>
#include <time.h>

#include "nrf24l01p.h"
#include "nrf24l01p_settings.h"
#include "periph/spi.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define CHANNELS 128

#define ENABLE_DEBUG    (1)
#include "debug.h"

#include "tools.h"

static nrf24l01p_t nrf24l01p_0;

char rx_handler_stack[THREAD_STACKSIZE_MAIN];

static void prtbin(unsigned byte)
{
	for (char i = 0; i < 8; i++)
		printf("%u", (byte >> (7 - i)) & 0x0001);

	puts("\n");
}

static void print_register(char reg, int num_bytes)
{

	char buf_return[num_bytes];

	spi_transfer_regs(SPI_PORT, CS_PIN,
			  (CMD_R_REGISTER | (REGISTER_MASK & reg)),
			  NULL, (uint8_t *)buf_return, num_bytes);


	if (num_bytes < 2) {
		printf("0x%x returned: ", reg);


		for (int i = 0; i < num_bytes; i++)
			prtbin(buf_return[i]);
	} else {
		printf("0x%x returned: ", reg);

		for (int i = 0; i < num_bytes; i++)
			printf("%x ", buf_return[i]);

	printf("\n\n");
	}
}

static void print_regs(void)
{
	printf("################## Print Registers ###################\n");
	spi_acquire(SPI_PORT, CS_PIN, SPI_MODE_0, SPI_CLK_400KHZ);

	puts("REG_CONFIG: ");
	print_register(REG_CONFIG, 1);

	puts("REG_EN_AA: ");
	print_register(REG_EN_AA, 1);

	puts("REG_EN_RXADDR: ");
	print_register(REG_EN_RXADDR, 1);

	puts("REG_SETUP_AW: ");
	print_register(REG_SETUP_AW, 1);

	puts("REG_SETUP_RETR: ");
	print_register(REG_SETUP_RETR, 1);

	puts("REG_RF_CH: ");
	print_register(REG_RF_CH, 1);

	puts("REG_RF_SETUP: ");
	print_register(REG_RF_SETUP, 1);

	puts("REG_STATUS: ");
	print_register(REG_STATUS, 1);

	puts("REG_OBSERVE_TX: ");
	print_register(REG_OBSERVE_TX, 1);

	puts("REG_RPD: ");
	print_register(REG_RPD, 1);

	puts("REG_RX_ADDR_P0: ");
	print_register(REG_RX_ADDR_P0, INITIAL_ADDRESS_WIDTH);

	puts("REG_TX_ADDR: ");
	print_register(REG_TX_ADDR, INITIAL_ADDRESS_WIDTH);

	puts("REG_RX_PW_P0: ");
	print_register(REG_RX_PW_P0, 1);

	puts("REG_FIFO_STATUS: ");
	print_register(REG_FIFO_STATUS, 1);

	puts("REG_DYNPD: ");
	print_register(REG_DYNPD, 1);

	puts("REG_FEATURE: ");
	print_register(REG_FEATURE, 1);

	spi_release(SPI_PORT);
}

static void get_power(void)
{
	printf("Power %ddBm\n", nrf24l01p_get_power(&nrf24l01p_0));
}

static int monitor(void)
{
	int r;

	/*
	 * Scan from 2.400GHz to 2.527GHz:
	 *
	 * Received Power Detector (RPD), located in register 09, bit 0
	 * triggers at received power levels above -64 dBm that are present
	 * in the RF channel you receive on.
	 * If the received power is less than -64 dBm, RDP = 0.
	 *
	 *
	 * The RPD can be read out at any time while nRF24L01+ is in receive
	 * mode. This offers a snapshot of the current received power level
	 * in the channel.
	 *
	 * If no packets are received the RPD is latched at the end of a
	 * receive period as a result of host MCU setting CE low or RX time out
	 * controlled by Enhanced ShockBurstTM.
	 */
	for (uint8_t chan = 0; chan < CHANNELS; chan++ ) {
		
		nrf24l01p_start(&nrf24l01p_0);

		r = nrf24l01p_set_channel(&nrf24l01p_0, chan);
		if (r < 0) {
			puts("nrf24: cannot change channel");
			return -1;
		}

		/* setup device as receiver */
		r = nrf24l01p_set_rxmode(&nrf24l01p_0);
		if (r < 0) {
			puts("nrf24: cannot switch to rx mode");
			return -1;
		}

		xtimer_usleep(4000);

		nrf24l01p_stop(&nrf24l01p_0);

		spi_acquire(SPI_PORT, CS_PIN, SPI_MODE_0, SPI_CLK_400KHZ);
		printf("ch [%d]: ", chan);
		print_register(REG_RPD, 1);
		spi_release(SPI_PORT);

	}

	return 0;
}

/* RX handler that waits for a message from the ISR */
void *nrf24l01p_rx_handler(void *arg)
{
	(void)arg;
	msg_t msg_q[1];
	msg_init_queue(msg_q, 1);
	unsigned int pid = thread_getpid();
	char *rx_buf;

	nrf24l01p_register(&nrf24l01p_0, &pid);

	msg_t m;

	while (msg_receive(&m)) {
	        DEBUG("nrf24: got a message\n");

	switch (m.type) {
		case RCV_PKT_NRF24L01P:
			DEBUG("Received packet");

			rx_buf = malloc (sizeof(char) * NRF24L01P_MAX_DATA_LENGTH);
			if(!rx_buf) {
				puts("nrf24: malloc failure");
					goto oom;
			}

			/* CE low */
			nrf24l01p_stop(m.content.ptr);

			/* read payload */
			nrf24l01p_read_payload(m.content.ptr, rx_buf, NRF24L01P_MAX_DATA_LENGTH);

			/* flush rx fifo */
			nrf24l01p_flush_rx_fifo(m.content.ptr);

			/* CE high */
			nrf24l01p_start(m.content.ptr);

			//memcpy(userspace_buff, rx_buf, sizeof(rx_buf));

			/* print rx buffer */
			if(ENABLE_DEBUG) {
				for (int i = 0; i < NRF24L01P_MAX_DATA_LENGTH; i++)
					DEBUG("%i ", rx_buf[i]);
				DEBUG("\n");
			}

			free(rx_buf);
oom:
			break;
		default:
			puts("nrf24: rx a uncategorized message.");
			break;
		}
	}

	/* Never should reach here */
	return NULL;
}

/**
 * @set_rx
 */
static int rx_to_stdout(void)
{
	if (pid_nrf24l01 < 0) {
		pid_nrf24l01 = thread_create(rx_handler_stack,
					      sizeof(rx_handler_stack),
					      THREAD_PRIORITY_MAIN - 1,
					      0,
					      nrf24l01p_rx_handler,
					      0,
					      "nrf24_rx_handler");
		if(pid_nrf24l01 < 0) {
			puts("nrf24: starting the thread has falied");
			return -1;
		}
	} else if (pid_nrf24l01 > 0) {
		return 0;
	}

	/* setup device as receiver */
	if (nrf24l01p_set_rxmode(&nrf24l01p_0) < 0) {
		puts("nrf24: cannot switch to rx mode");
		return -1;
	}

	printf("nrf24: Print to stdout the received messages, pid=%d\n", pid_nrf24l01);

	return 0;
}

static void print_usage(void)
{
	puts("nrf24 <command> <option>");
	puts("\tdump\t  - Print transciever Registers");
	puts("\trx-mode\t  - Print to stdout the received messages");
	puts("\tget-power - Get the transmit power of the device [-18, -12, -6, 0]dbm");
	puts("\tmonitor   - Start spectrum analyzer");
}

int _nrf24_handler(int argc, char **argv)
{
	if (argc < 2) {
		print_usage();
		return -1;
	}  else if (strncmp(argv[1], "rx-mode", 8) == 0) {
		rx_to_stdout();
	}  else if (strncmp(argv[1], "dump", 5) == 0) {
		print_regs();
	} else if (strncmp(argv[1], "get-power", 10) == 0) {
		get_power();
	} else if (strncmp(argv[1], "monitor", 7) == 0) {
		monitor();
	} else {
		print_usage();
	}

	return 0;
}


/**
 * @device_nrf24l01p_register
 */
int device_nrf24l01p_register(unsigned int *device)
{
	int r;

	r = nrf24l01p_init(&nrf24l01p_0, SPI_PORT, CE_PIN, CS_PIN, IRQ_PIN);

	if (r < 0) {
		puts("nrf24: init of the device has failed");
		return -1;
	}

	nrf24l01p_get_id(&nrf24l01p_0, device);

	return 0;
}
