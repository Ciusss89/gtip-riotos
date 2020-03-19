CC=cc
CFLAGS := -O2 -Wall -Wno-parentheses
CPPFLAGS += \
	    -I./include \
	    -D_GNU_SOURCE

DEPS = ihb.h
OBJ = main.o ihblib.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ihbtool: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) ihbtool

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f ihb $(DESTDIR)$(PREFIX)/bin
