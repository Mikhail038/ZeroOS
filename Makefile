GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o kernel.o

TRGT = mykernel

%.o: %.cpp
	@g++ $(GPPPARAMS) -o $@ -c $<
	@echo '=== Kernel built ==='

%.o: %.s
	@as $(ASPARAMS) -o $@ $<
	@echo '=== ASM entry built ==='


$(TRGT).bin: linker.ld $(objects)
	@ld $(LDPARAMS) -T $< -o $@ $(objects)
	@echo '=== BIN built ==='


$(TRGT).iso: $(TRGT).bin
	@mkdir -p iso
	@mkdir -p iso/boot
	@mkdir -p iso/boot/grub
	@cp $< iso/boot
	@echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	@echo 'set default=0' >> iso/boot/grub/grub.cfg
	@echo '' >> iso/boot/grub/grub.cfg
	@echo 'menuentry "ZeroOS" {' >> iso/boot/grub/grub.cfg
	@echo '   multiboot /boot/$(TRGT).bin' >> iso/boot/grub/grub.cfg
	@echo '   boot' >> iso/boot/grub/grub.cfg
	@echo '} ' >> iso/boot/grub/grub.cfg
	@grub-mkrescue --output=$@ iso
	@echo '=== ISO built ==='

# ===========================================================================================

VM_NAME = ZeroOS Tester

run: image
	@echo '=== VM started ==='
	@VirtualBoxVM --startvm "$(VM_NAME)"


image: $(TRGT).bin $(TRGT).iso
	@rm -rf iso

install: $(TRGT).bin
	@sudo cp $< /boot/$(TRGT).bin
	@echo '=== BIN loaded to boot (do not forget to upd grub cfg) ==='

clean:
	@rm -rf *.o
	@rm -rf *.bin
	@rm -rf iso
	@rm -rf *.iso
	@echo '=== Cleaned (.o .bin .iso) ==='

# (killall VirtualBoxVM) || true