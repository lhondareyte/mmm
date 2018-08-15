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
/*
 * From SYNTH PUNK CONSOLE Mark II
 * Copyright (c) 2010-2011 Luc Hondareyte
 *
 * $Id: midi.c,v 0.4 2011/03/11 10:12:06 luc Exp luc $
 */
#include <stdio.h>
#include <stdlib.h>
#include "midi.h"

#ifndef __AVR__
void MIDI_Event(void)
#else

#include "io.h"

ISR (UART_RECEIV_VECT)
#endif
{
	#if defined (__AVR__)
	buffer=UART_RX_BUF;
	#endif
	if (bit_is_set(buffer,7)) {
		// Channel messages
		if (buffer < MIDI_SYSEX_MSG) {
			status=(buffer&0xF0);
			channel=(buffer&0x0F);
			next=MIDI_DATA1;
		}
		// System Common messages
		else if ( buffer < MIDI_TIMECL_MSG ) {
			status=buffer;
			switch (buffer) {
			case MIDI_SYSEX_MSG:
				#ifndef __AVR__
			     	fprintf (stdout, "SYSEX Start\n");
				#endif
				next=MIDI_SYSEX_MSG;
				break;

			case MIDI_EOX_MSG:
				#ifndef __AVR__
			     	fprintf (stdout, "SYSEX End\n");
				#endif
				next=MIDI_UNKNOW_MSG;
				ready=TRUE;
				break;
			
			case MIDI_UNDEF1_MSG:
				ready=TRUE;
				break;

			case MIDI_UNDEF2_MSG:
				ready=TRUE;
				break;
			
			case MIDI_TUNERQ_MSG:
				ready=TRUE;
				break;
			
			default: 
				next=MIDI_DATA1;
				break;
			}
		}
		// System RealTime messages
		else {	
			switch (buffer) {
		
			// System Real time messages
			case MIDI_RESET_MSG : 
				#ifndef __AVR__
			     	fprintf (stdout, "RESET\n");
				#endif
				#if defined ( __MIDI_RESET_ENABLE__ )
				exit (0);
				#endif
				break;

			default:
				#ifndef __AVR__
			     	fprintf (stdout, "Real Time Message - Status= 0x%02x, DATA1= 0x%02x, DATA2= 0x%02x\n", 
					 status, data1, data2);
				#endif
				break;
			}
		}
	}
	// Traitement des DATA Bytes
	else {
		switch (next) {
			case MIDI_DATA1: 
				data1=buffer;
				if (status == MIDI_PROGCH_MSG || status == MIDI_CHANAF_MSG || 
					status == MIDI_QUARTER_MSG || status == MIDI_SONGSEL_MSG ) {
					next=MIDI_UNKNOW_MSG;
					ready=TRUE;
					#ifndef __AVR__
					switch (status) {
						case MIDI_PROGCH_MSG:
							fprintf(stdout, "Program Change: 0x%02x\n", data1);
							break;

						case MIDI_CHANAF_MSG:
							fprintf(stdout, "Channel AfterTouch: 0x%02x\n", data1);
							break;

						case MIDI_QUARTER_MSG:
							fprintf(stdout, "QT Frame: 0x%02x\n", data1);
							break;

						case MIDI_SONGSEL_MSG:
							fprintf(stdout, "Song Select: 0x%02x\n", data1);
							break;
					}
					#endif
				}
				else next=MIDI_DATA2;
				break;
						
			case MIDI_DATA2: 
				data2=buffer;
				switch (status) {
					case MIDI_NOTEON_MSG : 
						#ifndef __AVR__
						fprintf(stdout, "Note ON   note: 0x%02x + 0x%02x",
									data1, data2);
						#endif
						if (data2==0) 
						{	
							status=MIDI_NOTOFF_MSG;
						#ifndef __AVR__
							fprintf(stdout, " (Note OFF)");
						#endif
						}
						#ifndef __AVR__
							fprintf(stdout, "\n");
						#endif
						break;

					case MIDI_NOTOFF_MSG : 
						#ifndef __AVR__
						fprintf(stdout, "Note OFF   note: 0x%02x\n", data1);
						#endif
						break;

					case MIDI_POLYAF_MSG : 
						#ifndef __AVR__
						fprintf(stdout, "Poly. AT   note: 0x%02x velo: 0x%02x\n", 
									data1, data2);
						#endif
						break;

					case MIDI_CTRLCH_MSG : 
						#ifndef __AVR__
						fprintf(stdout, "Control Change: 0x%02x + 0x%02x\n", 
									data1, data2);
						#endif
						break;

					case MIDI_PITCHB_MSG : 
						#ifndef __AVR__
						fprintf(stdout, "Pitch Bend: 0x%02x + 0x%02x\n", 
									data1, data2);
						#endif
						break;

					case MIDI_SONGPOS_MSG :
						#ifndef __AVR__
						fprintf(stdout, "Song Position: 0x%02x + 0x%02x\n", 
									data1, data2); 
						#endif
						break;

					default: 
						#ifndef __AVR__
						fprintf (stdout, "Message PANIC MIDI!!\n");
						#endif
						status=MIDI_UNKNOW_MSG;
						next=MIDI_UNKNOW_MSG;
						ready=FALSE;
						#if defined (__MIDI_RESET_ON_ERROR__)
						exit (0);
						#endif
						break;
				}
				next=MIDI_UNKNOW_MSG;
				ready=TRUE;
				break;

			case MIDI_SYSEX_MSG :
				#if defined (MIDI_MANUFACTURER_ID)

				#endif
				next=MIDI_SYSEX_MSG;
				break;

			// About running status
			case MIDI_UNKNOW_MSG :
				if (status == MIDI_PROGCH_MSG || status == MIDI_CHANAF_MSG || 
					status == MIDI_QUARTER_MSG || status == MIDI_SONGSEL_MSG ) {
					data1=buffer;
					ready=TRUE;
					next=MIDI_UNKNOW_MSG;
				#ifndef __AVR__
					fprintf(stdout, "(Running status On) - 1 \n");
					switch (status) {
						case MIDI_PROGCH_MSG : 
							break;
	
						case MIDI_CHANAF_MSG : 
							fprintf(stdout, "Canal AfterTouch: 0x%02x\n", data1);
							break;
					}
				#endif
				}
				else if (status < MIDI_SYSEX_MSG) {
					data1=buffer;
					next=MIDI_DATA2;
				#ifndef __AVR__
					fprintf(stdout, "Running status - Status= 0x%02x, DATA1= 0x%02x, DATA2= 0x%02x\n", 
							status, data1, data2);
				#endif
				}
				else {
					status=MIDI_UNKNOW_MSG;
					next=MIDI_UNKNOW_MSG;
				}
				break;

			default:
				#ifndef __AVR__
				fprintf (stdout, "PANIC MIDI!!\n");
				#endif
				status=MIDI_UNKNOW_MSG;
				next=MIDI_UNKNOW_MSG;
				ready=FALSE;
				#if defined (__MIDI_RESET_ON_ERROR__)
				exit (0);
				#endif
				break;
		}
	}
}

