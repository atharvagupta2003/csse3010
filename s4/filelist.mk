MYLIB_PATH=$(SOURCELIB_ROOT)/../repo/mylib

CFLAGS += -I$(MYLIB_PATH)


LIBSRCS += $(MYLIB_PATH)/s4819560_mfs_pb.c
LIBSRCS += $(MYLIB_PATH)/s4819560_console.c
LIBSRCS += $(MYLIB_PATH)/s4819560_sysmon.c

# Including memory heap model
LIBSRCS += $(FREERTOS_PATH)/portable/MemMang/heap_2.c