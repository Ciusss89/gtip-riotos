/**
 * Author:	Giuseppe Tipaldi
 * Created:	2020
 **/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "shell.h"

#ifdef MODULE_UPTIME
#include "uptime/uptime.h"
#endif

static const shell_command_t shell_commands[] = {
#ifdef MODULE_UPTIME
	{ "uptime", UPTIME_THREAD_HELP, _uptime_handler},
#endif
	{ NULL, NULL, NULL }
};

static char line_buf[SHELL_DEFAULT_BUFSIZE];

int main(void)
{
#ifdef MODULE_UPTIME
	uptime_thread_start();
#endif

	printf("RIOT-OS, MCU=%s Board=%s\n\r", RIOT_MCU, RIOT_BOARD);
	shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

	 /* should be never reached */
	return 0;
}
