PRG=mm
CFLAGS= -D __MIDI_RESET_ENABLE__
SRCS=mm.c midi.c
OBJS=mm.o midi.o

.c.o:
	@printf "Compiling $< ..." 
	@$(CC) $(CFLAGS) -c  $< -o $@
	@echo "done."
$(PRG): $(OBJS)
	@printf "Linking..."
	@$(CC) $(CFLAGS) $(OBJS) -s -o $(PRG)
	@echo "done."
clean:
	@printf "Cleaning..."
	@rm -f *.o $(PRG) *~
	@echo "done."
install:
	@printf "Installation..."
	@instal -m 755 $(PRG) /usr/local/bin
	@echo "done."
