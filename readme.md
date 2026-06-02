# CataOS

**CataOS** is a simple x86 operating system built from the ground up from scratch.
It has no external dependencies and is completely stand-alone.

The project is written mostly in **Assembly** and **C**, with additional **C++** support.
There are also future plans to explore a full **Rust** overhaul.

---

## Overview

CataOS is a passion project I've decided to built to understand how an operating system works and how assembly code is translated to machine instructions. While I haven't defined an end for this project, I'd like to work on this throughout my years, learning and expanding my knowledge on low-level systems programming, kernel development, bootloading, memory management, and hardware interaction on x86 systems

This project is intended to be:

- Minimal
- Self-contained and independent
- Easy to understand
- Built from scratch
- Expandable

---

## Goals

The main goals of CataOS are:

- Build a working x86 operating system from scratch
- Keep the codebase small and readable
- Learn how bootloaders, kernels, and hardware interfaces work
- Implement a basic CLI with ability to read/write files
- Support future expansion with cleaner abstractions
- Eventually improve the system with safer and more modern language support

---

## Current Status

CataOS is currently in active development.

At this time of writing, the project includes :

- Bootloader development
- Kernel initialization
- Basic text output
- CPU and memory setup
- Early device and interrupt handling
- File system or disk access experiments



---

## Features

Planned or existing features include:

- x86 boot process
- Custom kernel
- Low-level hardware control
- Assembly and C-based core
- Future C++ support
- Possible Rust migration in the future
- No external runtime dependencies
- Bare-metal execution

---

## Tech Stack

- **Assembly** for boot and early hardware setup -> already implemented
- **C** for kernel and low-level system code -> kernel is currently being developed
- **C++** for higher-level kernel components
- **Rust** planned for future modernization
- **x86** target architecture

---

## Repository Structure
Cata-OS
-> build_scripts
-> src
  -> bootloader
    -> stage1
    -> stage2
  -> kernel
    -> arch/i686
    -> hal
    -> util
-> tools
    -> fat
-> readme.md


# How to Install
-> clone the repo
-> ensure that gcc, c, python, qemu are installed (will be bundled in a .iso file very soon)
-> `make`
-> `qemu-system-i386 -fda build/main_floppy.img`
-> enjoy the os in all its glory
