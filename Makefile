# =========================================================================================== 

ASPARAMS = --32
LDPARAMS = -melf_i386

GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
GPPPARAMS += -g -ggdb3
GPPPARAMS += -Iinclude

OBJECTS = 	obj/loader.o \
			obj/kernel.o \
			obj/gdt.o \
			obj/drivers/driver.o \
			obj/drivers/keyboard.o \
			obj/drivers/mouse.o \
			obj/hardware_communications/irq.o \
			obj/hardware_communications/irq_table.o \
			obj/user_interface/display.o

TRGT = zos_kernel

# =========================================================================================== 

all: $(TRGT).bin

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -c -o $@ $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

$(TRGT).bin: linker.ld $(OBJECTS)
	@ld $(LDPARAMS) -T $< -o $@ $(OBJECTS)

$(TRGT).iso: $(TRGT).bin
	@mkdir -p iso
	@mkdir -p iso/boot
	@mkdir -p iso/boot/grub
	@cp $< iso/boot
	@echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	@echo 'set default=0' >> iso/boot/grub/grub.cfg
	@echo '' >> iso/boot/grub/grub.cfg
	@echo 'menuentry "ZeroOS" {' >> iso/boot/grub/grub.cfg
	@echo '   multiboot  /boot/$(TRGT).bin' >> iso/boot/grub/grub.cfg
	@echo '   boot' >> iso/boot/grub/grub.cfg
	@echo '} ' >> iso/boot/grub/grub.cfg
	@grub-mkrescue --output=$@ iso
	@echo '=== ISO built ==='

# =========================================================================================== 

VM_NAME = ZeroOS Tester

# =========================================================================================== 

run: image
	@echo '=== VM started ==='
	@VirtualBoxVM --startvm "$(VM_NAME)"


image: $(TRGT).bin $(TRGT).iso
	@rm -rf iso

install: $(TRGT).bin
	@sudo cp $< /boot/$(TRGT).bin
	@echo '=== BIN loaded to boot (do not forget to upd grub cfg) ==='

clean:
	@rm -rf obj
	@rm -rf *.bin
	@rm -rf iso
	@rm -rf *.iso

# ===========================================================================================

# (killall VirtualBoxVM) || true                                            