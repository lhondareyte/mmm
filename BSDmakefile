PROG	=	mm
SRCS	!=	ls *c
CFLAGS	+=	-D __MIDI_RESET_ENABLE__

DESTDIR	=	/usr/local/bin

MK_MAN	= no
MK_DEBUG_FILES = no

clean:
	@rm -f  *.o $(PROG) *debug *full .depend*

.include <bsd.prog.mk>
