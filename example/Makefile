CC = clang
CFLAGS = -Wall -Wextra -Werror -Wno-unused-function -pedantic -g

default: *
	$(CC) -std=c99 $(CFLAGS) dynamic-array.c -o da
	$(CC) -std=c99 $(CFLAGS) string-view.c -o sv
	$(CC) -std=c99 $(CFLAGS) logging.c -o logging
	$(CC) -std=c99 $(CFLAGS) arena.c -o arena

c90: *
	$(CC) -std=c90 $(CFLAGS) dynamic-array.c -o da
	$(CC) -std=c90 $(CFLAGS) string-view.c -o sv
	$(CC) -std=c90 $(CFLAGS) logging.c -o logging
	$(CC) -std=c90 $(CFLAGS) arena.c -o arena

debug: *
	$(CC) -fsanitize=address -std=c99 $(CFLAGS) dynamic-array.c -o da
	$(CC) -fsanitize=address -std=c99 $(CFLAGS) string-view.c -o sv
	$(CC) -fsanitize=address -std=c99 $(CFLAGS) logging.c -o logging
	$(CC) -fsanitize=address -std=c99 $(CFLAGS) arena.c -o arena

clean: *
	-rm da
	-rm sv
	-rm logging
	-rm arena
	-rm logs.log
