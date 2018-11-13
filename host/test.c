#include <stdio.h>

#include "voglib_host.h"


#ifndef VOG_CLIENT_TEST
#define VOG_CLIENT_TEST "/home/wizerdwolf/Projects/voglib/client/dist/default/production/client.production.hex"
#endif

#ifndef VOG_CLIENT_DEV
#define VOG_CLIENT_DEV "dsPIC33EP128GP502"
#endif // VOG_CLIENT_DEV


int main()
{
	vogdev *pic = vog_init(VOG_CLIENT_DEV, VOG_CLIENT_TEST);
	vog_run(pic);

	int loop = 1;
	uint8_t data = 0;
	while (loop) {
		printf("INPUT:\t");
		data = getchar();

		if (data == 'q')
			loop = 0;
		else {
			vog_write_byte(pic, data);
			vog_read_byte(pic, &data);
			printf("OUTPUT:\t%c\n", data);
		}
	}

	vog_close(pic);
}
