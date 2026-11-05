# Worksheet 2 – Operating Systems

## Task 1 – Creating a Tiny Operating System

### Introduction

The aim of Task 1 was to create a basic operating system kernel using assembly language, configure a bootloader and produce a bootable ISO image. I used NASM to assemble the kernel, GNU LD to link it and GRUB to load the operating system. I also used QEMU to test the operating system in a virtual environment.

### 1. Assembly Loader

I created `loader.asm` inside the source folder. This file contains the initial assembly code for the kernel.

The code includes a Multiboot header containing a magic number, flags and checksum. These values allow GRUB to recognise the kernel as a Multiboot-compatible executable.

The loader then sets the EAX register to `0xCAFEBABE` and enters an infinite loop. This provides a simple way to check that the kernel has successfully loaded and started executing.

### 2. Linker Script

I created `link.ld` to define how the kernel is organised when it is linked.

The linker script specifies the sections of the kernel, including `.text`, `.rodata`, `.data` and `.bss`. This ensures that the assembled code is organised into an executable ELF file.

### 3. GRUB Configuration

I created a `menu.lst` file inside `iso/boot/grub/`.

This file configures the GRUB boot menu and tells it where to find the kernel. The configuration includes the operating system entry and the location of `kernel.elf`.

I also copied the GRUB bootloader file `stage2_eltorito` into the ISO directory.

### 4. Creating the Bootable ISO

I used NASM to assemble the loader:

```bash
nasm -f elf source/loader.asm -o source/loader.o
```

I then used the linker to generate the kernel ELF file:

```bash
ld -T ./source/link.ld -melf_i386 source/loader.o -o kernel.elf
```

After copying the required GRUB and kernel files into the ISO directory, I used `genisoimage` to generate the bootable ISO image:

```bash
genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o os.iso iso
```

### 5. Testing Using QEMU

I used QEMU to test the operating system:

```bash
qemu-system-i386 -nographic -boot d -cdrom os.iso -m 32 -d cpu -D logQ.txt
```

The GRUB boot menu appeared and successfully loaded the kernel.

I checked the CPU execution log using:

```bash
grep -i "CAFEBABE" logQ.txt
```

The output showed that the EAX register contained `CAFEBABE`, confirming that the kernel had reached the assembly loader code and was executing successfully.

### 6. Makefile

I created a Makefile to automate the build process. This means I do not need to manually enter each assembly, linking and ISO creation command.

The Makefile includes the following targets:

* `make` – Builds the operating system ISO.
* `make run` – Launches the ISO using QEMU.
* `make clean` – Removes generated build files and logs.

This makes the development process easier and allows the operating system to be rebuilt whenever changes are made.

### Conclusion

Task 1 allowed me to gain practical experience with assembly language, linking, bootloaders and virtualisation. I successfully created a basic kernel, configured GRUB and generated a bootable ISO image. Testing the operating system in QEMU and checking the EAX register confirmed that the kernel was successfully executing.
