toolchain: toolchain_binutils toolchain_gcc

BINUTILS_URL=https://ftp.gnu.org/gnu/binutils/binutils-2.46.0.tar.xz
GCC_URL=https://ftp.gnu.org/gnu/gcc/gcc-16.1.0/gcc-16.1.0.tar.xz
TOOLCHAIN_PREFIX=toolchain/i686-elf
TARGET=i686-elf
toolchain_binutils:
	mkdir toolchain
	cd toolchain && wget $(BINUTILS_URL)
	cd toolchain && tar -xf binutils-2.46.0.tar.xz
	mkdir toolchain/binutils-build-2.46.0
	cd toolchain/binutils-build-2.46.0 && ../binutils-2.46.0/configure \
		--prefix="$(TOOLCHAIN_PREFIX)"    \
		--target=$(TARGET)                \
		--with-sysroot                    \
		--disable-nls                     \
		--disable-werror
	$(MAKE)-j8 -C toolchain/binutils-build-2.46.0
	$(MAKE) -C toolchain/binutils-build-2.46.0 install

toolchain_gcc: toolchain_binutils
	mkdir toolchain
	cd toolchain && wget $(GCC_URL)
	cd toolchain && tar -xf gcc-16.1.0.tar.xz
	mkdir toolchain/gcc-build-16.1.0
	cd toolchain/gcc-build-16.1.0 && ../gcc-16.1.0/configure \
		--prefix="$(TOOLCHAIN_PREFIX)"    \
		--target=$(TARGET)                \
		--disable-nls                     \
		--enable-languages=c,c++					\
		--without-headers
	$(MAKE)-j8 -C toolchain/binutils-build-2.46.0
	$(MAKE) -C toolchain/binutils-build-2.46.0 install
