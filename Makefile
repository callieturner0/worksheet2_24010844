ASM = nasm
LD = ld
QEMU = qemu-system-i386

all: os.iso

source/loader.o: source/loader.asm
	$(ASM) -f elf source/loader.asm -o source/loader.o

kernel.elf: source/loader.o source/link.ld
	$(LD) -T ./source/link.ld -melf_i386 source/loader.o -o kernel.elf

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
	$(QEMU) -nographic -boot d -cdrom os.iso -m 32 -d cpu -D logQ.txt

clean:
	rm -f source/loader.o kernel.elf iso/boot/kernel.elf os.iso logQ.txt

.PHONY: all run clean
