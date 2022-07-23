ARCH?=i686
TARGET=$(ARCH)-elf
ARCH_DIR=arch/$(ARCH)

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=
DB_CPPFLAGS?=

KERNEL_SRC=src/kernel
LIBC_SRC=src/libc
 
DESTDIR?=
PREFIX?=build
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -masm=intel
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -D__is_libk -Iinclude/kernel -Iinclude/libc
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -L$(PREFIX)/usr/lib -lk -lgcc
DB_CPPFLAGS:= -D__K_DEBUG

include $(KERNEL_SRC)/make.config
include $(KERNEL_SRC)/$(ARCH_DIR)/make.config

# includes variable "LIBC_OBJS" and "LIBK_OBJS"
include $(LIBC_SRC)/make.config

OBJS=\
$(K_OBJS) \
$(LIBK_OBJS) \
$(LIBC_OBJS) \

LINK_LIST=\
$(LDFLAGS) \
$(K_OBJS) \
$(LIBS)

DB_K_OBJS=$(K_OBJS:.o=.db.o)
DB_LIBK_OBJS=$(LIBK_OBJS:.o=.db.o)
DB_OBJS=$(OBJS:.o=.db.o)

.PHONY: all clean iso debug
.SUFFIXES: .o .c .asm .db.o

all: dirs libk zeanix libc iso

dirs:
	mkdir -p build
	cp -r sysroot/* build
	cp -r include/libc/* build/usr/include

iso: dirs libk zeanix libc
	cd build && tar -cvf boot/initrd.tar -T {*,boot}
	grub-mkrescue -o zeanix.iso build

debug: dirs libk-db zeanix-db libc-db
	cd build && tar -cvf boot/initrd.tar -T {*,boot}
	grub-mkrescue -o zeanix.iso build

zeanix: $(K_OBJS) $(KERNEL_SRC)/$(ARCH_DIR)/linker.ld 
	$(TARGET)-gcc -T $(KERNEL_SRC)/$(ARCH_DIR)/linker.ld -o build/boot/$@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot build/boot/$@

zeanix-db: $(DB_K_OBJS) $(KERNEL_SRC)/$(ARCH_DIR)/linker.ld 
	$(TARGET)-gcc -T $(KERNEL_SRC)/$(ARCH_DIR)/linker.ld -o build/boot/$(@:-db=) $(CFLAGS) $(LDFLAGS) $(DB_K_OBJS) $(LIBS)
	grub-file --is-x86-multiboot build/boot/$(@:-db=)

libk: $(LIBK_OBJS)
	$(TARGET)-ar rcs build/usr/lib/$@.a $(LIBK_OBJS)

libk-db: $(DB_LIBK_OBJS)
	$(TARGET)-ar rcs build/usr/lib/$(@:-db=).a $(DB_LIBK_OBJS)

libc:

libc-db:

.c.o:
	$(TARGET)-gcc -MD -c $< -o $@ -std=gnu17 $(CFLAGS) $(CPPFLAGS)

.c.db.o:
	$(TARGET)-gcc -MD -g -c $< -o $@ -std=gnu17 $(CFLAGS) $(CPPFLAGS) $(DB_CPPFLAGS)

.asm.o:
	nasm $< -f elf32 -o $@

.asm.db.o:
	nasm $< -f elf32 -F DWARF -o $@

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(DB_OBJS) $(DB_OBJS:.o=.d)
	rm -rf build
