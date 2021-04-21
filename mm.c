/*
 *   Copyright (c)2014-2017,  Luc Hondareyte
 *   All rights reserved.
 *     
 *   Redistribution and use in source and binary forms, with or without 
 *   modification, are permitted provided that the following conditions 
 *   are met:
 *  
 *   1. Redistributions of source code must retain the above copyright 
 *      notice, this list of conditions and the following disclaimer.
 *  
 *   2. redistributions in binary form must reproduce the above copyright 
 *      notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *  
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
 *   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 *   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 *   OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 *   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 *   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 *   SUCH DAMAGE.
 * 
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
