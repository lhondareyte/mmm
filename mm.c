#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "midi.h"


#ifdef __AVR__
 #include "io.h"
#endif

#ifndef __AVR_MIDI_H__
 #include "midi.h"
#endif

#ifndef bit_is_set
 #define bit_is_set(var,pos) ((var) & (1<<(pos)))
#endif

int main(int argc, char *argv[]) {

	FILE *fp;
	uint8_t c;
	if ( argc != 2 ) {
		fprintf ( stderr, "Usage : %s <midi_device>\n", argv[0] );
		return 1;
	}
	if ((fp=fopen(argv[1],"r"))==NULL)
	{
		perror(argv[1]);
		exit (1);
	}
	while (!feof(fp))
	{
		c=fgetc(fp);
		buffer=c;
		MIDI_Event();
	}
	return 0;
}
