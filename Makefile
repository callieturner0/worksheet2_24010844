ASM = nasm
LD = ld
CC = gcc
QEMU = qemu-system-i386

CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-pic -fno-pie -O0

all: os.iso

source/loader.o: source/loader.asm
	$(ASM) -f elf source/loader.asm -o source/loader.o

source/kernel.o: source/kernel.c
	$(CC) $(CFLAGS) -c source/kernel.c -o source/kernel.o

source/functions.o: source/functions.c
	$(CC) $(CFLAGS) -c source/functions.c -o source/functions.o

kernel.elf: source/loader.o source/kernel.o source/functions.o source/link.ld
	$(LD) -T ./source/link.ld -melf_i386 source/loader.o source/kernel.o source/functions.o -o kernel.elf

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
	$(QEMU) -nographic -boot d -cdrom os.iso -m 32

clean:
	rm -f source/*.o kernel.elf iso/boot/kernel.elf os.iso logQ.txt

.PHONY: all run clean