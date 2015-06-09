# Name: Makefile
# Project: handschuh
# Author: Christian Starkjohann, Nils Braun
# Creation Date: 2008-04-07
# Tabsize: 4
# Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
# License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
# This Revision: $Id$

DEVICE  = atmega16
F_CPU   = 16000000	# in Hz
FUSE_L  = 0xDF      # CKSEL=111, SUT=01
FUSE_H  = 0xD9      # defaults
PROGRAMMER = stk500v2 
CFLAGS  = -DDEBUG_LEVEL=0 -std=c99 -Wall -Os

# Path names
BUILD_DIR = build
EXTERNAL_DIR = externals
SRC_DIR = src
INCLUDE_DIR = include
MAIN_NAME = main


#######################################################################
GCC = avr-gcc
CFLAGS += -Iinclude -I$(EXTERNAL_DIR)
AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE)
COMPILE = $(GCC) -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)
COMPILE_C = $(COMPILE) -c
COMPILE_S = $(COMPILE) -x assembler-with-cpp -c

EXTERNAL_DIRECTORIES = $(wildcard $(EXTERNAL_DIR)/*)
EXTERNAL_OBJECTS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(foreach dir, $(EXTERNAL_DIRECTORIES), $(wildcard $(dir)/*.c)))
EXTERNAL_OBJECTS += $(patsubst %.s, $(BUILD_DIR)/%.o, $(foreach dir, $(EXTERNAL_DIRECTORIES), $(wildcard $(dir)/*.s)))
EXTERNAL_OBJECTS += $(patsubst %.S, $(BUILD_DIR)/%.o, $(foreach dir, $(EXTERNAL_DIRECTORIES), $(wildcard $(dir)/*.S)))
EXTERNAL_HEADERS = $(foreach dir, $(EXTERNAL_DIRECTORIES), $(wildcard $(dir)/*.h))

OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c))
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ....... to build $(MAIN_NAME).hex"
	@echo "make program ... to flash fuses and firmware"
	@echo "make fuse ...... to flash the fuses"
	@echo "make flash ..... to flash the firmware (use this on metaboard)"
	@echo "make clean ..... to delete objects and hex file"

program: flash fuse

hex: $(MAIN_NAME).hex

# rule for programming fuse bits:
fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m

# rule for uploading firmware:
flash: $(MAIN_NAME).hex
	$(AVRDUDE) -U flash:w:$(MAIN_NAME).hex:i

# rule for deleting dependent files (those which can be built by Make):
clean:
	rm -rf $(BUILD_DIR) $(MAIN_NAME).hex

# file targets:
$(BUILD_DIR)/$(MAIN_NAME).elf: create-directories $(OBJECTS) $(EXTERNAL_OBJECTS)
	$(COMPILE) -o $(BUILD_DIR)/$(MAIN_NAME).elf $(OBJECTS) $(EXTERNAL_OBJECTS)

$(MAIN_NAME).hex: $(BUILD_DIR)/$(MAIN_NAME).elf
	rm -f $(MAIN_NAME).hex
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/$(MAIN_NAME).elf $(MAIN_NAME).hex
	avr-size $(MAIN_NAME).hex

# debugging targets:
disasm:	$(BUILD_DIR)/$(MAIN_NAME).elf
	avr-objdump -d $(BUILD_DIR)/$(MAIN_NAME).elf

cpp:
	$(COMPILE) -E $(MAIN_NAME).c

# Directories
create-directories:	create-root-directories	create-external-directories
	
create-root-directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(EXTERNAL_DIR)
	
create-external-directories:
	$(foreach dir, $(EXTERNAL_DIRECTORIES), mkdir -p $(BUILD_DIR)/$(dir);)

# General building stuff
$(BUILD_DIR)/%.o: %.c $(EXTERNAL_HEADERS) $(HEADERS)
	$(COMPILE_C) $< -o $@
	
$(BUILD_DIR)/%.o: %.S $(EXTERNAL_HEADERS) $(HEADERS)
	$(COMPILE_S) $< -o $@
	
$(BUILD_DIR)/%.o: src/%.c $(EXTERNAL_HEADERS) $(HEADERS)
	$(COMPILE_C) $< -o $@
	
$(BUILD_DIR)/%.o: src/%.S $(EXTERNAL_HEADERS) $(HEADERS)
	$(COMPILE_C) $< -o $@ 