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

static int ifconfig(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	for (struct netif *iface = netif_list; iface != NULL;
	     iface = iface->next) {

		printf("%s_%02u: ", iface->name, iface->num);
		char addrstr[IPV4_ADDR_MAX_STR_LEN];

		printf(" inet %s\n", ipv4_addr_to_str(addrstr,
				     (ipv4_addr_t *)&iface->ip_addr, sizeof(addrstr)));
		puts("");
	}
	return 0;
}

static const shell_command_t shell_commands[] = {
    { "ifconfig", "Shows assigned IPv4/6 addresses", ifconfig },
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
