export CFLAGS = -std=c99 -g
export LINKFLAGS=
export ASMFLAGS=
export CC=gcc
export CXX=g++
export LD=gcc
export ASM=nasm
export LINKFLAGS =
export LIBS=

export TARGET=i686-elf
export TARGET_ASM=nasm
export TARGET_ASMFLAGS=
export TARGET_CGLAGS= -std=c99 -g
export TARGET_CC=$(TARGET)-gcc
export TARGET_CXX=$(TARGET)-g++
export TARGET_LD=$(TARGET)-gcc
export TARGET_LIBS=
export TARGET_LINKFLAGS=

export SRC_DIR=src
export OS_DIR= $(abspath .)
export TOOLS_DIR=tools
export BUILD_DIR=$(abspath build)

BINUTILS_VERSION=2.46.0
BINUTILS_URL=https://ftp.gnu.org/gnu/binutils/binutils-$(BINUTILS_VERSION).tar.xz

GCC_VERSION=16.1.0
GCC_URL=https://ftp.gnu.org/gnu/gcc/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.xz
