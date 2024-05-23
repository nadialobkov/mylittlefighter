CC = gcc
CFLAGS = -Wall
AFLAGS = $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)

PROG = main
OBJECTS = square.o

.PHONY: all clean purge

all: $(PROG)

%.o: %.c %.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(PROG): $(OBJECTS)	
	$(CC) $(CFLAGS) $(AFLAGS) -o $@ $^

clean:
	@echo "removendo temporarios"
	@rm -f *.o

purge: clean
	@echo "removendo executavel"
	@rm -f $(PROG)
