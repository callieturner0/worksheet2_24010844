ASM = nasm
LD = ld
CC = gcc
QEMU = qemu-system-i386

CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-pic -fno-pie -O0
ASFLAGS = -f elf

all: os.iso

# Assembly files
source/loader.o: source/loader.asm
	$(ASM) $(ASFLAGS) source/loader.asm -o source/loader.o

drivers/io.o: drivers/io.s
	$(ASM) $(ASFLAGS) drivers/io.s -o drivers/io.o

drivers/interrupt_handlers.o: drivers/interrupt_handlers.s
	$(ASM) $(ASFLAGS) drivers/interrupt_handlers.s -o drivers/interrupt_handlers.o

drivers/interrupt_asm.o: drivers/interrupt_asm.s
	$(ASM) $(ASFLAGS) drivers/interrupt_asm.s -o drivers/interrupt_asm.o

drivers/hardware_interrupt_enabler.o: drivers/hardware_interrupt_enabler.s
	$(ASM) $(ASFLAGS) drivers/hardware_interrupt_enabler.s -o drivers/hardware_interrupt_enabler.o

# C source files
source/kernel.o: source/kernel.c
	$(CC) $(CFLAGS) -c source/kernel.c -o source/kernel.o

source/functions.o: source/functions.c
	$(CC) $(CFLAGS) -c source/functions.c -o source/functions.o

drivers/framebuffer.o: drivers/framebuffer.c drivers/framebuffer.h
	$(CC) $(CFLAGS) -c drivers/framebuffer.c -o drivers/framebuffer.o

drivers/pic.o: drivers/pic.c drivers/pic.h drivers/io.h drivers/types.h
	$(CC) $(CFLAGS) -c drivers/pic.c -o drivers/pic.o

drivers/interrupts.o: drivers/interrupts.c drivers/interrupts.h drivers/pic.h drivers/io.h drivers/framebuffer.h drivers/keyboard.h
	$(CC) $(CFLAGS) -c drivers/interrupts.c -o drivers/interrupts.o

drivers/keyboard.o: drivers/keyboard.c drivers/keyboard.h drivers/io.h
	$(CC) $(CFLAGS) -c drivers/keyboard.c -o drivers/keyboard.o

# Link the kernel
kernel.elf: source/loader.o source/kernel.o source/functions.o drivers/framebuffer.o drivers/io.o drivers/interrupt_handlers.o drivers/interrupt_asm.o drivers/hardware_interrupt_enabler.o drivers/pic.o drivers/interrupts.o drivers/keyboard.o
	$(LD) -T ./source/link.ld -melf_i386 $^ -o kernel.elf

# Create ISO
iso/boot/grub/stage2_eltorito:
	mkdir -p iso/boot/grub
	cp /opt/os/stage2_eltorito iso/boot/grub/

iso/boot/kernel.elf: kernel.elf
	mkdir -p iso/boot
	cp kernel.elf iso/boot/kernel.elf

os.iso: kernel.elf iso/boot/kernel.elf iso/boot/grub/stage2_eltorito iso/boot/grub/menu.lst
	genisoimage -R \
	-b boot/grub/stage2_eltorito \
	-no-emul-boot \
	-boot-load-size 4 \
	-A os \
	-input-charset utf8 \
	-quiet \
	-boot-info-table \
	-o os.iso \
	iso

run: os.iso
	$(QEMU) -display curses -monitor telnet::45458,server,nowait -serial mon:stdio -boot d -cdrom os.iso -m 32

clean:
	rm -f source/*.o drivers/*.o kernel.elf iso/boot/kernel.elf os.iso logQ.txt

.PHONY: all run clean