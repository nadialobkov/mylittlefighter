CC = gcc
CFLAGS = -Wall
AFLAGS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

PROG = main
OBJECTS = main.o player.o box.o joystick.o button.o mylittlefighter.o

.PHONY: all clean purge

all: $(PROG)

debug: CFLAGS += -DDEBUG
debug: $(PROG)

boxes: CFLAGS += -DBOXES
boxes: $(PROG)

%.o: %.c %.h
	$(CC) -c $(CFLAGS) -o $@ $<

$(PROG): $(OBJECTS)	
	$(CC) $(CFLAGS) -o $@ $^ $(AFLAGS) 

clean:
	@echo "removendo temporarios"
	@rm -f *.o

purge: clean
	@echo "removendo executavel"
	@rm -f $(PROG)
