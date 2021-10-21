/*
 * Copyright (c) 2014-2017 Luc Hondareyte
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>


#ifdef __AVR__
 #include "io.h"
#endif

extern void MIDI_Event(void);
extern volatile uint8_t buffer;

#ifndef bit_is_set
 #define bit_is_set(var,pos) ((var) & (1<<(pos)))
#endif

        
char            t_fmt[64], t_buf[64];
struct timeval  tv;
struct tm       *tm;

void get_time(void) {
	gettimeofday(&tv, NULL);
	if((tm = localtime(&tv.tv_sec)) != NULL)
	{
		strftime(t_fmt, sizeof t_fmt, "%Y-%m-%d , %H:%M:%S ,", tm);
		snprintf(t_buf, sizeof t_buf, t_fmt, tv.tv_usec);
	}
}

int main(int argc, char *argv[]) {

	FILE *fp;
	uint8_t c;
	if ( argc != 2 ) {
		fprintf ( stderr, "Usage : %s <midi_device>\n", argv[0] );
		return 1;
	}
	if ((fp=fopen(argv[1],"r"))==NULL) {
		perror(argv[1]);
		exit (1);
	}
	while (!feof(fp)) {
		c=fgetc(fp);
		buffer=c;
		MIDI_Event();
	}
	return 0;
}
