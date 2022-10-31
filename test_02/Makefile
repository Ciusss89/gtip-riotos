## App name
#
APPLICATION = hardwiring-eth-controller

## Target
#
BOARD ?= nucleo-l476rg

## This has to be the absolute path to the RIOT base directory:
#
RIOTBASE ?= $(CURDIR)/../RIOT/

## Chose the debug level: LOG_NONE, LOG_WARNING, LOG_ERROR, LOG_DEBUG, LOG_ALL
#CFLAGS += -DLOG_LEVEL=LOG_ALL

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += shell
USEMODULE += shell_cmds_default
USEMODULE += ps

# make sure we read the local enc28j60 params file
CFLAGS += -I$(CURDIR)

CFLAGS+=-Wno-unused-parameter -Wno-unused-variable

# Uncomment next line to enable debug symbols
#CFLAGS+=-g -ggdb3

## Hardware setup
#
include $(CURDIR)/Makefile.hw.setup

## Networking setup
#
include $(CURDIR)/Makefile.net.setup

include $(RIOTBASE)/Makefile.include
