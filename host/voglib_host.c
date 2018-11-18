#include <stdlib.h>
#include <stdio.h>	// REMOVE_ME
#include <inttypes.h>	// REMOVE_ME

#include "voglib_host.h"

#ifdef DEBUG
#define VOG_DBG(format, ...)			\
	do {									\
			fprintf(stderr, "%s: %s(): %d:\n\t"format, __FILE__, __func__, __LINE__, ## __VA_ARGS__ );	\
	} while (0);
#else
#define VOG_DBG(format, args...)
#endif // DEBUG


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
	printf("\tUsing mdblib; trying to init\n");	// REMOVE_ME
	device->interface = mdb_init();
	printf("\tMdblib initialized:\t0x%"PRIXPTR"\n", (uintptr_t)device->interface);	// REMOVE_ME
	mdb_device(device->interface, devname);
	printf("\tDevice set\n");	// REMOVE_ME
	mdb_hwtool(device->interface, "SIM", 0, 0);
	printf("\tHwtool set\n");	// REMOVE_ME
	mdb_program(device->interface, program);
	printf("\tProgrammed file to device\n");	// REMOVE_ME
	device->in_addr = mdb_print_var_addr(device->interface, in_name);
	printf("\n");	// REMOVE_ME
	device->out_addr = mdb_print_var_addr(device->interface, out_name);
	printf("Reading addresses:\tin_addr: 0x%"MDB_PRIXPTR"\tout_addr: 0x%"MDB_PRIXPTR"\n", device->in_addr, device->out_addr);	// REMOVE_ME
    int bn1 = mdb_watch_name(device->interface, in_name, "R", 0);
    printf("1st watchpoint set:\t%d\n", bn1);
    int bn2 = mdb_watch_name(device->interface, out_name, "RW", 0);
    //int bn2 = mdb_break_addr(device->interface, device->out_addr, 0);
    printf("2nd watchpoint set:\t%d\n", bn2);

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
	mdb_run(device->interface);
	#endif // MDBLIB

	return;
}

void vog_read(vogdev *device, uint8_t *buf, size_t count)
{
	size_t i;
	for(i = 0; i < count; i++) {
		vog_read_byte(device, buf+i);
printf("Value check:\t\"%s\"\n", buf);
	}
}

void vog_read_byte(vogdev *device, uint8_t *data)
{
	#ifdef MDBLIB
	//mdb_wait(device->interface);
	while (mdb_state(device->interface) == mdb_running) mdb_noop(device->interface);
	mdb_step(device->interface);	// allow the value to be written
	mdbword output = (mdbword)mdb_print_var(device->interface, 'x', 1, out_name);
	*data = (uint8_t)(0x00FF & output);
	mdb_continue(device->interface);
	VOG_DBG("Read 0x%"MDB_PRIWORD" from device\n", output);

	#endif // MDBLIB
	return;
}

void vog_write(vogdev *device, const uint8_t *buf, size_t count)
{
	size_t i;
	for(i = 0; i < count; i++)
		vog_write_byte(device, buf[i]);
}

void vog_write_byte(vogdev *device, uint8_t data)
{
	#ifdef MDBLIB
	mdbword temp[1];	// mdb_write_mem() requires an array as input
	temp[0] = (mdbword)mdb_print_var(device->interface, 'x', 1, out_name);
	temp[0] &= ~0x00FF;	// not 100% the correct endian-ness; will determine with testing
	temp[0] |= (0x00FF & (mdbword)data);	// back up the word at the address before writing
	mdb_write_mem(device->interface, 'r', device->in_addr, 1, temp);

	uint8_t result = (uint8_t)mdb_print_var(device->interface, 'x', 1, in_name);
	if (data != result) {
		VOG_DBG("Value failed to write! wrote: 0x%"MDB_PRIWORD" Read: 0x%"MDB_PRIWORD"\n", data, result);
		exit(1);
	}

	while (mdb_state(device->interface) == mdb_running) mdb_noop(device->interface);
	mdb_continue(device->interface);
	while (mdb_state(device->interface) == mdb_running) mdb_noop(device->interface);
	mdb_continue(device->interface);
	VOG_DBG("Wrote 0x%"MDB_PRIWORD" to device\n", temp[0]);

	#endif // MDBLIB
	return;
}
