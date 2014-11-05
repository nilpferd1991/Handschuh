# Name: Makefile
# Project: custom-class example
# Author: Christian Starkjohann
# Creation Date: 2008-04-06
# Tabsize: 4
# Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
# License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
# This Revision: $Id$


# Concigure the following definitions according to your system.
# This Makefile has been tested on Mac OS X, Linux and Windows.

# Use the following 3 lines on Unix (uncomment the framework on Mac OS X):
USBFLAGS = `libusb-config --cflags`
USBLIBS = `libusb-config --libs`
NCLIBS = -lncurses
ROOTLIBS = `root-config --glibs`
ROOTFLAGS = `root-config --cflags`

NAME = main

OBJECTS = opendevice/opendevice.o $(NAME).o usb.o driver.o

CC		= g++
GCC		= g++
CFLAGS	= $(CPPFLAGS) $(USBFLAGS) -O -g -Wall
LIBS	= $(USBLIBS) $(NCLIBS)

PROGRAM = $(NAME)


all: $(PROGRAM)

.c.o:
	$(CC) $(CFLAGS) -c $<
	
.cpp.o:
	$(GCC) $(CFLAGS) -c $<

$(PROGRAM): $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS) $(LIBS)

strip: $(PROGRAM)
	strip $(PROGRAM)

clean:
	rm -f *.o $(PROGRAM)
