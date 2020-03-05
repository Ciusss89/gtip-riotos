/**
 * Author:	Giuseppe Tipaldi
 * Created:	2020
 **/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "shell.h"
#include "board.h"
#include "thread.h"

#include "periph/gpio.h"

#include "can/can.h"
#include "can/conn/raw.h"
#include "can/conn/isotp.h"
#include "can/device.h"

#define ENABLE_DEBUG    (1)
#include "debug.h"

#include "ihbcan.h"

struct ihb_can_perph *p;

static void _usage(void)
{
	puts("IHBCAN userspace");
	puts("\tihbcan list   - show can controller struct");
	puts("\tihbcan canON  - turn on can controller");
	puts("\tihbcan canOFF - turn off can controller");
}

static int  _scan_for_controller(struct ihb_can_perph *device)
{
	const char *raw_can = raw_can_get_name_by_ifnum(CAN_C);

	if (raw_can && strlen(raw_can) < CAN_NAME_LEN) {
		device->id = CAN_C;
		strcpy(device->name, raw_can);
		DEBUG("_scan_for_controller: CAN #%d, name=%s\n", device->id,
								  device->name);
		return 0;
	}

	return 1;
}

static uint8_t _power_up(uint8_t ifnum)
{
	uint8_t ret;

	if (ifnum >= CAN_DLL_NUMOF) {
		puts("Invalid interface number");
		return 1;
	}

	ret = raw_can_power_up(ifnum);
	if (ret != 0) {
		printf("Error when powering up: res=-%d\n", ret);
		return 1;
	}

	return ret;
}

static uint8_t _power_down(uint8_t ifnum)
{

	uint8_t ret = 0;
	if (ifnum >= CAN_DLL_NUMOF) {
		puts("Invalid interface number");
		return 1;
	}

	ret = raw_can_power_down(ifnum);
	if (ret != 0) {
		printf("Error when powering up: res=-%d\n", ret);
		return 1;
	}

	return ret;
}

int _ihb_can_handler(int argc, char **argv)
{

	if (argc < 2) {
		_usage();
		return 1;
	} else if (strncmp(argv[1], "list", 5) == 0) {
		printf("ihb dev=%d name=%s mcu_id=%s\n", p->id,
							 p->name,
							 p->controller_uid);
	} else if (strncmp(argv[1], "canON", 6) == 0) {
		return _power_up(p->id);
	} else if (strncmp(argv[1], "canOFF", 7) == 0) {
		return _power_down(p->id);
	} else {
		printf("unknown command: %s\n", argv[1]);
		return 1;
	}

	return 0;
}

int _can_init(struct ihb_can_perph *device)
{
	device = malloc(sizeof(struct ihb_can_perph));
	uint8_t unique_id[CPUID_LEN];
	uint8_t r = 1;
	char *b;

	if(CAN_DLL_NUMOF == 0)
		puts("no can controller avaible");
	else
		DEBUG("MCU has %d can controller\n", CAN_DLL_NUMOF);

	if(CPUID_LEN > MAX_CPUID_LEN) {
		puts("_can_init CPUID_LEN > MAX_CPUID_LEN");
		return 1;
	}

	cpuid_get(unique_id);
	if(ENABLE_DEBUG) {
		printf("Unique ID: 0x");
		for (uint8_t i = 0; i < CPUID_LEN; i++)
			printf("%02x", unique_id[i]);
		puts("");
	}

	b = data2str(unique_id, CPUID_LEN);
	
	/* Save the MCU unique ID */
	strcpy(device->controller_uid, b);

	/*
	 * Generate an Unique CAN ID from the MCU's unique ID
	 * the fletcher8 hash function returns a not null value
	 */
	p->can_id = fletcher8(unique_id, CPUID_LEN);
	DEBUG("CAN ID=%02x\n", p->can_id);

	r = _scan_for_controller(device);
	if(r != 0) {
		puts("_scan_for_controller: has failed");
		return 1;
	}

	/* IDEA:
	 *
	 * 1) NODO-iesimo: Power-ON tutti hanno un CAN-ID (1-255) derivato dal CPUID != 0
	 * 2) NODO-iesimo: Power-ON, INIT: Ogni nodeo manda un frame di IMALIVE
	 * 3) HOST: dump di tutti i frame con CAN-ID compreso tra 1 e 255
	 * 3) HOST: master quello che tra ha CAN-ID minore.
	 * 4) HOST: Assegna al can-id master, CAN-iD 0
	 * 5) HOST: avvisa gli altri nodi che non sono master.
	 * 5) HODO-iesimo: non master dump per can-id 0. Ok se traffico
	 * 6) HOST: avvia la comunicazione con il mater.
	 *
	 * */

	p = device;
	return r;
}
