CC=gcc
CFLAGS = -Wall

test: test_build test_run

test_build:
	@$(CC) $(CFLAGS) test_runner.c -o test_runner

test_run:
	@-./test_runner

clean:
	rm test_runner
