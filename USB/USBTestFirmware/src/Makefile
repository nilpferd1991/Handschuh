# WinAVR cross-compiler toolchain is used here
CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = avrdude

MAINNAME = USBTest

CFLAGS = -Wall -Os -Iusbdrv -I. -mmcu=atmega16 -DF_CPU=16000000L -std=c99
OBJFLAGS = -j .text -j .data -O ihex
DUDEFLAGS = -pm16 -cstk500v2 -v

RELEASEDIR = Release

# Object files for the firmware
OBJECTS = usbdrv/usbdrv.o usbdrv/usbdrvasm.o $(RELEASEDIR)/main.o $(RELEASEDIR)/usb.o

# By default, build the firmware and command-line client, but do not flash
all: $(RELEASEDIR)/$(MAINNAME).hex

# With this, you can flash the firmware by just typing "make flash" on command-line
flash: $(RELEASEDIR)/$(MAINNAME).hex
	$(DUDE) $(DUDEFLAGS) -U flash:w:$<

# One-liner to compile the command-line client from usbtest.c
#$(CMDLINE): usbtest.c
#	gcc -I ./libusb/include -L ./libusb/lib/gcc -O -Wall usbtest.c -o usbtest.exe -lusb

# Housekeeping if you want it
clean:
	$(RM) $(RELEASEDIR)/*.o $(RELEASEDIR)/*.hex $(RELEASEDIR)/*.elf usbdrv/*.o

# From .elf file to .hex
$(RELEASEDIR)/%.hex: $(RELEASEDIR)/%.elf
	$(OBJCOPY) $(OBJFLAGS) $< $@

# Main.elf requires additional objects to the firmware, not just main.o
$(RELEASEDIR)/%.elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Without this dependance, .o files will not be recompiled if you change 
# the config! I spent a few hours debugging because of this...
$(OBJECTS): usbconfig.h usb.h

# From C source to .o object file
usbdrv/%.o: usbdrv/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
$(RELEASEDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# From assembler source to .o object file
usbdrv/%.o: usbdrv/%.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@
