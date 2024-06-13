#location of freertos source files
FREERTOS_PATH=$(SOURCELIB_ROOT)/components/os/FreeRTOS

#Name of project
PROJ_NAME=main

#local source files
SRCS = $(PROJ_NAME).c
CFLAGS += -DENABLE_DEBUG_UART #enable Debug Com Port
###################################################
ROOT=$(shell pwd)

OBJS=$(SRCS:.c=.o)

###################################################

.PHONY: $(shell mkdir -p obj) lib proj all depend
all: proj

#Include freertos makefile
include $(FREERTOS_PATH)/filelist.mk

include $(SOURCELIB_ROOT)/../repo/s4/filelist.mk			
include $(SOURCELIB_ROOT)/components/boards/nucleo-f429zi/board.mk
include $(SOURCELIB_ROOT)/components/mk/m_launch.mk
include $(SOURCELIB_ROOT)/components/mk/m_common.mk

lib: 
	$(MAKE) -C lib

proj: 	$(PROJ_NAME).elf

include .depend
