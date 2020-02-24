#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>

#define NRF24_CMD_HELP "manage nrf24l transceiver"

//static char *userspace_buff;
static int8_t pid_nrf24l01 = -1;

int _nrf24_handler(int argc, char **argv);
int device_nrf24l01p_register(unsigned int *device);
#endif
