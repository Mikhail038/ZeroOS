# =========================================================================================== 

GPPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

GPPPARAMS += -g -ggdb3

DIRECTORIES = -IPorts -IExtraLibraries -IGdt -IGui -IInterruptions -IDrivers
GPPPARAMS += $(DIRECTORIES)

OBJECTS_DIRECTORY = Objects

OBJECTS = $(OBJECTS_DIRECTORY)/loader.o $(OBJECTS_DIRECTORY)/kernel.o 
OBJECTS += $(OBJECTS_DIRECTORY)/gdt.o $(OBJECTS_DIRECTORY)/display.o
OBJECTS += $(OBJECTS_DIRECTORY)/irq.o $(OBJECTS_DIRECTORY)/irq_table.o
OBJECTS += $(OBJECTS_DIRECTORY)/keyboard.o $(OBJECTS_DIRECTORY)/mouse.o
OBJECTS += $(OBJECTS_DIRECTORY)/driver.o

TRGT = zos_kernel

# =========================================================================================== 

all: MAKE_OBJ_DIR $(TRGT).bin

$(OBJECTS_DIRECTORY)/driver.o: Drivers/driver.cpp
	@g++ $(GPPPARAMS) -o $@ -c $<
	@echo '=== Driver Manager built ==='

$(OBJECTS_DIRECTORY)/mouse.o: Drivers/mouse.cpp
	@g++ $(GPPPARAMS) -o $@ -c $<
	@echo '=== Mouse Driver built ==='

$(OBJECTS_DIRECTORY)/keyboard.o: Drivers/keyboard.cpp
	@g++ $(GPPPARAMS) -o $@ -c $<
	@echo '=== Keyboard Driver built ==='

$(OBJECTS_DIRECTORY)/display.o: Gui/display.cpp
	@g++ $(GPPPARAMS) -o $@ -c $<
	@echo '=== Display built ==='

$(OBJECTS_DIRECTORY)/kernel.o: Kernel/kernel.cpp
	@g++ $(GPPPARAMS) -o $@ -c $<
	@echo '=== Kernel built ==='

$(OBJECTS_DIRECTORY)/irq.o: Interruptions/irq.cpp
	@g++ $(GPPPARAMS) -o $@ -c $<
	@echo '=== Interrupt Manager built ==='

$(OBJECTS_DIRECTORY)/gdt.o: Gdt/gdt.cpp
	@g++ $(GPPPARAMS) -o $@ -c $<
	@echo '=== GDT built ==='

$(OBJECTS_DIRECTORY)/irq_table.o: Interruptions/irq_table.s
	@as $(ASPARAMS) -o $@ $<
	@echo '=== Interrupt Table ASM built ==='

$(OBJECTS_DIRECTORY)/loader.o: Kernel/loader.s
	@as $(ASPARAMS) -o $@ $<
	@echo '=== ASM entry built ==='


$(TRGT).bin: Kernel/linker.ld $(OBJECTS)
	@ld $(LDPARAMS) -T $< -o $@ $(OBJECTS)
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
	@echo '   multiboot  /boot/$(TRGT).bin' >> iso/boot/grub/grub.cfg
	@echo '   boot' >> iso/boot/grub/grub.cfg
	@echo '} ' >> iso/boot/grub/grub.cfg
	@grub-mkrescue --output=$@ iso
	@echo '=== ISO built ==='

MAKE_OBJ_DIR:
	@mkdir -p $(OBJECTS_DIRECTORY)/

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
	@rm -rf $(OBJECTS_DIRECTORY)/*.o
	@rm -rf *.bin
	@rm -rf iso
	@rm -rf *.iso
	@echo '=== Cleaned ($(OBJECTS_DIRECTORY)/.o .bin .iso) ==='

# ===========================================================================================

# (killall VirtualBoxVM) || true                                            