#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "voglib_host.h"


#define GETC(stream)	({char __clear_input_char = getc(stream);	\
						__clear_input_char == '\n'					\
							? getc(stream)							\
							: __clear_input_char;					\
						})


#ifndef VOG_CLIENT_TEST
#define VOG_CLIENT_TEST "/home/wizerdwolf/Projects/voglib/client/dist/default/production/client.production.elf"
#endif

#ifndef VOG_CLIENT_DEV
#define VOG_CLIENT_DEV "dsPIC33EP128GP502"
#endif // VOG_CLIENT_DEV


int main()
{
	printf("Initializing pic...\n");
	vogdev *pic = vog_init(VOG_CLIENT_DEV, VOG_CLIENT_TEST);
	printf("Pic initialized\n");
	vog_run(pic);
/*
	int loop = 1;
	size_t basesize = 100;
	size_t size;
	size_t bread;
	uint8_t byte = 0;
	uint8_t *data = NULL;
	while (loop) {
		bread = 0;
		size = 0;

		printf("INPUT:\t");
		byte = getchar();
		while (byte != '\n') {
			if (bread >= size) {
				size += basesize;
				data = realloc(data, size);
			}

			data[bread] = byte;
			byte = getchar();
			bread++;
		}
		data[bread] = '\0';
		bread++;

		if (strncmp((const char *)data, "quit", strlen("quit")) == 0)
			loop = 0;
		else {
printf("Data read:\t\"%s\"\n", data);
			vog_write(pic, data, bread);
			memset(data, 0, size);
			vog_read(pic, data, bread);
			printf("OUTPUT:\t%s\n", data);
		}

		free(data);
		data = NULL;
	}
*/
	uint8_t data = 'k';
	vog_write_byte(pic, data);
	printf("Wrote:\t%c\t", data);
	vog_read_byte(pic, &data);
	printf("Read:\t%c\n", data);

	vog_close(pic);
}
