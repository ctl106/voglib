#ifndef VOGLIB_CLIENT_H
#define	VOGLIB_CLIENT_H

#include <xc.h>
#include <stdlib.h>

#ifdef VOG_DEFINE_GLOBALS
#define VOG_GLOBAL
#else
#define VOG_GLOBAL extern
#endif

VOG_GLOBAL uint8_t VOICE_OF_GOD_IN;
VOG_GLOBAL uint8_t VOICE_OF_GOD_OUT;


void vog_read(uint8_t *buf, size_t count);
void vog_read_byte(uint8_t *data);
void vog_write(uint8_t *buf, size_t count);
void vog_write_byte(uint8_t data);


#endif	/* VOGLIB_CLIENT_H */

