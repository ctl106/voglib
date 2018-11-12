#include <stdlib.h>

#include "voglib_host.h"

#define MDBLIB	// REMOVE_ME


static const char in_name[] = "VOICE_OF_GOD_IN";
static const char out_name[] = "VOICE_OF_GOD_OUT";


#ifdef MDBLIB
typedef mdbhandle	idev;
typedef mdbptr		iptr;
#else
typedef void	idev;
typedef void	iptr;
#endif // MDBLIB

struct _vogdev {
	idev *interface;
	iptr in_addr;
	iptr out_addr;
};

vogdev *vog_init(char *devname, char *program)
{
	vogdev *device = malloc(sizeof(vogdev));

	#ifdef MDBLIB
	device->interface = mdb_init();
	mdb_device(device->interface, devname);
	mdb_hwtool(device->interface, "SIM", 0);
	mdb_program(device->interface, program);
	device->in_addr = mdb_print_var_addr(device->interface, in_name);
	device->out_addr = mdb_print_var_addr(device->interface, out_name);
    mdb_watch(device->interface, device->in_addr, 'R', 0);
    mdb_watch(device->interface, device->out_addr, 'W', 0);

	#else
	free(device);
	device = NULL;
	#endif // MDBLIB

	return device;
}

void vog_close(vogdev *device)
{
	#ifdef MDBLIB
	mdb_delete_all(device->interface);
	mdb_quit(device->interface);
	mdb_close(device->interface);
	#endif

	free(device);
	return;
}

void vog_run(vogdev *device)
{
	#ifdef MDBLIB
	mdb_run(device->handle);
	#endif // MDBLIB

	return;
}

void vog_read(vogdev *device, char *buf, size_t count)
{
	size_t i;
	for(i = 0; i < count; i++)
		vog_read_byte(device, buf[i]);
}

void vog_read_byte(vogdev *device, uint8_t *data)
{
	#ifdef MDBLIB
	mdb_wait(device->interface);
	mdb_stepi(device->interface);	// allow the value to be written
	mdbword output = (mdbword)mdb_print_var(device->interface, 'd', 1, out_name);
	*data = (uint8_t)(0x00FF & output);
	mdb_continue(device->interface);

	#endif // MDBLIB
	return;
}

void vog_write(vogdev *device, cont char *buf, size_t count)
{
	size_t i;
	for(i = 0; i < count; i++)
		vog_write_byte(proc, buf[i]);
}

void vog_write_byte(vogdev *device, uint8_t data)
{
	#ifdef MDBLIB
	mdbword temp[1];	// mdb_write_mem() requires an array as input
	temp[0] = (mdbword)mdb_print_var(device->interface, 'd', 1, out_name);
	temp[0] &= ~0x00FF;	// not 100% the correct endian-ness; will determine with testing
	temp[0] |= (0x00FF & (mdbword)data);	// back up the word at the address before writing
	mdb_write_mem(device->interface, 'r', device->out_addr, 1, temp);
	mdb_continue(device->interface);

	#endif // MDBLIB
	return;
}
