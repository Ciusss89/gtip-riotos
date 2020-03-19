#ifndef IHB_H
#define IHB_H

#include <stdint.h>
#include <stdbool.h> 

#include "uthash.h"

static const uint8_t IHBMAGIC[8] = {0x5F, 0x49, 0x48, 0x42, 0x05, 0xF5, 0x56, 0x5F};

struct ihb_node {
	/* Data */
	char *canP;
	bool best;
	int canID;

	/* makes this structure hashable */
	UT_hash_handle hh;
};

static struct ihb_node *ihbs = NULL;

/*
 *
 */
void wipe_ihb_node();

/*
 *
 */
int init_socket_can(int *can_soc_fd, const char *d);

/*
 *
 */
int discovery(int fd, bool v, uint8_t *wanna_be_master, uint8_t *ihb_nodes);

#endif