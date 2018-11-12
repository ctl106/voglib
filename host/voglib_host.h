#ifndef VOGLIB_HOST_H_INCLUDED
#define VOGLIB_HOST_H_INCLUDED

/*
Define MDBLIB in the environment if using the mdblib interface.
This check is being added as I would like to implement better alternatives,
such as an open source emulator for the 16 bit Pic microcontrollers, and
accompanying trace and debug tools.
*/
#define MDBLIB	// REMOVE_ME


#include "stdint.h"


#ifdef MDBLIB
#include mdblib.h
#define VOG_PRIWORD	MDB_PRIWORD
#else
#error No supported interace was specified for voglib.
#endif // vog interface

typedef struct _vogdev vogdev;


/*
Basic usage of this library would look like this:
	vogword data = 0x00;	// could be some user-defined command
	vogdev *pic = vog_init("dsPIC33EP128GP502");
	vog_put(pic, data);		// writes command to the device
	vog_get(pic, &data);	// reads the response from the device
	printf("Result:\t%"VOG_PRIWORD"\n", data);
	vog_close(pic);			// closes the device and deconstructs

HOWEVER, the pug and get commands will be blocking since they have to wait on
the device to be ready to read/write back from/to the host device. As a result,
it may prove desireable to create a put and get thread seperate from the main
thread to prevent the whole vogdev *deviceess from locking up, if it is going to be doing
anything in addition to this reading and writing.
*/

vogdev *vog_init(char *devname, char *program);
void vog_close(vogdev *device);
void vog_run(vogdev *device);

void vog_read(vogdev *device, uint8_t *buf, size_t count);
void vog_read_byte(vogdev *device, uint8_t *data);
void vog_write(vogdev *device, cont uint8_t *buf, size_t count);
void vog_write_byte(vogdev *device, uint8_t data);

#endif // VOGLIB_HOST_H_INCLUDED
