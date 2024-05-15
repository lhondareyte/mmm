PRG    =  mmm
CFLAGS =  -D __MIDI_RESET_ENABLE__ -Werror
SRCS   =  mm.c midi.c
OBJS   =  mm.o midi.o
BINDIR = $(DESTDIR)$(PREFIX)/bin
all: $(PRG)

.c.o:
	@$(CC) $(CFLAGS) -c  $< -o $@

$(PRG): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -s -o $(PRG)

clean:
	@rm -f *.o $(PRG) *~

install:
	install -s -m 755  $(PRG) $(BINDIR)
