PREFIX = /usr/local
CC = gcc
CFILES = $(wildcard src/*.c)
EXECUTABLE = batterry-notification
CFLAGS = -O2

.PHONY: install clean run debug

$(EXECUTABLE).out:
	$(CC) $(CFILES) -o $(EXECUTABLE).out $(CFLAGS)

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
	$(CC) $(CFILES) -fsanitize=address -o $(EXECUTABLE)_debug.out $(CFLAGS) -DDEBUG
	./$(EXECUTABLE)_debug.out

