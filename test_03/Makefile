## App name
#
APPLICATION = gtip-test-01

## Target
#
BOARD ?= nucleo-l476rg

## This has to be the absolute path to the RIOT base directory:
#
RIOTBASE ?= $(CURDIR)/../../RIOT/

## Chose the debug level: LOG_NONE, LOG_WARNING, LOG_ERROR, LOG_DEBUG, LOG_ALL
CFLAGS += -DLOG_LEVEL=LOG_NONE

#CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += xtimer

USEMODULE += uptime
DIRS += $(CURDIR)/uptime

# make sure we read the local enc28j60 params file
CFLAGS += -I$(CURDIR)

include $(RIOTBASE)/Makefile.include
