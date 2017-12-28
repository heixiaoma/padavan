ifndef ROOTDIR
ROOTDIR=../..
endif

SYSDEPDIR=$(ROOTDIR)/user/shared/sysdeps

include boards.mk
include cflags.mk

CFLAGS += -fPIC
CFLAGS += -Wall -I. -I./include

ifeq ($(CONFIG_VENDOR),Broadcom)
CFLAGS += -I$(ROOTDIR)/$(PROPRIETARY)/include
else
CFLAGS += -I$(ROOTDIR)/$(LINUXDIR)/include/nvram
endif

LDFLAGS += -L.

OBJS := shutils.o netutils.o rtutils.o defaults.o nvram_linux.o notify_rc.o bin_sem_asus.o flash_mtd.o pids.o

ifeq ($(CONFIG_VENDOR),Ralink)
OBJS += api-ralink.o
vpath %.c $(SYSDEPDIR) $(SYSDEPDIR)/ralink $(SYSDEPDIR)/ralink/nvram
else ifeq ($(CONFIG_VENDOR),Broadcom)
OBJS += api-broadcom.o
vpath %.c $(SYSDEPDIR) $(SYSDEPDIR)/broadcom $(SYSDEPDIR)/broadcom/nvram$(BCMEX)
endif

all: libshared.so

libshared.so: $(OBJS)
	$(LD) -shared -o $@ $^

romfs:
	$(ROMFSINST) -S /lib/libshared.so

clean:
	rm -f *.o libshared.so
