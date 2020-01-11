## App name
#
APPLICATION = gtip-test-01

## Target
#
BOARD ?= nucleo-l476rg

## This has to be the absolute path to the RIOT base directory:
#
RIOTBASE ?= $(CURDIR)/../../RIOT/

## Generic (GNRC) network stack:
#
#USEMODULE += gnrc_netdev_default
#USEMODULE += auto_init_gnrc_netif

## Chose the debug level: LOG_NONE, LOG_WARNING, LOG_ERROR, LOG_DEBUG, LOG_ALL
#CFLAGS += -DLOG_LEVEL=LOG_ALL

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

# make sure we read the local enc28j60 params file
CFLAGS += -I$(CURDIR)

## Hardware setup
#
include $(CURDIR)/Makefile.hw.setup

## Networking setup
#
include $(CURDIR)/Makefile.net.setup

include $(RIOTBASE)/Makefile.include
