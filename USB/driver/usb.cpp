//============================================================================
// Name        : usb.cpp
// Author      : Nils Braun
// Version     : 1
// Description : Treiber für die Verbindung zum ATmega16 über USB-Schnittstelle
//============================================================================
#include "usb.h"

usb_dev_handle * handle;

// Initialisiert die USB-Schnittstelle und stellt eine Verbindung zur Hardware her

void usbInit()
{
	const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
	char                vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
	int                 vid, pid;

	usb_init();

	/* compute VID/PID from usbconfig.h so that there is a central source of information */
	vid = rawVid[1] * 256 + rawVid[0];
	pid = rawPid[1] * 256 + rawPid[0];

	/* The following function is in opendevice.c: */
	if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
	{
		fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
		exit(1);
	}

	/* Since we use only control endpoint 0, we don't need to choose a
	 * configuration and interface. Reading device descriptor and setting a
	 * configuration and interface is done through endpoint 0 after all.
	 * However, newer versions of Linux require that we claim an interface
	 * even for endpoint 0. Enable the following code if your operating system
	 * needs it: */

	int retries = 1, usbConfiguration = 1, usbInterface = 0;
	if(usb_set_configuration(handle, usbConfiguration))
	{
		fprintf(stderr, "Warning: could not set configuration: %s\n", usb_strerror());
	}
	/* now try to claim the interface and detach the kernel HID driver on
	 * Linux and other operating systems which support the call. */
	while(usb_claim_interface(handle, usbInterface) != 0 && retries-- > 0)
	{
#ifdef LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP
		if(usb_detach_kernel_driver_np(handle, 0) < 0)
		{
			fprintf(stderr, "Warning: could not detach kernel driver: %s\n", usb_strerror());
		}
#endif
	}
}

void usbClose()
{
    usb_close(handle);
}

void usbRead(const int requestCode, const int value, uint8_t * buffer, const int size)
{
	const int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, requestCode, value, 0, (char*)buffer, size, 5000);
	if(cnt < 0)
	{
		fprintf(stderr, "USB error: %s\n", usb_strerror());
		exit(1);
	}
	else if(cnt < size)
	{
		fprintf(stderr, "only %d bytes received.\n", cnt);
	}
}

void usbWrite(const int requestCode, const int value)
{
	const int cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, requestCode, value, 0, NULL, 0, 5000);
	if(cnt < 0)
	{
		fprintf(stderr, "USB error: %s\n", usb_strerror());
		exit(1);
	}
}
