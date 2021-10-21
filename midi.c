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

