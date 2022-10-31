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
USEMODULE += netdev_default
USEMODULE += auto_init_gnrc_netif

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 0

# Modules to include:
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps


# make sure we read the local enc28j60 params file
CFLAGS += -I$(CURDIR)

#include $(CURDIR)/Makefile.hw.setup
include $(RIOTBASE)/Makefile.include
