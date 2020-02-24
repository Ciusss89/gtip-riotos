## App name
#
APPLICATION = gtip-test-05

## Target
#
BOARD ?= nucleo-l476rg

## This has to be the absolute path to the RIOT base directory:
#
RIOTBASE ?= $(CURDIR)/../../RIOT/

## Chose the debug level: LOG_NONE, LOG_WARNING, LOG_ERROR, LOG_DEBUG, LOG_ALL
#CFLAGS += -DLOG_LEVEL=LOG_ALL

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1
CFLAGS += -DDEVELHELP

## Generic (GNRC) network stack:
#
USEMODULE += gnrc
USEMODULE += auto_init_gnrc_netif
USEMODULE += netstats_l2

USEMODULE += gnrc_ipv6
USEMODULE += netstats_ipv6
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_icmpv6
USEMODULE += gnrc_icmpv6_error
USEMODULE += gnrc_icmpv6_echo
USEMODULE += gnrc_txtsnd
USEMODULE += gnrc_pktdump

# Modules to include:
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += xtimer

# make sure we read the local file
CFLAGS += -I$(CURDIR)

USEMODULE += uptime
DIRS += $(CURDIR)/uptime

#USEMODULE += userspace_nrf24
#DIRS += $(CURDIR)/userspace-nrf24

## Hardware setup
#
include $(CURDIR)/Makefile.hw.setup

## Networking setup
#
#include $(CURDIR)/Makefile.net.setup

include $(RIOTBASE)/Makefile.include
