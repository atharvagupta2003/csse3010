MYLIB_PATH=$(SOURCELIB_ROOT)/../repo/mylib

CFLAGS += -I$(MYLIB_PATH)


LIBSRCS += $(MYLIB_PATH)/s4819560_lta1000g.c
LIBSRCS += $(MYLIB_PATH)/s4819560_mfs_pb.c
LIBSRCS += $(MYLIB_PATH)/s4819560_rgb.c
LIBSRCS += $(MYLIB_PATH)/s4819560_mfs_trimpot.c 