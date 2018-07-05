CC      = gcc
PROF    = -O -ggdb
NOCRYPT =
C_FLAGS =  -Wall $(PROF) $(NOCRYPT)
L_FLAGS =  $(PROF) -lcrypt

SRC_FILES := $(wildcard *.c)
O_FILES := $(patsubst %.c, %.o, $(SRC_FILES))

rom: $(O_FILES)
	rm -f ghost
	$(CC) $(L_FLAGS) -o ghost $(O_FILES)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<

clean:
	rm -f $(O_FILES) rom


