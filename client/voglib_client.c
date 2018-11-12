#define VOG_DEFINE_GLOBALS
#include "voglib_client.h"


void vog_read(uint8_t *buf, size_t count)
{
	size_t i;
	for (i = 0; i < count; i++)
		vog_read_byte(buf + i);
}

void vog_read_byte(uint8_t *data)
{
	*data = VOICE_OF_GOD_IN;
}

void vog_write(uint8_t *buf, size_t count)
{
	size_t i;
	for (i = 0; i < count; i++)
		vog_write_byte(buf+i);
}

void vog_write_byte(uint8_t data)
{
	VOICE_OF_GOD_OUT = data;
}
