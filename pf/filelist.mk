MYLIB_PATH=$(SOURCELIB_ROOT)/../repo/mylib

CFLAGS += -I$(MYLIB_PATH)


LIBSRCS += $(MYLIB_PATH)/s4819560_console.c
LIBSRCS += $(MYLIB_PATH)/s4819560_rcmcont.c
LIBSRCS += $(MYLIB_PATH)/s4819560_txradio.c
LIBSRCS += $(MYLIB_PATH)/s4819560_mfs_led.c
LIBSRCS += $(MYLIB_PATH)/s4819560_hamming.c
LIBSRCS += $(MYLIB_PATH)/s4819560_board_pb.c

# Including memory heap model
LIBSRCS += $(FREERTOS_PATH)/portable/MemMang/heap_2.c