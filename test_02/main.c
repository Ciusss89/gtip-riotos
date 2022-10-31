/**
 * Author:	Giuseppe Tipaldi
 * Created:	2020
 **/

#include <errno.h>
#include <stdio.h>

#include "lwip.h"
#include "lwip/netif.h"
#include "net/ipv4/addr.h"
#include "shell.h"

static const shell_command_t shell_commands[] = {
    { NULL, NULL, NULL }
};

static char line_buf[SHELL_DEFAULT_BUFSIZE];

int main(void)
{
	printf("RIOT-OS, MCU=%s Board=%s\n\r", RIOT_MCU, RIOT_BOARD);
	shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

	 /* should be never reached */
	return 0;
}
