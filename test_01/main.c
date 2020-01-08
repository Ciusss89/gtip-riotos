/**
 * Author:	Giuseppe Tipaldi
 * Created:	2020
 **/

#include <unistd.h>
#include <stdio.h>

#include "shell.h"

#include "msg.h"
static msg_t queue[8];

int main(void)
{
	printf("RIOT-OS, MCU=%s Board=%s\n\r", RIOT_MCU, RIOT_BOARD);

	/* the main thread needs a msg queue to be able to run `ping6`*/
	msg_init_queue(queue, ARRAY_SIZE(queue));

	/* start shell */
	char line_buf[SHELL_DEFAULT_BUFSIZE];

	shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

	/* should be never reached */
	return 0;
}
