/*
 *   Copyright (c)2014-2017,  Luc Hondareyte <lhondareyte_AT_laposte.net>.
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
 *  From Librairie MIDI pour AVR
 *  $Id: midi.h,v 1.7 2011/03/11 10:10:32 luc Exp luc $
 *
 *  (c)2011 - Luc HONDAREYTE <luc.hondareyte_AT_laposte.net>
 */
#include <stdint.h>
 
#ifndef __AVR_MIDI_H__
 #define __AVR_MIDI_H__

#ifndef FALSE
 #define FALSE          0
#endif
#ifndef TRUE
 #define TRUE           1
#endif

#ifndef bit_is_set
 #define bit_is_set(var,pos) ((var) & (1<<(pos)))
#endif

void MIDI_Event(void);


/*
 *  Channel messages
 */

#define MIDI_UNKNOW_MSG		0x00
#define MIDI_NOTOFF_MSG		0x80
#define MIDI_NOTEON_MSG		0x90		
#define MIDI_POLYAF_MSG		0xA0		
#define MIDI_CTRLCH_MSG		0xB0
#define MIDI_MODECH_MSG		0xB0
#define MIDI_PROGCH_MSG		0xC0
#define MIDI_CHANAF_MSG		0xD0
#define MIDI_PITCHB_MSG		0xE0

/*
 *  System common messages
 */

#define MIDI_SYSEX_MSG		0xF0
#define MIDI_QUARTER_MSG	0xF1
#define MIDI_SONGPOS_MSG	0xF2
#define MIDI_SONGSEL_MSG	0xF3

/*
 *  One-byte System common messages
 */

#define MIDI_UNDEF1_MSG		0xF4
#define MIDI_UNDEF2_MSG		0xF5
#define MIDI_TUNERQ_MSG		0xF6
#define MIDI_EOX_MSG		0xF7

/*
 *  System Real Time messages
 */

#define MIDI_TIMECL_MSG		0xF8
#define MIDI_TIMETCK_MSG	0xF9
#define MIDI_START_MSG		0xFA
#define MIDI_CONTINUE_MSG	0xFB
#define MIDI_STOP_MSG		0xFC
#define MIDI_UNDEF3_MSG		0xFD
#define MIDI_ACTIVE_MSG		0xFE
#define MIDI_RESET_MSG		0xFF

#define MIDI_DATA1		1
#define MIDI_DATA2		2


volatile uint8_t buffer; 	// buffer de reception
volatile uint8_t status;
volatile uint8_t channel;	// Canal MIDI du message recu
volatile uint8_t next;		// Prochaine valeur attendue
volatile uint8_t data1;		//
volatile uint8_t data2;
volatile uint8_t ready;		// Message prêt ou non 
uint8_t activechannel;		// Canal MIDI actif

/*
 * SysEx message buffer
 */

#if defined (MIDI_MANUFACTURER_ID)

#endif

/*
 * Pseudo fonctions 
 */
 
#define	MIDI_SetRcvChannel(c)	activechannel=c
void	MIDI_Ack( void );
void	MIDI_Init( void );
uint8_t	MIDI_MsgReady( void );

#endif /* __AVR_MIDI_H__ */
