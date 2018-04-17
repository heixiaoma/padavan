ifndef ROOTDIR
ROOTDIR=../..
endif

include boards.mk
include cflags.mk

SYSDEPDIR     := $(ROOTDIR)/user/shared/sysdeps
VENDOR        := $(shell tr [:upper:] [:lower:] <<< "$(CONFIG_VENDOR)" | tr \  _)
EXTRA_OBJS    :=
EXTRA_CFLAGS  :=
EXTRA_LDFLAGS :=
include $(SYSDEPDIR)/$(VENDOR).mk

CFLAGS += -fPIC
CFLAGS += -Wall -I. -I./include $(EXTRA_CFLAGS)

LDFLAGS += -L. $(EXTRA_LDFLAGS)

OBJS := shutils.o netutils.o rtutils.o defaults.o nvram_linux.o notify_rc.o bin_sem_asus.o flash_mtd.o pids.o
OBJS += api-$(VENDOR).o $(EXTRA_OBJS)

vpath %.c $(SYSDEPDIR) $(SYSDEPDIR)/$(VENDOR)

all: libshared.so

libshared.so: $(OBJS)
	$(LD) -shared -o $@ $^

romfs:
	$(ROMFSINST) -S /lib/libshared.so

clean:
	rm -f *.o libshared.so
