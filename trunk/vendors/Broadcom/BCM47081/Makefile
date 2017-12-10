KERNELZ		= $(IMAGEDIR)/vmlinuz
RAMDISK		= $(IMAGEDIR)/target.squashfs
IMAGE		= $(IMAGEDIR)/$(CONFIG_FIRMWARE_PRODUCT_ID)_$(FIRMWARE_KERNEL_VER).$(FIRMWARE_ROOTFS_VER)-$(FIRMWARE_BUILDS_VER).trx
BOARD_DIR	= $(ROOTDIR)/configs/boards/$(CONFIG_FIRMWARE_PRODUCT_ID)
CTOOLS_DIR	= $(ROOTDIR)/$(PROPRIETARY)/ctools
CUR_DIR		= $(shell pwd)
COMP		= lzma
ROMFS_DIRS	= \
	bin \
	dev dev/pts \
	home \
	etc \
	etc_ro etc_ro/ppp/peers etc_ro/ppp/plugins etc_ro/ppp/3g \
	etc_ro/Wireless etc_ro/Wireless \
	lib lib/modules \
	mnt \
	media \
	opt \
	proc \
	sbin \
	sys \
	usr usr/bin usr/sbin usr/lib/pppd usr/codepages \
	tmp \
	var

clean:
	rm -f $(RAMDISK) $(KERNELZ) $(KERNELZ)-*
	$(MAKE) -C $(CTOOLS_DIR) $@

romfs:
	[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)
	for i in $(ROMFS_DIRS); do \
		[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)/$$i; \
	done

.PHONY: ctools
ctools:
	$(MAKE) -C $(CTOOLS_DIR)

romfs.post:: ctools
	[ -d $(IMAGEDIR) ] || mkdir -p $(IMAGEDIR)
	sudo /sbin/ldconfig -r $(ROMFSDIR) -C /etc_ro/ld.so.cache
	rm -f $(RAMDISK)
	$(CTOOLS_DIR)/mksquashfs $(ROMFSDIR) $(RAMDISK) -noappend -all-root

image: ctools
	##############CREATE IMAGE#################
	rm -f $(KERNELZ) $(KERNELZ)-*
	$(CROSS_COMPILE)objcopy -O binary -R .note -R .note.gnu.build-id -R .comment -S $(LINUXDIR)/vmlinux $(KERNELZ)
	$(CTOOLS_DIR)/lzma_4k e $(KERNELZ) $(KERNELZ)-$(COMP)
	$(CTOOLS_DIR)/trx -o $(KERNELZ)-$(COMP).trx $(KERNELZ)-$(COMP) $(RAMDISK)
	$(CTOOLS_DIR)/trx_asus -i $(KERNELZ)-$(COMP).trx -r $(CONFIG_FIRMWARE_PRODUCT_ID),$(FIRMWARE_KERNEL_VER).$(FIRMWARE_ROOTFS_VER),$(IMAGE)
