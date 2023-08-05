# Zeanix
## Current State of Zeanix
THIS REPO HAS BEEN ARCHIVED. If you want a kernel from me, see [Seanix](https://github.com/VLPatton/seanix).
## Making a Bootable \*.iso File
### Requirements
The following commands only work on a Linux, or potentially other \*nix system, and the following must be installed:
* Custom GCC (see GCC and Binutils below)
* Custom Binutils (see GCC and Binutils below)
* GNU Make (Debain/Ubuntu/Mint: `make`; Arch: `base-devel` (or `make`))
* GRUB (Debian/Ubuntu/Mint: `grub`; Arch: `grub`)
### GCC and Binutils
Download the latest tarballs for both GCC (preferably 12.1) and Binutils (preferably 2.38), then extract the tarballs. Within the commands below, there is an option for both GCC's and Binutils' `configure` scripts, `--target=i686-elf`. **If you are planning on building for a different architecture**, replace `i686` (e.g, `--target=x86_64-elf`). There is also another configure option, `--prefix=/usr/local`. This can be changed to `/`, `/usr`, or anything custom (such as `~/x-tools`), as long as the `$PREFIX/bin` folder is in the `$PATH` environment variable that you're building with (e.g, with `$PREFIX=~/x-tools`, run `export PATH=$PATH:~/x-tools/bin`).

Run the following commands within the GCC directory (Note, you are building GCC from source and this may take a while):
<pre>
$ mkdir -pv build && cd build
$ ../configure --target=i686-elf --prefix=/usr/local --disable-nls --enable-languages=c,c++ --without-headers
$ make all-gcc
$ make all-target-libgcc
# make install-gcc && make install-target-libgcc
</pre>
Run the following commands within the Binutils directory (Note, you are building Binutils from source and this may take a while):
<pre>
$ mkdir -pv build && cd build
$ ../configure --target=i686-elf --prefix=/usr/local --with-sysroot --disable-nls --disable-werror
$ make
# make install
</pre>
### Commands
**It is important that you are in the root directory of the repo, otherwise these commands will not funtion properly.** Run the following commands in the root of the repo as a regular user:
<pre>
make clean  # remove unwanted objects or pieces of build/
make iso    # create build/ and use it to create bootable iso
</pre>
### Running Qemu without debugging
**It is important that you are in the root directory of the repo, otherwise these commands will not funtion properly.** Run the following commands in the root of the repo as a regular user:
<pre>
qemu-system-$(ARCH) -m 512 -serial file:debug.log -cdrom zeanix.iso
</pre>
### Debugging
**It is important that you are in the root directory of the repo, otherwise these commands will not funtion properly.** Run the following commands as a regular user to create a bootable ISO with debug information:
<pre>
make clean  # remove unwanted objects or pieces of build/
make debug  # run GCC on all C files, but with "__K_DEBUG" defined and with debug symbols
            # it will create the iso automatically with GDB information
</pre>
To debug with Qemu and GDB, on i686, make sure you have Qemu and GDB installed (any GDB install will work, including from the package manager). You can then debug with the following commands (in two separate shell instances):
<pre>
qemu-system-i386 -s -S -no-shutdown -no-reboot -serial file:debug.log -m 512 -cdrom zeanix.iso # tty1
gdb # tty2
</pre>
The options for Qemu will force it to wait for GDB, not reboot the guest upon triple fault, not close the instance upon shutdown, redirect serial output of the OS into a file called "debug.log" (it will only be invalid encoding if there is an error), and provide Zeanix with 512MB of RAM, while booting from the iso file created by Make. If GDB gives an error about `$REPOROOT/.gdbinit` or something similar not being auto-loaded for security, you can either disable this security or copy the commands in the `.gdbinit` file and paste them into GDB. 
