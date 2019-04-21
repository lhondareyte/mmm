PRG    =  mmm
CFLAGS =  -D __MIDI_RESET_ENABLE__
SRCS   =  mm.c midi.c
OBJS   =  mm.o midi.o
PREFIX ?= /usr/local

all: $(PRG)

.c.o:
	@$(CC) $(CFLAGS) -c  $< -o $@

$(PRG): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -s -o $(PRG)

clean:
	@rm -f *.o $(PRG) *~

install:
	mkdir -p $(PREFIX)/bin
	install -m 555 -o root -g wheel $(PRG) $(PREFIX)/bin
