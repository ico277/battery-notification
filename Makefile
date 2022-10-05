PREFIX = /usr/local
CC = gcc
CFILES = $(wildcard src/*.c)
EXECUTABLE = batterry-notification
CFLAGS = -O2 $(shell pkg-config --cflags --libs libnotify)
LDFLAGS = -lnotify

.PHONY: install clean run debug

$(EXECUTABLE).out:
	$(CC) $(CFILES) -o $(EXECUTABLE).out $(LDFLAGS) $(CFLAGS)

install: ./$(EXECUTABLE).out
	cp ./$(EXECUTABLE).out $(PREFIX)/bin/$(EXECUTABLE)

uninstall: $(PREFIX)/bin/$(EXECUTABLE)
	rm $(PREFIX)/bin/$(EXECUTABLE)

clean:
	rm ./$(EXECUTABLE).out 2> /dev/null || true
	rm ./$(EXECUTABLE)_debug.out 2> /dev/null || true
	rm ./vgcore* 2> /dev/null || true

run: ./$(EXECUTABLE).out
	./$(EXECUTABLE).out

debug: clean
	$(CC) $(CFILES) -fsanitize=address -o $(EXECUTABLE)_debug.out $(LDFLAGS) $(CFLAGS) -DDEBUG
	./$(EXECUTABLE)_debug.out

